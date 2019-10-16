#include "PivotHitCol.h"
#include "Scene\Scene.h"
#include "Scene\Layer.h"
#include "Time.h"
#include "GameObject.h"
#include "InputManager.h"


PivotHitCol::PivotHitCol() :
	m_pHitCol(nullptr)
{
	m_bAttack = false;
}

PivotHitCol::PivotHitCol(const PivotHitCol & _PlayerWeaponHitCol)
{
	*this = _PlayerWeaponHitCol;
	m_pLayer = _PlayerWeaponHitCol.m_pLayer;
	m_pScene = _PlayerWeaponHitCol.m_pScene;
	m_iReferenceCount = 1;
	m_pHitCol = nullptr;
}


PivotHitCol::~PivotHitCol()
{
	SAFE_RELEASE(m_pHitCol);
}

void PivotHitCol::Start()
{
}

bool PivotHitCol::Init()
{
	ComponentInit();
	return true;
}

int PivotHitCol::Input(float _fTime)
{
	return 0;
}

int PivotHitCol::Update(float _fTime)
{
	return 0;
}

int PivotHitCol::LateUpdate(float _fTime)
{
	return 0;
}

int PivotHitCol::Collision(float _fTime)
{
	return 0;
}

int PivotHitCol::PrevRender(float _fTime)
{
	return 0;
}

int PivotHitCol::Render(float _fTime)
{
	return 0;
}

PivotHitCol * PivotHitCol::Clone() const
{
	return new PivotHitCol(*this);
}

void PivotHitCol::ComponentInit()
{

	// ColliderOBB
	//m_pHitCol = m_pGameObject->AddComponent<ColliderOBB>("WeaponHitCol");
	//m_pHitCol->SetProfile("Player");
	//m_pHitCol->SetChannel("Player");
	//m_pHitCol->SetOBBInfo(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f));
	//m_pHitCol->SetCallback<PlayerWeaponHitCol>(CCBS_ENTER, this, &PlayerWeaponHitCol::HitEnter);
	//m_pHitCol->SetCallback<PlayerWeaponHitCol>(CCBS_STAY, this, &PlayerWeaponHitCol::HitStay);
	//m_pHitCol->SetCallback<PlayerWeaponHitCol>(CCBS_EXIT, this, &PlayerWeaponHitCol::HitExit);
}

void PivotHitCol::HitEnter(Collider * _pSrc, Collider * _pDest, float _fTime)
{
}

void PivotHitCol::HitStay(Collider * _pSrc, Collider * _pDest, float _fTime)
{
}

void PivotHitCol::HitExit(Collider * _pSrc, Collider * _pDest, float _fTime)
{
}

void PivotHitCol::SetbAttack(bool _bAttack)
{
	m_bAttack = _bAttack;
}

bool PivotHitCol::GetAttack()
{
	return m_bAttack;
}

void PivotHitCol::SetSoketbone(std::string _strBoneName, std::string _strProFile, std::string _strChannel, std::string _strColName/* = "HitCol"*/, bool _AutoFunc /*= true*/)
{
	//m_pTransform->SetLocalPosition(2.0f, 7.0f, -5.0f);
	m_pTransform->SetWorldScale(1.0f, 1.0f, 1.0f);
	//m_pGameObject->SetBoneSoket("Bip01-R-Hand");
	m_pGameObject->SetBoneSoket(_strBoneName);

	// ColliderSpherer
	m_pHitCol = m_pGameObject->AddComponent<ColliderSphere>(_strColName);
	m_pHitCol->SetProfile(_strProFile);
	m_pHitCol->SetChannel(_strChannel);
	m_pHitCol->SetSphereInfo(Vector3(0.0f, 0.0f, 0.0f), 0.5f);
	m_pHitCol->SetScaleEnable(false);

	if (true == _AutoFunc)
	{
		m_pHitCol->SetCallback<PivotHitCol>(CCBS_ENTER, this, &PivotHitCol::HitEnter);
		m_pHitCol->SetCallback<PivotHitCol>(CCBS_STAY, this, &PivotHitCol::HitStay);
		m_pHitCol->SetCallback<PivotHitCol>(CCBS_EXIT, this, &PivotHitCol::HitExit);
	}
}

void PivotHitCol::SetHitColInfo(const Vector3 & _vCenter, float _fRadius)
{
	m_pHitCol->SetSphereInfo(_vCenter, _fRadius);
}

ColliderSphere * PivotHitCol::GetHitCollider()
{
	return m_pHitCol;
}
