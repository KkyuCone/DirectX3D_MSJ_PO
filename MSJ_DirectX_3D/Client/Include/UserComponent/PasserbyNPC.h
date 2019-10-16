#pragma once
#include "Component\Collider.h"
#include "Component\ColliderSphere.h"
#include "Base.h"

ENGINE_USING

class PasserbyNPC : public Base
{
private:
	bool			m_bNPCBtEnable;

private:
	Renderer*		m_pNPCRenderer;
	Material*		m_pNPCMaterial;
	Animation*		m_pNPCAniamtion;
	ColliderSphere*	m_pNPCCollider;

private:
	class NPCTalkBtManager* pNPCBtManager;

public:
	PasserbyNPC();
	PasserbyNPC(const PasserbyNPC& _PasserbyNPC);
	~PasserbyNPC();

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);
	virtual int Render(float _fTime);
	virtual PasserbyNPC* Clone()	const;

private:
	void ComponentInit();

public:
	void SetNPCBtScript(class NPCTalkBtManager* _Script);

private:
	void NPCColldierEnter(Collider * _pSrc, Collider * _pDest, float _fTime);
	void NPCColldierStay(Collider * _pSrc, Collider * _pDest, float _fTime);
	void NPCColldierExit(Collider * _pSrc, Collider * _pDest, float _fTime);
};

