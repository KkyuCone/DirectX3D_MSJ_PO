
Texture2D<float4> NoiseTex : register(t4);                  // ������ �ؽ�ó���� ���� ������ �����´�.
Texture2D<float> HeightTex : register(t5);                  // ���̸ʿ��� ���� �浹 ������ ���� �ؽ�ó (�����)

// ���� ���۰���
struct RainDrop
{
    float3  vPosition;
    float3  vVel;
    float   fState;
};

RWStructuredBuffer<RainDrop> RainData : register(u0);

cbuffer RainConstants : register(b0)
{
    float4x4    ToHeight;           // ���� �������� ���� �� ���������� ��ȯ ��
    float3      BoundCenter;        // ���� ���������� ���� ������ ����
    float       DT;                 // ���������� ������Ʈ�� �ð�
    float3      BoundHalfSize;      // ���� ���� ũ�� (�ڽ�����, ����)
    float       WindVeriation;      // �� ����� ������ �ٶ� ����
    float2      WindFoce;           // �ٶ� ����
    float       VertSpeed;          // ���� ���� �ӵ�
    float       HeightMapSize;      // ���� �� �ؽ�ó�� ũ��
}

static const int g_iNumDispatch = 4;
static const int g_iNumThreads = 32;
static const int g_iDimSize = g_iNumDispatch * g_iNumThreads;

[numthreads(g_iNumThreads, g_iNumThreads, 1)]
void SimulateRain(uint3 dispatchThreadId : SV_DispatchThreadID)
{
	// ����� ������ ���
    uint GroupIdx = dispatchThreadId.x + dispatchThreadId.y * g_iDimSize;
    RainDrop curDrop = RainData[GroupIdx];

	// ���ο� ��ġ�� ���(�����)
    curDrop.vPosition += curDrop.vVel * DT;

	// ��ƼŬ�� ..�� ������� ���� ���� �� ����
    float2 offsetAmount = (curDrop.vPosition.xz - BoundCenter.xz) / BoundHalfSize.xz;
    curDrop.vPosition.xz -= BoundHalfSize.xz * ceil(0.5 * offsetAmount - 0.5);

	// ��ƼŬ�� ���� ������ ��� ��� �ٽ� ������.. (����������)
    if (abs(curDrop.vPosition.y - BoundCenter.y) > BoundHalfSize.y)
    {
		// ���� ������ ��ƼŬ ������
		// �ؽ�ó���� ������ ���ø�
        float4 vNoiseNorm = NoiseTex.Load(int3(dispatchThreadId.xy, 0));
        float4 vNoise = (vNoiseNorm * 2.0) - 1.0;

		// ���� �߽��� ���������ؼ� ����
        curDrop.vPosition.xz = BoundCenter.xz + BoundHalfSize.xz * vNoise.xy;

		// ���� ���� ���� �������� ���� ���̰� ����
        curDrop.vPosition.y = BoundCenter.y + BoundHalfSize.y;
        curDrop.vPosition.y -= dot(vNoiseNorm.zw, 0.2f) * BoundHalfSize.y;

		// �ٶ����� �������� �ʱ� �ӵ� ����
        curDrop.vVel.xz = lerp(WindFoce, WindFoce * vNoise.zw, WindVeriation);
        curDrop.vVel.y = VertSpeed;
    }

	// ��ƼŬ�� �浹�ߴ��� ���� Ȯ��
	// ������� ���� ��ġ�� ���� �� �������� ��ȯ
    float4 posInHeight = float4(curDrop.vPosition, 1.0);
    posInHeight = mul(posInHeight, ToHeight);
    posInHeight.xy = 0.5 * (posInHeight.xy + 1.0);
    posInHeight.y = 1.0 - posInHeight.y;
    posInHeight.xy *= HeightMapSize;

	// �ʿ��� ���̰� ���ø�
    float height = HeightTex.Load(uint3(posInHeight.xy, 0));

	// �ʳ��� ���� ����� ���� ���� ��
    curDrop.fState = posInHeight.z < height ? 1.0 : -1.0;

	// ������
    RainData[GroupIdx] = curDrop;
}
