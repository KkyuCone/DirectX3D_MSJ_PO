#pragma once
#include "Component/UserComponent.h"
#include "Component/UIBar.h"
#include "Component/UIText.h"

ENGINE_USING

struct State
{
	int m_iCur;			// ���� ��ġ��
	int m_iMin;
	int m_iMax;			// �ִ밪
};

class StateBar : public UserComponent
{
private:
	float m_fRatio;		// ����
	State m_State;

	class UIBar* m_StateBar;
	class UIText* m_StateText;

public:
	StateBar();
	StateBar(const StateBar& _StateBar);
	~StateBar();

public:
	UIBar* GetUIBar();
	UIText* GetStateText();

public:
	void SetMin(int _iMin);
	void SetMax(int _iMax);

	void SetUIBar(UIBar* _Bar);
	void SetUIText(UIText* _Text);

	void SetColor(Vector4 _vColor);
	void SetChangeBarShader(std::string& _strKey);
	void SetStateBarTexture(const std::string& _strName, const TCHAR* _pFileName, const std::string& _strPathName = PATH_UI_BAR);

public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��, �� ���̾�� ȣ�����ش�.
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);	// ������ �ϱ� ������ ȣ�����ִ� �Լ��Ĥ���
	virtual int Render(float _fTime);
	virtual StateBar* Clone()	const;

private:
	void UpdateRatio(float _fPercent);			// ����üũ
	void UpdateRatio();
	void ShowStateText();						// �ؽ�Ʈ �ٲٱ�

public:
	void Heal(int _Value);
	void Damage(int _Value);
};

