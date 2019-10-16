#include "Share.fx"

// 조명 누적을 담당할 셰이더파일
// 렌더매니저에 있는 GBuffer 텍스처를 받는다. 
// 조명처리에서 Albedo는 필요 없으므로 노말, 깊이, 재질만 받아와서 처리한다.
// NullBuffer로 출력을 할 것이다.
// NullBuffer는 출력 방식이 이미 정해져 있다.
// DirectX에서 nullBuffer를 출력한다고 하면 화면 전체크기를 그리는걸로 인식한다.
// 입력레이아웃 nullptr, 버텍스버퍼, 인덱스버퍼 또한 nullptr로 초기화해서 넘긴다.
// 방향성 조명을 계산해야한다.


// Gbuffer에 찍혀있는 걸 가지고 블렌드 등 연산을 처리할 거임
Texture2D g_GBufferAlbedo : register(t10);
Texture2D g_GBufferNormal : register(t11);
Texture2D g_GBufferDepth : register(t12);
Texture2D g_GBufferMaterial : register(t13);


Texture2D g_LightDiffuse : register(t14);
Texture2D g_LightSpecular : register(t15);


// 화면 전체크기
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

// 조명 누적할 구조체
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

    // w값으로 나누는 이유는 투영행렬만 곱하면 거리에따른 크기가 적용되지 않기때문
    output.vUV.x = (vPosition.x / vPosition.w) * 0.5f + 0.5f;       // -1.0 ~ 1.0f -> 0 ~ 1
    output.vUV.y = (vPosition.y / vPosition.w) * -0.5f + 0.5f;       // 0~1로 하기 위해 (-1 ~ 0)
  
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

    // w값으로 나누는 이유는 투영행렬만 곱하면 거리에따른 크기가 적용되지 않기때문
    output.vUV.x = (vPosition.x / vPosition.w) * 0.5f + 0.5f; // -1.0 ~ 1.0f -> 0 ~ 1
    output.vUV.y = (vPosition.y / vPosition.w) * -0.5f + 0.5f; // 0~1로 하기 위해 (-1 ~ 0)
  
    return output;
}


// 라이트 누적
PS_OUTPUT_LIGHTACC PS_LightAcc(VS_OUTPUT_UV _Input)
{
    PS_OUTPUT_LIGHTACC output = (PS_OUTPUT_LIGHTACC) 0;

    // 깊이값 가져오기
    float4 vDepth = g_GBufferDepth.Sample(g_LinearSampler, _Input.vUV);

    // 엄청 앞에 있으니까 필요 X
    if (0.0f == vDepth.w)
    {
        clip(-1);
    }

    // UV 좌표를 이용해서 w로 나눈 좌표를 구한다
    // UV좌표는 0~1  사이의 좌표값을 가지고 투영좌표는 -1~1사이의 좌표값을 가지니까
    // 동일하게 만들어주기 위해서 2를 곱한후 -1를 한다. 
    float3 vPosition;
    vPosition.x = _Input.vUV.x * 2.0f - 1.0f;
    vPosition.y = _Input.vUV.y * -2.0f + 1.0f;
    vPosition.z = vDepth.x;

    // w를 곱해주면 w로 나눠주기 전.. 즉 거리에 따른(깊이) 원근감이 적용되기 이전의 행렬을 알 수 있다.
    // 투영행렬 자체로는 원근감이 적용 안된상태이기 때문.
    // 뷰공간의 z값으로xyz를 나눠줘야 원근감이 적용됨 (깊이는 0~1)
    vPosition *= vDepth.w;
    
    // 투영행렬의 역행렬을 곱해서 뷰공간으로 변환한다. ( 조명을 투영이 아닌 뷰공간에서 처리하기로 했으니까)
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
    output.vSpecular = tInfo.vSpecular + tInfo.vEmissve;            // 현재 Sepcular안에 Emissive값이 포함되어있음 


    // 림라이트 - 렌더그룹
    float LimRG = g_GBufferRG.Sample(g_LinearSampler, _Input.vUV).r;

    if (2 == LimRG)
    {
        float3 vViewPos = vDepth.yzw;
        float3 vCameraPos = float3(0.0, 0.0, 0.0);

        float3 fPos = normalize(vCameraPos - vViewPos);
        float fScala = dot(fPos, vNormal);

    // 90도인 경우 외곽선
        if (fScala < 0.1)
        {
            float RimLightColor = smoothstep(1.0f - 0.5f, 1.0f, 1.0 - max(0, dot(vNormal, fPos)));
            RimLightColor *= 0.4f;
            output.vSpecular.xyz += float3(RimLightColor, RimLightColor, RimLightColor);
        }
    }

    // 안개 적용후 최종 색상 계산
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

// 조명에 블렌드 적용
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
    
    // 현재 vDiffuse는 diffuse랑 ambient가 더한 값이 들어있다
    output.vColor = vAlbedo * vDiffuse + vSpecular;
    output.vColor.a = vAlbedo.a;

    return output;
}

// 라이트 블렌드 렌더
PS_OUTPUT PS_LightBlendRender(VS_OUTPUT_UV _Input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;

    output.vColor = g_DiffuseTexture.Sample(g_LinearSampler, _Input.vUV);
    return output;

}