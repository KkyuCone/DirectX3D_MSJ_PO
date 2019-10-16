#include "Share.fx"

// Res      : 백 버퍼의 높이와 너비를 4로 나눈 값
// Domain   : 백 버퍼의 높이와 너비를 곱한 후 16으로 나눈 값
// GroupSize : 백 버퍼의 높이와 너비를 곱한 후 16으로 나눈 다음 1024를 곱한 값 
cbuffer DownScaleConstants : register(b11)
{
    uint2 Res;              // 다운 스케일 해상도 계산 : x - 너비, y - 높이   : 백버퍼의 높이와 너비를 4로 나눈 값
    uint Domain;            // 다운 스케일 이미지의 총 픽셀 수                : 백퍼의 높이와 너비를 곱한 후 16으로 나눈값 즉 Res의 총 픽셀수
    uint GroupSize;         // 첫 패스에 적용된 그룹 수 계산                  : 백버퍼의 높이와 너비를 곱한 후 16으로 나눈 다음 1024를 곱한 값
    float Adaptation;       // 적응 값
    float fBloomThreshold;  // 블룸 임계값 비율
    float2 Empty;
}


Texture2D HDRTex : register(t5);
StructuredBuffer<float> AverageValuesID : register(t6);
StructuredBuffer<float> PrevAvgLum : register(t7); // 적응 - 이전..직전프레임의 평균 휘도값 저장용

RWStructuredBuffer<float> AverageLum : register(u0);            // 순서없는애로 연결
RWTexture2D<float4> HDRDownScale : register(u3);                // 저장하는애 블룸


groupshared float SharedPositions[1024]; // 공유메모리

static const float4 LUM_FACTOR = float4(0.299, 0.587, 0.114, 0); //  휘도 계산을 위해 상수값 정해놨음

// 계산 셰이더 코드

// 첫번째 헬퍼 함수 : 각 스레드에 대해 4x4 다운 스케일 수행
float DownScale4x4(uint2 _CurPixel, uint _groupThreadID)
{
    float avgLum = 0.0; // 평균 휘도값

    // 픽셀 결합 생략
    if (_CurPixel.y < Res.y)
    {
        float fCount = 0.0f;
        // 4x4 픽셀 그룹 값 합산
        int3 iFullResPos = int3(_CurPixel * 4, 0);
        float4 downScaled = float4(0.0, 0.0, 0.0, 0.0);

		[unroll]
        for (int i = 0; i < 4; i++)
        {
			[unroll]
            for (int j = 0; j < 4; j++)
            {
                downScaled += HDRTex.Load(iFullResPos, int2(j, i));
            }
        }


        fCount = downScaled.a;
        //downScaled /= 16.0;        
        downScaled.rgb /= (fCount + 0.01f); // 평균
        downScaled.a = 1.0f;
        HDRDownScale[_CurPixel.xy] = downScaled; // 1/4 이미지 해상도 저장
        avgLum = dot(downScaled, LUM_FACTOR);           // 픽셀별 휘도 값 계산
        //HDRDownScale[_CurPixel.xy] = avgLum; // 1/4 이미지 해상도 저장
        // 공유 메모리에 결과 기록
        SharedPositions[_groupThreadID] = avgLum;

    }

    // 동기화 후 다음 단계
    // GroupMemoryBarrierWithGroupSync : 모든 그룹 공유 액세스가 완료되고 그룹의 모든 스레드가이 호출에 도달 할 때까지 그룹의 모든 스레드 실행을 차단
    GroupMemoryBarrierWithGroupSync();

    return avgLum;
}


// 두번째 헬퍼 함수
// 아래 함수는 위에서 구한 값을 4개의 값으로 다운 스케일한다.
float DownScale1024to4(uint _dispatchThreadID, uint _groupThreadID, float _avgLum)
{
    // 다운 스케일 코드를 확장
    [unroll]
    for (uint groupSize = 4, step1 = 1, step2 = 2, step3 = 3; groupSize < 1024; groupSize *= 4, step1 *= 4, step2 *= 4, step3 *= 4)
    {
        // 픽셀 결합 계산 생략
        if (_groupThreadID % groupSize == 0)
        {
            // 휘도값 계산
            float stepAvgLum = _avgLum;
            stepAvgLum += (_dispatchThreadID + step1) < Domain ? SharedPositions[_groupThreadID + step1] : _avgLum;
            stepAvgLum += (_dispatchThreadID + step2) < Domain ? SharedPositions[_groupThreadID + step2] : _avgLum;
            stepAvgLum += (_dispatchThreadID + step3) < Domain ? SharedPositions[_groupThreadID + step3] : _avgLum;


            // 결과 값 저장
            _avgLum = stepAvgLum;
            SharedPositions[_groupThreadID] = stepAvgLum;
        }

        // 동기화 후 다음 단계
        GroupMemoryBarrierWithGroupSync();
    }

    return _avgLum;
}

// 세번째 헬퍼 함수
// 4개의 값을 하나의 평균값으로 다운스케일 한  후 저장한다.
void DownScale4to1(uint _dispatchTreadID, uint _groupThreadID, uint _groupID, float _avgLum)
{
    if (_groupThreadID == 0)
    {
        // 스레드 그룹에 대한 평균 휘도 값 계산
        float fFinalAvgLum = _avgLum;

        fFinalAvgLum += (_dispatchTreadID + 256) < Domain ? SharedPositions[_groupThreadID + 256] : _avgLum;
        fFinalAvgLum += (_dispatchTreadID + 512) < Domain ? SharedPositions[_groupThreadID + 512] : _avgLum;
        fFinalAvgLum += (_dispatchTreadID + 768) < Domain ? SharedPositions[_groupThreadID + 768] : _avgLum;

        fFinalAvgLum /= 1024.0;

        // 최종 값을 1D UAV에 기록 후 다음 과정으로
        AverageLum[_groupID] = fFinalAvgLum;
    }
}

// 위에서 구한 값은 셰이더 엔트리 포인트에 대입된다.  ( 엔트리 포인트 : 제어가 운영체제에서 컴퓨터 프로그램으로 이동하는 것 )
// 첫번째 계산 셰이더
[numthreads(1024, 1, 1)]
void DownScaleFirstPass(uint3 _dispatchThreadId : SV_DispatchThreadID,
                        uint3 _groupThreadId : SV_GroupThreadID,
                        uint3 _groupId : SV_GroupID)
{
    uint2 CurPixel = uint2(_dispatchThreadId.x % Res.x, _dispatchThreadId.x / Res.x);

    // 16픽셀 그룹을 하나의 픽셀로 줄여 공유 메모리에 저장
    float avgLum = DownScale4x4(CurPixel, _groupThreadId.x);

    // 1024에서 4로 다운 스케일함
    avgLum = DownScale1024to4(_dispatchThreadId.x, _groupThreadId.x, avgLum);

    // 4에서 1로 다운스케일
    DownScale4to1(_dispatchThreadId.x, _groupThreadId.x, _groupId.x, avgLum);

}


// 두번째 계산셰이더   -> 첫번째 계산셰이더에서 넘어온 중간 결과를 다시 다운 스케일해서 최종 평균 휘도 값을 구함
// 첫번째 계산셰이더의 실행이 완ㄹ 되면 동일한 상수 버퍼를 사용한
// 두번째 계산셰이더를 실행하되 중간 값 휘도 SRV와 평균 휘도 UAV값을 지정해 사용한다.
#define MAX_GROUP 64

// 공유 메모리 그룹에 중간 값 저장
groupshared float SharedAvgFinal[MAX_GROUP];

[numthreads(MAX_GROUP, 1, 1)]
void DownScaleSecondPass(uint3 _groupID : SV_GroupID,
                        uint3 _groupThreadID : SV_GroupThreadID,
                        uint3 _dispatchThreadID : SV_DispatchThreadID,
                        uint _groupIndex : SV_GroupIndex)
{
    // 공유 메모리에 ID 값 저장
    float avgLum = 0.0f;

    if (_dispatchThreadID.x < GroupSize)
    {
        avgLum = AverageValuesID[_dispatchThreadID.x];
    }

    SharedAvgFinal[_dispatchThreadID.x] = avgLum;

    // 동기화 후 다음 단계
    GroupMemoryBarrierWithGroupSync();

    // 64에서 16으로 다운 스케일
    if (_dispatchThreadID.x % 4 == 0)
    {
        // 휘도값 계산
        float stepAvgLum = avgLum;
        stepAvgLum += (_dispatchThreadID.x + 1) < GroupSize ? SharedAvgFinal[_dispatchThreadID.x + 1] : avgLum;
        stepAvgLum += (_dispatchThreadID.x + 2) < GroupSize ? SharedAvgFinal[_dispatchThreadID.x + 2] : avgLum;
        stepAvgLum += (_dispatchThreadID.x + 3) < GroupSize ? SharedAvgFinal[_dispatchThreadID.x + 3] : avgLum;


        // 결과 값 저장 ( 공유메모리에.. )
        avgLum = stepAvgLum;
        SharedAvgFinal[_dispatchThreadID.x] = stepAvgLum;

    }

    // 동기화 후 다음 단계
    GroupMemoryBarrierWithGroupSync();

    // 16에서 4로 다운 스케일
    if (_dispatchThreadID.x % 16 == 0)
    {
        // 휘도값 계산
        float stepAvgLum = avgLum;
        stepAvgLum += (_dispatchThreadID.x + 4) < GroupSize ? SharedAvgFinal[_dispatchThreadID.x + 4] : avgLum;
        stepAvgLum += (_dispatchThreadID.x + 8) < GroupSize ? SharedAvgFinal[_dispatchThreadID.x + 8] : avgLum;
        stepAvgLum += (_dispatchThreadID.x + 12) < GroupSize ? SharedAvgFinal[_dispatchThreadID.x + 12] : avgLum;


        // 결과 값 저장 ( 공유메모리에.. )
        avgLum = stepAvgLum;
        SharedAvgFinal[_dispatchThreadID.x] = stepAvgLum;

    }

    // 동기화 후 다음 단계
    GroupMemoryBarrierWithGroupSync();


    // 4에서 1로 다운 스케일 ( 얘가 마지막)
    if (_dispatchThreadID.x == 0)
    {
        // 휘도값 계산
        float fFinalstepAvgLum = avgLum;
        fFinalstepAvgLum += (_dispatchThreadID.x + 16) < GroupSize ? SharedAvgFinal[_dispatchThreadID.x + 16] : avgLum;
        fFinalstepAvgLum += (_dispatchThreadID.x + 32) < GroupSize ? SharedAvgFinal[_dispatchThreadID.x + 32] : avgLum;
        fFinalstepAvgLum += (_dispatchThreadID.x + 48) < GroupSize ? SharedAvgFinal[_dispatchThreadID.x + 48] : avgLum;

        fFinalstepAvgLum /= 64.0f;

        // 적응 휘도 계산
        // lerp(x,y,s) : 선형보간인 x + s(y - x) 를 리턴
        //if (0.0 == PrevAvgLum[0])
        //    PrevAvgLum = 1.0f;

        float fAdaptedAverageLum = lerp(PrevAvgLum[0], fFinalstepAvgLum, Adaptation);

        AverageLum[0] = max(fAdaptedAverageLum, 0.0001); // 평균 휘도값을 구했음 (최종값)
        //PrevAvgLum[0] = AverageLum[0];
        
    }

}

// 여기까지 하면 평균 휘도값을 구했고, 이제 톤 매핑할 차례
// LDR 값을 출력하는 풀 스크린을 렌더링할것임 ㅇㅅㅇ -> 렌더타겟 사용




// 블룸
Texture2D<float4> HDRDownScaleTex : register(t5);
StructuredBuffer<float> AvgLum : register(t6);
RWTexture2D<float4> Bloom : register(u0);

[numthreads(1024, 1, 1)]
void BrightPass(uint3 _DispatchThreadID : SV_DispatchThreadID)
{
    // 다운스케일된 HDR과 SRV의 평균 휘도를 계산 후 임시로 UAV에 저장
    // 그 후에 첫 패스에서 휘도 다운스케일에 쓰인 그룹 숫자만큼 컴퓨트 셰이더를 적용한다.
    uint2 CurPixel = uint2(_DispatchThreadID.x % Res.x, _DispatchThreadID.x / Res.x);

    // 픽셀 결합 생략
    if (CurPixel.y < Res.y)
    {
        float4 Color = HDRDownScaleTex.Load(int3(CurPixel, 0));
        float Lum = dot(Color, LUM_FACTOR);
        float avgLum = AvgLum[0];

        // 색상 스케일 계산
        float ColorScale = saturate(Lum - avgLum * fBloomThreshold);

        // 블룸 스케일 조정 후 저장
        Bloom[CurPixel.xy] = Color * ColorScale;

    }

}