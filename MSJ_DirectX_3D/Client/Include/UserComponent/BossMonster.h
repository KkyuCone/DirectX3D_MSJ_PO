#pragma once
#include "Base.h"
#include "Component\Collider.h"
#include "Component\ColliderRay.h"
#include "Component\ColliderSphere.h"
#include "Component\Navigation.h"
#include "Component\AudioSound.h"

ENGINE_USING

class BossMonster : public Base
{
private:
	BaseState		m_eState;				// �ִϸ��̼� �� ���� ����
	BaseUIState		m_tUIState;				// HP, MP, EXP ����
	float			m_fSpeed;
	float			m_fCurWalkTime;
	float			m_fWalkTime;				// Walk�����϶� �ȴ� �ð�
	float			m_fCurDotHealTime;
	float			m_fTime;
	bool			m_bTrace;					// ���� ��� ����
	bool			m_bShot;					// �ǰݿ���
	bool			m_bAttack;					// ���� ���
	bool			m_bDotHeal;					// ��Ʈ�� ����
	bool			m_bDeath;					// ��� ����

private:
	GameObject*	m_pPivotObject;					// �Ǻ��� �̵��ϴ� �ִϸ��̼� ������ �ʿ��� ���� ������Ʈ

	Renderer* m_pBossRenderer;
	Material* m_pBossMaterial;
	Animation*	m_pBossAnimation;
	Navigation* m_pBossNavigation;

	GameObject* m_pShotColliderObject;
	ColliderSphere* m_pShotCollider;			// �ǰ� �ݶ��̴�
	ColliderSphere* m_pTraceCollider;
	ColliderSphere* m_pHPUIEnableCollider;		// UI ��� ������ �ݶ��̴�

	GameObject* m_pTraceObject;

private:
	// ���� ���ݿ� �ݶ��̴� (����)
	GameObject* m_pLeftHandColObject;
	GameObject* m_pRightHandColObject;

	class PivotHitCol* m_pHitLHandScript;
	class PivotHitCol* m_pHitRHandScript;

private:
	// UI - HP ����
	GameObject* m_pUIBossObject;
	class BossUIState* m_pUIBossScript;

private:
	AudioSound*		m_pHitSound;
	AudioSound*		m_pShotSound;

public:
	BossMonster();
	BossMonster(const BossMonster& _BossMonster);
	~BossMonster();

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);
	virtual int Render(float _fTime);
	virtual BossMonster* Clone()	const;

public:
	void SetUIBossObject(GameObject* _pObject);
	void SetUIBossScript(class BossUIState* _Script);
	void SetPivotObject(GameObject* _pObject);
	void SetShotObject(GameObject* _pObject);

	void SetHitLHandCollider(GameObject* _pObject, class PivotHitCol* _HitCol);
	void SetHitRHandCollider(GameObject* _pObject, class PivotHitCol* _HitCol);

private:
	void ComponentInit();

private:
	// ���� �Լ�
	void Idle(float _fTime);
	void Walk(float _fTime);
	void Run(float _fTime);
	void Wait(float _fTime);
	void Death(float _fTime);
	void Groggy(float _fTime);

	void Attack01(float _fTime);
	void Attack02(float _fTime);
	void Attack03(float _fTime);
	void Attack04(float _fTime);
	void Attack05(float _fTime);

	void Shot(float _fTime);

	void HealStart(float _fTime);
	void HealLoop(float _fTime);
	void HealEnd(float _fTime);

	void MoveAttack(float _fTime);

	void RollingAttackStart(float _fTime);
	void RollingAttackLoop(float _fTime);
	void RollingAttackEnd(float _fTime);

private:
	void ChangeAnimation(std::string _strAniName);
	bool ChangeAnimationToIdle(float _fTime);				// ���̵� ���·� ��ȯ ( ���������϶� )

	void MovePivotPos(float _fTime);

private:
	void TraceEnter(Collider * _pSrc, Collider * _pDest, float _fTime);
	void TraceStay(Collider * _pSrc, Collider * _pDest, float _fTime);
	void TraceExit(Collider * _pSrc, Collider * _pDest, float _fTime);

	void ShotEnter(Collider * _pSrc, Collider * _pDest, float _fTime);
	void ShotStay(Collider * _pSrc, Collider * _pDest, float _fTime);
	void ShotExit(Collider * _pSrc, Collider * _pDest, float _fTime);

	void UIHPEnter(Collider * _pSrc, Collider * _pDest, float _fTime);
	void UIHPStay(Collider * _pSrc, Collider * _pDest, float _fTime);
	void UIHPExit(Collider * _pSrc, Collider * _pDest, float _fTime);

	void HitEnter(Collider * _pSrc, Collider * _pDest, float _fTime);
	void HitStay(Collider * _pSrc, Collider * _pDest, float _fTime);
	void HitExit(Collider * _pSrc, Collider * _pDest, float _fTime);

private:
	void Damage(int _Value);
	void Heal(int _iHeal);
	void DotHeal(float _fTime);
	void UpdateHPBarPercent();
	void UpdateWalkDir();
	void RandomAttackState(float _fTime);

	bool DotHealOnOff();
	bool RandomCheck(float _fPercent);
};

