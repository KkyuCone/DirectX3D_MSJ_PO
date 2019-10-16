

/////////////////////////////////////////////////////////// ���ؽ����̴�
// ����, ��ġ�� ( �Է�, ���)
struct VS_INPUT_COLOR
{
    float3 vPos : POSITION;
    float4 vColor : COLOR;
};

struct VS_OUTPUT_COLOR
{
    float4 vPos : SV_POSITION;
    float4 vColor : COLOR;
};

// ��ġ, ���, ���� (�Է�, ���)
struct VS_INPUT_NORMAL_COLOR
{
    float3 vPos     : POSITION;
    float3 vNormal  : NORMAL;
    float4 vColor   : COLOR;
};

struct VS_OUTPUT_NORMAL_COLOR
{
    float4 vPos         : SV_POSITION;
    float3 vNormal      : NORMAL;
    float4 vColor       : COLOR;
    float3 vViewPos     : POSITION;             // ������� z���� w���� �����Ƿ� w �׳� �ϳ� ����� ������
    float4 vProjPos     : POSITION1;
};

struct VS_INPUT_3D
{
    float3 vPos     : POSITION;
    float3 vNormal  : NORMAL;
    float2 vUV      : TEXCOORD;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float4 vWeights : BLENDWEIGHTS;
    float4 vIndices : BLENDINDICES;
};

struct VS_OUTPUT_3D
{
    float4 vPos : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vUV : TEXCOORD;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float3 vViewPos : POSITION;
    float4 vProjPos : POSITION1;
};

struct VS_INPUT3D_INSTANCING
{
    float3 vPos         : POSITION;
    float3 vNormal      : NORMAL;
    float2 vUV          : TEXCOORD;
    float3 vTangent     : TANGENT;
    float3 vBinormal    : BINORMAL;
    float4 vWeights     : BLENDWEIGHTS;
    float4 vIndices     : BLENDINDICES;
    matrix matWVP       : WORLD;
    matrix matWV        : WORLDVIEW;
    matrix matWVRot     : WORLDVIEWROT;
};

struct VS_OUTPUT3D_INSTANCING
{
    float4 vPos         : SV_POSITION;
    float3 vNormal      : NORMAL;
    float2 vUV          : TEXCOORD;
    float3 vTangent     : TANGENT;
    float3 vBinormal    : BINORMAL;
    float3 vViewPos     : POSITION;
    float4 vProjPos     : POSITION1;
    matrix matWVP       : WORLD;
    matrix matWV        : WORLDVIEW;
    matrix matWVRot     : WORLDVIEWROT;
};

struct VS_INPUT_POSITION
{
    float3 vPos : POSITION;
};

struct VS_OUTPUT_POSITION
{
    float4 vPos : SV_POSITION;
};

//

struct VS_OUTPUT_SKY
{
    float4 vPos : SV_POSITION;
    float3 vUV : TEXCOORD;
};

struct VS_INPUT_UV
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUTPUT_UV
{
    float4 vPos : SV_POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUTPUT_WORLDBAR
{
	float4 vPos : SV_POSITION;
	float4 vProjPos : POSITION;             // �������� ��ġ
	float2 vUV : TEXCOORD;
};

// �ν��Ͻ̿�
struct VS_INPUT_UV_INSTANCINGE
{
    float3 vPos     : POSITION;
    float2 vUV      : TEXCOORD;
    matrix matWVP   : WORLD;
    matrix matWV    : WORLDVIEW;
    matrix matWVRot : WORLDVIEWROT;
};

struct VS_OUTPUT_UV_INSTANCING
{
    float4 vPos     : SV_POSITION;
    float2 vUV      : TEXCOORD;
    matrix matWVP   : WORLD;
    matrix matWV    : WORLDVIEW;
    matrix matWVRot : WORLDVIEWROT;
};


/////////////////////////////////////////////////////////////// �ȼ����̴�

struct PS_OUTPUT
{
    float4 vColor : SV_TARGET;
};

struct PS_O_OUTPUT
{
	float vOColor : SV_TARGET;
};

struct PS_OUTPUT_GBUFFER
{
    float4 vAlbedo      : SV_TARGET;
    float4 vNormal      : SV_TARGET1;
    float4 vDepth       : SV_TARGET2;
    float4 vMaterial    : SV_TARGET3;
    float4 vRenderGroup : SV_TARGET4;
};


/////////////////////////////////////////////////////////////// �������

cbuffer Transform : register(b0)
{
    matrix g_WRot;    // WorldRotationMatrix;	// ���� ȸ����� 
    matrix g_WVRot;   // WorldViewRotation
    matrix g_W;
    matrix g_V;
    matrix g_InvV;
    matrix g_Proj;
    matrix g_InvProj;
    matrix g_WV;
    matrix g_WVP;
    matrix g_VP;        // ������ ���� �ʿ� 
}

cbuffer Material : register(b1)
{
    float4  g_vMaterialDiffuse;
    float4  g_vMaterialAmbient;
    float4  g_vMaterialSpecurlar;
    float4  g_vMaterialEmissive;
    int     g_iMaterialNormal;
    int     g_iMaterialSpecular;
    int     g_iSkinning;                    // �ִϸ��̼� ���� (1, 0)
    float   g_vMaterialEmpty;
}

cbuffer Light : register(b2)
{
    matrix g_vShadowLightVP;
    matrix g_vInvCamView;
    float4  g_vLightDiffuse;
    float4  g_vLightAmbient;
    float4  g_vLightSpecualr;
    float3  g_vLightPosition;
    int     g_iLightType;
    float3  g_vLightDir;
    float   g_fLightDistance;
    float   g_fLightInAngle;
    float   g_fLightOutAngle;
	float	g_Bias;
    float  g_vLightEmtpy;
}

// �������
#define RENDER_FORWARD  0
#define RENDER_DEFERRED 1

cbuffer Rendering : register(b3)
{
    int     g_iRenderMode;
    int     g_bSSAO;
    int     g_bShadow;
    int     g_bPostEffect;
    int     g_bFog;
    int     g_bRain;
    float2 vRenEmpty;
}

// 2D �ִϸ��̼� ���� - ComputeFrameUV(float2 _vUV)
#define AFT_ATLAS   0
#define AFT_FRAME   1

cbuffer AnimationFrame : register(b8)
{
    int     g_iAniFrameType;
    int     g_iAniFrameOption;
    float2  g_vAniFrameTextureSize;
    float2  g_vAniFrameStart;
    float2  g_vAniFrameEnd;
    int     g_iAniFrameFrame;
    float3  g_vAniFrameEmpty;
}

cbuffer DepthFog : register(b5)
{
    float3  g_FogColor; // �Ȱ� ����
    float   g_FogStartDepth; // �Ȱ� ���� ����
    float3  g_FogHightLightColor; // �Ȱ� ���� ���� ����
    float   g_FogGlobalDensity; // ��ü���� ����..?����
    float3  g_FogSunDir; // �Ȱ� �� ����
    float   g_FogHeightFalloff;
    float3  g_CamPos;
    float   fEmpty;
}


//////////////////////////////////////////////////////////

Texture2D g_DiffuseTexture  : register(t0);
Texture2D g_NormalTexture   : register(t1);
Texture2D g_SpecularTexture : register(t2);
Texture2D g_BoneTexture     : register(t3);         // ��Ű���� ���� �ִϸ��̼� ����� �������� VTF������� �־��ٰ���
Texture2D g_GBufferRG		: register(t6);

Texture2D<float> g_AOTex : register(t7);            // Ambient Occlusion Texture
Texture2D<float> g_ShadowTex : register(t9);               // �׸��ڸ�

SamplerState g_LinearSampler    : register(s0);      // ���� ���÷�
SamplerState g_PointSampler     : register(s1);      // ����Ʈ ���÷�
SamplerState g_WrapSampler      : register(s2);     // Wrap���÷�
SamplerState g_ClampSampler     : register(s3);     // Ŭ���� ���÷�
SamplerState g_ShadowSampler	: register(s4); // ������ ���÷�

SamplerComparisonState g_PCFSmp	: register(s5);

VS_OUTPUT_COLOR VS_StandardVertex3D(VS_INPUT_3D _Input)
{
    VS_OUTPUT_COLOR Output = (VS_OUTPUT_COLOR) 0;

    return Output;
}


PS_OUTPUT PS_StandardVertex3D(VS_OUTPUT_COLOR _Input)
{
    PS_OUTPUT Output = (PS_OUTPUT) 0;
    return Output;

}

// ����Ʈ ������ ����Ʈ ���(Compute Light)�Լ�

struct LightInfo
{
    float4 vDiffuse;
    float4 vAmbient;
    float4 vSpecular;
    float4 vEmissve;
};

#define LIGHT_DIR 0
#define LIGHT_POINT 1
#define LIGHT_SPOT 2


LightInfo ComputeLight(float3 _vViewPosition, float3 _vViewNormal)
{
    // ��������� ����Ʈ�� ������ٰ���
    LightInfo tInfo = (LightInfo) 0;

    float3 vLightDir = float3(0.0f, 0.0f, 0.0f);
    float fIntensity = 1.0f;                        // ���� ����

    // �ش� ������ Directional�� ���
    if (g_iLightType == LIGHT_DIR)
    {
        // ��������� ��������� ��ȯ�Ѵ�.
        // ��������� �������� ���ؾ��Ѵ�. ( �׷��� ������ ����)
        //vLightDir = -normalize(mul(float4(g_vLightDir, 0.0f), g_V).xyz);
        vLightDir = g_vLightDir;

    }

    // POINT ����
    if (g_iLightType == LIGHT_POINT)
    {
        // �������� ������ ���ϴ� ���͸� �����ֱ�
        vLightDir = g_vLightPosition - _vViewPosition;
        vLightDir = normalize(vLightDir);

        // ����(��ü�� ����)�� ������ �Ÿ��� �����ֱ�
        float fDistance = distance(g_vLightPosition, _vViewPosition);

        if (fDistance > g_fLightDistance)
        {
            // ����Ʈ�� �Ѱ�Ÿ��� ������
            fIntensity = 0.0f;
        }
        else
        {
            //fIntensity = 1.0f - fDistance / g_fLightDistance;

            // ���� ������ 0.3~0.7���� ����
            fIntensity = (1.0f - fDistance / g_fLightDistance) * 0.7f + 0.3f;
        }
    }

    // SPOT ����
    if (g_iLightType == LIGHT_SPOT)
    {
        // �������� ������ ���ϴ� ���� ���ϱ� (�����), _vViewPosition(����, �ٶ󺸴� ��ġ)
        vLightDir = g_vLightPosition - _vViewPosition;
        vLightDir = normalize(vLightDir);

        // ������ ������ �Ÿ� ���ϱ�
        float fDistance = distance(g_vLightPosition, _vViewPosition);

        if(fDistance > g_fLightDistance)
        {
            fIntensity = 0.0f;
        }
        else
        {
            // �ٱ��� �ݰ� �ȿ� ��� �Դ��� �Ǵ��ϱ�
            float3 vDir = -vLightDir;

            //  vDir�� ������ ���ߴ� ���͸� ���ؼ� ������  �ݰ� �ȿ� �������� �Ǵ��ϱ�
            // g_vLightDir �������� �� �־��ĸ� ��������ʿ��� -1.0f�� ���߱⶧��
            float fDot = dot(vDir, g_vLightDir);

            if (fDot < g_fLightOutAngle)
            {
                // �ٱ��� �ݰ��� ������
                fIntensity = 0.0f;
            }
            else if(fDot >= g_fLightInAngle)
            {
                // ���� �ݰ濡 �ִ��� �Ǵ� ( �ٱ��ʺ��� ���� ������ ���� )
                fIntensity = 1.0f;
            }
            else
            {
                //�ٱ��� �ݰ濡 �ִ� ���
                fIntensity = (g_fLightInAngle - fDot) / (g_fLightInAngle - g_fLightOutAngle);

            }

            // ���� �ε巴�� ���ֱ�(���κ��� �ʹ� �ѷ��ϰ� -��- ��..�׷��� �ε巴������)
            fIntensity = fIntensity * 0.3f + 0.7f;

            // �Ÿ��� ���� ���� ����
            float fDistanceIntensity = (1.0f - fDistance / g_fLightDistance) * 0.3f + 0.7f;

            // �� ����
            fIntensity *= fDistanceIntensity;
        }
    }

    tInfo.vDiffuse = g_vLightDiffuse * g_vMaterialDiffuse * max(0, dot(_vViewNormal, vLightDir)) * fIntensity;
    tInfo.vAmbient = g_vLightAmbient * g_vMaterialAmbient;

    // �ݻ纤�� ���ϱ�
    // �ݻ纤�� r�� r = I(�Ի纤��, ������ �����¹���) - 2(dot(n(�븻����), I))n�ε� ���� ���ö� ���������� ������ͼ� 
    // r = 2(n����I) - I�� 
    float3 vReflect = 2.0f * dot(_vViewNormal, vLightDir) * _vViewNormal - vLightDir;
    vReflect = normalize(vReflect);

    // View ���͸� ���Ѵ�. ( ��������� ó���ҷ���)
    float3 vView = -normalize(_vViewPosition);

    tInfo.vSpecular = g_vLightSpecualr * g_vMaterialSpecurlar * pow(max(0, dot(vReflect, vView)), g_vMaterialSpecurlar.w) * fIntensity;

    return tInfo;
}


float SpotShadowPCF(float3 _Pos, float2 _InputUV)
{
    // ���� ��ġ�� ������ ���� ������ ��ȯ
    float4 ShadowMapPos = mul(float4(_Pos, 1.0), g_vShadowLightVP);

    float3 UVD = ShadowMapPos.xyz / ShadowMapPos.w;

    UVD.xy = 0.5f * UVD.xy + 0.5f;
    UVD.y = 1.0f - UVD.y;

	float Bias = 0.0f;

	int iRG = (int)(g_GBufferRG.Sample(g_LinearSampler, _InputUV).x);

	if (0 == iRG)
	{
		// ����
		Bias = 0.0001f;
	}
	else if (1 == iRG)
	{
		// ����ƽ
		Bias = 0.0002f;
	}
	else if (2 == iRG)
	{
		// �ִϸ��̼��� �ִ� ��ü��
		Bias = 0.0002f;
	}

	UVD.z -= Bias;
	float ReturnValue = 
		g_ShadowTex.SampleCmpLevelZero(g_PCFSmp, UVD.xy, UVD.z);

	return ReturnValue;
}

// �Ȱ�
// ApplyFog �Լ�
//: �Ȱ��� ���� ���¿��� �Ȱ� ����(_FogColor)���� ī�޶� ����(_CameraPosY), 
// ī�޶�κ��� �Ÿ��� ���� �Ȱ��� ����� �ȼ�(_CameraToPixel) ������ ��ȯ
float3 ApplyFog(float3 _OriginFogColor, float _CameraPosY, float3 _CameraToPixel)
{
    float fPixelDist = length(_CameraToPixel);
    float3 fEyeToPixelNorm = _CameraToPixel / fPixelDist;

	// �ȼ� �Ÿ��� ���� �Ȱ� ���� ���� ���
    float fFogDist = max(fPixelDist - g_FogStartDepth, 0.0); // �Ȱ� �Ÿ�

	// �Ȱ� ���⿡ ���� �Ÿ� ���
    float fFogHeightDensityAtViewer = exp(-g_FogHeightFalloff * _CameraPosY);
    float fFogDistInt = fFogDist * fFogHeightDensityAtViewer;

	// �Ȱ� ���⿡ ���� ���� ���
    float fEyeToPixelY = _CameraToPixel.y * (fFogDist / fPixelDist);
    float t = g_FogHeightFalloff * fEyeToPixelY;
    const float ThresholdT = 0.01f;
    float fFogHeightInt = abs(t) > ThresholdT ? (1.0 - exp(-t)) / t : 1.0f;

	// �� ��� ���� ���� ���� �μ� ���
    float fFogFinalFactor = exp(-g_FogGlobalDensity * fFogDistInt * fFogHeightInt);

	// �¾� ���̶���Ʈ ��� �� �Ȱ� ���� ȥ��
    float fSunHighlightFactor = saturate(dot(fEyeToPixelNorm, g_FogSunDir));
    fSunHighlightFactor = pow(fSunHighlightFactor, 8.0);
    float3 fFogFinalColor = lerp(g_FogColor, g_FogHightLightColor, fSunHighlightFactor);

    return lerp(fFogFinalColor, _OriginFogColor, fFogFinalFactor);
}

LightInfo ComputeLight(float3 _vViewPosition, float3 _vViewNormal,
            float4 _vDiffuse, float4 _vAmbient, float4 _vSpecular, float4 _vEmissive, 
            float _fSpecularPower, float _AO, float2 _InputUV)
{
    // ��������� ����Ʈ�� ������ٰ���
    LightInfo tInfo = (LightInfo) 0;

    float3 vLightDir = float3(0.0f, 0.0f, 0.0f);
    float fIntensity = 1.0f; // ���� ����

    // �ش� ������ Directional�� ���
    if (g_iLightType == LIGHT_DIR)
    {
        // ��������� ��������� ��ȯ�Ѵ�.
        // ��������� �������� ���ؾ��Ѵ�. ( �׷��� ������ ����)
        //vLightDir = -normalize(mul(float4(g_vLightDir, 0.0f), g_V).xyz);
        vLightDir = g_vLightDir;
    }

    // POINT ����
    if (g_iLightType == LIGHT_POINT)
    {
        // �������� ������ ���ϴ� ���͸� �����ֱ�
        vLightDir = g_vLightPosition - _vViewPosition;
        vLightDir = normalize(vLightDir);

        // ����(��ü�� ����)�� ������ �Ÿ��� �����ֱ�
        float fDistance = distance(g_vLightPosition, _vViewPosition);

        if (fDistance > g_fLightDistance)
        {
            // ����Ʈ�� �Ѱ�Ÿ��� ������
            fIntensity = 0.0f;
        }
        else
        {
            //fIntensity = 1.0f - fDistance / g_fLightDistance;

            // ���� ������ 0.3~0.7���� ����
            fIntensity = (1.0f - fDistance / g_fLightDistance) * 0.7f + 0.3f;
        }
    }

    // SPOT ����
    if (g_iLightType == LIGHT_SPOT)
    {
        // �������� ������ ���ϴ� ���� ���ϱ� (�����), _vViewPosition(����, �ٶ󺸴� ��ġ)
        vLightDir = g_vLightPosition - _vViewPosition;
        vLightDir = normalize(vLightDir);

        // ������ ������ �Ÿ� ���ϱ�
        float fDistance = distance(g_vLightPosition, _vViewPosition);

        if (fDistance > g_fLightDistance)
        {
            fIntensity = 0.0f;
        }
        else
        {
            // �ٱ��� �ݰ� �ȿ� ��� �Դ��� �Ǵ��ϱ�
            float3 vDir = -vLightDir;

            //  vDir�� ������ ���ߴ� ���͸� ���ؼ� ������  �ݰ� �ȿ� �������� �Ǵ��ϱ�
            // g_vLightDir �������� �� �־��ĸ� ��������ʿ��� -1.0f�� ���߱⶧��
            float fDot = dot(vDir, g_vLightDir);

            if (fDot < g_fLightOutAngle)
            {
                // �ٱ��� �ݰ��� ������
                fIntensity = 0.0f;
            }
            else if (fDot >= g_fLightInAngle)
            {
                // ���� �ݰ濡 �ִ��� �Ǵ� ( �ٱ��ʺ��� ���� ������ ���� )
                fIntensity = 1.0f;
            }
            else
            {
                //�ٱ��� �ݰ濡 �ִ� ���
                fIntensity = (g_fLightInAngle - fDot) / (g_fLightInAngle - g_fLightOutAngle);

            }

            // ���� �ε巴�� ���ֱ�(���κ��� �ʹ� �ѷ��ϰ� -��- ��..�׷��� �ε巴������)
            fIntensity = fIntensity * 0.3f + 0.7f;

            // �Ÿ��� ���� ���� ����
            float fDistanceIntensity = (1.0f - fDistance / g_fLightDistance) * 0.3f + 0.7f;

            // �� ����
            fIntensity *= fDistanceIntensity;
        }
    }

	float fSahdow = 1.0f;
    // ������
	if (g_iLightType == LIGHT_DIR)
	{
		if (0 != g_bShadow)
		{
			if (g_iLightType == LIGHT_DIR)
			{
				float3 vWorldPos = mul(float4(_vViewPosition, 1.0f), g_vInvCamView).xyz;
				fSahdow = SpotShadowPCF(vWorldPos, _InputUV);
			}
		}
	}

    tInfo.vDiffuse = g_vLightDiffuse * _vDiffuse * max(0, dot(_vViewNormal, vLightDir)) * fIntensity * fSahdow * _AO;
    tInfo.vAmbient = g_vLightAmbient * _vAmbient * _AO;

    // �ݻ纤�� ���ϱ�
    // �ݻ纤�� r�� r = I(�Ի纤��, ������ �����¹���) - 2(dot(n(�븻����), I))n�ε� ���� ���ö� ���������� ���ͼ� 
    // r = 2(n����I) - I�� 
    float3 vReflect = 2.0f * dot(_vViewNormal, vLightDir) * _vViewNormal - vLightDir;
    vReflect = normalize(vReflect);

    // View ���͸� ���Ѵ�.
    float3 vView = -normalize(_vViewPosition);

    tInfo.vSpecular = g_vLightSpecualr * _vSpecular * pow(max(0, dot(vReflect, vView)), _fSpecularPower) * fIntensity * fSahdow;
	tInfo.vEmissve = _vSpecular * _vEmissive;

    return tInfo;
}

////////////////////////////////////// ���� ��ȯ ( �����Ұ���, ��Ƽ����κа����� )

// float4 -> flaot
float ConvertColor(float4 _vColor)
{
    // 0~1������ ���� 0~255������.. ( �ϴ� �Ҽ��� ���������� �ٲ۴�. )
    uint4 vColor = (uint4) 0;
    vColor.r = uint(_vColor.r * 255);
    vColor.g = uint(_vColor.g * 255);
    vColor.b = uint(_vColor.b * 255);
    vColor.a = uint(_vColor.a * 255);

    // �״㿣 �� �������� uint = 4����Ʈ�� �� �־��ٰ���
    // ���� 1����Ʈ�� �־��ٰ��� ( 0~255���̹Ƿ� 4����Ʈ�� ����� �� �־��� �� ���� )
    // ARGB ����
    uint iColor = 0;
    iColor = (uint) (vColor.a);
    iColor = (iColor << 8) | vColor.r;
    iColor = (iColor << 8) | vColor.g;
    iColor = (iColor << 8) | vColor.b;

    // �׳� float���� ������ ������ �߻��Ѵ�.
    // �� ������ �ФФФ� �ѳ� ġ������.. 
    // asfloat�� ������ �߻��ϱ� �ʱ� ���� ����Ѵ�. (HLSL���� ����)
    return asfloat(iColor);
}

// float -> float4
float4 ConvertColor(float _fColor)
{
    float4  vColor;
    uint    iColor = asuint(_fColor);

    // BGRA
    vColor.b = (iColor & 0x000000ff) / 255.f;
    vColor.g = ((iColor >> 8) & 0x000000ff) / 255.f;
    vColor.r = ((iColor >> 16) & 0x000000ff) / 255.f;
    vColor.a = ((iColor >> 24) & 0x000000ff) / 255.f;

    return vColor;
}


// �ִϸ��̼� ��Ű��

// �ִϸ��̼� ��Ű���� ���� VTF(Vertex Texture Fatch)����� ������ ���̴�.
// �ȼ����̴����� Sample()�� ���� �ش� �ȼ��� ��������
// ���ؽ����̴����� Load(�ε���)�� ���� �ش� �������� �����´�. -> 5.0�������� ��밡��
// �ؽ�ó�� �迭ó�� �����ϰ� �����Ѵ�.

// ��Ű�� ������ 2����������� ���� �����Ѵ�.
// 1. ������ �����ϴ�.. -> Tangent, Binormal ����
// 2. ������ ���� X     -> Tangent, Binormal X

// �� ������ �����ϸ� ź��Ʈ�� ���̳븻�� �����Ѵ�.
// ������ �������� ������ ź��Ʈ(U)�� ���̳븻(V)�� �������� �ʴ´�.
// �Ʒ��� ��Ű������ ����ü�̴�.
struct _tagSkinning
{
    float3 vPos;
    float3 vNormal;
    float3 vTangent;
    float3 vBinormal;
};

// �ش� ������ �� �ε����� �־ ���������� �ε��Ѵ�.
matrix GetBoneMatrix(int _idx)
{
    // matBone -> Pos, Normal, Tangent, BiNormal ������ ��ķ� ���� �ִ�. ( �ش� �ε����� �°� )
    // Noraml�� �������� ���� ���, �� ������ �������� �ʴ� �޽��� ���
    // Tangent�� BiNormal�� 0 �̴�. 
    // ���� �ؽ�ó���� ����ĵ��� ������ ����ȭ�� �Ǿ� �ִ�.
    matrix matBone =
    {
        g_BoneTexture.Load(int3(_idx * 4, 0, 0)),               // Pos
        g_BoneTexture.Load(int3(_idx * 4 + 1, 0, 0)),           // Normal
        g_BoneTexture.Load(int3(_idx * 4 + 2, 0, 0)),           // Tangnet
		g_BoneTexture.Load(int3(_idx * 4 + 3, 0, 0))            // BiNormal
    };

    return matBone;
}

_tagSkinning Skinning(float3 _vPos, float3 _vNormal, float3 _vTangnet,
            float3 _vBiNormal, float4 vWeights, float4 _vIndices)
{
    _tagSkinning tSkinning = (_tagSkinning) 0;

    float fWeights[4];
    fWeights[0] = vWeights.x;
    fWeights[1] = vWeights.y;
    fWeights[2] = vWeights.z;
    fWeights[3] = 1.0f - vWeights.x - vWeights.y - vWeights.z;      // ����ġ�� �׻� �ִ밪�� 1�̱⶧���� �������� �־��ش�.

    // ���������� �� Tangnet,�� BiNormal ���
    for (int i = 0; i < 4; ++i)
    {
        matrix matBone = GetBoneMatrix((int) _vIndices[i]);         // �ش� �ε����� �ش��ϴ� �������� ��������

        tSkinning.vPos += fWeights[i] * mul(float4(_vPos, 1.0f), matBone).xyz;
        tSkinning.vNormal += fWeights[i] * mul(float4(_vNormal, 0.0f), matBone).xyz;
        tSkinning.vTangent += fWeights[i] * mul(float4(_vTangnet, 0.0f), matBone).xyz;
        tSkinning.vBinormal += fWeights[i] * mul(float4(_vBiNormal, 0.0f), matBone).xyz;
    }

    tSkinning.vNormal = normalize(tSkinning.vNormal);
    tSkinning.vTangent = normalize(tSkinning.vTangent);
    tSkinning.vBinormal = normalize(tSkinning.vBinormal);

    return tSkinning;
}


_tagSkinning Skinning(float3 _vPos, float3 _vNormal,
                         float4 vWeights, float4 _vIndices)
{
    _tagSkinning tSkinning = (_tagSkinning) 0;

    float fWeights[4];
    fWeights[0] = vWeights.x;
    fWeights[1] = vWeights.y;
    fWeights[2] = vWeights.z;
    fWeights[3] = 1.0f - vWeights.x - vWeights.y - vWeights.z; // ����ġ�� �׻� �ִ밪�� 1�̱⶧���� �������� �־��ش�.

    // ������ ������ϱ� ������  Tangnet�� BiNormal�� ������� �ʴ´�.
    for (int i = 0; i < 4; ++i)
    {
        matrix matBone = GetBoneMatrix((int) _vIndices[i]); // �ش� �ε����� �ش��ϴ� �������� ��������

        tSkinning.vPos += fWeights[i] * mul(float4(_vPos, 1.0f), matBone).xyz;
        tSkinning.vNormal += fWeights[i] * mul(float4(_vNormal, 0.0f), matBone).xyz;
    }

    tSkinning.vNormal = normalize(tSkinning.vNormal);

    return tSkinning;
}


// ������ UV����ϱ� -> ��Ʋ������ ���������� �����ϰ�.. �������ΰ��� ���ؽ�ó ��ü�� 1�������̱⶧���� �׳� ���
float2 ComputeFrameUV(float2 _vUV)
{
    float2 vResult = _vUV;

    // ��Ʋ��
    // ��ü�������� ������ ����ϴ°��� (�Ʒ��͵�)
    if (AFT_ATLAS == g_iAniFrameType)
    {
        // x��ǥ
        if (0.0f == _vUV.x)
        {
            // ó���κ� ->�ε��� 0�� �κ�... (���κд�..)
            vResult.x = g_vAniFrameStart.x / g_vAniFrameTextureSize.x;
        }
        else
        {
            vResult.x = g_vAniFrameEnd.x / g_vAniFrameTextureSize.x;
        }

        // y��ǥ
        if (0.0f == _vUV.y)
        {
            vResult.y = g_vAniFrameStart.y / g_vAniFrameTextureSize.y;
        }
        else
        {
            vResult.y = g_vAniFrameEnd.y / g_vAniFrameTextureSize.y;
        }
    }

    return vResult;
}


// �ν��Ͻ� ������ ���
float2 ComputeInstancingFrameUV(float2 _vUV, float2 _vStart, float2 _vEnd)
{
    float2 vResult = _vUV;

    // ��Ʋ��
    // ��ü�������� ������ ����ϴ°��� (�Ʒ��͵�)
    if (AFT_ATLAS == g_iAniFrameType)
    {
        // x��ǥ
        if (0.0f == _vUV.x)
        {
            // ó���κ� ->�ε��� 0�� �κ�... (���κд�..)
            vResult.x = _vStart.x / g_vAniFrameTextureSize.x;
        }
        else
        {
            vResult.x = _vEnd.x / g_vAniFrameTextureSize.x;
        }

        // y��ǥ
        if (0.0f == _vUV.y)
        {
            vResult.y = _vStart.y / g_vAniFrameTextureSize.y;
        }
        else
        {
            vResult.y = _vEnd.y / g_vAniFrameTextureSize.y;
        }
    }

    return vResult;
}