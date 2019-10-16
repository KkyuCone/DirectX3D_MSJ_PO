cbuffer RainGenCBuffer : register(b0)
{
    float4x4 matRain;
}

float4 VS_RainGen(float4 Pos : POSITION) : SV_Position
{
    return mul(Pos, matRain);
}