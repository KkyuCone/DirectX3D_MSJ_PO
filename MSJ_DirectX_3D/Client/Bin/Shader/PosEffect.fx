#include "Share.fx"

// ȭ�� ��üũ��
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

// Ǯ ��ũ�� ���带 ������ �ϴ� �Ͱ� �����ϰ� �Ѵ�. ( ���� null�� ����� �����ϱ� �װ� �״�� ��� )
// NULL �Է� ���̾ƿ� �ε����� ���ؽ� ���۸� ���ε��Ѵ�.

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

// �� ���ο� ����ϴ� �ȼ� ���̴��� ���� ���ҽ��� ����Ѵ�.
Texture2D<float4> HDRTexture : register(t5);
StructuredBuffer<float> AvgLum : register(t6);          // ��� �ֵ���

// ����Ʈ ���÷� ����� (�̸� �����س���)

// �Ʒ��� ������۵� �ʿ���
cbuffer FinalPassConstants : register(b11)
{
    float MiddleGrey : packoffset(c0);              // �߰� ȸ����
    float LumWhiteSqr : packoffset(c0.y);           // ��� ��
    float2 vEmpty;
}

static const float3 LUM_FACTOR = float3(0.299, 0.587, 0.114);

// HDR���� ���� �� ���� �Լ� 
float3 ToneMapping(float3 _HDRColor)
{
    // ���� �ȼ��� ���� �ֵ� ������ ���
    // LScale : ������ ������ �ֵ� ��
    float LScale = dot(_HDRColor, LUM_FACTOR);

    // HDR -> LDR�� ��ȯ ����    : �ȼ��� �ֵ����� ���ϴ� �߰� �ֵ� ������ �������� �����Ѵ�.
    LScale *= MiddleGrey / AvgLum[0];
    LScale = (LScale + (LScale * LScale / LumWhiteSqr)) / (1.0 + LScale);

    // �ֵ� �������� �ȼ� ���� ����
    return _HDRColor * LScale;
}

// �ȼ� ���̴� ��Ʈ�� ����Ʈ ǥ��
float4 PS_FinalPass(VS_OUTPUT_HDR _Input) : SV_TARGET
{
    // ���� ���� ���
    // Color�� LDR�� ��ȯ�ϱ� ���� HDR ���� �ֵ� ������ -> ���߿� LDR�� ��ȯ��
    float3 Color = HDRTexture.Sample(g_PointSampler, _Input.UV.xy).xyz;

    // �� ����   
    Color = ToneMapping(Color);

    // LDR �� ��� ( �̰� ȭ�鿡 ǥ���ϸ� �� )
    return float4(Color, 1.0);
}


// ���� - �����
// HDR ���� LDR�� ��ȯ�ؼ� ȭ�鿡 ǥ���ϴ� ������ �� �ʿ�����..
// -> HDR���� ����ȭ�ؼ� �� ���� LDR�� ����� ���
//    : �̷��� �� ��� ���� �����ϰ� �����Ǿ� ���� �ʴ� �̻� ���� �̹����� ũ�� ��� ���� ����.
//      �Ϻ� �ȼ��� ���� ������ �ȼ��� ���� �������� ��� �ȼ� ����ȭ�� ��ο� �κ��� �˰�, ���� �κ��� �Ͼ�Ը� ����
//
// �� ���� ����� ���� ����ȭ�ϴ� �� ��ſ� �� �Ű������� �������� HDR�� �������� ������.
// �߰��� ����ϴ� MiddleGrey(ȸ��)�� LumWhiteSqr(���)�� �� �Ű�������.
// �̶� HDR �ȼ� ������ LDR�� ��ȯ�ϴ� ����!
// -> LScale = MiddleGrey / AvgLum
// -> Color = (LScale + LScale * LScale / LumWhiteSqr) / (1.0 + LScale);

// HDR ����
// 1. ��� �ֵ����� �������� �ֵ��� �ٿ� �������ϴ� �ܰ�
// 2. �� ������ ���� ���� �н� �ܰ�
//
// ��ǻƮ ���̴��� �����ϴ� �������� �ȼ� ���̴��� ����� �ֵ��� �ٿ� �������ϴ� �۾��� �̷���
// -> �����߰� �ܰ踦 ������.
// -> �� �ܰ踶�� �Է� �� ��� 1/4�� �������� �ٿ��� ���������� �ϳ��� �ȼ��� �ٿ�����ϰ� ��
//
// ��ǻƮ ���̴��� ���� �޸𸮸� ������
//
// ��ǻƮ ���̴��� ���ÿ� ���� �����带 ������.
// �̶� ���� �޸𸮴� ���� �׷����� ���� ��� �����尡 ������ ������ �� �ְ� ����Ǿ��ִ� �޸� ��������
// ������ ���̿��� �����͸� �������� �� �ְ� ��.
// �׷��� �ټ��� �ȼ��� �ϳ��� �ȼ��� �ٿ������ �� �� �ְ� ��.
//
// DirectX11�� HDR �ؽ�ó�� �ϳ��� �ֵ� ������ �ٿ� ������ �� �� ���� (���� ����)
// 1. �� ������ �׷��� 1024�� �̻��� �����带 ������ �� ����.
// 2. ���� �޸��� ũ��� 16KB�� ���� ���Ѵ�.
// 3. �� ������� ���� �޸𸮿� 256����Ʈ �̻��� �����͸� ����� �� ����.
//
//
// �׷��� ������� 16�ȼ��� �ٿ�����ϰ�  �׷�� 1024 �����带 ����Ѵ�.
// �̷��� �ص� ��ü HDR �ؽ�ó�� �ϳ��� �ȼ��� �ٿ������ ����;; (�ٽᵵ �Ұ��� �ϴ� �Ҹ��� )
// �׷��� �߰� ���۸� ��ġ�� ������ �̿���.
