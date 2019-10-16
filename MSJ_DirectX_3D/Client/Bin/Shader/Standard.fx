#include "Share.fx"

// ������Ʈ ������ �������
cbuffer RGCBuffer : register(b4)
{
    int iRenderGroup;
    float3 Empty;
}


TextureCube g_CubeTexture : register(t10);

// Color�� �״�� �������ϴ� ���̴� ������
VS_OUTPUT_COLOR VS_StandardColor(VS_INPUT_COLOR _Input)
{
    VS_OUTPUT_COLOR output = (VS_OUTPUT_COLOR) 0;

    //output.vPos = float4(_Input.vPos, 1.0f);
    output.vPos = mul(float4(_Input.vPos, 1.0f), g_WVP);
    output.vColor = _Input.vColor;

    return output;
}

PS_OUTPUT PS_StandardColor(VS_OUTPUT_COLOR _Input)
{
    PS_OUTPUT output = (PS_OUTPUT)0;

    output.vColor = _Input.vColor;

    return output;
}

// ����Ʈ ������ ���̴�
VS_OUTPUT_NORMAL_COLOR VS_StandardNormalColor(VS_INPUT_NORMAL_COLOR _Input)
{
    VS_OUTPUT_NORMAL_COLOR output = (VS_OUTPUT_NORMAL_COLOR) 0;

    output.vProjPos = mul(float4(_Input.vPos, 1.0f), g_WVP);
    output.vPos = mul(float4(_Input.vPos, 1.0f), g_WVP);
    output.vColor = _Input.vColor;

    // ������� ��ġ�� ���ϱ�
    output.vViewPos = mul(float4(_Input.vPos, 1.0f), g_WV).xyz;
    // �� ���� Normal ���ϱ�
    output.vNormal = normalize(mul(float4(_Input.vNormal, 0.0f), g_WVRot).xyz);

    return output;
}

// �븻 �ؽ�ó ���� (���������� ���̴�)
VS_OUTPUT_3D VS_StandardTextureNormal(VS_INPUT_3D _Input)
{
    VS_OUTPUT_3D output = (VS_OUTPUT_3D) 0;

    output.vProjPos = mul(float4(_Input.vPos, 1.0f), g_WVP);        // �����������..
    output.vPos = output.vProjPos;
    output.vUV = _Input.vUV;

    // ������� ��ġ ���ϱ�
    output.vViewPos = mul(float4(_Input.vPos, 1.0f), g_WV).xyz;

    // ������� Normal�� ���Ѵ�.
    output.vNormal = normalize(mul(float4(_Input.vNormal, 0.0f), g_WV).xyz);
    
    return output;
}

// �⺻ �ؽ�ó ���� ���̴� 
VS_OUTPUT_3D VS_Standard3D(VS_INPUT_3D _Input)
{
    VS_OUTPUT_3D output = (VS_OUTPUT_3D) 0;

    // ��Ű��
    _tagSkinning tSkinning = (_tagSkinning) 0;

    tSkinning.vPos = _Input.vPos;
    tSkinning.vNormal = _Input.vNormal;
    tSkinning.vTangent = _Input.vTangent;
    tSkinning.vBinormal = _Input.vBinormal;

    // ���� �ִϸ��̼��� �����Ѵٸ� ��Ű���� �ؾ��Ѵ�.
    if (1 == g_iSkinning)
    {
        // �븻��(����)�� �����Ѵٸ�.. -> �������� ( Tangent, BiNormal �ʿ�)
        if ( 1 == g_iMaterialNormal)
        {
            tSkinning = Skinning(_Input.vPos, _Input.vNormal, _Input.vTangent, _Input.vBinormal, _Input.vWeights, _Input.vIndices);

        }
        else
        {
            // �������� X
            tSkinning = Skinning(_Input.vPos, _Input.vNormal, _Input.vWeights, _Input.vIndices);
        }

    }

    //float3 vPos = _Input.vPos;

    output.vProjPos = mul(float4(tSkinning.vPos, 1.0f), g_WVP);       // ����������������ġ (��Ű������)
    output.vPos = output.vProjPos;
    output.vUV = _Input.vUV;

    // ������� ��ġ�� ���Ѵ�.
    output.vViewPos = mul(float4(tSkinning.vPos, 1.0f), g_WV).xyz;

    // ������� Normal�� ���Ѵ�.
    output.vNormal = normalize(mul(float4(tSkinning.vNormal, 0.0f), g_WVRot).xyz);

    // ��Ƽ���� �븻 ���� �ִٸ� ������
    if( 1 == g_iMaterialNormal)
    {
        output.vTangent = normalize(mul(float4(tSkinning.vTangent, 0.0f), g_WVRot).xyz);
        output.vBinormal = normalize(mul(float4(tSkinning.vBinormal, 0.0f), g_WVRot).xyz);
    }

    return output;

}

// �ν��Ͻ�
VS_OUTPUT3D_INSTANCING VS_Standard3D_Instancing(VS_INPUT3D_INSTANCING _Input)
{
    VS_OUTPUT3D_INSTANCING output = (VS_OUTPUT3D_INSTANCING) 0;

    // ��Ű��
    _tagSkinning tSkinning = (_tagSkinning) 0;

    tSkinning.vPos = _Input.vPos;
    tSkinning.vNormal = _Input.vNormal;
    tSkinning.vTangent = _Input.vTangent;
    tSkinning.vBinormal = _Input.vBinormal;

    // ���� �ִϸ��̼��� �����Ѵٸ� ��Ű���� �ؾ��Ѵ�.
    if (1 == g_iSkinning)
    {
        // �븻��(����)�� �����Ѵٸ�.. -> �������� ( Tangent, BiNormal �ʿ�)
        if (1 == g_iMaterialNormal)
        {
            tSkinning = Skinning(_Input.vPos, _Input.vNormal, _Input.vTangent, _Input.vBinormal, _Input.vWeights, _Input.vIndices);

        }
        else
        {
            // �������� X
            tSkinning = Skinning(_Input.vPos, _Input.vNormal, _Input.vWeights, _Input.vIndices);
        }

    }

    //float3 vPos = _Input.vPos;

    output.vProjPos = mul(float4(tSkinning.vPos, 1.0f), _Input.matWVP); // ����������������ġ (��Ű������)
    output.vPos = output.vProjPos;
    output.vUV = _Input.vUV;

    // ������� ��ġ�� ���Ѵ�.
    output.vViewPos = mul(float4(tSkinning.vPos, 1.0f), _Input.matWV).xyz;

    // ������� Normal�� ���Ѵ�.
    output.vNormal = normalize(mul(float4(tSkinning.vNormal, 0.0f), _Input.matWVRot).xyz);

    // ��Ƽ���� �븻 ���� �ִٸ� ������
    if (1 == g_iMaterialNormal)
    {
        output.vTangent = normalize(mul(float4(tSkinning.vTangent, 0.0f), _Input.matWVRot).xyz);
        output.vBinormal = normalize(mul(float4(tSkinning.vBinormal, 0.0f), _Input.matWVRot).xyz);
    }

    output.matWVP = _Input.matWVP;
    output.matWV = _Input.matWV;
    output.matWVRot = _Input.matWVRot;

    return output;

}

///////////////////////////////////////////////////////////////////////////////// �ȼ� ���̴� �Լ�

// ȯ��� ���̴�
VS_OUTPUT_SKY VS_Sky(VS_INPUT_POSITION _Input)
{
    VS_OUTPUT_SKY output = (VS_OUTPUT_SKY) 0;
    output.vPos = mul(float4(_Input.vPos, 1.0f), g_WVP).xyww;
    output.vUV = _Input.vPos;

    return output;

}

PS_OUTPUT PS_Sky(VS_OUTPUT_SKY _Input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    output.vColor = g_CubeTexture.Sample(g_LinearSampler, _Input.vUV);
    return output;
}

PS_OUTPUT PS_Cylinder(VS_OUTPUT_SKY _Input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    output.vColor = float4(1.0f, 0.0f, 0.0f, 1.0f);
    return output;
}

PS_OUTPUT PS_Cone(VS_OUTPUT_SKY _Input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    output.vColor = g_CubeTexture.Sample(g_LinearSampler, _Input.vUV);
    return output;
}

PS_OUTPUT_GBUFFER PS_StandardNormalColor(VS_OUTPUT_NORMAL_COLOR _Input)
{
    PS_OUTPUT_GBUFFER output = (PS_OUTPUT_GBUFFER) 0;

    //float4 color;

    if (RENDER_FORWARD == g_iRenderMode)
    {
        // ������ ������
        LightInfo tInfo = ComputeLight(_Input.vViewPos.xyz, _Input.vNormal.xyz);
        output.vAlbedo = _Input.vColor * (tInfo.vDiffuse + tInfo.vAmbient) + tInfo.vSpecular;
    }
    else
    {
        // ���۵巻���� ( ������ ���߿� ó���ϴ� ������ ���߿� �Ѳ����� ��)
        output.vAlbedo = _Input.vColor;
        output.vNormal.xyz = _Input.vNormal;
        output.vNormal.w = g_vMaterialSpecurlar.w;
        output.vDepth.xyz = (float3) (_Input.vProjPos.z / _Input.vProjPos.w);
        output.vDepth.w = _Input.vProjPos.w;
        output.vDepth.yzw = _Input.vViewPos;
        output.vMaterial.r = ConvertColor(g_vMaterialDiffuse);
        output.vMaterial.g = ConvertColor(g_vMaterialAmbient);
        output.vMaterial.b = ConvertColor(g_vMaterialSpecurlar);
        output.vMaterial.a = ConvertColor(g_vMaterialEmissive);
    }

    output.vRenderGroup.rgba = iRenderGroup;

    return output;
}


PS_OUTPUT_GBUFFER PS_Standard3D(VS_OUTPUT_3D _Input)
{
    PS_OUTPUT_GBUFFER output = (PS_OUTPUT_GBUFFER) 0;

    if (RENDER_FORWARD == g_iRenderMode)
    {
        // ������ ������
        LightInfo tInfo = ComputeLight(_Input.vViewPos.xyz, _Input.vNormal.xyz);

        float4 vColor = g_DiffuseTexture.Sample(g_LinearSampler, _Input.vUV);

        output.vAlbedo = vColor * (tInfo.vDiffuse + tInfo.vAmbient) + tInfo.vSpecular;
        if (output.vAlbedo.a < 0.5)
        {
            clip(-1);
        }

    }
    else
    {
        // ���۵巻���� ( ������ ���߿� ó���ϴ� ������ ���߿� �Ѳ����� ��)

        float4 vColor = g_DiffuseTexture.Sample(g_LinearSampler, _Input.vUV);
        output.vAlbedo = vColor;

        if (output.vAlbedo.a < 0.5)
        {
            clip(-1);
        }

        float3 vNormal = _Input.vNormal;

        // Normal(�븻) �ؽ�ó (�븻)�� �ִٸ�
        if (1 == g_iMaterialNormal)
        {
            // ���÷��� ��ǻ��� ������ ����Ѵ�. Spec�� 
            float4 vNormalColor = g_NormalTexture.Sample(g_LinearSampler, _Input.vUV);
            vNormal = vNormalColor.xyz * 2.0f - 1.0f; // ������ -1~1������ ������ �ٲ۴�. (��������)

            float3x3 mat =
            {
                _Input.vTangent, // X�� (����)
               _Input.vBinormal, // Y�� (������)
               _Input.vNormal               // Z��
            };

            vNormal = normalize(mul(vNormal, mat)); // �� �������� ��ȯ��Ų��.

        }

        output.vNormal.xyz = vNormal;

        output.vNormal.w = g_vMaterialSpecurlar.w;
        output.vDepth.xyz = (float3) (_Input.vProjPos.z / _Input.vProjPos.w);
        output.vDepth.w = _Input.vProjPos.w;
        output.vDepth.yzw = _Input.vViewPos;

        float4 vSpecularColor = g_vMaterialSpecurlar;

        // Sepcualr �ؽ�ó �� �ִٸ� ����
        if (1 == g_iMaterialSpecular)
        {
            float4 vSpecular = g_SpecularTexture.Sample(g_LinearSampler, _Input.vUV);
            vSpecularColor.xyz = vSpecular.xyz;
        }

        output.vMaterial.r = ConvertColor(g_vMaterialDiffuse);
        output.vMaterial.g = ConvertColor(g_vMaterialAmbient);
        output.vMaterial.b = ConvertColor(vSpecularColor);
        output.vMaterial.a = ConvertColor(g_vMaterialEmissive);


        output.vRenderGroup.rgba = iRenderGroup;
    }

   return output;
}

PS_OUTPUT PS_Standard3D_Depth(VS_OUTPUT_3D _Input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    float Depth = _Input.vProjPos.z / _Input.vProjPos.w;

    //Depth *= 10.0;
    //output.vColor = float4(Depth, Depth, g_fStaticShadowBias, 1.0f);
    //output.vColor.w = _Input.vProjPos.w;
    //output.vColor.yzw = _Input.vViewPos;

    return output;
}

PS_O_OUTPUT PS_Dynamic_Depth(VS_OUTPUT_3D _Input)
{
	PS_O_OUTPUT output = (PS_O_OUTPUT)0.0f;

    float4 vColor = g_DiffuseTexture.Sample(g_LinearSampler, _Input.vUV);

    if (vColor.a < 0.5)
    {
        clip(-1);
    }

    float Depth = _Input.vProjPos.z / _Input.vProjPos.w;
    output.vOColor = Depth;

    return output;
}

PS_OUTPUT_GBUFFER PS_Standard3D_Instancing(VS_OUTPUT3D_INSTANCING _Input)
{
    PS_OUTPUT_GBUFFER output = (PS_OUTPUT_GBUFFER) 0;

    if (RENDER_FORWARD == g_iRenderMode)
    {
        // ������ ������
        LightInfo tInfo = ComputeLight(_Input.vViewPos.xyz, _Input.vNormal.xyz);

        float4 vColor = g_DiffuseTexture.Sample(g_LinearSampler, _Input.vUV);

        output.vAlbedo = vColor * (tInfo.vDiffuse + tInfo.vAmbient) + tInfo.vSpecular;

        if (output.vAlbedo.a < 0.5)
        {
            clip(-1);
        }
    }
    else
    {
        // ���۵巻���� ( ������ ���߿� ó���ϴ� ������ ���߿� �Ѳ����� ��)

        float4 vColor = g_DiffuseTexture.Sample(g_LinearSampler, _Input.vUV);
        output.vAlbedo = vColor;

        if (output.vAlbedo.a < 0.5)
        {
            clip(-1);
        }

        float3 vNormal = _Input.vNormal;

        // Normal(�븻) �ؽ�ó (�븻)�� �ִٸ�
        if (1 == g_iMaterialNormal)
        {
            // ���÷��� ��ǻ��� ������ ����Ѵ�. Spec�� 
            float4 vNormalColor = g_NormalTexture.Sample(g_LinearSampler, _Input.vUV);
            vNormal = vNormalColor.xyz * 2.0f - 1.0f; // ������ -1~1������ ������ �ٲ۴�. (��������)

            float3x3 mat =
            {
                _Input.vTangent, // X�� (����)
               _Input.vBinormal, // Y�� (������)
               _Input.vNormal               // Z��
            };

            vNormal = normalize(mul(vNormal, mat)); // �� �������� ��ȯ��Ų��.

        }

        output.vNormal.xyz = vNormal;

        output.vNormal.w = g_vMaterialSpecurlar.w;
        output.vDepth.xyz = (float3) (_Input.vProjPos.z / _Input.vProjPos.w);
        output.vDepth.w = _Input.vProjPos.w;
        output.vDepth.yzw = _Input.vViewPos;

        float4 vSpecularColor = g_vMaterialSpecurlar;

        // Sepcualr �ؽ�ó �� �ִٸ� ����
        if (1 == g_iMaterialSpecular)
        {
            float4 vSpecular = g_SpecularTexture.Sample(g_LinearSampler, _Input.vUV);
            vSpecularColor.xyz = vSpecular.xyz;
        }

        output.vMaterial.r = ConvertColor(g_vMaterialDiffuse);
        output.vMaterial.g = ConvertColor(g_vMaterialAmbient);
        output.vMaterial.b = ConvertColor(vSpecularColor);
        output.vMaterial.a = ConvertColor(g_vMaterialEmissive);
    }

    output.vRenderGroup.rgba = iRenderGroup;

    return output;
}

PS_OUTPUT PS_Standard3D_Instancing_Depth(VS_OUTPUT3D_INSTANCING _Input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    float4 vColor = g_DiffuseTexture.Sample(g_LinearSampler, _Input.vUV);
    if (vColor.a < 0.5)
    {
        clip(-1);
    }
    float Depth = _Input.vProjPos.z / _Input.vProjPos.w;

    output.vColor = float4(Depth, Depth, Depth, 1.0f);
    return output;
}



