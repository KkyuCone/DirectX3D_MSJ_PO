#pragma once
#include "Base.h"
#include "MouseItemSkill.h"
#include "Component/AudioSound.h"

ENGINE_USING
class QuickSlot : public Base
{
private:
	GameObject* m_pQuickSlotBGObj;
	UIBar* m_pQuickSlotBG;

	std::vector<QuickSlotType*> m_vecQuickSlots;

private:
	QuickSlotType*	m_pSelQuickSlot;
	AudioSound* m_pEffectSound;

public:
	QuickSlot();
	QuickSlot(const QuickSlot& _QuickSlot);
	~QuickSlot();

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);
	virtual int Render(float _fTime);
	virtual QuickSlot* Clone()	const;

private:
	MouseItemSkill*		m_pMouseItemImageScript;

public:
	void SetMouseItemSkillScript(class MouseItemSkill* _Script);

private:
	// 클릭한 인벤토리 아이템 정보
	void SelectItem(float _fTime);
	void UpdateClickQuickSLotImage(int _Index);
	void UpdateItemCount(int _Index);

public:
	void UpdateItemSlot(int _Index, ITEM_TYPE _eItemType, int _iCount);
	void UpdateSkillSlot(int _Index, SKILL_TYPE _eSkillType, float _CoolTime);
	void SetClickInfoClear();
	void QuickSlotOff();
	void QuickSlotOn();

private:
	void AddSkill(SKILL_TYPE _eSkillType);

private:
	// 기존에 인벤토리를 클릭한 경우 체크
	bool GetPrevInventoryClick();
};



