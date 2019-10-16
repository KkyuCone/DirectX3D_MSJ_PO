#include "Share.fx"
// 림라이트 적용할 상수버퍼
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

    output.vProjPos = mul(float4(_Input.vPos, 1.0f), g_WVP); // 투영공간에서의위치
    output.vPos = output.vProjPos;
    output.vUV = _Input.vUV;

    // 뷰공간의 위치를 구한다.
    output.vViewPos = mul(float4(_Input.vPos, 1.0f), g_WV).xyz;

    // 뷰공간의 Normal을 구한다.
    output.vNormal = normalize(mul(float4(_Input.vNormal, 0.0f), g_WVRot).xyz);

    // 머티리얼에 노말 값이 있다면 적용함
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

    // 디테일 맵관련 ( 확대했을때 픽셀이 깨지는 현상을 완화하기위한 것임 )
    // 디퓨즈맵에 디테일 맵을 곱하는 것은 텍스처 타일링으로 인해 발생하는 반복패턴을 숨길때 사용하는방법..
    // 우리는 반복패턴을 숨기려고 곱해줬음
    float2 vDetailUV = _Input.vUV;
    vDetailUV.x *= g_fDetailLevelX;
    vDetailUV.y *= g_fDetailLevelY;

    if (RENDER_FORWARD == g_iRenderMode)
    {
        // 포워드 렌더링
        LightInfo tInfo = ComputeLight(_Input.vViewPos.xyz, _Input.vNormal.xyz);

        float4 vColor = g_DiffuseTexture.Sample(g_LinearSampler, vDetailUV);

        // 스플래팅 컬러를 구한다.
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
        // 디퍼드렌더링 ( 조명은 나중에 처리하는 렌더링 나중에 한꺼번에 함)

        float4 vColor = g_DiffuseTexture.Sample(g_LinearSampler, vDetailUV);

        float3 vNormal = _Input.vNormal;

        float4 vNormalCol = g_NormalTexture.Sample(g_LinearSampler, vDetailUV);

        float4 vSpecularColor = g_vMaterialSpecurlar;

        // Sepcualr 텍스처 가 있다면 적용
        if (1 == g_iMaterialSpecular)
        {
            float4 vSpecular = g_SpecularTexture.Sample(g_LinearSampler, vDetailUV);
            vSpecular *= 0.5f;
            vSpecularColor.xyz = vSpecular.xyz;
        }

        	// 스플래팅 컬러를 구한다.
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
            _Input.vTangent, // X축 (접선)
           _Input.vBinormal, // Y축 (종법선)vNormalCol
           _Input.vNormal       // Z축
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



// 지형 디버깅용
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
