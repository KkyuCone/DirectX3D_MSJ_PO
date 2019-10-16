#include "Inventory.h"
#include "Scene\Scene.h"
#include "Scene\Layer.h"
#include "Device.h"
#include "Time.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Weapon.h"


Inventory::Inventory() :
	m_bInventoryEnable(false)
	, m_pInventoryBGObject(nullptr)
	, m_pCloseButtonObject(nullptr)
	, m_pCloseButton(nullptr)
	, m_pInventoryBGImage(nullptr)
	, m_pScrollBarBGObject(nullptr)
	, m_pScrollBarBGImage(nullptr)
	, m_pScrollBarObject(nullptr)
	, m_pScrollBarButton(nullptr)
	, m_pScrollBarUpObject(nullptr)
	, m_pScrollBarUpButton(nullptr)
	, m_pScrollBarDownObject(nullptr)
	, m_pScrollBarDownButton(nullptr)
	, m_pInventoryMoveObject(nullptr)
	, m_pInventoryMoveButton(nullptr)
	, m_pInventoryBGTr(nullptr)
	, m_pInventoryMoveTr(nullptr)
	, m_pCloseButtonTr(nullptr)
	, m_pScrollBarBGTr(nullptr)
	, m_pScrollbarTr(nullptr)
	, m_pScrollBarUpTr(nullptr)
	, m_pScrollBarDownTr(nullptr)
	, m_pClickInvenItem(nullptr)
	, m_pMouseItemImageScript(nullptr)
	, m_pEffectSound(nullptr)
{
	m_vPrevMousePos = Vector2::Zero;
	m_vCurMousePos = Vector2::Zero;

	m_vPrevBarPos = Vector2::Zero;
	m_vCurBarPos = Vector2::Zero;

	m_iScrollMin = 0;
	m_iScrollMax = 4;
	m_iScrollCur = m_iScrollMax;
}

Inventory::Inventory(const Inventory & _Inventory) : Base(_Inventory)
{
	*this = _Inventory;
	m_pLayer = _Inventory.m_pLayer;
	m_iReferenceCount = 1;
}


Inventory::~Inventory()
{
	SAFE_RELEASE(m_pInventoryBGObject);
	SAFE_RELEASE(m_pInventoryBGImage);

	SAFE_RELEASE(m_pInventoryMoveObject);
	SAFE_RELEASE(m_pInventoryMoveButton);

	SAFE_RELEASE(m_pCloseButton);
	SAFE_RELEASE(m_pCloseButtonObject);

	// 스크롤
	SAFE_RELEASE(m_pScrollBarBGObject);
	SAFE_RELEASE(m_pScrollBarBGImage);

	SAFE_RELEASE(m_pScrollBarObject);
	SAFE_RELEASE(m_pScrollBarButton);

	SAFE_RELEASE(m_pScrollBarUpObject);
	SAFE_RELEASE(m_pScrollBarUpButton);

	SAFE_RELEASE(m_pScrollBarDownObject);
	SAFE_RELEASE(m_pScrollBarDownButton);

	for (int i =0; i < m_pItemObjects.size(); ++i)
	{
		SAFE_RELEASE(m_pItemObjects[i]->pEmptyObj);
		SAFE_RELEASE(m_pItemObjects[i]->pItemObj);
		SAFE_RELEASE(m_pItemObjects[i]->pEmptyImage);
		SAFE_RELEASE(m_pItemObjects[i]->pItemButton);
		SAFE_RELEASE(m_pItemObjects[i]->pEmptyTr);
		SAFE_RELEASE(m_pItemObjects[i]->pItemTr);
		SAFE_RELEASE(m_pItemObjects[i]->pItemTensTr);
		SAFE_RELEASE(m_pItemObjects[i]->pItemTensCountImage);
		SAFE_RELEASE(m_pItemObjects[i]->pItemTensCountObject);
		SAFE_RELEASE(m_pItemObjects[i]->pItemUnitsTr);
		SAFE_RELEASE(m_pItemObjects[i]->pItemUnitsCountImage);
		SAFE_RELEASE(m_pItemObjects[i]->pItemUnitsCountObject);
		SAFE_DELETE(m_pItemObjects[i]);
	}

	m_pItemObjects.clear();

	// 트랜스폼들
	SAFE_RELEASE(m_pInventoryBGTr);
	SAFE_RELEASE(m_pInventoryMoveTr);
	SAFE_RELEASE(m_pCloseButtonTr);
	SAFE_RELEASE(m_pScrollBarBGTr);
	SAFE_RELEASE(m_pScrollbarTr);
	SAFE_RELEASE(m_pScrollBarUpTr);
	SAFE_RELEASE(m_pScrollBarDownTr);

	SAFE_RELEASE(m_pMouseItemImageScript);

	// 돈
	for (int i = 0; i < m_vecMoney.size(); ++i)
	{
		SAFE_RELEASE(m_vecMoney[i]->pObject);
		SAFE_RELEASE(m_vecMoney[i]->pTr);
		SAFE_RELEASE(m_vecMoney[i]->pNumberImage);		
		delete m_vecMoney[i];
	}
	m_vecMoney.clear();

	SAFE_RELEASE(m_pEffectSound);
}

void Inventory::Start()
{
	// 인벤토리 배경
	m_pInventoryBGObject = GameObject::CreateObject("InventoryBG", m_pLayer);
	m_pInventoryBGImage = m_pInventoryBGObject->AddComponent<UIBar>("InventoryBG");
	m_pInventoryBGImage->SetTexture("HPBar", TEXT("Inventory_BG_Coin.png"), PATH_UI_INVENTORY);
	m_pInventoryBGImage->SetShader(SHADER_UI_ONLY_IMAGE);

	Transform* pInvenBGTR = m_pInventoryBGObject->GetTransform();
	pInvenBGTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f, _RESOLUTION.iHeight * 0.5f, 1.0f);
	pInvenBGTR->SetWorldScale(583.5f, 378.5f, 1.0f);
	m_pInventoryBGTr = pInvenBGTR;
	Vector3 vInvenBasePos = pInvenBGTR->GetWorldPosition();								// 인벤토리 기본 위치

	SAFE_RELEASE(pInvenBGTR);

	// 인벤토리 이동용 오브젝트
	m_pInventoryMoveObject = GameObject::CreateObject("InventoryMoveObject", m_pLayer);
	m_pInventoryMoveButton = m_pInventoryMoveObject->AddComponent<UIButton>("InventoryMoveButton");
	m_pInventoryMoveButton->SetStateTexture(BS_NORMAL, "UI_InvenMoveNormal", TEXT("Inventory_MoveObj.png"), PATH_UI_INVENTORY);
	m_pInventoryMoveButton->SetStateTexture(BS_CLICK, "UI_InvenMoveNormal", TEXT("Inventory_MoveObj.png"), PATH_UI_INVENTORY);
	m_pInventoryMoveButton->SetStateTexture(BS_MOUSEON, "UI_InvenMoveNormal", TEXT("Inventory_MoveObj.png"), PATH_UI_INVENTORY);
	m_pInventoryMoveButton->SetStateTexture(BS_PUSH, "UI_InvenMoveNormal", TEXT("Inventory_MoveObj.png"), PATH_UI_INVENTORY);
	m_pInventoryMoveButton->SetCallBackFunc(BS_MOUSEON, this, &Inventory::InventoryPrevPos);
	m_pInventoryMoveButton->SetCallBackFunc(BS_PUSH, this, &Inventory::InventoryMove);

	Transform* pInvenMoveObjTR = m_pInventoryMoveObject->GetTransform();
	pInvenMoveObjTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f, _RESOLUTION.iHeight * 0.5f + 335.0f, 0.3f);
	pInvenMoveObjTR->SetWorldScale(543.5f, 40.0f, 1.0f);
	m_pInventoryMoveTr = pInvenMoveObjTR;
	SAFE_RELEASE(pInvenMoveObjTR);


	// 인벤토리 닫기 버튼
	m_pCloseButtonObject = GameObject::CreateObject("InvenetoryCloseButton", m_pLayer);
	//m_pCloseButtonObject->SetParent(m_pGameObject);
	m_pCloseButton = m_pCloseButtonObject->AddComponent<UIButton>("InventoryCloseButton");
	m_pCloseButton->SetStateTexture(BS_NORMAL, "UI_InvenCloseNormal", TEXT("c_header_close.png"), PATH_UI_INVENTORY);
	m_pCloseButton->SetStateTexture(BS_CLICK, "UI_InvenCloseClick", TEXT("c_header_close-PUSH.png"), PATH_UI_INVENTORY);
	m_pCloseButton->SetStateTexture(BS_MOUSEON, "UI_InvenCloseMouseON", TEXT("c_header_close-HOVER.png"), PATH_UI_INVENTORY);
	m_pCloseButton->SetCallBackFunc(BS_CLICK, this, &Inventory::InventoryOff);

	Transform* pInvenCloseButtonTR = m_pCloseButtonObject->GetTransform();
	pInvenCloseButtonTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f + 546.0f, _RESOLUTION.iHeight * 0.5f + 342.0f, 0.8f);
	pInvenCloseButtonTR->SetWorldScale(29.0f, 29.0f, 1.0f);
	m_pCloseButtonTr = pInvenCloseButtonTR;
	SAFE_RELEASE(pInvenCloseButtonTR);

	// 인벤토리 스크롤
	// 스크롤 -  바 배경
	m_pScrollBarBGObject = GameObject::CreateObject("Inventory_ScrollBarBG", m_pLayer);
	m_pScrollBarBGImage = m_pScrollBarBGObject->AddComponent<UIBar>("Inventory_ScrollBarBG");
	m_pScrollBarBGImage->SetTexture("Inventory_ScrollBarBG", TEXT("scroll_bar.png"), PATH_UI_SCROLLBAR);
	m_pScrollBarBGImage->SetShader(SHADER_UI_ONLY_IMAGE);

	Transform* pScrollBarBGTR = m_pScrollBarBGObject->GetTransform();
	pScrollBarBGTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f + 559.0f, _RESOLUTION.iHeight * 0.5f + 32.0f, 0.8f);
	pScrollBarBGTR->SetWorldScale(14.0f, 305.0f, 1.0f);
	m_pScrollBarBGTr = pScrollBarBGTR;
	SAFE_RELEASE(pScrollBarBGTR);

	// 스크롤 - 위(버튼)
	m_pScrollBarUpObject = GameObject::CreateObject("Inventory_ScrollBar_UpButton", m_pLayer);
	m_pScrollBarUpButton = m_pScrollBarUpObject->AddComponent<UIButton>("Inventory_ScrollBar_UpButton");
	m_pScrollBarUpButton->SetStateTexture(BS_NORMAL, "UI_ScrollBar_UpButton_Normal", TEXT("scroll_top_arrow.png"), PATH_UI_SCROLLBAR);
	m_pScrollBarUpButton->SetStateTexture(BS_CLICK, "UI_ScrollBar_UpButton_Click", TEXT("scroll_top_arrow-HOVER.png"), PATH_UI_SCROLLBAR);
	m_pScrollBarUpButton->SetStateTexture(BS_MOUSEON, "UI_ScrollBar_UpButton_MouseON", TEXT("scroll_top_arrow-HOVER.png"), PATH_UI_SCROLLBAR);
	m_pScrollBarUpButton->SetStateColor(BS_CLICK, Vector4(0.6f, 0.6f, 0.6f, 1.0f));
	m_pScrollBarUpButton->SetCallBackFunc(BS_CLICK, this, &Inventory::InventoryRaiseItem);

	Transform* pScrollBarUpTR = m_pScrollBarUpObject->GetTransform();
	pScrollBarUpTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f + 556.0f, _RESOLUTION.iHeight * 0.5f + 316.0f, 0.6f);
	pScrollBarUpTR->SetWorldScale(22.0f, 22.0f, 1.0f);
	m_pScrollBarUpTr = pScrollBarUpTR;
	SAFE_RELEASE(pScrollBarUpTR);

	// 스크롤 - 아래(버튼)
	m_pScrollBarDownObject = GameObject::CreateObject("Inventory_ScrollBar_DownButton", m_pLayer);
	m_pScrollBarDownButton = m_pScrollBarDownObject->AddComponent<UIButton>("Inventory_ScrollBar_DownButton");
	m_pScrollBarDownButton->SetStateTexture(BS_NORMAL, "UI_ScrollBar_DownButton_Normal", TEXT("scroll_bot_arrow.png"), PATH_UI_SCROLLBAR);
	m_pScrollBarDownButton->SetStateTexture(BS_CLICK, "UI_ScrollBar_DownButton_Click", TEXT("scroll_bot_arrow-HOVER.png"), PATH_UI_SCROLLBAR);
	m_pScrollBarDownButton->SetStateTexture(BS_MOUSEON, "UI_ScrollBar_DownButton_MouseON", TEXT("scroll_bot_arrow-HOVER.png"), PATH_UI_SCROLLBAR);
	m_pScrollBarDownButton->SetStateColor(BS_CLICK, Vector4(0.6f, 0.6f, 0.6f, 1.0f));
	m_pScrollBarDownButton->SetCallBackFunc(BS_CLICK, this, &Inventory::InventoryLiftItem);

	Transform* pScrollBarDownTR = m_pScrollBarDownObject->GetTransform();
	pScrollBarDownTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f + 556.0f, _RESOLUTION.iHeight * 0.5f + 32.0f, 0.6f);
	pScrollBarDownTR->SetWorldScale(22.0f, 22.0f, 1.0f);
	m_pScrollBarDownTr = pScrollBarDownTR;
	SAFE_RELEASE(pScrollBarDownTR);

	// 스크롤 - 바
	m_pScrollBarObject = GameObject::CreateObject("Inventory_ScrollBar", m_pLayer);
	m_pScrollBarButton = m_pScrollBarObject->AddComponent<UIButton>("Inventory_ScrollBar");
	m_pScrollBarButton->SetStateTexture(BS_NORMAL, "UI_ScrollBar_Normal", TEXT("scroll_handler.png"), PATH_UI_SCROLLBAR);
	m_pScrollBarButton->SetStateTexture(BS_CLICK, "UI_ScrollBar_Click", TEXT("scroll_handler-HOVER.png"), PATH_UI_SCROLLBAR);
	m_pScrollBarButton->SetStateTexture(BS_MOUSEON, "UI_ScrollBar_MouseON", TEXT("scroll_handler-HOVER.png"), PATH_UI_SCROLLBAR);
	m_pScrollBarButton->SetStateColor(BS_CLICK, Vector4(0.6f, 0.6f, 0.6f, 1.0f));
	m_pScrollBarButton->SetCallBackFunc(BS_MOUSEON, this, &Inventory::InventoryBarPrevPos);
	m_pScrollBarButton->SetCallBackFunc(BS_PUSH, this, &Inventory::InventoryBarMove);
	m_pScrollBarButton->SetCallBackFunc(BS_CLICK, this, &Inventory::InventoryBarMoveClickUp);

	Transform* pScrollBarTR = m_pScrollBarObject->GetTransform();
	pScrollBarTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f + 556.0f, _RESOLUTION.iHeight * 0.5f + 245.0f, 0.6f);				// 세로  52.0f ~ 245.0f
	pScrollBarTR->SetWorldScale(22.0f, 73.0f, 1.0f);
	m_pScrollbarTr = pScrollBarTR;
	SAFE_RELEASE(pScrollBarTR);

	// 인벤토리 타일 생성
	vInvenBasePos.x += 10.0f;
	vInvenBasePos.y += 275.0f;
	vInvenBasePos.z = 0.7f;

	Vector3 vInventoryPos = vInvenBasePos;

	m_pItemObjects.resize(INVEN_X * INVEN_Y);

	for (int y = 0; y < INVEN_Y; ++y)
	{
		vInventoryPos = vInvenBasePos;
		float fYPos = y * 60.5f;
		vInventoryPos.y -= fYPos;

		for (int x = 0; x < INVEN_X; ++x)
		{
			vInventoryPos.x = vInvenBasePos.x;
			float fXPos = x * 61.0f;
			vInventoryPos.x += fXPos;

			// 인벤토리 해당칸
			InventoryType* pNewInven = new InventoryType;
			int IndexNumber = x + (9 * y);
			m_pItemObjects[IndexNumber] = pNewInven;

			// 기본 인벤토리 이미지 생성
			GameObject* pInvenEmptyObj = GameObject::CreateObject("InvenEmpty", m_pLayer);
			UIButton* pInvenEmptyImage = pInvenEmptyObj->AddComponent<UIButton>("InvenEmptyImage");
			pInvenEmptyImage->SetStateTexture(BS_NORMAL, "InvenEmptyImage_NORMAL", TEXT("Inventory_NORMAL.png"), PATH_UI_INVENTORY);
			pInvenEmptyImage->SetStateTexture(BS_MOUSEON, "InvenEmptyImage_MOUSEON", TEXT("Inventory_MouseON.png"), PATH_UI_INVENTORY);
			pInvenEmptyImage->SetStateTexture(BS_CLICK, "InvenEmptyImage_MOUSEON", TEXT("Inventory_MouseON.png"), PATH_UI_INVENTORY);
			pInvenEmptyImage->SetStateTexture(BS_RCLICK, "InvenEmptyImage_NORMAL", TEXT("Inventory_NORMAL.png"), PATH_UI_INVENTORY);
			pInvenEmptyImage->SetStateColor(BS_PUSH, Vector4(0.6f, 0.6f, 0.6f, 1.0f));
			pInvenEmptyImage->SetStateColor(BS_CLICK, Vector4(0.6f, 0.6f, 0.6f, 1.0f));
			//pInvenEmptyImage->SetShader(SHADER_UI_BAR);
			Transform*	pInvenEmptyTr = pInvenEmptyObj->GetTransform();
			pInvenEmptyTr->SetWorldPosition(vInventoryPos.x, vInventoryPos.y, 0.7f);
			pInvenEmptyTr->SetWorldScale(61.0f, 60.5f, 1.0f);

			pNewInven->pEmptyObj = pInvenEmptyObj;
			pNewInven->pEmptyImage = pInvenEmptyImage;
			pNewInven->pEmptyTr = pInvenEmptyTr;
			SAFE_RELEASE(pInvenEmptyTr);
			SAFE_RELEASE(pInvenEmptyImage);
			SAFE_RELEASE(pInvenEmptyObj);

			// 아이템 이미지
			GameObject* pInvenItemObj = GameObject::CreateObject("InvenItem", m_pLayer);
			UIButton* pInvenItemImage = pInvenItemObj->AddComponent<UIButton>("InvenItem");
			pInvenItemImage->SetStateTexture(BS_NORMAL, "Item_NONE", TEXT("Item_NONE.png"), PATH_UI_ITEM);
			pInvenItemImage->SetStateTexture(BS_MOUSEON, "Item_NONE", TEXT("Item_NONE.png"), PATH_UI_ITEM);
			pInvenItemImage->SetStateTexture(BS_CLICK, "Item_NONE", TEXT("Item_NONE.png"), PATH_UI_ITEM);
			pInvenItemImage->SetStateTexture(BS_PUSH, "Item_NONE", TEXT("Item_NONE.png"), PATH_UI_ITEM);
			pInvenItemImage->SetStateTexture(BS_RCLICK, "Item_NONE", TEXT("Item_NONE.png"), PATH_UI_ITEM);
			pInvenItemImage->SetStateColor(BS_PUSH, Vector4(0.6f, 0.6f, 0.6f, 1.0f));
			pInvenItemImage->SetStateColor(BS_CLICK, Vector4(0.6f, 0.6f, 0.6f, 1.0f));
			pInvenItemImage->SetCallBackFunc(BS_CLICK, this, &Inventory::SelectItem);
			pInvenItemImage->SetCallBackFunc(BS_RCLICK, this, &Inventory::UseItem);
			Transform*	pInvenItemTr = pInvenItemObj->GetTransform();
			pInvenItemTr->SetWorldPosition(vInventoryPos.x + 6.0f, vInventoryPos.y + 6.5f, 0.6f);
			pInvenItemTr->SetWorldScale(47.0f, 47.0f, 1.0f);

			pNewInven->pItemObj = pInvenItemObj;
			pNewInven->pItemButton = pInvenItemImage;
			pNewInven->pItemTr = pInvenItemTr;
			SAFE_RELEASE(pInvenItemTr);
			SAFE_RELEASE(pInvenItemImage);
			SAFE_RELEASE(pInvenItemObj);

			// 아이템 개수 표시하는 이미지
			// 10의자리
			GameObject*	pNewItemTensObject = GameObject::CreateObject("ItemTensCount", m_pLayer);
			UIBar*		pNewItemTensImage = pNewItemTensObject->AddComponent<UIBar>("ItemTensCountImage");
			pNewItemTensImage->SetTexture("ItemCountNum_NONE", TEXT("ItemCountNum_NONE.png"), PATH_UI_NUMBER);
			pNewItemTensImage->SetShader(SHADER_UI_ONLY_IMAGE);

			Transform*	pNewItemTensTr = pNewItemTensObject->GetTransform();
			pNewItemTensTr->SetWorldPosition(vInventoryPos.x + 34.0f, vInventoryPos.y + 3.0f, 0.4f);
			pNewItemTensTr->SetWorldScale(13.0f, 15.0f, 1.0f);

			pNewInven->pItemTensCountObject = pNewItemTensObject;
			pNewInven->pItemTensCountImage = pNewItemTensImage;
			pNewInven->pItemTensTr = pNewItemTensTr;
			SAFE_RELEASE(pNewItemTensTr);
			SAFE_RELEASE(pNewItemTensImage);
			SAFE_RELEASE(pNewItemTensObject);

			// 1의 자리
			GameObject*	pNewItemUnitsObject = GameObject::CreateObject("ItemTensCount", m_pLayer);
			UIBar*		pNewItemUnitsImage = pNewItemUnitsObject->AddComponent<UIBar>("ItemTensCountImage");
			pNewItemUnitsImage->SetTexture("ItemCountNum_NONE", TEXT("ItemCountNum_NONE.png"), PATH_UI_NUMBER);
			pNewItemUnitsImage->SetShader(SHADER_UI_ONLY_IMAGE);

			Transform*	pNewItemUnitsTr = pNewItemUnitsObject->GetTransform();
			pNewItemUnitsTr->SetWorldPosition(vInventoryPos.x + 44.0f, vInventoryPos.y + 3.0f, 0.4f);
			pNewItemUnitsTr->SetWorldScale(13.0f, 15.0f, 1.0f);

			pNewInven->pItemUnitsCountObject = pNewItemUnitsObject;
			pNewInven->pItemUnitsCountImage = pNewItemUnitsImage;
			pNewInven->pItemUnitsTr = pNewItemUnitsTr;
			SAFE_RELEASE(pNewItemUnitsTr);
			SAFE_RELEASE(pNewItemUnitsImage);
			SAFE_RELEASE(pNewItemUnitsObject);

			pNewInven->iIndex = IndexNumber;
			pNewInven->bItem = false;
		}
	}

	InventoryCutItem();
	MoneyInit();
	InventoryOff();


	AudioSound* pEffectSound = m_pGameObject->AddComponent<AudioSound>("InvenBtClick");
	pEffectSound->SetSound("InvenBtClick", m_pScene, false, "Button-4.mp3", PATH_SOUND_UI);
	m_pEffectSound = pEffectSound;
	SAFE_RELEASE(pEffectSound);
}

bool Inventory::Init()
{
	return true;
}

int Inventory::Input(float _fTime)
{
	return 0;
}

int Inventory::Update(float _fTime)
{
	InventoryScrollMoveMouseWheel(_fTime);
	return 0;
}

int Inventory::LateUpdate(float _fTime)
{

	return 0;
}

int Inventory::Collision(float _fTime)
{
	return 0;
}

int Inventory::PrevRender(float _fTime)
{
	return 0;
}

int Inventory::Render(float _fTime)
{
	return 0;
}

Inventory * Inventory::Clone() const
{
	return new Inventory(*this);
}

void Inventory::MoneyInit()
{
	Vector3 vBaseNumPos = Vector3(_RESOLUTION.iWidth * 0.5f + 558.0f, _RESOLUTION.iHeight * 0.5f + 12.0f, 0.5f);
	for (int i = 0; i < CIPHER_VALUE; ++i)
	{
		MoneyNumber* NewMoneyNumber = new MoneyNumber;

		GameObject* pNewObject = GameObject::CreateObject("InventoryMoney", m_pLayer);
		UIBar* pNewBar = pNewObject->AddComponent<UIBar>("InventoryMoney");
		pNewBar->SetTexture("ItemCountNum_0", TEXT("ItemCountNum_0.png"), PATH_UI_NUMBER);
		pNewBar->SetShader(SHADER_UI_ONLY_IMAGE);
		Transform* pNewObjTr = pNewObject->GetTransform();
		Vector3 vMoneypos = vBaseNumPos;
		vMoneypos.x -= (12.0f * i);
		pNewObjTr->SetWorldPosition(vMoneypos);
		pNewObjTr->SetWorldScale(14.0f, 20.0f, 1.0f);

		NewMoneyNumber->pObject = pNewObject;
		NewMoneyNumber->pTr = pNewObjTr;
		NewMoneyNumber->pNumberImage = pNewBar;

		m_vecMoney.push_back(NewMoneyNumber);
		SAFE_RELEASE(pNewObjTr);
	}

	// 돈계산
	m_iMoney = 10000;
	CalculateMoney(0);
}

bool Inventory::GetInventoryEnable()
{
	return m_bInventoryEnable;
}

void Inventory::SetPlayerWeaponScript(Weapon * _Script)
{
	pPlayerWeapon = _Script;
}

void Inventory::InventoryEnable(bool _Enable)
{
	m_bInventoryEnable = _Enable;
}

void Inventory::InventoryInputKey()
{
	m_bInventoryEnable = !m_bInventoryEnable;

	if (true == m_bInventoryEnable)
	{
		InventoryOn();
	}
	else
	{
		InventoryOff();
	}
}

void Inventory::InventoryOff()
{
	// 인벤토리 끄기
	m_bInventoryEnable = false;
	m_pInventoryBGObject->Enable(false);
	m_pCloseButtonObject->Enable(false);
	m_pScrollBarBGObject->Enable(false);
	m_pScrollBarObject->Enable(false);
	m_pScrollBarUpObject->Enable(false);
	m_pScrollBarDownObject->Enable(false);

	for (int i = 0; i < m_pItemObjects.size(); ++i)
	{
		m_pItemObjects[i]->pEmptyObj->Enable(false);
		m_pItemObjects[i]->pItemObj->Enable(false);
		m_pItemObjects[i]->pItemTensCountObject->Enable(false);
		m_pItemObjects[i]->pItemUnitsCountObject->Enable(false);
	}

	for (int i = 0; i < m_vecMoney.size(); ++i)
	{
		m_vecMoney[i]->pObject->Enable(false);
	}
}

void Inventory::InventoryOff(float _fTime)
{
	m_bInventoryEnable = false;
	InventoryOff();
}

void Inventory::InventoryOn()
{
	// 인벤토리 켜기
	m_bInventoryEnable = true;
	m_pInventoryBGObject->Enable(true);
	m_pCloseButtonObject->Enable(true);
	m_pScrollBarBGObject->Enable(true);
	m_pScrollBarObject->Enable(true);
	m_pScrollBarUpObject->Enable(true);
	m_pScrollBarDownObject->Enable(true);

	for (int i = 0; i < m_pItemObjects.size(); ++i)
	{
		m_pItemObjects[i]->pEmptyObj->Enable(true);
		m_pItemObjects[i]->pItemObj->Enable(true);
		m_pItemObjects[i]->pItemTensCountObject->Enable(true);
		m_pItemObjects[i]->pItemUnitsCountObject->Enable(true);
	}

	for (int i = 0; i < m_vecMoney.size(); ++i)
	{
		m_vecMoney[i]->pObject->Enable(true);
	}

	InventoryCutItem();
}

void Inventory::InventoryPrevPos(float _fTime)
{
	m_vPrevMousePos = GET_SINGLETON(InputManager)->GetUIMouseViewportPos();
}

void Inventory::InventoryMove(float _fTime)
{
	// InputManager
	m_vCurMousePos = GET_SINGLETON(InputManager)->GetUIMouseViewportPos();
	Vector2 MovePos = m_vCurMousePos - m_vPrevMousePos;
	Vector3 vMovePos = Vector3(MovePos.x, MovePos.y, 0.0f);

	m_pInventoryBGTr->Move(vMovePos);
	m_pInventoryMoveTr->Move(vMovePos);
	m_pCloseButtonTr->Move(vMovePos);
	m_pScrollBarBGTr->Move(vMovePos);
	m_pScrollbarTr->Move(vMovePos);
	m_pScrollBarUpTr->Move(vMovePos);
	m_pScrollBarDownTr->Move(vMovePos);

	for (int i = 0; i < m_pItemObjects.size(); ++i)
	{
		m_pItemObjects[i]->pEmptyTr->Move(vMovePos);
		m_pItemObjects[i]->pItemTr->Move(vMovePos);
		m_pItemObjects[i]->pItemTensTr->Move(vMovePos);
		m_pItemObjects[i]->pItemUnitsTr->Move(vMovePos);
	}

	for (int i = 0; i < m_vecMoney.size(); ++i)
	{
		m_vecMoney[i]->pTr->Move(vMovePos);
	}

	m_vPrevMousePos = m_vCurMousePos;
}

// y축 - 48.25
void Inventory::InventoryRaiseItem(float _fTime)
{
	++m_iScrollCur;
	if (m_iScrollCur > m_iScrollMax)
	{
		m_iScrollCur = m_iScrollMax;
		return;
	}

	Vector3 vMovePos = Vector3(0.0f, +48.25, 0.0f);
	m_pScrollbarTr->Move(vMovePos);

	vMovePos.y = -60.5f;
	for (int i = 0; i < m_pItemObjects.size(); ++i)
	{
		m_pItemObjects[i]->pEmptyTr->Move(vMovePos);
		m_pItemObjects[i]->pItemTr->Move(vMovePos);
		m_pItemObjects[i]->pItemTensTr->Move(vMovePos);
		m_pItemObjects[i]->pItemUnitsTr->Move(vMovePos);
	}

	InventoryCutItem();
}

void Inventory::InventoryLiftItem(float _fTime)
{
	--m_iScrollCur;
	if (m_iScrollCur < m_iScrollMin)
	{
		m_iScrollCur = m_iScrollMin;
		return;
	}

	Vector3 vMovePos = Vector3(0.0f, -48.25, 0.0f);
	m_pScrollbarTr->Move(vMovePos);
	vMovePos.y = +60.5f;
	for (int i = 0; i < m_pItemObjects.size(); ++i)
	{
		m_pItemObjects[i]->pEmptyTr->Move(vMovePos);
		m_pItemObjects[i]->pItemTr->Move(vMovePos);
		m_pItemObjects[i]->pItemTensTr->Move(vMovePos);
		m_pItemObjects[i]->pItemUnitsTr->Move(vMovePos);
	}
	InventoryCutItem();
}

void Inventory::InventoryBarPrevPos(float _fTime)
{
	m_vPrevMousePos = GET_SINGLETON(InputManager)->GetUIMouseViewportPos();
}

void Inventory::InventoryBarMove(float _fTime)
{
	// 바 이동하기 -> 근접하면 바로 이동시키기 (오차범위 10.0f)
	m_vCurMousePos = GET_SINGLETON(InputManager)->GetUIMouseViewportPos();
	Vector3 vMovePos = Vector3(0.0f, m_vCurMousePos.y - m_vPrevMousePos.y, 0.0f);
	vMovePos = vMovePos * 1.5f * _fTime;

	// 세로  52.0f ~ 245.0f , 48.25                 /    20.0 ~ 213,     20.0f -> 68.25 -> 116.5 -> 
	// _RESOLUTION.iHeight * 0.5f + 32.0f
	// _RESOLUTION.iWidth * 0.5f + 556.0f, _RESOLUTION.iHeight * 0.5f + 245.0f
	// 여기서 0, 1, 2, 3

	// 기준이 될 바 위치
	int iScroll = -1;
	float fScrollYPosLevel = m_pScrollBarBGTr->GetWorldPosition().y + 20.0f + (48.25f * (float)m_iScrollCur);
	float fYPosUp = fScrollYPosLevel + 5.0f;
	float fYPosDown = fScrollYPosLevel - 5.0f;

	float fCurBarYPos = m_pScrollbarTr->GetWorldPosition().y + vMovePos.y;

	switch (m_iScrollCur)
	{
	case 0:
		if (fYPosUp < fCurBarYPos)
		{
			iScroll = m_iScrollCur + 1;
		}
		else if (fYPosDown > fCurBarYPos)
		{
			// 이상태는 그대로 멈춰있어야함 
			iScroll = m_iScrollCur;
		}
		break;
	case 4:
		if (fYPosUp < fCurBarYPos)
		{
			// 이상태는 그대로 멈춰있어야함 
			iScroll = m_iScrollCur;
		}
		else if (fYPosDown > fCurBarYPos)
		{
			// 한칸 내려가야됨
			iScroll = m_iScrollCur -1;
		}
		break;
	default:
		if (fYPosUp < fCurBarYPos)
		{
			iScroll = m_iScrollCur + 1;
		}
		else if (fYPosDown > fCurBarYPos)
		{
			// 한칸 내려야함
			iScroll = m_iScrollCur - 1;
		}
		break;
	}

	if (iScroll != -1)
	{
		// 보정단계
		if (iScroll > m_iScrollCur)
		{
			InventoryRaiseItem(_fTime);
		}
		else if(iScroll < m_iScrollCur)
		{
			InventoryLiftItem(_fTime);
		}
		else
		{
			m_iScrollCur = iScroll;
		}
		float fYPos = m_pScrollBarBGTr->GetWorldPosition().y + 20.0f + (48.25f * (float)m_iScrollCur);
		vMovePos = m_pScrollbarTr->GetWorldPosition();
		vMovePos.y = fYPos;
		m_pScrollbarTr->SetWorldPosition(vMovePos);
	}
	else
	{
		m_pScrollbarTr->Move(vMovePos);
	}
	
	m_vPrevBarPos = m_vCurMousePos;
}

void Inventory::InventoryBarMoveClickUp(float _fTime)
{
	float fYPos = m_pScrollBarBGTr->GetWorldPosition().y + 20.0f + (48.25f * (float)m_iScrollCur);
	Vector3 vMovePos = m_pScrollbarTr->GetWorldPosition();
	vMovePos.y = fYPos;
	m_pScrollbarTr->SetWorldPosition(vMovePos);
}

void Inventory::InventoryScrollMoveMouseWheel(float _fTime)
{

	short sWheel = GET_SINGLETON(InputManager)->GetWheelDir();		// 마우스 휠 방향

																	// 0이 아닌 경우는 마우스 휠을 움직이였다는 소리
	if (0 != sWheel)
	{
		if (sWheel > 0)
		{
			InventoryRaiseItem(_fTime);
		}
		else
		{
			InventoryLiftItem(_fTime);
		}
	}
}


void Inventory::InventoryCutItem()
{

	if (false == m_bInventoryEnable)
		return;

	// 안보여야 되는 애들 Eanble = fasle;
	// m_pScrollBarUpTr -  위버튼, m_pScrollBarDownTr - 아래 버튼
	Vector3 vScrollBarUpPos = m_pScrollBarUpTr->GetWorldPosition();
	Vector3 vScrollBarDownPos = m_pScrollBarDownTr->GetWorldPosition();

	for (int i = 0; i < m_pItemObjects.size(); ++i)
	{
		Vector3 vCurItemPos = m_pItemObjects[i]->pEmptyTr->GetWorldPosition();

		if (vScrollBarUpPos.y < vCurItemPos.y || vScrollBarDownPos.y > vCurItemPos.y)
		{
			m_pItemObjects[i]->pEmptyObj->Enable(false);
			m_pItemObjects[i]->pItemObj->Enable(false);
			m_pItemObjects[i]->pItemTensCountObject->Enable(false);
			m_pItemObjects[i]->pItemUnitsCountObject->Enable(false);
		}
		else
		{
			m_pItemObjects[i]->pEmptyObj->Enable(true);
			m_pItemObjects[i]->pItemObj->Enable(true);
			m_pItemObjects[i]->pItemTensCountObject->Enable(true);
			m_pItemObjects[i]->pItemUnitsCountObject->Enable(true);

			switch (m_pItemObjects[i]->eItemType)
			{
			case ITEM_TYPE::POTION_HP:
			case ITEM_TYPE::POTION_MP:
				if (m_pItemObjects[i]->iCount > 0)
				{
					UpdateItemCount(i);
				}
				break;
			default:
				m_pItemObjects[i]->pItemTensCountImage->SetTexture("ItemCountNum_NONE", TEXT("ItemCountNum_NONE.png"), PATH_UI_NUMBER);
				m_pItemObjects[i]->pItemUnitsCountImage->SetTexture("ItemCountNum_NONE", TEXT("ItemCountNum_NONE.png"), PATH_UI_NUMBER);
				break;
			}
		}
	}
}


// 금액 계산하는거 하는중이여씀 ㅇㅇ
bool Inventory::CalculateMoney(int _Value)
{
	int iCalMoney = m_iMoney;
	int iPowN = 1;

	int iMaxValue = (int)(pow(10, CIPHER_VALUE));
	iCalMoney += _Value;

	if (iCalMoney < 0)
	{
		return false;
	}
	else if (iCalMoney >= iMaxValue)
	{
		iCalMoney = (iMaxValue - 1);
	}
	m_iMoney = iCalMoney;

	// 각 자리수에 맞는 번호 넣기
	for (int i = 0; i < CIPHER_VALUE; ++i)
	{
		iPowN = (int)(pow(10, (CIPHER_VALUE-(1+i))));

		int iMoneyNumber = iCalMoney / iPowN;
		if (iCalMoney < iPowN && m_iMoney < iPowN)
		{
			m_vecMoney[CIPHER_VALUE - (1 + i)]->pNumberImage->SetTexture("ItemCountNum_None", TEXT("ItemCountNum_NONE.png"), PATH_UI_NUMBER);
			continue;
		}
		iCalMoney -= (iMoneyNumber * iPowN);

		std::string strTexName = "ItemCountNum_";
		std::string strNumber = std::to_string(iMoneyNumber);
		strTexName += strNumber + ".png";
		m_vecMoney[CIPHER_VALUE - (1 + i)]->pNumberImage->SetTexture(strTexName, StringToLPWSTR(strTexName).c_str(), PATH_UI_NUMBER);
	}
	return true;
}

bool Inventory::BuyItem(ITEM_TYPE _eItemType, int _iCount, int _Value)
{
	// 가격 계산 후 Add해준다
	int iPrice = _iCount * _Value;
	if (true == CalculateMoney(-iPrice))
	{
		AddItem(_eItemType, _iCount);
		return true;
	}
	return false;
}

bool Inventory::AddItem(ITEM_TYPE _eItemType, int _iCount)
{
	// 일단 추가할 수 있는 아이템인지,
	// 추가가 가능하다면 가장 빠른칸에 넣어준다.
	for (int i = 0; i < m_pItemObjects.size(); ++i)
	{
		if (false == m_pItemObjects[i]->bItem)
		{
			switch (_eItemType)
			{
			case POTION_HP:
				m_pItemObjects[i]->pItemButton->SetStateTexture(BS_NORMAL, "Item_PotionHP", TEXT("potion10_Tex.tga"), PATH_UI_ITEM);
				m_pItemObjects[i]->pItemButton->SetStateTexture(BS_MOUSEON, "Item_PotionHP", TEXT("potion10_Tex.tga"), PATH_UI_ITEM);
				m_pItemObjects[i]->pItemButton->SetStateTexture(BS_CLICK, "Item_PotionHP", TEXT("potion10_Tex.tga"), PATH_UI_ITEM);
				m_pItemObjects[i]->pItemButton->SetStateTexture(BS_PUSH, "Item_PotionHP", TEXT("potion10_Tex.tga"), PATH_UI_ITEM);
				m_pItemObjects[i]->eItemType = ITEM_TYPE::POTION_HP;
				m_pItemObjects[i]->bItem = true;
				m_pItemObjects[i]->iValue = POTION_HP_VALUE;
				m_pItemObjects[i]->strItemName = "PotionHP";
				break;
			case POTION_MP:
				m_pItemObjects[i]->pItemButton->SetStateTexture(BS_NORMAL, "Item_PotionMP", TEXT("potion9_Tex.tga"), PATH_UI_ITEM);
				m_pItemObjects[i]->pItemButton->SetStateTexture(BS_MOUSEON, "Item_PotionMP", TEXT("potion9_Tex.tga"), PATH_UI_ITEM);
				m_pItemObjects[i]->pItemButton->SetStateTexture(BS_CLICK, "Item_PotionMP", TEXT("potion9_Tex.tga"), PATH_UI_ITEM);
				m_pItemObjects[i]->pItemButton->SetStateTexture(BS_PUSH, "Item_PotionMP", TEXT("potion9_Tex.tga"), PATH_UI_ITEM);
				m_pItemObjects[i]->eItemType = ITEM_TYPE::POTION_MP;
				m_pItemObjects[i]->bItem = true;
				m_pItemObjects[i]->iValue = POTION_MP_VALUE;
				m_pItemObjects[i]->strItemName = "PotionMP";
				break;
			case ITEM_WEAPON:
				m_pItemObjects[i]->pItemButton->SetStateTexture(BS_NORMAL, "Item_Weapon2", TEXT("AnotherWeapon.png"), PATH_UI_INVENTORY);
				m_pItemObjects[i]->pItemButton->SetStateTexture(BS_MOUSEON, "Item_Weapon2", TEXT("AnotherWeapon.png"), PATH_UI_INVENTORY);
				m_pItemObjects[i]->pItemButton->SetStateTexture(BS_CLICK, "Item_Weapon2", TEXT("AnotherWeapon.png"), PATH_UI_INVENTORY);
				m_pItemObjects[i]->pItemButton->SetStateTexture(BS_PUSH, "Item_Weapon2", TEXT("AnotherWeapon.png"), PATH_UI_INVENTORY);
				m_pItemObjects[i]->eItemType = ITEM_TYPE::ITEM_WEAPON;
				m_pItemObjects[i]->bItem = true;
				m_pItemObjects[i]->iValue = POTION_HP_VALUE;
				m_pItemObjects[i]->strItemName = "Weapon2";
				break;
			default:
				return false;
			}
			m_pItemObjects[i]->iCount = _iCount;
			UpdateItemCount(i);
			return true;
		}
	}

	return false;
}

void Inventory::DeleteItem(int _Index)
{
	if (_Index >= m_pItemObjects.size() && _Index < 0)
	{
		return;
	}
	ClearItem(_Index);
}

void Inventory::DeleteClickedItem()
{
	if (nullptr == m_pClickInvenItem || false == m_pClickInvenItem->bItem)
		return;

	int Num = m_pClickInvenItem->iIndex;
	ClearItem(Num);
}

void Inventory::ClearItem(int _Index)
{
	m_pItemObjects[_Index]->pItemButton->SetStateTexture(BS_NORMAL, "InvenEdgeImage_Normal", TEXT("Inventory_MouseON_Empty.png"), PATH_UI_INVENTORY);
	m_pItemObjects[_Index]->pItemButton->SetStateTexture(BS_MOUSEON, "InvenEdgeImage_MouseON", TEXT("Inventory_MouseON.png"), PATH_UI_INVENTORY);
	m_pItemObjects[_Index]->pItemButton->SetStateTexture(BS_CLICK, "InvenEdgeImage_Empty", TEXT("Inventory_MouseON.png"), PATH_UI_INVENTORY);
	m_pItemObjects[_Index]->pItemButton->SetStateColor(BS_CLICK, Vector4(0.6f, 0.6f, 0.6f, 1.0f));

	m_pItemObjects[_Index]->bItem = false;
	m_pItemObjects[_Index]->strItemName = "";
	m_pItemObjects[_Index]->eItemType = ITEM_TYPE::ITEM_NONE;
	m_pItemObjects[_Index]->iCount = 0;
	m_pItemObjects[_Index]->iValue = 0;

	m_pItemObjects[_Index]->pItemTensCountImage->SetTexture("ItemCountNum_NONE", TEXT("ItemCountNum_NONE.png"), PATH_UI_NUMBER);
	m_pItemObjects[_Index]->pItemUnitsCountImage->SetTexture("ItemCountNum_NONE", TEXT("ItemCountNum_NONE.png"), PATH_UI_NUMBER);
}

void Inventory::UpdateItemCount(int _Index)
{
	std::string ImageName = "ItemCountNum_";
	int iTensNum = (int)(m_pItemObjects[_Index]->iCount * 0.1);
	int iUnitsNum = m_pItemObjects[_Index]->iCount - (10 * iTensNum);

	if (iTensNum > 0)
	{
		std::string strNum = IntToString(iTensNum);
		ImageName += strNum;
		std::string strImageTexName = ImageName;
		ImageName += ".png";
		std::wstring wstrImageName = StringToLPWSTR(ImageName);
		m_pItemObjects[_Index]->pItemTensCountImage->SetTexture(strImageTexName, wstrImageName.c_str(), PATH_UI_NUMBER);

		// 1의 자리는 무조건 표시
		ImageName = "ItemCountNum_";
		strNum = IntToString(iUnitsNum);
		ImageName += strNum;
		strImageTexName = ImageName;
		ImageName += ".png";
		wstrImageName = StringToLPWSTR(ImageName);
		m_pItemObjects[_Index]->pItemUnitsCountImage->SetTexture(strImageTexName, wstrImageName.c_str(), PATH_UI_NUMBER);
	}
	else
	{
		m_pItemObjects[_Index]->pItemTensCountImage->SetTexture("ItemCountNum_NONE", TEXT("ItemCountNum_NONE.png"), PATH_UI_NUMBER);

		// 여기선 1의자리 여부 표시 0일경우 X, 0보다 커야함
		if (iUnitsNum > 0)
		{
			std::string strNum = IntToString(iUnitsNum);
			ImageName += strNum;
			std::string strImageTexName = ImageName;
			ImageName += ".png";
			std::wstring wstrImageName = StringToLPWSTR(ImageName);
			m_pItemObjects[_Index]->pItemUnitsCountImage->SetTexture(strImageTexName, wstrImageName.c_str(), PATH_UI_NUMBER);
		}
		else
		{
			m_pItemObjects[_Index]->pItemUnitsCountImage->SetTexture("ItemCountNum_NONE", TEXT("ItemCountNum_NONE.png"), PATH_UI_NUMBER);
		}
	}
}

void Inventory::SelectItem(float _fTime)
{
	m_pEffectSound->Stop();
	m_pEffectSound->Play();
	if (nullptr == m_pClickInvenItem)
	{
		if (nullptr == m_pMouseItemImageScript)
		{
			return;
		}

		for (int i = 0; i < m_pItemObjects.size(); ++i)
		{
			if (BS_MOUSEON == m_pItemObjects[i]->pItemButton->GetButtonState())
			{
				m_pClickInvenItem = m_pItemObjects[i];

				//무기인 경우엔 바로 장착해준다.
				if (m_pClickInvenItem->eItemType == ITEM_TYPE::ITEM_WEAPON)
				{
					if (nullptr != pPlayerWeapon)
					{
						pPlayerWeapon->ChangeWeapon("PC_Weapons_08");
					}
					m_pClickInvenItem->pItemButton->SetStateTexture(BS_NORMAL, "Item_NONE", TEXT("Item_NONE.png"), PATH_UI_ITEM);
					m_pClickInvenItem->pItemButton->SetStateTexture(BS_MOUSEON, "Item_NONE", TEXT("Item_NONE.png"), PATH_UI_ITEM);
					m_pClickInvenItem->pItemButton->SetStateTexture(BS_CLICK, "Item_NONE", TEXT("Item_NONE.png"), PATH_UI_ITEM);
					m_pClickInvenItem->pItemButton->SetStateTexture(BS_PUSH, "Item_NONE", TEXT("Item_NONE.png"), PATH_UI_ITEM);
					m_pClickInvenItem->bItem = false;
					m_pClickInvenItem->iCount = 0;
					m_pClickInvenItem->iValue = 0;
					m_pClickInvenItem->eItemType = ITEM_TYPE::ITEM_NONE;
					UpdateItemCount(m_pClickInvenItem->iIndex);
					m_pClickInvenItem = nullptr;
					return;
				}
			}
		}

		if (nullptr == m_pClickInvenItem)
		{
			return;
		}

		
		// 퀵슬롯을 클릭했는지 검사한다.
		if (true == m_pMouseItemImageScript->GetbClickQuickslot())
		{
			// 퀵슬롯을 먼저 클릭했다면
			// 퀵슬롯 아이템이 스킬인지 소비템인지 판단한다
			if (SKILL_TYPE::SKILL_NONE == m_pMouseItemImageScript->GetClickSkillKind())
			{
				// 소비템인 경우
				// 현재 클릭한 애가 같은 아이템인지 다른아이템인지 비교한다.
				// 다른 아이템일 경우에 장비템인 경우엔 따로 처리
				if (m_pClickInvenItem->eItemType > ITEM_POTION_MAX)
				{
					// 현재 클릭한 애가 소비템이 아닌 경우
					m_pMouseItemImageScript->SetbClickQuickslot(false);
					m_pMouseItemImageScript->UpdateClickSkillImage(SKILL_TYPE::SKILL_NONE, 0, 0.0f, 0.0f);
					m_pClickInvenItem = nullptr;
					return;
				}
				else
				{
					// 현재 클릭한 것이 소비템인 경우
					if (m_pClickInvenItem->eItemType == m_pMouseItemImageScript->GetClickItemKind())
					{
						// 같은 소비템인 경우 (합산), 99 개 초과시 나눔
						int AddCount = m_pMouseItemImageScript->GetClickItemCount() + m_pClickInvenItem->iCount;

						if (AddCount > 99)
						{
							// 초과
							m_pClickInvenItem->iCount = 99;
							UpdateItemCount(m_pClickInvenItem->iIndex);

							AddCount -= 99;
							m_pMouseItemImageScript->UpdateClickItemImage(m_pClickInvenItem->eItemType, AddCount, m_pMouseItemImageScript->GetIndex());
							m_pMouseItemImageScript->UpdateQuickSlot();
						}
						else
						{
							// 합산
							m_pClickInvenItem->iCount = AddCount;
							UpdateItemCount(m_pClickInvenItem->iIndex);
						}

						m_pMouseItemImageScript->SetbClickQuickslot(false);
						m_pMouseItemImageScript->UpdateClickSkillImage(SKILL_TYPE::SKILL_NONE, 0, 0.0f, 0.0f);
						m_pMouseItemImageScript->UpdateQuickSlot();
						m_pClickInvenItem = nullptr;

						return;
					}
					else
					{
						// 빈공간인 경우
						if (m_pClickInvenItem->bItem == false)
						{
							UpdateInventorySlot(m_pMouseItemImageScript->GetClickItemKind(), m_pMouseItemImageScript->GetClickItemCount(), m_pClickInvenItem->iIndex);

							m_pMouseItemImageScript->SetPrevItemType(m_pMouseItemImageScript->GetClickItemKind());
							m_pMouseItemImageScript->UpdateClickItemImage(ITEM_TYPE::ITEM_NONE, 0, m_pMouseItemImageScript->GetIndex());
							m_pMouseItemImageScript->UpdateQuickSlot();
							m_pClickInvenItem = nullptr;
							return;
						}
						else if(m_pMouseItemImageScript->GetClickItemKind() != ITEM_TYPE::ITEM_NONE)
						{
							// 다른 소비템인 경우엔 스왑한다.
							ITEM_TYPE eTempItemType = m_pClickInvenItem->eItemType;
							int iTempCount = m_pClickInvenItem->iCount;

							UpdateInventorySlot(m_pMouseItemImageScript->GetClickItemKind(), m_pMouseItemImageScript->GetClickItemCount(), m_pClickInvenItem->iIndex);

							m_pMouseItemImageScript->SetPrevItemType(m_pMouseItemImageScript->GetClickItemKind());
							m_pMouseItemImageScript->UpdateClickItemImage(eTempItemType, iTempCount, m_pMouseItemImageScript->GetIndex());
							m_pMouseItemImageScript->UpdateQuickSlot();
							m_pMouseItemImageScript->UpdateClickItemImage(ITEM_TYPE::ITEM_NONE, 0, 0);
							m_pClickInvenItem = nullptr;
							return;
						}
					}
				}
			}
			else
			{
				// 스킬인 경우 ( 스킬인 경우는 인벤토리에 넣을 수 없다. )
				m_pMouseItemImageScript->SetQuickSlotClickClear();
				m_pMouseItemImageScript->UpdateClickSkillImage(SKILL_TYPE::SKILL_NONE, 0, 0.0f, 0.0f);
				m_pClickInvenItem = nullptr;
				return;
			}
		}

		for (int i = 0; i < m_pItemObjects.size(); ++i)
		{
			if (BS_MOUSEON == m_pItemObjects[i]->pItemButton->GetButtonState() && true == m_pItemObjects[i]->bItem)
			{
				m_pClickInvenItem = m_pItemObjects[i];
				UpdateClickItemImage();
			}
		}
		return;
	}
	else
	{
		// 이미 선택된 인벤토리 아이템이 있다면
		// 위치를 바꿔주거나 같은 소비템일 경우는 합쳐준다.
		InventoryType* NewClickItem = nullptr;

		for (int i = 0; i < m_pItemObjects.size(); ++i)
		{
			if (BS_MOUSEON == m_pItemObjects[i]->pItemButton->GetButtonState())
			{
				NewClickItem = m_pItemObjects[i];
			}
		}

		if (nullptr == NewClickItem || NewClickItem->iIndex == m_pClickInvenItem->iIndex)
		{
			m_pClickInvenItem = nullptr;
			UpdateClickItemImage();
			m_pMouseItemImageScript->UpdateClickItemImage(ITEM_TYPE::ITEM_NONE, 0, m_pMouseItemImageScript->GetIndex());
			return;
		}

		if (true == NewClickItem->bItem)
		{
			// 아이템이 있는 경우
			// 아이템이 같은경우
			if (NewClickItem->eItemType == m_pClickInvenItem->eItemType)
			{
				// 소비아이템일 경우만, 합쳐준다.
				if (m_pClickInvenItem->eItemType < ITEM_TYPE::ITEM_POTION_MAX)
				{
					// 합치고
					int iNewClickCount = NewClickItem->iCount;
					int iPrevClickCount = m_pClickInvenItem->iCount;

					int iAddCount = iNewClickCount + iPrevClickCount;

					if (iAddCount < 100)
					{
						NewClickItem->iCount += m_pClickInvenItem->iCount;
						UpdateItemCount(NewClickItem->iIndex);

						// 기존꺼는 정보 제거
						m_pClickInvenItem->pItemButton->SetStateTexture(BS_NORMAL, "Item_NONE", TEXT("Item_NONE.png"), PATH_UI_ITEM);
						m_pClickInvenItem->pItemButton->SetStateTexture(BS_MOUSEON, "Item_NONE", TEXT("Item_NONE.png"), PATH_UI_ITEM);
						m_pClickInvenItem->pItemButton->SetStateTexture(BS_CLICK, "Item_NONE", TEXT("Item_NONE.png"), PATH_UI_ITEM);
						m_pClickInvenItem->pItemButton->SetStateTexture(BS_PUSH, "Item_NONE", TEXT("Item_NONE.png"), PATH_UI_ITEM);
						m_pClickInvenItem->bItem = false;
						m_pClickInvenItem->iCount = 0;
						m_pClickInvenItem->iValue = 0;
						m_pClickInvenItem->eItemType = ITEM_TYPE::ITEM_NONE;
						UpdateItemCount(m_pClickInvenItem->iIndex);
						m_pClickInvenItem = nullptr;
						UpdateClickItemImage();
						m_pMouseItemImageScript->UpdateClickItemImage(ITEM_TYPE::ITEM_NONE, 0, m_pMouseItemImageScript->GetIndex());
						return;
					}
					else
					{
						if (99 == iPrevClickCount)
						{
							int tempCount = iPrevClickCount;
							iPrevClickCount = iNewClickCount;
							iNewClickCount = tempCount;
						}
						else
						{
							iNewClickCount = 99;
							iPrevClickCount = iAddCount - iNewClickCount;
						}

						NewClickItem->iCount = iNewClickCount;
						m_pClickInvenItem->iCount = iPrevClickCount;

						UpdateItemCount(NewClickItem->iIndex);
						UpdateItemCount(m_pClickInvenItem->iIndex);

						m_pClickInvenItem = nullptr;
						UpdateClickItemImage();
						m_pMouseItemImageScript->UpdateClickItemImage(ITEM_TYPE::ITEM_NONE, 0, m_pMouseItemImageScript->GetIndex());
						return;
					}
				}
			}
		}

		// 아이템이 같거나 빈 공간의 경우 -> 위치 바꾸기 ( 얜 나중에 시간있을 때 바꾸자)
		GameObject* pTempObj = m_pClickInvenItem->pItemObj;
		Transform* pTempTr = m_pClickInvenItem->pItemTr;
		UIButton* pTempButton = m_pClickInvenItem->pItemButton;
		ITEM_TYPE pTempType = m_pClickInvenItem->eItemType;
		int iTempCount = m_pClickInvenItem->iCount;
		bool bItem = m_pClickInvenItem->bItem;
		Vector3 pTempPos = m_pClickInvenItem->pItemTr->GetWorldPosition();

		m_pClickInvenItem->pItemTr->SetWorldPosition(NewClickItem->pItemTr->GetWorldPosition());
		NewClickItem->pItemTr->SetWorldPosition(pTempPos);

		m_pItemObjects[m_pClickInvenItem->iIndex]->pItemObj = m_pItemObjects[NewClickItem->iIndex]->pItemObj;
		m_pItemObjects[m_pClickInvenItem->iIndex]->pItemButton = m_pItemObjects[NewClickItem->iIndex]->pItemButton;
		m_pItemObjects[m_pClickInvenItem->iIndex]->pItemTr = m_pItemObjects[NewClickItem->iIndex]->pItemTr;
		m_pItemObjects[m_pClickInvenItem->iIndex]->eItemType = m_pItemObjects[NewClickItem->iIndex]->eItemType;
		m_pItemObjects[m_pClickInvenItem->iIndex]->iCount = m_pItemObjects[NewClickItem->iIndex]->iCount;
		m_pItemObjects[m_pClickInvenItem->iIndex]->bItem = m_pItemObjects[NewClickItem->iIndex]->bItem;

		UpdateItemCount(m_pClickInvenItem->iIndex);

		m_pItemObjects[NewClickItem->iIndex]->pItemObj = pTempObj;
		m_pItemObjects[NewClickItem->iIndex]->pItemButton = pTempButton;
		m_pItemObjects[NewClickItem->iIndex]->pItemTr = pTempTr;
		m_pItemObjects[NewClickItem->iIndex]->eItemType = pTempType;
		m_pItemObjects[NewClickItem->iIndex]->iCount = iTempCount;
		m_pItemObjects[NewClickItem->iIndex]->bItem = bItem;

		UpdateItemCount(NewClickItem->iIndex);

	}
	m_pClickInvenItem = nullptr;
	UpdateClickItemImage();
}

void Inventory::UseItem(float _fTime)
{
	// 오른쪽마우스 클릭한 아이템 사용하기
	// 무기인 경우 아이템 장착
	// 포션인 경우엔  포션 사용
	int a = 0;
}

void Inventory::SetMouseItemSkillScript(MouseItemSkill * _Script)
{
	m_pMouseItemImageScript = _Script;
}

void Inventory::UpdateClickItemImage()
{
	if (nullptr == m_pMouseItemImageScript)
	{
		return;
	}

	if (nullptr == m_pClickInvenItem)
	{
		m_pMouseItemImageScript->UpdateClickItemImage(ITEM_TYPE::ITEM_NONE, 0, -1);
		m_pMouseItemImageScript->SetbClickInventory(false);
		return;
	}

	switch (m_pClickInvenItem->eItemType)
	{
	case ITEM_TYPE::POTION_HP:
	case ITEM_TYPE::POTION_MP:
		m_pMouseItemImageScript->UpdateClickItemImage(m_pClickInvenItem->eItemType, m_pClickInvenItem->iCount, m_pClickInvenItem->iIndex);
		m_pMouseItemImageScript->SetbClickInventory(true);
		break;
	default:
		m_pMouseItemImageScript->UpdateClickItemImage(ITEM_TYPE::ITEM_NONE, 0, -1);
		m_pMouseItemImageScript->SetbClickInventory(false);
		return;
	}
}

void Inventory::UpdateInventorySlot(ITEM_TYPE _eItemType, int _iCount, int _Index)
{
	if (_Index >= m_pItemObjects.size())
	{
		return;
	}

	switch (_eItemType)
	{
	case POTION_HP:
		m_pItemObjects[_Index]->pItemButton->SetStateTexture(BS_NORMAL, "Item_PotionHP", TEXT("potion10_Tex.tga"), PATH_UI_ITEM);
		m_pItemObjects[_Index]->pItemButton->SetStateTexture(BS_MOUSEON, "Item_PotionHP", TEXT("potion10_Tex.tga"), PATH_UI_ITEM);
		m_pItemObjects[_Index]->pItemButton->SetStateTexture(BS_CLICK, "Item_PotionHP", TEXT("potion10_Tex.tga"), PATH_UI_ITEM);
		m_pItemObjects[_Index]->pItemButton->SetStateTexture(BS_PUSH, "Item_PotionHP", TEXT("potion10_Tex.tga"), PATH_UI_ITEM);
		m_pItemObjects[_Index]->iValue = POTION_HP_VALUE;
		break;
	case POTION_MP:
		m_pItemObjects[_Index]->pItemButton->SetStateTexture(BS_NORMAL, "Item_PotionMP", TEXT("potion9_Tex.tga"), PATH_UI_ITEM);
		m_pItemObjects[_Index]->pItemButton->SetStateTexture(BS_MOUSEON, "Item_PotionMP", TEXT("potion9_Tex.tga"), PATH_UI_ITEM);
		m_pItemObjects[_Index]->pItemButton->SetStateTexture(BS_CLICK, "Item_PotionMP", TEXT("potion9_Tex.tga"), PATH_UI_ITEM);
		m_pItemObjects[_Index]->pItemButton->SetStateTexture(BS_PUSH, "Item_PotionMP", TEXT("potion9_Tex.tga"), PATH_UI_ITEM);
		m_pItemObjects[_Index]->iValue = POTION_MP_VALUE;
		break;
	case ITEM_EQUIP:
		break;
	default:
		m_pItemObjects[_Index]->pItemButton->SetStateTexture(BS_NORMAL, "Item_NONE", TEXT("Item_NONE.png"), PATH_UI_ITEM);
		m_pItemObjects[_Index]->pItemButton->SetStateTexture(BS_MOUSEON, "Item_NONE", TEXT("Item_NONE.png"), PATH_UI_ITEM);
		m_pItemObjects[_Index]->pItemButton->SetStateTexture(BS_CLICK, "Item_NONE", TEXT("Item_NONE.png"), PATH_UI_ITEM);
		m_pItemObjects[_Index]->pItemButton->SetStateTexture(BS_PUSH, "Item_NONE", TEXT("Item_NONE.png"), PATH_UI_ITEM);
		m_pItemObjects[_Index]->iCount = 0;
		m_pItemObjects[_Index]->iValue = 0;
		m_pItemObjects[_Index]->eItemType = ITEM_TYPE::ITEM_NONE;
		m_pItemObjects[_Index]->bItem = false;
		UpdateItemCount(_Index);
		m_pClickInvenItem = nullptr;
		return;
	}

	m_pItemObjects[_Index]->bItem = true;
	m_pItemObjects[_Index]->eItemType = _eItemType;
	m_pItemObjects[_Index]->iCount = _iCount;
	UpdateItemCount(_Index);
	m_pClickInvenItem = nullptr;
}

void Inventory::SetClickItemClaer()
{
	m_pClickInvenItem = nullptr;
}
