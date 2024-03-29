#pragma once
#include "..\GameObject.h"

ENGINE_BEGIN

#define INSTNACING_BUFFER_SIZE 4096
#define INSTANCING_COUNT		5				// 昔什渡縮生稽 郊蝦 置社 鯵呪

// 虞戚闘研 級壱 赤澗 神崎詮闘級精 魚稽 鯉系聖 皐辞
// 繕誤反引研 匝暗鞘 しさし
// STL精 汗軒艦猿 **研 戚遂背辞 紫遂拝暗績 しさし ( 益撹 魚稽 幻級嬢爽澗暗 )
typedef struct ENGINE_DLL _tagRenderList
{
	GameObject**	pObjectList;
	int				iSize;				// 馬蟹 滴奄
	int				iCapacity;			// 遂勲 ( 恥 鯵呪 )

	_tagRenderList()
	{
		iSize = 0;
		iCapacity = 100;			//析舘 獣拙精 100生稽 獣拙馬壱 採膳馬檎 蓄亜馬澗 莫縦生稽 廃陥.
		pObjectList = new GameObject*[iCapacity];
		ZeroMemory(pObjectList, sizeof(GameObject*)*iCapacity);
	}

	~_tagRenderList()
	{
		SAFE_DELETE_ARRAY(pObjectList);
	}

}RenderList, *PRenderList;

// MRT (Multi-RenderTarget)
// MRT澗 歯稽 幻球澗 依戚 焼観 奄糎税 兄希古艦煽 適掘什税 RenderTartget乞製拭辞 
// MRT稽 紫遂拝 兄希展為級幻 皐紳陥.
// 聡 歯稽幻球澗 依戚 焼観 奄糎税 依聖 醗遂馬澗 号縦生稽 紫遂廃陥.
// 兄希展為級聖 広嬢爽澗 (MRT稽 幻級..) 姥繕端
// 燕戚舛左研 紫遂拝 呪 赤壱 燕戚葵聖 嘘端亜 亜管馬亀系 廃陥.
// AddMRT(), AddDepth(), FindMRT()
typedef struct ENGINE_DLL _tagMRT
{
	std::vector<class RenderTarget*>		RenderTargetVec;		// MRT税 兄希展為 乞製
	std::vector<ID3D11RenderTargetView*>	OldRenderTargetVec;		// 杉球 兄希 展為
	ID3D11DepthStencilView*					pDepth;					// 燕戚舛左
	ID3D11DepthStencilView*					pOldDepth;				// 奄糎税, 森穿 紀什坂

	_tagMRT() :
		pDepth(nullptr),
		pOldDepth(nullptr)
	{

	}

}MRT, *PMRT;


class ENGINE_DLL RenderManager
{
private:
	bool		m_bNavMeshRender;					//革搾五習 兄希食採(祷拭辞紫遂)
private:
	std::unordered_map<std::string, class RenderState*>		m_RenderStateMap;	// 兄希 什砺戚闘 乞製 ( 掘什斗虞戚煽, 紀什什度叔, 鷺兄球 ) 食採
	std::unordered_map<std::string, class RenderTarget*>	m_RenderTargetMap;	// 兄希 展為乞製 ( 巨遁球 兄希元聖 馬奄 是背 琶推.. MRT遂亀)

	std::unordered_map<std::string, PMRT>					m_MRTMap;			// MRT

	RenderList		m_tLightList;				// 虞戚闘 軒什闘 (虞戚闘 更床澗走..), 繕誤乞製
	RenderList		m_tRenderGroup[RG_END];		// 兄希 益血紺 乞製 (兄希 益血紺稽 兄希元 背匝暗績)

	RENDER_MODE		m_eMode;					// 兄希乞球 (匂趨球, 巨遁球, 奄沙精 巨遁球)
	RenderCbuffer	m_tRenderCBuffer;			// 兄希乞球研 捨戚希拭 角移匝 雌呪獄遁遂 姥繕端
	DepthFogCBuffer	m_tDepthFogCBuffer;

	// 戚薦 兄希古艦煽亜 児巴君研 硝 琶推亜 蒸陥. ( 軒社什 古艦煽拭辞 持失板 捨戚希稽 郊稽 隔嬢早製 捨戚希拭辞 据馬澗 児巴君 災君辞 紫遂馬檎喫)
	//class Sampler*	m_pLinearSampler;
	//class Sampler*	m_pPointSampler;

	class RenderState*	m_pDepthDisable;		// 紀什 紫遂 X
	class RenderState*	m_pAccBlend;			// 刊旋遂 鷺兄球
	class RenderState*	m_pAlphaBlend;			// 硝督 鷺兄球
	class RenderState*	m_pCullNone;			// CullNone 乞球 (Rasterizer)

	class Shader*		m_pLightAccDirShader;		// 繕誤刊旋遂 捨戚希 - Directinal Light
	class Shader*		m_pLightAccPointShader;		// 繕誤刊旋遂 捨戚希 - Point Light
	class Shader*		m_pLightAccSpotShader;		// 繕誤刊旋遂 捨戚希 - Spot Light
	class Shader*		m_pLightBlendShader;		// 虞戚闘拭 鷺兄球 旋遂廃 捨戚希
	class Shader*		m_pLightBlendRenderShader;	// 虞戚闘 鷺兄球 兄希 捨戚希

	class Mesh*			m_pLightPointVolume;		// 瑳拳五習 - Point
	class Mesh*			m_pLightSpotVolume;			// 瑳拳五習 - Spot

	ID3D11InputLayout*	m_pLightPointLayout;		// 瑳拳五習傾戚焼数 - Point
	ID3D11InputLayout*	m_pLightSpotLayout;			// 瑳拳五習傾戚焼数 - Spot

private:
	// 昔什渡縮 淫恵
	// 湛腰属 4郊戚闘 : 五習 , 砧腰属 4郊戚闘 : 袴銅軒杖 -> __int64
	std::unordered_map<unsigned __int64, PInstancingGeometry>	m_mapInstancingGeometry;

	std::list<PInstancingGeometry> m_InstancingList[RG_END];

	PInstancingBuffer	m_pStaticInstancing;
	PInstancingBuffer	m_pAnimFrameInstancing;
	PInstancingBuffer	m_pAnimInstancing;
	PInstancingBuffer	m_pColliderInstancing;			// 是坦軍 剰亀 蓄亜背爽檎 吉陥.
	PInstancingBuffer	m_pLightInstancing;				// 剰亀 しさし

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
	// 昔什渡縮 淫恵
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
	//匂什闘 戚薙闘 淫恵

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

	ID3D11Buffer*				m_pDownScaleBuffer;			// 番亀葵聖 域至拝 凶 掻娃葵聖 煽舌拝 獄遁
	ID3D11UnorderedAccessView*	m_pDownScaleUAV;			// 授辞 蒸澗 羨悦 坂
	ID3D11ShaderResourceView*	m_pDownScaleSRV;			// 捨戚希 軒社什 坂

	ID3D11Buffer*				m_pAvgLumBuffer;			// 汝液 番亀 葵聖 煽舌拝 獄遁
	ID3D11UnorderedAccessView*	m_pAvgLumUAV;				// 授辞 蒸澗 羨悦 坂
	ID3D11ShaderResourceView*	m_pAvgLumSRV;				// 捨戚希 軒社什 坂

	TDownScaleCBuffer m_DownScaleCBuffer;
	TFinalPassCBuffer m_FinalPassCBuffer;

	class Shader*	m_pPostEffectShader;
	class Shader*	m_pDownScaleFirstPassShader;
	class Shader*	m_pDownScaleSecondPassShader;

	// 旋誓
	// 唖 覗傾績 兄希元聖 拝 凶原陥 薄仙 汝液 番亀葵聖 獄遁拭 煽舌馬壱 送穿 覗傾績税 汝液 番亀 葵聖 
	// 陥献 獄遁拭 煽舌..
	ID3D11Buffer*				m_pPrevAdaptionBuffer;			// 陥製 覗傾績税 送穿 汝液 番亀葵聖 煽舌拝 獄遁
	ID3D11UnorderedAccessView*	m_pPrevAdaptionUAV;
	ID3D11ShaderResourceView*	m_pPrevAdaptionSRV;

	// 鷺結
	// 項精 波漆拭辞 爽痕税 嬢砧錘 波漆 楕生稽 笹聖 碑軒澗 依
	// 1. 陥錘 什追析吉 HDR 努什坦
	// 2. 琶斗元鞠奄 穿税 鷺結 葵聖 績獣稽 煽舌馬奄 是廃 慎蝕
	// 3. 置曽 鷺結 葵
	ID3D11Texture2D*			m_pDownScaleSceneRT;		// 陥錘 什追析吉 HDR 努什坦
	ID3D11UnorderedAccessView*	m_pDownScaleSceneUAV;
	ID3D11ShaderResourceView*	m_pDownScaleSceneSRV;

	ID3D11Texture2D*			m_pTempRT[2];				// 績獣 努什坦 (慎蝕)
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
	void Adaptation(float _fTime);						// 旋誓
	void Bloom();										// 鷺結
	void Blur();										// 鷺君

private:
	RenderGroupCBuffer m_tRGCBuffer;

private:
	bool m_bSSAO;										// SSAO旋遂 食採
	bool m_bDepthFog;									// 燕戚 奄鋼 照鯵 旋遂 食採 
	bool m_bRain;										// 悪酔 反引 旋遂 食採

public:
	void SetSSAOEnable(bool _Enable);
	bool GetSSAOEnable();

	void SetDepthFogEnable(bool _Enable);
	bool GetDepthFogEnable();

	void SetRainEnable(bool _Enable);					// 悪酔 反引 旋遂 食採
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
	void AddRenderObject(GameObject* _pObject);			// 神崎詮闘級 兄希益血紺稽 隔嬢爽奄

public:
	// Rasterizer 幻級奄 
	bool CreateRasterizerState(const std::string& _strName, D3D11_FILL_MODE _eFill = D3D11_FILL_SOLID,
		D3D11_CULL_MODE _eCull = D3D11_CULL_BACK);

	// DepthState 幻級奄
	bool CreateDepthState(const std::string& _strName, BOOL _bEnable = TRUE,
		D3D11_DEPTH_WRITE_MASK _eWirteMaske = D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_FUNC _eDpethFunc = D3D11_COMPARISON_LESS);

	// Blend Option sruct 幻級奄
	bool AddTargetBlendDesc(const std::string& _strName,
		BOOL _bEnable = FALSE,
		D3D11_BLEND _eSrcBlend = D3D11_BLEND_ONE,
		D3D11_BLEND _eDestBlend = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP _eOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND _eSrcBlendAlpha = D3D11_BLEND_ONE,
		D3D11_BLEND _eDestBlendAlpha = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP _eAlphaOp = D3D11_BLEND_OP_ADD,
		UINT _iWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);

	// Blend State 幻級奄
	bool CreateBlendState(const std::string& _strName, BOOL _bAlphaToCoverage = FALSE,
		BOOL _bIndependentBlend = FALSE);

	// 兄希展為 幻級奄 ( 戚硯, 亜稽, 室稽, 兄希展為 努什坦 匂庫, 適軒嬢事雌, 紀什 匂庫)
	bool CreateRenderTarget(const std::string& _strName,
		UINT _iWidth, UINT _iHeight, DXGI_FORMAT _eFormat,
		float _fClearColor[4], int _iSamplerCount = 1,
		DXGI_FORMAT _eDepthFormat = DXGI_FORMAT_UNKNOWN);

	// 巨獄益遂 兄希展為
	bool OnDebugRenderTarget(const std::string& _strName, const Vector3& _vPos,
		const Vector3& _vScale);

public:
	// MRT遂 敗呪
	bool AddMRT(const std::string& _strMRTName, const std::string& _strTarget);
	bool AddDepth(const std::string& _strMRTName, const std::string& _strTargetName);
	void ClearMRT(const std::string& _strMRTName);
	void SetMRT(const std::string& _strMRTName);
	void ResetMRT(const std::string& _strMRTName);

public:
	class RenderState* FindRenderState(const std::string& _strName);		// 兄希什砺戚闘 達奄
	class RenderTarget* FindRenderTarget(const std::string& _strName);		// 兄希 展為 達奄
	PMRT FindMRT(const std::string& _strName);								// MRT 達奄

public:
	void ComputeInstancing();					// 昔什渡縮 拝闇走 焼観走 歳嫌拙穣 敗呪
	void Render(float _fTime);

private:
	void RenderForward(float _fTime);			// 匂趨球 兄希元
	void RenderDeferred(float _fTime);			// 巨遁球 兄希元

	// GBuffer  ( G : Geometry )
	// 巨遁球澗 曽嫌紺稽 蟹寛操醤廃陥. 
	// GBuffer研 是廃 兄希展為聖 幻級嬢爽奄
	void RenderGBuffer(float _fTime);

	// 繕誤兄希 敗呪級
	void RenderLightAcc(float _fTime);									// 繕誤級 刊旋拝 敗呪
	void RenderLightDir(float _fTime, class Light* _pLight);			// DirectionalLight Render
	void RenderLightPoint(float _fTime, class Light* _pLight);			// PointLight Render
	void RenderLightSpot(float _fTime, class Light* _pLight);			// SpotLight Render

	void RenderLightBlend(float _fTime);
	void RenderLightBlendRender(float _fTime);

	// 硝督旋遂吉 蕉級精 魚稽 昔什渡縮 敗呪稽 皐奄
	void RenderInstancingAlpha(float _fTime);

	void RenderPostProcessing(float _fTime);

	void RenderShadow(float _fTime);
	void RenderShadowAllMesh(class Light* _pLight, Matrix _matView, Matrix _matProj, float _fTime);

	void RenderSSAO(float _fTime);

	void RenderDepthFog(float _fTime);

	// UI 兄希
	void RenderUI(float _fTime);							// 昔什渡縮 焼観蕉級戚櫛 昔什渡縮 UI 却陥 兄希
	void RenderInstancingUI(float _fTime);

	DECLARE_SINGLETON(RenderManager)
};

ENGINE_END

