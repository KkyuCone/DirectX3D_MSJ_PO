#pragma once
#include "Component\Collider.h"
#include "Component\ColliderRay.h"
#include "Component\ColliderSphere.h"
#include "Component\Navigation.h"
#include "Component\AudioSound.h"
#include "Base.h"

ENGINE_USING

class HookaFootmanA : public Base
{
private:
	BaseState		m_eState;				// 애니메이션 등 상태 관련
	BaseUIState		m_tUIState;				// HP, MP, EXP 관련
	float			m_fSpeed;
	float			m_fCurWalkTime;
	float			m_fWalkTime;				// Walk상태일때 걷는 시간
	bool			m_bTrace;					// 추적 모드 여부
	bool			m_bShot;					// 피격여부
	bool			m_bAttack;
	bool			m_bDeath;					// 사망 여부

private:
	GameObject* m_pTraceObject;
	GameObject*	m_pPivotObject;

private:
	Renderer* m_pHookaRenderer;
	Material* m_pHookaMaterial;
	Animation*	m_pHookaAnimation;

	class PivotHitCol* m_pHitObejctScript;					// 타격
	ColliderSphere* m_pShotCollider;			// 피격
	ColliderSphere* m_pTraceCollider;	
	
	Navigation* m_pNavigation;

private:
	// 다른 오브젝트
	Transform* m_pPlayerTrnasform;

private:
	// UI 관련
	GameObject* m_pHPObject;

private:
	GameObject*		m_pHitSoundObject;
	AudioSound*		m_pHitSound;

	GameObject*		m_pShotSoundObject;
	AudioSound*		m_pShotSound;

	Player* m_pPlayerScript;

public:
	HookaFootmanA();
	HookaFootmanA(const HookaFootmanA& _HookaFootmanA);
	~HookaFootmanA();

public:
	virtual void Start();				// 배치될려는 레이어에 포함될때 (배치될때) 호출, 즉 레이어에서 호출해준다.
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);	// 렌더를 하기 직전에 호출해주는 함수ㅔㄱㅁ
	virtual int Render(float _fTime);
	virtual HookaFootmanA* Clone()	const;

private:
	void Idle(float _fTime);
	void Walk(float _fTime);
	void Run(float _fTime);				// 추적상태일때만 Run
	void Wait(float _fTime);
	void Talk(float _fTime);
	void Dance_Q(float _fTime);
	void Death(float _fTime);
	void Attack01(float _fTime);
	void Attack02(float _fTime);
	void Shot(float _fTime);

public:
	void SetPivotObject(GameObject* _pAniMoveObject);
	void SetHPBarObject(GameObject* _pHPBar);
	void SetHitCollider(PivotHitCol* _HitCol);
	GameObject* GetHPBarObject();

private:
	void TraceEnter(Collider * _pSrc, Collider * _pDest, float _fTime);
	void TraceStay(Collider * _pSrc, Collider * _pDest, float _fTime);
	void TraceExit(Collider * _pSrc, Collider * _pDest, float _fTime);

	void ShotEnter(Collider * _pSrc, Collider * _pDest, float _fTime);
	void ShotStay(Collider * _pSrc, Collider * _pDest, float _fTime);
	void ShotExit(Collider * _pSrc, Collider * _pDest, float _fTime);

	void HitEnter(Collider * _pSrc, Collider * _pDest, float _fTime);
	void HitStay(Collider * _pSrc, Collider * _pDest, float _fTime);
	void HitExit(Collider * _pSrc, Collider * _pDest, float _fTime);

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

public:
	void Damage(int _Value);
	void Heal(int _Value);
};

