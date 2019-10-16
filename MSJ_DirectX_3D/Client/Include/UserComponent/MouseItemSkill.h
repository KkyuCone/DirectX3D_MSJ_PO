#pragma once
#include "Component\Renderer.h"
#include "Component\Transform.h"
#include "Component\Material.h"
#include "Component\Animation.h"
#include "Component\UIButton.h"
#include "Component\UIBar.h"
#include "Base.h"

ENGINE_USING

class MouseItemSkill : public Base
{
private:
	// 마우스 따라다니는 이미지
	GameObject*			m_pClickObject;
	UIBar*				m_pClickImage;
	Transform*			m_pClickTr;

	ITEM_TYPE m_eClickItem;
	SKILL_TYPE m_eClickSkill;

	ITEM_TYPE m_ePrevClickItem;
	SKILL_TYPE m_ePrevClickSkill;

	int iItemCount;
	int iIndex;
	float fCoolTime;
	float fCurCoolTime;
	bool bClickInventory;
	bool bClickQuickSlot;

public:
	MouseItemSkill();
	MouseItemSkill(const MouseItemSkill& _MouseItemSkill);
	~MouseItemSkill();

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);
	virtual int Render(float _fTime);
	virtual MouseItemSkill* Clone()	const;

private:
	class QuickSlot* pQuickslot;
	class Inventory* pInventory;

public:
	void UpdateQuickSlot();
	void UpdateInventory();

public:
	void SetQuickslotScript(class QuickSlot* _pScript);
	void SetInventoryScript(class Inventory* _pScript);

public:
	void UpdateClickImageMove();
	void UpdateClickItemImage(ITEM_TYPE _eItemType, int _iCount, int _Index);
	void UpdateClickSkillImage(SKILL_TYPE _eSkillType, int _Index, float _CoolTime, float _CurCoolTime);

public:
	void SetbClickInventory(bool _Value);
	void SetbClickQuickslot(bool _Value);

	void SetClickNone();

	void SetPrevItemType(ITEM_TYPE _eType);
	void SetPrevSkillType(SKILL_TYPE _eSkillType);

	void SetInventoryClickClear();
	void SetQuickSlotClickClear();

public:
	ITEM_TYPE	GetClickItemKind();
	int			GetClickItemCount();
	SKILL_TYPE GetClickSkillKind();

	bool GetbClickInventory();
	bool GetbClickQuickslot();
	int GetIndex();
};

