#pragma once
#include "Component\UserComponent.h"
#include "EnvSelectManager.h"
#include "Component\Renderer.h"
#include "Component\Transform.h"
#include "Component\Material.h"
#include "Component\Animation.h"
#include "Component\UIButton.h"
#include "Component\UIBar.h"
#include "Component\UIText.h"

ENGINE_USING

enum SAVELOAD_TYPE
{
	SL_NONE = 0,
	SL_SAVE,
	SL_LOAD,
};

enum SCENE_TYPE
{
	SCT_NONE = 0,
	SCT_MAIN,
	SCT_TEST,
};

class MapToolButtons : public UserComponent
{
private:
	SAVELOAD_TYPE m_eSLType;
	SCENE_TYPE m_eSCTType;

	// 버튼
	GameObject*		m_pNaviBtObj;
	Transform*		m_pNaviBtTr;
	UIButton*		m_pNaviBtButton;

	GameObject*		m_pStaticBtObj;
	Transform*		m_pStaticBtTr;
	UIButton*		m_pStaticBtButton;

	GameObject*		m_pAniBtObj;
	Transform*		m_pAniBtTr;
	UIButton*		m_pAniBtButton;

	GameObject*		m_pLandBtObj;
	Transform*		m_pLandBtTr;
	UIButton*		m_pLandBtButton;

	GameObject*		m_pSaveBtObj;
	Transform*		m_pSaveBtTr;
	UIButton*		m_pSaveBtButton;

	GameObject*		m_pLoadBtObj;
	Transform*		m_pLoadBtTr;
	UIButton*		m_pLoadBtButton;

	GameObject*		m_pTitleSceneMoveBtObj;
	Transform*		m_pTitleSceneMoveBtTr;
	UIButton*		m_pTitleSceneMoveBtButton;

	GameObject*		m_pSelectDeleteBtObj;
	Transform*		m_pSelectDeleteBtTr;
	UIButton*		m_pSelectDeleteBtButton;

	// Save, Load할때 사용할 버튼
	GameObject*		m_pMainSceneBtObj;
	Transform*		m_pMainSceneBtTr;
	UIButton*		m_pMainSceneBtButton;

	GameObject*		m_pTestSceneBtObj;
	Transform*		m_pTestSceneBtTr;
	UIButton*		m_pTestSceneBtButton;

private:
	class EnvSelectManager*	m_pEnvSelScript;			// 스크립트

public:
	MapToolButtons();
	MapToolButtons(const MapToolButtons& _MapToolButtons);
	~MapToolButtons();

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);
	virtual int Render(float _fTime);
	virtual MapToolButtons* Clone()	const;

public:
	void SetEnvSelScript(class EnvSelectManager* _Script);

public:
	void CreateBaseBt();
	void ShowSaveBt(float _fTime);
	void ShowLoadBt(float _fTime);

	void SaveALoadObjects();

	void ClickMainSceneBt(float _fTime);
	void ClickTestSceneBt(float _fTime);

	void SelDeleteBtEnable(bool _Enable);

	void ChangeTitleScene(float _fTime);

private:
	void SetNavBtClick(float _fTime);
	void SetStaticBtClick(float _fTime);
	void SetAniBtClick(float _fTime);
	void SetLandBtClick(float _fTime);

	void DeleteBtClick(float _fTime);
};

