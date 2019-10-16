#include "MovePivot.h"
#include "Scene\Scene.h"
#include "Scene\Layer.h"
#include "Time.h"
#include "GameObject.h"
#include "InputManager.h"


MovePivot::MovePivot()
{
	m_pHitCol = nullptr;
}

MovePivot::MovePivot(const MovePivot & _MovePivot)
{
	*this = _MovePivot;
	m_pLayer = _MovePivot.m_pLayer;
	m_pScene = _MovePivot.m_pScene;
	m_pHitCol = nullptr;
	m_iReferenceCount = 1;
}

MovePivot::~MovePivot()
{
	SAFE_RELEASE(m_pHitCol);
}

void MovePivot::Start()
{
	/*SAFE_RELEASE(m_pHitCol);
	m_pHitCol = m_pGameObject->FindComponentFromTag<ColliderSphere>("HookaShotCol");*/
}

bool MovePivot::Init()
{
	ComponentInit();
	return true;
}

int MovePivot::Input(float _fTime)
{
	return 0;
}

int MovePivot::Update(float _fTime)
{
	return 0;
}

int MovePivot::LateUpdate(float _fTime)
{
	return 0;
}

int MovePivot::Collision(float _fTime)
{
	return 0;
}

int MovePivot::PrevRender(float _fTime)
{
	return 0;
}

int MovePivot::Render(float _fTime)
{
	return 0;
}

MovePivot * MovePivot::Clone() const
{
	return new MovePivot(*this);
}

void MovePivot::ComponentInit()
{
	m_pTransform->SetWorldScale(1.0f, 1.0f, 1.0f);

	//m_pHitCol = m_pGameObject->AddComponent<ColliderSphere>("HookaShotCol");
	//m_pHitCol->SetProfile("Enemy");
	//m_pHitCol->SetChannel("Enemy");
	//m_pHitCol->SetSphereInfo(Vector3(0.0f, 2.2f, 0.0f), 1.8f);
	//m_pHitCol->SetScaleEnable(false);
	//m_pHitCol->PickEnable(false);
}
