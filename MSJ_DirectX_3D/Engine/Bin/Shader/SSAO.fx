
cbuffer DownScaleCBuffer : register(b0)
{
    uint2 Res;              // 화면 백버퍼의 1/4해상도 값
    float2 ResRcp;          // Res의 역수값
    float4 vProjParams;     // 투영 매개변수, 디퍼드 라이트 계싼할때 사용한거랑 동일
    matrix matView;         // 카메라 뷰 행렬
    float fOffsetRadius;    // 랜덤 위치를 찾기 위한 반지름 범위
    float fRadius;          // 앰비언트 반구의 반지름 값
    float fMaxDepth;
    int iEmpty;
}

Texture2D<float4> DepthTex : register(t0);
Texture2D<float4> NormalTex : register(t1);
RWStructuredBuffer<float4> MiniDepthRW : register(u0);

// 깊이버퍼를 선형 깊이 값으로 변환하는 함수
float ConvertDepthToLinearDepth(float _Depth)
{
    float LinearDepth = vProjParams.z / (_Depth + vProjParams.w);
    return LinearDepth;
}


// 다운스케일 하는 함수 1/4로..
[numthreads(1024, 1, 1)]
void DownScale(uint3 _DispatchTreadID : SV_DispatchThreadID)
{
    uint3 CurPixel = uint3(_DispatchTreadID.x % Res.x, _DispatchTreadID.x / Res.x, 0);

    // 픽셀 결합 생략
    if(CurPixel.y < Res.y)
    {
        float fminDepth = 1.0f;
        float3 AvgNormal = float3(0.0f, 0.0f, 0.0f);
        uint3 FullResPixel = CurPixel * 2;

        [unroll]
        for (int i = 0; i < 2; ++i)
        {
            [unroll]
            for (int j = 0; j < 2; ++j)
            {
                // 픽셀 깊이 값 계산 후 최솟값 저장
                float CurDepth = DepthTex.Load(FullResPixel, int2(j, i)).x;
                fminDepth = min(CurDepth, fminDepth);

                // 뷰 공간 노말을 합산 후 평균값 계산
                float3 NormalWorld = NormalTex.Load(FullResPixel, int2(j, i)).xyz;
                NormalWorld = normalize(NormalWorld * 2.0f - 1.0f);
                AvgNormal += mul(NormalWorld, (float3x3) matView);
            }
        }

        MiniDepthRW[_DispatchTreadID.x].x = ConvertDepthToLinearDepth(fminDepth);
        float3 avgNormalView = mul(AvgNormal * 0.25f, (float3x3)matView);
        MiniDepthRW[_DispatchTreadID.x].yzw = avgNormalView;
    }
}

// 이제 총 픽셀 수를 1024로 나눈 만큼의 X 스레드 그룹은 다운스케일한 컴퓨트 셰이더에 적용한다.
// 깊이 및 노멀 값을 다운 스케일 버퍼에 저장한 후엔 AO값을 계산해야한다.
// 1/4 크기 깊이 노멀 SRV를 입력 값으로 설정하고 AO, UAV값을 출력값으로 설정한다.

StructuredBuffer<float4> MinDepth : register(t0);
RWTexture2D<float> AO : register(u0);
RWStructuredBuffer<float> SharedDepth : register(u1);

//groupshared float SharedDepth[1024];

static const float NumSamplesRcp = 1.0 / 8.0;
static const uint NumSamples = 8;
static const float2 SampleOffsets[NumSamples] =
{
    float2(0.2803166, 0.08997212),
	float2(-0.5130632, 0.6877457),
	float2(0.425495, 0.8665376),
	float2(0.8732584, 0.3858971),
	float2(0.0498111, -0.6287371),
	float2(-0.9674183, 0.1236534),
	float2(-0.3788098, -0.09177673),
	float2(0.6985874, -0.5610316),
};

float GetDepth(int2 _Pos)
{
    // clamp(x, min, max) : x가  min값 보다 적을 경우 min값을, max보다 클경우 max값으로 반환
    // 해당 픽셀 위치 가져오기
    float x = clamp(_Pos.x, 0, Res.x - 1);
    float y = clamp(_Pos.y, 0, Res.y - 1);

    // 깊이값들을 보고 최소깊이 인덱스위치를 찾는다.
    int minDepthIdx = x + y * Res.x;
    return MinDepth[minDepthIdx].x;
}

float3 GetNormal(int2 _Pos)
{
    float x = clamp(_Pos.x, 0, Res.x - 1);
    float y = clamp(_Pos.y, 0, Res.y - 1);

    int miniDepthIdx = x + y * Res.x;
    return MinDepth[miniDepthIdx].yzw;
}

// SSAO 계산하는 함수
float ComputeAO(int2 _CenterPixelPos, float2 _CenterClipPos)
{
    // 노멀 계산에 필요한 깊이 값 계산
    float fCenterDepth = GetDepth(_CenterPixelPos.xy);

    // 중심 픽셀의 뷰 공간 위치 구하기
    float3 CenterPos;
    CenterPos.xy = _CenterClipPos * vProjParams.xy * fCenterDepth;
    CenterPos.z = fCenterDepth;

    // 중심 픽셀의 뷰 공간 노말값 구하기
    float3 CenterNormal = GetNormal(_CenterPixelPos.xy);
    CenterNormal = normalize(CenterNormal);

    // 랜덤 샘플링 오프셋
    float RotationAngle = dot(float2(_CenterClipPos), float2(73.0, 197.0));
    float2 RandSinCos;
    // sincos(X, s, c) sin(X)와 cos(X)를 동시에 s, c로 리턴.
    sincos(RotationAngle, RandSinCos.x, RandSinCos.y);
    float2x2 RandRotMatrix = float2x2(RandSinCos.y, -RandSinCos.x,
                                        RandSinCos.x, RandSinCos.y);

    // 각 샘플에 대해 앰비언트 오클루션 값 계산
    float AO = 0.0f;
    
    [unroll]
    for (uint i = 0; i < NumSamples; ++i)
    {
        // 텍스처 공간 위치 및 깊이 계산
        float2 SampleOff = fOffsetRadius.xx * mul(SampleOffsets[i], RandRotMatrix);
        float CurDepth = GetDepth(_CenterPixelPos + int2(SampleOff.x, -SampleOff.y));

        // 뷰 공간 위치 값 계산
        float3 CurPos;
        CurPos.xy = (_CenterClipPos + 2.0f * SampleOff * ResRcp) * vProjParams.xy * CurDepth;
        CurPos.z = CurDepth;

        float3 CenterToCurPos = CurPos - CenterPos;
        float LenCenterToCurPos = length(CenterToCurPos);
        float AngleFactor = 1.0f - dot(CenterToCurPos / LenCenterToCurPos, CenterNormal);
        float DistFactor = LenCenterToCurPos / fRadius;

        // 샘플 AO 인수 합산
        AO += saturate(max(DistFactor, AngleFactor));
    }

    return AO * NumSamplesRcp;
}


// SSAO 컴퓨트셰이더
[numthreads(1024, 1, 1)]
void ComputeSSAO(uint3 _GroupThreadID : SV_GroupThreadID, 
                uint3 _DispathThreadID : SV_DispatchThreadID)
{
    uint2 CurPixel = uint2(_DispathThreadID.x % Res.x, _DispathThreadID.x / Res.x);
    SharedDepth[_GroupThreadID.x] = MinDepth[_DispathThreadID.x].x;

    GroupMemoryBarrierWithGroupSync();

    // 픽셀 결합 생략
    if (CurPixel.y < Res.y)
    {
        // 현재 픽셀에 대한 XY 클립 공간 위치 계산
        // Y는 역수 값으로 표시됨
        float2 CenterClipPos = 2.0f * float2(CurPixel) * ResRcp;
        CenterClipPos = float2(CenterClipPos.x - 1.0f, 1.0f - CenterClipPos.y);
        AO[CurPixel] = ComputeAO(CurPixel, CenterClipPos);

    }

}