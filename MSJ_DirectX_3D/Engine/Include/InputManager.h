#pragma once
#include "Engine.h"

ENGINE_BEGIN

// 原酔什 淫恵 DIK澗 蒸生糠稽.. DIK研 績獣稽 幻級嬢捜.
#define DIK_LBUTTON 0xfd
#define DIK_RBUTTON 0xfe
#define DIK_MBUTTON	0xff

typedef struct ENGINE_DLL _tagKeyInfo
{
	bool	bPress;		// 徹研 刊牽壱 赤澗走
	bool	bPush;		// 徹研 喚袈澗走
	bool	bRelease;	// 脅嬢蛎澗走 (照刊牽澗暗)

	_tagKeyInfo()
	{
		bPress = false;
		bPush = false;
		bRelease = false;
	}
}KeyInfo, *PKeyInfo;

// Key人 淫恵吉 適掘什澗 Call Back 敗呪 莫殿稽 幻級暗績 しさし
// Client拭辞 背雁 徹研 刊牽檎 益 徹拭 尻衣吉 敗呪研 硲窒拝 依戚陥. ( event 敗呪坦軍 持唖馬檎喫 )
// Axis澗 刊牽壱 赤澗 井酔 (域紗 刊牽壱 赤澗井酔) function<獣娃葵, 什追析葵> , ex ) 戚疑徹
// Action精 喚袈聖 井酔, 兜聖凶 , function<獣娃葵>		ex) 什迭徹 去

// void稽 閤澗 戚政澗嬢恐 敗呪亜 級嬢臣走 乞牽奄凶庚戚陥 しさし
// 奄糎 Axis拭 脊径閤聖 徹櫛 敗呪亜 郊昔球鞠嬢赤澗 食採研 蓄亜背層陥.

typedef struct ENGINE_DLL _tagKeyScale
{
	unsigned char cKey;		// 乞窮徹澗 char稽 閤聖 呪赤陥. ( 徹澗 256鯵, 0~255)
	float fScale;
}KeyScale, *PKeyScale;

typedef struct ENGINE_DLL _tagBindAxisInfo
{
	std::string strName;
	bool bFunctionBind;									// 敗呪人 尻衣鞠嬢赤澗走 食採
	std::function<void(float, float)>	Func;			// 尻衣吉 敗呪
	std::list<PKeyScale> KeyList;
	bool bGlobal;				// 陥献樟級拭辞亀 因遂生稽 承闇走 食採

	_tagBindAxisInfo()
	{
		bFunctionBind = false;
		bGlobal = false;
	}

	~_tagBindAxisInfo()
	{
		// 疑旋拝雁 吉暗 肢薦背爽奄 しさし
		Safe_Delete_VectorList(KeyList);
	}

}BindAxisInfo, *PBindAxisInfo;

// 衝芝 舛左 (繕杯徹 舛左)
typedef struct ENGINE_DLL _tagBindActionKeyInfo
{
	MOUSE_BUTTON	eMouseType;					// 原酔什 葵(図楕, 神献楕, 亜錘汽)
	unsigned char	cKey;
	bool			bFKKey[FK_END];				// 繕杯徹亜 赤澗走 食採 
	KeyInfo			tKeyInfo;
	//bool			bKeyState[KS_END];			// 徹雌殿
	//bool			bFKKeyState[FK_END][KS_END];	// 繕杯徹 雌殿 [繕杯徹][徹雌殿]

	_tagBindActionKeyInfo()
	{
		cKey = 0;
		memset(bFKKey, 0, sizeof(bool) * FK_END);
		eMouseType = MB_NONE;
		/*memset(bKeyState, 0, sizeof(bool) * KS_END);
		memset(bFKKeyState, 0, sizeof(bool) * KS_END * FK_END);*/
	}
}ActionKeyInfo, *PActionKeyInfo;

typedef struct ENGINE_DLL _tagBindActionInfo
{
	std::string strName;
	bool bFunctionBind[KS_END];				// 敗呪亜 広食赤澗走 食採
	std::function<void(float)>		Func[KS_END];			// 剰亀 唖 敗呪級戚赤製 (Press, Push, Release 析凶税 災君臣 敗呪級 )
	std::list<PActionKeyInfo> KeyList;
	bool bGlobal;

	_tagBindActionInfo()
	{
		memset(bFunctionBind, 0, sizeof(bool) * KS_END);
		bGlobal = false;
	}

}BindActionInfo, *PBindActionInfo;


class ENGINE_DLL InputManager
{
private:
	IDirectInput8*	m_pInput;
	IDirectInputDevice8*	m_pKeyboard;
	IDirectInputDevice8*	m_pMouseDevice;
	//IDirectInputDevice8*	m_pJoyStick;

	unsigned char			m_Key[256];
	unsigned char			m_AddKeyList[256];				// 去系吉 徹 鯉系
	int						m_iAddKeySize;					// 去系吉 徹 鯵呪
	KeyInfo					m_tKeyInfo[256];				// 乞窮徹 壕伸.. 喚形遭 徹 葵拭 喚袈澗走 焼観走 妊獣馬檎吉陥.
	
	short					m_sWheel;						// 原酔什 蕃 淫恵, 蕃拭企廃 戚坤闘亜 級嬢臣凶 紫遂 2郊戚闘昔 戚政澗-> 脊径閤聖凶 鋼鋼梢
	DIMOUSESTATE2			m_tMouseState;					// 原酔什 雌殿( 推葬精 原酔什拭 企廃 獄動戚 弦焼閃辞 しけし..益暗拭 企廃暗猿走 鋼慎馬形檎 2研 紫遂)
	KeyInfo					m_tMouseInfo[8];
	
	//DIJOYSTATE2				m_tJoyStickState;				// 繕戚什峠 雌殿

	// 原酔什 疎妊 淫恵
	Vector2					m_vWindowMousePos;
	Vector2					m_vWindowMouseMove;
	Vector2					m_vViewportMousePos;
	Vector2					m_vViewportMouseMove;

	Vector2					m_vViewportUIMousePos;
	Vector2					m_vViewportUIMouseMove;

	// 原酔什遂 神崎詮闘
	class GameObject*		m_pMouseObj;
	class ColliderRay*		m_pMouseRay;


public:
	void SetWheel(short _sWheel);
	short GetWheelDir() const;
	bool MousePress(MOUSE_BUTTON _eMouseButton);
	bool MousePush(MOUSE_BUTTON _eMouseButton);
	bool MouseRelease(MOUSE_BUTTON _eMouseButton);

	Vector2 GetMouseWindowPos()	const;
	Vector2 GetMouseWindowMove() const;
	Vector2 GetMouseViewportPos() const;
	Vector2 GetMouseViewportMove() const;
	Vector2 GetUIMouseViewportPos() const;
	Vector2 GetUIMouseViewportMove() const;

	bool MouseRayEnable() const;
	class ColliderRay* GetMouseRay() const;

public:
	void ChangeMouseScene(class Scene* _pScene);

private:
	std::unordered_map<std::string, PBindAxisInfo> m_mapAxis;
	std::unordered_map<std::string, PBindActionInfo> m_mapAction;

public:
	bool Init(HINSTANCE _hInst, HWND _hWnd);
	void Update(float _fTime);
	void Render(float _fTime);
	PBindAxisInfo FindAxis(const std::string& _strName);
	PBindActionInfo FindAction(const std::string& _strName);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	bool ReadJoyStick();

private:
	void UpdateKeyboard(float _fTime);
	void UpdateMouse(float _fTime);
	void UpdateAxis(float _fTime);			// 繕杯徹亜 焼観依級 喚袈澗走 端滴
	void UpdateAction(float _fTime);		// 去系吉 繕杯徹級戚 喚袈澗走 端滴

public:
	// 徹 去系 (舘授備 徹幻 去系馬澗 依戚陥.)
	void AddAxisKey(const std::string& _strName, unsigned char _cKey, float _fScale);
	void AddActionKey(const std::string& _strName, unsigned char _cKey, int _iFKey = 0/*= FK_CTRL*/, MOUSE_BUTTON _eMouseButton = MB_NONE);

	// Axis, Action 肢薦
	void DeleteAxisKey(const std::string& _strName, unsigned char _cKey);
	//void DeleteActionKey(const std::string& _strName);

public:
	// 背雁 徹研 達焼辞 穿蝕徹昔走 実特馬奄
	void SetAxisGlobal(const std::string& _strName, bool _bGlobal)
	{
		PBindAxisInfo pBind = FindAxis(_strName);

		if (nullptr == pBind)
		{
			return;
		}

		pBind->bGlobal = _bGlobal;
	}

	// 徹人 敗呪 尻衣背爽奄
	void BindAxis(const std::string& _strName, void(*_pFunc)(float, float))
	{
		PBindAxisInfo pBind = FindAxis(_strName);

		// 徹亜 蒸生檎 歯稽 幻級嬢層陥.
		if (nullptr == pBind)
		{
			pBind = new BindAxisInfo;
			pBind->strName = _strName;
			m_mapAxis.insert(std::make_pair(_strName, pBind));
		}

		pBind->Func = std::bind(_pFunc, std::placeholders::_1, std::placeholders::_2);
		pBind->bFunctionBind = true;
	}

	template <typename T>
	void BindAxis(const std::string& _strName, T* _pObject, void(T::*_pFunc)(float, float))
	{
		PBindAxisInfo pBind = FindAxis(_strName);

		if (nullptr == pBind)
		{
			pBind = new BindAxisInfo;
			pBind->strName = _strName;
			m_mapAxis.insert(std::make_pair(_strName, pBind));
		}

		pBind->Func = std::bind(_pFunc, _pObject, std::placeholders::_1, std::placeholders::_2);
		pBind->bFunctionBind = true;
	}


public:
	// 繕杯徹 淫恵
	void SetActionGlobal(const std::string& _strName, bool _bGlobla)
	{
		PBindActionInfo pBind = FindAction(_strName);

		if (nullptr == pBind)
		{
			return;
		}

		pBind->bGlobal = true;
	}

	void BindAction(const std::string& _strName, KEY_STATE _eKeyState, void(*_pFunc)(float))
	{
		PBindActionInfo pBind = FindAction(_strName);

		if (nullptr == pBind)
		{
			pBind = new BindActionInfo;
			pBind->strName = _strName;
			m_mapAction.insert(std::make_pair(_strName, pBind));
		}

		pBind->Func[_eKeyState] = std::bind(_pFunc, std::placeholders::_1);
		pBind->bFunctionBind[_eKeyState] = true;
	}

	template <typename T>
	void BindAction(const std::string& _strName, 
		KEY_STATE _eKeyState, T* _pObject, void(T::*_pFunc)(float))
	{
		PBindActionInfo pBind = FindAction(_strName);

		if (nullptr == pBind)
		{
			pBind = new BindActionInfo;
			pBind->strName = _strName;
			m_mapAction.insert(std::make_pair(_strName, pBind));
		}

		pBind->Func[_eKeyState] = std::bind(_pFunc, _pObject, std::placeholders::_1);
		pBind->bFunctionBind[_eKeyState] = true;
	}

public:
	DECLARE_SINGLETON(InputManager);
};

ENGINE_END

