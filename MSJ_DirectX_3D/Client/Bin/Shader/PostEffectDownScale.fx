#include "Share.fx"

// Res      : �� ������ ���̿� �ʺ� 4�� ���� ��
// Domain   : �� ������ ���̿� �ʺ� ���� �� 16���� ���� ��
// GroupSize : �� ������ ���̿� �ʺ� ���� �� 16���� ���� ���� 1024�� ���� �� 
cbuffer DownScaleConstants : register(b11)
{
    uint2 Res;              // �ٿ� ������ �ػ� ��� : x - �ʺ�, y - ����   : ������� ���̿� �ʺ� 4�� ���� ��
    uint Domain;            // �ٿ� ������ �̹����� �� �ȼ� ��                : ������ ���̿� �ʺ� ���� �� 16���� ������ �� Res�� �� �ȼ���
    uint GroupSize;         // ù �н��� ����� �׷� �� ���                  : ������� ���̿� �ʺ� ���� �� 16���� ���� ���� 1024�� ���� ��
    float Adaptation;       // ���� ��
    float fBloomThreshold;  // ��� �Ӱ谪 ����
    float2 Empty;
}


Texture2D HDRTex : register(t5);
StructuredBuffer<float> AverageValuesID : register(t6);
StructuredBuffer<float> PrevAvgLum : register(t7); // ���� - ����..������������ ��� �ֵ��� �����

RWStructuredBuffer<float> AverageLum : register(u0);            // �������¾ַ� ����
RWTexture2D<float4> HDRDownScale : register(u3);                // �����ϴ¾� ���


groupshared float SharedPositions[1024]; // �����޸�

static const float4 LUM_FACTOR = float4(0.299, 0.587, 0.114, 0); //  �ֵ� ����� ���� ����� ���س���

// ��� ���̴� �ڵ�

// ù��° ���� �Լ� : �� �����忡 ���� 4x4 �ٿ� ������ ����
float DownScale4x4(uint2 _CurPixel, uint _groupThreadID)
{
    float avgLum = 0.0; // ��� �ֵ���

    // �ȼ� ���� ����
    if (_CurPixel.y < Res.y)
    {
        float fCount = 0.0f;
        // 4x4 �ȼ� �׷� �� �ջ�
        int3 iFullResPos = int3(_CurPixel * 4, 0);
        float4 downScaled = float4(0.0, 0.0, 0.0, 0.0);

		[unroll]
        for (int i = 0; i < 4; i++)
        {
			[unroll]
            for (int j = 0; j < 4; j++)
            {
                downScaled += HDRTex.Load(iFullResPos, int2(j, i));
            }
        }


        fCount = downScaled.a;
        //downScaled /= 16.0;        
        downScaled.rgb /= (fCount + 0.01f); // ���
        downScaled.a = 1.0f;
        HDRDownScale[_CurPixel.xy] = downScaled; // 1/4 �̹��� �ػ� ����
        avgLum = dot(downScaled, LUM_FACTOR);           // �ȼ��� �ֵ� �� ���
        //HDRDownScale[_CurPixel.xy] = avgLum; // 1/4 �̹��� �ػ� ����
        // ���� �޸𸮿� ��� ���
        SharedPositions[_groupThreadID] = avgLum;

    }

    // ����ȭ �� ���� �ܰ�
    // GroupMemoryBarrierWithGroupSync : ��� �׷� ���� �׼����� �Ϸ�ǰ� �׷��� ��� �����尡�� ȣ�⿡ ���� �� ������ �׷��� ��� ������ ������ ����
    GroupMemoryBarrierWithGroupSync();

    return avgLum;
}


// �ι�° ���� �Լ�
// �Ʒ� �Լ��� ������ ���� ���� 4���� ������ �ٿ� �������Ѵ�.
float DownScale1024to4(uint _dispatchThreadID, uint _groupThreadID, float _avgLum)
{
    // �ٿ� ������ �ڵ带 Ȯ��
    [unroll]
    for (uint groupSize = 4, step1 = 1, step2 = 2, step3 = 3; groupSize < 1024; groupSize *= 4, step1 *= 4, step2 *= 4, step3 *= 4)
    {
        // �ȼ� ���� ��� ����
        if (_groupThreadID % groupSize == 0)
        {
            // �ֵ��� ���
            float stepAvgLum = _avgLum;
            stepAvgLum += (_dispatchThreadID + step1) < Domain ? SharedPositions[_groupThreadID + step1] : _avgLum;
            stepAvgLum += (_dispatchThreadID + step2) < Domain ? SharedPositions[_groupThreadID + step2] : _avgLum;
            stepAvgLum += (_dispatchThreadID + step3) < Domain ? SharedPositions[_groupThreadID + step3] : _avgLum;


            // ��� �� ����
            _avgLum = stepAvgLum;
            SharedPositions[_groupThreadID] = stepAvgLum;
        }

        // ����ȭ �� ���� �ܰ�
        GroupMemoryBarrierWithGroupSync();
    }

    return _avgLum;
}

// ����° ���� �Լ�
// 4���� ���� �ϳ��� ��հ����� �ٿ���� ��  �� �����Ѵ�.
void DownScale4to1(uint _dispatchTreadID, uint _groupThreadID, uint _groupID, float _avgLum)
{
    if (_groupThreadID == 0)
    {
        // ������ �׷쿡 ���� ��� �ֵ� �� ���
        float fFinalAvgLum = _avgLum;

        fFinalAvgLum += (_dispatchTreadID + 256) < Domain ? SharedPositions[_groupThreadID + 256] : _avgLum;
        fFinalAvgLum += (_dispatchTreadID + 512) < Domain ? SharedPositions[_groupThreadID + 512] : _avgLum;
        fFinalAvgLum += (_dispatchTreadID + 768) < Domain ? SharedPositions[_groupThreadID + 768] : _avgLum;

        fFinalAvgLum /= 1024.0;

        // ���� ���� 1D UAV�� ��� �� ���� ��������
        AverageLum[_groupID] = fFinalAvgLum;
    }
}

// ������ ���� ���� ���̴� ��Ʈ�� ����Ʈ�� ���Եȴ�.  ( ��Ʈ�� ����Ʈ : ��� �ü������ ��ǻ�� ���α׷����� �̵��ϴ� �� )
// ù��° ��� ���̴�
[numthreads(1024, 1, 1)]
void DownScaleFirstPass(uint3 _dispatchThreadId : SV_DispatchThreadID,
                        uint3 _groupThreadId : SV_GroupThreadID,
                        uint3 _groupId : SV_GroupID)
{
    uint2 CurPixel = uint2(_dispatchThreadId.x % Res.x, _dispatchThreadId.x / Res.x);

    // 16�ȼ� �׷��� �ϳ��� �ȼ��� �ٿ� ���� �޸𸮿� ����
    float avgLum = DownScale4x4(CurPixel, _groupThreadId.x);

    // 1024���� 4�� �ٿ� ��������
    avgLum = DownScale1024to4(_dispatchThreadId.x, _groupThreadId.x, avgLum);

    // 4���� 1�� �ٿ����
    DownScale4to1(_dispatchThreadId.x, _groupThreadId.x, _groupId.x, avgLum);

}


// �ι�° �����̴�   -> ù��° �����̴����� �Ѿ�� �߰� ����� �ٽ� �ٿ� �������ؼ� ���� ��� �ֵ� ���� ����
// ù��° �����̴��� ������ �Ϥ� �Ǹ� ������ ��� ���۸� �����
// �ι�° �����̴��� �����ϵ� �߰� �� �ֵ� SRV�� ��� �ֵ� UAV���� ������ ����Ѵ�.
#define MAX_GROUP 64

// ���� �޸� �׷쿡 �߰� �� ����
groupshared float SharedAvgFinal[MAX_GROUP];

[numthreads(MAX_GROUP, 1, 1)]
void DownScaleSecondPass(uint3 _groupID : SV_GroupID,
                        uint3 _groupThreadID : SV_GroupThreadID,
                        uint3 _dispatchThreadID : SV_DispatchThreadID,
                        uint _groupIndex : SV_GroupIndex)
{
    // ���� �޸𸮿� ID �� ����
    float avgLum = 0.0f;

    if (_dispatchThreadID.x < GroupSize)
    {
        avgLum = AverageValuesID[_dispatchThreadID.x];
    }

    SharedAvgFinal[_dispatchThreadID.x] = avgLum;

    // ����ȭ �� ���� �ܰ�
    GroupMemoryBarrierWithGroupSync();

    // 64���� 16���� �ٿ� ������
    if (_dispatchThreadID.x % 4 == 0)
    {
        // �ֵ��� ���
        float stepAvgLum = avgLum;
        stepAvgLum += (_dispatchThreadID.x + 1) < GroupSize ? SharedAvgFinal[_dispatchThreadID.x + 1] : avgLum;
        stepAvgLum += (_dispatchThreadID.x + 2) < GroupSize ? SharedAvgFinal[_dispatchThreadID.x + 2] : avgLum;
        stepAvgLum += (_dispatchThreadID.x + 3) < GroupSize ? SharedAvgFinal[_dispatchThreadID.x + 3] : avgLum;


        // ��� �� ���� ( �����޸𸮿�.. )
        avgLum = stepAvgLum;
        SharedAvgFinal[_dispatchThreadID.x] = stepAvgLum;

    }

    // ����ȭ �� ���� �ܰ�
    GroupMemoryBarrierWithGroupSync();

    // 16���� 4�� �ٿ� ������
    if (_dispatchThreadID.x % 16 == 0)
    {
        // �ֵ��� ���
        float stepAvgLum = avgLum;
        stepAvgLum += (_dispatchThreadID.x + 4) < GroupSize ? SharedAvgFinal[_dispatchThreadID.x + 4] : avgLum;
        stepAvgLum += (_dispatchThreadID.x + 8) < GroupSize ? SharedAvgFinal[_dispatchThreadID.x + 8] : avgLum;
        stepAvgLum += (_dispatchThreadID.x + 12) < GroupSize ? SharedAvgFinal[_dispatchThreadID.x + 12] : avgLum;


        // ��� �� ���� ( �����޸𸮿�.. )
        avgLum = stepAvgLum;
        SharedAvgFinal[_dispatchThreadID.x] = stepAvgLum;

    }

    // ����ȭ �� ���� �ܰ�
    GroupMemoryBarrierWithGroupSync();


    // 4���� 1�� �ٿ� ������ ( �갡 ������)
    if (_dispatchThreadID.x == 0)
    {
        // �ֵ��� ���
        float fFinalstepAvgLum = avgLum;
        fFinalstepAvgLum += (_dispatchThreadID.x + 16) < GroupSize ? SharedAvgFinal[_dispatchThreadID.x + 16] : avgLum;
        fFinalstepAvgLum += (_dispatchThreadID.x + 32) < GroupSize ? SharedAvgFinal[_dispatchThreadID.x + 32] : avgLum;
        fFinalstepAvgLum += (_dispatchThreadID.x + 48) < GroupSize ? SharedAvgFinal[_dispatchThreadID.x + 48] : avgLum;

        fFinalstepAvgLum /= 64.0f;

        // ���� �ֵ� ���
        // lerp(x,y,s) : ���������� x + s(y - x) �� ����
        //if (0.0 == PrevAvgLum[0])
        //    PrevAvgLum = 1.0f;

        float fAdaptedAverageLum = lerp(PrevAvgLum[0], fFinalstepAvgLum, Adaptation);

        AverageLum[0] = max(fAdaptedAverageLum, 0.0001); // ��� �ֵ����� ������ (������)
        //PrevAvgLum[0] = AverageLum[0];
        
    }

}

// ������� �ϸ� ��� �ֵ����� ���߰�, ���� �� ������ ����
// LDR ���� ����ϴ� Ǯ ��ũ���� �������Ұ��� ������ -> ����Ÿ�� ���




// ���
Texture2D<float4> HDRDownScaleTex : register(t5);
StructuredBuffer<float> AvgLum : register(t6);
RWTexture2D<float4> Bloom : register(u0);

[numthreads(1024, 1, 1)]
void BrightPass(uint3 _DispatchThreadID : SV_DispatchThreadID)
{
    // �ٿ���ϵ� HDR�� SRV�� ��� �ֵ��� ��� �� �ӽ÷� UAV�� ����
    // �� �Ŀ� ù �н����� �ֵ� �ٿ���Ͽ� ���� �׷� ���ڸ�ŭ ��ǻƮ ���̴��� �����Ѵ�.
    uint2 CurPixel = uint2(_DispatchThreadID.x % Res.x, _DispatchThreadID.x / Res.x);

    // �ȼ� ���� ����
    if (CurPixel.y < Res.y)
    {
        float4 Color = HDRDownScaleTex.Load(int3(CurPixel, 0));
        float Lum = dot(Color, LUM_FACTOR);
        float avgLum = AvgLum[0];

        // ���� ������ ���
        float ColorScale = saturate(Lum - avgLum * fBloomThreshold);

        // ��� ������ ���� �� ����
        Bloom[CurPixel.xy] = Color * ColorScale;

    }

}