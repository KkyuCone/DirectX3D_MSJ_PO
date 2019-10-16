#pragma once
#include "Collider.h"

ENGINE_BEGIN

class ENGINE_DLL ColliderAABB :
	public Collider
{
private:
	friend class GameObject;

protected:
	ColliderAABB();
	ColliderAABB(const ColliderAABB& _Com);
	virtual ~ColliderAABB();

public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��, �� ���̾�� ȣ�����ش�.
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual bool Collision(Collider* _pDest);
	virtual int PrevRender(float _fTime);	// ������ �ϱ� ������ ȣ�����ִ� �Լ�
	virtual int Render(float _fTime);
	virtual ColliderAABB* Clone()	const;
};

ENGINE_END
