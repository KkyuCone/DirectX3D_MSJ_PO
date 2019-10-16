#include "Share.fx"

// ���� ������ ����� ���̴�����
// �����Ŵ����� �ִ� GBuffer �ؽ�ó�� �޴´�. 
// ����ó������ Albedo�� �ʿ� �����Ƿ� �븻, ����, ������ �޾ƿͼ� ó���Ѵ�.
// NullBuffer�� ����� �� ���̴�.
// NullBuffer�� ��� ����� �̹� ������ �ִ�.
// DirectX���� nullBuffer�� ����Ѵٰ� �ϸ� ȭ�� ��üũ�⸦ �׸��°ɷ� �ν��Ѵ�.
// �Է·��̾ƿ� nullptr, ���ؽ�����, �ε������� ���� nullptr�� �ʱ�ȭ�ؼ� �ѱ��.
// ���⼺ ������ ����ؾ��Ѵ�.


// Gbuffer�� �����ִ� �� ������ ���� �� ������ ó���� ����
Texture2D g_GBufferAlbedo : register(t10);
Texture2D g_GBufferNormal : register(t11);
Texture2D g_GBufferDepth : register(t12);
Texture2D g_GBufferMaterial : register(t13);


Texture2D g_LightDiffuse : register(t14);
Texture2D g_LightSpecular : register(t15);


// ȭ�� ��üũ��
static const float2 vNullPos[4] =
{
    float2(-1.f, 1.f),
	float2(1.f, 1.f),
	float2(-1.f, -1.f),
	float2(1.f, -1.f)
};

// UV
static const float2 vNullUV[4] =
{
    float2(0.f, 0.f),
	float2(1.f, 0.f),
	float2(0.f, 1.f),
	float2(1.f, 1.f)
};

// ���� ������ ����ü
struct PS_OUTPUT_LIGHTACC
{
    float4 vDiffuse : SV_TARGET;
    float4 vSpecular : SV_TARGET1;
};

// Directional Light VS
VS_OUTPUT_UV VS_LightAccDir(uint _iVertexID : SV_VertexID)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV) 0;

    output.vPos = float4(vNullPos[_iVertexID], 0.0f, 1.0f);
    output.vUV = vNullUV[_iVertexID];

    return output;
}

// Point Light VS
VS_OUTPUT_UV VS_LightAccPoint(VS_INPUT_POSITION _Input)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV) 0;

    float4 vPosition = mul(float4(_Input.vPos, 1.0f), g_WVP);
    output.vPos = vPosition;

    // w������ ������ ������ ������ĸ� ���ϸ� �Ÿ������� ũ�Ⱑ ������� �ʱ⶧��
    output.vUV.x = (vPosition.x / vPosition.w) * 0.5f + 0.5f;       // -1.0 ~ 1.0f -> 0 ~ 1
    output.vUV.y = (vPosition.y / vPosition.w) * -0.5f + 0.5f;       // 0~1�� �ϱ� ���� (-1 ~ 0)
  
    return output;
}

VS_OUTPUT_UV VS_LightAccSpot(VS_INPUT_POSITION _Input)
{
    //     g_fLightInAngle, g_fLightOutAngle, g_fLightDistance
    VS_OUTPUT_UV output = (VS_OUTPUT_UV) 0;

    //float4 TPosition = _Input.vPos;
    //TPosition.x = _Input.vPos.x
    //TPosition.y = g_fLightDistance;

    float4 vPosition = mul(float4(_Input.vPos, 1.0f), g_WVP);

    output.vPos = vPosition;

    // w������ ������ ������ ������ĸ� ���ϸ� �Ÿ������� ũ�Ⱑ ������� �ʱ⶧��
    output.vUV.x = (vPosition.x / vPosition.w) * 0.5f + 0.5f; // -1.0 ~ 1.0f -> 0 ~ 1
    output.vUV.y = (vPosition.y / vPosition.w) * -0.5f + 0.5f; // 0~1�� �ϱ� ���� (-1 ~ 0)
  
    return output;
}


// ����Ʈ ����
PS_OUTPUT_LIGHTACC PS_LightAcc(VS_OUTPUT_UV _Input)
{
    PS_OUTPUT_LIGHTACC output = (PS_OUTPUT_LIGHTACC) 0;

    // ���̰� ��������
    float4 vDepth = g_GBufferDepth.Sample(g_LinearSampler, _Input.vUV);

    // ��û �տ� �����ϱ� �ʿ� X
    if (0.0f == vDepth.w)
    {
        clip(-1);
    }

    // UV ��ǥ�� �̿��ؼ� w�� ���� ��ǥ�� ���Ѵ�
    // UV��ǥ�� 0~1  ������ ��ǥ���� ������ ������ǥ�� -1~1������ ��ǥ���� �����ϱ�
    // �����ϰ� ������ֱ� ���ؼ� 2�� ������ -1�� �Ѵ�. 
    float3 vPosition;
    vPosition.x = _Input.vUV.x * 2.0f - 1.0f;
    vPosition.y = _Input.vUV.y * -2.0f + 1.0f;
    vPosition.z = vDepth.x;

    // w�� �����ָ� w�� �����ֱ� ��.. �� �Ÿ��� ����(����) ���ٰ��� ����Ǳ� ������ ����� �� �� �ִ�.
    // ������� ��ü�δ� ���ٰ��� ���� �ȵȻ����̱� ����.
    // ������� z������xyz�� ������� ���ٰ��� ����� (���̴� 0~1)
    vPosition *= vDepth.w;
    
    // ��������� ������� ���ؼ� ��������� ��ȯ�Ѵ�. ( ������ ������ �ƴ� ��������� ó���ϱ�� �����ϱ�)
    vPosition = mul(float4(vPosition, vDepth.w), g_InvProj).xyz;

    float4 vNormalColor = g_GBufferNormal.Sample(g_PointSampler, _Input.vUV);
    float3 vNormal = vNormalColor.xyz;

    float4 vMaterial = g_GBufferMaterial.Sample(g_PointSampler, _Input.vUV);
    float4 vDiffuse = ConvertColor(vMaterial.r);
    float4 vAmbient = ConvertColor(vMaterial.g);
    float4 vSpeuclar = ConvertColor(vMaterial.b);
    float4 vEmissive = ConvertColor(vMaterial.a);

    float AO = 1.0f;

    if( 0 != g_bSSAO )
    {
        if (g_iLightType == LIGHT_DIR)
        {
            AO = g_AOTex.Sample(g_LinearSampler, _Input.vUV);
        }
    }

    LightInfo tInfo = ComputeLight(vPosition, vNormal,
            vDiffuse, vAmbient, vSpeuclar, vEmissive, vNormalColor.w, AO, _Input.vUV);

    output.vDiffuse = tInfo.vDiffuse + tInfo.vAmbient;
    output.vSpecular = tInfo.vSpecular + tInfo.vEmissve;            // ���� Sepcular�ȿ� Emissive���� ���ԵǾ����� 


    // ������Ʈ - �����׷�
    float LimRG = g_GBufferRG.Sample(g_LinearSampler, _Input.vUV).r;

    if (2 == LimRG)
    {
        float3 vViewPos = vDepth.yzw;
        float3 vCameraPos = float3(0.0, 0.0, 0.0);

        float3 fPos = normalize(vCameraPos - vViewPos);
        float fScala = dot(fPos, vNormal);

    // 90���� ��� �ܰ���
        if (fScala < 0.1)
        {
            float RimLightColor = smoothstep(1.0f - 0.5f, 1.0f, 1.0 - max(0, dot(vNormal, fPos)));
            RimLightColor *= 0.4f;
            output.vSpecular.xyz += float3(RimLightColor, RimLightColor, RimLightColor);
        }
    }

    // �Ȱ� ������ ���� ���� ���
    if (0 != g_bFog)
    {
        float3 vWorldPos = mul(float4(vPosition, 1.0f), g_vInvCamView).xyz;
        //float3 CamToPixel = vWorldPos - _vViewPosition;
        float3 CamToPixel = vWorldPos - g_CamPos;
        float fEyePos = g_CamPos.y;
        output.vDiffuse.xyz = ApplyFog(output.vDiffuse.xyz, fEyePos, CamToPixel);
        output.vSpecular.xyz = ApplyFog(output.vSpecular.xyz, fEyePos, CamToPixel);
    }

    return output;
}

// ���� ���� ����
PS_OUTPUT PS_LightBlend(VS_OUTPUT_UV _Input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;

    float4 vAlbedo = g_GBufferAlbedo.Sample(g_LinearSampler, _Input.vUV);
    float4 vDiffuse = g_LightDiffuse.Sample(g_LinearSampler, _Input.vUV);
    float4 vSpecular = g_LightSpecular.Sample(g_LinearSampler, _Input.vUV);

    if (0.0f == vAlbedo.a)
    {
        clip(-1);
    }
    
    // ���� vDiffuse�� diffuse�� ambient�� ���� ���� ����ִ�
    output.vColor = vAlbedo * vDiffuse + vSpecular;
    output.vColor.a = vAlbedo.a;

    return output;
}

// ����Ʈ ���� ����
PS_OUTPUT PS_LightBlendRender(VS_OUTPUT_UV _Input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;

    output.vColor = g_DiffuseTexture.Sample(g_LinearSampler, _Input.vUV);
    return output;

}