#include "Weapon.h"
#include "Scene\Scene.h"
#include "Scene\Layer.h"
#include "Time.h"
#include "GameObject.h"
#include "InputManager.h"

#include "HookaFootmanA.h"
#include "EnchantedDoll.h"
#include "BossMonster.h"


Weapon::Weapon() :
	 m_pWeaponRenderer(nullptr)
	, m_pWeaponMaterial(nullptr)
	, m_bHit(false)
{
	m_eAK = AK_PLAYER_HIT;
}

Weapon::Weapon(const Weapon & _Weapon)
{
	*this = _Weapon;
	m_pLayer = _Weapon.m_pLayer;
	m_pScene = _Weapon.m_pScene;
	m_iReferenceCount = 1;

	m_pWeaponMaterial = nullptr;
	m_pWeaponRenderer = nullptr;
}



Weapon::~Weapon()
{
	SAFE_RELEASE(m_pWeaponRenderer);
	SAFE_RELEASE(m_pWeaponMaterial);
}

void Weapon::Start()
{
}

bool Weapon::Init()
{
	ComponentInit();
	return true;
}

int Weapon::Input(float _fTime)
{
	return 0;
}

int Weapon::Update(float _fTime)
{
	return 0;
}

int Weapon::LateUpdate(float _fTime)
{
	return 0;
}

int Weapon::Collision(float _fTime)
{
	return 0;
}

int Weapon::PrevRender(float _fTime)
{
	return 0;
}

int Weapon::Render(float _fTime)
{
	return 0;
}

Weapon * Weapon::Clone() const
{
	return new Weapon(*this);
}

void Weapon::ComponentInit()
{
	m_pTransform->SetLocalPosition(2.0f, 7.0f, 1.0f);
	m_pTransform->SetWorldScale(1.0f, 1.0f, 1.0f);

	m_pWeaponRenderer = m_pGameObject->AddComponent<Renderer>("PlayerWeaponRenderer");
	m_pWeaponRenderer->SetMesh("PlayerWeapon", TEXT("PC_Weapons_29.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);
	m_pWeaponMaterial = m_pGameObject->FindComponentFromType<Material>(CT_MATERIAL);
	Vector4 vColor = Vector4(3.0f, 2.0f, 2.0f, 1.0f);
	m_pWeaponMaterial->SetColor(vColor, vColor, vColor, Vector4::One, 5.0f);

	m_pGameObject->SetBoneSoket("Bip01-R-Hand");

	SAFE_RELEASE(m_pWeaponMaterial);
	SAFE_RELEASE(m_pWeaponRenderer);
}

void Weapon::ChangeWeapon(std::string _Name)
{
	std::string strChangeWeapon = _Name;
	strChangeWeapon += ".msh";

	m_pWeaponRenderer = m_pGameObject->FindComponentFromType<Renderer>(CT_RENDERER);
	m_pWeaponRenderer->SetMesh(_Name, StringToLPWSTR(strChangeWeapon).c_str(), Vector3::Axis[AXIS_Z], PATH_MESH);
	m_pWeaponMaterial = m_pGameObject->FindComponentFromType<Material>(CT_MATERIAL);
	Vector4 vColor = Vector4(3.0f, 2.0f, 2.0f, 1.0f);
	m_pWeaponMaterial->SetColor(vColor, vColor, vColor, Vector4::One, 5.0f);
	SAFE_RELEASE(m_pWeaponMaterial);
	SAFE_RELEASE(m_pWeaponRenderer);
}
