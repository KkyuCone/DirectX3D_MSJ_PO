#pragma once
#include "Base.h"
#include "Component/AudioSound.h"
ENGINE_USING

struct NPCBtInfo
{
	BtType	eBtType;
	GameObject* pObject;
	Transform*	pTr;
	UIButton*	pButton;

	NPCBtInfo()
	{
		eBtType = BtType::BT_NONE;
		pObject = nullptr;
		pTr = nullptr;
		pButton = nullptr;
	}
};

class NPCTalkBtManager : public Base
{
private:
	bool	m_bNPCButtonEnable;
	NPCKind m_eNPCKind;

private:
	// 다이얼로그 매니저
	class DialogueManager* pDialogueManager;

	// 상점 매니저
	class ShopManager*	pShopManager;

	// 인벤토리
	class Inventory*	pInventory;
	AudioSound* m_pEffectSound;
public:
	NPCTalkBtManager();
	NPCTalkBtManager(const NPCTalkBtManager& _NPCTalkBtManater);
	~NPCTalkBtManager();

private:
	// 대화하기 & 상점, 퀘스트, 나가기 버튼 관리
	std::vector<NPCBtInfo*> m_vecNPCBts;

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);
	virtual int Render(float _fTime);
	virtual NPCTalkBtManager* Clone()	const;

private:
	void ComponentInit();

public:
	bool GetNPCButtonEnable();

	void SetDialogueManager(class DialogueManager* _Manager);
	void SetShopManager(class ShopManager*	_Manager);
	void SetInventory(class Inventory* _Inventory);

public:
	void NPCButtonOff();
	void NPCButtonOff(float _fTime);

	void ShopNPCButtonON(NPCKind _NPC);
	void NoneNPCButtonON(NPCKind _NPC);

	void NPCButtonON(NPCKind _NPCType);

private:
	void ClickNPCBtConversation(float _fTime);
	void ClickNPCBtQuest(float _fTime);
	void ClickNPCBtExit(float _fTime);
	void ClickNPCBtShop(float _fTime);

};

