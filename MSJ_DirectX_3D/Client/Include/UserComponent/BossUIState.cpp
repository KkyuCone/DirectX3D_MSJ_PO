#include "BossUIState.h"
#include "Scene\Scene.h"
#include "Scene\SceneManager.h"
#include "Scene\Layer.h"
#include "Render\RenderManager.h"
#include "Device.h"
#include "Time.h"
#include "GameObject.h"
#include "PathManager.h"

BossUIState::BossUIState() :
	m_pBoss(nullptr), m_pHPBackground(nullptr), m_pHP(nullptr)
	, m_pBossImage(nullptr), m_pHPBackgorundImage(nullptr), m_pHPImage(nullptr)
{
}

BossUIState::BossUIState(const BossUIState & _BossUIState)
{
	*this = _BossUIState;
	m_iReferenceCount = 1;
}


BossUIState::~BossUIState()
{
	SAFE_RELEASE(m_pBossImage);
	SAFE_RELEASE(m_pHPBackgorundImage);
	SAFE_RELEASE(m_pHPImage);

	SAFE_RELEASE(m_pBoss);
	SAFE_RELEASE(m_pHPBackground);
	SAFE_RELEASE(m_pHP);
}

void BossUIState::Start()
{
}

bool BossUIState::Init()
{
	ComponentInit();
	return true;
}

int BossUIState::Input(float _fTime)
{
	return 0;
}

int BossUIState::Update(float _fTime)
{
	return 0;
}

int BossUIState::LateUpdate(float _fTime)
{
	return 0;
}

int BossUIState::Collision(float _fTime)
{
	return 0;
}

int BossUIState::PrevRender(float _fTime)
{
	return 0;
}

int BossUIState::Render(float _fTime)
{
	return 0;
}

BossUIState * BossUIState::Clone() const
{
	return new BossUIState(*this);
}

void BossUIState::ComponentInit()
{
	// 보스 
	m_pBoss = GameObject::CreateObject("BossUIBackground", m_pLayer);
	m_pBossImage = m_pBoss->AddComponent<UIBar>("BossUIBackground");
	m_pBossImage->SetTexture("BossUIBackground", TEXT("BossPhotoUI.png"), PATH_UI_BAR);
	m_pBossImage->SetShader(SHADER_UI_ONLY_IMAGE);

	Transform* pBackgroundTr = m_pBoss->GetTransform();
	pBackgroundTr->SetWorldPosition(_RESOLUTION.iWidth * 0.5f - 250.f, _RESOLUTION.iHeight * 0.5f + 425.0f, 1.8f);
	pBackgroundTr->SetWorldScale(80.0f, 80.0f, 1.0f);
	SAFE_RELEASE(pBackgroundTr);

	// 보스 HP 배경
	m_pHPBackground = GameObject::CreateObject("BossHPBackground", m_pLayer);
	m_pHPBackgorundImage = m_pHPBackground->AddComponent<UIBar>("BossHPBackground");
	m_pHPBackgorundImage->SetTexture("BossHPBackground", TEXT("tgb_level_empty_bar.png"), PATH_UI_BAR);
	m_pHPBackgorundImage->SetShader(SHADER_UI_ONLY_IMAGE);

	Transform* pHpBackground = m_pHPBackground->GetTransform();
	pHpBackground->SetWorldPosition(_RESOLUTION.iWidth * 0.5f - 183.0f, _RESOLUTION.iHeight * 0.5f + 430.0f, 2.0f);
	pHpBackground->SetWorldScale(536.0f, 68.0f, 1.0f);
	SAFE_RELEASE(pHpBackground);

	// 보스  HP
	m_pHP = GameObject::CreateObject("BossHP", m_pLayer);
	m_pHPImage = m_pHP->AddComponent<UIBar>("BossHP");
	m_pHPImage->SetTexture("BossHP", TEXT("MonsterHPBar.png"), PATH_UI_BAR);
	m_pHPImage->SetShader(SHADER_UI_ONLY_IMAGE);
	m_pHPImage->SetRatio(1.0f);

	Transform* pHPTr = m_pHP->GetTransform();
	pHPTr->SetWorldPosition(_RESOLUTION.iWidth * 0.5f - 170.0f, _RESOLUTION.iHeight * 0.5f + 442.5f, 1.9f);
	pHPTr->SetWorldScale(510.0f, 42.0f, 1.0f);
	SAFE_RELEASE(pHPTr);

	BossUIOnOff(false);
}

void BossUIState::SetPercent(float _fPerecent)
{
	if (_fPerecent < 0.0f || _fPerecent > 1.0f)
	{
		return;
	}

	m_fPercent = _fPerecent;
	m_pHPImage->SetRatio(m_fPercent);
}

float BossUIState::GetPercent()
{
	return m_fPercent;
}

void BossUIState::BossUIOnOff(bool _Value)
{
	// UI 온오프
	if (true == _Value)
	{
		// ON
		m_pBoss->Enable(true);
		m_pHPBackground->Enable(true);
		m_pHP->Enable(true);
	}
	else
	{
		// OFF
		m_pBoss->Enable(false);
		m_pHPBackground->Enable(false);
		m_pHP->Enable(false);
	}
}
