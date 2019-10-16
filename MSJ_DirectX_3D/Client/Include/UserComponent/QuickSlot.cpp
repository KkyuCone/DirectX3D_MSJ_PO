#include "QuickSlot.h"
#include "Scene\Scene.h"
#include "Scene\Layer.h"
#include "Device.h"
#include "Time.h"
#include "GameObject.h"
#include "InputManager.h"


QuickSlot::QuickSlot() :
	m_pQuickSlotBGObj(nullptr)
	, m_pQuickSlotBG(nullptr)
	, m_pMouseItemImageScript(nullptr)
	, m_pSelQuickSlot(nullptr)
	, m_pEffectSound(nullptr)
{
}

QuickSlot::QuickSlot(const QuickSlot & _QuickSlot) : Base(_QuickSlot)
{
	*this = _QuickSlot;
	m_pLayer = _QuickSlot.m_pLayer;
	m_iReferenceCount = 1;
}


QuickSlot::~QuickSlot()
{
	SAFE_RELEASE(m_pMouseItemImageScript);

	SAFE_RELEASE(m_pQuickSlotBG);
	SAFE_RELEASE(m_pQuickSlotBGObj);

	for (int i = 0; i < m_vecQuickSlots.size(); ++i)
	{
		SAFE_RELEASE(m_vecQuickSlots[i]->pEmptyQuickSlotButton);
		SAFE_RELEASE(m_vecQuickSlots[i]->pEmptyQuickSlotObject);
		SAFE_RELEASE(m_vecQuickSlots[i]->pQuickSlotTr);
		SAFE_RELEASE(m_vecQuickSlots[i]->pQuickSlotButton);
		SAFE_RELEASE(m_vecQuickSlots[i]->pQuickSlotObject);
		SAFE_RELEASE(m_vecQuickSlots[i]->pQuickSlotNumImage);
		SAFE_RELEASE(m_vecQuickSlots[i]->pQuickSlotNumObject);
		SAFE_RELEASE(m_vecQuickSlots[i]->pItemTensCountImage);
		SAFE_RELEASE(m_vecQuickSlots[i]->pItemTensTr);
		SAFE_RELEASE(m_vecQuickSlots[i]->pItemTensCountObject);
		SAFE_RELEASE(m_vecQuickSlots[i]->pItemUnitsCountImage);
		SAFE_RELEASE(m_vecQuickSlots[i]->pItemUnitsTr);
		SAFE_RELEASE(m_vecQuickSlots[i]->pItemUnitsCountObject);
		SAFE_RELEASE(m_vecQuickSlots[i]->pCoolTimeObject);
		SAFE_RELEASE(m_vecQuickSlots[i]->pCoolTimeTr);
		SAFE_RELEASE(m_vecQuickSlots[i]->pCoolTimeImage);
		SAFE_DELETE(m_vecQuickSlots[i]);
	}

	SAFE_RELEASE(m_pEffectSound);
}

void QuickSlot::Start()
{
}

bool QuickSlot::Init()
{
	m_pQuickSlotBGObj = GameObject::CreateObject("QuickSlot_BG", m_pLayer);

	m_pQuickSlotBG = m_pQuickSlotBGObj->AddComponent<UIBar>("QuickSlot_BG");
	m_pQuickSlotBG->SetTexture("QuickSlot_BG", TEXT("ab2_main_frame.png"), PATH_UI_GAGEBAR);
	m_pQuickSlotBG->SetShader(SHADER_UI_BAR);

	Transform* pQuickBGSlot = m_pQuickSlotBGObj->GetTransform();
	pQuickBGSlot->SetWorldScale(1162.2f, 139.8f, 1.0f);
	pQuickBGSlot->SetWorldPosition(_RESOLUTION.iWidth * 0.5f - 590.0f, _RESOLUTION.iHeight * 0.5f - 528.f, 1.4f);

	Vector3 vQuickSlotBasePos = pQuickBGSlot->GetWorldPosition();
	vQuickSlotBasePos.x += 74.6f;
	vQuickSlotBasePos.y += 37.0f;

	SAFE_RELEASE(m_pQuickSlotBG);
	SAFE_RELEASE(pQuickBGSlot);

	m_vecQuickSlots.resize(10);
	// �� ���Ե� �����ϱ� 1~0������, �������� ���� X , �� 10��
	for (int i = 0; i < 10; ++i)
	{
		// ������ ���� �� �־��ֱ�
		QuickSlotType* pNewQuickSlot = new QuickSlotType;
		m_vecQuickSlots[i] = pNewQuickSlot;

		Vector3 vNewQuickSlotPos = vQuickSlotBasePos;
		vNewQuickSlotPos.x += (76.22f* i);

		// �����Կ� �ʿ��� �ֵ� �����ϱ�
		GameObject* pNewEmptyQuickObj = GameObject::CreateObject("InvenEmpty", m_pLayer);
		UIButton* pNewEmptyQuickImage = pNewEmptyQuickObj->AddComponent<UIButton>("InvenEmptyImage");
		pNewEmptyQuickImage->SetStateTexture(BS_NORMAL, "InvenEmptyImage_NORMAL", TEXT("Inventory_NORMAL.png"), PATH_UI_INVENTORY);
		pNewEmptyQuickImage->SetStateTexture(BS_MOUSEON, "InvenEmptyImage_MOUSEON", TEXT("Inventory_MouseON.png"), PATH_UI_INVENTORY);
		pNewEmptyQuickImage->SetStateTexture(BS_CLICK, "InvenEmptyImage_MOUSEON", TEXT("Inventory_MouseON.png"), PATH_UI_INVENTORY);
		pNewEmptyQuickImage->SetStateColor(BS_CLICK, Vector4(0.6f, 0.6f, 0.6f, 1.0f));
		//pNewEmptyQuickImage->SetCallBackFunc(BS_CLICK, this, &QuickSlot::SelectItem);
		//pInvenEmptyImage->SetShader(SHADER_UI_BAR); SelectItem
		Transform*	pInvenEmptyTr = pNewEmptyQuickObj->GetTransform();
		pInvenEmptyTr->SetWorldPosition(vNewQuickSlotPos.x, vNewQuickSlotPos.y, 0.7f);
		pInvenEmptyTr->SetWorldScale(73.2f, 72.6f, 1.0f);

		pNewQuickSlot->pEmptyQuickSlotObject = pNewEmptyQuickObj;
		pNewQuickSlot->pEmptyQuickSlotButton = pNewEmptyQuickImage;
		SAFE_RELEASE(pInvenEmptyTr);
		//SAFE_RELEASE(pNewEmptyQuickImage);
		//SAFE_RELEASE(pNewEmptyQuickObj);

		// ������ - ��ų, ������ �̹���
		GameObject* pNewSIObject = GameObject::CreateObject("QuickSkillItem", m_pLayer);
		UIButton*	pNewSIImage = pNewSIObject->AddComponent<UIButton>("QuickSkillItemImage");
		pNewSIImage->SetStateTexture(BS_NORMAL, "Icon_SKill_NONE", TEXT("Skill_NONE.png"), PATH_UI_SKILL_TERA_ICON);
		pNewSIImage->SetStateTexture(BS_MOUSEON, "Icon_SKill_NONE", TEXT("Skill_NONE.png"), PATH_UI_SKILL_TERA_ICON);
		pNewSIImage->SetStateTexture(BS_CLICK, "Icon_SKill_NONE", TEXT("Skill_NONE.png"), PATH_UI_SKILL_TERA_ICON);
		pNewSIImage->SetStateTexture(BS_PUSH, "Icon_SKill_NONE", TEXT("Skill_NONE.png"), PATH_UI_SKILL_TERA_ICON);
		pNewSIImage->SetStateColor(BS_CLICK, Vector4(0.6f, 0.6f, 0.6f, 1.0f));
		pNewSIImage->SetCallBackFunc(BS_CLICK, this, &QuickSlot::SelectItem);

		Transform* pNewSITr = pNewSIObject->GetTransform();
		pNewSITr->SetWorldPosition(vNewQuickSlotPos.x + 8.0f, vNewQuickSlotPos.y + 8.0f, 0.6f);
		pNewSITr->SetWorldScale(56.0f, 56.0f, 1.0f);

		pNewQuickSlot->pQuickSlotObject = pNewSIObject;
		pNewQuickSlot->pQuickSlotButton = pNewSIImage;
		pNewQuickSlot->pQuickSlotTr = pNewSITr;

		SAFE_RELEASE(pNewSITr);
		//SAFE_RELEASE(pNewSIImage);
		//SAFE_RELEASE(pNewSIObject);

		// ������ - ��Ÿ�� �̹���SHADER_UI_COOLTIME
		GameObject* pNewCoolTimeObject = GameObject::CreateObject("CoolTimeObject", m_pLayer);
		UIBar*		pNewCoolTimeImage = pNewCoolTimeObject->AddComponent<UIBar>("CoolTimeImage");
		pNewCoolTimeImage->SetTexture("CoolTimeImage", TEXT("CoolTimeImage.png"), PATH_UI_SKILL_ICON);
		pNewCoolTimeImage->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
		pNewCoolTimeImage->SetShader(SHADER_UI_COOLTIME);

		Transform*	pNewCoolTr = pNewCoolTimeObject->GetTransform();
		pNewCoolTr->SetWorldPosition(vNewQuickSlotPos.x + 4.0f, vNewQuickSlotPos.y + 4.0f, 0.5f);
		pNewCoolTr->SetWorldScale(63.0f, 63.0f, 1.0f);

		pNewQuickSlot->pCoolTimeObject = pNewCoolTimeObject;
		pNewQuickSlot->pCoolTimeImage = pNewCoolTimeImage;
		pNewQuickSlot->pCoolTimeTr = pNewCoolTr;

		pNewQuickSlot->pCoolTimeObject->Enable(false);

		SAFE_RELEASE(pNewCoolTr);
		//SAFE_RELEASE(pNewCoolTimeImage);
		//SAFE_RELEASE(pNewCoolTimeObject);

		// ������ ��ȣ ����
		std::string strQuickSlotNum = "QuickSlotNum_";
		int iSlotNum = i;

		if (iSlotNum == 9)
		{
			iSlotNum = 0;
		}
		else
		{
			++iSlotNum;
		}
		pNewQuickSlot->iSlotNumber = iSlotNum;
		std::string strSlotNum = IntToString(iSlotNum);
		strQuickSlotNum += strSlotNum;

		std::string strQuickSlotImageName = strQuickSlotNum;
		strQuickSlotImageName += ".png";

		std::wstring wstrTextureName = StringToLPWSTR(strQuickSlotImageName);

		GameObject* pNewQuickNumObj = GameObject::CreateObject("QuickSlotNum", m_pLayer);
		UIBar* pNewQuickNumImage = pNewQuickNumObj->AddComponent<UIBar>("QuickSlotNum");
		pNewQuickNumImage->SetTexture(strQuickSlotImageName, wstrTextureName.c_str(), PATH_UI_QUICKSLOT);
		pNewQuickNumImage->SetShader(SHADER_UI_BAR);

		vNewQuickSlotPos.x += 6.0f;
		vNewQuickSlotPos.y += 55.0f;
		Transform*	pNewQuickNumTr = pNewQuickNumObj->GetTransform();
		pNewQuickNumTr->SetWorldPosition(vNewQuickSlotPos.x, vNewQuickSlotPos.y, 0.3f);
		pNewQuickNumTr->SetWorldScale(11.0f, 13.0f, 1.0f);

		pNewQuickSlot->pQuickSlotNumObject = pNewQuickNumObj;
		pNewQuickSlot->pQuickSlotNumImage = pNewQuickNumImage;
		SAFE_RELEASE(pNewQuickNumTr);
		//SAFE_RELEASE(pNewQuickNumImage);
		//SAFE_RELEASE(pNewQuickNumObj);

		// ������ ���� ǥ���ϴ� �̹���
		// 10���ڸ�
		GameObject*	pNewItemTensObject = GameObject::CreateObject("ItemTensCount", m_pLayer);
		UIBar*		pNewItemTensImage = pNewItemTensObject->AddComponent<UIBar>("ItemTensCountImage");
		pNewItemTensImage->SetTexture("ItemCountNum_NONE", TEXT("ItemCountNum_NONE.png"), PATH_UI_NUMBER);
		pNewItemTensImage->SetShader(SHADER_UI_ONLY_IMAGE);

		Transform*	pNewItemTensTr = pNewItemTensObject->GetTransform();
		pNewItemTensTr->SetWorldPosition(vNewQuickSlotPos.x + 38.0f, vNewQuickSlotPos.y - 50.0f, 0.4f);
		pNewItemTensTr->SetWorldScale(13.0f, 15.0f, 1.0f);

		pNewQuickSlot->pItemTensCountObject = pNewItemTensObject;
		pNewQuickSlot->pItemTensCountImage = pNewItemTensImage;
		pNewQuickSlot->pItemTensTr = pNewItemTensTr;
		SAFE_RELEASE(pNewItemTensTr);
		//SAFE_RELEASE(pNewItemTensImage);
		//SAFE_RELEASE(pNewItemTensObject);

		// 1�� �ڸ�
		GameObject*	pNewItemUnitsObject = GameObject::CreateObject("ItemTensCount", m_pLayer);
		UIBar*		pNewItemUnitsImage = pNewItemUnitsObject->AddComponent<UIBar>("ItemTensCountImage");
		pNewItemUnitsImage->SetTexture("ItemCountNum_NONE", TEXT("ItemCountNum_NONE.png"), PATH_UI_NUMBER);
		pNewItemUnitsImage->SetShader(SHADER_UI_ONLY_IMAGE);

		Transform*	pNewItemUnitsTr = pNewItemUnitsObject->GetTransform();
		pNewItemUnitsTr->SetWorldPosition(vNewQuickSlotPos.x + 48.0f, vNewQuickSlotPos.y - 50.0f, 0.4f);
		pNewItemUnitsTr->SetWorldScale(13.0f, 15.0f, 1.0f);

		pNewQuickSlot->pItemUnitsCountObject = pNewItemUnitsObject;
		pNewQuickSlot->pItemUnitsCountImage = pNewItemUnitsImage;
		pNewQuickSlot->pItemUnitsTr = pNewItemUnitsTr;
		SAFE_RELEASE(pNewItemUnitsTr);
		//SAFE_RELEASE(pNewItemUnitsImage);
		//SAFE_RELEASE(pNewItemUnitsObject);

		pNewQuickSlot->iCount = 0;
		pNewQuickSlot->iIndex = i;
		pNewQuickSlot->bQuickSlot = false;
	}

	AddSkill(SKILL_TYPE::SKILL_01);
	AddSkill(SKILL_TYPE::SKILL_02);
	AddSkill(SKILL_TYPE::SKILL_03);
	AddSkill(SKILL_TYPE::SKILL_04);
	AddSkill(SKILL_TYPE::SKILL_05);

	AudioSound* pEffectSound = m_pGameObject->AddComponent<AudioSound>("QuickBtClick");
	pEffectSound->SetSound("QuickBtClick", m_pScene, false, "Button-4.mp3", PATH_SOUND_UI);
	m_pEffectSound = pEffectSound;
	SAFE_RELEASE(pEffectSound);
	return true;
}

int QuickSlot::Input(float _fTime)
{
	return 0;
}

int QuickSlot::Update(float _fTime)
{
	return 0;
}

int QuickSlot::LateUpdate(float _fTime)
{
	return 0;
}

int QuickSlot::Collision(float _fTime)
{
	return 0;
}

int QuickSlot::PrevRender(float _fTime)
{
	return 0;
}

int QuickSlot::Render(float _fTime)
{
	return 0;
}

QuickSlot * QuickSlot::Clone() const
{
	return new QuickSlot(*this);
}

void QuickSlot::SetMouseItemSkillScript(MouseItemSkill * _Script)
{
	m_pMouseItemImageScript = _Script;
}

void QuickSlot::SelectItem(float _fTime)
{
	m_pEffectSound->Stop();
	m_pEffectSound->Play();
	if (nullptr == m_pSelQuickSlot)
	{

		// ���� �������� Ŭ���� ���� ���� ��� 
		// �κ��丮�� Ŭ���� ���� �ִ��� üũ����
		if (true == GetPrevInventoryClick())
		{
			for (int i = 0; i < m_vecQuickSlots.size(); ++i)
			{
				if (BS_MOUSEON == m_vecQuickSlots[i]->pQuickSlotButton->GetButtonState())
				{
					m_pSelQuickSlot = m_vecQuickSlots[i];
				}
			}

			// ��ã�� ���� ���� -��-;; ������ �����س��� ��..
			if (nullptr == m_pSelQuickSlot)
			{
				return;
			}

			// ������ �κ��丮�� Ŭ���ߴٸ� ( �� �κ��丮 ������ Ŭ�� �� ������ Ŭ�� )
			// 1. �����Կ� �ش� �κ��丮 ��ǰ�� ����� �� �ִ��� üũ
			// 1-1. ���߿� Ŭ���� �����Կ� ��ų�� �ִ� ��� 
			// -> ���콺 �̹��� NONE
			// -> �ش� ��ũ��Ʈ Ŭ���� ���� nullptr�� �ʱ�ȭ
			// -> ���콺 ������ Ÿ�� -> NONE

			// 1-2. ���߿� Ŭ���� �����Կ� �������� �ִ� ���
			// -> ���� ������ �������� ���� ������ ��ģ�� + ���콺 �̹��� NONE, Ŭ���� ���� nullptr, ���콺 ������Ÿ�� NONE
			// -> ������ ������ �ٸ� ��� �κ��丮 �ش� ������ ��ġ�� Swap

			// 1-3. ���߿� Ŭ���� �����Կ� �ƹ��͵� ���� ��� 
			// -> �����Կ� ������ �߰�, �κ��丮�� ������ ����, ���콺 �̹��� �� ������ Ÿ�� NONE

			if (ITEM_TYPE::ITEM_NONE != m_pMouseItemImageScript->GetClickItemKind())
			{
				if (ITEM_TYPE::ITEM_NONE == m_pSelQuickSlot->eItemType 
					&& SKILL_TYPE::SKILL_NONE == m_pSelQuickSlot->eSkillType)
				{
					// 1-3
					UpdateItemSlot(m_pSelQuickSlot->iIndex, m_pMouseItemImageScript->GetClickItemKind(), m_pMouseItemImageScript->GetClickItemCount());
					
					m_pMouseItemImageScript->UpdateClickItemImage(ITEM_TYPE::ITEM_NONE, 0, -1);
					m_pMouseItemImageScript->UpdateInventory();
					m_pSelQuickSlot = nullptr;
					return;
				}
				else
				{
					// 1-1, 1-2 ��츦 ������ ����
					if (m_pMouseItemImageScript->GetClickItemKind() == m_pSelQuickSlot->eItemType)
					{
						// ���� �������� ���
						// ������ �κ��丮 ����, ������ �÷��ش�. ( �ִ밳�� ���޽� �и� )
						int AddCount = m_pSelQuickSlot->iCount + m_pMouseItemImageScript->GetClickItemCount();
						
						if (AddCount > 99)
						{
							int iIndex = m_pMouseItemImageScript->GetIndex();
							AddCount -= 99;
							m_pSelQuickSlot->iCount = 99;
							UpdateItemCount(m_pSelQuickSlot->iIndex);
							m_pMouseItemImageScript->UpdateClickItemImage(m_pSelQuickSlot->eItemType, AddCount, iIndex);
						}
						else
						{
							m_pSelQuickSlot->iCount += m_pMouseItemImageScript->GetClickItemCount();
							UpdateItemCount(m_pSelQuickSlot->iIndex);
							m_pMouseItemImageScript->UpdateClickItemImage(ITEM_TYPE::ITEM_NONE, 0, -1);
						}

						m_pMouseItemImageScript->UpdateInventory();
						m_pMouseItemImageScript->SetClickNone();
						m_pSelQuickSlot = nullptr;
			
					}
					else
					{
						// �ٸ� �������� ���, ��� ��ų�� �ƴϿ��� �Ѵ�.
						if (SKILL_TYPE::SKILL_NONE == m_pSelQuickSlot->eSkillType)
						{
							m_pMouseItemImageScript;
							// ������ �κ��丮 �����۰� �������ش�.
							ITEM_TYPE eTempItemType = m_pSelQuickSlot->eItemType;
							int iTempCount = m_pSelQuickSlot->iCount;

							UpdateItemSlot(m_pSelQuickSlot->iIndex, m_pMouseItemImageScript->GetClickItemKind(), m_pMouseItemImageScript->GetClickItemCount());
							m_pMouseItemImageScript->UpdateClickItemImage(eTempItemType, iTempCount, m_pMouseItemImageScript->GetIndex());
							m_pMouseItemImageScript->UpdateInventory();
							m_pMouseItemImageScript->UpdateClickItemImage(ITEM_TYPE::ITEM_NONE, 0, 0);
							m_pSelQuickSlot = nullptr;
						}
					}
				}
				
			}

			// ��ų�� ���� �׳ɺ������´�.
			m_pMouseItemImageScript->SetInventoryClickClear();
			m_pMouseItemImageScript->UpdateClickItemImage(ITEM_TYPE::ITEM_NONE, 0, 0);
			m_pSelQuickSlot = nullptr;
			return;
		}

		for (int i = 0; i < m_vecQuickSlots.size(); ++i)
		{
			if (BS_MOUSEON == m_vecQuickSlots[i]->pQuickSlotButton->GetButtonState() && true == m_vecQuickSlots[i]->bQuickSlot)
			{
				m_pSelQuickSlot = m_vecQuickSlots[i];
				UpdateClickQuickSLotImage(m_pSelQuickSlot->iIndex);
				return;
			}
		}
	}
	else
	{
		// �̹� ���õ� �κ��丮 �������� �ִٸ�
		// ��ġ�� �ٲ��ְų� ���� �Һ����� ���� �����ش�.
		QuickSlotType* NewClickItem = nullptr;

		for (int i = 0; i < m_vecQuickSlots.size(); ++i)
		{
			if (BS_MOUSEON == m_vecQuickSlots[i]->pQuickSlotButton->GetButtonState())
			{
				NewClickItem = m_vecQuickSlots[i];
			}
		}

		if (nullptr == NewClickItem || NewClickItem->iIndex == m_pSelQuickSlot->iIndex)
		{
			m_pSelQuickSlot = nullptr;
			UpdateClickQuickSLotImage(-1);
			return;
		}

		if (true == NewClickItem->bQuickSlot)
		{
			int iNewClickCount = NewClickItem->iCount;
			int iPrevClickCount = m_pSelQuickSlot->iCount;

			// ������, ��ų�� ������.
			if (NewClickItem->eItemType == ITEM_NONE)
			{
				int iTempCount = iPrevClickCount;
				iPrevClickCount = iNewClickCount;
				iNewClickCount = iTempCount;

				// ��ų 
				if (m_pSelQuickSlot->eSkillType == SKILL_TYPE::SKILL_NONE)
				{
					// ���� - �Һ�, ���� - ��ų -> ����, ��ų ��Ÿ��, ���� ��Ÿ��, �̹��� ����
					ITEM_TYPE ePrevItem = m_pSelQuickSlot->eItemType;
					SKILL_TYPE eNewSkill = NewClickItem->eSkillType;

					float fCoolTime = NewClickItem->fCoolTime;
					float fCurCoolTime = NewClickItem->fCurCoolTime;

					NewClickItem->fCurCoolTime = 0.0f;
					m_pSelQuickSlot->fCurCoolTime = fCurCoolTime;

					UpdateSkillSlot(m_pSelQuickSlot->iIndex, eNewSkill, NewClickItem->fCoolTime);
					UpdateItemSlot(NewClickItem->iIndex, ePrevItem, iNewClickCount);

					m_pSelQuickSlot = nullptr;
					UpdateClickQuickSLotImage(-1);
					return;
				}
				else
				{
					// ���� - ��ų, ���� - ��ų  -> �̹���, ��ų ��Ÿ��, ���� ��Ÿ�� ����
					SKILL_TYPE eNewSkill = NewClickItem->eSkillType;

					float fCoolTime = NewClickItem->fCoolTime;
					float fCurCoolTime = NewClickItem->fCurCoolTime;

					NewClickItem->fCurCoolTime = m_pSelQuickSlot->fCurCoolTime;
					m_pSelQuickSlot->fCurCoolTime = fCurCoolTime;

					UpdateSkillSlot(NewClickItem->iIndex, m_pSelQuickSlot->eSkillType, m_pSelQuickSlot->fCoolTime);
					UpdateSkillSlot(m_pSelQuickSlot->iIndex, eNewSkill, fCoolTime);
				}

				m_pSelQuickSlot = nullptr;
				UpdateClickQuickSLotImage(-1);
				return;
			}
			else
			{
				// �Һ����� ��� ( �Һ���, �Һ���)
				if (NewClickItem->eItemType == m_pSelQuickSlot->eItemType)
				{
					// �Һ�������� ��츸, �����ش�.
					if (m_pSelQuickSlot->eItemType < ITEM_TYPE::ITEM_POTION_MAX)
					{

						int iAddCount = iNewClickCount + iPrevClickCount;

						if (iAddCount < 100)
						{
							NewClickItem->iCount += m_pSelQuickSlot->iCount;
							UpdateItemCount(NewClickItem->iIndex);

							// �������� ���� ����
							UpdateItemSlot(m_pSelQuickSlot->iIndex, ITEM_TYPE::ITEM_NONE, 0);
							m_pSelQuickSlot = nullptr;
							m_pMouseItemImageScript->UpdateClickItemImage(ITEM_TYPE::ITEM_NONE, 0, m_pMouseItemImageScript->GetIndex());
							UpdateClickQuickSLotImage(-1);
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
							m_pSelQuickSlot->iCount = iPrevClickCount;

							UpdateItemCount(NewClickItem->iIndex);
							UpdateItemCount(m_pSelQuickSlot->iIndex);

							m_pSelQuickSlot = nullptr;
							m_pMouseItemImageScript->UpdateClickItemImage(ITEM_TYPE::ITEM_NONE, 0, m_pMouseItemImageScript->GetIndex());
							UpdateClickQuickSLotImage(-1);
							return;
						}
					}
				}
				else if (NewClickItem->eItemType != m_pSelQuickSlot->eItemType 
					&& SKILL_TYPE::SKILL_NONE == m_pSelQuickSlot->eSkillType
					&&  SKILL_TYPE::SKILL_NONE == NewClickItem->eSkillType)
				{
					ITEM_TYPE eTempType = m_pSelQuickSlot->eItemType;
					int iTempCount = m_pSelQuickSlot->iCount;

					UpdateItemSlot(m_pSelQuickSlot->iIndex, NewClickItem->eItemType, NewClickItem->iCount);
					UpdateItemSlot(NewClickItem->iIndex, eTempType, iTempCount);

					m_pSelQuickSlot = nullptr;
					m_pMouseItemImageScript->UpdateClickSkillImage(SKILL_TYPE::SKILL_NONE, 0, 0.0f, 0.0f);
					UpdateClickQuickSLotImage(-1);
					return;
				}
				else
				{
					// ��ų -> �Һ�
					SKILL_TYPE  eTempSkillType = m_pSelQuickSlot->eSkillType;
					float fTempCoolTime = m_pSelQuickSlot->fCoolTime;
					float fTempCurCoolTime = m_pSelQuickSlot->fCurCoolTime;

					UpdateItemSlot(m_pSelQuickSlot->iIndex, NewClickItem->eItemType, NewClickItem->iCount);
					UpdateSkillSlot(NewClickItem->iIndex, eTempSkillType, fTempCoolTime);
					NewClickItem->fCurCoolTime = fTempCurCoolTime;

					m_pSelQuickSlot = nullptr;
					m_pMouseItemImageScript->UpdateClickSkillImage(SKILL_TYPE::SKILL_NONE, 0, 0.0f, 0.0f);
					UpdateClickQuickSLotImage(-1);
					return;
				}
			}
		}
		

		// �������� ���ų� �� ������ ��� -> ��ġ �ٲٱ�
		ITEM_TYPE pTempType = m_pSelQuickSlot->eItemType;
		SKILL_TYPE eTempSkillType = m_pSelQuickSlot->eSkillType;

		if (ITEM_TYPE::ITEM_NONE == pTempType)
		{
			// ��ų -> �����
			NewClickItem->fCurCoolTime = m_pSelQuickSlot->fCurCoolTime;
			UpdateSkillSlot(NewClickItem->iIndex, eTempSkillType, m_pSelQuickSlot->fCoolTime);
			UpdateSkillSlot(m_pSelQuickSlot->iIndex, SKILL_TYPE::SKILL_NONE, 0.0f);
		}
		else
		{
			// ������ -> �����
			UpdateItemSlot(NewClickItem->iIndex, pTempType, m_pSelQuickSlot->iCount);
			UpdateItemSlot(m_pSelQuickSlot->iIndex, ITEM_TYPE::ITEM_NONE, 0);
		}

	}
	m_pSelQuickSlot = nullptr;
	UpdateClickQuickSLotImage(-1);
}

void QuickSlot::UpdateClickQuickSLotImage(int _Index)
{
	if (nullptr == m_pMouseItemImageScript)
	{
		return;
	}

	if (nullptr == m_pSelQuickSlot || -1 == _Index)
	{
		m_pMouseItemImageScript->UpdateClickItemImage(ITEM_TYPE::ITEM_NONE, 0, -1);
		m_pMouseItemImageScript->SetbClickQuickslot(false);
		return;
	}

	if (ITEM_TYPE::ITEM_NONE != m_pSelQuickSlot->eItemType && SKILL_TYPE::SKILL_NONE == m_pSelQuickSlot->eSkillType)
	{
		switch (m_pSelQuickSlot->eItemType)
		{
		case ITEM_TYPE::POTION_HP:
		case ITEM_TYPE::POTION_MP:
			m_pMouseItemImageScript->UpdateClickItemImage(m_pSelQuickSlot->eItemType, m_pSelQuickSlot->iCount, _Index);
			m_pMouseItemImageScript->SetbClickQuickslot(true);
			break;
		default:
			break;
		}
	}
	else if (SKILL_TYPE::SKILL_NONE != m_pSelQuickSlot->eSkillType && ITEM_TYPE::ITEM_NONE == m_pSelQuickSlot->eItemType)
	{
		switch (m_pSelQuickSlot->eSkillType)
		{
		case SKILL_TYPE::SKILL_01:
		case SKILL_TYPE::SKILL_02:
		case SKILL_TYPE::SKILL_03:
		case SKILL_TYPE::SKILL_04:
		case SKILL_TYPE::SKILL_05:
			m_pMouseItemImageScript->UpdateClickSkillImage(m_pSelQuickSlot->eSkillType, _Index, m_pSelQuickSlot->fCoolTime, m_pSelQuickSlot->fCurCoolTime);
			m_pMouseItemImageScript->SetbClickQuickslot(true);
			break;
		default:
			break;
		}
	}
	
}


void QuickSlot::UpdateItemCount(int _Index)
{
	std::string ImageName = "ItemCountNum_";
	int iTensNum = (int)(m_vecQuickSlots[_Index]->iCount * 0.1);
	int iUnitsNum = m_vecQuickSlots[_Index]->iCount - (10 * iTensNum);

	if (iTensNum > 0)
	{
		std::string strNum = IntToString(iTensNum);
		ImageName += strNum;
		std::string strImageTexName = ImageName;
		ImageName += ".png";
		std::wstring wstrImageName = StringToLPWSTR(ImageName);
		m_vecQuickSlots[_Index]->pItemTensCountImage->SetTexture(strImageTexName, wstrImageName.c_str(), PATH_UI_NUMBER);

		// 1�� �ڸ��� ������ ǥ��
		ImageName = "ItemCountNum_";
		strNum = IntToString(iUnitsNum);
		ImageName += strNum;
		strImageTexName = ImageName;
		ImageName += ".png";
		wstrImageName = StringToLPWSTR(ImageName);
		m_vecQuickSlots[_Index]->pItemUnitsCountImage->SetTexture(strImageTexName, wstrImageName.c_str(), PATH_UI_NUMBER);
	}
	else
	{
		m_vecQuickSlots[_Index]->pItemTensCountImage->SetTexture("ItemCountNum_NONE", TEXT("ItemCountNum_NONE.png"), PATH_UI_NUMBER);

		// ���⼱ 1���ڸ� ���� ǥ�� 0�ϰ�� X, 0���� Ŀ����
		if (iUnitsNum > 0)
		{
			std::string strNum = IntToString(iUnitsNum);
			ImageName += strNum;
			std::string strImageTexName = ImageName;
			ImageName += ".png";
			std::wstring wstrImageName = StringToLPWSTR(ImageName);
			m_vecQuickSlots[_Index]->pItemUnitsCountImage->SetTexture(strImageTexName, wstrImageName.c_str(), PATH_UI_NUMBER);
		}
		else
		{
			m_vecQuickSlots[_Index]->pItemUnitsCountImage->SetTexture("ItemCountNum_NONE", TEXT("ItemCountNum_NONE.png"), PATH_UI_NUMBER);
		}
	}
}

void QuickSlot::UpdateItemSlot(int _Index, ITEM_TYPE _eItemType, int _iCount)
{
	if (_Index >= m_vecQuickSlots.size())
	{
		return;
	}

	switch (_eItemType)
	{
	case POTION_HP:
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_NORMAL, "Item_PotionHP", TEXT("potion10_Tex.tga"), PATH_UI_ITEM);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_MOUSEON, "Item_PotionHP", TEXT("potion10_Tex.tga"), PATH_UI_ITEM);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_CLICK, "Item_PotionHP", TEXT("potion10_Tex.tga"), PATH_UI_ITEM);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_PUSH, "Item_PotionHP", TEXT("potion10_Tex.tga"), PATH_UI_ITEM);
		break;
	case POTION_MP:
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_NORMAL, "Item_PotionMP", TEXT("potion9_Tex.tga"), PATH_UI_ITEM);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_MOUSEON, "Item_PotionMP", TEXT("potion9_Tex.tga"), PATH_UI_ITEM);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_CLICK, "Item_PotionMP", TEXT("potion9_Tex.tga"), PATH_UI_ITEM);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_PUSH, "Item_PotionMP", TEXT("potion9_Tex.tga"), PATH_UI_ITEM);
		break;
	default:
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_NORMAL, "ItemCountNum_NONE", TEXT("ItemCountNum_NONE.png"), PATH_UI_NUMBER);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_MOUSEON, "ItemCountNum_NONE", TEXT("ItemCountNum_NONE.png"), PATH_UI_NUMBER);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_CLICK, "ItemCountNum_NONE", TEXT("ItemCountNum_NONE.png"), PATH_UI_NUMBER);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_PUSH, "ItemCountNum_NONE", TEXT("ItemCountNum_NONE.png"), PATH_UI_NUMBER);
		m_vecQuickSlots[_Index]->iCount = 0;
		m_vecQuickSlots[_Index]->eItemType = ITEM_TYPE::ITEM_NONE;
		m_vecQuickSlots[_Index]->bQuickSlot = false;
		UpdateItemCount(_Index);
		return;
	}
	m_vecQuickSlots[_Index]->eItemType = _eItemType;
	m_vecQuickSlots[_Index]->eSkillType = SKILL_TYPE::SKILL_NONE;
	m_vecQuickSlots[_Index]->bCoolTime = false;
	m_vecQuickSlots[_Index]->bQuickSlot = true;
	m_vecQuickSlots[_Index]->iCount = _iCount;
	UpdateItemCount(_Index);
}

void QuickSlot::UpdateSkillSlot(int _Index, SKILL_TYPE _eSkillType, float _CoolTime)
{
	if (_Index >= m_vecQuickSlots.size())
	{
		return;
	}

	switch (_eSkillType)
	{
	case SKILL_01:
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_NORMAL, "Icon_SKill01", TEXT("Cuttingslash_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_CLICK, "Icon_SKill01", TEXT("Cuttingslash_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_MOUSEON, "Icon_SKill01", TEXT("Cuttingslash_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_PUSH, "Icon_SKill01", TEXT("Cuttingslash_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		break;
	case SKILL_02:
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_NORMAL, "Icon_SKill02", TEXT("Stingerblade_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_CLICK, "Icon_SKill02", TEXT("Stingerblade_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_MOUSEON, "Icon_SKill02", TEXT("Stingerblade_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_PUSH, "Icon_SKill02", TEXT("Stingerblade_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		break;
	case SKILL_03:
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_NORMAL, "Icon_SKill03", TEXT("Whirlwind_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_CLICK, "Icon_SKill03", TEXT("Whirlwind_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_MOUSEON, "Icon_SKill03", TEXT("Whirlwind_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_PUSH, "Icon_SKill03", TEXT("Whirlwind_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		break;
	case SKILL_04:
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_NORMAL, "Icon_SKill04", TEXT("advancingcharge_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_CLICK, "Icon_SKill04", TEXT("advancingcharge_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_MOUSEON, "Icon_SKill04", TEXT("advancingcharge_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_PUSH, "Icon_SKill04", TEXT("advancingcharge_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		break;
	case SKILL_05:
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_NORMAL, "Icon_SKill05", TEXT("Cutting_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_CLICK, "Icon_SKill05", TEXT("Cutting_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_MOUSEON, "Icon_SKill05", TEXT("Cutting_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_PUSH, "Icon_SKill05", TEXT("Cutting_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		break;
	default:
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_NORMAL, "Icon_SKill_NONE", TEXT("Skill_NONE.png"), PATH_UI_SKILL_TERA_ICON);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_CLICK, "Icon_SKill_NONE", TEXT("Skill_NONE.png"), PATH_UI_SKILL_TERA_ICON);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_MOUSEON, "Icon_SKill_NONE", TEXT("Skill_NONE.png"), PATH_UI_SKILL_TERA_ICON);
		m_vecQuickSlots[_Index]->pQuickSlotButton->SetStateTexture(BS_PUSH, "Icon_SKill_NONE", TEXT("Skill_NONE.png"), PATH_UI_SKILL_TERA_ICON);

		m_vecQuickSlots[_Index]->eSkillType = SKILL_TYPE::SKILL_NONE;
		m_vecQuickSlots[_Index]->eItemType = ITEM_TYPE::ITEM_NONE;
		m_vecQuickSlots[_Index]->bQuickSlot = false;
		m_vecQuickSlots[_Index]->bCoolTime = false;
		m_vecQuickSlots[_Index]->fCoolTime = 0.0f;
		return;
	}

	m_vecQuickSlots[_Index]->eSkillType = _eSkillType;
	m_vecQuickSlots[_Index]->eItemType = ITEM_TYPE::ITEM_NONE;
	m_vecQuickSlots[_Index]->bQuickSlot = true;
	m_vecQuickSlots[_Index]->bCoolTime = true;
	m_vecQuickSlots[_Index]->fCoolTime = _CoolTime;
	m_vecQuickSlots[_Index]->fCurCoolTime = 0.0f;
	m_vecQuickSlots[_Index]->iCount = 0;
	UpdateItemCount(_Index);
}

void QuickSlot::SetClickInfoClear()
{
	m_pSelQuickSlot = nullptr;
}

void QuickSlot::QuickSlotOff()
{
	m_pQuickSlotBGObj->Enable(false);

	for (int i = 0; i < m_vecQuickSlots.size(); ++i)
	{
		m_vecQuickSlots[i]->pEmptyQuickSlotObject->Enable(false);
		m_vecQuickSlots[i]->pQuickSlotObject->Enable(false);
		//m_vecQuickSlots[i]->pCoolTimeObject->Enable(false);
		m_vecQuickSlots[i]->pQuickSlotNumObject->Enable(false);
		m_vecQuickSlots[i]->pItemTensCountObject->Enable(false);
		m_vecQuickSlots[i]->pItemUnitsCountObject->Enable(false);
	}
}

void QuickSlot::QuickSlotOn()
{
	m_pQuickSlotBGObj->Enable(true);

	for (int i = 0; i < m_vecQuickSlots.size(); ++i)
	{
		m_vecQuickSlots[i]->pEmptyQuickSlotObject->Enable(true);
		m_vecQuickSlots[i]->pQuickSlotObject->Enable(true);
		//m_vecQuickSlots[i]->pCoolTimeObject->Enable(true);
		m_vecQuickSlots[i]->pQuickSlotNumObject->Enable(true);
		m_vecQuickSlots[i]->pItemTensCountObject->Enable(true);
		m_vecQuickSlots[i]->pItemUnitsCountObject->Enable(true);
	}
}

void QuickSlot::AddSkill(SKILL_TYPE _eSkillType)
{
	QuickSlotType* pNewAddSkillSlot = nullptr;

	for (int i = 0; i < m_vecQuickSlots.size(); ++i)
	{
		if (false == m_vecQuickSlots[i]->bQuickSlot)
		{
			pNewAddSkillSlot = m_vecQuickSlots[i];
			break;
		}
	}

	if (nullptr == pNewAddSkillSlot)
	{
		return;
	}

	switch (_eSkillType)
	{
	case SKILL_01:
		pNewAddSkillSlot->pQuickSlotButton->SetStateTexture(BS_NORMAL, "Icon_SKill01", TEXT("Cuttingslash_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		pNewAddSkillSlot->pQuickSlotButton->SetStateTexture(BS_CLICK, "Icon_SKill01", TEXT("Cuttingslash_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		pNewAddSkillSlot->pQuickSlotButton->SetStateTexture(BS_MOUSEON, "Icon_SKill01", TEXT("Cuttingslash_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		pNewAddSkillSlot->pQuickSlotButton->SetStateTexture(BS_PUSH, "Icon_SKill01", TEXT("Cuttingslash_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		pNewAddSkillSlot->fCoolTime = 1.0f;
		break;
	case SKILL_02:
		pNewAddSkillSlot->pQuickSlotButton->SetStateTexture(BS_NORMAL, "Icon_SKill02", TEXT("Stingerblade_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		pNewAddSkillSlot->pQuickSlotButton->SetStateTexture(BS_CLICK, "Icon_SKill02", TEXT("Stingerblade_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		pNewAddSkillSlot->pQuickSlotButton->SetStateTexture(BS_MOUSEON, "Icon_SKill02", TEXT("Stingerblade_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		pNewAddSkillSlot->pQuickSlotButton->SetStateTexture(BS_PUSH, "Icon_SKill02", TEXT("Stingerblade_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		pNewAddSkillSlot->fCoolTime = 1.0f;
		break;
	case SKILL_03:
		pNewAddSkillSlot->pQuickSlotButton->SetStateTexture(BS_NORMAL, "Icon_SKill03", TEXT("Whirlwind_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		pNewAddSkillSlot->pQuickSlotButton->SetStateTexture(BS_CLICK, "Icon_SKill03", TEXT("Whirlwind_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		pNewAddSkillSlot->pQuickSlotButton->SetStateTexture(BS_MOUSEON, "Icon_SKill03", TEXT("Whirlwind_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		pNewAddSkillSlot->pQuickSlotButton->SetStateTexture(BS_PUSH, "Icon_SKill03", TEXT("Whirlwind_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		pNewAddSkillSlot->fCoolTime = 1.0f;
		break;
	case SKILL_04:
		pNewAddSkillSlot->pQuickSlotButton->SetStateTexture(BS_NORMAL, "Icon_SKill04", TEXT("advancingcharge_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		pNewAddSkillSlot->pQuickSlotButton->SetStateTexture(BS_CLICK, "Icon_SKill04", TEXT("advancingcharge_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		pNewAddSkillSlot->pQuickSlotButton->SetStateTexture(BS_MOUSEON, "Icon_SKill04", TEXT("advancingcharge_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		pNewAddSkillSlot->pQuickSlotButton->SetStateTexture(BS_PUSH, "Icon_SKill04", TEXT("advancingcharge_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		pNewAddSkillSlot->fCoolTime = 1.0f;
		break;
	case SKILL_05:
		pNewAddSkillSlot->pQuickSlotButton->SetStateTexture(BS_NORMAL, "Icon_SKill05", TEXT("Cutting_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		pNewAddSkillSlot->pQuickSlotButton->SetStateTexture(BS_CLICK, "Icon_SKill05", TEXT("Cutting_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		pNewAddSkillSlot->pQuickSlotButton->SetStateTexture(BS_MOUSEON, "Icon_SKill05", TEXT("Cutting_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		pNewAddSkillSlot->pQuickSlotButton->SetStateTexture(BS_PUSH, "Icon_SKill05", TEXT("Cutting_Tex.tga"), PATH_UI_SKILL_TERA_ICON);
		pNewAddSkillSlot->fCoolTime = 1.0f;
		break;
	default:
		pNewAddSkillSlot->pQuickSlotButton->SetStateTexture(BS_NORMAL, "Icon_SKill_NONE", TEXT("Skill_NONE.png"), PATH_UI_SKILL_TERA_ICON);
		pNewAddSkillSlot->pQuickSlotButton->SetStateTexture(BS_CLICK, "Icon_SKill_NONE", TEXT("Skill_NONE.png"), PATH_UI_SKILL_TERA_ICON);
		pNewAddSkillSlot->pQuickSlotButton->SetStateTexture(BS_MOUSEON, "Icon_SKill_NONE", TEXT("Skill_NONE.png"), PATH_UI_SKILL_TERA_ICON);
		pNewAddSkillSlot->pQuickSlotButton->SetStateTexture(BS_PUSH, "Icon_SKill_NONE", TEXT("Skill_NONE.png"), PATH_UI_SKILL_TERA_ICON);

		pNewAddSkillSlot->eSkillType = SKILL_TYPE::SKILL_NONE;
		pNewAddSkillSlot->eItemType = ITEM_TYPE::ITEM_NONE;
		pNewAddSkillSlot->bQuickSlot = false;
		pNewAddSkillSlot->bCoolTime = false;
		pNewAddSkillSlot->fCoolTime = 0.0f;
		return;
	}

	pNewAddSkillSlot->eSkillType = _eSkillType;
	pNewAddSkillSlot->eItemType = ITEM_TYPE::ITEM_NONE;
	pNewAddSkillSlot->bQuickSlot = true;
	pNewAddSkillSlot->bCoolTime = true;
	pNewAddSkillSlot->fCurCoolTime = 0.0f;
}

bool QuickSlot::GetPrevInventoryClick()
{
	if (nullptr == m_pMouseItemImageScript)
	{
		return false;
	}

	if (ITEM_TYPE::ITEM_NONE == m_pMouseItemImageScript->GetClickItemKind())
	{
		return false;
	}

	return true;
}
