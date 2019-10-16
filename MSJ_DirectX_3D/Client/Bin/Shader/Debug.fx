
// ���ؽ�
struct VS_INPUT_UV
{
    float3 vPos : POSITION;
    float2 vUV  : TEXCOORD;
};

struct VS_OUTPUT_UV
{
    float4 vPos : SV_POSITION;
    float2 vUV  : TEXCOORD;
};

// �ȼ�
struct PS_OUTPUT
{
    float4 vColor : SV_TARGET;
};

// �������
cbuffer Debug : register(b9)
{
    matrix g_DebugWVPMatrix;
}

///////////////////////////////////////////////////

Texture2D g_DiffuseTexture : register(t0);

SamplerState g_LinearSampler : register(s0); // ���� ���÷�
SamplerState g_PointSampler : register(s1);

VS_OUTPUT_UV VS_Debug(VS_INPUT_UV _Input)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV) 0;

    output.vPos = mul(float4(_Input.vPos, 1.0f), g_DebugWVPMatrix);     // ������� ���ϱ�
    output.vUV = _Input.vUV;

    return output;
}

PS_OUTPUT PS_Debug(VS_OUTPUT_UV _Input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;

    float4 vColor = g_DiffuseTexture.Sample(g_LinearSampler, _Input.vUV);

    // ���İ� 0 �ξֵ��� ����� �ʿ䰡 ������ ��� �׳� clipó����
    if(0.0f == vColor.a)
    {
        clip(-1);
    }

    output.vColor = vColor;

    //output.vColor = float4(1.0f, 0.0f, 0.0f, 1.0f);

    return output;
}