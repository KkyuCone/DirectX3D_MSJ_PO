#include "MouseItemSkill.h"
#include "Inventory.h"
#include "QuickSlot.h"

#include "Scene\Scene.h"
#include "Scene\Layer.h"
#include "Device.h"
#include "Time.h"
#include "GameObject.h"
#include "InputManager.h"


MouseItemSkill::MouseItemSkill() :
	m_pClickObject(nullptr)
	, m_pClickImage(nullptr)
	, m_pClickTr(nullptr)
	, iItemCount(0)
	, bClickInventory(false)
	, bClickQuickSlot(false)
	, pQuickslot(nullptr)
	, pInventory(nullptr)
{
	fCoolTime = 0.0f;
	fCurCoolTime = 0.0f;
	m_eClickItem = ITEM_TYPE::ITEM_NONE;
	m_eClickSkill = SKILL_TYPE::SKILL_NONE;

	m_ePrevClickItem = ITEM_TYPE::ITEM_NONE;
	m_ePrevClickSkill = SKILL_TYPE::SKILL_NONE;
}

MouseItemSkill::MouseItemSkill(const MouseItemSkill & _MouseItemSkill)
{
	*this = _MouseItemSkill;
	m_pLayer = _MouseItemSkill.m_pLayer;
	m_iReferenceCount = 1;
}


MouseItemSkill::~MouseItemSkill()
{
	SAFE_RELEASE(m_pClickTr);
	SAFE_RELEASE(m_pClickImage);
	SAFE_RELEASE(m_pClickObject);
}

void MouseItemSkill::Start()
{
	// 마우스 따라다니는 이미지
	m_pClickObject = GameObject::CreateObject("InvenMouseImage", m_pLayer);
	m_pClickImage = m_pClickObject->AddComponent<UIBar>("InvenEmptyImage");
	m_pClickImage->SetTexture("Item_NONE", TEXT("Item_NONE.png"), PATH_UI_ITEM);
	m_pClickImage->SetShader(SHADER_UI_ONLY_IMAGE);
	m_pClickTr = m_pClickObject->GetTransform();
	m_pClickTr->SetWorldPosition(0.0f, 0.0f, 0.2f);
	m_pClickTr->SetWorldScale(47.0f, 47.0f, 1.0f);
}

bool MouseItemSkill::Init()
{
	return true;
}

int MouseItemSkill::Input(float _fTime)
{
	return 0;
}

int MouseItemSkill::Update(float _fTime)
{
	UpdateClickImageMove();

	if (nullptr == pQuickslot)
	{
		int a = 0;
	}
	else
	{
		int a = 0;
	}
	return 0;
}

int MouseItemSkill::LateUpdate(float _fTime)
{
	return 0;
}

int MouseItemSkill::Collision(float _fTime)
{
	return 0;
}

int MouseItemSkill::PrevRender(float _fTime)
{
	return 0;
}

int MouseItemSkill::Render(float _fTime)
{
	return 0;
}

MouseItemSkill * MouseItemSkill::Clone() const
{
	return new MouseItemSkill(*this);
}


void MouseItemSkill::UpdateQuickSlot()
{
	if (nullptr == pQuickslot)
	{
		return;
	}

	if (ITEM_NONE == m_ePrevClickItem)
	{
		pQuickslot->UpdateSkillSlot(iIndex, m_eClickSkill, fCoolTime);
	}
	else
	{
		pQuickslot->UpdateItemSlot(iIndex, m_eClickItem, iItemCount);
		pQuickslot->SetClickInfoClear();
	}

	bClickInventory = false;
	bClickQuickSlot = false;
}

void MouseItemSkill::UpdateInventory()
{
	if (nullptr == pInventory)
	{
		return;
	}
	pInventory->UpdateInventorySlot(m_eClickItem, iItemCount, iIndex);
	m_eClickItem = ITEM_TYPE::ITEM_NONE;
	m_eClickSkill = SKILL_TYPE::SKILL_NONE;
	iIndex = -1;
	bClickInventory = false;
	bClickQuickSlot = false;
}

void MouseItemSkill::SetQuickslotScript(QuickSlot * _pScript)
{
	pQuickslot = _pScript;
}

void MouseItemSkill::SetInventoryScript(Inventory * _pScript)
{
	pInventory = _pScript;
}

void MouseItemSkill::UpdateClickImageMove()
{
	Vector2 vMousePos = GET_SINGLETON(InputManager)->GetUIMouseViewportPos();
	m_pClickTr->SetWorldPosition(vMousePos.x - 20.0f, vMousePos.y - 20.0f, 0.2f);
}

void MouseItemSkill::UpdateClickItemImage(ITEM_TYPE _eItemType, int _iCount, int _Index)
{
	switch (_eItemType)
	{
	case ITEM_TYPE::POTION_HP:
		m_pClickImage->SetTexture("Item_PotionHP", TEXT("potion10_Tex.png"), PATH_UI_ITEM);
		break;
	case ITEM_TYPE::POTION_MP:
		m_pClickImage->SetTexture("Item_PotionMP", TEXT("potion9_Tex.png"), PATH_UI_ITEM);
		break;
	default:
		m_pClickImage->SetTexture("Item_NONE", TEXT("Item_NONE.png"), PATH_UI_ITEM);
		m_eClickItem = ITEM_TYPE::ITEM_NONE;
		iItemCount = 0;
		return;
	}
	iIndex = _Index;
	iItemCount = _iCount;
	m_eClickItem = _eItemType;
}

void MouseItemSkill::UpdateClickSkillImage(SKILL_TYPE _eSkillType, int _Index, float _CoolTime, float _CurCoolTime)
{
	switch (_eSkillType)
	{
	case SKILL_01:
		m_pClickImage->SetTexture("Icon_SKill01", TEXT("Cuttingslash_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		break;
	case SKILL_02:
		m_pClickImage->SetTexture("Icon_SKill02", TEXT("Stingerblade_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		break;
	case SKILL_03:
		m_pClickImage->SetTexture("Icon_SKill03", TEXT("Whirlwind_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		break;
	case SKILL_04:
		m_pClickImage->SetTexture("Icon_SKill04", TEXT("advancingcharge_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		break;
	case SKILL_05:
		m_pClickImage->SetTexture("Icon_SKill05", TEXT("Cutting_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		break;
	default:
		iIndex = 0;
		fCoolTime = 0.0f;
		fCurCoolTime = 0.0f;
		m_pClickImage->SetTexture("Icon_SKill_NONE", TEXT("Skill_NONE.png"), PATH_UI_SKILL_TERA_ICON);
		m_eClickSkill = SKILL_TYPE::SKILL_NONE;
		return;
	}
	fCoolTime = _CoolTime;
	fCurCoolTime = _CurCoolTime;
	iIndex = _Index;
	m_eClickSkill = _eSkillType;
}

void MouseItemSkill::SetbClickInventory(bool _Value)
{
	bClickInventory = _Value;
}

void MouseItemSkill::SetbClickQuickslot(bool _Value)
{
	bClickQuickSlot = _Value;
}

void MouseItemSkill::SetClickNone()
{
	m_pClickImage->SetTexture("Icon_SKill_NONE", TEXT("Skill_NONE.png"), PATH_UI_SKILL_TERA_ICON);
	m_eClickSkill = SKILL_TYPE::SKILL_NONE;
}

void MouseItemSkill::SetPrevItemType(ITEM_TYPE _eType)
{
	m_ePrevClickItem = _eType;
	m_ePrevClickSkill = SKILL_TYPE::SKILL_NONE;
}

void MouseItemSkill::SetPrevSkillType(SKILL_TYPE _eSkillType)
{
	m_ePrevClickItem = ITEM_TYPE::ITEM_NONE;
	m_ePrevClickSkill = _eSkillType;
}

void MouseItemSkill::SetInventoryClickClear()
{
	if (nullptr == pInventory)
	{
		return;
	}

	pInventory->SetClickItemClaer();
}

void MouseItemSkill::SetQuickSlotClickClear()
{
	if (nullptr == pQuickslot)
	{
		return;
	}

	pQuickslot->SetClickInfoClear();
}

ITEM_TYPE MouseItemSkill::GetClickItemKind()
{
	return m_eClickItem;
}

int MouseItemSkill::GetClickItemCount()
{
	return iItemCount;
}

SKILL_TYPE MouseItemSkill::GetClickSkillKind()
{
	return m_eClickSkill;
}

bool MouseItemSkill::GetbClickInventory()
{
	return bClickInventory;
}

bool MouseItemSkill::GetbClickQuickslot()
{
	return bClickQuickSlot;
}

int MouseItemSkill::GetIndex()
{
	return iIndex;
}
