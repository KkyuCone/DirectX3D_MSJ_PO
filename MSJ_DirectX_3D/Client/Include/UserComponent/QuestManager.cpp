#include "QuestManager.h"
#include "Scene\Scene.h"
#include "Scene\Layer.h"
#include "Device.h"
#include "Time.h"
#include "GameObject.h"
#include "InputManager.h"
#include "QuickSlot.h"
#include "ExcelManager.h"


QuestManager::QuestManager() :
	m_pMiniQuestBGObject(nullptr), m_pMiniQuestBGTr(nullptr), m_pMiniQuestBGImage(nullptr)
	, m_pContentObject(nullptr), m_pContentTr(nullptr), m_pContentText(nullptr)
	, m_pProgressObject(nullptr), m_pProgressTr(nullptr), m_pProgressText(nullptr)
	, m_pQuestPrevBtObject(nullptr), m_pQuestPrevBtTr(nullptr), m_pQuestPrevBt(nullptr)
	, m_pQuestNextBtObject(nullptr), m_pQuestNextBtTr(nullptr), m_pQuestNextBt(nullptr)
	, m_pQuestMinusBtObject(nullptr), m_pQuestMinusBtTr(nullptr), m_pQuestMinusBt(nullptr)
	, m_pQuestPlusBtObject(nullptr), m_pQuestPlusBtTr(nullptr), m_pQuestPlusBt(nullptr)
	, m_pQuestTitleBGObject(nullptr), m_pQuestTitleBGTr(nullptr), m_pQuestTitleBG(nullptr)
	, m_pQuestClearObject(nullptr), m_pQuestClearTr(nullptr), m_pQuestClearImage(nullptr)
	, m_pQuestClearBGObject(nullptr), m_pQuestClearBGTr(nullptr), m_pQuestClearBGImage(nullptr)
	, m_pEffectSound(nullptr)
{
	m_bCurQuestClear = false;
	m_bClickPlusBt = false;
	m_bGoddessClear = false;
	m_bBoyClear = false;
	eQuestNPCKind = NPCKind::NPC_NONE;
}

QuestManager::QuestManager(const QuestManager & _QuestManager)
{
	*this = _QuestManager;
	m_pLayer = _QuestManager.m_pLayer;
	m_iReferenceCount = 1;
}

QuestManager::~QuestManager()
{
	SAFE_RELEASE(m_pMiniQuestBGImage);
	SAFE_RELEASE(m_pMiniQuestBGTr);
	SAFE_RELEASE(m_pMiniQuestBGObject);

	SAFE_RELEASE(m_pContentText);
	SAFE_RELEASE(m_pContentTr);
	SAFE_RELEASE(m_pContentObject);

	SAFE_RELEASE(m_pProgressText);
	SAFE_RELEASE(m_pProgressTr);
	SAFE_RELEASE(m_pProgressObject);

	SAFE_RELEASE(m_pQuestPrevBt);
	SAFE_RELEASE(m_pQuestPrevBtTr);
	SAFE_RELEASE(m_pQuestPrevBtObject);

	SAFE_RELEASE(m_pQuestNextBt);
	SAFE_RELEASE(m_pQuestNextBtTr);
	SAFE_RELEASE(m_pQuestNextBtObject);

	SAFE_RELEASE(m_pQuestMinusBt);
	SAFE_RELEASE(m_pQuestMinusBtTr);
	SAFE_RELEASE(m_pQuestMinusBtObject);

	SAFE_RELEASE(m_pQuestPlusBt);
	SAFE_RELEASE(m_pQuestPlusBtTr);
	SAFE_RELEASE(m_pQuestPlusBtObject);

	SAFE_RELEASE(m_pQuestTitleBG);
	SAFE_RELEASE(m_pQuestTitleBGTr);
	SAFE_RELEASE(m_pQuestTitleBGObject);

	SAFE_RELEASE(m_pQuestClearImage);
	SAFE_RELEASE(m_pQuestClearTr);
	SAFE_RELEASE(m_pQuestClearObject);

	SAFE_RELEASE(m_pQuestClearBGImage);
	SAFE_RELEASE(m_pQuestClearBGTr);
	SAFE_RELEASE(m_pQuestClearBGObject);

	SAFE_RELEASE(m_pEffectSound);
}

void QuestManager::Start()
{
}

bool QuestManager::Init()
{
	ComponentInit();
	return true;
}

int QuestManager::Input(float _fTime)
{
	return 0;
}

int QuestManager::Update(float _fTime)
{
	return 0;
}

int QuestManager::LateUpdate(float _fTime)
{
	if (m_bClickPlusBt)
	{
		m_pQuestPlusBtObject->Enable(false);
		m_pQuestMinusBtObject->Enable(true);
	}
	else
	{
		m_pQuestPlusBtObject->Enable(true);
		m_pQuestMinusBtObject->Enable(false);
	}
	return 0;
}

int QuestManager::Collision(float _fTime)
{
	return 0;
}

int QuestManager::PrevRender(float _fTime)
{
	return 0;
}

int QuestManager::Render(float _fTime)
{
	return 0;
}

QuestManager * QuestManager::Clone() const
{
	return new QuestManager(*this);
}

void QuestManager::ComponentInit()
{
	// 배경
	m_pMiniQuestBGObject = GameObject::CreateObject("MiniQuestBG", m_pLayer);
	m_pMiniQuestBGImage = m_pMiniQuestBGObject->AddComponent<UIBar>("MiniQuestBG");
	m_pMiniQuestBGImage->SetTexture("MiniQuestBG", TEXT("papper_container.png"), PATH_UI_QUEST);
	m_pMiniQuestBGImage->SetShader(SHADER_UI_ONLY_IMAGE);

	Transform* pMiniQuestBGTr = m_pMiniQuestBGObject->GetTransform();
	pMiniQuestBGTr->SetWorldPosition(_RESOLUTION.iWidth * 0.5f + 630.0f, _RESOLUTION.iHeight * 0.5f + 300.0f, 1.5f);
	pMiniQuestBGTr->SetWorldScale(333.0f, 232.0f, 1.0f);
	m_pMiniQuestBGTr = pMiniQuestBGTr;
	SAFE_RELEASE(pMiniQuestBGTr);

	// 타이틀 이미지
	m_pQuestTitleBGObject = GameObject::CreateObject("MiniQuestTitle", m_pLayer);
	m_pQuestTitleBG = m_pQuestTitleBGObject->AddComponent<UIBar>("MiniQuestTitle");
	m_pQuestTitleBG->SetTexture("MiniQuestTitle", TEXT("not_container_2_Qeust.png"), PATH_UI_QUEST);
	m_pQuestTitleBG->SetShader(SHADER_UI_ONLY_IMAGE);

	Transform* pMiniQuestTitleTr = m_pQuestTitleBGObject->GetTransform();
	pMiniQuestTitleTr->SetWorldPosition(_RESOLUTION.iWidth * 0.5f + 625.0f, _RESOLUTION.iHeight * 0.5f + 480.0f, 1.4f);
	pMiniQuestTitleTr->SetWorldScale(345.96f, 54.18f, 1.0f);
	m_pMiniQuestBGTr = pMiniQuestTitleTr;
	SAFE_RELEASE(pMiniQuestTitleTr);

	// 축소
	m_pQuestMinusBtObject = GameObject::CreateObject("MiniQuestMinusBt", m_pLayer);
	m_pQuestMinusBt = m_pQuestMinusBtObject->AddComponent<UIButton>("MiniQuestMinusBt");
	m_pQuestMinusBt->SetStateTexture(BS_NORMAL, "UI_MiniNextBt_Normal", TEXT("MinusBt_Normal.png"), PATH_UI_BUTTON_BUTTON);
	m_pQuestMinusBt->SetStateTexture(BS_CLICK, "UI_MiniNextBt_Click", TEXT("MinusBt_Click.png"), PATH_UI_BUTTON_BUTTON);
	m_pQuestMinusBt->SetStateTexture(BS_MOUSEON, "UI_MiniNextBt_MouseON", TEXT("MinusBt_MouseOn.png"), PATH_UI_BUTTON_BUTTON);
	m_pQuestMinusBt->SetCallBackFunc(BS_CLICK, this, &QuestManager::DownsizeBtFunc);
	m_pQuestMinusBt->SetPushNoneOnlyMouseON(true);

	Transform* pMiniMinusBtTr = m_pQuestMinusBtObject->GetTransform();
	pMiniMinusBtTr->SetWorldPosition(_RESOLUTION.iWidth * 0.5f + 920.0f, _RESOLUTION.iHeight * 0.5f + 490.0f, 1.3f);
	pMiniMinusBtTr->SetWorldScale(35.0f, 35.0f, 1.0f);
	m_pQuestMinusBtTr = pMiniMinusBtTr;
	SAFE_RELEASE(pMiniMinusBtTr);

	// 확대
	m_pQuestPlusBtObject = GameObject::CreateObject("MiniQuestMinusBt", m_pLayer);
	m_pQuestPlusBt = m_pQuestPlusBtObject->AddComponent<UIButton>("MiniQuestMinusBt");
	m_pQuestPlusBt->SetStateTexture(BS_NORMAL, "UI_MiniNextBt_Normal", TEXT("MinusBt_Normal.png"), PATH_UI_BUTTON_BUTTON);
	m_pQuestPlusBt->SetStateTexture(BS_CLICK, "UI_MiniNextBt_Click", TEXT("MinusBt_Click.png"), PATH_UI_BUTTON_BUTTON);
	m_pQuestPlusBt->SetStateTexture(BS_MOUSEON, "UI_MiniNextBt_MouseON", TEXT("MinusBt_MouseOn.png"), PATH_UI_BUTTON_BUTTON);
	m_pQuestPlusBt->SetCallBackFunc(BS_CLICK, this, &QuestManager::ExpansionBtFunc);
	m_pQuestPlusBt->SetPushNoneOnlyMouseON(true);

	Transform* pMiniPlusBtTr = m_pQuestPlusBtObject->GetTransform();
	pMiniPlusBtTr->SetWorldPosition(_RESOLUTION.iWidth * 0.5f + 920.0f, _RESOLUTION.iHeight * 0.5f + 490.0f, 1.3f);
	pMiniPlusBtTr->SetWorldScale(35.0f, 35.0f, 1.0f);
	m_pQuestPlusBtTr = pMiniPlusBtTr;
	SAFE_RELEASE(pMiniPlusBtTr);
	m_pQuestPlusBtObject->Enable(false);

	// Quest 텍스트
	m_pContentObject = GameObject::CreateObject("MiniQuestTitleText", m_pLayer);
	Transform* pContentTr = m_pContentObject->GetTransform();
	pContentTr->SetWorldPosition(1640.0f, 820.0f, 0.0f);
	m_pContentTr = pContentTr;
	SAFE_RELEASE(pContentTr);

	m_pContentText = m_pContentObject->AddComponent<UIText>("Text");
	m_pContentText->SetText(TEXT(""));
	m_pContentText->Shadow(false);
	m_pContentText->SetFont("NanoomGothicBold");
	m_pContentText->SetColor(1.f, 1.f, 1.f, 1.f);
	m_pContentText->SetOpacity(1.0f);
	m_pContentText->AlphaBlend(true);
	m_pContentText->SetRenderArea(0.f, 0.f, 270.f, 200.f);

	// 퀘스트 완료 / 미완료 표시
	m_pQuestClearBGObject = GameObject::CreateObject("QuestClearBG", m_pLayer);
	m_pQuestClearBGImage = m_pQuestClearBGObject->AddComponent<UIBar>("QuestClearBG");
	m_pQuestClearBGImage->SetTexture("QuestClearBG", TEXT("quest_list_row_checkbox.png"), PATH_UI_QUEST);
	m_pQuestClearBGImage->SetShader(SHADER_UI_ONLY_IMAGE);

	Transform* pQuestClearBGTr = m_pQuestClearBGObject->GetTransform();
	pQuestClearBGTr->SetWorldPosition(_RESOLUTION.iWidth * 0.5f + 640.0f, _RESOLUTION.iHeight * 0.5f + 438.0f, 1.2f);
	pQuestClearBGTr->SetWorldScale(45.0f, 45.0f, 1.0f);
	m_pQuestClearBGTr = pQuestClearBGTr;
	SAFE_RELEASE(pQuestClearBGTr);


	m_pQuestClearObject = GameObject::CreateObject("QuestClearCheck", m_pLayer);
	m_pQuestClearImage = m_pQuestClearObject->AddComponent<UIBar>("QuestClearCheck");
	m_pQuestClearImage->SetTexture("QuestClearCheck", TEXT("quest_list_row_checkbox_selected.png"), PATH_UI_QUEST);
	m_pQuestClearImage->SetShader(SHADER_UI_ONLY_IMAGE);

	Transform* pQuestClearTr = m_pQuestClearObject->GetTransform();
	pQuestClearTr->SetWorldPosition(_RESOLUTION.iWidth * 0.5f + 640.0f, _RESOLUTION.iHeight * 0.5f + 443.0f, 1.1f);
	pQuestClearTr->SetWorldScale(45.0f, 35.0f, 1.0f);
	m_pQuestClearTr = pQuestClearTr;
	SAFE_RELEASE(pQuestClearTr);
	m_pQuestClearObject->Enable(false);

	AudioSound* pEffectSound = m_pGameObject->AddComponent<AudioSound>("QuestBtClick");
	pEffectSound->SetSound("QuestBtClick", m_pScene, false, "Button-4.mp3", PATH_SOUND_UI);
	m_pEffectSound = pEffectSound;
	SAFE_RELEASE(pEffectSound);
}

NPCKind QuestManager::GetQuestNPC() const
{
	return eQuestNPCKind;
}

bool QuestManager::GetQuestClear(NPCKind _NPC) const
{
	switch (_NPC)
	{
	case NPC_GODDESS:
		return m_bGoddessClear;
	case NPC_BOY:
		return m_bBoyClear;
	}
	return false;
}

void QuestManager::SetQuestClear(bool _Value, NPCKind _NPC)
{
	if (eQuestNPCKind == _NPC)
	{
		m_bCurQuestClear = false;
		m_pQuestClearObject->Enable(m_bCurQuestClear);
		m_pContentText->SetText(TEXT(""));
		return;
	}
	m_bCurQuestClear = _Value;
	m_pQuestClearObject->Enable(m_bCurQuestClear);
}



void QuestManager::ExpansionBtFunc(float _fTime)
{
	m_pEffectSound->Stop();
	m_pEffectSound->Play();
	// 확대 버튼 눌렀을 경우
	m_bClickPlusBt = true;

	m_pMiniQuestBGObject->Enable(true);
	m_pContentObject->Enable(true);
	//m_pProgressObject->Enable(true);
	//m_pQuestPrevBtObject->Enable(true);
	//m_pQuestNextBtObject->Enable(true);
	m_pQuestClearBGObject->Enable(true);
	m_pQuestClearObject->Enable(false);

	if (true == m_bCurQuestClear)
	{
		m_pQuestClearObject->Enable(true);
	}
}

void QuestManager::DownsizeBtFunc(float _fTime)
{
	m_pEffectSound->Stop();
	m_pEffectSound->Play();
	// 축소 버튼 눌렀을 경우
	m_bClickPlusBt = false;
	
	m_pMiniQuestBGObject->Enable(false);
	m_pContentObject->Enable(false);
	//m_pProgressObject->Enable(false);
	//m_pQuestPrevBtObject->Enable(false);
	//m_pQuestNextBtObject->Enable(false);
	m_pQuestClearObject->Enable(false);
	m_pQuestClearBGObject->Enable(false);
}

void QuestManager::ChangeQuestText(const char * _strName, int _Row, int _Col, int _Sheet, NPCKind _QuestNPC)
{
	std::wstring strChangeContent = GET_SINGLETON(ExcelManager)->ReadWString(_strName, _Row, _Col, _Sheet);
	m_pContentText->SetText(strChangeContent.c_str());
	eQuestNPCKind = _QuestNPC;
	m_bCurQuestClear = false;
}

void QuestManager::ClearQuest(NPCKind _NPC)
{
	if (eQuestNPCKind == _NPC)
	{
		m_pQuestClearObject->Enable(true);
		m_bCurQuestClear = true;
		if (_NPC == NPCKind::NPC_GODDESS)
		{
			m_bGoddessClear = true;
		}
		else
		{
			m_bBoyClear = true;
		}
	}
	else
	{
		m_pQuestClearObject->Enable(false);
		m_bCurQuestClear = false;
	}
}
