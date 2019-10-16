#pragma once
#include "GameObject.h"
#include "Component\UserComponent.h"
#include "Component\Renderer.h"
#include "Component\Transform.h"
#include "Component\Collider.h"
#include "Component\ColliderSphere.h"
#include "Component\ColliderOBB.h"
#include "Component\Material.h"
#include "Component\Animation.h"
#include "Component\Navigation.h"

ENGINE_USING

class Monster : public UserComponent
{
private:
	float m_BulletReTime;
	float m_BulletCurTime;

private:
	Transform* pTransform;
	Renderer* m_pRenderer;
	ColliderSphere* m_pSphereCollider;
	ColliderSphere* m_pTraceCollider;
	ColliderOBB*	m_pOBBCollider;
	Material* m_pMaterial;

	Animation* m_pAnimation;
	Navigation* m_pNavigation;

private:
	// �ٸ� ������Ʈ
	Transform* m_pPlayerTrnasform;

public:
	Monster();
	Monster(const Monster& _Monster);
	~Monster();

public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��, �� ���̾�� ȣ�����ش�.
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);	// ������ �ϱ� ������ ȣ�����ִ� �Լ��Ĥ���
	virtual int Render(float _fTime);
	virtual Monster* Clone()	const;

public:
	void Trace(Collider* _pSrc, Collider* _pDest, float _fTime);
	void TraceExit(Collider* _pSrc, Collider* _pDest, float _fTime);

private:
	void Fire(Vector3 _Dir);
};

