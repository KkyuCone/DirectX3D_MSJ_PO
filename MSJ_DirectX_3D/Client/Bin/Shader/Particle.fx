#include "Share.fx"

// ����Ʈ�� �簢�� �޽��� Ȯ���� ����. �׷��� ��ġ�� ũ�Ⱑ �ʿ���, ��ġ�� �߽��� �̶�� �����ϸ��
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
    float4 vProjPos : POSITION;             // �������� ��ġ
    float2 vUV : TEXCOORD;
	float fParticlePercent : PERSENT;
};

struct GS_OUTPUT_PARTICLE_INSTANCING
{
    float4 vPos : SV_POSITION;
    float4 vProjPos : POSITION; // �������� ��ġ
    float2 vUV : TEXCOORD;
    int iFrame : FRAME;
	float fParticlePercent : PERSENT;
};


// ��ƼŬ �������
cbuffer Particle : register(b11)
{
    float3  g_vParticlePos;          // ������� ��ġ����
    float   g_fSizeX;               // ���� ������
    float3  g_vAxisX;               // X��
    float   g_fSizeY;               // ���� ������
    float3  g_vAxisY;               // Y��
    float   g_fPercent;
}

// Texture2DArray
Texture2DArray g_FrameTexture : register(t10);          // 2D�ִϸ��̼� �Ϸ��� ������ �迭�� �ε����� z������ ��
Texture2D g_GBufferDepth : register(t11);               // ����Ʈ��ƼŬ�� ���ؼ� ���̿� �ؽ�ó�� �ʿ��Ѵ�. ( ©���°� �ƴ϶� ���մ°�ó�� ǥ���Ϸ���.. �ڿ��� ���İ� ���� )  -> ������ ȭ�鿡���� ��ġ������ ������ ���ĺ����� �������

// ����Ʈ�� �ϳ� ������ �Ŵϱ� �״�� return��Ų��.
VS_INPUT_PARTICLE VS_Particle(VS_INPUT_PARTICLE _Input)
{
    return _Input;
}

VS_INPUT_FRAMEANIMATION_INSTANCING VS_Particle_INSTANCING(VS_INPUT_FRAMEANIMATION_INSTANCING _Input)
{
    return _Input;
}


// �Լ� ȣ�� ���
// 1. Call by Value    -> �� ��ȯ
// 2. Call by Address  -> ������ ��ȯ
// 3. Call by Reference -> ���۷��� ��ȯ (����)

// ���ϼ��̴� -> ���ϼ��̴��� ����� �� �ִ� Ű������� ������ �ִ�.
// ���� ���� �� �Ѱ��� �����ðŴϱ� Input-> 1��,  �׸��� � Primitive�� ��������� �����´�.
// inout -> ���۷������ �����ϸ� �ȴ�. out�� ���� ���ϸ� �ϱ� ���Ѱ��̱⶧���� �Ϻ��� ���۷������ ���� �����.
[maxvertexcount(4)]         // ���ؽ� ���� ( �츰 �簢�� �޽� ����Ŵϱ� 4�� �ʿ� )
void GS_Particle(point VS_INPUT_PARTICLE _Input[1], inout TriangleStream<GS_OUTPUT_PARTICLE> _stream)
{
    float fHalfX = g_fSizeX * 0.5f;
    float fHalfY = g_fSizeY * 0.5f;
    //float fHalfX = 1.5f;
    //float fHalfY = 1.5f;

    // ���� ��ġ���� �����Ѵ�.
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

// �ν��Ͻ̿� ��ƼŬ
[maxvertexcount(4)] 
void GS_Particle_INSTANCING(point VS_INPUT_FRAMEANIMATION_INSTANCING _Input[1], inout TriangleStream<GS_OUTPUT_PARTICLE_INSTANCING> _stream)
{
    VS_INPUT_FRAMEANIMATION_INSTANCING ReInput = _Input[0];

    float fHalfX = _Input[0].vScale.x / 2.0f;
    float fHalfY = _Input[0].vScale.y / 2.0f;

    // ���� ��ġ���� �����Ѵ�.
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
        // ��Ʋ���� ��� �ش� UV���� �´¾ָ� ȭ�鿡 ����ϸ� �ȴ�.
        output.vColor = g_DiffuseTexture.Sample(g_LinearSampler, _Input.vUV);
    }
    else
    {
        // g_iAniFrameFrame-> �迭�� �ε����� z������ ��
        output.vColor = g_FrameTexture.Sample(g_LinearSampler, float3(_Input.vUV, g_iAniFrameFrame));
    }
    
    if(0.0f == output.vColor.a)
    {
        clip(-1);
    }

    // ���������� ��ǥ�� �̿��ؼ� ȭ�鿡���� UV��ǥ�� ���Ѵ�. (NDC����)
    float2 vDepthUV = _Input.vProjPos.xy / _Input.vProjPos.w;
    vDepthUV.x = vDepthUV.x * 0.5f + 0.5f;      // 0 ~ 1���̷�..
    vDepthUV.y = vDepthUV.y * -0.5f + 0.5f;

    float4 vDepth = g_GBufferDepth.Sample(g_PointSampler, vDepthUV);

    // �� ������ Z���� �̿��ؼ� ���Ѵ�.
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
        // �ش� ������Ʈ���� �ӰŸ��� ���ٸ�
        fDepthDistance = 0.4f;      // �ּҰ�
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
        // ��Ʋ���� ��� �ش� UV���� �´¾ָ� ȭ�鿡 ����ϸ� �ȴ�.
        output.vColor = g_DiffuseTexture.Sample(g_LinearSampler, _Input.vUV);
    }
    else
    {
        // g_iAniFrameFrame-> �迭�� �ε����� z������ ��
        output.vColor = g_FrameTexture.Sample(g_LinearSampler, float3(_Input.vUV, _Input.iFrame));
    }
    
    if (0.0f == output.vColor.a)
    {
        clip(-1);
    }

    // ���������� ��ǥ�� �̿��ؼ� ȭ�鿡���� UV��ǥ�� ���Ѵ�. (NDC����)
    float2 vDepthUV = _Input.vProjPos.xy / _Input.vProjPos.w;
    vDepthUV.x = vDepthUV.x * 0.5f + 0.5f; // 0 ~ 1���̷�..
    vDepthUV.y = vDepthUV.y * -0.5f + 0.5f;

    float4 vDepth = g_GBufferDepth.Sample(g_PointSampler, vDepthUV);

    // �� ������ Z���� �̿��ؼ� ���Ѵ�.
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
        // �ش� ������Ʈ���� �ӰŸ��� ���ٸ�
        fDepthDistance = 0.4f; // �ּҰ�
    }

    float fAlpha = fDepthDistance / 0.4f;
    fAlpha = min(fAlpha, 1.0f);

    output.vColor.a *= fAlpha;

    return output;

}