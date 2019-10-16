#pragma once
#include "MouseItemSkill.h"
#include "Component/AudioSound.h"
#include "Base.h"

ENGINE_USING

#define CIPHER_VALUE 6

#define INVEN_X 9
#define INVEN_Y 9

struct MoneyNumber
{
	GameObject* pObject;
	Transform*	pTr;
	UIBar*		pNumberImage;

	MoneyNumber()
	{
		pObject = nullptr;
		pTr = nullptr;
		pNumberImage = nullptr;
	}
};

class Inventory : public Base
{
private:
	bool	m_bInventoryEnable;
	int		m_iScrollMin;
	int		m_iScrollMax;
	int		m_iScrollCur;

private:
	// 배경
	GameObject* m_pInventoryBGObject;
	Transform*	m_pInventoryBGTr;
	UIBar*	m_pInventoryBGImage;

	// 인벤토리 이동용 오브젝트
	GameObject*	m_pInventoryMoveObject;
	Transform*	m_pInventoryMoveTr;
	UIButton*	m_pInventoryMoveButton;

	//UI 버튼들 (닫기 버튼같은거)
	GameObject* m_pCloseButtonObject;
	Transform*	m_pCloseButtonTr;
	UIButton*	m_pCloseButton;

	// 스크롤 바 
	GameObject* m_pScrollBarBGObject;				// 바 배경이미지
	Transform*	m_pScrollBarBGTr;
	UIBar*		m_pScrollBarBGImage;

	GameObject*	m_pScrollBarObject;
	Transform*	m_pScrollbarTr;
	UIButton*	m_pScrollBarButton;

	GameObject*	m_pScrollBarUpObject;
	Transform*	m_pScrollBarUpTr;
	UIButton*	m_pScrollBarUpButton;

	GameObject*	m_pScrollBarDownObject;
	Transform*	m_pScrollBarDownTr;
	UIButton*	m_pScrollBarDownButton;

	// 아이템칸들 (인벤토리에 있는) 
	std::vector<InventoryType*> m_pItemObjects;

	// 돈
	int m_iMoney;
	std::vector<MoneyNumber*> m_vecMoney;

	AudioSound* m_pEffectSound;

private:
	class Weapon*		pPlayerWeapon;

private:
	// 클릭한 인벤토리 아이템 정보
	InventoryType*		m_pClickInvenItem;


private:
	Vector2 m_vPrevMousePos;
	Vector2 m_vCurMousePos;

	Vector2 m_vPrevBarPos;
	Vector2 m_vCurBarPos;

public:
	Inventory();
	Inventory(const Inventory& _Inventory);
	~Inventory();

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);
	virtual int Render(float _fTime);
	virtual Inventory* Clone()	const;

private:
	void MoneyInit();

public:
	bool GetInventoryEnable();
	void SetPlayerWeaponScript(class Weapon* _Script);

public:
	// 인벤토리 On, Off
	void InventoryEnable(bool _Enable);
	void InventoryInputKey();
	void InventoryOff();
	void InventoryOff(float _fTime);
	void InventoryOn();

public:
	// 인벤토리 이동
	void InventoryPrevPos(float _fTime);
	void InventoryMove(float _fTime);

public:
	// 인벤토리 스크롤 ( 올리기, 내리기 )
	void InventoryRaiseItem(float _fTime);
	void InventoryLiftItem(float _fTime);
	void InventoryCutItem();

	void InventoryBarPrevPos(float _fTime);
	void InventoryBarMove(float _fTime);
	void InventoryBarMoveClickUp(float _fTime);

	void InventoryScrollMoveMouseWheel(float _fTime);

public:
	// 아이템 추가 및 삭제
	bool CalculateMoney(int _Value);
	bool BuyItem(ITEM_TYPE _eItemType, int _iCount = 0, int _Value = 0);bool AddItem(ITEM_TYPE _eItemType, int _iCount = 0);
	void DeleteItem(int _Index);
	void DeleteClickedItem();								//  클릭한 아이템 삭제(정보만 삭제하는거임)
	void ClearItem(int _Index);

	// 아이템 개수 업데이트
	void UpdateItemCount(int _Index);

	// 클릭한 인벤토리 아이템 정보
	void SelectItem(float _fTime);
	void UseItem(float _fTime);

private:
	// 마우스 이미지 관련
	MouseItemSkill*		m_pMouseItemImageScript;

public:
	void SetMouseItemSkillScript(class MouseItemSkill* _Script);

private:
	void UpdateClickItemImage();

	public:
		void UpdateInventorySlot(ITEM_TYPE _eItemType, int _iCount, int _Index);
		void SetClickItemClaer();
};

