#include "Share.fx"

// 포인트를 사각형 메쉬로 확장할 것임. 그래서 위치와 크기가 필요함, 위치는 중심점 이라고 생각하면됨
struct VS_INPUT_PARTICLE
{
    float3 vPos : POSITION;
};

struct VS_INPUT_FRAMEANIMATION_INSTANCING
{
    matrix matWVP : WORLDVIEWPROJ;
    matrix matVP : VIEWPROJ;
    float3 vParticlePos : WORLDPOS;
    float2 vScale : PARTICLESCALE;
    float2 vStart : COORDSTART;
    float2 vEnd : COORDEND;
    int iFrame : FRAME;

};


struct GS_OUTPUT_PARTICLE
{
    float4 vPos : SV_POSITION;
    float4 vProjPos : POSITION;             // 투영후의 위치
    float2 vUV : TEXCOORD;
	float fParticlePercent : PERSENT;
};

struct GS_OUTPUT_PARTICLE_INSTANCING
{
    float4 vPos : SV_POSITION;
    float4 vProjPos : POSITION; // 투영후의 위치
    float2 vUV : TEXCOORD;
    int iFrame : FRAME;
	float fParticlePercent : PERSENT;
};


// 파티클 상수버퍼
cbuffer Particle : register(b11)
{
    float3  g_vParticlePos;          // 월드상의 위치정보
    float   g_fSizeX;               // 가로 사이즈
    float3  g_vAxisX;               // X축
    float   g_fSizeY;               // 세로 사이즈
    float3  g_vAxisY;               // Y축
    float   g_fPercent;
}

// Texture2DArray
Texture2DArray g_FrameTexture : register(t10);          // 2D애니메이션 하려구 ㅇㅅㅇ 배열의 인덱스가 z값으로 들어감
Texture2D g_GBufferDepth : register(t11);               // 소프트파티클을 위해서 깊이용 텍스처가 필요한다. ( 짤리는게 아니라 못뚫는것처럼 표현하려구.. 뒤에꺼 알파값 조절 )  -> 투영후 화면에서의 위치정보를 가지고 알파비율을 만들거임

// 포인트를 하나 가져올 거니까 그대로 return시킨다.
VS_INPUT_PARTICLE VS_Particle(VS_INPUT_PARTICLE _Input)
{
    return _Input;
}

VS_INPUT_FRAMEANIMATION_INSTANCING VS_Particle_INSTANCING(VS_INPUT_FRAMEANIMATION_INSTANCING _Input)
{
    return _Input;
}


// 함수 호출 방식
// 1. Call by Value    -> 값 반환
// 2. Call by Address  -> 포인터 반환
// 3. Call by Reference -> 레퍼런스 반환 (참조)

// 기하셰이더 -> 기하셰이더는 사용할 수 있는 키워드들이 정해져 있다.
// 지금 현재 점 한개를 가져올거니까 Input-> 1개,  그리고 어떤 Primitive로 만들것인지 가져온다.
// inout -> 레퍼런스라고 생각하면 된다. out은 단지 리턴만 하기 위한것이기때문에 완벽한 레퍼런스라고 보긴 힘들다.
[maxvertexcount(4)]         // 버텍스 개수 ( 우린 사각형 메쉬 만들거니까 4개 필요 )
void GS_Particle(point VS_INPUT_PARTICLE _Input[1], inout TriangleStream<GS_OUTPUT_PARTICLE> _stream)
{
    float fHalfX = g_fSizeX * 0.5f;
    float fHalfY = g_fSizeY * 0.5f;
    //float fHalfX = 1.5f;
    //float fHalfY = 1.5f;

    // 정점 위치들을 구성한다.
    float3 vPos[4];
    vPos[0] = g_vParticlePos - g_vAxisX * fHalfX - g_vAxisY * fHalfY;
    vPos[1] = g_vParticlePos - g_vAxisX * fHalfX + g_vAxisY * fHalfY;
    vPos[2] = g_vParticlePos + g_vAxisX * fHalfX - g_vAxisY * fHalfY;
    vPos[3] = g_vParticlePos + g_vAxisX * fHalfX + g_vAxisY * fHalfY;

    float2 vUv[4] =
    {
        float2(0.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 1.0f),
		float2(1.0f, 0.0f)
    };

    GS_OUTPUT_PARTICLE output = (GS_OUTPUT_PARTICLE) 0;

    for (int i = 0; i < 4; ++i)
    {
        output.vProjPos = mul(float4(vPos[i], 1.0f), g_VP);
        output.vPos = output.vProjPos;
        output.vUV = ComputeFrameUV(vUv[i]);
		output.fParticlePercent = g_fPercent;
        _stream.Append(output);
    }
}

// 인스턴싱용 파티클
[maxvertexcount(4)] 
void GS_Particle_INSTANCING(point VS_INPUT_FRAMEANIMATION_INSTANCING _Input[1], inout TriangleStream<GS_OUTPUT_PARTICLE_INSTANCING> _stream)
{
    VS_INPUT_FRAMEANIMATION_INSTANCING ReInput = _Input[0];

    float fHalfX = _Input[0].vScale.x / 2.0f;
    float fHalfY = _Input[0].vScale.y / 2.0f;

    // 정점 위치들을 구성한다.
    float3 vPos[4];
    vPos[0] = _Input[0].vParticlePos - g_vAxisX * fHalfX - g_vAxisY * fHalfY;
    vPos[1] = _Input[0].vParticlePos - g_vAxisX * fHalfX + g_vAxisY * fHalfY;
    vPos[2] = _Input[0].vParticlePos + g_vAxisX * fHalfX - g_vAxisY * fHalfY;
    vPos[3] = _Input[0].vParticlePos + g_vAxisX * fHalfX + g_vAxisY * fHalfY;

    float2 vUv[4] =
    {
        float2(0.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 1.0f),
		float2(1.0f, 0.0f)
    };

    GS_OUTPUT_PARTICLE_INSTANCING output = (GS_OUTPUT_PARTICLE_INSTANCING) 0;
    output.iFrame = _Input[0].iFrame;
    for (int i = 0; i < 4; ++i)
    {
        output.vProjPos = mul(float4(vPos[i], 1.0f), _Input[0].matVP);
        output.vPos = output.vProjPos;
        output.vUV = ComputeInstancingFrameUV(vUv[i], ReInput.vStart, ReInput.vEnd);
        _stream.Append(output);
    }
}

PS_OUTPUT PS_Particle(GS_OUTPUT_PARTICLE _Input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;

    if(AFT_ATLAS == g_iAniFrameType)
    {
        // 아틀라스의 경우 해당 UV값에 맞는애를 화면에 출력하면 된다.
        output.vColor = g_DiffuseTexture.Sample(g_LinearSampler, _Input.vUV);
    }
    else
    {
        // g_iAniFrameFrame-> 배열의 인덱스가 z값으로 들어감
        output.vColor = g_FrameTexture.Sample(g_LinearSampler, float3(_Input.vUV, g_iAniFrameFrame));
    }
    
    if(0.0f == output.vColor.a)
    {
        clip(-1);
    }

    // 투영공간의 좌표를 이용해서 화면에서의 UV좌표를 구한다. (NDC공간)
    float2 vDepthUV = _Input.vProjPos.xy / _Input.vProjPos.w;
    vDepthUV.x = vDepthUV.x * 0.5f + 0.5f;      // 0 ~ 1사이로..
    vDepthUV.y = vDepthUV.y * -0.5f + 0.5f;

    float4 vDepth = g_GBufferDepth.Sample(g_PointSampler, vDepthUV);

    // 뷰 공간의 Z값을 이용해서 비교한다.
    float fDepthDistance = vDepth.w - _Input.vProjPos.w;

    if (0.0f == vDepth.w)
    {
        fDepthDistance = 1.0f;
    }
    else if (0.0f > fDepthDistance)
    {
        clip(-1);
    }
    else if (0.0f == fDepthDistance)
    {
        // 해당 오브젝트와으 ㅣ거리가 같다면
        fDepthDistance = 0.4f;      // 최소값
    }

    float fAlpha = fDepthDistance / 0.4f;
    fAlpha = min(fAlpha, 1.0f);

    output.vColor.a *= fAlpha;

	if (_Input.fParticlePercent < _Input.vUV.x)
	{
		output.vColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	}

    return output;

}

PS_OUTPUT PS_Particle_INSTANCING(GS_OUTPUT_PARTICLE_INSTANCING _Input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;

    if (AFT_ATLAS == g_iAniFrameType)
    {
        // 아틀라스의 경우 해당 UV값에 맞는애를 화면에 출력하면 된다.
        output.vColor = g_DiffuseTexture.Sample(g_LinearSampler, _Input.vUV);
    }
    else
    {
        // g_iAniFrameFrame-> 배열의 인덱스가 z값으로 들어감
        output.vColor = g_FrameTexture.Sample(g_LinearSampler, float3(_Input.vUV, _Input.iFrame));
    }
    
    if (0.0f == output.vColor.a)
    {
        clip(-1);
    }

    // 투영공간의 좌표를 이용해서 화면에서의 UV좌표를 구한다. (NDC공간)
    float2 vDepthUV = _Input.vProjPos.xy / _Input.vProjPos.w;
    vDepthUV.x = vDepthUV.x * 0.5f + 0.5f; // 0 ~ 1사이로..
    vDepthUV.y = vDepthUV.y * -0.5f + 0.5f;

    float4 vDepth = g_GBufferDepth.Sample(g_PointSampler, vDepthUV);

    // 뷰 공간의 Z값을 이용해서 비교한다.
    float fDepthDistance = vDepth.w - _Input.vProjPos.w;

    if (0.0f == vDepth.w)
    {
        fDepthDistance = 1.0f;
    }
    else if (0.0f > fDepthDistance)
    {
        clip(-1);
    }
    else if (0.0f == fDepthDistance)
    {
        // 해당 오브젝트와으 ㅣ거리가 같다면
        fDepthDistance = 0.4f; // 최소값
    }

    float fAlpha = fDepthDistance / 0.4f;
    fAlpha = min(fAlpha, 1.0f);

    output.vColor.a *= fAlpha;

    return output;

}