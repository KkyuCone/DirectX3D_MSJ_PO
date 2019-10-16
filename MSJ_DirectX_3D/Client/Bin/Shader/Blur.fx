// 2�н�
Texture2D<float4> Input : register(t0); 
RWTexture2D<float4> BlurOP : register(u0);

Texture2D<float4> InputBlur : register(t1);
RWTexture2D<float4> BloomOutput : register(u1);

cbuffer DownScaleConstants : register(b11)
{
    uint2 Res; // �ٿ� ������ �ػ� ��� : x - �ʺ�, y - ����
    uint Domain; // �ٿ� ������ �̹����� �� �ȼ� ��
    uint GroupSize; // ù �н��� ����� �׷� �� ���
    float Adaptation; // ���� ��
    float fBloomThreshold; // ��� �Ӱ谪 ����
    float2 Empty;
}

// ����þ� ���ø� ����ġ�� ������ ��� �迭
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
    // �ٿ���ϵ� HDR�� SRV�� ��� �ֵ��� ��� �� �ӽ÷� UAV�� ����
    // �� �Ŀ� ù �н����� �ֵ� �ٿ���Ͽ� ���� �׷� ���ڸ�ŭ ��ǻƮ ���̴��� �����Ѵ�.
    uint2 CurPixel = uint2(_DispatchThreadID.x % Res.x, _DispatchThreadID.x / Res.x);

    // ����
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
    // �ٿ���ϵ� HDR�� SRV�� ��� �ֵ��� ��� �� �ӽ÷� UAV�� ����
    // �� �Ŀ� ù �н����� �ֵ� �ٿ���Ͽ� ���� �׷� ���ڸ�ŭ ��ǻƮ ���̴��� �����Ѵ�.
    uint2 CurPixel = uint2(_DispatchThreadID.x % Res.x, _DispatchThreadID.x / Res.x);

    // ����
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

