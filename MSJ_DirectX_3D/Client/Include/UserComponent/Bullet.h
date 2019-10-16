#pragma once
#include "Component\UserComponent.h"
#include "Component\Renderer.h"
#include "Component\Transform.h"
#include "Component\ColliderSphere.h"

ENGINE_USING

class Bullet : public UserComponent
{
private:
	float m_fSpeed;
	float m_fDist;

private:
	Renderer* m_pRenderer;
	ColliderSphere* m_pColliderSphere;

public:
	Bullet();
	Bullet(const Bullet& _Bullet);
	~Bullet();

public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��, �� ���̾�� ȣ�����ش�.
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);	// ������ �ϱ� ������ ȣ�����ִ� �Լ��Ĥ���
	virtual int Render(float _fTime);
	virtual Bullet* Clone()	const;

};

