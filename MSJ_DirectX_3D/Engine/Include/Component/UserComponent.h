#pragma once
#include "Component.h"

ENGINE_BEGIN
// UserComponent�� ���ӿ�����Ʈ�� ��ũ��Ʈ?������ �Ѵٰ� �����ϸ� �ɵ�
class ENGINE_DLL UserComponent : public Component
{
private:
	friend class GameObject;

protected:
	UserComponent();
	UserComponent(const UserComponent& _Component);
	virtual ~UserComponent() = 0;

public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��, �� ���̾�� ȣ�����ش�.
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);	// ������ �ϱ� ������ ȣ�����ִ� �Լ��Ĥ���
	virtual int Render(float _fTime);
	virtual Component* Clone()	const;
};

ENGINE_END

