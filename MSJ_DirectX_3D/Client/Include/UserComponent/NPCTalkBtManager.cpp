#include "NPCTalkBtManager.h"
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
#include "ShopManager.h"
#include "DialogueManager.h"
#include "Inventory.h"


NPCTalkBtManager::NPCTalkBtManager() :
	pDialogueManager(nullptr), pShopManager(nullptr), pInventory(nullptr)
	, m_pEffectSound(nullptr)
{
	m_eNPCKind = NPCKind::NPC_NONE;
	m_bNPCButtonEnable = false;
}

NPCTalkBtManager::NPCTalkBtManager(const NPCTalkBtManager & _NPCTalkBtManater)
{
	m_pScene = _NPCTalkBtManater.m_pScene;
	m_pLayer = _NPCTalkBtManater.m_pLayer;
	m_iReferenceCount = 1;
}


NPCTalkBtManager::~NPCTalkBtManager()
{
	for (int i = 0; i < m_vecNPCBts.size(); ++i)
	{
		SAFE_RELEASE(m_vecNPCBts[i]->pObject);
		SAFE_RELEASE(m_vecNPCBts[i]->pTr);
		SAFE_RELEASE(m_vecNPCBts[i]->pButton);
		delete m_vecNPCBts[i];
	}
	m_vecNPCBts.clear();

	SAFE_RELEASE(m_pEffectSound);
}

void NPCTalkBtManager::Start()
{
}

bool NPCTalkBtManager::Init()
{
	ComponentInit();
	return true;
}

int NPCTalkBtManager::Input(float _fTime)
{
	return 0;
}

int NPCTalkBtManager::Update(float _fTime)
{
	return 0;
}

int NPCTalkBtManager::LateUpdate(float _fTime)
{
	return 0;
}

int NPCTalkBtManager::Collision(float _fTime)
{
	return 0;
}

int NPCTalkBtManager::PrevRender(float _fTime)
{
	return 0;
}

int NPCTalkBtManager::Render(float _fTime)
{
	return 0;
}

NPCTalkBtManager * NPCTalkBtManager::Clone() const
{
	return new NPCTalkBtManager(*this);
}

void NPCTalkBtManager::ComponentInit()
{
	// 대화하기, 퀘스트 / 상점, 나가기
	m_vecNPCBts.resize(4);

	for (int i = 0 ; i < m_vecNPCBts.size(); ++i)
	{
		NPCBtInfo* pNewNPCBtType = new NPCBtInfo;

		GameObject* pObject = GameObject::CreateObject("NPCButton", m_pLayer);
		UIButton* pBt = pObject->AddComponent<UIButton>("NPCButton");
		pBt->SetStateTexture(BS_NORMAL, "UI_NPCButton_Normal", TEXT("button_gray.png"), PATH_UI_BUTTON_BUTTON);
		pBt->SetStateTexture(BS_CLICK, "UI_NPCButton_Click", TEXT("button_gray-PUSH.png"), PATH_UI_BUTTON_BUTTON);
		pBt->SetStateTexture(BS_MOUSEON, "UI_NPCButton_MouseOn", TEXT("button_gray-HOVER.png"), PATH_UI_BUTTON_BUTTON);
		pBt->SetPushNoneOnlyMouseON(true);
		//pItemSelectBt->SetCallBackFunc(BS_CLICK, this, &ShopManager::ClickShopItem_None);

		Transform* pBtTr = pObject->GetTransform();
		pBtTr->SetWorldPosition(_RESOLUTION.iWidth * 0.5f - 121.1f, _RESOLUTION.iHeight * 0.5f + ( 220.0f -(120.0f * i)), 0.1f);
		//pBtTr->SetWorldScale(346.5f, 130.0f, 1.0f);
		pBtTr->SetWorldScale(242.2f, 91.0f, 1.0f);

		pNewNPCBtType->pObject = pObject;
		pNewNPCBtType->pTr = pBtTr;
		pNewNPCBtType->pButton = pBt;
		SAFE_RELEASE(pBtTr);

		m_vecNPCBts[i] = pNewNPCBtType;
	}

	m_vecNPCBts[3]->pTr->SetWorldPosition(m_vecNPCBts[1]->pTr->GetWorldPosition());

	m_vecNPCBts[0]->pButton->SetStateTexture(BS_NORMAL, "UI_NPCButton_Conversation_Normal", TEXT("NPC_Talk_button_gray.png"), PATH_UI_BUTTON_BUTTON);
	m_vecNPCBts[0]->pButton->SetStateTexture(BS_CLICK, "UI_NPCButton_Conversation_Click", TEXT("NPC_Talk_button_gray_Click.png"), PATH_UI_BUTTON_BUTTON);
	m_vecNPCBts[0]->pButton->SetStateTexture(BS_MOUSEON, "UI_NPCButton_Conversation_MouseON", TEXT("NPC_Talk_button_gray_MouseON.png"), PATH_UI_BUTTON_BUTTON);
	m_vecNPCBts[0]->pButton->SetCallBackFunc(BS_CLICK, this, &NPCTalkBtManager::ClickNPCBtConversation);
	m_vecNPCBts[0]->eBtType = BtType::BT_CONVERSATION;

	m_vecNPCBts[1]->pButton->SetStateTexture(BS_NORMAL, "UI_NPCButton_Quest_Normal", TEXT("NPC_Quest_button_gray.png"), PATH_UI_BUTTON_BUTTON);
	m_vecNPCBts[1]->pButton->SetStateTexture(BS_CLICK, "UI_NPCButton_Quest_Click", TEXT("NPC_Quest_button_gray_Click.png"), PATH_UI_BUTTON_BUTTON);
	m_vecNPCBts[1]->pButton->SetStateTexture(BS_MOUSEON, "UI_NPCButton_Quest_MouseON", TEXT("NPC_Quest_button_gray_MouseON.png"), PATH_UI_BUTTON_BUTTON);
	m_vecNPCBts[1]->pButton->SetCallBackFunc(BS_CLICK, this, &NPCTalkBtManager::ClickNPCBtQuest);
	m_vecNPCBts[1]->eBtType = BtType::BT_QUEST;

	m_vecNPCBts[2]->pButton->SetStateTexture(BS_NORMAL, "UI_NPCButton_Exit_Normal", TEXT("NPC_Exit_button_gray.png"), PATH_UI_BUTTON_BUTTON);
	m_vecNPCBts[2]->pButton->SetStateTexture(BS_CLICK, "UI_NPCButton_Exit_Click", TEXT("NPC_Exit_button_gray_Click.png"), PATH_UI_BUTTON_BUTTON);
	m_vecNPCBts[2]->pButton->SetStateTexture(BS_MOUSEON, "UI_NPCButton_Exit_MouseON", TEXT("NPC_Exit_button_gray_MouseON.png"), PATH_UI_BUTTON_BUTTON);
	m_vecNPCBts[2]->pButton->SetCallBackFunc(BS_CLICK, this, &NPCTalkBtManager::ClickNPCBtExit);
	m_vecNPCBts[2]->eBtType = BtType::BT_EXIT;

	m_vecNPCBts[3]->pButton->SetStateTexture(BS_NORMAL, "UI_NPCButton_Shop_Normal", TEXT("NPC_Shop_button_gray.png"), PATH_UI_BUTTON_BUTTON);
	m_vecNPCBts[3]->pButton->SetStateTexture(BS_CLICK, "UI_NPCButton_Shop_Click", TEXT("NPC_Shop_button_gray_Click.png"), PATH_UI_BUTTON_BUTTON);
	m_vecNPCBts[3]->pButton->SetStateTexture(BS_MOUSEON, "UI_NPCButton_Shop_MouseON", TEXT("NPC_Shop_button_gray_MouseON.png"), PATH_UI_BUTTON_BUTTON);
	m_vecNPCBts[3]->pButton->SetCallBackFunc(BS_CLICK, this, &NPCTalkBtManager::ClickNPCBtShop);
	m_vecNPCBts[3]->eBtType = BtType::BT_SHOP;

	NPCButtonOff();

	AudioSound* pEffectSound = m_pGameObject->AddComponent<AudioSound>("NPCTalkBtClick");
	pEffectSound->SetSound("NPCTalkBtClick", m_pScene, false, "Button-4.mp3", PATH_SOUND_UI);
	m_pEffectSound = pEffectSound;
	SAFE_RELEASE(pEffectSound);
}

bool NPCTalkBtManager::GetNPCButtonEnable()
{
	return m_bNPCButtonEnable;
}

void NPCTalkBtManager::SetDialogueManager(DialogueManager * _Manager)
{
	pDialogueManager = _Manager;
}

void NPCTalkBtManager::SetShopManager(ShopManager * _Manager)
{
	pShopManager = _Manager;
}

void NPCTalkBtManager::SetInventory(Inventory * _Inventory)
{
	pInventory = _Inventory;
}

void NPCTalkBtManager::NPCButtonOff()
{
	m_bNPCButtonEnable = false;
	for (int i = 0; i < m_vecNPCBts.size(); ++i)
	{
		m_vecNPCBts[i]->pObject->Enable(false);
	}
}

void NPCTalkBtManager::NPCButtonOff(float _fTime)
{
	NPCButtonOff();
}

void NPCTalkBtManager::ShopNPCButtonON(NPCKind _NPC)
{
	m_vecNPCBts[0]->pObject->Enable(true);
	m_vecNPCBts[2]->pObject->Enable(true);
	m_vecNPCBts[3]->pObject->Enable(true);
}

void NPCTalkBtManager::NoneNPCButtonON(NPCKind _NPC)
{
	m_vecNPCBts[0]->pObject->Enable(true);
	m_vecNPCBts[1]->pObject->Enable(true);
	m_vecNPCBts[2]->pObject->Enable(true);
}

void NPCTalkBtManager::NPCButtonON(NPCKind _NPCType)
{
	m_eNPCKind = _NPCType;
	switch (_NPCType)
	{
	case NPC_SHOP:
		ShopNPCButtonON(_NPCType);
		break;
	case NPC_GODDESS:
	case NPC_BOY:
	case NPC_PASSERBY:
		NoneNPCButtonON(_NPCType);
		break;
	default:
		break;
	}
}

void NPCTalkBtManager::ClickNPCBtConversation(float _fTime)
{
	m_pEffectSound->Stop();
	m_pEffectSound->Play();
	if (nullptr == pDialogueManager)
	{
		return;
	}
	// 대화하자~
	if (m_eNPCKind != NPCKind::NPC_NONE)
	{
		pDialogueManager->DialogueOn(m_eNPCKind, NPCDialogueKind::NDK_CONVERSATION);
	}

	m_eNPCKind = NPC_NONE;
	NPCButtonOff();
}

void NPCTalkBtManager::ClickNPCBtQuest(float _fTime)
{
	m_pEffectSound->Stop();
	m_pEffectSound->Play();
	if (nullptr == pDialogueManager)
	{
		return;
	}
	if (m_eNPCKind != NPCKind::NPC_NONE)
	{
		if (m_eNPCKind != NPCKind::NPC_PASSERBY)
		{
			pDialogueManager->DialogueOn(m_eNPCKind, NPCDialogueKind::NDK_QUEST);
		}
	}

	m_eNPCKind = NPC_NONE;
	NPCButtonOff();
}

void NPCTalkBtManager::ClickNPCBtExit(float _fTime)
{
	m_pEffectSound->Stop();
	m_pEffectSound->Play();
	NPCButtonOff();
}

void NPCTalkBtManager::ClickNPCBtShop(float _fTime)
{
	m_pEffectSound->Stop();
	m_pEffectSound->Play();
	if (nullptr == pShopManager)
	{
		return;
	}

	// 상점일 경우엔 상점창을 띄워준다.
	// 자동으로 인벤토리도 띄워준다.
	pShopManager->ShopOn();
	pInventory->InventoryOn();
	NPCButtonOff();
}
