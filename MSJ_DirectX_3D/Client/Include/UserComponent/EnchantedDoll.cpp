#include "EnchantedDoll.h"
#include "Device.h"
#include "Scene\Scene.h"
#include "Scene\Layer.h"
#include "Time.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Component\ColliderSphere.h"
#include "Component\ColliderOBB.h"
#include "Component\Navigation.h"
#include "Component\Camera.h"
#include "Component\TPCamera.h"
#include "Component\Particle.h"
#include "Render\RenderManager.h"
#include "Navigation\NavigationManager.h"
#include "Navigation\NavigationMesh.h"

EnchantedDoll::EnchantedDoll() :
	m_fSpeed(10.0f)
	, m_pPivotObject(nullptr), m_pDollRenderer(nullptr), m_pDollMaterial(nullptr), m_pDollAnimation(nullptr)
	, m_pShotCollider(nullptr), m_pTraceCollider(nullptr)
	, m_pPlayerTrnasform(nullptr)
	, m_pNavigation(nullptr)
	, m_bTrace(false), m_pTraceObject(nullptr)
	, m_pHPObject(nullptr)
	, m_bDeath(false), m_bShot(false)
{
	m_eAK = AK_DOLL;
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

EnchantedDoll::EnchantedDoll(const EnchantedDoll & _EnchantedDoll)
{
	*this = _EnchantedDoll;
	m_pScene = _EnchantedDoll.m_pScene;
	m_pLayer = _EnchantedDoll.m_pLayer;
	m_iReferenceCount = 1;

	m_pDollMaterial = nullptr;
	m_pDollRenderer = nullptr;
	m_pDollAnimation = nullptr;
	m_pNavigation = nullptr;
	m_bTrace = false;

	m_pPivotObject = nullptr;
	m_pHPObject = nullptr;
	m_bShot = false;
}


EnchantedDoll::~EnchantedDoll()
{
	SAFE_RELEASE(m_pDollMaterial);
	SAFE_RELEASE(m_pDollRenderer);

	SAFE_RELEASE(m_pPlayerTrnasform);

	SAFE_RELEASE(m_pNavigation);
	SAFE_RELEASE(m_pDollAnimation);

	SAFE_RELEASE(m_pTraceCollider);
	SAFE_RELEASE(m_pShotCollider);

	SAFE_RELEASE(m_pNavigation);

	SAFE_RELEASE(m_pPivotObject);
	SAFE_RELEASE(m_pHPObject);
}

void EnchantedDoll::SetMonsterPivotObject(GameObject * _pObject)
{
	m_pPivotObject = _pObject;
}

void EnchantedDoll::Start()
{
	SAFE_RELEASE(m_pDollRenderer);
	m_pDollRenderer = m_pGameObject->FindComponentFromType<Renderer>(CT_RENDERER);

	SAFE_RELEASE(m_pDollMaterial);
	m_pDollMaterial = m_pGameObject->FindComponentFromType<Material>(CT_MATERIAL);

	SAFE_RELEASE(m_pDollAnimation);
	m_pDollAnimation = m_pGameObject->FindComponentFromType<Animation>(CT_ANIMATION);

	SAFE_RELEASE(m_pNavigation);
	m_pNavigation = m_pGameObject->FindComponentFromType<Navigation>(CT_NAVIGATION);
	m_pNavigation->SetMouseMove(false);

	if (nullptr != m_pNavigation)
	{
		SAFE_RELEASE(m_pTraceCollider);
		m_pTraceCollider = m_pGameObject->FindComponentFromTag<ColliderSphere>("Trace");

		m_pTraceCollider->SetCallback<EnchantedDoll>(CCBS_ENTER, this, &EnchantedDoll::TraceEnter);
		m_pTraceCollider->SetCallback<EnchantedDoll>(CCBS_STAY, this, &EnchantedDoll::TraceStay);
		m_pTraceCollider->SetCallback<EnchantedDoll>(CCBS_EXIT, this, &EnchantedDoll::TraceExit);
		m_pTraceCollider->SetScaleEnable(false);
	}

	SAFE_RELEASE(m_pShotCollider);
	m_pShotCollider = m_pGameObject->FindComponentFromTag<ColliderSphere>("EnchantedDollCol");
	m_pShotCollider->SetCallback<EnchantedDoll>(CCBS_ENTER, this, &EnchantedDoll::ShotEnter);
	m_pShotCollider->SetCallback<EnchantedDoll>(CCBS_STAY, this, &EnchantedDoll::ShotStay);
	m_pShotCollider->SetCallback<EnchantedDoll>(CCBS_EXIT, this, &EnchantedDoll::ShotExit);

	m_pTransform->SetLookAtActive(false);
}

bool EnchantedDoll::Init()
{
	m_pTransform->SetWorldScale(0.06f, 0.06f, 0.06f);

	m_pDollRenderer = m_pGameObject->AddComponent<Renderer>("EnchantedDollRenderer");
	m_pDollRenderer->SetMesh("EnchantedDoll", TEXT("EnchantedDoll.msh"), Vector3::Axis[AXIS_X], PATH_MESH);

	m_pDollMaterial = m_pGameObject->FindComponentFromType<Material>(CT_MATERIAL);

	// ������ �ݶ��̴�
	m_pTraceCollider = m_pGameObject->AddComponent<ColliderSphere>("Trace");
	m_pTraceCollider->SetProfile("Trace");
	m_pTraceCollider->SetChannel("Trace");
	m_pTraceCollider->SetSphereInfo(Vector3::Zero, 20.0f);
	m_pTraceCollider->SetScaleEnable(false);
	m_pTraceCollider->PickEnable(false);

	// �Ϲ� �ݶ��̴� (�ǰ� ����)
	m_pShotCollider = m_pGameObject->AddComponent<ColliderSphere>("EnchantedDollCol");
	m_pShotCollider->SetProfile("Enemy");
	m_pShotCollider->SetChannel("Enemy");
	m_pShotCollider->SetSphereInfo(Vector3::Zero, 1.0f);
	m_pShotCollider->PickEnable(false);

	// �ִϸ��̼� ����
	m_pDollAnimation = m_pGameObject->FindComponentFromType<Animation>(CT_ANIMATION);

	if (nullptr == m_pDollAnimation)
	{
		m_pDollAnimation = m_pGameObject->AddComponent<Animation>("EnchantedDollAni");
		m_pDollAnimation->LoadBone("EnchantedDoll.bne");
		m_pDollAnimation->Load("EnchantedDoll.anm");
		m_pDollAnimation->ChangeClip("Idle");
	}

	m_pNavigation = m_pGameObject->AddComponent<Navigation>("Nav");

	return true;
}

int EnchantedDoll::Input(float _fTime)
{
	return 0;
}

int EnchantedDoll::Update(float _fTime)
{
	// 1. ������� �˻�
	// 2. ��� X -> �ǰ� ���� �˻�
	if (!DeathCheck(_fTime))
	{
		ShotCheck(_fTime);
	}

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
	case WAIT:
		Wait(_fTime);
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

int EnchantedDoll::LateUpdate(float _fTime)
{
	MovePivotPos(_fTime);
	return 0;
}

int EnchantedDoll::Collision(float _fTime)
{
	return 0;
}

int EnchantedDoll::PrevRender(float _fTime)
{
	return 0;
}

int EnchantedDoll::Render(float _fTime)
{
	return 0;
}

EnchantedDoll * EnchantedDoll::Clone() const
{
	return new EnchantedDoll(*this);
}

void EnchantedDoll::Idle(float _fTime)
{

	// Idle -> Run, Walk(���� �̵�), Wait, Talk 
	ChangeAnimation("Idle");

	if (true == m_pDollAnimation->IsAnimationEnd())
	{
		// ���� - Walk, Wait, Talk �� ���º���
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

void EnchantedDoll::Walk(float _fTime)
{
	// Walk -> Walk(�ٸ����� or ��������), Idle, Run
	ChangeAnimation("Walk");

	// Walk�ִϸ��̼ǿ��� ���� �������� �� 70% Ȯ���� �� �Ȱ� �Ѵ�.
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
			m_fCurWalkTime = 0.0f;
			m_eState = BaseState::IDLE;
		}
	}
	else
	{
		m_fCurWalkTime += _fTime;
		m_pTransform->Move(AXIS_X, 1.0f, _fTime);
	}
}

void EnchantedDoll::Run(float _fTime)
{
	ChangeAnimation("Run");
}

void EnchantedDoll::Wait(float _fTime)
{
	ChangeAnimation("Wait");
	ChangeIdleState();
}

void EnchantedDoll::Death(float _fTime)
{
	ChangeAnimation("Death");
	m_bTrace = false;
	m_pTransform->SetLookAtActive(false);
	m_pNavigation->SetAIFindPath(false);

	if (true == m_pDollAnimation->IsAnimationEnd())
	{
		m_pGameObject->Active(false);
	}
}

void EnchantedDoll::Attack01(float _fTime)
{
	ChangeAnimation("Atk01");
	ChangeIdleState();
}

void EnchantedDoll::Attack02(float _fTime)
{
	ChangeAnimation("Atk02");
	ChangeIdleState();
}

void EnchantedDoll::Shot(float _fTime)
{
	ChangeAnimation("Hit");
	if (ChangeIdleState())
	{
		m_bShot = false;
	}
}

void EnchantedDoll::SetPivotObject(GameObject * _pAniMoveObject)
{
	m_pPivotObject = _pAniMoveObject;
}

void EnchantedDoll::SetHPBarObject(GameObject * _pHPBar)
{
	m_pHPObject = _pHPBar;
}

GameObject * EnchantedDoll::GetHPBarObject()
{
	return m_pHPObject;
}

void EnchantedDoll::TraceEnter(Collider * _pSrc, Collider * _pDest, float _fTime)
{
	if ("PlayerBody" == _pDest->GetTag())
	{
		if (true == m_bDeath)
		{
			return;
		}

		// ���ݸ���� ��쿡�� ������ ��� �����.
		if (m_eState == BaseState::ATK01 || m_eState == BaseState::ATK02)
		{
			return;
		}

		// �Ÿ� ����
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

void EnchantedDoll::TraceStay(Collider * _pSrc, Collider * _pDest, float _fTime)
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

		// ���ݸ���� ��쿡�� ������ ��� �����.
		if (m_eState == BaseState::ATK01 || m_eState == BaseState::ATK02)
		{
			return;
		}

		// �Ÿ� ����
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

void EnchantedDoll::TraceExit(Collider * _pSrc, Collider * _pDest, float _fTime)
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

void EnchantedDoll::ShotEnter(Collider * _pSrc, Collider * _pDest, float _fTime)
{
	GameObject* pHitObject = _pDest->GetGameObject();

	if ("PlayerWeaponHitCol" == pHitObject->GetTag())
	{
		if (false == m_bShot)
		{
			m_bShot = true;
			Damage(1000);
		}
	}

	SAFE_RELEASE(pHitObject);
}

void EnchantedDoll::ShotStay(Collider * _pSrc, Collider * _pDest, float _fTime)
{
	GameObject* pHitObject = _pDest->GetGameObject();

	if ("PlayerWeaponHitCol" == pHitObject->GetTag())
	{
		if (false == m_bShot)
		{
			m_bShot = true;
			Damage(2000);
		}
	}

	SAFE_RELEASE(pHitObject);
}

void EnchantedDoll::ShotExit(Collider * _pSrc, Collider * _pDest, float _fTime)
{
}


bool EnchantedDoll::ChangeIdleState()
{
	// �ش� �ִϸ��̼� ������ ������ �ٷ� Idle��� ����
	if (true == m_pDollAnimation->IsAnimationEnd())
	{
		m_eState = BaseState::IDLE;
		return true;
	}
	return false;
}

void EnchantedDoll::ChangeAnimation(std::string _strName)
{
	PANIMATIONCLIP pCurAniClip = m_pDollAnimation->GetCurrentClip();

	if (pCurAniClip->strName != _strName)
	{
		m_pDollAnimation->ChangeClip(_strName);
	}
}

void EnchantedDoll::MovePivotPos(float _fTime)
{
	if (nullptr == m_pPivotObject)
		return;

	PANIMATIONCLIP pCurAniClip = m_pDollAnimation->GetCurrentClip();

	if (pCurAniClip->strName != "Atk01" && pCurAniClip->strName != "Atk02")
	{
		int a = 0;
		return;
	}

	// �ش� �Ǻ� ��ġ�� �̵�
	if (true == m_pDollAnimation->IsAnimationEnd())
	{
		Transform* pPivotTr = m_pPivotObject->GetTransform();
		Vector3 vPivot = pPivotTr->GetWorldPositionAtMatrix();
		m_pTransform->SetWorldPosition(vPivot);
		SAFE_RELEASE(pPivotTr);
	}
}

void EnchantedDoll::SetWalkStateDir()
{
	// ��ȸ�Ҷ��� ������ ��ġ�� �����ؼ� �����ϰԲ� �Ѵ�.
	// ȸ���� Y ȸ���� ���������Ѵ�.
	float fWanderRadius = 2.0f;					// ���� ��ġ�κ��� ��ǥ ���������� �Ÿ�
	int iWanderYRot = 0;
	int iWanderYRotMin = 0;
	int iWanderYRotMax = (rand() % 180) + 180;	// 180 ~ 360

												// ���� ȸ�� ���� ���� (Y��)
	std::uniform_int_distribution<int> YRotRange(iWanderYRotMin, iWanderYRotMax);
	std::mt19937_64 rn;
	iWanderYRot = YRotRange(rn);

	m_pTransform->RotateY((float)iWanderYRot);

	int iWalkTime = rand() % 4;
	m_fWalkTime = (float)iWalkTime;

	m_fCurWalkTime = 0.0f;
}

void EnchantedDoll::RandomAttackState(float _fTime)
{
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

bool EnchantedDoll::DeathCheck(float _fTime)
{
	if (m_bDeath)
	{
		m_eState = BaseState::DEATH;
		m_pNavigation->SetAIFindPath(false);
		return true;
	}

	return false;
}

bool EnchantedDoll::ShotCheck(float _fTime)
{
	if (m_bShot)
	{
		m_eState = BaseState::SHOT;
		m_pNavigation->SetAIFindPath(false);
		return true;
	}
	return false;
}
void EnchantedDoll::Damage(int _Value)
{
	int iCurHP = m_tUIState.m_iHP;
	iCurHP -= _Value;

	if (iCurHP <= 0)
	{
		// ���
		m_tUIState.m_iHP = 0;
		m_eState = BaseState::DEATH;
		m_bDeath = true;
	}
	else
	{
		m_tUIState.m_iHP = iCurHP;
	}

	UpdateHPBarPercent();
}

void EnchantedDoll::Heal(int _Value)
{
	int iCurHP = m_tUIState.m_iHP;
	iCurHP += _Value;

	if (iCurHP >= m_tUIState.m_iHP)
	{
		// ���� ������ 
		m_tUIState.m_iHP = m_tUIState.m_iHPMax;
	}
	else
	{
		m_tUIState.m_iHP = iCurHP;
	}

	UpdateHPBarPercent();
}

void EnchantedDoll::UpdateHPBarPercent()
{
	// UI ó��
	if (nullptr == m_pHPObject)
	{
		//m_pHPObject = 
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

	SAFE_RELEASE(pParticle);
}

