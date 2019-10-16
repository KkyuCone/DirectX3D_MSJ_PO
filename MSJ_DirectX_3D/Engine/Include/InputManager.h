#pragma once
#include "Engine.h"

ENGINE_BEGIN

// ���콺 ���� DIK�� �����Ƿ�.. DIK�� �ӽ÷� �������.
#define DIK_LBUTTON 0xfd
#define DIK_RBUTTON 0xfe
#define DIK_MBUTTON	0xff

typedef struct ENGINE_DLL _tagKeyInfo
{
	bool	bPress;		// Ű�� ������ �ִ���
	bool	bPush;		// Ű�� ��������
	bool	bRelease;	// ����´��� (�ȴ����°�)

	_tagKeyInfo()
	{
		bPress = false;
		bPush = false;
		bRelease = false;
	}
}KeyInfo, *PKeyInfo;

// Key�� ���õ� Ŭ������ Call Back �Լ� ���·� ������� ������
// Client���� �ش� Ű�� ������ �� Ű�� ����� �Լ��� ȣ���� ���̴�. ( event �Լ�ó�� �����ϸ�� )
// Axis�� ������ �ִ� ��� (��� ������ �ִ°��) function<�ð���, �����ϰ�> , ex ) �̵�Ű
// Action�� ������ ���, ������ , function<�ð���>		ex) ��ųŰ ��

// void�� �޴� �����¾ �Լ��� ������ �𸣱⶧���̴� ������
// ���� Axis�� �Է¹��� Ű�� �Լ��� ���ε�Ǿ��ִ� ���θ� �߰����ش�.

typedef struct ENGINE_DLL _tagKeyScale
{
	unsigned char cKey;		// ���Ű�� char�� ���� ���ִ�. ( Ű�� 256��, 0~255)
	float fScale;
}KeyScale, *PKeyScale;

typedef struct ENGINE_DLL _tagBindAxisInfo
{
	std::string strName;
	bool bFunctionBind;									// �Լ��� ����Ǿ��ִ��� ����
	std::function<void(float, float)>	Func;			// ����� �Լ�
	std::list<PKeyScale> KeyList;
	bool bGlobal;				// �ٸ����鿡���� �������� ������ ����

	_tagBindAxisInfo()
	{
		bFunctionBind = false;
		bGlobal = false;
	}

	~_tagBindAxisInfo()
	{
		// �����Ҵ� �Ȱ� �������ֱ� ������
		Safe_Delete_VectorList(KeyList);
	}

}BindAxisInfo, *PBindAxisInfo;

// �׼� ���� (����Ű ����)
typedef struct ENGINE_DLL _tagBindActionKeyInfo
{
	MOUSE_BUTTON	eMouseType;					// ���콺 ��(����, ������, ���)
	unsigned char	cKey;
	bool			bFKKey[FK_END];				// ����Ű�� �ִ��� ���� 
	KeyInfo			tKeyInfo;
	//bool			bKeyState[KS_END];			// Ű����
	//bool			bFKKeyState[FK_END][KS_END];	// ����Ű ���� [����Ű][Ű����]

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
	bool bFunctionBind[KS_END];				// �Լ��� �����ִ��� ����
	std::function<void(float)>		Func[KS_END];			// �굵 �� �Լ��������� (Press, Push, Release �϶��� �ҷ��� �Լ��� )
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
	unsigned char			m_AddKeyList[256];				// ��ϵ� Ű ���
	int						m_iAddKeySize;					// ��ϵ� Ű ����
	KeyInfo					m_tKeyInfo[256];				// ���Ű �迭.. ������ Ű ���� �������� �ƴ��� ǥ���ϸ�ȴ�.
	
	short					m_sWheel;						// ���콺 �� ����, �ٿ����� �̺�Ʈ�� ���ö� ��� 2����Ʈ�� ������-> �Է¹����� �ݹݾ�
	DIMOUSESTATE2			m_tMouseState;					// ���콺 ����( ������ ���콺�� ���� ��ư�� �������� ������..�װſ� ���Ѱű��� �ݿ��Ϸ��� 2�� ���)
	KeyInfo					m_tMouseInfo[8];
	
	//DIJOYSTATE2				m_tJoyStickState;				// ���̽�ƽ ����

	// ���콺 ��ǥ ����
	Vector2					m_vWindowMousePos;
	Vector2					m_vWindowMouseMove;
	Vector2					m_vViewportMousePos;
	Vector2					m_vViewportMouseMove;

	Vector2					m_vViewportUIMousePos;
	Vector2					m_vViewportUIMouseMove;

	// ���콺�� ������Ʈ
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
	void UpdateAxis(float _fTime);			// ����Ű�� �ƴѰ͵� �������� üũ
	void UpdateAction(float _fTime);		// ��ϵ� ����Ű���� �������� üũ

public:
	// Ű ��� (�ܼ��� Ű�� ����ϴ� ���̴�.)
	void AddAxisKey(const std::string& _strName, unsigned char _cKey, float _fScale);
	void AddActionKey(const std::string& _strName, unsigned char _cKey, int _iFKey = 0/*= FK_CTRL*/, MOUSE_BUTTON _eMouseButton = MB_NONE);

	// Axis, Action ����
	void DeleteAxisKey(const std::string& _strName, unsigned char _cKey);
	//void DeleteActionKey(const std::string& _strName);

public:
	// �ش� Ű�� ã�Ƽ� ����Ű���� �����ϱ�
	void SetAxisGlobal(const std::string& _strName, bool _bGlobal)
	{
		PBindAxisInfo pBind = FindAxis(_strName);

		if (nullptr == pBind)
		{
			return;
		}

		pBind->bGlobal = _bGlobal;
	}

	// Ű�� �Լ� �������ֱ�
	void BindAxis(const std::string& _strName, void(*_pFunc)(float, float))
	{
		PBindAxisInfo pBind = FindAxis(_strName);

		// Ű�� ������ ���� ������ش�.
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
	// ����Ű ����
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

