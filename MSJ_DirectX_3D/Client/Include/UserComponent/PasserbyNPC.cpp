#include "PasserbyNPC.h"
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

PasserbyNPC::PasserbyNPC() :
	m_bNPCBtEnable(false)
	, m_pNPCRenderer(nullptr), m_pNPCMaterial(nullptr), m_pNPCAniamtion(nullptr)
	, m_pNPCCollider(nullptr)
	, pNPCBtManager(nullptr)
{
}

PasserbyNPC::PasserbyNPC(const PasserbyNPC & _PasserbyNPC)
{
	*this = _PasserbyNPC;
	m_pScene = _PasserbyNPC.m_pScene;
	m_pLayer = _PasserbyNPC.m_pLayer;
	m_iReferenceCount = 1;

	m_pNPCRenderer = nullptr;
	m_pNPCMaterial = nullptr;
	m_pNPCAniamtion = nullptr;
}


PasserbyNPC::~PasserbyNPC()
{
	SAFE_RELEASE(m_pNPCMaterial);
	SAFE_RELEASE(m_pNPCAniamtion);
	SAFE_RELEASE(m_pNPCRenderer);
	SAFE_RELEASE(m_pNPCCollider);
}

void PasserbyNPC::Start()
{
	SAFE_RELEASE(m_pNPCRenderer);
	m_pNPCRenderer = m_pGameObject->FindComponentFromType<Renderer>(CT_RENDERER);

	SAFE_RELEASE(m_pNPCMaterial);
	m_pNPCMaterial = m_pGameObject->FindComponentFromType<Material>(CT_MATERIAL);

	SAFE_RELEASE(m_pNPCAniamtion);
	m_pNPCAniamtion = m_pGameObject->FindComponentFromType<Animation>(CT_ANIMATION);

	SAFE_RELEASE(m_pNPCCollider);
	m_pNPCCollider = m_pGameObject->FindComponentFromTag<ColliderSphere>("PasserbyNPCCollider");
	m_pNPCCollider->SetCallback<PasserbyNPC>(CCBS_ENTER, this, &PasserbyNPC::NPCColldierEnter);
	m_pNPCCollider->SetCallback<PasserbyNPC>(CCBS_STAY, this, &PasserbyNPC::NPCColldierStay);
	m_pNPCCollider->SetCallback<PasserbyNPC>(CCBS_EXIT, this, &PasserbyNPC::NPCColldierExit);
	m_pNPCCollider->SetScaleEnable(false);
}

bool PasserbyNPC::Init()
{
	ComponentInit();
	return true;
}

int PasserbyNPC::Input(float _fTime)
{
	return 0;
}

int PasserbyNPC::Update(float _fTime)
{
	return 0;
}

int PasserbyNPC::LateUpdate(float _fTime)
{
	return 0;
}

int PasserbyNPC::Collision(float _fTime)
{
	return 0;
}

int PasserbyNPC::PrevRender(float _fTime)
{
	return 0;
}

int PasserbyNPC::Render(float _fTime)
{
	return 0;
}

PasserbyNPC * PasserbyNPC::Clone() const
{
	return new PasserbyNPC(*this);
}

void PasserbyNPC::ComponentInit()
{
	m_pTransform->SetWorldScale(0.05f, 0.05f, 0.05f);
	m_pTransform->SetWorldPosition(Vector3::Zero);

	m_pNPCRenderer = m_pGameObject->AddComponent<Renderer>("PasserbyNPCRenderer");
	m_pNPCRenderer->SetMesh("Human_F_Aide", TEXT("Human_F_Aide.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	m_pNPCMaterial = m_pGameObject->FindComponentFromType<Material>(CT_MATERIAL);

	// 플레이어와 충돌용도 충돌체
	m_pNPCCollider = m_pGameObject->AddComponent<ColliderSphere>("PasserbyNPCCollider");
	m_pNPCCollider->SetProfile("NPC");
	m_pNPCCollider->SetChannel("NPC");
	m_pNPCCollider->SetSphereInfo(Vector3(0.0f, 3.0f, 0.0f), 5.0f);
	m_pNPCCollider->SetScaleEnable(false);
	m_pNPCCollider->PickEnable(false);

	m_pNPCAniamtion = m_pGameObject->FindComponentFromType<Animation>(CT_ANIMATION);
	if (nullptr == m_pNPCAniamtion)
	{
		m_pNPCAniamtion = m_pGameObject->AddComponent<Animation>("PasserbyNPCAni");
		m_pNPCAniamtion->LoadBone("Human_F_Aide.bne");
		m_pNPCAniamtion->Load("Human_F_Aide.anm");
		m_pNPCAniamtion->ChangeClip("Wait");
	}
}

void PasserbyNPC::SetNPCBtScript(NPCTalkBtManager * _Script)
{
	pNPCBtManager = _Script;
}

void PasserbyNPC::NPCColldierEnter(Collider * _pSrc, Collider * _pDest, float _fTime)
{
}

void PasserbyNPC::NPCColldierStay(Collider * _pSrc, Collider * _pDest, float _fTime)
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
				pNPCBtManager->NPCButtonON(NPCKind::NPC_PASSERBY);
			}
		}
		SAFE_RELEASE(pScript);
		SAFE_RELEASE(pObject);
	}
}

void PasserbyNPC::NPCColldierExit(Collider * _pSrc, Collider * _pDest, float _fTime)
{
}
