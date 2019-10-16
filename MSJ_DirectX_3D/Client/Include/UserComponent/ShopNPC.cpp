#include "ShopNPC.h"
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


ShopNPC::ShopNPC() :
	m_pNPCRenderer(nullptr), m_pNPCMaterial(nullptr), m_pNPCAniamtion(nullptr)
	, m_pNPCCollider(nullptr), pNPCBtManager(nullptr)
{
}

ShopNPC::ShopNPC(const ShopNPC & _ShopNPC)
{
	*this = _ShopNPC;
	m_pScene = _ShopNPC.m_pScene;
	m_pLayer = _ShopNPC.m_pLayer;
	m_iReferenceCount = 1;

	m_pNPCRenderer = nullptr;
	m_pNPCMaterial = nullptr;
	m_pNPCAniamtion = nullptr;
}


ShopNPC::~ShopNPC()
{
	SAFE_RELEASE(m_pNPCRenderer);
	SAFE_RELEASE(m_pNPCMaterial);
	SAFE_RELEASE(m_pNPCAniamtion);
	SAFE_RELEASE(m_pNPCCollider);
}

void ShopNPC::Start()
{
	SAFE_RELEASE(m_pNPCRenderer);
	m_pNPCRenderer = m_pGameObject->FindComponentFromType<Renderer>(CT_RENDERER);

	SAFE_RELEASE(m_pNPCMaterial);
	m_pNPCMaterial = m_pGameObject->FindComponentFromType<Material>(CT_MATERIAL);

	SAFE_RELEASE(m_pNPCAniamtion);
	m_pNPCAniamtion = m_pGameObject->FindComponentFromType<Animation>(CT_ANIMATION);

	SAFE_RELEASE(m_pNPCCollider);
	m_pNPCCollider = m_pGameObject->FindComponentFromTag<ColliderSphere>("ShopNPCCollider");
	m_pNPCCollider->SetCallback<ShopNPC>(CCBS_ENTER, this, &ShopNPC::NPCColldierEnter);
	m_pNPCCollider->SetCallback<ShopNPC>(CCBS_STAY, this, &ShopNPC::NPCColldierStay);
	m_pNPCCollider->SetCallback<ShopNPC>(CCBS_EXIT, this, &ShopNPC::NPCColldierExit);
	m_pNPCCollider->SetScaleEnable(false);
}

bool ShopNPC::Init()
{
	ComponentInit();
	return true;
}

int ShopNPC::Input(float _fTime)
{
	return 0;
}

int ShopNPC::Update(float _fTime)
{
	return 0;
}

int ShopNPC::LateUpdate(float _fTime)
{
	return 0;
}

int ShopNPC::Collision(float _fTime)
{
	return 0;
}

int ShopNPC::PrevRender(float _fTime)
{
	return 0;
}

int ShopNPC::Render(float _fTime)
{
	return 0;
}

ShopNPC * ShopNPC::Clone() const
{
	return new ShopNPC(*this);
}

void ShopNPC::ComponentInit()
{
	m_pTransform->SetWorldScale(0.05f, 0.05f, 0.05f);
	m_pTransform->SetWorldPosition(Vector3::Zero);

	m_pNPCRenderer = m_pGameObject->AddComponent<Renderer>("ShopNPCRenderer");
	m_pNPCRenderer->SetMesh("Highelf_M_Alchemist", TEXT("Highelf_M_Alchemist.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	m_pNPCMaterial = m_pGameObject->FindComponentFromType<Material>(CT_MATERIAL);

	// 플레이어와 충돌용도 충돌체
	m_pNPCCollider = m_pGameObject->AddComponent<ColliderSphere>("ShopNPCCollider");
	m_pNPCCollider->SetProfile("NPC");
	m_pNPCCollider->SetChannel("NPC");
	m_pNPCCollider->SetSphereInfo(Vector3(0.0f, 3.0f, 0.0f), 5.0f);
	m_pNPCCollider->SetScaleEnable(false);
	m_pNPCCollider->PickEnable(false);

	m_pNPCAniamtion = m_pGameObject->FindComponentFromType<Animation>(CT_ANIMATION);
	if (nullptr == m_pNPCAniamtion)
	{
		m_pNPCAniamtion = m_pGameObject->AddComponent<Animation>("ShopNPCAni");
		m_pNPCAniamtion->LoadBone("Highelf_M_Alchemist.bne");
		m_pNPCAniamtion->Load("Highelf_M_Alchemist.anm");
		m_pNPCAniamtion->ChangeClip("Wait");
	}
}

void ShopNPC::SetNPCBtScript(NPCTalkBtManager * _Script)
{
	pNPCBtManager = _Script;
}

void ShopNPC::NPCColldierEnter(Collider * _pSrc, Collider * _pDest, float _fTime)
{
}

void ShopNPC::NPCColldierStay(Collider * _pSrc, Collider * _pDest, float _fTime)
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
				pNPCBtManager->NPCButtonON(NPCKind::NPC_SHOP);
			}
		}
		SAFE_RELEASE(pScript);
		SAFE_RELEASE(pObject);
	}
}

void ShopNPC::NPCColldierExit(Collider * _pSrc, Collider * _pDest, float _fTime)
{
}