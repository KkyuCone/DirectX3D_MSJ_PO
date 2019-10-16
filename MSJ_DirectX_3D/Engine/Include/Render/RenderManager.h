#pragma once
#include "..\GameObject.h"

ENGINE_BEGIN

#define INSTNACING_BUFFER_SIZE 4096
#define INSTANCING_COUNT		5				// �ν��Ͻ����� �ٲ� �ּ� ����

// ����Ʈ�� ��� �ִ� ������Ʈ���� ���� ����� ����
// ����ȿ���� �ٰž� ������
// STL�� �����ϱ� **�� �̿��ؼ� ����Ұ��� ������ ( �׳� ���� ������ִ°� )
typedef struct ENGINE_DLL _tagRenderList
{
	GameObject**	pObjectList;
	int				iSize;				// �ϳ� ũ��
	int				iCapacity;			// �뷮 ( �� ���� )

	_tagRenderList()
	{
		iSize = 0;
		iCapacity = 100;			//�ϴ� ������ 100���� �����ϰ� �����ϸ� �߰��ϴ� �������� �Ѵ�.
		pObjectList = new GameObject*[iCapacity];
		ZeroMemory(pObjectList, sizeof(GameObject*)*iCapacity);
	}

	~_tagRenderList()
	{
		SAFE_DELETE_ARRAY(pObjectList);
	}

}RenderList, *PRenderList;

// MRT (Multi-RenderTarget)
// MRT�� ���� ����� ���� �ƴ� ������ �����Ŵ��� Ŭ������ RenderTartget�������� 
// MRT�� ����� ����Ÿ�ٵ鸸 ���´�.
// �� ���θ���� ���� �ƴ� ������ ���� Ȱ���ϴ� ������� ����Ѵ�.
// ����Ÿ�ٵ��� �����ִ� (MRT�� ����..) ����ü
// ���������� ����� �� �ְ� ���̰��� ��ü�� �����ϵ��� �Ѵ�.
// AddMRT(), AddDepth(), FindMRT()
typedef struct ENGINE_DLL _tagMRT
{
	std::vector<class RenderTarget*>		RenderTargetVec;		// MRT�� ����Ÿ�� ����
	std::vector<ID3D11RenderTargetView*>	OldRenderTargetVec;		// ���� ���� Ÿ��
	ID3D11DepthStencilView*					pDepth;					// ��������
	ID3D11DepthStencilView*					pOldDepth;				// ������, ���� ������

	_tagMRT() :
		pDepth(nullptr),
		pOldDepth(nullptr)
	{

	}

}MRT, *PMRT;


class ENGINE_DLL RenderManager
{
private:
	bool		m_bNavMeshRender;					//�׺�޽� ��������(���������)
private:
	std::unordered_map<std::string, class RenderState*>		m_RenderStateMap;	// ���� ������Ʈ ���� ( �����Ͷ�����, �������ٽ�, ���� ) ����
	std::unordered_map<std::string, class RenderTarget*>	m_RenderTargetMap;	// ���� Ÿ�ٸ��� ( ���۵� �������� �ϱ� ���� �ʿ�.. MRT�뵵)

	std::unordered_map<std::string, PMRT>					m_MRTMap;			// MRT

	RenderList		m_tLightList;				// ����Ʈ ����Ʈ (����Ʈ ��������..), �������
	RenderList		m_tRenderGroup[RG_END];		// ���� �׷캰 ���� (���� �׷캰�� ������ ���ٰ���)

	RENDER_MODE		m_eMode;					// ������� (������, ���۵�, �⺻�� ���۵�)
	RenderCbuffer	m_tRenderCBuffer;			// ������带 ���̴��� �Ѱ��� ������ۿ� ����ü
	DepthFogCBuffer	m_tDepthFogCBuffer;

	// ���� �����Ŵ����� ���÷��� �� �ʿ䰡 ����. ( ���ҽ� �Ŵ������� ������ ���̴��� �ٷ� �־����� ���̴����� ���ϴ� ���÷� �ҷ��� ����ϸ��)
	//class Sampler*	m_pLinearSampler;
	//class Sampler*	m_pPointSampler;

	class RenderState*	m_pDepthDisable;		// ���� ��� X
	class RenderState*	m_pAccBlend;			// ������ ����
	class RenderState*	m_pAlphaBlend;			// ���� ����
	class RenderState*	m_pCullNone;			// CullNone ��� (Rasterizer)

	class Shader*		m_pLightAccDirShader;		// �������� ���̴� - Directinal Light
	class Shader*		m_pLightAccPointShader;		// �������� ���̴� - Point Light
	class Shader*		m_pLightAccSpotShader;		// �������� ���̴� - Spot Light
	class Shader*		m_pLightBlendShader;		// ����Ʈ�� ���� ������ ���̴�
	class Shader*		m_pLightBlendRenderShader;	// ����Ʈ ���� ���� ���̴�

	class Mesh*			m_pLightPointVolume;		// �����޽� - Point
	class Mesh*			m_pLightSpotVolume;			// �����޽� - Spot

	ID3D11InputLayout*	m_pLightPointLayout;		// �����޽����̾ƿ� - Point
	ID3D11InputLayout*	m_pLightSpotLayout;			// �����޽����̾ƿ� - Spot

private:
	// �ν��Ͻ� ����
	// ù��° 4����Ʈ : �޽� , �ι�° 4����Ʈ : ��Ƽ���� -> __int64
	std::unordered_map<unsigned __int64, PInstancingGeometry>	m_mapInstancingGeometry;

	std::list<PInstancingGeometry> m_InstancingList[RG_END];

	PInstancingBuffer	m_pStaticInstancing;
	PInstancingBuffer	m_pAnimFrameInstancing;
	PInstancingBuffer	m_pAnimInstancing;
	PInstancingBuffer	m_pColliderInstancing;			// ��ó�� �굵 �߰����ָ� �ȴ�.
	PInstancingBuffer	m_pLightInstancing;				// �굵 ������

	class Shader*		m_pStaticInstancingShader;
	class Shader*		m_pAnimFrameInstancingShader;
	class Shader*		m_pAnimInstancingShader;
	class Shader*		m_pUIButtonInstancingShader;
	class Shader*		m_pUIImageInstancingShader;
	class Shader*		m_pUIBarInstancingShader;

	ID3D11InputLayout*		m_pStaticInstancingLayout;
	ID3D11InputLayout*		m_pAnimFrameInstancingLayout;
	ID3D11InputLayout*		m_pAnimInstancingLayout;
	ID3D11InputLayout*		m_pUIInstancingLayout;

private:
	// �ν��Ͻ� ����
	PInstancingGeometry FindInstancingGeometry(unsigned __int64 _iKey);
	PInstancingBuffer CreateInstancingBuffer(int _iSize, int _iCount = INSTNACING_BUFFER_SIZE);
	void ResizeInstancingBuffer(PInstancingBuffer _pBuffer, int _iCount);
	void AddInstancingData(PInstancingBuffer _pBuffer, int _iPos, void* _pData);
	void CopyInstancingData(PInstancingBuffer _pBuffer, int _iCount);

private:
	bool m_bShadowCompute;
	class Shader*	m_pDynamicShadowShader;
	class Shader*	m_pInstancingShadowShader;
	class Shader*	m_pStaticShadowShader;

public:
	void SetShadowCompute(bool _Value);
	bool GetShadowCompute();

private:
	//����Ʈ ����Ʈ ����

	// HDR
	bool	m_bPostEffect;
	bool	m_bBloom;
	//bool	m_bShadow;
	//bool	m_bBlur;

	UINT m_uiWidth;
	UINT m_uiHeight;
	UINT m_uiDomain;
	UINT m_uiDownScaleGroups;
	float m_fAdatation;
	float m_fBloomThreshold;
	float m_fBloomScale;

	float m_fMiddleGrey;
	float m_fWhite;

	float m_fDOFFarStart;
	float m_fDOFFarRange;

	ID3D11Buffer*				m_pDownScaleBuffer;			// �ֵ����� ����� �� �߰����� ������ ����
	ID3D11UnorderedAccessView*	m_pDownScaleUAV;			// ���� ���� ���� ��
	ID3D11ShaderResourceView*	m_pDownScaleSRV;			// ���̴� ���ҽ� ��

	ID3D11Buffer*				m_pAvgLumBuffer;			// ��� �ֵ� ���� ������ ����
	ID3D11UnorderedAccessView*	m_pAvgLumUAV;				// ���� ���� ���� ��
	ID3D11ShaderResourceView*	m_pAvgLumSRV;				// ���̴� ���ҽ� ��

	TDownScaleCBuffer m_DownScaleCBuffer;
	TFinalPassCBuffer m_FinalPassCBuffer;

	class Shader*	m_pPostEffectShader;
	class Shader*	m_pDownScaleFirstPassShader;
	class Shader*	m_pDownScaleSecondPassShader;

	// ����
	// �� ������ �������� �� ������ ���� ��� �ֵ����� ���ۿ� �����ϰ� ���� �������� ��� �ֵ� ���� 
	// �ٸ� ���ۿ� ����..
	ID3D11Buffer*				m_pPrevAdaptionBuffer;			// ���� �������� ���� ��� �ֵ����� ������ ����
	ID3D11UnorderedAccessView*	m_pPrevAdaptionUAV;
	ID3D11ShaderResourceView*	m_pPrevAdaptionSRV;

	// ���
	// ���� �ȼ����� �ֺ��� ��ο� �ȼ� ������ ���� �긮�� ��
	// 1. �ٿ� �����ϵ� HDR �ؽ�ó
	// 2. ���͸��Ǳ� ���� ��� ���� �ӽ÷� �����ϱ� ���� ����
	// 3. ���� ��� ��
	ID3D11Texture2D*			m_pDownScaleSceneRT;		// �ٿ� �����ϵ� HDR �ؽ�ó
	ID3D11UnorderedAccessView*	m_pDownScaleSceneUAV;
	ID3D11ShaderResourceView*	m_pDownScaleSceneSRV;

	ID3D11Texture2D*			m_pTempRT[2];				// �ӽ� �ؽ�ó (����)
	ID3D11UnorderedAccessView*	m_pTempUAV[2];
	ID3D11ShaderResourceView*	m_pTempSRV[2];

	ID3D11Texture2D*			m_pBloomRT;
	ID3D11UnorderedAccessView*	m_pBloomUAV;
	ID3D11ShaderResourceView*	m_pBloomSRV;

	class Shader*	m_pBloomBritePassShader;
	class Shader*	m_pBloomBlurVerticalShader;
	class Shader*	m_pBloomBlurHorizonShader;

private:
	void PostEffectInit();
	void DownScale(RenderTarget* _pHDRSRV);				// HDR
	void TonMapping(RenderTarget* _pHDRSRV);			// HDR
	void Adaptation(float _fTime);						// ����
	void Bloom();										// ���
	void Blur();										// ��

private:
	RenderGroupCBuffer m_tRGCBuffer;

private:
	bool m_bSSAO;										// SSAO���� ����
	bool m_bDepthFog;									// ���� ��� �Ȱ� ���� ���� 
	bool m_bRain;										// ���� ȿ�� ���� ����

public:
	void SetSSAOEnable(bool _Enable);
	bool GetSSAOEnable();

	void SetDepthFogEnable(bool _Enable);
	bool GetDepthFogEnable();

	void SetRainEnable(bool _Enable);					// ���� ȿ�� ���� ����
	bool GetRainEnable();

public:
	void SetPostEffectEnable(bool _Enable);
	void SetBloomEnable(bool _Enable);

	public:
		void SetNavMeshRender(bool _Value);
		bool GetNavMeshRender();

public:
	class Light* GetFirstLight()	const;

public:
	bool Init();
	void ShaderLyaoutInit();
	bool SSAOInit();
	bool DepthFogInit();
	bool RenderStateInit();
	bool RenderTargetInit();
	void AddRenderObject(GameObject* _pObject);			// ������Ʈ�� �����׷캰�� �־��ֱ�

public:
	// Rasterizer ����� 
	bool CreateRasterizerState(const std::string& _strName, D3D11_FILL_MODE _eFill = D3D11_FILL_SOLID,
		D3D11_CULL_MODE _eCull = D3D11_CULL_BACK);

	// DepthState �����
	bool CreateDepthState(const std::string& _strName, BOOL _bEnable = TRUE,
		D3D11_DEPTH_WRITE_MASK _eWirteMaske = D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_FUNC _eDpethFunc = D3D11_COMPARISON_LESS);

	// Blend Option sruct �����
	bool AddTargetBlendDesc(const std::string& _strName,
		BOOL _bEnable = FALSE,
		D3D11_BLEND _eSrcBlend = D3D11_BLEND_ONE,
		D3D11_BLEND _eDestBlend = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP _eOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND _eSrcBlendAlpha = D3D11_BLEND_ONE,
		D3D11_BLEND _eDestBlendAlpha = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP _eAlphaOp = D3D11_BLEND_OP_ADD,
		UINT _iWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);

	// Blend State �����
	bool CreateBlendState(const std::string& _strName, BOOL _bAlphaToCoverage = FALSE,
		BOOL _bIndependentBlend = FALSE);

	// ����Ÿ�� ����� ( �̸�, ����, ����, ����Ÿ�� �ؽ�ó ����, Ŭ�������, ���� ����)
	bool CreateRenderTarget(const std::string& _strName,
		UINT _iWidth, UINT _iHeight, DXGI_FORMAT _eFormat,
		float _fClearColor[4], int _iSamplerCount = 1,
		DXGI_FORMAT _eDepthFormat = DXGI_FORMAT_UNKNOWN);

	// ����׿� ����Ÿ��
	bool OnDebugRenderTarget(const std::string& _strName, const Vector3& _vPos,
		const Vector3& _vScale);

public:
	// MRT�� �Լ�
	bool AddMRT(const std::string& _strMRTName, const std::string& _strTarget);
	bool AddDepth(const std::string& _strMRTName, const std::string& _strTargetName);
	void ClearMRT(const std::string& _strMRTName);
	void SetMRT(const std::string& _strMRTName);
	void ResetMRT(const std::string& _strMRTName);

public:
	class RenderState* FindRenderState(const std::string& _strName);		// ����������Ʈ ã��
	class RenderTarget* FindRenderTarget(const std::string& _strName);		// ���� Ÿ�� ã��
	PMRT FindMRT(const std::string& _strName);								// MRT ã��

public:
	void ComputeInstancing();					// �ν��Ͻ� �Ұ��� �ƴ��� �з��۾� �Լ�
	void Render(float _fTime);

private:
	void RenderForward(float _fTime);			// ������ ������
	void RenderDeferred(float _fTime);			// ���۵� ������

	// GBuffer  ( G : Geometry )
	// ���۵�� �������� ��������Ѵ�. 
	// GBuffer�� ���� ����Ÿ���� ������ֱ�
	void RenderGBuffer(float _fTime);

	// ������ �Լ���
	void RenderLightAcc(float _fTime);									// ����� ������ �Լ�
	void RenderLightDir(float _fTime, class Light* _pLight);			// DirectionalLight Render
	void RenderLightPoint(float _fTime, class Light* _pLight);			// PointLight Render
	void RenderLightSpot(float _fTime, class Light* _pLight);			// SpotLight Render

	void RenderLightBlend(float _fTime);
	void RenderLightBlendRender(float _fTime);

	// ��������� �ֵ��� ���� �ν��Ͻ� �Լ��� ����
	void RenderInstancingAlpha(float _fTime);

	void RenderPostProcessing(float _fTime);

	void RenderShadow(float _fTime);
	void RenderShadowAllMesh(class Light* _pLight, Matrix _matView, Matrix _matProj, float _fTime);

	void RenderSSAO(float _fTime);

	void RenderDepthFog(float _fTime);

	// UI ����
	void RenderUI(float _fTime);							// �ν��Ͻ� �ƴѾֵ��̶� �ν��Ͻ� UI �Ѵ� ����
	void RenderInstancingUI(float _fTime);

	DECLARE_SINGLETON(RenderManager)
};

ENGINE_END

