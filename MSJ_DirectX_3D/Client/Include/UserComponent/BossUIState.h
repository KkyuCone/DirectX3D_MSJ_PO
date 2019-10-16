#pragma once
#include "Component/UserComponent.h"
#include "Component/Transform.h"
#include "Component/UIBar.h"
#include "Component/UIText.h"

ENGINE_USING

class BossUIState : public UserComponent
{
private:
	float m_fPercent;

private:
	GameObject*	m_pBoss;				// 보스몹 형태
	GameObject*	m_pHPBackground;			// HP 배경
	GameObject*	m_pHP;

	UIBar*		m_pBossImage;
	UIBar*		m_pHPBackgorundImage;
	UIBar*		m_pHPImage;

public:
	BossUIState();
	BossUIState(const BossUIState& _BossUIState);
	~BossUIState();

public:
	virtual void Start();		
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);	
	virtual int Render(float _fTime);
	virtual BossUIState* Clone()	const;

private:
	void ComponentInit();

public:
	void  SetPercent(float _fPerecent);
	float GetPercent();

	void BossUIOnOff(bool _Value);
};

