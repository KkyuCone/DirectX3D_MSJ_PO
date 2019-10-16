#pragma once
#include "Component.h"

ENGINE_BEGIN

enum UI_TYPE
{
	UI_NONE = 0,
	UI_BUTTON,
	UI_IMAGE,
	UI_BAR,
	UI_TEXT,
};

class ENGINE_DLL UI : public Component
{
private:
	friend class GameObject;

protected:
	UI();
	UI(const UI& _Com);
	virtual ~UI() = 0;

public:
	UI_TYPE m_eUIType;

public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��, �� ���̾�� ȣ�����ش�.
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);	// ������ �ϱ� ������ ȣ�����ִ� �Լ��Ĥ���
	virtual int Render(float _fTime);
	virtual UI* Clone()	const;
};

ENGINE_END