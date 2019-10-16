#pragma once
#include "Base.h"
#include "Component/AudioSound.h"
ENGINE_USING

class DialogueManager : public Base
{
private:
	bool	m_bEmceeQuest;				// 퀘스트가 진행중인지 여부
	bool	m_bCurQuestDialogue;
	bool	m_bCurQuestClear;
	bool	m_bQuestAnswer;
	bool	m_bQuestQnswerOn;
	bool	m_bDialogueEnable;			// 다이얼로그 창을 띄울지 여부
	int		m_iSheetNumber;				// 시트 번호
	int		m_iRowNumber;				// 행 번호
	int		m_iColNumber;				// 열 번호

	NPCKind m_eNPCKind;
	NPCKind m_eQuestNPC;				// 퀘스트를 수락한 NPC
	NPCDialogueKind m_eDK;
	AudioSound* m_pEffectSound;
private:
	// 이름(대화상대명)
	GameObject*		m_pNameObject;
	Transform*		m_pNameTr;
	UIText*			m_pNameText;

	// 내용
	GameObject*		m_pDialogueObject;
	Transform*		m_pDialogueTr;
	UIText*			m_pDilaogueText;

	// 대화상자 배경 이미지
	GameObject*		m_pDialogueBGObject;
	Transform*		m_pDialogueBGTr;
	UIBar*			m_pDialogueBGImage;

	// 이전, 다음 버튼 & 수락, 거절 버튼   -  수락, 거절은 퀘스트일 경우만 적용
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
	// 다이얼로그 창 ON 상태일때 Off시킬 것들
	class QuickSlot* pQuickSlotScript;
	std::list<GameObject*> pPalyerUIStateList;

	// 퀘스트창 
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

