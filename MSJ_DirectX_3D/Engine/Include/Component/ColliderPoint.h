#pragma once
#include "Collider.h"

ENGINE_BEGIN

class ENGINE_DLL ColliderPoint :
	public Collider
{
private:
	friend class GameObject;

protected:
	ColliderPoint();
	ColliderPoint(ColliderPoint& _Com);
	virtual ~ColliderPoint();

public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��, �� ���̾�� ȣ�����ش�.
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual bool Collision(Collider* _pDest);
	virtual int PrevRender(float _fTime);	// ������ �ϱ� ������ ȣ�����ִ� �Լ��Ĥ���
	virtual int Render(float _fTime);
	virtual ColliderPoint* Clone()	const;
};

ENGINE_END
