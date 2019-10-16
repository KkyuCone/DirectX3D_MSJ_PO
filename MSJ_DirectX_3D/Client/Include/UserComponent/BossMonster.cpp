#include "BossMonster.h"
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
#include "BossUIState.h"
#include "Player.h"


BossMonster::BossMonster() :
	m_pUIBossObject(nullptr), m_pUIBossScript(nullptr)
	, m_pBossRenderer(nullptr), m_pBossMaterial(nullptr), m_pBossAnimation(nullptr), m_pBossNavigation(nullptr)
	,  m_pTraceCollider(nullptr)
	, m_pPivotObject(nullptr)
	, m_pLeftHandColObject(nullptr), m_pRightHandColObject(nullptr), m_pHitLHandScript(nullptr), m_pHitRHandScript(nullptr)
	, m_pTraceObject(nullptr), m_pHPUIEnableCollider(nullptr)
	, m_pHitSound(nullptr)
	, m_pShotSound(nullptr)
{
	m_eAK = AK_BOSS;
	m_eState = BaseState::IDLE;

	m_tUIState.m_iLevel = 20;
	m_tUIState.m_iHP = 50000;
	m_tUIState.m_iMP = 1;
	m_tUIState.m_iCurEXP = 100;

	m_tUIState.m_iHPMax = 50000;
	m_tUIState.m_iMPMax = 1;
	m_tUIState.m_iMaxEXP = 100;

	m_fCurWalkTime = 0.0f;
	m_fWalkTime = 3.0f;

	m_bDotHeal = false;
	m_fCurDotHealTime = false;
	m_bAttack = false;
}

BossMonster::BossMonster(const BossMonster & _BossMonster)
{
	*this = _BossMonster;
	m_pUIBossObject = nullptr;
	m_pUIBossScript = nullptr;

	m_pBossMaterial = nullptr;
	m_pBossRenderer = nullptr;
	m_pBossAnimation = nullptr;
	m_pBossNavigation = nullptr;
	m_bTrace = false;
	m_bDotHeal = false;

	m_pPivotObject = nullptr;
	m_pHitSound = nullptr;
	m_pShotSound = nullptr;
}


BossMonster::~BossMonster()
{
	SAFE_RELEASE(m_pBossMaterial);
	SAFE_RELEASE(m_pBossRenderer);
	SAFE_RELEASE(m_pBossAnimation);
	SAFE_RELEASE(m_pBossNavigation);
	SAFE_RELEASE(m_pTraceCollider);

	SAFE_RELEASE(m_pShotCollider);
	SAFE_RELEASE(m_pHPUIEnableCollider);

	SAFE_RELEASE(m_pHitSound);
	SAFE_RELEASE(m_pShotSound);
}

void BossMonster::Start()
{
	SAFE_RELEASE(m_pBossRenderer);
	m_pBossRenderer = m_pGameObject->FindComponentFromType<Renderer>(CT_RENDERER);
	
	SAFE_RELEASE(m_pBossMaterial);
	m_pBossMaterial = m_pGameObject->FindComponentFromType<Material>(CT_MATERIAL);

	SAFE_RELEASE(m_pBossAnimation);
	m_pBossAnimation = m_pGameObject->FindComponentFromType<Animation>(CT_ANIMATION);

	SAFE_RELEASE(m_pBossNavigation);
	m_pBossNavigation = m_pGameObject->FindComponentFromType<Navigation>(CT_NAVIGATION);

	if (nullptr != m_pBossNavigation)
	{
		SAFE_RELEASE(m_pTraceCollider);
		m_pTraceCollider = m_pGameObject->FindComponentFromTag<ColliderSphere>("Trace");

		m_pTraceCollider->SetCallback<BossMonster>(CCBS_ENTER, this, &BossMonster::TraceEnter);
		m_pTraceCollider->SetCallback<BossMonster>(CCBS_STAY, this, &BossMonster::TraceStay);
		m_pTraceCollider->SetCallback<BossMonster>(CCBS_EXIT, this, &BossMonster::TraceExit);
		m_pTraceCollider->SetScaleEnable(false);
	}

	//SAFE_RELEASE(m_pShotCollider);
	//m_pShotCollider = m_pGameObject->FindComponentFromTag<ColliderSphere>("BossShotCol");
	//m_pShotCollider->SetCallback<BossMonster>(CCBS_ENTER, this, &BossMonster::ShotEnter);
	//m_pShotCollider->SetCallback<BossMonster>(CCBS_STAY, this, &BossMonster::ShotStay);
	//m_pShotCollider->SetCallback<BossMonster>(CCBS_EXIT, this, &BossMonster::ShotExit);
	//m_pShotCollider->SetScaleEnable(false);

	SAFE_RELEASE(m_pHPUIEnableCollider);
	m_pHPUIEnableCollider = m_pGameObject->FindComponentFromTag<ColliderSphere>("BossHPUICol");
	m_pHPUIEnableCollider->SetCallback<BossMonster>(CCBS_ENTER, this, &BossMonster::UIHPEnter);
	m_pHPUIEnableCollider->SetCallback<BossMonster>(CCBS_STAY, this, &BossMonster::UIHPStay);
	m_pHPUIEnableCollider->SetCallback<BossMonster>(CCBS_EXIT, this, &BossMonster::UIHPExit);
	m_pHPUIEnableCollider->SetScaleEnable(false);

	m_pTransform->SetLookAtActive(false);

	SAFE_RELEASE(m_pHitSound);
	m_pHitSound = m_pGameObject->FindComponentFromTag<AudioSound>("BossHitSound");
	//m_pHitSound->Play();

	SAFE_RELEASE(m_pShotSound);
	m_pShotSound = m_pGameObject->FindComponentFromTag<AudioSound>("BossShotSound");
}

bool BossMonster::Init()
{
	ComponentInit();
	return true;
}

int BossMonster::Input(float _fTime)
{
	return 0;
}

int BossMonster::Update(float _fTime)
{
	if (true == m_bDeath)
	{
		m_eState = BaseState::DEATH;
	}
	else if(false == m_bDotHeal)
	{
		if (DotHealOnOff())
		{
			m_bDotHeal = true;
			m_eState = BaseState::BOSS_HEAL_START;
		}
	}

	if (true == m_bDotHeal)
	{
		if (m_eState == BaseState::BOSS_HEAL_START 
			|| m_eState == BaseState::BOSS_HEAL_LOOP
			|| m_eState == BaseState::BOSS_HEAL_END)
		{
			DotHeal(_fTime);
		}
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
		Run(_fTime);
		break;
	case WAIT:
		Wait(_fTime);
		break;
	case DEATH:
		Death(_fTime);
		break;
	case GROGGY:
		Death(_fTime);
		break;
	case ATK01:
		Attack01(_fTime);
		break;
	case ATK02:
		Attack02(_fTime);
		break;
	case ATK03:
		Attack03(_fTime);
		break;
	case ATK04:
		Attack04(_fTime);
		break;
	case ATK05:
		Attack05(_fTime);
		break;
	case SHOT:
		Shot(_fTime);
		break;
	case BOSS_HEAL_START:
		HealStart(_fTime);
		break;
	case BOSS_HEAL_LOOP:
		HealLoop(_fTime);
		break;
	case BOSS_HEAL_END:
		HealEnd(_fTime);
		break;
	case BOSS_MOVE_ATK:
		MoveAttack(_fTime);
		break;
	case BOSS_ROLLING_ATK_START:
		RollingAttackStart(_fTime);
		break;
	case BOSS_ROLLING_ATK_LOOP:
		RollingAttackLoop(_fTime);
		break;
	case BOSS_ROLLING_ATK_END:
		RollingAttackEnd(_fTime);
		break;
	default:
		break;
	}
	return 0;
}

int BossMonster::LateUpdate(float _fTime)
{
	return 0;
}

int BossMonster::Collision(float _fTime)
{
	return 0;
}

int BossMonster::PrevRender(float _fTime)
{
	Vector4 vColor = Vector4(20.0f, 0.0f, 0.0f, 1.0f);
	m_pBossRenderer->SetMaterialColor(vColor, vColor, vColor, Vector4::One, 5.0f);
	return 0;
}

int BossMonster::Render(float _fTime)
{
	return 0;
}

BossMonster * BossMonster::Clone() const
{
	return new BossMonster(*this);
}

void BossMonster::SetUIBossObject(GameObject * _pObject)
{
	m_pUIBossObject = _pObject;
}

void BossMonster::SetUIBossScript(BossUIState * _Script)
{
	m_pUIBossScript = _Script;
}

void BossMonster::SetPivotObject(GameObject * _pObject)
{
	m_pPivotObject = _pObject;
}

void BossMonster::SetShotObject(GameObject * _pObject)
{
	// 피격용
	m_pShotColliderObject = _pObject;
	m_pShotCollider = m_pShotColliderObject->FindComponentFromTag<ColliderSphere>("HitCol");
	m_pShotCollider->SetCallback<BossMonster>(CCBS_ENTER, this, &BossMonster::ShotEnter);
	m_pShotCollider->SetCallback<BossMonster>(CCBS_STAY, this, &BossMonster::ShotStay);
	m_pShotCollider->SetCallback<BossMonster>(CCBS_EXIT, this, &BossMonster::ShotExit);
	m_pShotCollider->SetScaleEnable(false);
}

void BossMonster::SetHitLHandCollider(GameObject * _pObject, PivotHitCol * _HitCol)
{
	m_pLeftHandColObject = _pObject;
	m_pHitLHandScript = _HitCol;
	ColliderSphere* pHitCol = m_pHitLHandScript->GetHitCollider();
	pHitCol->SetCallback<BossMonster>(CCBS_ENTER, this, &BossMonster::HitEnter);
	pHitCol->SetCallback<BossMonster>(CCBS_STAY, this, &BossMonster::HitStay);
	pHitCol->SetCallback<BossMonster>(CCBS_EXIT, this, &BossMonster::HitExit);
	pHitCol->SetScaleEnable(false);
}

void BossMonster::SetHitRHandCollider(GameObject * _pObject, PivotHitCol * _HitCol)
{
	m_pRightHandColObject = _pObject;
	m_pHitRHandScript = _HitCol;
	ColliderSphere* pHitCol = m_pHitRHandScript->GetHitCollider();
	pHitCol->SetCallback<BossMonster>(CCBS_ENTER, this, &BossMonster::HitEnter);
	pHitCol->SetCallback<BossMonster>(CCBS_STAY, this, &BossMonster::HitStay);
	pHitCol->SetCallback<BossMonster>(CCBS_EXIT, this, &BossMonster::HitExit);
	pHitCol->SetScaleEnable(false);
}

void BossMonster::ComponentInit()
{
	m_pTransform->SetWorldScale(0.06f, 0.06f, 0.06f);
	m_pBossRenderer = m_pGameObject->AddComponent<Renderer>("BossRenderer");
	m_pBossRenderer->SetMesh("Kumas_Mother", TEXT("Kumas_Mother.msh"), Vector3::Axis[AXIS_X], PATH_MESH);
	m_pBossMaterial = m_pGameObject->FindComponentFromType<Material>(CT_MATERIAL);

	// 추적용 콜라이더
	m_pTraceCollider = m_pGameObject->AddComponent<ColliderSphere>("Trace");
	m_pTraceCollider->SetProfile("Trace");
	m_pTraceCollider->SetChannel("Trace");
	m_pTraceCollider->SetSphereInfo(Vector3::Zero, 40.0f);
	m_pTraceCollider->SetScaleEnable(false);
	m_pTraceCollider->PickEnable(false);

	//// 피격용 콜라이더
	//m_pShotCollider = m_pGameObject->AddComponent<ColliderSphere>("BossShotCol");
	//m_pShotCollider->SetProfile("Enemy");
	//m_pShotCollider->SetChannel("Enemy");
	//m_pShotCollider->SetSphereInfo(Vector3::Zero, 10.0f);
	//m_pShotCollider->SetScaleEnable(false);
	//m_pShotCollider->PickEnable(false);

	// UI 띄우기용 콜라이더
	m_pHPUIEnableCollider = m_pGameObject->AddComponent<ColliderSphere>("BossHPUICol");
	m_pHPUIEnableCollider->SetProfile("Enemy");
	m_pHPUIEnableCollider->SetChannel("Enemy");
	m_pHPUIEnableCollider->SetSphereInfo(Vector3::Zero, 50.0f);
	m_pHPUIEnableCollider->SetScaleEnable(false);
	m_pHPUIEnableCollider->PickEnable(false);

	if (nullptr == m_pBossAnimation)
	{
		m_pBossAnimation = m_pGameObject->AddComponent<Animation>("BossAnimation");
		m_pBossAnimation->LoadBone("Kumas_Mother.bne");
		m_pBossAnimation->Load("Kumas_Mother.anm");
		m_pBossAnimation->ChangeClip("Groggy");
	}

	m_pBossNavigation = m_pGameObject->AddComponent<Navigation>("Nav");

	m_pHitSound = m_pGameObject->AddComponent<AudioSound>("BossHitSound");
	m_pHitSound->SetSound("BossHitSound", m_pScene, false, "WHOOSH_Miss_mono.wav", PATH_SOUND_CHAR_ATTACK);
	m_pHitSound->SetVolume(0.7f);
	//m_pHitSound->Play();

	m_pShotSound = m_pGameObject->AddComponent<AudioSound>("BossShotSound");
	m_pShotSound->SetSound("BossShotSound", m_pScene, false, "HAMMER_Hit_Body_Gore_stereo.wav", PATH_SOUND_CHAR_ATTACK);
	m_pShotSound->SetVolume(0.05f);
}


void BossMonster::MovePivotPos(float _fTime)
{
	if (nullptr == m_pPivotObject)
		return;

	PANIMATIONCLIP pCurAniClip = m_pBossAnimation->GetCurrentClip();

	// 피봇 이동하는 애니메이션만 여기서 체크한다.
	/*if ("Atk01" != pCurAniClip->strName)
	{
	return;
	}*/

	// 해당 피봇 위치로 이동
	if (true == m_pBossAnimation->IsAnimationEnd())
	{
		Transform* pPivotTr = m_pPivotObject->GetTransform();
		Vector3 vPivot = pPivotTr->GetPrevWorldPosiitonAtMatrix();
		m_pTransform->SetWorldPosition(vPivot);
		SAFE_RELEASE(pPivotTr);
	}
}

void BossMonster::TraceEnter(Collider * _pSrc, Collider * _pDest, float _fTime)
{
	if ("PlayerBody" == _pDest->GetTag())
	{
		if (true == m_bDeath || true == m_bAttack || true == m_bDotHeal)
		{
			return;
		}

		// 거리 제한
		Transform* pDestTR = _pDest->GetTransform();
		float fDistance = m_pTransform->GetWorldPositionAtMatrix().Distance(pDestTR->GetWorldPositionAtMatrix());

		if (3.0f > fDistance)
		{
			m_eState = BaseState::IDLE;
			RandomAttackState(_fTime);
			m_bTrace = false;
			m_pTransform->SetLookAtActive(false);
			m_pBossNavigation->SetAIFindPath(false);
		}
		else
		{
			if (m_pTraceObject == nullptr)
			{
				m_eState = BaseState::RUN;
				GameObject* pTraceObject = _pDest->GetGameObject();
				m_pTransform->LookAt(pTraceObject);
				m_pTransform->SetLookAtActive(true);
				m_pBossNavigation->SetAIFindPath(true);
				m_pBossNavigation->SetTarget(pTraceObject);
				m_pBossNavigation->SetTargetDetectTime(1.0f);
				m_pTraceObject = pTraceObject;
				m_bTrace = true;
				SAFE_RELEASE(pTraceObject);
			}
		}
		SAFE_RELEASE(pDestTR);
	}
}

void BossMonster::TraceStay(Collider * _pSrc, Collider * _pDest, float _fTime)
{
	if ("PlayerBody" == _pDest->GetTag())
	{
		if (false == m_bTrace || true == m_bAttack || true == m_bDotHeal)
		{
			m_pTransform->SetLookAtActive(false);
			m_pBossNavigation->SetAIFindPath(false);
			return;
		}

		if (nullptr == m_pBossNavigation->GetTarget())
		{
			m_pTransform->SetLookAtActive(false);
			m_pBossNavigation->SetAIFindPath(false);
			return;
		}

		// 거리 제한
		Transform* pDestTR = _pDest->GetTransform();
		float fDistance = m_pTransform->GetWorldPositionAtMatrix().Distance(pDestTR->GetWorldPositionAtMatrix());

		if (10.0f > fDistance)
		{
			m_eState = BaseState::IDLE;
			RandomAttackState(_fTime);
			m_pTransform->SetLookAtActive(false);
			m_pBossNavigation->SetAIFindPath(false);
			m_bTrace = false;
		}
		else
		{
			m_eState = BaseState::RUN;
			m_pTransform->SetLookAtActive(true);
			m_pBossNavigation->SetAIFindPath(true);
		}
		SAFE_RELEASE(pDestTR);
	}
}

void BossMonster::TraceExit(Collider * _pSrc, Collider * _pDest, float _fTime)
{
	if ("PlayerShotCol" == _pDest->GetTag())
	{
		m_pTraceObject = nullptr;
		m_bTrace = false;
		m_eState = BaseState::IDLE;
		m_pTransform->SetLookAtActive(false);
		m_pBossNavigation->SetAIFindPath(false);
		m_pBossNavigation->SetTargetDetectTime(0.0f);
		m_pBossNavigation->SetTarget(m_pTraceObject);
	}
}

void BossMonster::ShotEnter(Collider * _pSrc, Collider * _pDest, float _fTime)
{
	GameObject* pHitObject = _pDest->GetGameObject();

	if (pHitObject->GetTag() == "PlayerWeaponHitCol")
	{
		PivotHitCol* pHitCol = pHitObject->FindComponentFromTag<PivotHitCol>("PlayerWeaponHitCol");
		if (true == pHitCol->GetAttack() && false == m_bShot)
		{
			// 공격에따른 데미지 입히기
			GameObject* pHitColParent = pHitObject->GetParent();
			if (nullptr == pHitColParent)
			{
				SAFE_RELEASE(pHitObject);
				SAFE_RELEASE(pHitCol);
				return;
			}
			Damage(2000);
		}
		SAFE_RELEASE(pHitCol);
	}
	else if (pHitObject->GetTag() == "PlayerShotCol")
	{
		m_bTrace = false;
		m_pTransform->SetLookAtActive(false);
		m_pBossNavigation->SetAIFindPath(false);
	}

	SAFE_RELEASE(pHitObject);
}

void BossMonster::ShotStay(Collider * _pSrc, Collider * _pDest, float _fTime)
{
	GameObject* pHitObject = _pDest->GetGameObject();

	if ("PlayerWeaponHitCol" == pHitObject->GetTag())
	{
		PivotHitCol* pHitCol = pHitObject->FindComponentFromTag<PivotHitCol>("PlayerWeaponHitCol");
		if (true == pHitCol->GetAttack() && false == m_bShot)
		{
			m_bShot = true;
			Damage(2000);
		}
		SAFE_RELEASE(pHitCol);
	}
	else if (pHitObject->GetTag() == "PlayerShotCol")
	{
		m_bTrace = false;
		m_pTransform->SetLookAtActive(false);
		m_pBossNavigation->SetAIFindPath(false);
	}

	SAFE_RELEASE(pHitObject);
}

void BossMonster::ShotExit(Collider * _pSrc, Collider * _pDest, float _fTime)
{
	GameObject* pHitObject = _pDest->GetGameObject();

	if ("PlayerWeaponHitCol" == pHitObject->GetTag())
	{
		m_bShot = false;
	}

	SAFE_RELEASE(pHitObject);
}

void BossMonster::UIHPEnter(Collider * _pSrc, Collider * _pDest, float _fTime)
{
	// UI ON
	if ("PlayerShotCol" == _pDest->GetTag())
	{
		m_pUIBossScript->BossUIOnOff(true);
	}
}

void BossMonster::UIHPStay(Collider * _pSrc, Collider * _pDest, float _fTime)
{
}

void BossMonster::UIHPExit(Collider * _pSrc, Collider * _pDest, float _fTime)
{
	// UI OFF
	if ("PlayerShotCol" == _pDest->GetTag())
	{
		m_pUIBossScript->BossUIOnOff(false);
	}
}

void BossMonster::HitEnter(Collider * _pSrc, Collider * _pDest, float _fTime)
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
		PANIMATIONCLIP pCurClip = m_pBossAnimation->GetCurrentClip();
		Player* pPlayerScript = pParentObj->FindComponentFromTag<Player>("Player");

		if (m_eState == BaseState::ATK01)
		{
			if (pCurClip->iChangeFrame >= 28 && pCurClip->iChangeFrame < 69)
			{
				pPlayerScript->Damage(1000);
			}
		}
		else if (m_eState == BaseState::ATK02)
		{
			if (pCurClip->iChangeFrame >= 69 && pCurClip->iChangeFrame < 108)
			{
				pPlayerScript->Damage(1500);
			}
		}
		else if (m_eState == BaseState::ATK03)
		{
			if (pCurClip->iChangeFrame >= 50 && pCurClip->iChangeFrame < 153)
			{
				pPlayerScript->Damage(1500);
			}
		}
		else if (m_eState == BaseState::ATK04)
		{
			if (pCurClip->iChangeFrame >= 12 && pCurClip->iChangeFrame < 167)
			{
				pPlayerScript->Damage(1500);
			}
		}
		else if (m_eState == BaseState::ATK05)
		{
			if (pCurClip->iChangeFrame >= 10 && pCurClip->iChangeFrame < 106)
			{
				pPlayerScript->Damage(1500);
			}
		}
		else if (m_eState == BaseState::BOSS_ROLLING_ATK_LOOP)
		{
			if (pCurClip->iChangeFrame >= 0 && pCurClip->iChangeFrame < 30)
			{
				pPlayerScript->Damage(1500);
			}
		}

		SAFE_RELEASE(pDestObject);
		SAFE_RELEASE(pPlayerScript);
	}
}

void BossMonster::HitStay(Collider * _pSrc, Collider * _pDest, float _fTime)
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
		PANIMATIONCLIP pCurClip = m_pBossAnimation->GetCurrentClip();
		Player* pPlayerScript = pParentObj->FindComponentFromTag<Player>("Player");

		if (m_eState == BaseState::ATK01)
		{
			if (pCurClip->iChangeFrame >= 28 && pCurClip->iChangeFrame < 69)
			{
				pPlayerScript->Damage(1000);
			}
		}
		else if (m_eState == BaseState::ATK02)
		{
			if (pCurClip->iChangeFrame >= 69 && pCurClip->iChangeFrame < 108)
			{
				pPlayerScript->Damage(1500);
			}
		}
		else if (m_eState == BaseState::ATK03)
		{
			if (pCurClip->iChangeFrame >= 50 && pCurClip->iChangeFrame < 153)
			{
				pPlayerScript->Damage(1500);
			}
		}
		else if (m_eState == BaseState::ATK04)
		{
			if (pCurClip->iChangeFrame >= 12 && pCurClip->iChangeFrame < 167)
			{
				pPlayerScript->Damage(1500);
			}
		}
		else if (m_eState == BaseState::ATK05)
		{
			if (pCurClip->iChangeFrame >= 10 && pCurClip->iChangeFrame < 106)
			{
				pPlayerScript->Damage(1500);
			}
		}
		else if (m_eState == BaseState::BOSS_ROLLING_ATK_LOOP)
		{
			if (pCurClip->iChangeFrame >= 0 && pCurClip->iChangeFrame < 30)
			{
				pPlayerScript->Damage(1500);
			}
		}

		SAFE_RELEASE(pDestObject);
		SAFE_RELEASE(pPlayerScript);
	}
}

void BossMonster::HitExit(Collider * _pSrc, Collider * _pDest, float _fTime)
{
}

void BossMonster::Damage(int _Value)
{
	int iCurHP = m_tUIState.m_iHP;
	iCurHP -= _Value;
	if (iCurHP <= 0)
	{
		// 사망
		m_tUIState.m_iHP = 0;
		m_eState = BaseState::DEATH;
		m_bDeath = true;
	}
	else
	{
		m_tUIState.m_iHP = iCurHP;
		m_pShotSound->Play();
	}

	if (m_eState != BaseState::SHOT)
	{
		m_eState = BaseState::SHOT;
	}

	//Vector4 vColor = Vector4(20.0f, 0.0f, 0.0f, 1.0f);
	//m_pBossMaterial->SetColor(vColor, vColor, vColor, Vector4::One, 5.0f);
	UpdateHPBarPercent();
}

void BossMonster::Heal(int _Value)
{
	int iCurHP = m_tUIState.m_iHP;
	iCurHP += _Value;

	if (iCurHP >= m_tUIState.m_iHPMax)
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

void BossMonster::DotHeal(float _fTime)
{
	//float fPrevTime = _fTime;
	//float fAddTime = fPrevTime - m_fCurDotHealTime;
	if (2.0f < m_fCurDotHealTime)
	{
		m_fTime = m_fCurDotHealTime;
		m_fCurDotHealTime = 0.0f;
		Heal(5000);
	}
	else
	{
		float fAddTime = _fTime;
		fAddTime -= m_fCurDotHealTime;
		m_fCurDotHealTime += _fTime;
	}
}


void BossMonster::UpdateHPBarPercent()
{
	// UI 처리
	if (nullptr == m_pUIBossObject || nullptr == m_pUIBossScript)
	{
		return;
	}

	// 얜 UI Bar임 'ㅅ' 퍼센트 넣어주기!
	float fHPPercent = (float)(m_tUIState.m_iHP) / m_tUIState.m_iHPMax;
	m_pUIBossScript->SetPercent(fHPPercent);
}

void BossMonster::UpdateWalkDir()
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

void BossMonster::RandomAttackState(float _fTime)
{
	// 공격 패턴들 랜덤으로 돌려주기 ( 일반 공격 5개 구르기공격 1개)
	// 일반 랜덤난수 돌리기
	m_bAttack = true;
	
	int iAddAttack = (int)(ATK01);
	int iRandAttack = (rand() % 6) + iAddAttack;
	BaseState eRandAttack = (BaseState)(iRandAttack);
	m_eState = eRandAttack;
}

bool BossMonster::DotHealOnOff()
{
	float fHpPercent = (float)(m_tUIState.m_iHP) / m_tUIState.m_iHPMax;

	// HP가 40% 미만일 경우 
	// 30%의 확률로 도트힐 애니메이션 실행
	if (fHpPercent < 0.4f)
	{
		return RandomCheck(0.3f);
	}
	return false;
}

bool BossMonster::RandomCheck(float _fPercent)
{
	// 음수 X , 1.0보다 큰 값 X   = 잘못된 값들임
	if (_fPercent < 0.0f || _fPercent > 1.0f)
	{
		return false;
	}

	// 퍼센트에 따른 랜덤체크
	std::random_device seed;
	std::default_random_engine eng(seed());
	std::bernoulli_distribution Check(_fPercent);
	bool bRandomCheck = Check(eng);
	return bRandomCheck;
}
