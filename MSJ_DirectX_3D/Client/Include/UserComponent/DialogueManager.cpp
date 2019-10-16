#include "DialogueManager.h"
#include "Scene\Scene.h"
#include "Scene\Layer.h"
#include "Device.h"
#include "Time.h"
#include "GameObject.h"
#include "InputManager.h"
#include "QuickSlot.h"
#include "QuestManager.h"
#include "ExcelManager.h"


DialogueManager::DialogueManager() :
	m_pNameObject(nullptr), m_pNameText(nullptr), m_pNameTr(nullptr)
	, m_pDialogueObject(nullptr), m_pDilaogueText(nullptr), m_pDialogueTr(nullptr)
	, m_pDialogueBGObject(nullptr), m_pDialogueBGImage(nullptr), m_pDialogueBGTr(nullptr)
	, m_pPrevBtObject(nullptr), m_pPrevBt(nullptr), m_pPrevBtTr(nullptr)
	, m_pNextBtObject(nullptr), m_pNextBt(nullptr), m_pNextBtTr(nullptr)
	, m_pYesBtObject(nullptr), m_pYestBt(nullptr), m_pYesBtTr(nullptr)
	, m_pNoBtObject(nullptr), m_pNoBt(nullptr), m_pNoBtTr(nullptr)
	, pQuestManagerScript(nullptr), m_bCurQuestClear(false)
	, m_pEffectSound(nullptr)
{
	m_bEmceeQuest = false;
	m_bCurQuestDialogue = false;
	m_bDialogueEnable = false;
	m_bQuestAnswer = false;
	m_bQuestQnswerOn = false;
	m_iSheetNumber = -1;
	m_iRowNumber = -1;
	m_iColNumber = -1;
	m_eNPCKind = NPCKind::NPC_NONE;
	m_eQuestNPC = NPCKind::NPC_NONE;
	m_eDK = NPCDialogueKind::NDK_NONE;
}

DialogueManager::DialogueManager(const DialogueManager & _DialogueManager)
{
	*this = _DialogueManager;
	m_pLayer = _DialogueManager.m_pLayer;
	m_iReferenceCount = 1;
}


DialogueManager::~DialogueManager()
{
	SAFE_RELEASE(m_pNameText);
	SAFE_RELEASE(m_pNameTr);
	SAFE_RELEASE(m_pNameObject);

	SAFE_RELEASE(m_pDilaogueText);
	SAFE_RELEASE(m_pDialogueTr);
	SAFE_RELEASE(m_pDialogueObject);

	SAFE_RELEASE(m_pDialogueBGImage);
	SAFE_RELEASE(m_pDialogueBGTr);
	SAFE_RELEASE(m_pDialogueBGObject);

	SAFE_RELEASE(m_pPrevBt);
	SAFE_RELEASE(m_pPrevBtTr)
	SAFE_RELEASE(m_pPrevBtObject);

	SAFE_RELEASE(m_pNextBt);
	SAFE_RELEASE(m_pNextBtTr);
	SAFE_RELEASE(m_pNextBtObject);

	SAFE_RELEASE(m_pYestBt);
	SAFE_RELEASE(m_pYesBtTr);
	SAFE_RELEASE(m_pYesBtObject);

	SAFE_RELEASE(m_pNoBt);
	SAFE_RELEASE(m_pNoBtTr);
	SAFE_RELEASE(m_pNoBtObject);

	SAFE_RELEASE(m_pEffectSound);
}

void DialogueManager::Start()
{
}

bool DialogueManager::Init()
{
	ComponentInit();

	//GET_SINGLETON(ExcelManager)->BookCreate("Ikmeyng");
	//GET_SINGLETON(ExcelManager)->LoadExcelFile("Ikmeyng", "Ikmeyng.xls", PATH_EXCEL);
	//std::string ddd = GET_SINGLETON(ExcelManager)->ReadString("Ikmeyng", 2, 1, 0);
	return true;
}

int DialogueManager::Input(float _fTime)
{
	return 0;
}

int DialogueManager::Update(float _fTime)
{
	return 0;
}

int DialogueManager::LateUpdate(float _fTime)
{
	if (m_bQuestQnswerOn == true)
	{
		m_bQuestQnswerOn = false;
		m_pYesBtObject->Enable(true);
		m_pNoBtObject->Enable(true);
	}
	return 0;
}

int DialogueManager::Collision(float _fTime)
{
	return 0;
}

int DialogueManager::PrevRender(float _fTime)
{
	return 0;
}

int DialogueManager::Render(float _fTime)
{
	return 0;
}

DialogueManager * DialogueManager::Clone() const
{
	return nullptr;
}

void DialogueManager::SetQuickSlotScript(QuickSlot * _pScript)
{
	pQuickSlotScript = _pScript;
}

void DialogueManager::PushPalyerObject(GameObject * _pObject)
{
	pPalyerUIStateList.push_back(_pObject);
}

void DialogueManager::SetQuestManagerScript(QuestManager * _pScript)
{
	pQuestManagerScript = _pScript;
}

void DialogueManager::ComponentInit()
{
	// 대화상자 배경 이미지
	m_pDialogueBGObject = GameObject::CreateObject("DialogueBG", m_pLayer);
	m_pDialogueBGImage = m_pDialogueBGObject->AddComponent<UIBar>("DialogueBG");
	m_pDialogueBGImage->SetTexture("DialogueBG", TEXT("select_dropdown_container_None.png"), PATH_UI_QUEST);
	m_pDialogueBGImage->SetShader(SHADER_UI_ONLY_IMAGE);

	Transform* pDialogueBGTr = m_pDialogueBGObject->GetTransform();
	pDialogueBGTr->SetWorldPosition(_RESOLUTION.iWidth * 0.5f - 695.0f, _RESOLUTION.iHeight * 0.5f - 550.0f, 0.8f);
	pDialogueBGTr->SetWorldScale(1395.0f, 404.0f, 1.0f);
	m_pDialogueBGTr = pDialogueBGTr;
	SAFE_RELEASE(pDialogueBGTr);

	// 캐릭터 명(NPC)
	m_pNameObject = GameObject::CreateObject("DialogueNameTag", m_pLayer);
	Transform* pNameTextTr = m_pNameObject->GetTransform();
	pNameTextTr->SetWorldPosition(340.0f, 280.0f, 0.0f);
	SAFE_RELEASE(pNameTextTr);

	m_pNameText = m_pNameObject->AddComponent<UIText>("Text");
	m_pNameText->SetText(TEXT("ㅇㅅㅇ"));
	m_pNameText->Shadow(false);
	m_pNameText->SetFont("HumanCircleHeadLineM");
	m_pNameText->SetColor(1.f, 1.f, 1.f, 1.f);
	m_pNameText->SetOpacity(1.0f);
	m_pNameText->AlphaBlend(true);
	m_pNameText->SetRenderArea(0.f, 0.f, 300.f, 50.f);

	// 내용
	m_pDialogueObject = GameObject::CreateObject("", m_pLayer);
	Transform* pDialogueTextTr = m_pDialogueObject->GetTransform();
	pDialogueTextTr->SetWorldPosition(340.0f, 0.0f, 0.0f);
	SAFE_RELEASE(pDialogueTextTr);

	m_pDilaogueText = m_pDialogueObject->AddComponent<UIText>("Text");
	m_pDilaogueText->SetText(TEXT("익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이익명이"));
	m_pDilaogueText->Shadow(false);
	m_pDilaogueText->SetFont("NanoomGothicBold");
	m_pDilaogueText->SetColor(1.f, 1.f, 1.f, 1.f);
	m_pDilaogueText->SetOpacity(1.0f);
	m_pDilaogueText->AlphaBlend(true);
	m_pDilaogueText->SetRenderArea(0.f, 0.f, 1250.f, 280.f);

	// 버튼위치
	// 이전
	m_pPrevBtObject = GameObject::CreateObject("PrevBt", m_pLayer);
	m_pPrevBt = m_pPrevBtObject->AddComponent<UIButton>("PrevBt");
	m_pPrevBt->SetStateTexture(BS_NORMAL, "UI_PrevBt_Normal", TEXT("Prev_button_green.png"), PATH_UI_BUTTON_BUTTON);
	m_pPrevBt->SetStateTexture(BS_CLICK, "UI_PrevBt_Click", TEXT("Prev_button_green-PUSH.png"), PATH_UI_BUTTON_BUTTON);
	m_pPrevBt->SetStateTexture(BS_MOUSEON, "UI_PrevBt_MouseON", TEXT("Prev_button_green-HOVER.png"), PATH_UI_BUTTON_BUTTON);
	//m_pPrevBt->SetCallBackFunc(BS_CLICK, this, &DialogueManager::ClickShopItem_AW);
	m_pPrevBt->SetPushNoneOnlyMouseON(true);

	Transform* pPrevBtTr = m_pPrevBtObject->GetTransform();
	pPrevBtTr->SetWorldPosition(_RESOLUTION.iWidth * 0.5f - 635.0f, _RESOLUTION.iHeight * 0.5f - 520.0f, 0.7f);
	pPrevBtTr->SetWorldScale(173.0f, 65.0f, 1.0f);
	m_pPrevBtTr = pPrevBtTr;
	SAFE_RELEASE(pPrevBtTr);

	// 예
	m_pYesBtObject = GameObject::CreateObject("YesBt", m_pLayer);
	m_pYestBt = m_pYesBtObject->AddComponent<UIButton>("YesBt");
	m_pYestBt->SetStateTexture(BS_NORMAL, "UI_YesBt_Normal", TEXT("Quest_Yes_button_green.png"), PATH_UI_BUTTON_BUTTON);
	m_pYestBt->SetStateTexture(BS_CLICK, "UI_YesBt_Click", TEXT("Quest_Yes_button_green-PUSH.png"), PATH_UI_BUTTON_BUTTON);
	m_pYestBt->SetStateTexture(BS_MOUSEON, "UI_YesBt_MouseON", TEXT("Quest_Yes_button_green-HOVER.png"), PATH_UI_BUTTON_BUTTON);
	m_pYestBt->SetCallBackFunc(BS_CLICK, this, &DialogueManager::YesBtFunc);
	m_pYestBt->SetPushNoneOnlyMouseON(true);

	Transform* pYesBtTr = m_pYesBtObject->GetTransform();
	pYesBtTr->SetWorldPosition(_RESOLUTION.iWidth * 0.5f - 635.0f, _RESOLUTION.iHeight * 0.5f - 520.0f, 0.7f);
	pYesBtTr->SetWorldScale(173.0f, 65.0f, 1.0f);
	m_pYesBtTr = pYesBtTr;
	SAFE_RELEASE(pYesBtTr);

	// 다음
	m_pNextBtObject = GameObject::CreateObject("NextBt", m_pLayer);
	m_pNextBt = m_pNextBtObject->AddComponent<UIButton>("NextBt");
	m_pNextBt->SetStateTexture(BS_NORMAL, "UI_NextBt_Normal", TEXT("Next_button_green.png"), PATH_UI_BUTTON_BUTTON);
	m_pNextBt->SetStateTexture(BS_CLICK, "UI_NextBt_Click", TEXT("Next_button_green-PUSH.png"), PATH_UI_BUTTON_BUTTON);
	m_pNextBt->SetStateTexture(BS_MOUSEON, "UI_NextBt_MouseON", TEXT("Next_button_green-HOVER.png"), PATH_UI_BUTTON_BUTTON);
	m_pNextBt->SetCallBackFunc(BS_CLICK, this, &DialogueManager::NextBtFunc);
	m_pNextBt->SetPushNoneOnlyMouseON(true);

	Transform* pNextBtTr = m_pNextBtObject->GetTransform();
	pNextBtTr->SetWorldPosition(_RESOLUTION.iWidth * 0.5f + 470.0f, _RESOLUTION.iHeight * 0.5f - 520.0f, 0.7f);
	pNextBtTr->SetWorldScale(173.0f, 65.0f, 1.0f);
	m_pNextBtTr = pNextBtTr;
	SAFE_RELEASE(pNextBtTr);

	// 거절
	m_pNoBtObject = GameObject::CreateObject("NoBt", m_pLayer);
	m_pNoBt = m_pNoBtObject->AddComponent<UIButton>("NoBt");
	m_pNoBt->SetStateTexture(BS_NORMAL, "UI_NoBt_Normal", TEXT("Quest_No_button_green.png"), PATH_UI_BUTTON_BUTTON);
	m_pNoBt->SetStateTexture(BS_CLICK, "UI_NoBt_Click", TEXT("Quest_No_button_green-PUSH.png"), PATH_UI_BUTTON_BUTTON);
	m_pNoBt->SetStateTexture(BS_MOUSEON, "UI_NoBt_MouseON", TEXT("Quest_No_button_green-HOVER.png"), PATH_UI_BUTTON_BUTTON);
	m_pNoBt->SetCallBackFunc(BS_CLICK, this, &DialogueManager::NoBtFunc);
	m_pNoBt->SetPushNoneOnlyMouseON(true);

	Transform* pNoBtTr = m_pNoBtObject->GetTransform();
	pNoBtTr->SetWorldPosition(_RESOLUTION.iWidth * 0.5f + 470.0f, _RESOLUTION.iHeight * 0.5f - 520.0f, 0.7f);
	pNoBtTr->SetWorldScale(173.0f, 65.0f, 1.0f);
	m_pNoBtTr = pNoBtTr;
	SAFE_RELEASE(pNoBtTr);

	AllDialogueOff();

	AudioSound* pEffectSound = m_pGameObject->AddComponent<AudioSound>("DialogueBtClick");
	pEffectSound->SetSound("DialogueBtClick", m_pScene, false, "Button-4.mp3", PATH_SOUND_UI);
	m_pEffectSound = pEffectSound;
	SAFE_RELEASE(pEffectSound);
}

void DialogueManager::PlayerStateOn()
{
	if (nullptr != pQuickSlotScript)
	{
		pQuickSlotScript->QuickSlotOn();
	}

	std::list<GameObject*>::iterator Iter = pPalyerUIStateList.begin();
	std::list<GameObject*>::iterator IterEnd = pPalyerUIStateList.end();

	for (; Iter != IterEnd; ++Iter)
	{
		(*Iter)->Enable(true);
	}
}

void DialogueManager::PlayerStateOff()
{
	if (nullptr != pQuickSlotScript)
	{
		pQuickSlotScript->QuickSlotOff();
	}

	std::list<GameObject*>::iterator Iter = pPalyerUIStateList.begin();
	std::list<GameObject*>::iterator IterEnd = pPalyerUIStateList.end();

	for (; Iter != IterEnd; ++Iter)
	{
		(*Iter)->Enable(false);
	}
}

void DialogueManager::DialogueOn(NPCKind _NPC, NPCDialogueKind _NDK)
{
	if (NPCDialogueKind::NDK_CONVERSATION == _NDK)
	{
		ConversationDialogueOn(_NPC);
	}
	else if(NPCDialogueKind::NDK_QUEST == _NDK)
	{
		QuestDialougeOn(_NPC);
	}

	PlayerStateOff();

}

void DialogueManager::DialogueOn(int _Row, int _Col, int _iSheet)
{
	AllDialogueOn();
	m_pYesBtObject->Enable(false);
	m_pNoBtObject->Enable(false);
	m_pNextBtObject->Enable(true);
	m_pPrevBtObject->Enable(true);

	// 텍스트를 맞는걸로 해주기
	std::wstring strName = GET_SINGLETON(ExcelManager)->ReadWString("Dialogue", _Row, 1, _iSheet);
	m_pNameText->SetText(strName.c_str());

	// 첫문장까지 여기서 띄우기
	std::wstring strContent = GET_SINGLETON(ExcelManager)->ReadWString("Dialogue", _Row, _Col, _iSheet);
	m_pDilaogueText->SetText(strContent.c_str());
}

void DialogueManager::DialogueOff()
{
	AllDialogueOff();
	PlayerStateOn();
}

void DialogueManager::AllDialogueOff()
{
	m_pDialogueBGObject->Enable(false);
	m_pNameObject->Enable(false);
	m_pDialogueObject->Enable(false);
	m_pPrevBtObject->Enable(false);
	m_pYesBtObject->Enable(false);
	m_pNextBtObject->Enable(false);
	m_pNoBtObject->Enable(false);
}

void DialogueManager::AllDialogueOn()
{
	m_pDialogueBGObject->Enable(true);
	m_pNameObject->Enable(true);
	m_pDialogueObject->Enable(true);
	m_pPrevBtObject->Enable(true);
	m_pYesBtObject->Enable(true);
	m_pNextBtObject->Enable(true);
	m_pNoBtObject->Enable(true);
}

void DialogueManager::ConversationDialogueOn(NPCKind _NPC)
{
	m_bCurQuestDialogue = false;
	m_eDK = NPCDialogueKind::NDK_CONVERSATION;
	m_iSheetNumber = 0;
	m_iColNumber = 3;
	switch (_NPC)
	{
	case NPC_SHOP:
		m_iRowNumber = 3;
		break;
	case NPC_GODDESS:
		m_iRowNumber = 9;
		break;
	case NPC_BOY:
		m_iRowNumber = 12;
		break;
	case NPC_PASSERBY:
		m_iRowNumber = 6;
		break;
	default:
		return;
	}
	m_eNPCKind = _NPC;
	DialogueOn(m_iRowNumber, m_iColNumber, m_iSheetNumber);
}

void DialogueManager::EmceeQuestDialogueOn()
{
	// 이미 퀘스트가 진행중인 문구
	if (m_eQuestNPC != m_eNPCKind)
	{
		m_iRowNumber = 30;
	}
	else if (m_eQuestNPC == m_eNPCKind)
	{
		if (m_eQuestNPC == NPCKind::NPC_GODDESS)
		{
			m_iRowNumber = 15;
		}
		else
		{
			m_iRowNumber = 25;
		}
	}
	else
	{
		DialogueOff();
		return;
	}
	m_iSheetNumber = 1;
	m_iColNumber = 3;
	DialogueOn(m_iRowNumber, m_iColNumber, m_iSheetNumber);
	m_pPrevBtObject->Enable(false);
	m_pNextBtObject->Enable(true);
	m_pYesBtObject->Enable(false);
	m_pNoBtObject->Enable(false);
}

void DialogueManager::QuestClearDialogueOn()
{
	if (m_eNPCKind == NPCKind::NPC_GODDESS)
	{
		m_iRowNumber = 14;
	}
	else
	{
		m_iRowNumber = 24;
	}
	m_iSheetNumber = 1;
	m_iColNumber = 3;
	std::wstring strChangeContent = GET_SINGLETON(ExcelManager)->ReadWString("Dialogue", m_iRowNumber, m_iColNumber, m_iSheetNumber);
	m_pDilaogueText->SetText(strChangeContent.c_str());

	int iNextRowNum = m_iRowNumber + 1;
	std::string strNum = GET_SINGLETON(ExcelManager)->ReadString("Dialogue", iNextRowNum, 2, m_iSheetNumber);

	AllDialogueOn();
	m_pPrevBtObject->Enable(false);
	m_pNextBtObject->Enable(true);
	m_pYesBtObject->Enable(false);
	m_pNoBtObject->Enable(false);
}

void DialogueManager::ExcelSheetInit()
{
	m_iSheetNumber = -1;
	m_iRowNumber = -1;
	m_iColNumber = -1;
}

void DialogueManager::QuestDialougeOn(NPCKind _NPC)
{
	m_eNPCKind = _NPC;
	// 퀘스트를 완료했는지 먼저 체크
	if (pQuestManagerScript->GetQuestNPC() == m_eNPCKind
		&& pQuestManagerScript->GetQuestClear(_NPC) == true)
	{
		pQuestManagerScript->SetQuestClear(false, m_eNPCKind);
		QuestClearDialogueOn();
		m_bCurQuestClear = true;
		return;
	}

	m_bCurQuestDialogue = true;
	if (m_bEmceeQuest == true)
	{
		EmceeQuestDialogueOn();
		return;
	}

	m_eDK = NPCDialogueKind::NDK_QUEST;
	m_iSheetNumber = 1;
	m_iColNumber = 3;
	switch (_NPC)
	{
	case NPC_SHOP:
		m_iSheetNumber = 0;
		m_iColNumber = -1;
		return;
	case NPC_GODDESS:
		m_iRowNumber = 7;
		break;
	case NPC_BOY:
		m_iRowNumber = 18;
		break;
	default:
		break;
	}
	m_eNPCKind = _NPC;
	DialogueOn(m_iRowNumber, m_iColNumber, m_iSheetNumber);
}


void DialogueManager::PrevBtFunc(float _fTime)
{
	m_pEffectSound->Stop();
	m_pEffectSound->Play();
	// 이전 버튼인 경우엔 
	// 대화의 경우는 그냥 return
	if (m_eDK == NPCDialogueKind::NDK_CONVERSATION)
	{
		return;
	}

	int iPrevNum = m_iRowNumber - 1;
	std::string strNum = GET_SINGLETON(ExcelManager)->ReadString("Dialogue", iPrevNum, 2, m_iSheetNumber);

	if (strNum.find("FE") != std::string::npos 
		|| strNum.find("Num") != std::string::npos)
	{
		return;
	}

	--m_iRowNumber;

	std::wstring strChangeContent = GET_SINGLETON(ExcelManager)->ReadWString("Dialogue", m_iRowNumber, m_iColNumber, m_iSheetNumber);
	m_pDilaogueText->SetText(strChangeContent.c_str());
}

void DialogueManager::NextBtFunc(float _fTime)
{
	m_pEffectSound->Stop();
	m_pEffectSound->Play();
	// 맨처음 클리어 문구인지판단
	if ((m_eDK == NPCDialogueKind::NDK_CONVERSATION || m_bQuestAnswer == true)
		|| (m_bEmceeQuest == true && m_bCurQuestDialogue == true) 
		|| m_bCurQuestClear == true)
	{
		ExcelSheetInit();
		DialogueOff();
		m_eNPCKind = NPCKind::NPC_NONE;
		m_bQuestAnswer = false;
		m_bCurQuestClear = false;
		return;
	}

	// 다음 버튼 클릭시
	// End가 나올때까지 Text를 넘기도록 한다.
	++m_iRowNumber;

	std::wstring strChangeContent = GET_SINGLETON(ExcelManager)->ReadWString("Dialogue", m_iRowNumber, m_iColNumber, m_iSheetNumber);
	m_pDilaogueText->SetText(strChangeContent.c_str());

	int iNextRowNum = m_iRowNumber+1;
	std::string strNum = GET_SINGLETON(ExcelManager)->ReadString("Dialogue", iNextRowNum, 2, m_iSheetNumber);

	if (strNum.find("#aEnd") != std::string::npos)
	{
		// 퀘스트 대화의 끝무렵이라면
		// 수락, 거절 버튼을 활성화하고 , 이전과 다음버튼은 비활성화 시킨다.
		m_pPrevBtObject->Enable(false);
		m_pNextBtObject->Enable(false);
		m_bQuestQnswerOn = true;
	}
}

void DialogueManager::YesBtFunc(float _fTime)
{
	m_pEffectSound->Stop();
	m_pEffectSound->Play();
	// 수락버튼
	switch (m_eNPCKind)
	{
	case NPC_GODDESS:
		m_iRowNumber = 12;
		pQuestManagerScript->ChangeQuestText("Dialogue", 40, 3, 1, NPC_GODDESS);
		break;
	case NPC_BOY:
		m_iRowNumber = 21;
		pQuestManagerScript->ChangeQuestText("Dialogue", 41, 3, 1, NPC_BOY);
		break;
	default:
		return;
	}
	m_bEmceeQuest = true;
	m_iColNumber = 3;
	std::wstring strChangeContent = GET_SINGLETON(ExcelManager)->ReadWString("Dialogue", m_iRowNumber, m_iColNumber, m_iSheetNumber);
	m_pDilaogueText->SetText(strChangeContent.c_str());

	m_pPrevBtObject->Enable(false);
	m_pNextBtObject->Enable(true);
	m_pYesBtObject->Enable(false);
	m_pNoBtObject->Enable(false);
	m_bQuestAnswer = true;
	m_eQuestNPC = m_eNPCKind;
}

void DialogueManager::NoBtFunc(float _fTime)
{
	m_pEffectSound->Stop();
	m_pEffectSound->Play();
	// 거절 버튼
	switch (m_eNPCKind)
	{
	case NPC_GODDESS:
		m_iRowNumber = 13;
		break;
	case NPC_BOY:
		m_iRowNumber = 22;
		break;
	default:
		return;
	}
	m_iColNumber = 3;
	std::wstring strChangeContent = GET_SINGLETON(ExcelManager)->ReadWString("Dialogue", m_iRowNumber, m_iColNumber, m_iSheetNumber);
	m_pDilaogueText->SetText(strChangeContent.c_str());

	m_pPrevBtObject->Enable(false);
	m_pNextBtObject->Enable(true);
	m_pYesBtObject->Enable(false);
	m_pNoBtObject->Enable(false);
	m_bQuestAnswer = true;
	m_eNPCKind = NPCKind::NPC_NONE;
}
