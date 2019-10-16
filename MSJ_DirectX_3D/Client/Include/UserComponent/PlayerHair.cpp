#include "PlayerHair.h"
#include "Scene\Scene.h"
#include "Scene\Layer.h"
#include "Time.h"
#include "GameObject.h"
#include "Component\Collider.h"
#include "Component\ColliderRay.h"
#include "Component\ColliderSphere.h"
#include "Component\ColliderOBB.h"
#include "Component\Navigation.h"
#include "Navigation\NavigationManager.h"
#include "Navigation\NavigationMesh.h"
#include "Render\RenderManager.h"



PlayerHair::PlayerHair() :
	m_pFaceTransform(nullptr)
	, m_pFaceRenderer(nullptr)
	, m_pFaceMaterial(nullptr)
	, m_pFaceAnimation(nullptr)
{
}

PlayerHair::PlayerHair(const PlayerHair & _Face)
{
	*this = _Face;
	m_pLayer = _Face.m_pLayer;
	m_pScene = _Face.m_pScene;
	m_iReferenceCount = 1;
}


PlayerHair::~PlayerHair()
{
	SAFE_RELEASE(m_pFaceTransform);
	SAFE_RELEASE(m_pFaceAnimation);
	SAFE_RELEASE(m_pFaceMaterial);
	SAFE_RELEASE(m_pFaceRenderer);
}

void PlayerHair::Start()
{
}

bool PlayerHair::Init()
{
	ComponentInit();
	return true;
}

int PlayerHair::Input(float _fTime)
{
	return 0;
}

int PlayerHair::Update(float _fTime)
{
	return 0;
}

int PlayerHair::LateUpdate(float _fTime)
{
	return 0;
}

int PlayerHair::Collision(float _fTime)
{
	return 0;
}

int PlayerHair::PrevRender(float _fTime)
{
	return 0;
}

int PlayerHair::Render(float _fTime)
{
	return 0;
}

PlayerHair * PlayerHair::Clone() const
{
	return nullptr;
}

void PlayerHair::ComponentInit()
{
	m_pFaceTransform = m_pGameObject->GetTransform();
	m_pFaceTransform->SetLocalPosition(1.0f, 0.0f, 0.0f);
	m_pFaceTransform->SetLocalScale(2.4f, 2.4f, 2.4f);

	m_pFaceRenderer = m_pGameObject->AddComponent<Renderer>("PlayerHeadRenderer");
	m_pFaceRenderer->SetMesh("PlayerHair", TEXT("Popori_F_Head_Hair12_Ani.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	m_pFaceMaterial = m_pGameObject->FindComponentFromType<Material>(CT_MATERIAL);
	Vector4 vColor = Vector4(3.0f, 3.0f, 3.0f, 1.0f);
	m_pFaceMaterial->SetColor(vColor, vColor, vColor, Vector4::One, 5.0f);
	m_pFaceMaterial->SetColor(vColor, vColor, vColor, Vector4::One, 5.0f, 1);
	//m_pMaterial->SetEmissiveColor(1.0f);


	m_pFaceAnimation = m_pGameObject->FindComponentFromType<Animation>(CT_ANIMATION);

	if (nullptr == m_pFaceAnimation)
	{
		m_pFaceAnimation = m_pGameObject->AddComponent<Animation>("PalyerHeadAnimation");
		m_pFaceAnimation->LoadBone("Popori_F_Head_Hair12_Ani.bne");			// 본정보 가져오기
		m_pFaceAnimation->Load("Popori_F_Head_Hair12_Ani.anm");				// 애니메이션 정보가져오기

		m_pFaceAnimation->ChangeClip("Wait");
		m_pGameObject->SetBoneSoket("Bip01-Head");
		//m_pFaceAnimation->SetParentBoneName("Bip01-Neck");
		//m_pFaceAnimation->SetParentBoneName("Bip01-Head");  // SetBoneSoket
	}

	//Navigation*	pNav = m_pGameObject->AddComponent<Navigation>("FaceNav");
	////pNav->SetAIFindPath(true);
	//pNav->SetMouseMove(false);

	//SAFE_RELEASE(pNav);


	SAFE_RELEASE(m_pFaceTransform);
}

void PlayerHair::AnimationChange(std::string _strName)
{
	m_pFaceAnimation->ChangeClip(_strName);
}
