#include "GoddessNPC.h"
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
#include "PivotHitCol.h"
#include "Player.h"
#include "NPCTalkBtManager.h"


GoddessNPC::GoddessNPC() :
	m_pNPCRenderer(nullptr), m_pNPCMaterial(nullptr), m_pNPCAniamtion(nullptr)
	, m_pNPCCollider(nullptr)
{
}

GoddessNPC::GoddessNPC(const GoddessNPC & _GoddessNPC)
{
	*this = _GoddessNPC;
	m_pScene = _GoddessNPC.m_pScene;
	m_pLayer = _GoddessNPC.m_pLayer;
	m_iReferenceCount = 1;

	m_pNPCRenderer = nullptr;
	m_pNPCMaterial = nullptr;
	m_pNPCAniamtion = nullptr;
}


GoddessNPC::~GoddessNPC()
{
	SAFE_RELEASE(m_pNPCRenderer);
	SAFE_RELEASE(m_pNPCMaterial);
	SAFE_RELEASE(m_pNPCAniamtion);
	SAFE_RELEASE(m_pNPCCollider);
}

void GoddessNPC::Start()
{
	SAFE_RELEASE(m_pNPCRenderer);
	m_pNPCRenderer = m_pGameObject->FindComponentFromType<Renderer>(CT_RENDERER);

	SAFE_RELEASE(m_pNPCMaterial);
	m_pNPCMaterial = m_pGameObject->FindComponentFromType<Material>(CT_MATERIAL);

	SAFE_RELEASE(m_pNPCAniamtion);
	m_pNPCAniamtion = m_pGameObject->FindComponentFromType<Animation>(CT_ANIMATION);

	SAFE_RELEASE(m_pNPCCollider);
	m_pNPCCollider = m_pGameObject->FindComponentFromTag<ColliderSphere>("GoddessNPCCollider");
	m_pNPCCollider->SetCallback<GoddessNPC>(CCBS_ENTER, this, &GoddessNPC::NPCColldierEnter);
	m_pNPCCollider->SetCallback<GoddessNPC>(CCBS_STAY, this, &GoddessNPC::NPCColldierStay);
	m_pNPCCollider->SetCallback<GoddessNPC>(CCBS_EXIT, this, &GoddessNPC::NPCColldierExit);
	m_pNPCCollider->SetScaleEnable(false);
}

bool GoddessNPC::Init()
{
	ComponentInit();
	return true;
}

int GoddessNPC::Input(float _fTime)
{
	return 0;
}

int GoddessNPC::Update(float _fTime)
{
	return 0;
}

int GoddessNPC::LateUpdate(float _fTime)
{
	return 0;
}

int GoddessNPC::Collision(float _fTime)
{
	return 0;
}

int GoddessNPC::PrevRender(float _fTime)
{
	return 0;
}

int GoddessNPC::Render(float _fTime)
{
	return 0;
}

GoddessNPC * GoddessNPC::Clone() const
{
	return new GoddessNPC(*this);
}

void GoddessNPC::ComponentInit()
{
	m_pTransform->SetWorldScale(0.05f, 0.05f, 0.05f);
	m_pTransform->SetWorldPosition(Vector3::Zero);

	m_pNPCRenderer = m_pGameObject->AddComponent<Renderer>("GoddessNPCRenderer");
	m_pNPCRenderer->SetMesh("Q_Mistel", TEXT("Q_Mistel.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	m_pNPCMaterial = m_pGameObject->FindComponentFromType<Material>(CT_MATERIAL);

	m_pNPCCollider = m_pGameObject->AddComponent<ColliderSphere>("GoddessNPCCollider");
	m_pNPCCollider->SetProfile("NPC");
	m_pNPCCollider->SetChannel("NPC");
	m_pNPCCollider->SetSphereInfo(Vector3(0.0f, 3.0f, 0.0f), 5.0f);
	m_pNPCCollider->SetScaleEnable(false);
	m_pNPCCollider->PickEnable(false);

	m_pNPCAniamtion = m_pGameObject->FindComponentFromType<Animation>(CT_ANIMATION);
	if (nullptr == m_pNPCAniamtion)
	{
		m_pNPCAniamtion = m_pGameObject->AddComponent<Animation>("GoddessNPCAni");
		m_pNPCAniamtion->LoadBone("Q_Mistel.bne");
		m_pNPCAniamtion->Load("Q_Mistel.anm");
		m_pNPCAniamtion->ChangeClip("Talk");
	}
}

void GoddessNPC::SetNPCBtScript(NPCTalkBtManager * _Script)
{
	pNPCBtManager = _Script;
}

void GoddessNPC::NPCColldierEnter(Collider * _pSrc, Collider * _pDest, float _fTime)
{
}

void GoddessNPC::NPCColldierStay(Collider * _pSrc, Collider * _pDest, float _fTime)
{
	// 플레이어와 충돌중인경우
	// 키입력시 (F) 대화창 고르게하기
	if (_pDest->GetTag() == "PlayerBody")
	{
		GameObject* pObject = _pDest->GetGameObject();
		Player* pScript = pObject->FindComponentFromTag<Player>("Player");
		if (true == pScript->GetFButtonClick())
		{
			// 충돌한 채로 F버튼을 눌렀을 경우
			// NPC 버튼 ON
			if (pNPCBtManager != nullptr)
			{
				pNPCBtManager->NPCButtonON(NPCKind::NPC_GODDESS);
			}
		}
		SAFE_RELEASE(pScript);
		SAFE_RELEASE(pObject);
	}
}

void GoddessNPC::NPCColldierExit(Collider * _pSrc, Collider * _pDest, float _fTime)
{
}
