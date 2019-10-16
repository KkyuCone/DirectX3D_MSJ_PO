#pragma once
#include "Component\Collider.h"
#include "Component\ColliderRay.h"
#include "Base.h"

ENGINE_USING

class EnchantedDoll : public Base
{
private:
	BaseState		m_eState;				// �ִϸ��̼� �� ���� ����
	BaseUIState		m_tUIState;				// HP, MP, EXP ����
	float			m_fSpeed;
	float			m_fCurWalkTime;
	float			m_fWalkTime;				// Walk�����϶� �ȴ� �ð�
	bool			m_bTrace;					// ���� ��� ����
	bool			m_bShot;					// �ǰݿ���
	bool			m_bDeath;					// ��� ����

private:
	GameObject* m_pTraceObject;
	GameObject*	m_pPivotObject;
	int m_iPivotIndex;

private:
	Renderer* m_pDollRenderer;
	Material* m_pDollMaterial;
	Animation*	m_pDollAnimation;

	ColliderSphere* m_pShotCollider;			// �ǰ� �ݶ��̴�
	ColliderSphere* m_pTraceCollider;

	Navigation* m_pNavigation;

private:
	// �ٸ� ������Ʈ
	Transform* m_pPlayerTrnasform;

private:
	// UI ����
	GameObject* m_pHPObject;

public:
	EnchantedDoll();
	EnchantedDoll(const EnchantedDoll& _EnchantedDoll);
	~EnchantedDoll();

public:
	void SetMonsterPivotObject(GameObject* _pObject);

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);
	virtual int Render(float _fTime);
	virtual EnchantedDoll* Clone()	const;

private:
	void Idle(float _fTime);
	void Walk(float _fTime);
	void Run(float _fTime);				// ���������϶��� Run
	void Wait(float _fTime);
	void Death(float _fTime);
	void Attack01(float _fTime);
	void Attack02(float _fTime);
	void Shot(float _fTime);

public:
	void SetPivotObject(GameObject* _pAniMoveObject);
	void SetHPBarObject(GameObject* _pHPBar);
	GameObject* GetHPBarObject();

private:
	void TraceEnter(Collider * _pSrc, Collider * _pDest, float _fTime);
	void TraceStay(Collider * _pSrc, Collider * _pDest, float _fTime);
	void TraceExit(Collider * _pSrc, Collider * _pDest, float _fTime);

	void ShotEnter(Collider * _pSrc, Collider * _pDest, float _fTime);
	void ShotStay(Collider * _pSrc, Collider * _pDest, float _fTime);
	void ShotExit(Collider * _pSrc, Collider * _pDest, float _fTime);

private:
	bool ChangeIdleState();
	void ChangeAnimation(std::string _strName);

	void MovePivotPos(float _fTime);
	void SetWalkStateDir();

	void RandomAttackState(float _fTime);
	bool DeathCheck(float _fTime);
	bool ShotCheck(float _fTime);

private:
	void UpdateHPBarPercent();

private:
	void Damage(int _Value);
	void Heal(int _Value);
};

