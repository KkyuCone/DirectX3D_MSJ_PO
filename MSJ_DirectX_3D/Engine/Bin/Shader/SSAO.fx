
cbuffer DownScaleCBuffer : register(b0)
{
    uint2 Res;              // ȭ�� ������� 1/4�ػ� ��
    float2 ResRcp;          // Res�� ������
    float4 vProjParams;     // ���� �Ű�����, ���۵� ����Ʈ ����Ҷ� ����ѰŶ� ����
    matrix matView;         // ī�޶� �� ���
    float fOffsetRadius;    // ���� ��ġ�� ã�� ���� ������ ����
    float fRadius;          // �ں��Ʈ �ݱ��� ������ ��
    float fMaxDepth;
    int iEmpty;
}

Texture2D<float4> DepthTex : register(t0);
Texture2D<float4> NormalTex : register(t1);
RWStructuredBuffer<float4> MiniDepthRW : register(u0);

// ���̹��۸� ���� ���� ������ ��ȯ�ϴ� �Լ�
float ConvertDepthToLinearDepth(float _Depth)
{
    float LinearDepth = vProjParams.z / (_Depth + vProjParams.w);
    return LinearDepth;
}


// �ٿ���� �ϴ� �Լ� 1/4��..
[numthreads(1024, 1, 1)]
void DownScale(uint3 _DispatchTreadID : SV_DispatchThreadID)
{
    uint3 CurPixel = uint3(_DispatchTreadID.x % Res.x, _DispatchTreadID.x / Res.x, 0);

    // �ȼ� ���� ����
    if(CurPixel.y < Res.y)
    {
        float fminDepth = 1.0f;
        float3 AvgNormal = float3(0.0f, 0.0f, 0.0f);
        uint3 FullResPixel = CurPixel * 2;

        [unroll]
        for (int i = 0; i < 2; ++i)
        {
            [unroll]
            for (int j = 0; j < 2; ++j)
            {
                // �ȼ� ���� �� ��� �� �ּڰ� ����
                float CurDepth = DepthTex.Load(FullResPixel, int2(j, i)).x;
                fminDepth = min(CurDepth, fminDepth);

                // �� ���� �븻�� �ջ� �� ��հ� ���
                float3 NormalWorld = NormalTex.Load(FullResPixel, int2(j, i)).xyz;
                NormalWorld = normalize(NormalWorld * 2.0f - 1.0f);
                AvgNormal += mul(NormalWorld, (float3x3) matView);
            }
        }

        MiniDepthRW[_DispatchTreadID.x].x = ConvertDepthToLinearDepth(fminDepth);
        float3 avgNormalView = mul(AvgNormal * 0.25f, (float3x3)matView);
        MiniDepthRW[_DispatchTreadID.x].yzw = avgNormalView;
    }
}

// ���� �� �ȼ� ���� 1024�� ���� ��ŭ�� X ������ �׷��� �ٿ������ ��ǻƮ ���̴��� �����Ѵ�.
// ���� �� ��� ���� �ٿ� ������ ���ۿ� ������ �Ŀ� AO���� ����ؾ��Ѵ�.
// 1/4 ũ�� ���� ��� SRV�� �Է� ������ �����ϰ� AO, UAV���� ��°����� �����Ѵ�.

StructuredBuffer<float4> MinDepth : register(t0);
RWTexture2D<float> AO : register(u0);
RWStructuredBuffer<float> SharedDepth : register(u1);

//groupshared float SharedDepth[1024];

static const float NumSamplesRcp = 1.0 / 8.0;
static const uint NumSamples = 8;
static const float2 SampleOffsets[NumSamples] =
{
    float2(0.2803166, 0.08997212),
	float2(-0.5130632, 0.6877457),
	float2(0.425495, 0.8665376),
	float2(0.8732584, 0.3858971),
	float2(0.0498111, -0.6287371),
	float2(-0.9674183, 0.1236534),
	float2(-0.3788098, -0.09177673),
	float2(0.6985874, -0.5610316),
};

float GetDepth(int2 _Pos)
{
    // clamp(x, min, max) : x��  min�� ���� ���� ��� min����, max���� Ŭ��� max������ ��ȯ
    // �ش� �ȼ� ��ġ ��������
    float x = clamp(_Pos.x, 0, Res.x - 1);
    float y = clamp(_Pos.y, 0, Res.y - 1);

    // ���̰����� ���� �ּұ��� �ε�����ġ�� ã�´�.
    int minDepthIdx = x + y * Res.x;
    return MinDepth[minDepthIdx].x;
}

float3 GetNormal(int2 _Pos)
{
    float x = clamp(_Pos.x, 0, Res.x - 1);
    float y = clamp(_Pos.y, 0, Res.y - 1);

    int miniDepthIdx = x + y * Res.x;
    return MinDepth[miniDepthIdx].yzw;
}

// SSAO ����ϴ� �Լ�
float ComputeAO(int2 _CenterPixelPos, float2 _CenterClipPos)
{
    // ��� ��꿡 �ʿ��� ���� �� ���
    float fCenterDepth = GetDepth(_CenterPixelPos.xy);

    // �߽� �ȼ��� �� ���� ��ġ ���ϱ�
    float3 CenterPos;
    CenterPos.xy = _CenterClipPos * vProjParams.xy * fCenterDepth;
    CenterPos.z = fCenterDepth;

    // �߽� �ȼ��� �� ���� �븻�� ���ϱ�
    float3 CenterNormal = GetNormal(_CenterPixelPos.xy);
    CenterNormal = normalize(CenterNormal);

    // ���� ���ø� ������
    float RotationAngle = dot(float2(_CenterClipPos), float2(73.0, 197.0));
    float2 RandSinCos;
    // sincos(X, s, c) sin(X)�� cos(X)�� ���ÿ� s, c�� ����.
    sincos(RotationAngle, RandSinCos.x, RandSinCos.y);
    float2x2 RandRotMatrix = float2x2(RandSinCos.y, -RandSinCos.x,
                                        RandSinCos.x, RandSinCos.y);

    // �� ���ÿ� ���� �ں��Ʈ ��Ŭ��� �� ���
    float AO = 0.0f;
    
    [unroll]
    for (uint i = 0; i < NumSamples; ++i)
    {
        // �ؽ�ó ���� ��ġ �� ���� ���
        float2 SampleOff = fOffsetRadius.xx * mul(SampleOffsets[i], RandRotMatrix);
        float CurDepth = GetDepth(_CenterPixelPos + int2(SampleOff.x, -SampleOff.y));

        // �� ���� ��ġ �� ���
        float3 CurPos;
        CurPos.xy = (_CenterClipPos + 2.0f * SampleOff * ResRcp) * vProjParams.xy * CurDepth;
        CurPos.z = CurDepth;

        float3 CenterToCurPos = CurPos - CenterPos;
        float LenCenterToCurPos = length(CenterToCurPos);
        float AngleFactor = 1.0f - dot(CenterToCurPos / LenCenterToCurPos, CenterNormal);
        float DistFactor = LenCenterToCurPos / fRadius;

        // ���� AO �μ� �ջ�
        AO += saturate(max(DistFactor, AngleFactor));
    }

    return AO * NumSamplesRcp;
}


// SSAO ��ǻƮ���̴�
[numthreads(1024, 1, 1)]
void ComputeSSAO(uint3 _GroupThreadID : SV_GroupThreadID, 
                uint3 _DispathThreadID : SV_DispatchThreadID)
{
    uint2 CurPixel = uint2(_DispathThreadID.x % Res.x, _DispathThreadID.x / Res.x);
    SharedDepth[_GroupThreadID.x] = MinDepth[_DispathThreadID.x].x;

    GroupMemoryBarrierWithGroupSync();

    // �ȼ� ���� ����
    if (CurPixel.y < Res.y)
    {
        // ���� �ȼ��� ���� XY Ŭ�� ���� ��ġ ���
        // Y�� ���� ������ ǥ�õ�
        float2 CenterClipPos = 2.0f * float2(CurPixel) * ResRcp;
        CenterClipPos = float2(CenterClipPos.x - 1.0f, 1.0f - CenterClipPos.y);
        AO[CurPixel] = ComputeAO(CurPixel, CenterClipPos);

    }

}