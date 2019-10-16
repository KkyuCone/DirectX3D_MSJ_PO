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
	// ���
	GameObject* m_pInventoryBGObject;
	Transform*	m_pInventoryBGTr;
	UIBar*	m_pInventoryBGImage;

	// �κ��丮 �̵��� ������Ʈ
	GameObject*	m_pInventoryMoveObject;
	Transform*	m_pInventoryMoveTr;
	UIButton*	m_pInventoryMoveButton;

	//UI ��ư�� (�ݱ� ��ư������)
	GameObject* m_pCloseButtonObject;
	Transform*	m_pCloseButtonTr;
	UIButton*	m_pCloseButton;

	// ��ũ�� �� 
	GameObject* m_pScrollBarBGObject;				// �� ����̹���
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

	// ������ĭ�� (�κ��丮�� �ִ�) 
	std::vector<InventoryType*> m_pItemObjects;

	// ��
	int m_iMoney;
	std::vector<MoneyNumber*> m_vecMoney;

	AudioSound* m_pEffectSound;

private:
	class Weapon*		pPlayerWeapon;

private:
	// Ŭ���� �κ��丮 ������ ����
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
	// �κ��丮 On, Off
	void InventoryEnable(bool _Enable);
	void InventoryInputKey();
	void InventoryOff();
	void InventoryOff(float _fTime);
	void InventoryOn();

public:
	// �κ��丮 �̵�
	void InventoryPrevPos(float _fTime);
	void InventoryMove(float _fTime);

public:
	// �κ��丮 ��ũ�� ( �ø���, ������ )
	void InventoryRaiseItem(float _fTime);
	void InventoryLiftItem(float _fTime);
	void InventoryCutItem();

	void InventoryBarPrevPos(float _fTime);
	void InventoryBarMove(float _fTime);
	void InventoryBarMoveClickUp(float _fTime);

	void InventoryScrollMoveMouseWheel(float _fTime);

public:
	// ������ �߰� �� ����
	bool CalculateMoney(int _Value);
	bool BuyItem(ITEM_TYPE _eItemType, int _iCount = 0, int _Value = 0);bool AddItem(ITEM_TYPE _eItemType, int _iCount = 0);
	void DeleteItem(int _Index);
	void DeleteClickedItem();								//  Ŭ���� ������ ����(������ �����ϴ°���)
	void ClearItem(int _Index);

	// ������ ���� ������Ʈ
	void UpdateItemCount(int _Index);

	// Ŭ���� �κ��丮 ������ ����
	void SelectItem(float _fTime);
	void UseItem(float _fTime);

private:
	// ���콺 �̹��� ����
	MouseItemSkill*		m_pMouseItemImageScript;

public:
	void SetMouseItemSkillScript(class MouseItemSkill* _Script);

private:
	void UpdateClickItemImage();

	public:
		void UpdateInventorySlot(ITEM_TYPE _eItemType, int _iCount, int _Index);
		void SetClickItemClaer();
};

