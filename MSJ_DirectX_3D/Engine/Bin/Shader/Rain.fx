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
    float4x4    ViewProj;       // ���յ� �� ���� ���
    float3      ViewDir;        // ����ȭ�� �� ����
    float       Scale;          // ����� �޽��� ũ��
    float4      AmbientColor;   // ����� Ambient Color
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

// ���� ����
struct CSBuffer
{
    float3 vPosition;
    float3 vVel;
    float fState;
};

// ���ؽ� ���̴����� ����� ���� �ùķ��̼� �� �����Ͱ�
StructuredBuffer<CSBuffer> RainData : register(t5);

// �ȼ� ���̴����� ����� ����¥��.. 
Texture2D RainStreakTex : register(t6);

VS_OUTPUT_RAIN VS_Rain(uint VertexID : SV_VERTEXID)
{
    VS_OUTPUT_RAIN output = (VS_OUTPUT_RAIN) 0;

	// ���� ����� ���� ��������
    CSBuffer curDrop = RainData[VertexID / 6];

	// ���� ����� ��ġ
    float3 pos = curDrop.vPosition;

	// ����� Ȯ�� ���� ���
    float3 rainDir = normalize(curDrop.vVel);
    float3 rainRight = normalize(cross(ViewDir, rainDir));

	// ���� �𼭸����� ����� ��ġ Ȯ��
    float2 offsets = arrBasePos[VertexID % 6];
    pos += rainRight * offsets.x * Scale * 0.025;
    pos += rainDir * offsets.y * Scale;

	// �� �𼭸��� ���� �������� ��ȯ
    output.vPos = mul(float4(pos, 1.0), ViewProj);

	// UV��ǥ ����
    output.vUV = arrUV[VertexID % 6];
	
	// ���鿡 �浹�� ��ƼŬ Ŭ����
    output.fClip = curDrop.fState;

    return output;
}

// �ȼ� ���̴����� RainStreakTex �ؽ�ó�� �����÷��� �̿��ؼ� ����� �ؽ�ó���� ���ø��� ��
float4 PS_Rain(VS_OUTPUT_RAIN _Input) : SV_Target
{
    float fTexAlpha = RainStreakTex.Sample(g_LinearSampler, _Input.vUV).r;
    return float4(AmbientColor.rgb, AmbientColor.a * fTexAlpha);
}