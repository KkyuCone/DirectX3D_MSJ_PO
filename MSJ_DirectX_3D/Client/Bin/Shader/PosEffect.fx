#include "Share.fx"

// 화면 전체크기
static const float2 vNullPos[4] =
{
    float2(-1.f, 1.f),
	float2(1.f, 1.f),
	float2(-1.f, -1.f),
	float2(1.f, -1.f)
};

// UV
static const float2 vNullUV[4] =
{
    float2(0.f, 0.f),
	float2(1.f, 0.f),
	float2(0.f, 1.f),
	float2(1.f, 1.f)
};

// 풀 스크린 쿼드를 렌더링 하는 것과 동일하게 한다. ( 위에 null로 만든거 있으니까 그거 그대로 사용 )
// NULL 입력 에이아웃 인덱스와 버텍스 버퍼를 바인딩한다.

struct VS_OUTPUT_HDR
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
};


VS_OUTPUT_HDR VS_FullScreenQuad(uint VertexID : SV_VertexID)
{
    VS_OUTPUT_HDR output = (VS_OUTPUT_HDR) 0;

    output.Position = float4(vNullPos[VertexID].xy, 0.0, 1.0);
    output.UV = vNullPos[VertexID].xy;

    return output;
}

// 톤 매핑에 사용하는 픽셀 셰이더는 다음 리소스를 사용한다.
Texture2D<float4> HDRTexture : register(t5);
StructuredBuffer<float> AvgLum : register(t6);          // 평균 휘도값

// 포인트 샘플러 사용함 (미리 정의해놨음)

// 아래의 상수버퍼도 필요함
cbuffer FinalPassConstants : register(b11)
{
    float MiddleGrey : packoffset(c0);              // 중간 회색값
    float LumWhiteSqr : packoffset(c0.y);           // 흰색 값
    float2 vEmpty;
}

static const float3 LUM_FACTOR = float3(0.299, 0.587, 0.114);

// HDR값에 대한 톤 매핑 함수 
float3 ToneMapping(float3 _HDRColor)
{
    // 현재 픽셀에 대한 휘도 스케일 계산
    // LScale : 스케일 조정된 휘도 값
    float LScale = dot(_HDRColor, LUM_FACTOR);

    // HDR -> LDR로 변환 공식    : 픽셀의 휘도값을 원하는 중간 휘도 값으로 스케일을 조정한다.
    LScale *= MiddleGrey / AvgLum[0];
    LScale = (LScale + (LScale * LScale / LumWhiteSqr)) / (1.0 + LScale);

    // 휘도 스케일을 픽셀 색상에 적용
    return _HDRColor * LScale;
}

// 픽셀 셰이더 엔트리 포인트 표현
float4 PS_FinalPass(VS_OUTPUT_HDR _Input) : SV_TARGET
{
    // 색상 샘플 계산
    // Color은 LDR로 변환하기 위한 HDR 색상 휘도 스케일 -> 나중엔 LDR로 변환됨
    float3 Color = HDRTexture.Sample(g_PointSampler, _Input.UV.xy).xyz;

    // 톤 매핑   
    Color = ToneMapping(Color);

    // LDR 값 출력 ( 이거 화면에 표시하면 됨 )
    return float4(Color, 1.0);
}


// 설명 - 톤매핑
// HDR 값을 LDR로 변환해서 화면에 표시하는 과정이 왜 필요한지..
// -> HDR값을 정규화해서 그 값을 LDR로 사용할 경우
//    : 이렇게 할 경우 값이 균일하게 분포되어 있지 않는 이상 최종 이미지가 크게 향상 되지 않음.
//      일부 픽셀만 비교적 나머지 픽셀이 비교적 밝은값일 경우 픽셀 정규화시 어두운 부분은 검게, 밝은 부분은 하얗게만 나옴
//
// 톤 매핑 방식을 값을 정규화하는 것 대신에 두 매개변수를 기준으로 HDR의 스케일을 조정함.
// 중간에 사용하는 MiddleGrey(회색)과 LumWhiteSqr(흰색)이 이 매개변수임.
// 이때 HDR 픽셀 색상을 LDR로 변환하는 공식!
// -> LScale = MiddleGrey / AvgLum
// -> Color = (LScale + LScale * LScale / LumWhiteSqr) / (1.0 + LScale);

// HDR 과정
// 1. 평균 휘도값을 기준으로 휘도를 다운 스케일하는 단계
// 2. 톤 매핑을 위한 최종 패스 단계
//
// 컴퓨트 셰이더를 생성하는 과정에서 픽셀 셰이더를 사용해 휘도를 다운 스케일하는 작업이 이뤄짐
// -> 여러중간 단계를 거쳤음.
// -> 각 단계마다 입력 값 대비 1/4로 스케일을 줄여서 최종적으로 하나의 픽셀에 다운스케일하게 됨
//
// 컴퓨트 셰이더는 공유 메모리를 지원함
//
// 컴퓨트 셰이더는 동시에 여러 스레드를 실행함.
// 이때 공유 메모리는 같은 그룹으로 묶인 모든 스레드가 빠르게 접근할 수 있게 설계되어있는 메모리 영역으로
// 스레드 사이에서 데이터를 공유ㅠ할 수 있게 함.
// 그래서 다수의 픽셀을 하나의 픽셀로 다운스케일을 할 수 있게 됨.
//
// DirectX11은 HDR 텍스처를 하나의 휘도 값으로 다운 스케일 할 수 없음 (제약 존재)
// 1. 각 스레드 그룹은 1024개 이상의 스레드를 포함할 수 없다.
// 2. 공유 메모리의 크기는 16KB를 넘지 못한다.
// 3. 각 스레드는 공유 메모리에 256바이트 이상의 데이터를 사용할 수 없다.
//
//
// 그래서 스레드당 16픽셀을 다운스케일하고  그룹당 1024 스레드를 사용한다.
// 이렇게 해도 전체 HDR 텍스처를 하나의 픽셀에 다운스케일을 못함;; (다써도 불가능 하단 소리임 )
// 그래서 중간 버퍼를 거치는 과정을 이용함.
