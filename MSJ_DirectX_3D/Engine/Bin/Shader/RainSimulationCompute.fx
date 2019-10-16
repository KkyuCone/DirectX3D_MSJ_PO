
Texture2D<float4> NoiseTex : register(t4);                  // 노이즈 텍스처에서 랜덤 값들을 가져온다.
Texture2D<float> HeightTex : register(t5);                  // 높이맵에서 씬의 충돌 감지에 대한 텍스처 (비관련)

// 강우 버퍼관련
struct RainDrop
{
    float3  vPosition;
    float3  vVel;
    float   fState;
};

RWStructuredBuffer<RainDrop> RainData : register(u0);

cbuffer RainConstants : register(b0)
{
    float4x4    ToHeight;           // 월드 공간에서 높이 맵 공간으로의 변환 값
    float3      BoundCenter;        // 월드 공간에서의 강우 영역의 중점
    float       DT;                 // 마지막으로 업데이트한 시간
    float3      BoundHalfSize;      // 강우 영역 크기 (박스기준, 절반)
    float       WindVeriation;      // 각 빗방울 사이의 바람 편차
    float2      WindFoce;           // 바람 세기
    float       VertSpeed;          // 자유 낙하 속도
    float       HeightMapSize;      // 높이 맵 텍스처의 크기
}

static const int g_iNumDispatch = 4;
static const int g_iNumThreads = 32;
static const int g_iDimSize = g_iNumDispatch * g_iNumThreads;

[numthreads(g_iNumThreads, g_iNumThreads, 1)]
void SimulateRain(uint3 dispatchThreadId : SV_DispatchThreadID)
{
	// 빗방울 스레드 계산
    uint GroupIdx = dispatchThreadId.x + dispatchThreadId.y * g_iDimSize;
    RainDrop curDrop = RainData[GroupIdx];

	// 새로운 위치값 계산(빗방울)
    curDrop.vPosition += curDrop.vVel * DT;

	// 파티클을 ..즉 빗방울을 정한 영역 내 유지
    float2 offsetAmount = (curDrop.vPosition.xz - BoundCenter.xz) / BoundHalfSize.xz;
    curDrop.vPosition.xz -= BoundHalfSize.xz * ceil(0.5 * offsetAmount - 0.5);

	// 파티클이 수직 영역을 벗어날 경우 다시 위에서.. (리스폰개념)
    if (abs(curDrop.vPosition.y - BoundCenter.y) > BoundHalfSize.y)
    {
		// 랜덤 값으로 파티클 리스폰
		// 텍스처에서 노이즈 샘플링
        float4 vNoiseNorm = NoiseTex.Load(int3(dispatchThreadId.xy, 0));
        float4 vNoise = (vNoiseNorm * 2.0) - 1.0;

		// 영역 중심을 기준으로해서 정렬
        curDrop.vPosition.xz = BoundCenter.xz + BoundHalfSize.xz * vNoise.xy;

		// 영역 위쪽 지점 기준으로 랜덤 높이값 설정
        curDrop.vPosition.y = BoundCenter.y + BoundHalfSize.y;
        curDrop.vPosition.y -= dot(vNoiseNorm.zw, 0.2f) * BoundHalfSize.y;

		// 바람세기 기준으로 초기 속도 설정
        curDrop.vVel.xz = lerp(WindFoce, WindFoce * vNoise.zw, WindVeriation);
        curDrop.vVel.y = VertSpeed;
    }

	// 파티클이 충돌했는지 여부 확인
	// 빗방울의 월드 위치를 높이 맵 공간으로 변환
    float4 posInHeight = float4(curDrop.vPosition, 1.0);
    posInHeight = mul(posInHeight, ToHeight);
    posInHeight.xy = 0.5 * (posInHeight.xy + 1.0);
    posInHeight.y = 1.0 - posInHeight.y;
    posInHeight.xy *= HeightMapSize;

	// 맵에서 높이값 샘플링
    float height = HeightTex.Load(uint3(posInHeight.xy, 0));

	// 맵높이 값을 빗방울 높이 값과 비교
    curDrop.fState = posInHeight.z < height ? 1.0 : -1.0;

	// 최종값
    RainData[GroupIdx] = curDrop;
}
