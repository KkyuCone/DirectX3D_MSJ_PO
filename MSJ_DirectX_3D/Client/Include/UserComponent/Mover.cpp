#include "Mover.h"
#include "Scene\Scene.h"
#include "Scene\Layer.h"
#include "Time.h"
#include "GameObject.h"
#include "InputManager.h"

#include "Component\Camera.h"
#include "Component\TPCamera.h"
#include "Render\RenderManager.h"


Mover::Mover() :
	m_pMoverRenderer(nullptr)
	, m_pMoverMaterial(nullptr)
	, m_pMoverAnimation(nullptr)
	, m_pTPCamera(nullptr)
	, m_pMoverNavigation(nullptr)
	, m_pSpeed(10.0f)
{
}

Mover::Mover(const Mover & _Mover)
{
}


Mover::~Mover()
{
	SAFE_RELEASE(m_pMoverRenderer);
	SAFE_RELEASE(m_pMoverMaterial);
	SAFE_RELEASE(m_pMoverAnimation);
	SAFE_RELEASE(m_pMoverNavigation);

	GET_SINGLETON(InputManager)->DeleteAxisKey("RotationLeft", DIK_A);
	GET_SINGLETON(InputManager)->DeleteAxisKey("RotationRight", DIK_D);
}

void Mover::Start()
{
	SAFE_RELEASE(m_pMoverMaterial);
	m_pMoverMaterial = m_pGameObject->FindComponentFromType<Material>(CT_MATERIAL);
	Vector4 vSColor = Vector4(3.0f, 3.0f, 3.0f, 1.0f);
	m_pMoverMaterial->SetColor(vSColor, vSColor, vSColor, Vector4::Zero, 5.0f);
}

bool Mover::Init()
{
	ComponentInit();
	InputInit();
	return true;
}

int Mover::Input(float _fTime)
{
	return 0;
}

int Mover::Update(float _fTime)
{
	return 0;
}

int Mover::LateUpdate(float _fTime)
{
	return 0;
}

int Mover::Collision(float _fTime)
{
	return 0;
}

int Mover::PrevRender(float _fTime)
{
	return 0;
}

int Mover::Render(float _fTime)
{
	return 0;
}

Mover * Mover::Clone() const
{
	return nullptr; 
}

void Mover::ComponentInit()
{
	Transform* pCurObjTr = m_pGameObject->GetTransform();
	pCurObjTr->SetWorldPosition(0.0f, 0.0f, 0.0f);
	pCurObjTr->SetWorldScale(0.10f, 0.10f, 0.10f);
	pCurObjTr->SetWorldPosition(Vector3::Zero);
	pCurObjTr->SetLocalRotation(0.0f, -95.0f, 0.0f);

	m_pMoverRenderer = m_pGameObject->AddComponent<Renderer>("MapToolMoverRenderer");
	m_pMoverRenderer->SetMesh("MapToolMoverRenderer", TEXT("Rabbit.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);
	//m_pMoverRenderer->SetMesh("MapToolMoverRenderer", TEXT("Popori_F_R31_Body.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	m_pMoverAnimation = m_pGameObject->FindComponentFromType<Animation>(CT_ANIMATION);

	if (nullptr == m_pMoverAnimation)
	{
		m_pMoverAnimation = m_pGameObject->AddComponent<Animation>("RabitAnimation");
		m_pMoverAnimation->LoadBone("Rabbit.bne");			// 본정보 가져오기
		m_pMoverAnimation->Load("Rabbit.anm");				// 애니메이션 정보가져오기

		m_pMoverAnimation->ChangeClip("Idle01");

		//Bip01-Spine3, Bip01-Spine2
		//m_pMoverAnimation->SetChangePivotBone("Bip01-Spine3");
	}

	m_pMoverMaterial = m_pGameObject->FindComponentFromType<Material>(CT_MATERIAL);

	m_pMoverNavigation = m_pGameObject->AddComponent<Navigation>("Nav");
	//pNav->SetAIFindPath(true);
	m_pMoverNavigation->SetMouseMove(true);
	m_pMoverNavigation->SetMapToolOnlyMousePos(true);

	SAFE_RELEASE(pCurObjTr);
	SAFE_RELEASE(m_pMoverRenderer);
	SAFE_RELEASE(m_pMoverMaterial);
	SAFE_RELEASE(m_pMoverAnimation);
	//SAFE_RELEASE(m_pMoverNavigation);
}

void Mover::InputInit()
{
	GET_SINGLETON(InputManager)->AddAxisKey("RotationLeft", DIK_A, -1.0f);
	GET_SINGLETON(InputManager)->AddAxisKey("RotationRight", DIK_D, 1.0f);

	GET_SINGLETON(InputManager)->BindAxis("RotationLeft", this, &Mover::Rotate);
	GET_SINGLETON(InputManager)->BindAxis("RotationRight", this, &Mover::Rotate);


	//
	GET_SINGLETON(InputManager)->AddActionKey("MoveFront", DIK_W);
	GET_SINGLETON(InputManager)->AddActionKey("MoveBack", DIK_S);
	GET_SINGLETON(InputManager)->AddActionKey("MoveUp", DIK_Q);
	GET_SINGLETON(InputManager)->AddActionKey("MoveDown", DIK_E);

	GET_SINGLETON(InputManager)->AddActionKey("MoveZERO", DIK_O);

	GET_SINGLETON(InputManager)->AddActionKey("SpeedMultiply", DIK_MULTIPLY);
	GET_SINGLETON(InputManager)->AddActionKey("SpeedDivide", DIK_DIVIDE);

	//
	GET_SINGLETON(InputManager)->BindAction("MoveFront", KS_PUSH, this, &Mover::MoveFront);
	GET_SINGLETON(InputManager)->BindAction("MoveBack", KS_PUSH, this, &Mover::MoveBack);
	GET_SINGLETON(InputManager)->BindAction("MoveUp", KS_PUSH, this, &Mover::MoveUp);
	GET_SINGLETON(InputManager)->BindAction("MoveDown", KS_PUSH, this, &Mover::MoveDown);

	GET_SINGLETON(InputManager)->BindAction("MoveZERO", KS_PRESS, this, &Mover::MoveZero);

	GET_SINGLETON(InputManager)->BindAction("SpeedMultiply", KS_PRESS, this, &Mover::SpeedMultiple);
	GET_SINGLETON(InputManager)->BindAction("SpeedDivide", KS_PRESS, this, &Mover::SpeedDivision);
}

// 회전
void Mover::Rotate(float _fScale, float _fTime)
{
	m_pTransform->RotateY(180.0f * _fScale, _fTime);
}

void Mover::SetTPCam(TPCamera * _Camera)
{
	m_pTPCamera = _Camera;
}

Vector3 Mover::GetCurWorldMousePos()
{
	if (nullptr == m_pMoverNavigation)
	{
		return Vector3::Zero;
	}

	return m_pMoverNavigation->GetMousePos();
}

// 이동
void Mover::MoveFront(float _fTime)
{
	m_pTransform->Move(AXIS_Z, m_pSpeed * 1.0f, _fTime);
}

void Mover::MoveBack(float _fTime)
{
	m_pTransform->Move(AXIS_Z, m_pSpeed * -1.0f, _fTime);
}

void Mover::MoveLeft(float _fTime)
{
	m_pTransform->Move(AXIS_X, m_pSpeed * -1.0f, _fTime);
}

void Mover::MoveRight(float _fTime)
{
	m_pTransform->Move(AXIS_X, 10.0f * 1.0f, _fTime);
}

void Mover::MoveUp(float _fTime)
{
	m_pTransform->Move(AXIS_Y, m_pSpeed * 1.0f, _fTime);
}

void Mover::MoveDown(float _fTime)
{
	m_pTransform->Move(AXIS_Y, m_pSpeed * -1.0f, _fTime);
}

void Mover::MoveZero(float _fTime)
{
	if (nullptr != m_pTPCamera)
	{
		m_pTPCamera->SetToolCamera(true);
	}
	m_pTransform->SetWorldPosition(0.0f, 0.0f, 0.0f);
}

void Mover::SpeedMultiple(float _fTime)
{
	m_pSpeed *= 2.0;
}

void Mover::SpeedDivision(float _fTime)
{
	m_pSpeed *= 0.5;
}
