#pragma once
#include "Flag.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"

ENGINE_BEGIN

// �ػ�
typedef struct ENGINE_DLL _tagResolution
{
	int	iWidth;
	int	iHeight;
}Resolution, *PResolution;

// Color Vertex Buffer
typedef struct ENGINE_DLL _tagVertexColor
{
	Vector3	vPos;
	Vector4 vColor;

	_tagVertexColor()
	{

	}

	_tagVertexColor(const _tagVertexColor& _Vtx)
	{
		*this = _Vtx;
	}

	_tagVertexColor(const Vector3& _vPos, const Vector4& _vColor) : vPos(_vPos), vColor(_vColor)
	{

	}
}VertexColor, *PVertexColor;

// Color Normal Vertex Buffer
typedef struct ENGINE_DLL _tagVertexNormalColor
{
	Vector3	vPos;
	Vector3 vNormal;
	Vector4 vColor;

	_tagVertexNormalColor()
	{

	}

	_tagVertexNormalColor(const _tagVertexNormalColor& _Vtx)
	{
		*this = _Vtx;
	}

	_tagVertexNormalColor(const Vector3& _vPos, const Vector3& _vNormal, const Vector4& _vColor) 
		: vPos(_vPos), vNormal(_vNormal), vColor(_vColor)
	{

	}
}VertexNormalColor, *PVertexNormalColor;

// Vertex UV
typedef struct ENGINE_DLL _tagVertexUV
{
	Vector3	vPos;
	Vector2 vUV;

	_tagVertexUV()
	{

	}

	_tagVertexUV(const _tagVertexUV& _Vertex)
	{
		*this = _Vertex;
	}

	_tagVertexUV(const Vector3& _vPos, const Vector2& _vUV) 
		: vPos(_vPos), vUV(_vUV)
	{

	}
}VertexUV, *PVertexUV;


// ����Ʈ ��ƼŬ ����ü  ( ���ϼ��̴��� �̿��ؼ� �� -> �簢�� �޽��� Ȯ���ؼ� ������ ���̴�. )
// -> �޽� ������, �������̿��ؼ� ��ƼŬ �����Ұ���
typedef struct ENGINE_DLL _tagVertexDefulatParticle
{
	Vector3	vPos;
	Vector2	vSize;

	_tagVertexDefulatParticle()
	{

	}

	_tagVertexDefulatParticle(const _tagVertexDefulatParticle& _Vertex)
	{
		*this = _Vertex;
	}

	_tagVertexDefulatParticle(const Vector3& _vPos, const Vector2& _vSize) : 
		vPos(_vPos), vSize(_vSize)
	{

	}
}VertexDefaultParticle, *PVertexDefaultParticle;

// ��ƼŬ ����ü -> �� cbuffer(b11)�� ���� ������ (���� 10)
typedef struct ENGINE_DLL _tagParticleCBuffer
{
	Vector3	vParticlePos;				// ��ƼŬ ��ġ ���� ( ���������.., �׷��� �Ŀ� VP�� �ʿ�)
	float	fSizeX;						// ���� ������
	Vector3 vAxisX;						// X��
	float	fSizeY;						// ���� ������
	Vector3	vAxisY;						// Y��
	float	fPercent;					// ���� HP�����͵� ������ ���ۼ����� �������� ���� �⺻�� 1.0f

	_tagParticleCBuffer() 
	{
		fPercent = 1.0f;
	}

}ParticleCBuffer, *PParticleCBuffer;

// Constant Buffer
typedef struct ENGINE_DLL _tagConstBuffer
{
	std::string	strName;
	ID3D11Buffer*	pBuffer;
	void*	pData;
	int		iSize;
	int		iRegisterSlot;				// �������� ��ȣ
	int		iConstant;					// ���ؽ����̴�, �ȼ����̴�, ���ϼ��̴� �� ��� ���̴����� ����Ұ���
}ConstBuffer, *PConstBuffer;

// �� Transform.h, cpp���� �����ش�.
typedef struct ENGINE_DLL _tagTransformConstBuffer
{
	Matrix	WorldRotationMatrix;		// ���� ȸ����� 
	Matrix	WorldViewRotationMatrix;	// �����ȸ�� ��� (��ֺ��� �̿�)
	Matrix	WorldMatrix;				// ���� ���
	Matrix	ViewMatrix;					// �� ���
	Matrix  InversViewMatrix;
	Matrix	ProjectionMatrix;			// �������� ���
	Matrix	InversProjectionMatrix;		// �������� �����
	Matrix	WorldViewMatrix;			// ���� �� ���
	Matrix	WorldViewProjectionMatrix;		// ���� �� �������� ���
	Matrix	ViewProjectionMatrix;			// �� �������� ���   -> �������Ҷ� �ʿ�
}TransformConstBuffer, *PTransformConstBuffer;

typedef struct ENGINE_DLL _tagMaterialCBuffer
{
	Vector4 vDiffuse;
	Vector4 vAmbient;
	Vector4 vSpecular;
	Vector4 vEmissive;
	int		iNormal;			// �븻���� �ִ��� ���� (��ǻ��� �ʼ����� �� �ܴ� ���û�����)
	int		iSpeuclar;			// Sepcular�ؽ�ó ����
	int		iSkinning;			// �ִϸ��̼� �ִ��� ����
	float	fEmpty;

	_tagMaterialCBuffer()
		: vDiffuse(Vector4::White), 
		vAmbient(Vector4::White), 
		vSpecular(Vector4::Black), 
		vEmissive(Vector4::Black),
		iNormal(0),
		iSpeuclar(0),
		iSkinning(0),
		fEmpty(0.0f)
	{

	}
}MaterialCBuffer, *PMaterialCBuffer;

// 3D 
typedef struct ENGINE_DLL _tagVertexTextur3D
{
	Vector3 vPosition;
	Vector3 vNormal;
	Vector2 vUV;
	Vector3 vTangent;
	Vector3 vBinormal;
	Vector4 vBlendWeights;
	Vector4 vBlendIndices;
}Vertex3D, *PVertex3D;

// ����Ʈ ����
typedef struct ENGINE_DLL _tagLightInfo
{
	Matrix matShadowLightVP;
	Matrix matInvCamView;
	Vector4	vDiffuse;		// ��ǻ�� ����Ʈ ����
	Vector4	vAmbient;
	Vector4	vSpecular;
	Vector3	vPosition;
	int		iType;			// ����Ʈ Ÿ�� (����Ʈ, �𷺼ų�, ����)
	Vector3 vDirection;		// ����Ʈ ����
	float	fDistance;		// ����Ʈ �Ÿ�
	float	fInAngle;		// ��������Ʈ ���� ���� ����
	float	fOutAngle;		// �ٱ��� ����
	float	fBias;
	float	vEmpty;			// �׳� ������� 16����Ʈ�� �����ֱ� ���ؼ� ����� ���� (�ǹ� ���� ����)

	_tagLightInfo() :
		vDiffuse(Vector4::White),
		vAmbient(Vector4(0.2f, 0.2f, 0.2f, 1.0f)),
		vSpecular(Vector4::White),
		iType(LT_DIR),
		fDistance(0.0f),
		fInAngle(0.0f),
		fOutAngle(0.0f),
		fBias(0.0f),
		vEmpty(0.0f)
	{
		matShadowLightVP.Identity();
	}
}LightInfo, *PLightInfo;

// ������带 �˷��� ������ۿ� ����ü
typedef struct ENGINE_DLL _tagRenderCBuffer
{
	int	iRenderMode;
	int	bSSAO;
	int	bShadow;
	int	bPostEffect;
	int	bFog;
	int bRain;
	Vector2 vEmpty;
	_tagRenderCBuffer()
	{
		bSSAO = 0;
		bShadow = 0;
		bPostEffect = 0;
		bFog = 0;
		bRain = false;
	}

}RenderCbuffer, *PRenderCBuffer;

// ����Ÿ�� ����׿� ����� �������
typedef struct ENGINE_DLL _tagDebugCBuffer
{
	Matrix WVPMatrix;
}DebugCBuffer, *PDebugCBuffer;


#pragma region �浹 ���� ����ü

// �� �浹
typedef struct ENGINE_DLL _tagSphereInfo
{
	Vector3 vCenter;
	float fRadius;

	_tagSphereInfo() : fRadius(0.0f)
	{
	}
}SphereInfo, *PSphereInfo;

// AABB �浹
typedef struct ENGINE_DLL _tagAABBInfo
{
	Vector3 vMin;
	Vector3 vMax;

	_tagAABBInfo()
	{

	}
}AABBInfo, *PAABBInfo;

// OBB �浹
typedef struct ENGINE_DLL _tagOBBInfo
{
	Vector3 vCenter;			// ��ü�� �߽���ġ
	Vector3 vAxis[AXIS_END];	// �� ( �⺻��(x,y,z��) )
	Vector3 vHalfLength;		// ������ ����

	_tagOBBInfo()
	{
		for (int i = 0; i < AXIS_END; ++i)
		{
			vAxis[i] = Vector3::Axis[i];
			vHalfLength[i] = 1.0f;
		}
	}

}OBBInfo, *POBBInfo;

// Ray �浹
typedef struct ENGINE_DLL _tagRayInfo
{
	Vector3 vOrigin;			// ������
	Vector3 vDir;				// ����
	float fDist;				// �Ÿ�

	_tagRayInfo()
	{
		fDist = -1.0f;			// ������ Infinity �� ó��
	}

}RayInfo, *PRayInfo;

#pragma endregion

typedef struct ENGINE_DLL _tagAnimationFrameCBuffer
{
	int iAnimationFrameType;		// �ִϸ��̼� Ÿ�� -> ��Ʋ��, ������ ���� 
	int iAnimationOption;			// �ִϸ��̼� �ɼ�
	Vector2 vTextureSize;			// �ؽ�ó ũ��
	Vector2 vStart;
	Vector2 vEnd;
	int iFrame;
	Vector3 vEmpty;
}AnimationFrameCBuffer, *PAnimationFrameCBuffer;

// ���� ������ ����, ���÷��ð��� ����ü
typedef struct ENGINE_DLL _tagLandScaleCBuffer
{
	float fDetailLevelX;
	float fDetailLevelZ;
	int iSplatCount;
	float fEmpty;

	_tagLandScaleCBuffer()
	{
		fDetailLevelX = 1;
		fDetailLevelZ = 1;
		iSplatCount = 0;
		fEmpty = 0.0f;
	}
}LandScaleCBuffer, *PLandScaleCBuffer;

typedef struct ENGINE_DLL _tagButtonCBuffer
{
	Vector4 vColor;
}ButtonCBuffer, *PButtonCBuffer;


typedef struct ENGINE_DLL _tagImageCBuffer
{
	Vector4 vColor;
}ImageCBuffer, *PImageCBuffer;

typedef struct ENGINE_DLL _tagBarCBuffer
{
	Vector4 vColor;
	float	fRatio;
	int		iHeight;			// ���ο���
	Vector2 vEmpty;
	
}BarCBuffer, *PBarCBuffer;

#pragma region �ν��Ͻ�

// �ν��Ͻ� ��� ����
typedef struct ENGINE_DLL _tagInstancingStaticBuffer
{
	Matrix matWVP;			// WVP
	Matrix matWV;
	Matrix matWVRot;
}InstancingStaticBuffer, *PInstancingStaticBuffer;

// �ν��Ͻ� - �ִϸ��̼� 2D ����
typedef struct ENGINE_DLL _tagInstancingAnimFrameBuffer
{
	Matrix matWVP;
	Matrix matVP;
	Vector3 vParticlePos;
	Vector2 vParticleScale;
	Vector2 vStart;
	Vector2 vEnd;
	int iFrame;

}InstancingAnimFrameBuffer, *PInstancingAnimFrameBuffer;

// �ν��Ͻ� ����
typedef struct ENGINE_DLL _tagInstancingBUffer
{
	ID3D11Buffer*	pBuffer;
	void*			pData;
	int				iSize;
	int				iCount;
	D3D11_USAGE		eUsage;
}InstancingBuffer, *PInstancingBuffer;

// �ν��Ͻ� ����ü
typedef struct ENGINE_DLL _tagInstancingGeometry
{
	bool	bAnimation2D;				// 2D�ִϸ��̼� ���� (������ �ִϸ��̼�)
	bool	bAnimation3D;				// 3D �ִϸ��̼� ����
	class GameObject** pGameObjectList;		// ���� ������Ʈ�� ���� 
	int		iSize;						// ����
	int		iCapacity;

	// ���� �ø���
	void Resize()
	{
		if (iSize == iCapacity)
		{
			iCapacity *= 2;
			class GameObject** pList = new GameObject*[iCapacity];

			memset(pList, 0, sizeof(GameObject*) * iCapacity);				// 0 ���� �ʱ�ȭ
			memcpy(pList, pGameObjectList, sizeof(GameObject*) * iSize);	// �ø��� ���������� �����ӿ�����Ʈ ����Ʈ �־��ֱ�

			SAFE_DELETE_ARRAY(pGameObjectList);
			pGameObjectList = pList;
		}
	}

	// �߰� 
	void Add(class GameObject* _pObject)
	{
		Resize();
		pGameObjectList[iSize] = _pObject;
		++iSize;
	}

	void Clear()
	{
		iSize = 0;
	}

	_tagInstancingGeometry()
	{
		iSize = 0;
		iCapacity = 100;
		bAnimation2D = false;
		bAnimation3D = false;
		pGameObjectList = new GameObject*[iCapacity];
		memset(pGameObjectList, 0, sizeof(GameObject*) * iCapacity);
	}

	~_tagInstancingGeometry()
	{
		SAFE_DELETE_ARRAY(pGameObjectList);
	}
}InstancingGeometry, *PInstancingGeometry;


#pragma endregion


#pragma region ����Ʈ ���μ��� ����

typedef struct _tagTDownScaleCBuffer
{
	UINT nWidth;
	UINT nHeight;
	UINT nTotalPixels;
	UINT nGroupSize;
	float fAdaptation;
	float fBloomThreshold;
	Vector2 vEmpty;
} TDownScaleCBuffer, *PTDownScaleCBuffer;

typedef struct _tagTFinalPassCBuffer
{
	float fMiddleGrey;
	float fLumWhiteSqr;
	float fBloomScale;
	float vEmpty;
	Vector2 fProjValue;
	Vector2 fDOFFarRange;
} TFinalPassCBuffer, *PTFinalPassCBuffer;

#pragma endregion


#pragma region �׸��� ����

typedef struct _tagShadowMatCBuffer
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;
	Matrix matLightView;
	Matrix matLightProj;

}ShadowMatCBuffer, *PShadowMatCBuffer;

typedef struct _tagShadowLightVSCBufferss
{
	Matrix fLightPosition;
	Matrix fEmpty;
}ShadowLightVSCBuffer, *PShadowLightVSCBuffer;

typedef struct _tagShadowLightPSBuffer
{
	Vector4 fAmbientColor;
	Vector4 fDiffuseColor;
}ShadowLightPSBuffer, *PShadowLightPSBuffer;

typedef struct _tagShadowBiasCBuffer
{
	float fShadowBias;
	Vector3 vEmpty;
}ShadowBiasCBuffer, *PShadowBiasCBuffer;

#pragma endregion

#pragma region SSOA

typedef struct _tagSSAODownScaleCBuffer
{
	UINT inWidth;
	UINT inHeight;
	float fHorResRcp;
	float fVerResRcp;
	Vector4 vProjParams;
	Matrix matView;
	float fOffsetRadius;
	float fRadius;
	float fMaxDepth;
	int iEmpty;
}SSAODownScaleCBuffer, *PSSAODownScaleCBuffer;

#pragma endregion

#pragma region DepthFog

typedef struct _tagDepthFogCBuffer
{
	Vector3 vFogColor;
	float	fFogStartDepth;
	Vector3	vFogHightLightColor;
	float	fFogGlobalDensity;
	Vector3	vFogSunDir;
	float	fFogHeightFalloff;
	Vector3  g_CamPos;
	float   fEmpty;
}DepthFogCBuffer, *PDepthFogCBuffer;

#pragma endregion

#pragma region RenderGroupCBuffer

//���� ������Ʈ �������µ� �ٲ�
typedef struct _tagRenderGroup
{
	int iRenderGroup;
	Vector3 vEmpty;
}RenderGroupCBuffer, *PRenderGroupCBuffer;
#pragma endregion


ENGINE_END