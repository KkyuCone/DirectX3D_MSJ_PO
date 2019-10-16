
// 버텍스
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

// 픽셀
struct PS_OUTPUT
{
    float4 vColor : SV_TARGET;
};

// 상수버퍼
cbuffer Debug : register(b9)
{
    matrix g_DebugWVPMatrix;
}

///////////////////////////////////////////////////

Texture2D g_DiffuseTexture : register(t0);

SamplerState g_LinearSampler : register(s0); // 선형 샘플러
SamplerState g_PointSampler : register(s1);

VS_OUTPUT_UV VS_Debug(VS_INPUT_UV _Input)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV) 0;

    output.vPos = mul(float4(_Input.vPos, 1.0f), g_DebugWVPMatrix);     // 월드행렬 곱하기
    output.vUV = _Input.vUV;

    return output;
}

PS_OUTPUT PS_Debug(VS_OUTPUT_UV _Input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;

    float4 vColor = g_DiffuseTexture.Sample(g_LinearSampler, _Input.vUV);

    // 알파가 0 인애들은 출력할 필요가 ㅇㅅㅇ 없어서 그냥 clip처리함
    if(0.0f == vColor.a)
    {
        clip(-1);
    }

    output.vColor = vColor;

    //output.vColor = float4(1.0f, 0.0f, 0.0f, 1.0f);

    return output;
}