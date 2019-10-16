#pragma once
#include "Component\Collider.h"
#include "Component\ColliderSphere.h"
#include "Base.h"

ENGINE_USING

class QuestAngelStatue : public Base
{
private:
	ColliderSphere*	m_pQuestCollider;

public:
	QuestAngelStatue();
	QuestAngelStatue(const QuestAngelStatue& _QuestAngelStatue);
	~QuestAngelStatue();

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);
	virtual int Render(float _fTime);
	virtual QuestAngelStatue* Clone()	const;

private:
	void ColliderEnter(Collider * _pSrc, Collider * _pDest, float _fTime);
	void ColliderStay(Collider * _pSrc, Collider * _pDest, float _fTime);
	void ColliderExit(Collider * _pSrc, Collider * _pDest, float _fTime);
};

