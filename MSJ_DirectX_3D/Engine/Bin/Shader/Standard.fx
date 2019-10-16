#include "Share.fx"

// 림라이트 적용할 상수버퍼
cbuffer RGCBuffer : register(b4)
{
    int iRenderGroup;
    float3 Empty;
}


TextureCube g_CubeTexture : register(t10);

// Color값 그대로 나오게하는 셰이더 ㅇㅅㅇ
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

// 라이트 적용한 셰이더
VS_OUTPUT_NORMAL_COLOR VS_StandardNormalColor(VS_INPUT_NORMAL_COLOR _Input)
{
    VS_OUTPUT_NORMAL_COLOR output = (VS_OUTPUT_NORMAL_COLOR) 0;

    output.vProjPos = mul(float4(_Input.vPos, 1.0f), g_WVP);
    output.vPos = mul(float4(_Input.vPos, 1.0f), g_WVP);
    output.vColor = _Input.vColor;

    // 뷰공간의 위치를 구하기
    output.vViewPos = mul(float4(_Input.vPos, 1.0f), g_WV).xyz;
    // 뷰 공간 Normal 구하기
    output.vNormal = normalize(mul(float4(_Input.vNormal, 0.0f), g_WVRot).xyz);

    return output;
}

// 노말 텍스처 적용 (범프적용할 셰이더)
VS_OUTPUT_3D VS_StandardTextureNormal(VS_INPUT_3D _Input)
{
    VS_OUTPUT_3D output = (VS_OUTPUT_3D) 0;

    output.vProjPos = mul(float4(_Input.vPos, 1.0f), g_WVP);        // 투영공간긍로..
    output.vPos = output.vProjPos;
    output.vUV = _Input.vUV;

    // 뷰공간의 위치 구하기
    output.vViewPos = mul(float4(_Input.vPos, 1.0f), g_WV).xyz;

    // 뷰공간의 Normal을 구한다.
    output.vNormal = normalize(mul(float4(_Input.vNormal, 0.0f), g_WV).xyz);
    
    return output;
}

// 기본 텍스처 적용 셰이더 
VS_OUTPUT_3D VS_Standard3D(VS_INPUT_3D _Input)
{
    VS_OUTPUT_3D output = (VS_OUTPUT_3D) 0;

    // 스키닝
    _tagSkinning tSkinning = (_tagSkinning) 0;

    tSkinning.vPos = _Input.vPos;
    tSkinning.vNormal = _Input.vNormal;
    tSkinning.vTangent = _Input.vTangent;
    tSkinning.vBinormal = _Input.vBinormal;

    // 만약 애니메이션이 존재한다면 스키닝을 해야한다.
    if (1 == g_iSkinning)
    {
        // 노말맵(범프)이 존재한다면.. -> 범프적용 ( Tangent, BiNormal 필요)
        if ( 1 == g_iMaterialNormal)
        {
            tSkinning = Skinning(_Input.vPos, _Input.vNormal, _Input.vTangent, _Input.vBinormal, _Input.vWeights, _Input.vIndices);

        }
        else
        {
            // 범프적용 X
            tSkinning = Skinning(_Input.vPos, _Input.vNormal, _Input.vWeights, _Input.vIndices);
        }

    }

    //float3 vPos = _Input.vPos;

    output.vProjPos = mul(float4(tSkinning.vPos, 1.0f), g_WVP);       // 투영공간에서의위치 (스키닝적용)
    output.vPos = output.vProjPos;
    output.vUV = _Input.vUV;

    // 뷰공간의 위치를 구한다.
    output.vViewPos = mul(float4(tSkinning.vPos, 1.0f), g_WV).xyz;

    // 뷰공간의 Normal을 구한다.
    output.vNormal = normalize(mul(float4(tSkinning.vNormal, 0.0f), g_WVRot).xyz);

    // 머티리얼에 노말 값이 있다면 적용함
    if( 1 == g_iMaterialNormal)
    {
        output.vTangent = normalize(mul(float4(tSkinning.vTangent, 0.0f), g_WVRot).xyz);
        output.vBinormal = normalize(mul(float4(tSkinning.vBinormal, 0.0f), g_WVRot).xyz);
    }

    return output;

}

// 인스턴싱
VS_OUTPUT3D_INSTANCING VS_Standard3D_Instancing(VS_INPUT3D_INSTANCING _Input)
{
    VS_OUTPUT3D_INSTANCING output = (VS_OUTPUT3D_INSTANCING) 0;

    // 스키닝
    _tagSkinning tSkinning = (_tagSkinning) 0;

    tSkinning.vPos = _Input.vPos;
    tSkinning.vNormal = _Input.vNormal;
    tSkinning.vTangent = _Input.vTangent;
    tSkinning.vBinormal = _Input.vBinormal;

    // 만약 애니메이션이 존재한다면 스키닝을 해야한다.
    if (1 == g_iSkinning)
    {
        // 노말맵(범프)이 존재한다면.. -> 범프적용 ( Tangent, BiNormal 필요)
        if (1 == g_iMaterialNormal)
        {
            tSkinning = Skinning(_Input.vPos, _Input.vNormal, _Input.vTangent, _Input.vBinormal, _Input.vWeights, _Input.vIndices);

        }
        else
        {
            // 범프적용 X
            tSkinning = Skinning(_Input.vPos, _Input.vNormal, _Input.vWeights, _Input.vIndices);
        }

    }

    //float3 vPos = _Input.vPos;

    output.vProjPos = mul(float4(tSkinning.vPos, 1.0f), _Input.matWVP); // 투영공간에서의위치 (스키닝적용)
    output.vPos = output.vProjPos;
    output.vUV = _Input.vUV;

    // 뷰공간의 위치를 구한다.
    output.vViewPos = mul(float4(tSkinning.vPos, 1.0f), _Input.matWV).xyz;

    // 뷰공간의 Normal을 구한다.
    output.vNormal = normalize(mul(float4(tSkinning.vNormal, 0.0f), _Input.matWVRot).xyz);

    // 머티리얼에 노말 값이 있다면 적용함
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

///////////////////////////////////////////////////////////////////////////////// 픽셀 셰이더 함수

// 환경맵 셰이더
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
        // 포워드 렌더링
        LightInfo tInfo = ComputeLight(_Input.vViewPos.xyz, _Input.vNormal.xyz);
        output.vAlbedo = _Input.vColor * (tInfo.vDiffuse + tInfo.vAmbient) + tInfo.vSpecular;
    }
    else
    {
        // 디퍼드렌더링 ( 조명은 나중에 처리하는 렌더링 나중에 한꺼번에 함)
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
        // 포워드 렌더링
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
        // 디퍼드렌더링 ( 조명은 나중에 처리하는 렌더링 나중에 한꺼번에 함)

        float4 vColor = g_DiffuseTexture.Sample(g_LinearSampler, _Input.vUV);
        output.vAlbedo = vColor;

        if (output.vAlbedo.a < 0.5)
        {
            clip(-1);
        }

        float3 vNormal = _Input.vNormal;

        // Normal(노말) 텍스처 (노말)이 있다면
        if (1 == g_iMaterialNormal)
        {
            // 샘플러는 디퓨즈와 같은거 사용한다. Spec도 
            float4 vNormalColor = g_NormalTexture.Sample(g_LinearSampler, _Input.vUV);
            vNormal = vNormalColor.xyz * 2.0f - 1.0f; // 공간을 -1~1사이의 값으로 바꾼다. (단위벡터)

            float3x3 mat =
            {
                _Input.vTangent, // X축 (접선)
               _Input.vBinormal, // Y축 (종법선)
               _Input.vNormal               // Z축
            };

            vNormal = normalize(mul(vNormal, mat)); // 뷰 공간으로 변환시킨다.

        }

        output.vNormal.xyz = vNormal;

        output.vNormal.w = g_vMaterialSpecurlar.w;
        output.vDepth.xyz = (float3) (_Input.vProjPos.z / _Input.vProjPos.w);
        output.vDepth.w = _Input.vProjPos.w;
        output.vDepth.yzw = _Input.vViewPos;

        float4 vSpecularColor = g_vMaterialSpecurlar;

        // Sepcualr 텍스처 가 있다면 적용
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
        // 포워드 렌더링
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
        // 디퍼드렌더링 ( 조명은 나중에 처리하는 렌더링 나중에 한꺼번에 함)

        float4 vColor = g_DiffuseTexture.Sample(g_LinearSampler, _Input.vUV);
        output.vAlbedo = vColor;

        if (output.vAlbedo.a < 0.5)
        {
            clip(-1);
        }

        float3 vNormal = _Input.vNormal;

        // Normal(노말) 텍스처 (노말)이 있다면
        if (1 == g_iMaterialNormal)
        {
            // 샘플러는 디퓨즈와 같은거 사용한다. Spec도 
            float4 vNormalColor = g_NormalTexture.Sample(g_LinearSampler, _Input.vUV);
            vNormal = vNormalColor.xyz * 2.0f - 1.0f; // 공간을 -1~1사이의 값으로 바꾼다. (단위벡터)

            float3x3 mat =
            {
                _Input.vTangent, // X축 (접선)
               _Input.vBinormal, // Y축 (종법선)
               _Input.vNormal               // Z축
            };

            vNormal = normalize(mul(vNormal, mat)); // 뷰 공간으로 변환시킨다.

        }

        output.vNormal.xyz = vNormal;

        output.vNormal.w = g_vMaterialSpecurlar.w;
        output.vDepth.xyz = (float3) (_Input.vProjPos.z / _Input.vProjPos.w);
        output.vDepth.w = _Input.vProjPos.w;
        output.vDepth.yzw = _Input.vViewPos;

        float4 vSpecularColor = g_vMaterialSpecurlar;

        // Sepcualr 텍스처 가 있다면 적용
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



