#include "ShopManager.h"
#include "Scene\Scene.h"
#include "Scene\Layer.h"
#include "Device.h"
#include "Time.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Inventory.h"


ShopManager::ShopManager() :
	m_pShopBGObject(nullptr), m_pShopBGTr(nullptr), m_pShopBGImage(nullptr)
	, m_pCloseBtObject(nullptr), m_pCloseBtTr(nullptr), m_pCloseBt(nullptr)
	, m_pBuyBtObject(nullptr), m_pBuyBtTr(nullptr), m_pBuyBt(nullptr)
	, m_pEffectSound(nullptr)
{
}

ShopManager::ShopManager(const ShopManager & _ShopManager)
{
	*this = _ShopManager;
	m_pLayer = _ShopManager.m_pLayer;
	m_iReferenceCount = 1;
}


ShopManager::~ShopManager()
{
	SAFE_RELEASE(m_pShopBGTr);
	SAFE_RELEASE(m_pShopBGImage);
	SAFE_RELEASE(m_pShopBGObject);

	SAFE_RELEASE(m_pCloseBtTr);
	SAFE_RELEASE(m_pCloseBt);
	SAFE_RELEASE(m_pCloseBtObject);

	SAFE_RELEASE(m_pBuyBtTr);
	SAFE_RELEASE(m_pBuyBt);
	SAFE_RELEASE(m_pBuyBtObject);

	for (int i = 0; i < m_vecShopItemObjects.size(); ++i)
	{
		SAFE_RELEASE(m_vecShopItemObjects[i]->pShopSelectObject);
		SAFE_RELEASE(m_vecShopItemObjects[i]->pShopSelectTr);
		SAFE_RELEASE(m_vecShopItemObjects[i]->pShopSelectImage);
		delete m_vecShopItemObjects[i];
	}
	m_vecShopItemObjects.clear();

	SAFE_RELEASE(m_pEffectSound);
}

void ShopManager::Start()
{
}

bool ShopManager::Init()
{
	// 배경 -> 0.3
	m_pShopBGObject = GameObject::CreateObject("ShopBG", m_pLayer);
	m_pShopBGImage = m_pShopBGObject->AddComponent<UIBar>("ShopBG");
	m_pShopBGImage->SetTexture("ShopBG", TEXT("ShopBG.png"), PATH_UI_SHOP);

	Transform* pShopBGTr = m_pShopBGObject->GetTransform();
	pShopBGTr->SetWorldPosition(_RESOLUTION.iWidth * 0.5f - 500.0f, _RESOLUTION.iHeight * 0.5f - 120.0f, 1.0f);
	pShopBGTr->SetWorldScale(412.5f, 499.5f, 1.0f);
	m_pShopBGTr = pShopBGTr;
	SAFE_RELEASE(pShopBGTr);

	// 닫기 버튼
	m_pCloseBtObject = GameObject::CreateObject("ShopCloseBt", m_pLayer);
	m_pCloseBt = m_pCloseBtObject->AddComponent<UIButton>("ShopCloseBt");
	m_pCloseBt->SetStateTexture(BS_NORMAL, "UI_InvenCloseNormal", TEXT("c_header_close.png"), PATH_UI_INVENTORY);
	m_pCloseBt->SetStateTexture(BS_CLICK, "UI_InvenCloseClick", TEXT("c_header_close-PUSH.png"), PATH_UI_INVENTORY);
	m_pCloseBt->SetStateTexture(BS_MOUSEON, "UI_InvenCloseMouseON", TEXT("c_header_close-HOVER.png"), PATH_UI_INVENTORY);
	m_pCloseBt->SetCallBackFunc(BS_CLICK, this, &ShopManager::ShopOff);

	Transform* pShopCloseBtTr = m_pCloseBtObject->GetTransform();
	pShopCloseBtTr->SetWorldPosition(_RESOLUTION.iWidth * 0.5f - 123.0f, _RESOLUTION.iHeight * 0.5f + 347.4f, 0.8f);
	pShopCloseBtTr->SetWorldScale(30.0f, 30.0f, 1.0f);
	m_pCloseBtTr = pShopCloseBtTr;
	SAFE_RELEASE(pShopCloseBtTr);

	// 구매 버튼
	m_pBuyBtObject = GameObject::CreateObject("ShopBuyBt", m_pLayer);
	m_pBuyBt = m_pBuyBtObject->AddComponent<UIButton>("ShopBuyBt");
	m_pBuyBt->SetStateTexture(BS_NORMAL, "UI_ShopBuyBtNormal", TEXT("ShopBuy_button_gray.png"), PATH_UI_SHOP);
	m_pBuyBt->SetStateTexture(BS_CLICK, "UI_ShopBuyBtClick", TEXT("ShopBuy_button_gray-PUSH.png"), PATH_UI_SHOP);
	m_pBuyBt->SetStateTexture(BS_MOUSEON, "UI_ShopBuyBtMouseON", TEXT("ShopBuy_button_gray-HOVER.png"), PATH_UI_SHOP);
	m_pBuyBt->SetPushNoneOnlyMouseON(true);
	m_pBuyBt->SetCallBackFunc(BS_CLICK, this, &ShopManager::ClickBuyButton);

	Transform* pBuyBtTr = m_pBuyBtObject->GetTransform();
	pBuyBtTr->SetWorldPosition(_RESOLUTION.iWidth * 0.5f - 262.0f, _RESOLUTION.iHeight * 0.5f - 120.0f, 0.8f);
	pBuyBtTr->SetWorldScale(173.0f, 65.0f, 1.0f);
	m_pBuyBtTr = pBuyBtTr;
	SAFE_RELEASE(pBuyBtTr);

	// 아이템 선택버튼
	m_vecShopItemObjects.resize(5);

	for (int i = 0; i < 5; ++i)
	{
		ShopItemType* pNewShopItemType = new ShopItemType;

		GameObject* pObject = GameObject::CreateObject("ShopItemSelectBt", m_pLayer);
		UIButton* pItemSelectBt = pObject->AddComponent<UIButton>("ShopCloseBt");
		pItemSelectBt->SetStateTexture(BS_NORMAL, "UI_ShopItem", TEXT("ShopItemBt.png"), PATH_UI_SHOP);
		pItemSelectBt->SetStateTexture(BS_CLICK, "UI_ShopItem", TEXT("ShopItemBt.png"), PATH_UI_SHOP);
		pItemSelectBt->SetStateTexture(BS_MOUSEON, "UI_ShopItem", TEXT("ShopItemBt.png"), PATH_UI_SHOP);
		pItemSelectBt->SetPushNoneOnlyMouseON(true);
		pItemSelectBt->SetCallBackFunc(BS_CLICK, this, &ShopManager::ClickShopItem_None);

		Transform* pItemTr = pObject->GetTransform();
		pItemTr->SetWorldPosition(_RESOLUTION.iWidth * 0.5f - 490.0f, _RESOLUTION.iHeight * 0.5f + (260.0f - (80.0f * i)), 0.7f);
		pItemTr->SetWorldScale(397.5f, 80.0f, 1.0f);

		pNewShopItemType->pShopSelectObject = pObject;
		pNewShopItemType->pShopSelectTr = pItemTr;
		pNewShopItemType->pShopSelectImage = pItemSelectBt;
		pNewShopItemType->iShopItemCount = 1;
		pNewShopItemType->strShopItemName = "NoneItem";
		pNewShopItemType->bShopItemPotion = false;
		SAFE_RELEASE(pItemTr);

		m_vecShopItemObjects[i] = pNewShopItemType;
	}

	// 아이템 선택 버튼들 알맞게 정보 바꾸기
	m_vecShopItemObjects[0]->pShopSelectImage->SetStateTexture(BS_NORMAL, "UI_ShopItem_AW_Normal", TEXT("ShopItemBt_AW.png"), PATH_UI_SHOP);
	m_vecShopItemObjects[0]->pShopSelectImage->SetStateTexture(BS_CLICK, "UI_ShopItem_AW_Click", TEXT("ShopItemBt_AW_Click.png"), PATH_UI_SHOP);
	m_vecShopItemObjects[0]->pShopSelectImage->SetStateTexture(BS_MOUSEON, "UI_ShopItem_AW_MouseON", TEXT("ShopItemBt_AW_MouseON.png"), PATH_UI_SHOP);
	m_vecShopItemObjects[0]->pShopSelectImage->SetCallBackFunc(BS_CLICK, this, &ShopManager::ClickShopItem_AW);
	m_vecShopItemObjects[0]->eItemType = ITEM_TYPE::ITEM_WEAPON;
	m_vecShopItemObjects[0]->strShopItemName = "ShopItem_AW";

	m_vecShopItemObjects[1]->pShopSelectImage->SetStateTexture(BS_NORMAL, "UI_ShopItem_HPPotion_Normal", TEXT("ShopItemBt_HPPotion.png"), PATH_UI_SHOP);
	m_vecShopItemObjects[1]->pShopSelectImage->SetStateTexture(BS_CLICK, "UI_ShopItem_HPPotion_Click", TEXT("ShopItemBt_HPPotion_Click.png"), PATH_UI_SHOP);
	m_vecShopItemObjects[1]->pShopSelectImage->SetStateTexture(BS_MOUSEON, "UI_ShopItem_HPPotion_MouseON", TEXT("ShopItemBt_HPPotion_MouseON.png"), PATH_UI_SHOP);
	m_vecShopItemObjects[1]->pShopSelectImage->SetCallBackFunc(BS_CLICK, this, &ShopManager::ClickShopItem_HPPotion);
	m_vecShopItemObjects[1]->eItemType = ITEM_TYPE::POTION_HP;
	m_vecShopItemObjects[1]->strShopItemName = "ShopItem_HPPotion";
	m_vecShopItemObjects[1]->bShopItemPotion = true;

	m_vecShopItemObjects[2]->pShopSelectImage->SetStateTexture(BS_NORMAL, "UI_ShopItem_MPPotion_Normal", TEXT("ShopItemBt_MPPotion.png"), PATH_UI_SHOP);
	m_vecShopItemObjects[2]->pShopSelectImage->SetStateTexture(BS_CLICK, "UI_ShopItem_MPPotion_Click", TEXT("ShopItemBt_MPPotion_Click.png"), PATH_UI_SHOP);
	m_vecShopItemObjects[2]->pShopSelectImage->SetStateTexture(BS_MOUSEON, "UI_ShopItem_MP_MouseON", TEXT("ShopItemBt_MPPotion_MouseON.png"), PATH_UI_SHOP);
	m_vecShopItemObjects[2]->pShopSelectImage->SetCallBackFunc(BS_CLICK, this, &ShopManager::ClickShopTiem_MPPotion);
	m_vecShopItemObjects[2]->eItemType = ITEM_TYPE::POTION_MP;
	m_vecShopItemObjects[2]->strShopItemName = "ShopItem_MPPotion";
	m_vecShopItemObjects[2]->bShopItemPotion = true;

	AudioSound* pEffectSound = m_pGameObject->AddComponent<AudioSound>("ShopBtClick");
	pEffectSound->SetSound("ShopBtClick", m_pScene, false, "Button-4.mp3", PATH_SOUND_UI);
	m_pEffectSound = pEffectSound;
	SAFE_RELEASE(pEffectSound);
	return true;
}

int ShopManager::Input(float _fTime)
{
	return 0;
}

int ShopManager::Update(float _fTime)
{
	return 0;
}

int ShopManager::LateUpdate(float _fTime)
{
	return 0;
}

int ShopManager::Collision(float _fTime)
{
	return 0;
}

int ShopManager::PrevRender(float _fTime)
{
	return 0;
}

int ShopManager::Render(float _fTime)
{
	return 0;
}

ShopManager * ShopManager::Clone() const
{
	return nullptr;
}

bool ShopManager::GetShopEnable()
{
	return m_bShopEnable;
}

void ShopManager::SetInventory(Inventory * _Script)
{
	pInventory = _Script;
}

void ShopManager::ShopEnable(bool _Enable)
{
	m_bShopEnable = _Enable;
}

void ShopManager::ShopOff()
{
	m_pClickShopItemInfo = nullptr;

	m_pShopBGObject->Enable(false);
	m_pCloseBtObject->Enable(false);
	m_pBuyBtObject->Enable(false);

	for (int i = 0; i < m_vecShopItemObjects.size(); ++i)
	{
		m_vecShopItemObjects[i]->pShopSelectObject->Enable(false);
	}
}

void ShopManager::ShopOff(float _fTime)
{
	m_pEffectSound->Stop();
	m_pEffectSound->Play();
	m_bShopEnable = false;
	NoneClickBt();
	ShopOff();
}

void ShopManager::ShopOn()
{
	m_pClickShopItemInfo = nullptr;

	m_pShopBGObject->Enable(true);
	m_pCloseBtObject->Enable(true);
	m_pBuyBtObject->Enable(true);

	for (int i = 0 ; i < m_vecShopItemObjects.size(); ++i)
	{
		m_vecShopItemObjects[i]->pShopSelectObject->Enable(true);
	}
}

void ShopManager::ClickShopItem_None(float _fTime)
{
	m_pClickShopItemInfo = nullptr;
}

void ShopManager::ClickShopItem_AW(float _fTime)
{
	m_pEffectSound->Stop();
	m_pEffectSound->Play();
	NoneClickBt();
	m_pClickShopItemInfo = m_vecShopItemObjects[0];
	m_vecShopItemObjects[0]->pShopSelectImage->SetStateTexture(BS_NORMAL, "UI_ShopItem_AW_Click", TEXT("ShopItemBt_AW_Click.png"), PATH_UI_SHOP);
	m_vecShopItemObjects[0]->pShopSelectImage->SetStateTexture(BS_CLICK, "UI_ShopItem_AW_Click", TEXT("ShopItemBt_AW_Click.png"), PATH_UI_SHOP);
	m_vecShopItemObjects[0]->pShopSelectImage->SetStateTexture(BS_MOUSEON, "UI_ShopItem_AW_Click", TEXT("ShopItemBt_AW_Click.png"), PATH_UI_SHOP);
}

void ShopManager::ClickShopItem_HPPotion(float _fTime)
{
	m_pEffectSound->Stop();
	m_pEffectSound->Play();
	NoneClickBt();
	m_pClickShopItemInfo = m_vecShopItemObjects[1];
	m_vecShopItemObjects[1]->pShopSelectImage->SetStateTexture(BS_NORMAL, "UI_ShopItem_HPPotion_Click", TEXT("UI_ShopItem_HPPotion_Click.png"), PATH_UI_SHOP);
	m_vecShopItemObjects[1]->pShopSelectImage->SetStateTexture(BS_CLICK, "UI_ShopItem_HPPotion_Click", TEXT("UI_ShopItem_HPPotion_Click.png"), PATH_UI_SHOP);
	m_vecShopItemObjects[1]->pShopSelectImage->SetStateTexture(BS_MOUSEON, "UI_ShopItem_HPPotion_Click", TEXT("UI_ShopItem_HPPotion_Click.png"), PATH_UI_SHOP);
}

void ShopManager::ClickShopTiem_MPPotion(float _fTime)
{
	m_pEffectSound->Stop();
	m_pEffectSound->Play();
	NoneClickBt();
	m_pClickShopItemInfo = m_vecShopItemObjects[2];
	m_vecShopItemObjects[2]->pShopSelectImage->SetStateTexture(BS_NORMAL, "UI_ShopItem_MPPotion_Click", TEXT("ShopItemBt_MPPotion_Click.png"), PATH_UI_SHOP);
	m_vecShopItemObjects[2]->pShopSelectImage->SetStateTexture(BS_CLICK, "UI_ShopItem_MPPotion_Click", TEXT("ShopItemBt_MPPotion_Click.png"), PATH_UI_SHOP);
	m_vecShopItemObjects[2]->pShopSelectImage->SetStateTexture(BS_MOUSEON, "UI_ShopItem_MPPotion_Click", TEXT("ShopItemBt_MPPotion_Click.png"), PATH_UI_SHOP);
}

void ShopManager::ClickBuyButton(float _fTime)
{
	m_pEffectSound->Stop();
	m_pEffectSound->Play();
	// 인벤토리 알아와서
	// 인벤토리에 아이템 추가해주기 ( AddItem )
	// 빈공간 있는지 여유 확인 및 같은 아이템이 존재할 경우 
	// 아이템 순서대로 빈아이템들 채우기 아이템은 99개까지 존재가능
	// 구매할 아이템이 선택되지 않았으면  return
	if (m_pClickShopItemInfo != nullptr && pInventory != nullptr)
	{
		int iCalMoeny = 0;
		int iCount = 0;
		switch (m_pClickShopItemInfo->eItemType)
		{
		case ITEM_TYPE::POTION_HP:
			iCalMoeny = POTION_HP_VALUE;
			iCount = 20;
			break;
		case ITEM_TYPE::POTION_MP:
			iCalMoeny = POTION_MP_VALUE;
			iCount = 20;
			break;
		case ITEM_TYPE::ITEM_WEAPON:
			iCalMoeny = WEAPON_VALUE;;
			iCount = 1;
			break;
		default:
			break;
		}
		pInventory->BuyItem(m_pClickShopItemInfo->eItemType, iCount, iCalMoeny);
		return;
	}
}

void ShopManager::NoneClickBt()
{
	m_pClickShopItemInfo = nullptr;

	m_vecShopItemObjects[0]->pShopSelectImage->SetStateTexture(BS_NORMAL, "UI_ShopItem_AW_Normal", TEXT("ShopItemBt_AW.png"), PATH_UI_SHOP);
	m_vecShopItemObjects[0]->pShopSelectImage->SetStateTexture(BS_CLICK, "UI_ShopItem_AW_Click", TEXT("ShopItemBt_AW_Click.png"), PATH_UI_SHOP);
	m_vecShopItemObjects[0]->pShopSelectImage->SetStateTexture(BS_MOUSEON, "UI_ShopItem_AW_MouseON", TEXT("ShopItemBt_AW_MouseON.png"), PATH_UI_SHOP);

	m_vecShopItemObjects[1]->pShopSelectImage->SetStateTexture(BS_NORMAL, "UI_ShopItem_HPPotion_Normal", TEXT("ShopItemBt_HPPotion.png"), PATH_UI_SHOP);
	m_vecShopItemObjects[1]->pShopSelectImage->SetStateTexture(BS_CLICK, "UI_ShopItem_HPPotion_Click", TEXT("ShopItemBt_HPPotion_Click.png"), PATH_UI_SHOP);
	m_vecShopItemObjects[1]->pShopSelectImage->SetStateTexture(BS_MOUSEON, "UI_ShopItem_HPPotion_MouseON", TEXT("ShopItemBt_HPPotion_MouseON.png"), PATH_UI_SHOP);

	m_vecShopItemObjects[2]->pShopSelectImage->SetStateTexture(BS_NORMAL, "UI_ShopItem_MPPotion_Normal", TEXT("ShopItemBt_MPPotion.png"), PATH_UI_SHOP);
	m_vecShopItemObjects[2]->pShopSelectImage->SetStateTexture(BS_CLICK, "UI_ShopItem_MPPotion_Click", TEXT("ShopItemBt_MPPotion_Click.png"), PATH_UI_SHOP);
	m_vecShopItemObjects[2]->pShopSelectImage->SetStateTexture(BS_MOUSEON, "UI_ShopItem_MP_MouseON", TEXT("ShopItemBt_MPPotion_MouseON.png"), PATH_UI_SHOP);
}
