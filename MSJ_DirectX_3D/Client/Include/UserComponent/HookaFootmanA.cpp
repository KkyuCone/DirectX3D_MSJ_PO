#include "HookaFootmanA.h"
#include "Device.h"
#include "Scene\Scene.h"
#include "Scene\Layer.h"
#include "Time.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Component\ColliderSphere.h"
#include "Component\ColliderOBB.h"
#include "Component\Camera.h"
#include "Component\TPCamera.h"
#include "Component\Particle.h"
#include "Render\RenderManager.h"
#include "Navigation\NavigationManager.h"
#include "Navigation\NavigationMesh.h"
#include "PivotHitCol.h"
#include "Player.h"
#include "PivotHitCol.h"

HookaFootmanA::HookaFootmanA() :
	m_fSpeed(10.0f)
	, m_pHookaRenderer(nullptr)
	, m_pHookaMaterial(nullptr)
	, m_pHookaAnimation(nullptr)
	, m_pShotCollider(nullptr) , m_pHitObejctScript(nullptr), m_pTraceCollider(nullptr)
	, m_pPlayerTrnasform(nullptr)
	, m_pNavigation(nullptr)
	, m_pPivotObject(nullptr)
	, m_bTrace(false), m_pTraceObject(nullptr)
	, m_pHPObject(nullptr)
	, m_bDeath(false), m_bShot(false), m_bAttack(false)
	, m_pHitSound(nullptr)
	, m_pShotSound(nullptr)
{
	m_eAK = AK_HOOKA;
	m_eState = BaseState::IDLE;

	m_tUIState.m_iLevel = 20;
	m_tUIState.m_iHP = 20000;
	m_tUIState.m_iMP = 1;
	m_tUIState.m_iCurEXP = 100;

	m_tUIState.m_iHPMax = 20000;
	m_tUIState.m_iMPMax = 1;
	m_tUIState.m_iMaxEXP = 100;

	m_fCurWalkTime = 0.0f;
	m_fWalkTime = 0.0f;
}

HookaFootmanA::HookaFootmanA(const HookaFootmanA & _HookaFootmanA) : Base(_HookaFootmanA)
{
	*this = _HookaFootmanA;
	m_pScene = _HookaFootmanA.m_pScene;
	m_pLayer = _HookaFootmanA.m_pLayer;
	m_iReferenceCount = 1;

	m_pHookaMaterial = nullptr;
	m_pHookaRenderer = nullptr;
	m_pHookaAnimation = nullptr;
	m_pNavigation = nullptr;
	m_bTrace = false;

	m_pPivotObject = nullptr;
	m_pHPObject = nullptr;
	m_bShot = false;

	m_pHitSound = nullptr;
	m_pShotSound = nullptr;
}


HookaFootmanA::~HookaFootmanA()
{
	SAFE_RELEASE(m_pHookaMaterial);
	SAFE_RELEASE(m_pHookaRenderer);

	SAFE_RELEASE(m_pPlayerTrnasform);

	SAFE_RELEASE(m_pNavigation);
	SAFE_RELEASE(m_pHookaAnimation);

	SAFE_RELEASE(m_pTraceCollider);
	SAFE_RELEASE(m_pShotCollider);

	SAFE_RELEASE(m_pPivotObject);
	SAFE_RELEASE(m_pHPObject);

	SAFE_RELEASE(m_pHitSound);
	SAFE_RELEASE(m_pShotSound);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void HookaFootmanA::Start()
{
	SAFE_RELEASE(m_pHookaRenderer);
	m_pHookaRenderer = m_pGameObject->FindComponentFromType<Renderer>(CT_RENDERER);

	SAFE_RELEASE(m_pHookaMaterial);
	m_pHookaMaterial = m_pGameObject->FindComponentFromType<Material>(CT_MATERIAL);

	SAFE_RELEASE(m_pHookaAnimation);
	m_pHookaAnimation = m_pGameObject->FindComponentFromType<Animation>(CT_ANIMATION);

	SAFE_RELEASE(m_pNavigation);
	m_pNavigation = m_pGameObject->FindComponentFromType<Navigation>(CT_NAVIGATION);
	m_pNavigation->SetMouseMove(false);

	if (nullptr != m_pNavigation)
	{
		SAFE_RELEASE(m_pTraceCollider);
		m_pTraceCollider = m_pGameObject->FindComponentFromTag<ColliderSphere>("Trace");

		m_pTraceCollider->SetCallback<HookaFootmanA>(CCBS_ENTER, this, &HookaFootmanA::TraceEnter);
		m_pTraceCollider->SetCallback<HookaFootmanA>(CCBS_STAY, this, &HookaFootmanA::TraceStay);
		m_pTraceCollider->SetCallback<HookaFootmanA>(CCBS_EXIT, this, &HookaFootmanA::TraceExit);
		m_pTraceCollider->SetScaleEnable(false);
	}

	SAFE_RELEASE(m_pShotCollider);
	m_pShotCollider = m_pGameObject->FindComponentFromTag<ColliderSphere>("HookaShotCol");
	m_pShotCollider->SetCallback<HookaFootmanA>(CCBS_ENTER, this, &HookaFootmanA::ShotEnter);
	m_pShotCollider->SetCallback<HookaFootmanA>(CCBS_STAY, this, &HookaFootmanA::ShotStay);
	m_pShotCollider->SetCallback<HookaFootmanA>(CCBS_EXIT, this, &HookaFootmanA::ShotExit);
	m_pShotCollider->SetScaleEnable(false);

	m_pTransform->SetLookAtActive(false);

	// 사운드 초기화
	SAFE_RELEASE(m_pHitSound);
	m_pHitSound = m_pGameObject->FindComponentFromTag<AudioSound>("HookaHitSound");
	//m_pHitSound->Play();

	SAFE_RELEASE(m_pShotSound);
	m_pShotSound = m_pGameObject->FindComponentFromTag<AudioSound>("HookaShotSound");
}

bool HookaFootmanA::Init()
{
	m_pTransform->SetWorldScale(0.06f, 0.06f, 0.06f);

	m_pHookaRenderer = m_pGameObject->AddComponent<Renderer>("HookaRenderer");
	m_pHookaRenderer->SetMesh("MonsterHooka", TEXT("HookaFootman_A.msh"), Vector3::Axis[AXIS_X], PATH_MESH);

	m_pHookaMaterial = m_pGameObject->FindComponentFromType<Material>(CT_MATERIAL);

	// 추적용 콜라이더
	m_pTraceCollider = m_pGameObject->AddComponent<ColliderSphere>("Trace");
	m_pTraceCollider->SetProfile("Trace");
	m_pTraceCollider->SetChannel("Trace");
	m_pTraceCollider->SetSphereInfo(Vector3::Zero, 20.0f);
	m_pTraceCollider->SetScaleEnable(false);
	m_pTraceCollider->PickEnable(false);

	// 일반 콜라이더 (피격 )
	m_pShotCollider = m_pGameObject->AddComponent<ColliderSphere>("HookaShotCol");
	m_pShotCollider->SetProfile("Enemy");
	m_pShotCollider->SetChannel("Enemy");
	m_pShotCollider->SetSphereInfo(Vector3(0.0f, 2.2f, 0.0f), 1.8f);
	m_pShotCollider->SetScaleEnable(false);
	m_pShotCollider->PickEnable(false);

	// 애니메이션 셋팅
	m_pHookaAnimation = m_pGameObject->FindComponentFromType<Animation>(CT_ANIMATION);

	if (nullptr == m_pHookaAnimation)
	{
		m_pHookaAnimation = m_pGameObject->AddComponent<Animation>("HookaAni");
		m_pHookaAnimation->LoadBone("HookaFootman_A.bne");
		m_pHookaAnimation->Load("HookaFootman_A.anm");
		m_pHookaAnimation->ChangeClip("Idle");
	}

	m_pNavigation = m_pGameObject->AddComponent<Navigation>("Nav");

	m_pHitSound = m_pGameObject->AddComponent<AudioSound>("HookaHitSound");
	m_pHitSound->SetSound("HookaHitSound", m_pScene, false, "WHOOSH_Miss_mono.wav", PATH_SOUND_CHAR_ATTACK);
	m_pHitSound->SetVolume(0.7f);
	//m_pHitSound->Play();

	m_pShotSound = m_pGameObject->AddComponent<AudioSound>("HookaShotSound");
	m_pShotSound->SetSound("HookaShotSound", m_pScene, false, "HAMMER_Hit_Body_Gore_stereo.wav", PATH_SOUND_CHAR_ATTACK);
	m_pShotSound->SetVolume(0.05f);

	return true;
}

int HookaFootmanA::Input(float _fTime)
{
	return 0;
}

int HookaFootmanA::Update(float _fTime)
{
	//m_pHookaMaterial->SetEmissiveColor(0.f);

	// 1. 사망여부 검사
	// 2. 사망 X -> 피격 여부 검사
	if (!DeathCheck(_fTime))
	{
		ShotCheck(_fTime);
	}

	// 상태판단
	switch (m_eState)
	{
	case IDLE:
		Idle(_fTime);
		break;
	case WALK:
		Walk(_fTime);
		break;
	case RUN:
		if (true == m_bTrace)
		{
			m_pTransform->LookAt(m_pTraceObject);
			m_pTransform->SetLookAtActive(true);
		}

		Run(_fTime);
		break;
	case TALK:
		Talk(_fTime);
		break;
	case DANCE_Q:
		Dance_Q(_fTime);
		break;
	case DEATH:
		Death(_fTime);
		break;
	case ATK01:
		Attack01(_fTime);
		break;
	case ATK02:
		Attack02(_fTime);
		break;
	case SHOT:
		Shot(_fTime);
		break;
	default:
		break;
	}
	return 0;
}

int HookaFootmanA::LateUpdate(float _fTime)
{
	// 피봇이 이동되는 애니메이션의 경우 해당 오브젝트를 
	// 끝 프레임에서 이동시켜준다.
	MovePivotPos(_fTime);
	//m_bShot = false;
	return 0;
}

int HookaFootmanA::Collision(float _fTime)
{
	return 0;
}

int HookaFootmanA::PrevRender(float _fTime)
{
	return 0;
}

int HookaFootmanA::Render(float _fTime)
{
	return 0;
}

HookaFootmanA * HookaFootmanA::Clone() const
{
	return new HookaFootmanA(*this);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void HookaFootmanA::Idle(float _fTime)
{
	// Idle -> Run, Walk(자유 이동), Wait, Talk 
	ChangeAnimation("Idle");

	if (true == m_pHookaAnimation->IsAnimationEnd())
	{
		// 랜덤 - Walk, Wait, Talk 로 상태변경
		int RandomNum = rand() % 5;

		switch (RandomNum)
		{
		case 0:
		case 1:
		case 2:
			m_eState = BaseState::WALK;
			SetWalkStateDir();
			break;
		case 3:
			m_eState = BaseState::WAIT;
			break;
		case 4:
			m_eState = BaseState::TALK;
			break;
		default:
			break;
		}
	}
}

void HookaFootmanA::Walk(float _fTime)
{
	// Walk -> Walk(다른방향 or 같은방향), Idle, Run
	ChangeAnimation("Walk");

	// Walk애니메이션에서 끝에 도달했을 때 70% 확률로 또 걷게 한다.

	if (m_fCurWalkTime >= m_fWalkTime)
	{
		std::random_device seed;
		std::default_random_engine eng(seed());
		std::bernoulli_distribution Check(0.7f);
		bool bReWalk = Check(eng);

		if (true == bReWalk)
		{
			m_eState = BaseState::WALK;
			SetWalkStateDir();
		}
		else
		{
			m_eState = BaseState::IDLE;
			m_fCurWalkTime = 0.0f;
		}
	}
	else
	{
		m_fCurWalkTime += _fTime;
		m_pTransform->Move(AXIS_X, 1.0f, _fTime);
	}

}

void HookaFootmanA::Run(float _fTime)
{
	// 추적 모드일때만 Run
	ChangeAnimation("Run");
}

void HookaFootmanA::Wait(float _fTime)
{
	ChangeAnimation("Wait");
	ChangeIdleState();
}

void HookaFootmanA::Talk(float _fTime)
{
	ChangeAnimation("Talk");
	ChangeIdleState();
}

void HookaFootmanA::Dance_Q(float _fTime)
{
	ChangeAnimation("DanceQ");
	ChangeIdleState();
}

void HookaFootmanA::Death(float _fTime)
{
	ChangeAnimation("Death");
	m_bTrace = false;
	m_pTransform->SetLookAtActive(false);
	m_pNavigation->SetAIFindPath(false);

	if (true == m_pHookaAnimation->IsAnimationEnd())
	{
		m_pGameObject->Active(false);
	}
}

void HookaFootmanA::Attack01(float _fTime)
{
	PANIMATIONCLIP pCurClip = m_pHookaAnimation->GetCurrentClip();

	if ("Atk03" == pCurClip->strName)
	{
		//pCurClip->iChangeFrame
		// 이미 돌리고 있는 애니메이션이라면
		if (pCurClip->iChangeFrame >= 20 && pCurClip->iChangeFrame < 30)
		{
			m_pHitSound->Play();
		}
	}

	ChangeAnimation("Atk03");
	// 타격할지 여부 정하기
	if (true == ChangeIdleState())
	{
		m_bAttack = false;
	}
}

void HookaFootmanA::Attack02(float _fTime)
{
	PANIMATIONCLIP pCurClip = m_pHookaAnimation->GetCurrentClip();

	if ("Atk02" == pCurClip->strName)
	{
		//pCurClip->iChangeFrame
		// 이미 돌리고 있는 애니메이션이라면
		if (pCurClip->iChangeFrame >= 12 && pCurClip->iChangeFrame < 22)
		{
			m_pHitSound->Play();
		}
	}

	ChangeAnimation("Atk02");
	if (true == ChangeIdleState())
	{
		m_bAttack = false;
	}
}

void HookaFootmanA::Shot(float _fTime)
{
	ChangeAnimation("EatQ");
	if (ChangeIdleState())
	{
		Vector4 vColor = Vector4(10.0f, 10.0f, 10.0f, 1.0f);
		m_pHookaMaterial->SetColor(vColor, vColor, vColor, Vector4::One, 5.0f);
		m_bShot = false;
	}
}

void HookaFootmanA::SetPivotObject(GameObject * _pAniMoveObject)
{
	m_pPivotObject = _pAniMoveObject;
}

void HookaFootmanA::SetHPBarObject(GameObject * _pHPBar)
{
	//_pHPBar->AddReference();
	m_pHPObject = _pHPBar;
}

void HookaFootmanA::SetHitCollider(PivotHitCol * _HitCol)
{
	m_pHitObejctScript = _HitCol;
	ColliderSphere* pHitCol = m_pHitObejctScript->GetHitCollider();
	pHitCol->SetCallback<HookaFootmanA>(CCBS_ENTER, this, &HookaFootmanA::HitEnter);
	pHitCol->SetCallback<HookaFootmanA>(CCBS_STAY, this, &HookaFootmanA::HitStay);
	pHitCol->SetCallback<HookaFootmanA>(CCBS_EXIT, this, &HookaFootmanA::HitExit);
	pHitCol->SetScaleEnable(false);
}

GameObject * HookaFootmanA::GetHPBarObject()
{
	return m_pHPObject;
}

// 추적
void HookaFootmanA::TraceEnter(Collider * _pSrc, Collider * _pDest, float _fTime)
{
	if ("PlayerBody" == _pDest->GetTag())
	{
		if (true == m_bDeath)
		{
			return;
		}

		// 공격모드일 경우에는 추적을 잠깐 멈춘다.
		if (m_eState == BaseState::ATK01 || m_eState == BaseState::ATK02)
		{
			return;
		}

		// 거리 제한
		Transform* pDestTR = _pDest->GetTransform();
		float fDistance = m_pTransform->GetWorldPositionAtMatrix().Distance(pDestTR->GetWorldPositionAtMatrix());

		if (3.0f > fDistance)
		{
			RandomAttackState(_fTime);
			m_bTrace = false;
			m_pNavigation->SetAIFindPath(false);
		}
		else
		{
			m_eState = BaseState::RUN;
			GameObject* pTraceObject = _pDest->GetGameObject();
			m_pTransform->LookAt(pTraceObject);
			m_pTransform->SetLookAtActive(true);
			m_pNavigation->SetAIFindPath(true);
			m_pNavigation->SetTarget(pTraceObject);
			m_pNavigation->SetTargetDetectTime(1.0f);
			m_pTraceObject = pTraceObject;
			m_bTrace = true;
			SAFE_RELEASE(pTraceObject);
		}
		SAFE_RELEASE(pDestTR);
	}
}

void HookaFootmanA::TraceStay(Collider * _pSrc, Collider * _pDest, float _fTime)
{
	if ("PlayerBody" == _pDest->GetTag())
	{
		if (false == m_bTrace)
		{
			return;
		}
		 
		if (nullptr == m_pNavigation->GetTarget())
		{
			return;
		}

		// 공격모드일 경우에는 추적을 잠깐 멈춘다.
		if (m_eState == BaseState::ATK01 || m_eState == BaseState::ATK02)
		{
			return;
		}

		// 거리 제한
		Transform* pDestTR = _pDest->GetTransform();
		float fDistance = m_pTransform->GetWorldPositionAtMatrix().Distance(pDestTR->GetWorldPositionAtMatrix());

		if (3.0f > fDistance)
		{
			RandomAttackState(_fTime);
			m_pNavigation->SetAIFindPath(false);
		}
		else
		{
			m_eState = BaseState::RUN;
			m_pNavigation->SetAIFindPath(true);
		}
		SAFE_RELEASE(pDestTR);
	}
}

void HookaFootmanA::TraceExit(Collider * _pSrc, Collider * _pDest, float _fTime)
{
	if ("PlayerBody" == _pDest->GetTag())
	{
		m_pTraceObject = nullptr;
		m_bTrace = false;
		m_eState = BaseState::IDLE;
		m_pTransform->SetLookAtActive(false);
		m_pNavigation->SetAIFindPath(false);
		m_pNavigation->SetTargetDetectTime(0.0f);
		m_pNavigation->SetTarget(m_pTraceObject);
	}
}

void HookaFootmanA::ShotEnter(Collider * _pSrc, Collider * _pDest, float _fTime)
{
	GameObject* pHitObject = _pDest->GetGameObject();

	if ("PlayerWeaponHitCol" == pHitObject->GetTag())
	{
		PivotHitCol* pHitCol = pHitObject->FindComponentFromTag<PivotHitCol>("PlayerWeaponHitCol");
		if (true == pHitCol->GetAttack() && false == m_bShot)
		{
			m_bShot = true;
			Damage(2500);
		}
		SAFE_RELEASE(pHitCol);
	}

	SAFE_RELEASE(pHitObject);
}

void HookaFootmanA::ShotStay(Collider * _pSrc, Collider * _pDest, float _fTime)
{
	GameObject* pHitObject = _pDest->GetGameObject();

	if ("PlayerWeaponHitCol" == pHitObject->GetTag())
	{
		PivotHitCol* pHitCol = pHitObject->FindComponentFromTag<PivotHitCol>("PlayerWeaponHitCol");
		if (true == pHitCol->GetAttack() && false == m_bShot)
		{
			m_bShot = true;
			Damage(2500);
		}
		SAFE_RELEASE(pHitCol);
	}

	SAFE_RELEASE(pHitObject);
}

void HookaFootmanA::ShotExit(Collider * _pSrc, Collider * _pDest, float _fTime)
{
}

void HookaFootmanA::HitEnter(Collider * _pSrc, Collider * _pDest, float _fTime)
{
	// 대상이 플레이어Shot 콜라이더가 아니면
	if (_pDest->GetTag() != "PlayerShotCol")
	{
		return;
	}

	if (m_bAttack == true)
	{
		// 공격 모션일때만 데미지 입히기
		GameObject* pDestObject = _pDest->GetGameObject();
		GameObject* pParentObj = pDestObject->GetParent();
		PANIMATIONCLIP pCurClip = m_pHookaAnimation->GetCurrentClip();
		Player* pPlayerScript = pParentObj->FindComponentFromTag<Player>("Player");
		m_pPlayerScript = pPlayerScript;
		if (pCurClip->strName != "Atk02" && pCurClip->strName != "Atk03")
		{
			m_bAttack = false;
			SAFE_RELEASE(pDestObject);
			SAFE_RELEASE(pPlayerScript);
			return;
		}
		if (m_eState == BaseState::ATK01)
		{
			if (pCurClip->iChangeFrame >= 141 && pCurClip->iChangeFrame < 163)
			{
				pPlayerScript->Damage(1000);
			}
		}
		else if (m_eState == BaseState::ATK02)
		{
			if (pCurClip->iChangeFrame >= 102 && pCurClip->iChangeFrame < 109)
			{
				pPlayerScript->Damage(1500);
			}
		}

		SAFE_RELEASE(pDestObject);
		SAFE_RELEASE(pPlayerScript);
	}
}

void HookaFootmanA::HitStay(Collider * _pSrc, Collider * _pDest, float _fTime)
{
	if (_pDest->GetTag() != "PlayerShotCol")
	{
		return;
	}

	if (m_bAttack == true)
	{
		// 공격 모션일때만 데미지 입히기
		GameObject* pDestObject = _pDest->GetGameObject();
		GameObject* pParentObj = pDestObject->GetParent();
		PANIMATIONCLIP pCurClip = m_pHookaAnimation->GetCurrentClip();
		Player* pPlayerScript = pParentObj->FindComponentFromTag<Player>("Player");
		m_pPlayerScript = pPlayerScript;
		if (pCurClip->strName != "Atk02" && pCurClip->strName != "Atk03")
		{
			m_bAttack = false;
			SAFE_RELEASE(pDestObject);
			SAFE_RELEASE(pPlayerScript);
			return;
		}
		if (m_eState == BaseState::ATK01)
		{
			if (pCurClip->iChangeFrame >= 12 && pCurClip->iChangeFrame < 34)
			{
				pPlayerScript->Damage(1000);
				m_bAttack = false;
			}
		}
		else if (m_eState == BaseState::ATK02)
		{
			if (pCurClip->iChangeFrame >= 14 && pCurClip->iChangeFrame < 21)
			{
				pPlayerScript->Damage(1500);
				m_bAttack = false;
			}
		}

		SAFE_RELEASE(pDestObject);
		SAFE_RELEASE(pPlayerScript);
	}
}

void HookaFootmanA::HitExit(Collider * _pSrc, Collider * _pDest, float _fTime)
{
}

bool HookaFootmanA::ChangeIdleState()
{
	// 해당 애니메이션 실행이 끝나면 바로 Idle모드 변경
	if (true == m_pHookaAnimation->IsAnimationEnd())
	{
		m_eState = BaseState::IDLE;
		return true;
	}
	return false;
}

void HookaFootmanA::ChangeAnimation(std::string _strName)
{
	// 애니메이션 바꾸기
	PANIMATIONCLIP pCurAniClip = m_pHookaAnimation->GetCurrentClip();

	if (pCurAniClip->strName != _strName)
	{
		m_pHookaAnimation->ChangeClip(_strName);
	}
}

void HookaFootmanA::MovePivotPos(float _fTime)
{
	if (nullptr == m_pPivotObject)
		return;

	PANIMATIONCLIP pCurAniClip = m_pHookaAnimation->GetCurrentClip();

	if ("Atk01" != pCurAniClip->strName)
	{
		return;
	}

	// 해당 피봇 위치로 이동
	if (true == m_pHookaAnimation->IsAnimationEnd())
	{
		Transform* pPivotTr = m_pPivotObject->GetTransform();
		Vector3 vPivot = pPivotTr->GetPrevWorldPosiitonAtMatrix();
		m_pTransform->SetWorldPosition(vPivot);
		SAFE_RELEASE(pPivotTr);
	}
}

void HookaFootmanA::SetWalkStateDir()
{
	// 배회할때는 임의의 위치를 지정해서 도달하게끔 한다.
	// 회전은 Y 회전을 기준으로한다.
	float fWanderRadius = 2.0f;					// 현재 위치로부터 목표 지점까지의 거리
	int iWanderYRot = 0;
	int iWanderYRotMin = 0;
	int iWanderYRotMax = (rand() % 180) + 180;	// 180 ~ 360

	// 랜덤 회전 방향 추출 (Y축)
	std::uniform_int_distribution<int> YRotRange(iWanderYRotMin, iWanderYRotMax);
	std::mt19937_64 rn;
	iWanderYRot = YRotRange(rn);

	m_pTransform->RotateY((float)iWanderYRot);

	int iWalkTime = rand() % 4;
	m_fWalkTime = (float)iWalkTime;

	m_fCurWalkTime = 0.0f;
}

void HookaFootmanA::RandomAttackState(float _fTime)
{
	m_bAttack = true;
	//m_eState = BaseState::ATK02;
	std::random_device seed;
	std::default_random_engine eng(seed());
	std::bernoulli_distribution Check(0.6f);
	bool bAttack01 = Check(eng);

	if (true == bAttack01)
	{
		m_eState = BaseState::ATK01;
	}
	else
	{
		m_eState = BaseState::ATK02;
	}
}
bool HookaFootmanA::DeathCheck(float _fTime)
{
	if (m_bDeath)
	{
		m_eState = BaseState::DEATH;
		m_pNavigation->SetAIFindPath(false);
		return true;
	}

	return false;
}
bool HookaFootmanA::ShotCheck(float _fTime)
{
	if (m_bShot)
	{
		m_eState = BaseState::SHOT;
		m_pNavigation->SetAIFindPath(false);
		return true;
	}
	return false;
}
void HookaFootmanA::Damage(int _Value)
{
	m_eState = BaseState::SHOT;
	int iCurHP = m_tUIState.m_iHP;
	iCurHP -= _Value;

	if (iCurHP <= 0)
	{
		// 사망
		m_pPlayerScript->AddEXP(2000);
		m_tUIState.m_iHP = 0;
		m_eState = BaseState::DEATH;
		m_bDeath = true;
	}
	else
	{
		m_pShotSound->Play();
		m_tUIState.m_iHP = iCurHP;
	}

	UpdateHPBarPercent();

	Vector4 vColor = Vector4(20.0f, 0.0f, 0.0f, 1.0f);
	m_pHookaMaterial->SetColor(vColor, vColor, vColor, Vector4::One, 20.0f);
}

void HookaFootmanA::Heal(int _Value)
{
	int iCurHP = m_tUIState.m_iHP;
	iCurHP += _Value;

	if (iCurHP >= m_tUIState.m_iHP)
	{
		// 피통 꽉차면 
		m_tUIState.m_iHP = m_tUIState.m_iHPMax;
	}
	else
	{
		m_tUIState.m_iHP = iCurHP;
	}

	UpdateHPBarPercent();
}

void HookaFootmanA::UpdateHPBarPercent()
{
	// UI 처리
	if (nullptr == m_pHPObject)
	{
		return;
	}

	Particle* pParticle = m_pHPObject->FindComponentFromType<Particle>(CT_PARTICLE);
	if (pParticle == nullptr)
	{
		return;
	}

	float fPercent = 0.0f;
	fPercent = (float)(m_tUIState.m_iHP) / (float)(m_tUIState.m_iHPMax);
	pParticle->UpdatePercent(fPercent);

	if (fPercent == 0.0f)
	{
		m_pHPObject->Enable(false);
	}

	SAFE_RELEASE(pParticle);
}