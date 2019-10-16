#include "QuestAngelStatue.h"
#include "Device.h"
#include "Scene\Scene.h"
#include "Scene\Layer.h"
#include "Time.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Component\ColliderSphere.h"
#include "Player.h"


QuestAngelStatue::QuestAngelStatue() : m_pQuestCollider(nullptr)
{
}

QuestAngelStatue::QuestAngelStatue(const QuestAngelStatue & _QuestAngelStatue)
{
	*this = _QuestAngelStatue;
	m_pScene = _QuestAngelStatue.m_pScene;
	m_pLayer = _QuestAngelStatue.m_pLayer;
	m_iReferenceCount = 1;
	m_pQuestCollider = nullptr;
}


QuestAngelStatue::~QuestAngelStatue()
{
	SAFE_RELEASE(m_pQuestCollider);
}

void QuestAngelStatue::Start()
{
	SAFE_RELEASE(m_pQuestCollider);
	m_pQuestCollider = m_pGameObject->FindComponentFromTag<ColliderSphere>("AngelCollider");
	m_pQuestCollider->SetCallback<QuestAngelStatue>(CCBS_ENTER, this, &QuestAngelStatue::ColliderEnter);
	m_pQuestCollider->SetCallback<QuestAngelStatue>(CCBS_STAY, this, &QuestAngelStatue::ColliderStay);
	m_pQuestCollider->SetCallback<QuestAngelStatue>(CCBS_EXIT, this, &QuestAngelStatue::ColliderExit);
	m_pQuestCollider->SetScaleEnable(false);
}

bool QuestAngelStatue::Init()
{
	m_pTransform->SetWorldPosition(339.0f, 0.0f, 262.3f);

	m_pQuestCollider = m_pGameObject->AddComponent<ColliderSphere>("AngelCollider");
	m_pQuestCollider->SetProfile("NPC");
	m_pQuestCollider->SetChannel("NPC");
	m_pQuestCollider->SetSphereInfo(Vector3(0.0f, 3.0f, 0.0f), 20.0f);
	m_pQuestCollider->SetScaleEnable(false);
	m_pQuestCollider->PickEnable(false);
	return true;
}

int QuestAngelStatue::Input(float _fTime)
{
	return 0;
}

int QuestAngelStatue::Update(float _fTime)
{
	return 0;
}

int QuestAngelStatue::LateUpdate(float _fTime)
{
	return 0;
}

int QuestAngelStatue::Collision(float _fTime)
{
	return 0;
}

int QuestAngelStatue::PrevRender(float _fTime)
{
	return 0;
}

int QuestAngelStatue::Render(float _fTime)
{
	return 0;
}

QuestAngelStatue * QuestAngelStatue::Clone() const
{
	return nullptr;
}

void QuestAngelStatue::ColliderEnter(Collider * _pSrc, Collider * _pDest, float _fTime)
{
}

void QuestAngelStatue::ColliderStay(Collider * _pSrc, Collider * _pDest, float _fTime)
{
}

void QuestAngelStatue::ColliderExit(Collider * _pSrc, Collider * _pDest, float _fTime)
{
}
