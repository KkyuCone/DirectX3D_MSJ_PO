#include "Share.fx"

struct VS_OUTPUT_RAIN
{
    float4  vPos : SV_Position;
    float   fClip : SV_ClipDistance;
    float2  vUV : TEXCOORD;
};

//struct PS_RainIn
//{
//    float2 tex : TEXCOORD;
//};

cbuffer RainConstants : register(b6)
{
    float4x4    ViewProj;       // 통합된 뷰 투영 행렬
    float3      ViewDir;        // 정규화된 뷰 방향
    float       Scale;          // 빗방울 메쉬의 크기
    float4      AmbientColor;   // 빗방울 Ambient Color
};

static const float2 arrBasePos[6] =
{
    float2(1.0, -1.0),
 	float2(1.0, 0.0),
 	float2(-1.0, -1.0),

 	float2(-1.0, -1.0),
	float2(1.0, 0.0),
	float2(-1.0, 0.0),
};
    
static const float2 arrUV[6] =
{
    float2(1.0, 0.0),
	float2(1.0, 1.0),
	float2(0.0, 0.0),

	float2(0.0, 0.0),
	float2(1.0, 1.0),
	float2(0.0, 1.0),
};

// 강우 버퍼
struct CSBuffer
{
    float3 vPosition;
    float3 vVel;
    float fState;
};

// 버텍스 셰이더에서 사용할 강우 시뮬레이션 된 데이터값
StructuredBuffer<CSBuffer> RainData : register(t5);

// 픽셀 셰이더에서 사용할 한줄짜리.. 
Texture2D RainStreakTex : register(t6);

VS_OUTPUT_RAIN VS_Rain(uint VertexID : SV_VERTEXID)
{
    VS_OUTPUT_RAIN output = (VS_OUTPUT_RAIN) 0;

	// 현재 빗방울 정보 가져오기
    CSBuffer curDrop = RainData[VertexID / 6];

	// 현재 빗방울 위치
    float3 pos = curDrop.vPosition;

	// 빗방울 확장 방향 계산
    float3 rainDir = normalize(curDrop.vVel);
    float3 rainRight = normalize(cross(ViewDir, rainDir));

	// 영역 모서리까지 빗방울 위치 확장
    float2 offsets = arrBasePos[VertexID % 6];
    pos += rainRight * offsets.x * Scale * 0.025;
    pos += rainDir * offsets.y * Scale;

	// 각 모서리를 투영 공간으로 변환
    output.vPos = mul(float4(pos, 1.0), ViewProj);

	// UV좌표 복사
    output.vUV = arrUV[VertexID % 6];
	
	// 지면에 충돌한 파티클 클리핑
    output.fClip = curDrop.fState;

    return output;
}

// 픽셀 셰이더에선 RainStreakTex 텍스처와 ㅅ매플러를 이용해서 빗방울 텍스처에서 샘플링을 함
float4 PS_Rain(VS_OUTPUT_RAIN _Input) : SV_Target
{
    float fTexAlpha = RainStreakTex.Sample(g_LinearSampler, _Input.vUV).r;
    return float4(AmbientColor.rgb, AmbientColor.a * fTexAlpha);
}