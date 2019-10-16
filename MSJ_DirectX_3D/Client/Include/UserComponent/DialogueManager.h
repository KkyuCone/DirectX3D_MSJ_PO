#pragma once
#include "Base.h"
#include "Component/AudioSound.h"
ENGINE_USING

class DialogueManager : public Base
{
private:
	bool	m_bEmceeQuest;				// ����Ʈ�� ���������� ����
	bool	m_bCurQuestDialogue;
	bool	m_bCurQuestClear;
	bool	m_bQuestAnswer;
	bool	m_bQuestQnswerOn;
	bool	m_bDialogueEnable;			// ���̾�α� â�� ����� ����
	int		m_iSheetNumber;				// ��Ʈ ��ȣ
	int		m_iRowNumber;				// �� ��ȣ
	int		m_iColNumber;				// �� ��ȣ

	NPCKind m_eNPCKind;
	NPCKind m_eQuestNPC;				// ����Ʈ�� ������ NPC
	NPCDialogueKind m_eDK;
	AudioSound* m_pEffectSound;
private:
	// �̸�(��ȭ����)
	GameObject*		m_pNameObject;
	Transform*		m_pNameTr;
	UIText*			m_pNameText;

	// ����
	GameObject*		m_pDialogueObject;
	Transform*		m_pDialogueTr;
	UIText*			m_pDilaogueText;

	// ��ȭ���� ��� �̹���
	GameObject*		m_pDialogueBGObject;
	Transform*		m_pDialogueBGTr;
	UIBar*			m_pDialogueBGImage;

	// ����, ���� ��ư & ����, ���� ��ư   -  ����, ������ ����Ʈ�� ��츸 ����
	GameObject*		m_pPrevBtObject;
	Transform*		m_pPrevBtTr;
	UIButton*		m_pPrevBt;

	GameObject*		m_pNextBtObject;
	Transform*		m_pNextBtTr;
	UIButton*		m_pNextBt;

	GameObject*		m_pYesBtObject;
	Transform*		m_pYesBtTr;
	UIButton*		m_pYestBt;

	GameObject*		m_pNoBtObject;
	Transform*		m_pNoBtTr;
	UIButton*		m_pNoBt;

private:
	// ���̾�α� â ON �����϶� Off��ų �͵�
	class QuickSlot* pQuickSlotScript;
	std::list<GameObject*> pPalyerUIStateList;

	// ����Ʈâ 
	class QuestManager*	pQuestManagerScript;

public:
	DialogueManager();
	DialogueManager(const DialogueManager& _DialogueManager);
	~DialogueManager();

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);
	virtual int Render(float _fTime);
	virtual DialogueManager* Clone()	const;

public:
	void SetQuickSlotScript(class QuickSlot* _pScript);
	void PushPalyerObject(GameObject* _pObject);
	void SetQuestManagerScript(class QuestManager* _pScript);

private:
	void ComponentInit();
	void PlayerStateOn();
	void PlayerStateOff();

public:
	void DialogueOn(NPCKind _NPC, NPCDialogueKind _NDK);
	void DialogueOn(int _Row, int _Col, int _iSheet);
	void DialogueOff();
	void AllDialogueOff();
	void AllDialogueOn();

private:
	void ExcelSheetInit();
	void QuestDialougeOn(NPCKind _NPC);
	void ConversationDialogueOn(NPCKind _NPC);
	void EmceeQuestDialogueOn();
	void QuestClearDialogueOn();

	void PrevBtFunc(float _fTime);
	void NextBtFunc(float _fTime);
	void YesBtFunc(float _fTime);
	void NoBtFunc(float _fTime);
};

