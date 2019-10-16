#pragma once
#include "Component\UserComponent.h"
#include "Component\Renderer.h"
#include "Component\Transform.h"
#include "Component\Material.h"
#include "Component\Animation.h"
#include "Component\UIButton.h"
#include "Component\UIBar.h"
#include "Component\UIText.h"

ENGINE_USING

#define POTION_HP_VALUE 300
#define POTION_MP_VALUE 300
#define WEAPON_VALUE	500

enum BaseState
{
	NONE = -1,
	IDLE = 0,
	WALK,
	RUN,
	WAIT,
	TALK,
	DANCE_Q,
	DEATH,
	DEATHWAIT,
	KNOCKBACK,
	GROGGY,
	TUMBLING,
	SKILL01,
	SKILL02,
	SKILL03,
	SKILL04,
	SKILL05,
	COMBO1,							// �÷��̾�
	COMBO1R,
	COMBO2,
	COMBO2R,
	COMBO3,
	COMBO3R,
	COMBO4,
	SHOT,							// �ǰ�
	BOSS_HEAL_START,
	BOSS_HEAL_LOOP,
	BOSS_HEAL_END,
	BOSS_MOVE_ATK,
	ATK01,							// ����
	ATK02,
	ATK03,
	ATK04,
	ATK05,
	BOSS_ROLLING_ATK_START,
	BOSS_ROLLING_ATK_LOOP,
	BOSS_ROLLING_ATK_END,
	MAX,
};

struct BaseUIState
{
	int m_iLevel;
	int m_iHP;
	int m_iMP;
	int m_iCurEXP;
	int m_iHPMax;
	int m_iMPMax;
	int m_iMaxEXP;
};

struct CurCoolTime
{
	float m_fCurCoolTime01;
	float m_fCurCoolTime02;
	float m_fCurCoolTime03;
	float m_fCurCoolTime04;
	float m_fCurCoolTime05;
};

// ������ Ÿ��
enum ITEM_TYPE
{
	ITEM_NONE = 0,
	POTION_HP,
	POTION_MP,
	ITEM_POTION_MAX,
	ITEM_WEAPON,
	ITEM_EQUIP,
};

enum SKILL_TYPE
{
	SKILL_NONE =0,
	SKILL_01,
	SKILL_02,
	SKILL_03,
	SKILL_04,
	SKILL_05,
	CUTTING_SLASH = SKILL_01,
	STINGER_BLADE = SKILL_02,
	RISING_ATTCK = SKILL_03,
	WIND_CUTTER1 = SKILL_04,
	WIND_CUTTER5 = SKILL_05,
};

struct ShopItemType
{
	std::string strShopItemName;
	ITEM_TYPE	eItemType;
	bool		bShopItemPotion;				// ����, ��� ����
	int			iShopItemCount;					// ����
	
	class GameObject*	pShopSelectObject;
	Transform*			pShopSelectTr;
	UIButton*			pShopSelectImage;
};

struct InventoryType
{
	bool			bItem;						// ������ ���� ����
	int				iIndex;
	std::string		strItemName;

	class GameObject* pEmptyObj;
	Transform*		pEmptyTr;
	UIButton*		pEmptyImage;

	class GameObject* pItemObj;
	Transform*		pItemTr;
	UIButton*		pItemButton;

	class GameObject*	pItemTensCountObject;
	Transform*			pItemTensTr;
	UIBar*				pItemTensCountImage;

	class GameObject*	pItemUnitsCountObject;
	Transform*			pItemUnitsTr;
	UIBar*				pItemUnitsCountImage;

	ITEM_TYPE	eItemType;
	int			iCount;					// ����
	int			iValue;					// ���� ������

	InventoryType()
	{
		bItem = false;

		pEmptyObj = nullptr;
		pEmptyTr = nullptr;
		pEmptyImage = nullptr;

		pItemObj = nullptr;
		pItemTr = nullptr;
		pItemButton = nullptr;

		pItemTensCountObject = nullptr;
		pItemTensTr = nullptr;
		pItemTensCountImage = nullptr;

		pItemUnitsCountObject = nullptr;
		pItemUnitsTr = nullptr;
		pItemUnitsCountImage = nullptr;

		eItemType = ITEM_TYPE::ITEM_NONE;
		strItemName = "";
		iIndex = -1;
		iCount = 0;
		iValue = 0;
	}
};

struct QuickSlotType
{
	bool		bQuickSlot;
	int			iIndex;

	// �� ����
	GameObject* pEmptyQuickSlotObject;
	UIButton*	pEmptyQuickSlotButton;

	// ������ OR ��ų ����
	GameObject* pQuickSlotObject;
	Transform*  pQuickSlotTr;
	UIButton*	pQuickSlotButton;

	// ��Ÿ�� �̹���
	GameObject*	pCoolTimeObject;
	Transform*	pCoolTimeTr;
	UIBar*		pCoolTimeImage;

	// ������ ��ȣ (Ű)
	GameObject* pQuickSlotNumObject;
	UIBar*		pQuickSlotNumImage;

	// ����
	class GameObject*	pItemTensCountObject;
	Transform*			pItemTensTr;
	UIBar*				pItemTensCountImage;

	class GameObject*	pItemUnitsCountObject;
	Transform*			pItemUnitsTr;
	UIBar*				pItemUnitsCountImage;

	ITEM_TYPE	eItemType;
	SKILL_TYPE	eSkillType;
	int			iSlotNumber;					// �ش� ���� Ű (Ű���� Ű)
	bool		bCoolTime;
	float		fCurCoolTime;
	float		fCoolTime;
	int			iCount;

	QuickSlotType()
	{
		pEmptyQuickSlotObject = nullptr;
		pEmptyQuickSlotButton = nullptr;
		pQuickSlotObject = nullptr;
		pQuickSlotTr = nullptr;
		pQuickSlotButton = nullptr;
		pQuickSlotNumObject = nullptr;
		pQuickSlotNumImage = nullptr;

		pItemTensCountObject = nullptr;
		pItemTensTr = nullptr;
		pItemTensCountImage = nullptr;

		pItemUnitsCountObject = nullptr;
		pItemUnitsTr = nullptr;
		pItemUnitsCountImage = nullptr;

		eItemType = ITEM_TYPE::ITEM_NONE;
		eSkillType = SKILL_TYPE::SKILL_NONE;
		iSlotNumber = 0;
		bCoolTime = false;
		fCurCoolTime = 0.0f;
		fCoolTime = 0.0f;
		iCount = 0;
	}
};

enum ActorKind
{
	AK_NONE = 0,
	AK_STATIC,
	AK_PLAYER,
	AK_PLAYER_HIT,
	AK_HOOKA,
	AK_DOLL,
	AK_BOSS,
	AK_NPC_POTION,
	AK_NPC_QUEST,
};

enum NPCKind
{
	NPC_NONE = 0,
	NPC_SHOP,
	NPC_GODDESS,
	NPC_PASSERBY,
	NPC_BOY,
};

enum NPCDialogueKind
{
	NDK_NONE = 0,
	NDK_CONVERSATION,
	NDK_QUEST
};

enum BtType
{
	BT_NONE = 0,
	BT_SHOP,
	BT_CONVERSATION,
	BT_QUEST,
	BT_EXIT,
};

class Base : public UserComponent
{
public:
	ActorKind m_eAK;				// ����

public:
	Base();
	Base(const Base& _Base);
	~Base();

public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��, �� ���̾�� ȣ�����ش�.
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);	// ������ �ϱ� ������ ȣ�����ִ� �Լ��Ĥ���
	virtual int Render(float _fTime);
	virtual Base* Clone()	const;
};

