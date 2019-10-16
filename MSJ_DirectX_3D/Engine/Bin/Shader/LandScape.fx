#include "Share.fx"
// ������Ʈ ������ �������
cbuffer LimLight : register(b4)
{
    uint LimLight;
    float3 Empty;
}

cbuffer LandScape : register(b10)
{
    float g_fDetailLevelX;
    float g_fDetailLevelY;
    int g_iSplatCount;
    float g_fLandScapeEmpty;
}

Texture2DArray g_DiffuseSplatTex : register(t10);
Texture2DArray g_NormalSplatTex : register(t11);
Texture2DArray g_SpecularSplatTex : register(t12);
Texture2DArray g_AlphaSplatTex : register(t13);

VS_OUTPUT_3D VS_LandScape(VS_INPUT_3D _Input)
{
    VS_OUTPUT_3D output = (VS_OUTPUT_3D) 0;

    output.vProjPos = mul(float4(_Input.vPos, 1.0f), g_WVP); // ����������������ġ
    output.vPos = output.vProjPos;
    output.vUV = _Input.vUV;

    // ������� ��ġ�� ���Ѵ�.
    output.vViewPos = mul(float4(_Input.vPos, 1.0f), g_WV).xyz;

    // ������� Normal�� ���Ѵ�.
    output.vNormal = normalize(mul(float4(_Input.vNormal, 0.0f), g_WVRot).xyz);

    // ��Ƽ���� �븻 ���� �ִٸ� ������
    if (1 == g_iMaterialNormal)
    {
        output.vTangent = normalize(mul(float4(_Input.vTangent, 0.0f), g_WVRot).xyz);
        output.vBinormal = normalize(mul(float4(_Input.vBinormal, 0.0f), g_WVRot).xyz);
    }

    return output;

}

PS_OUTPUT_GBUFFER PS_LandScape(VS_OUTPUT_3D _Input)
{
    PS_OUTPUT_GBUFFER output = (PS_OUTPUT_GBUFFER) 0;

    // ������ �ʰ��� ( Ȯ�������� �ȼ��� ������ ������ ��ȭ�ϱ����� ���� )
    // ��ǻ��ʿ� ������ ���� ���ϴ� ���� �ؽ�ó Ÿ�ϸ����� ���� �߻��ϴ� �ݺ������� ���涧 ����ϴ¹��..
    // �츮�� �ݺ������� ������� ��������
    float2 vDetailUV = _Input.vUV;
    vDetailUV.x *= g_fDetailLevelX;
    vDetailUV.y *= g_fDetailLevelY;

    if (RENDER_FORWARD == g_iRenderMode)
    {
        // ������ ������
        LightInfo tInfo = ComputeLight(_Input.vViewPos.xyz, _Input.vNormal.xyz);

        float4 vColor = g_DiffuseTexture.Sample(g_LinearSampler, vDetailUV);

        // ���÷��� �÷��� ���Ѵ�.
        for (int i = 0; i < g_iSplatCount; ++i)
        {
            float4 vAlpha = g_AlphaSplatTex.Sample(g_LinearSampler, float3(_Input.vUV, i));
            float4 vSplatDif = g_DiffuseSplatTex.Sample(g_LinearSampler, float3(vDetailUV, i));

            vColor = vColor * (1.f - vAlpha.r) + vSplatDif * vAlpha.r;
        }

        output.vAlbedo = vColor * (tInfo.vDiffuse + tInfo.vAmbient) + tInfo.vSpecular;
    }
    else
    {
        // ���۵巻���� ( ������ ���߿� ó���ϴ� ������ ���߿� �Ѳ����� ��)

        float4 vColor = g_DiffuseTexture.Sample(g_LinearSampler, vDetailUV);

        float3 vNormal = _Input.vNormal;

        float4 vNormalCol = g_NormalTexture.Sample(g_LinearSampler, vDetailUV);

        float4 vSpecularColor = g_vMaterialSpecurlar;

        // Sepcualr �ؽ�ó �� �ִٸ� ����
        if (1 == g_iMaterialSpecular)
        {
            float4 vSpecular = g_SpecularTexture.Sample(g_LinearSampler, vDetailUV);
            vSpecular *= 0.5f;
            vSpecularColor.xyz = vSpecular.xyz;
        }

        	// ���÷��� �÷��� ���Ѵ�.
        for (int i = 0; i < g_iSplatCount; ++i)
        {
            float4 vAlpha = g_AlphaSplatTex.Sample(g_LinearSampler, float3(_Input.vUV, i));
            float4 vSplatDif = g_DiffuseSplatTex.Sample(g_LinearSampler, float3(vDetailUV, i));
            float4 vSplatNrm = g_NormalSplatTex.Sample(g_LinearSampler, float3(vDetailUV, i));
            float4 vSplatSpc = g_SpecularSplatTex.Sample(g_LinearSampler, float3(vDetailUV, i));
            vSplatSpc.xyz = 0.0f;

            vColor = (vColor * (1.f - vAlpha.r)) + (vSplatDif * vAlpha.r);
            vNormalCol = (vNormalCol * (1.f - vAlpha.r)) + (vSplatNrm * vAlpha.r);
            vSpecularColor.xyz = (vSpecularColor.xyz * (1.f - vAlpha.r)) + (vSplatSpc.xyz * vAlpha.r);
        }

        vNormal = vNormalCol.xyz * 2.0f - 1.0f;

        float3x3 mat =
        {
            _Input.vTangent, // X�� (����)
           _Input.vBinormal, // Y�� (������)vNormalCol
           _Input.vNormal       // Z��
        };

        vNormal = normalize(mul(vNormal, mat));

       
        output.vAlbedo = vColor;
        output.vNormal.xyz = vNormal;
        output.vNormal.w = g_vMaterialSpecurlar.w;
        output.vDepth.xyz = (float3) (_Input.vProjPos.z / _Input.vProjPos.w);
        output.vDepth.w = _Input.vProjPos.w;


        output.vMaterial.r = ConvertColor(g_vMaterialDiffuse);
        output.vMaterial.g = ConvertColor(g_vMaterialAmbient);
        output.vMaterial.b = ConvertColor(vSpecularColor);
        output.vMaterial.a = ConvertColor(g_vMaterialEmissive);

        output.vRenderGroup.rgba = LimLight;

    }

    return output;
}



// ���� ������
VS_OUTPUT_COLOR VS_Debug_LandScape(VS_INPUT_COLOR _Input)
{
    VS_OUTPUT_COLOR output = (VS_OUTPUT_COLOR) 0;

    output.vPos = mul(float4(_Input.vPos, 1.0f), g_WVP);
    output.vColor = _Input.vColor;
    
    return output;
}

PS_OUTPUT PS_Debug_LandScape(VS_OUTPUT_COLOR _Input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    output.vColor = _Input.vColor;
    return output;
}
