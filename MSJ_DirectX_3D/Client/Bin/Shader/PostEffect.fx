#include "Share.fx"

// �� ���ο� ����ϴ� �ȼ� ���̴��� ���� ���ҽ��� ��� -> HDR, ��� �ֵ���
Texture2D<float4> HDRTexture : register(t5);            // HDR ������ �ؽ�ó
StructuredBuffer<float> AvgLum : register(t6);          // ��� �ֵ���
Texture2D<float4> BloomTexture : register(t8);          // ��� ������ �ؽ�ó �����, ��� ���� ���÷� ���
Texture2D<float4> DOFBlurTexture : register(t9);        // DOF (Depth Of Filed)�� �ؽ�ó
Texture2D<float4> DepthTexture : register(t10);         // ���� �ؽ�ó

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
    output.UV = vNullUV[VertexID].xy;

    return output;
}

// �Ʒ��� ������۵� �ʿ���
cbuffer FinalPassConstants : register(b12)
{
    float MiddleGrey;             // �߰� ȸ����
    float LumWhiteSqr;            // ��� ��
    float BloomScale;             // ��� ũ��
    float vEmpty;
    float2 ProjValue;             // ���� ��
    float2 DOFFarValue;           // DOF
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


// DOF - Z���� ���� ���� ���� ������ ��ȯ�ϴ� �Լ�
float ConvertZBufferToLinearDepth(float _Depth)
{
    float LinearDepth = ProjValue.x / (_Depth + ProjValue.y);
    return LinearDepth;
}

// DOF - HDR ���� ���� �Ÿ� DOF�� �����ϴ� �Լ�
float3 DistanceDOF(float3 _ColorFocus, float3 _ColorBlurred, float _Depth)
{
    // ���� ��� �� �� ���
    float BlurFactor = saturate((_Depth - DOFFarValue.x) * DOFFarValue.y);

    // �� ���� ����
    return lerp(_ColorFocus, _ColorBlurred, BlurFactor);

}


// �ȼ� ���̴� ��Ʈ�� ����Ʈ ǥ��
float4 PS_FinalPass(VS_OUTPUT_HDR _Input) : SV_TARGET
{
    // ���� ���� ���
    // Color�� LDR�� ��ȯ�ϱ� ���� HDR ���� �ֵ� ������ -> ���߿� LDR�� ��ȯ��
    float4 Color = HDRTexture.Sample(g_PointSampler, _Input.UV.xy);

    if (Color.x == 0.0 && Color.y == 0.0 && Color.z == 0.0)
    {
        clip(-1);
    }

    // �� ��鿡 ���� �ȼ��� ���ؼ� �Ÿ� DOF ���
    float2 Depth = DepthTexture.Sample(g_PointSampler, _Input.UV.xy).zw;

    if (Depth.x < 1.0)
    {
        // �ٿ� ������ HDR �ؽ�ó�� ���� �� ���� ���
        float3 ColorBlurred = DOFBlurTexture.Sample(g_LinearSampler, _Input.UV.xy).xyz;
 
        // ��ü �ػ� ���� ���� ���� ���̷� ��ȯ
        //Depth = ConvertZBufferToLinearDepth(Depth);

        // �Ÿ� DOF ���� ���
        Color.xyz = DistanceDOF(Color.xyz, ColorBlurred, Depth.y);

    }

    Color += BloomScale * BloomTexture.Sample(g_LinearSampler, _Input.UV.xy);           // ��� ����

    // �� ����   
    Color.xyz = ToneMapping(Color.xyz);

    // LDR �� ��� ( �̰� ȭ�鿡 ǥ���ϸ� �� )
    return float4(Color.xyz, 1.0);
}
