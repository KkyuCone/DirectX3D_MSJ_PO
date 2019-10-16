// 2패스
Texture2D<float4> Input : register(t0); 
RWTexture2D<float4> BlurOP : register(u0);

Texture2D<float4> InputBlur : register(t1);
RWTexture2D<float4> BloomOutput : register(u1);

cbuffer DownScaleConstants : register(b11)
{
    uint2 Res; // 다운 스케일 해상도 계산 : x - 너비, y - 높이
    uint Domain; // 다운 스케일 이미지의 총 픽셀 수
    uint GroupSize; // 첫 패스에 적용된 그룹 수 계산
    float Adaptation; // 적응 값
    float fBloomThreshold; // 블룸 임계값 비율
    float2 Empty;
}

// 가우시안 샘플링 가중치를 포함한 상수 배열
static const float SampleWeights[13] =
{
    0.002216,
    0.008764,
    0.026995,
    0.064759,
    0.120985,
    0.176033,
    0.199471,
    0.176033,
    0.120985,
    0.064759,
    0.026995,
    0.008764,
    0.002216,
};

#define kernelhalf 6

[numthreads(1024, 1, 1)]
void BlurVerticalFilter(uint3 _DispatchThreadID : SV_DispatchThreadID)
{
    // 다운스케일된 HDR과 SRV의 평균 휘도를 계산 후 임시로 UAV에 저장
    // 그 후에 첫 패스에서 휘도 다운스케일에 쓰인 그룹 숫자만큼 컴퓨트 셰이더를 적용한다.
    uint2 CurPixel = uint2(_DispatchThreadID.x % Res.x, _DispatchThreadID.x / Res.x);

    // 수직
    if (CurPixel.y < Res.y)
    {
        float4 vColor = float4(0.0, 0.0, 0.0, 0.0);

        for (int i = -kernelhalf; i <= kernelhalf; ++i)
        {
            uint2 CurVerticlPixel = CurPixel;
            CurVerticlPixel.y += i;
            vColor += Input.Load(int3(CurVerticlPixel, 0)) * SampleWeights[i + kernelhalf];
        }

        BlurOP[CurPixel] = float4(vColor.rgb, 1.0f);

    }
    GroupMemoryBarrierWithGroupSync();
}


[numthreads(1024, 1, 1)]
void BlurHorizonFilter(uint3 _DispatchThreadID : SV_DispatchThreadID)
{
    // 다운스케일된 HDR과 SRV의 평균 휘도를 계산 후 임시로 UAV에 저장
    // 그 후에 첫 패스에서 휘도 다운스케일에 쓰인 그룹 숫자만큼 컴퓨트 셰이더를 적용한다.
    uint2 CurPixel = uint2(_DispatchThreadID.x % Res.x, _DispatchThreadID.x / Res.x);

    // 수평
    if (CurPixel.y < Res.y)
    {
        float4 vColor = float4(0.0, 0.0, 0.0, 0.0);

        for (int i = -kernelhalf; i <= kernelhalf; ++i)
        {
            uint2 CurVerticlPixel = CurPixel;
            CurVerticlPixel.x += i;
            vColor += InputBlur.Load(int3(CurVerticlPixel, 0)) * SampleWeights[i + kernelhalf];
        }
        BloomOutput[CurPixel] = float4(vColor.rgb, 1.0f);
    }

    GroupMemoryBarrierWithGroupSync();
}

