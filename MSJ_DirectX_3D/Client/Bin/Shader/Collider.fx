#include "Share.fx"

// 색상값 가져올 상수버퍼
cbuffer Collider : register(b10)
{
    float4 g_vColliderColor;
}

VS_OUTPUT_POSITION VS_Collider(VS_INPUT_POSITION _Input)
{
    VS_OUTPUT_POSITION output = (VS_OUTPUT_POSITION) 0;

    output.vPos = mul(float4(_Input.vPos, 1.0f), g_WVP);
    
    return output;
}

PS_OUTPUT PS_Collider(VS_OUTPUT_POSITION _Input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;

    output.vColor = g_vColliderColor;

    //output.vColor = float4(1.0f, 0.0f, 0.0f, 1.0f);

    return output;
}