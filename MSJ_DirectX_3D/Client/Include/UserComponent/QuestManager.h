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
	// ����Ʈâ (�̴�)�̹���
	GameObject* m_pMiniQuestBGObject;
	Transform*	m_pMiniQuestBGTr;
	UIBar*		m_pMiniQuestBGImage;

	// ����Ʈ ����
	GameObject*	m_pContentObject;
	Transform*	m_pContentTr;
	UIText*		m_pContentText;

	// ����Ʈ �����
	GameObject*	m_pProgressObject;
	Transform*	m_pProgressTr;
	UIText*		m_pProgressText;

	// Prev ��ư
	GameObject*	m_pQuestPrevBtObject;
	Transform*	m_pQuestPrevBtTr;
	UIButton*	m_pQuestPrevBt;

	// Next ��ư
	GameObject*	m_pQuestNextBtObject;
	Transform*	m_pQuestNextBtTr;
	UIButton*	m_pQuestNextBt;

	// ����Ʈâ ��� ��ư
	GameObject* m_pQuestMinusBtObject;
	Transform*	m_pQuestMinusBtTr;
	UIButton*	m_pQuestMinusBt;

	// ����Ʈâ Ȯ�� ��ư
	GameObject* m_pQuestPlusBtObject;
	Transform*	m_pQuestPlusBtTr;
	UIButton*	m_pQuestPlusBt;

	// ����Ʈâ��
	GameObject* m_pQuestTitleBGObject;
	Transform*	m_pQuestTitleBGTr;
	UIBar*	m_pQuestTitleBG;

	// ����Ʈ �Ϸ�/�̿Ϸ� ǥ�� �̹���
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
	// �� ��ư�鿡 ���� �Լ�
	void ExpansionBtFunc(float _fTime);
	void DownsizeBtFunc(float _fTime);

public:
	void ChangeQuestText(const char* _strName, int _Row, int _Col, int _Sheet, NPCKind _QuestNPC);
	void ClearQuest(NPCKind _NPC);
};

