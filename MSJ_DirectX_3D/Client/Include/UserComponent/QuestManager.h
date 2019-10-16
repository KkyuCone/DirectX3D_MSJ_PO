#pragma once
#include "Base.h"
#include "Component/AudioSound.h"
ENGINE_USING

class QuestManager : public Base
{
private:
	bool m_bQuestEnable;
	bool m_bCurQuestClear;
	bool m_bClickPlusBt;

	bool m_bGoddessClear;
	bool m_bBoyClear;

private:
	// 퀘스트창 (미니)이미지
	GameObject* m_pMiniQuestBGObject;
	Transform*	m_pMiniQuestBGTr;
	UIBar*		m_pMiniQuestBGImage;

	// 퀘스트 내용
	GameObject*	m_pContentObject;
	Transform*	m_pContentTr;
	UIText*		m_pContentText;

	// 퀘스트 진행률
	GameObject*	m_pProgressObject;
	Transform*	m_pProgressTr;
	UIText*		m_pProgressText;

	// Prev 버튼
	GameObject*	m_pQuestPrevBtObject;
	Transform*	m_pQuestPrevBtTr;
	UIButton*	m_pQuestPrevBt;

	// Next 버튼
	GameObject*	m_pQuestNextBtObject;
	Transform*	m_pQuestNextBtTr;
	UIButton*	m_pQuestNextBt;

	// 퀘스트창 축소 버튼
	GameObject* m_pQuestMinusBtObject;
	Transform*	m_pQuestMinusBtTr;
	UIButton*	m_pQuestMinusBt;

	// 퀘스트창 확대 버튼
	GameObject* m_pQuestPlusBtObject;
	Transform*	m_pQuestPlusBtTr;
	UIButton*	m_pQuestPlusBt;

	// 퀘스트창명
	GameObject* m_pQuestTitleBGObject;
	Transform*	m_pQuestTitleBGTr;
	UIBar*	m_pQuestTitleBG;

	// 퀘스트 완료/미완료 표시 이미지
	GameObject*	m_pQuestClearObject;
	Transform*	m_pQuestClearTr;
	UIBar*		m_pQuestClearImage;

	GameObject*	m_pQuestClearBGObject;
	Transform*	m_pQuestClearBGTr;
	UIBar*		m_pQuestClearBGImage;

	//
	NPCKind eQuestNPCKind;
	AudioSound* m_pEffectSound;

public:
	QuestManager();
	QuestManager(const QuestManager& _QuestManager);
	~QuestManager();

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);
	virtual int Render(float _fTime);
	virtual QuestManager* Clone()	const;

private:
	void ComponentInit();

public:
	NPCKind GetQuestNPC() const;
	bool GetQuestClear(NPCKind _NPC) const;
	void SetQuestClear(bool _Value, NPCKind _NPC);

private:
	// 각 버튼들에 대한 함수
	void ExpansionBtFunc(float _fTime);
	void DownsizeBtFunc(float _fTime);

public:
	void ChangeQuestText(const char* _strName, int _Row, int _Col, int _Sheet, NPCKind _QuestNPC);
	void ClearQuest(NPCKind _NPC);
};

