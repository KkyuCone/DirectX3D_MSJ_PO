#pragma once
#include "MouseItemSkill.h"
#include "Base.h"
#include "Component/AudioSound.h"
ENGINE_USING

class ShopManager : public Base
{
private:
	bool m_bShopEnable;

private:
	// 상점 이미지들
	// 배경
	GameObject*	m_pShopBGObject;
	Transform*	m_pShopBGTr;
	UIBar*		m_pShopBGImage;

	// 닫기 버튼
	GameObject* m_pCloseBtObject;
	Transform*	m_pCloseBtTr;
	UIButton*	m_pCloseBt;

	// 구매 버튼`1
	GameObject* m_pBuyBtObject;
	Transform*	m_pBuyBtTr;
	UIButton*	m_pBuyBt;

	// 상점 선택버튼들 ( 종류별 )
	std::vector<ShopItemType*>		m_vecShopItemObjects;
	AudioSound* m_pEffectSound;

private:
	// 인벤토리
	class Inventory*	pInventory;

private:
	ShopItemType*			m_pClickShopItemInfo;

public:
	ShopManager();
	ShopManager(const ShopManager& _ShopManager);
	~ShopManager();

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);
	virtual int Render(float _fTime);
	virtual ShopManager* Clone()	const;

public:
	bool GetShopEnable();
	void SetInventory(class Inventory* _Script);

public:
	void ShopEnable(bool _Enable);
	void ShopOff();
	void ShopOff(float _fTime);
	void ShopOn();

private:
	void ClickShopItem_None(float _fTime);
	void ClickShopItem_AW(float _fTime);
	void ClickShopItem_HPPotion(float _fTime);
	void ClickShopTiem_MPPotion(float _fTime);

	void ClickBuyButton(float _fTime);
	void NoneClickBt();
};

