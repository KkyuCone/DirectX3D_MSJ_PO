#pragma once
#include "UI.h"

ENGINE_BEGIN

// ��ư ����
enum BUTTON_STATE
{
	BS_DISABLE,		// ��Ȱ��ȭ..?
	BS_NORMAL,
	BS_MOUSEON,
	BS_CLICK,
	BS_PUSH,
	BS_RCLICK,
	BS_END
};

class ENGINE_DLL UIButton :
	public UI
{
private:
	friend class GameObject;

protected:
	UIButton();
	UIButton(const UIButton& _Com);
	virtual ~UIButton();

protected:
	BUTTON_STATE	m_ePrevState;							// ���콺 ���� ����
	BUTTON_STATE	m_eState;								// ���콺 ����
	//bool			m_bStartTextureChange;					// �ؽ��� ü���� ���� (�ؽ�ó �� ��� ó�� )
	std::function<void(float)> m_CallBackFunc[BS_END];			// ���콺 ���¿� ���� �Լ� ����

	class Texture*		m_pTexture[BS_END];					// ���콺 ���¿� ���� �ؽ�ó
	Vector4				m_vColor[BS_END];					// ���콺 ���¿� ���� ����
	class Material*		m_pMaterial;						// ��Ƽ����
	ButtonCBuffer		m_tButtonCBuffer;					// ���� ������۷� �����ؾ���

	class Renderer*		m_pRenderer;
	
public:
	// ��ư ���¿� ���� ���� ����
	void SetStateColor(BUTTON_STATE _eState, const Vector4& _vColor);

	// ��ư ���¿� ���� �ؽ�ó ����
	void SetStateTexture(BUTTON_STATE _eState, const std::string& _strName,
		const TCHAR* _pFileName, const std::string& _strPathName = PATH_TEXTURE);

public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��, �� ���̾�� ȣ�����ش�.
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);	// ������ �ϱ� ������ ȣ�����ִ� �Լ��Ĥ���
	virtual int Render(float _fTime);
	virtual UIButton* Clone()	const;

public:
	void SetCallBackFunc(BUTTON_STATE _eState, void(*_pFunc)(float));

	template <typename FuncType>
	void SetCallBackFunc(BUTTON_STATE _eState, FuncType* _pObject, void(FuncType::*_pFunc)(float))
	{
		m_CallBackFunc[_eState] = std::bind(_pFunc, _pObject, std::placeholders::_1);
	}

public:
	BUTTON_STATE GetButtonState();

private:
	bool	m_bOnlyMouseOn;
public:
	void SetPushNoneOnlyMouseON(bool _Value);
	
};

ENGINE_END
