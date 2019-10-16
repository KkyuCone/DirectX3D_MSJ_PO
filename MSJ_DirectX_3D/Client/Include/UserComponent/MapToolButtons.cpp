#include "MapToolButtons.h"
#include "EnvSelectManager.h"
#include "Scene\Scene.h"
#include "Scene\SceneManager.h"
#include "Scene\Layer.h"
#include "Render\RenderManager.h"
#include "Device.h"
#include "Time.h"
#include "GameObject.h"
#include "InputManager.h"
#include "PathManager.h"
#include "..\SceneComponent\TitleScene.h"


MapToolButtons::MapToolButtons() :
	m_pNaviBtObj(nullptr), m_pNaviBtTr(nullptr), m_pNaviBtButton(nullptr)
	, m_pStaticBtObj(nullptr), m_pStaticBtTr(nullptr), m_pStaticBtButton(nullptr)
	, m_pAniBtObj(nullptr), m_pAniBtTr(nullptr), m_pAniBtButton(nullptr)
	, m_pSaveBtObj(nullptr), m_pSaveBtTr(nullptr), m_pSaveBtButton(nullptr)
	, m_pLoadBtObj(nullptr), m_pLoadBtTr(nullptr), m_pLoadBtButton(nullptr)
	, m_pEnvSelScript(nullptr)
	, m_pTitleSceneMoveBtObj(nullptr), m_pTitleSceneMoveBtTr(nullptr), m_pTitleSceneMoveBtButton(nullptr)
	, m_pSelectDeleteBtObj(nullptr), m_pSelectDeleteBtTr(nullptr), m_pSelectDeleteBtButton(nullptr)
	, m_pMainSceneBtObj(nullptr), m_pMainSceneBtTr(nullptr), m_pMainSceneBtButton(nullptr)
	, m_pTestSceneBtObj(nullptr), m_pTestSceneBtTr(nullptr), m_pTestSceneBtButton(nullptr)
	, m_pLandBtObj(nullptr), m_pLandBtTr(nullptr), m_pLandBtButton(nullptr)
{
	m_eSLType = SL_NONE;
	m_eSCTType = SCT_NONE;
}

MapToolButtons::MapToolButtons(const MapToolButtons & _MapToolButtons)
{
	*this = _MapToolButtons;
	m_iReferenceCount = 1;
	m_pLayer = _MapToolButtons.m_pLayer;
}


MapToolButtons::~MapToolButtons()
{
	SAFE_RELEASE(m_pNaviBtButton);
	SAFE_RELEASE(m_pNaviBtTr);
	SAFE_RELEASE(m_pNaviBtObj);

	SAFE_RELEASE(m_pStaticBtButton);
	SAFE_RELEASE(m_pStaticBtTr);
	SAFE_RELEASE(m_pStaticBtObj);

	SAFE_RELEASE(m_pAniBtButton);
	SAFE_RELEASE(m_pAniBtTr);
	SAFE_RELEASE(m_pAniBtObj);

	SAFE_RELEASE(m_pSaveBtButton);
	SAFE_RELEASE(m_pSaveBtTr);
	SAFE_RELEASE(m_pSaveBtObj);

	SAFE_RELEASE(m_pLoadBtButton);
	SAFE_RELEASE(m_pLoadBtTr);
	SAFE_RELEASE(m_pLoadBtObj);

	SAFE_RELEASE(m_pTitleSceneMoveBtButton);
	SAFE_RELEASE(m_pTitleSceneMoveBtTr);
	SAFE_RELEASE(m_pTitleSceneMoveBtObj);

	SAFE_RELEASE(m_pSelectDeleteBtButton);
	SAFE_RELEASE(m_pSelectDeleteBtTr);
	SAFE_RELEASE(m_pSelectDeleteBtObj);

	SAFE_RELEASE(m_pMainSceneBtButton);
	SAFE_RELEASE(m_pMainSceneBtTr);
	SAFE_RELEASE(m_pMainSceneBtObj);

	SAFE_RELEASE(m_pTestSceneBtButton);
	SAFE_RELEASE(m_pTestSceneBtTr);
	SAFE_RELEASE(m_pTestSceneBtObj);

	SAFE_RELEASE(m_pLandBtButton);
	SAFE_RELEASE(m_pLandBtTr);
	SAFE_RELEASE(m_pLandBtObj);
}

void MapToolButtons::Start()
{
	CreateBaseBt();				// 기본 버튼 생성 - Nav, Static, Ani, Save, Load
}

bool MapToolButtons::Init()
{
	return true;
}

int MapToolButtons::Input(float _fTime)
{
	return 0;
}

int MapToolButtons::Update(float _fTime)
{
	return 0;
}

int MapToolButtons::LateUpdate(float _fTime)
{
	return 0;
}

int MapToolButtons::Collision(float _fTime)
{
	return 0;
}

int MapToolButtons::PrevRender(float _fTime)
{
	return 0;
}

int MapToolButtons::Render(float _fTime)
{
	return 0;
}

MapToolButtons * MapToolButtons::Clone() const
{
	return new MapToolButtons(*this);
}

void MapToolButtons::SetEnvSelScript(EnvSelectManager * _Script)
{
	m_pEnvSelScript = _Script;
}

void MapToolButtons::CreateBaseBt()
{
	// Nav
	m_pNaviBtObj = GameObject::CreateObject("Tool_BT_Navi", m_pLayer);
	m_pNaviBtButton = m_pNaviBtObj->AddComponent<UIButton>("Tool_BT_Navi");
	m_pNaviBtButton->SetStateTexture(BS_NORMAL, "Tool_BT_Navi_NORMAL", TEXT("MapTool_Button_Nav.png"), PATH_UI_MAPTOOLSCENE);
	m_pNaviBtButton->SetStateColor(BS_CLICK, Vector4(0.8f, 0.8f, 0.8f, 1.0f) );
	m_pNaviBtButton->SetStateColor(BS_PUSH, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	m_pNaviBtButton->SetCallBackFunc(BS_CLICK, this, &MapToolButtons::SetNavBtClick);
	m_pNaviBtButton->SetPushNoneOnlyMouseON(true);

	Transform* pNavBtObjTR = m_pNaviBtObj->GetTransform();
	pNavBtObjTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f - 900.0f, _RESOLUTION.iHeight * 0.5f - 520.0f, 2.0f);
	//pNavBtObjTR->SetWorldScale(286.0f, 106.0f, 1.0f);
	pNavBtObjTR->SetWorldScale(200.2f, 74.2f, 1.0f);
	m_pNaviBtTr = pNavBtObjTR;
	SAFE_RELEASE(pNavBtObjTR);

	// Static
	m_pStaticBtObj = GameObject::CreateObject("Tool_BT_Static", m_pLayer);
	m_pStaticBtButton = m_pStaticBtObj->AddComponent<UIButton>("Tool_BT_Static");
	m_pStaticBtButton->SetStateTexture(BS_NORMAL, "Tool_BT_Static_NORMAL", TEXT("MapTool_Button_Static.png"), PATH_UI_MAPTOOLSCENE);
	m_pStaticBtButton->SetStateColor(BS_CLICK, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	m_pStaticBtButton->SetStateColor(BS_PUSH, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	m_pStaticBtButton->SetCallBackFunc(BS_CLICK, this, &MapToolButtons::SetStaticBtClick);
	m_pStaticBtButton->SetPushNoneOnlyMouseON(true);

	Transform* pStaticBtObjTR = m_pStaticBtObj->GetTransform();
	pStaticBtObjTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f - 650.0f, _RESOLUTION.iHeight * 0.5f - 520.0f, 2.0f);
	pStaticBtObjTR->SetWorldScale(200.2f, 74.2f, 1.0f);
	m_pStaticBtTr = pStaticBtObjTR;
	SAFE_RELEASE(pStaticBtObjTR);

	// Ani
	m_pAniBtObj = GameObject::CreateObject("Tool_BT_Ani", m_pLayer);
	m_pAniBtButton = m_pAniBtObj->AddComponent<UIButton>("Tool_BT_Ani");
	m_pAniBtButton->SetStateTexture(BS_NORMAL, "Tool_BT_Ani_NORMAL", TEXT("MapTool_Button_Ani.png"), PATH_UI_MAPTOOLSCENE);
	m_pAniBtButton->SetStateColor(BS_CLICK, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	m_pAniBtButton->SetStateColor(BS_PUSH, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	m_pAniBtButton->SetCallBackFunc(BS_CLICK, this, &MapToolButtons::SetAniBtClick);
	m_pAniBtButton->SetPushNoneOnlyMouseON(true);

	Transform* pAniBtObjTR = m_pAniBtObj->GetTransform();
	pAniBtObjTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f - 400.0f, _RESOLUTION.iHeight * 0.5f - 520.0f, 2.0f);
	pAniBtObjTR->SetWorldScale(200.2f, 74.2f, 1.0f);
	m_pAniBtTr = pAniBtObjTR;
	SAFE_RELEASE(pAniBtObjTR);

	// Land
	m_pLandBtObj = GameObject::CreateObject("Tool_BT_Land", m_pLayer);
	m_pLandBtButton = m_pLandBtObj->AddComponent<UIButton>("Tool_BT_Land");
	m_pLandBtButton->SetStateTexture(BS_NORMAL, "Tool_BT_Land_NORMAL", TEXT("MapTool_Button_Land.png"), PATH_UI_MAPTOOLSCENE);
	m_pLandBtButton->SetStateColor(BS_CLICK, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	m_pLandBtButton->SetStateColor(BS_PUSH, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	m_pLandBtButton->SetCallBackFunc(BS_CLICK, this, &MapToolButtons::SetLandBtClick);
	m_pLandBtButton->SetPushNoneOnlyMouseON(true);

	Transform* pLandBtObjTR = m_pLandBtObj->GetTransform();
	pLandBtObjTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f - 150.0f, _RESOLUTION.iHeight * 0.5f - 520.0f, 2.0f);
	pLandBtObjTR->SetWorldScale(200.2f, 74.2f, 1.0f);
	m_pLandBtTr = pLandBtObjTR;
	SAFE_RELEASE(pLandBtObjTR);

	// Save
	m_pSaveBtObj = GameObject::CreateObject("Tool_BT_Save", m_pLayer);
	m_pSaveBtButton = m_pSaveBtObj->AddComponent<UIButton>("Tool_BT_Save");
	m_pSaveBtButton->SetStateTexture(BS_NORMAL, "Tool_BT_Save_NORMAL", TEXT("MapTool_Button_Save.png"), PATH_UI_MAPTOOLSCENE);
	m_pSaveBtButton->SetStateColor(BS_CLICK, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	m_pSaveBtButton->SetStateColor(BS_PUSH, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	m_pSaveBtButton->SetCallBackFunc(BS_CLICK, this, &MapToolButtons::ShowSaveBt);
	m_pSaveBtButton->SetPushNoneOnlyMouseON(true);

	Transform* pSaveBtObjTR = m_pSaveBtObj->GetTransform();
	pSaveBtObjTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f + 640.0f, _RESOLUTION.iHeight * 0.5f - 520.0f, 2.0f);
	pSaveBtObjTR->SetWorldScale(143.0f, 53.0f, 1.0f);
	m_pSaveBtTr = pSaveBtObjTR;
	SAFE_RELEASE(pSaveBtObjTR);

	// Load
	m_pLoadBtObj = GameObject::CreateObject("Tool_BT_Load", m_pLayer);
	m_pLoadBtButton = m_pLoadBtObj->AddComponent<UIButton>("Tool_BT_Load");
	m_pLoadBtButton->SetStateTexture(BS_NORMAL, "Tool_BT_Load_NORMAL", TEXT("MapTool_Button_Load.png"), PATH_UI_MAPTOOLSCENE);
	m_pLoadBtButton->SetStateColor(BS_CLICK, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	m_pLoadBtButton->SetStateColor(BS_PUSH, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	m_pLoadBtButton->SetCallBackFunc(BS_CLICK, this, &MapToolButtons::ShowLoadBt);
	m_pLoadBtButton->SetPushNoneOnlyMouseON(true);

	Transform* pLoadBtObjTR = m_pLoadBtObj->GetTransform();
	pLoadBtObjTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f + 810.0f, _RESOLUTION.iHeight * 0.5f - 520.0f, 2.0f);
	pLoadBtObjTR->SetWorldScale(143.0f, 53.0f, 1.0f);
	m_pLoadBtTr = pLoadBtObjTR;
	SAFE_RELEASE(pLoadBtObjTR);

	// Title씬으로 이동 
	m_pTitleSceneMoveBtObj = GameObject::CreateObject("Tool_BT_Title", m_pLayer);
	m_pTitleSceneMoveBtButton = m_pTitleSceneMoveBtObj->AddComponent<UIButton>("Tool_BT_Title");
	m_pTitleSceneMoveBtButton->SetStateTexture(BS_NORMAL, "Tool_BT_Title_NORMAL", TEXT("button_theme16_Main.png"), PATH_UI_MAPTOOLSCENE);
	m_pTitleSceneMoveBtButton->SetStateColor(BS_CLICK, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	m_pTitleSceneMoveBtButton->SetStateColor(BS_PUSH, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	m_pTitleSceneMoveBtButton->SetCallBackFunc(BS_CLICK, this, &MapToolButtons::ChangeTitleScene);
	m_pTitleSceneMoveBtButton->SetPushNoneOnlyMouseON(true);

	Transform* pTitleSceneBtObjTR = m_pTitleSceneMoveBtObj->GetTransform();
	pTitleSceneBtObjTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f + 820.0f, _RESOLUTION.iHeight * 0.5f - 410.0f, 2.0f);
	pTitleSceneBtObjTR->SetWorldScale(118.0f, 114.0f, 1.0f);
	m_pTitleSceneMoveBtTr = pTitleSceneBtObjTR;
	SAFE_RELEASE(pTitleSceneBtObjTR);

	// 지우기 DeleteBtClick
	m_pSelectDeleteBtObj = GameObject::CreateObject("Tool_BT_Delete", m_pLayer);
	m_pSelectDeleteBtButton = m_pSelectDeleteBtObj->AddComponent<UIButton>("Tool_BT_Delete");
	m_pSelectDeleteBtButton->SetStateTexture(BS_NORMAL, "Tool_BT_Delete_NORMAL", TEXT("button_theme16_Delete.png"), PATH_UI_MAPTOOLSCENE);
	m_pSelectDeleteBtButton->SetStateColor(BS_CLICK, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	m_pSelectDeleteBtButton->SetStateColor(BS_PUSH, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	m_pSelectDeleteBtButton->SetCallBackFunc(BS_CLICK, this, &MapToolButtons::DeleteBtClick);
	m_pSelectDeleteBtButton->SetPushNoneOnlyMouseON(true);

	Transform* pSelDeleteBtObjTR = m_pSelectDeleteBtObj->GetTransform();
	pSelDeleteBtObjTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f + 660.0f, _RESOLUTION.iHeight * 0.5f - 410.0f, 2.0f);
	pSelDeleteBtObjTR->SetWorldScale(118.0f, 114.0f, 1.0f);
	m_pSelectDeleteBtTr = pSelDeleteBtObjTR;
	SAFE_RELEASE(pSelDeleteBtObjTR);
	m_pSelectDeleteBtObj->Enable(false);

	// Save & Load시 선택 버튼
	m_pMainSceneBtObj = GameObject::CreateObject("Tool_BT_MainScene", m_pLayer);
	m_pMainSceneBtButton = m_pMainSceneBtObj->AddComponent<UIButton>("Tool_BT_MainScene");
	m_pMainSceneBtButton->SetStateTexture(BS_NORMAL, "Tool_BT_MainScene_NORMAL", TEXT("SaveLoadMainScene.png"), PATH_UI_MAPTOOLSCENE);
	m_pMainSceneBtButton->SetStateColor(BS_CLICK, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	m_pMainSceneBtButton->SetStateColor(BS_PUSH, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	m_pMainSceneBtButton->SetCallBackFunc(BS_CLICK, this, &MapToolButtons::ClickMainSceneBt);
	m_pMainSceneBtButton->SetPushNoneOnlyMouseON(true);

	Transform* pMainSceneBtObjTR = m_pMainSceneBtObj->GetTransform();
	pMainSceneBtObjTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f - 350.0f, _RESOLUTION.iHeight * 0.5f, 2.0f);
	pMainSceneBtObjTR->SetWorldScale(244.0f, 83.0f, 1.0f);
	m_pMainSceneBtTr = pMainSceneBtObjTR;
	SAFE_RELEASE(pMainSceneBtObjTR);
	m_pMainSceneBtObj->Enable(false);

	m_pTestSceneBtObj = GameObject::CreateObject("Tool_BT_TestScene", m_pLayer);
	m_pTestSceneBtButton = m_pTestSceneBtObj->AddComponent<UIButton>("Tool_BT_TestScene");
	m_pTestSceneBtButton->SetStateTexture(BS_NORMAL, "Tool_BT_TestScene_NORMAL", TEXT("SaveLoadTestScene.png"), PATH_UI_MAPTOOLSCENE);
	m_pTestSceneBtButton->SetStateColor(BS_CLICK, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	m_pTestSceneBtButton->SetStateColor(BS_PUSH, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	m_pTestSceneBtButton->SetCallBackFunc(BS_CLICK, this, &MapToolButtons::ClickTestSceneBt);
	m_pTestSceneBtButton->SetPushNoneOnlyMouseON(true);

	Transform* pTestSceneBtObjTR = m_pTestSceneBtObj->GetTransform();
	pTestSceneBtObjTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f + 50.0f, _RESOLUTION.iHeight * 0.5f, 2.0f);
	pTestSceneBtObjTR->SetWorldScale(244.0f, 83.0f, 1.0f);
	m_pTestSceneBtTr = pTestSceneBtObjTR;
	SAFE_RELEASE(pTestSceneBtObjTR);
	m_pTestSceneBtObj->Enable(false);

}

void MapToolButtons::ShowSaveBt(float _fTime)
{
	m_pEnvSelScript->SetbSaveALoad(true);
	m_pMainSceneBtObj->Enable(true);
	m_pTestSceneBtObj->Enable(true);
	m_eSLType = SL_SAVE;
}

void MapToolButtons::ShowLoadBt(float _fTime)
{
	m_pEnvSelScript->SetbSaveALoad(true);
	m_pMainSceneBtObj->Enable(true);
	m_pTestSceneBtObj->Enable(true);
	m_eSLType = SL_LOAD;
}

void MapToolButtons::SaveALoadObjects()
{
	const char*		pPath = GET_SINGLETON(PathManager)->FindPathMultibyte(PATH_DATA_MAPINFO);

	char		strFullPath[MAX_PATH] = {};

	if (nullptr != pPath)
	{
		strcpy_s(strFullPath, pPath);
	}

	char strStaticFullPath[MAX_PATH] = {};
	strcpy_s(strStaticFullPath, strFullPath);
	char strAniFullPath[MAX_PATH] = {};
	strcpy_s(strAniFullPath, strFullPath);
	char strNavFullPath[MAX_PATH] = {};
	strcpy_s(strNavFullPath, strFullPath);

	if (SCT_MAIN == m_eSCTType)
	{

		const char * pFileName = "MainSceneObjInfo_Static.minfo";
		strcat_s(strStaticFullPath, pFileName);

		pFileName = "MainSceneObjInfo_Ani.minfo";
		strcat_s(strAniFullPath, pFileName);

		pFileName = "MainSceneObjInfo_Nav.minfo";
		strcat_s(strNavFullPath, pFileName);

		if (SL_SAVE == m_eSLType)
		{
			m_pEnvSelScript->SaveStatic(strStaticFullPath);
			m_pEnvSelScript->SaveAni(strAniFullPath);
			m_pEnvSelScript->SaveNav(strNavFullPath);
		}
		else if (SL_LOAD == m_eSLType)
		{
			m_pEnvSelScript->LoadStatic(strStaticFullPath);
			m_pEnvSelScript->LoadAni(strAniFullPath);
			m_pEnvSelScript->LoadNav(strNavFullPath);
		}
	}
	else if (SCT_TEST == m_eSCTType)
	{
		const char * pFileName = "TestSceneObjInfo_Static.minfo";
		strcat_s(strStaticFullPath, pFileName);

		pFileName = "TestSceneObjInfo_Ani.minfo";
		strcat_s(strAniFullPath, pFileName);

		pFileName = "TestSceneObjInfo_Nav.minfo";
		strcat_s(strNavFullPath, pFileName);

		if (SL_SAVE == m_eSLType)
		{
			m_pEnvSelScript->SaveStatic(strStaticFullPath);
			m_pEnvSelScript->SaveAni(strAniFullPath);
			m_pEnvSelScript->SaveNav(strNavFullPath);
		}
		else if (SL_LOAD == m_eSLType)
		{
			m_pEnvSelScript->LoadStatic(strStaticFullPath);
			m_pEnvSelScript->LoadAni(strAniFullPath);
			m_pEnvSelScript->LoadNav(strNavFullPath);
		}
	}

	m_pEnvSelScript->SetbSaveALoad(false);
	m_eSLType = SL_NONE;
	m_eSCTType = SCT_NONE;
	m_pMainSceneBtObj->Enable(false);
	m_pTestSceneBtObj->Enable(false);
}

void MapToolButtons::ClickMainSceneBt(float _fTime)
{
	m_eSCTType = SCT_MAIN;
	SaveALoadObjects();
}

void MapToolButtons::ClickTestSceneBt(float _fTime)
{
	m_eSCTType = SCT_TEST;
	SaveALoadObjects();
}


void MapToolButtons::SelDeleteBtEnable(bool _Enable)
{
	m_pSelectDeleteBtObj->Enable(_Enable);
}

void MapToolButtons::SetNavBtClick(float _fTime)
{
	if (nullptr == m_pEnvSelScript)
		return;

	m_pEnvSelScript->SetEnvSelectBtNav(_fTime);
	m_pSelectDeleteBtObj->Enable(true);
}

void MapToolButtons::SetStaticBtClick(float _fTime)
{
	if (nullptr == m_pEnvSelScript)
		return;

	m_pEnvSelScript->SetEnvSelectBtStatic(_fTime);
	m_pSelectDeleteBtObj->Enable(true);
}

void MapToolButtons::SetAniBtClick(float _fTime)
{
	if (nullptr == m_pEnvSelScript)
		return;

	m_pEnvSelScript->SetEnvSelectBtAni(_fTime);
	m_pSelectDeleteBtObj->Enable(true);
}

void MapToolButtons::SetLandBtClick(float _fTime)
{
	if (nullptr == m_pEnvSelScript)
		return;

	m_pEnvSelScript->SetEnvSelectBtLand(_fTime);
	m_pSelectDeleteBtObj->Enable(true);
}

void MapToolButtons::DeleteBtClick(float _fTime)
{
	m_pEnvSelScript->DeleteSelObject(_fTime);
}

void MapToolButtons::ChangeTitleScene(float _fTime)
{
	// 맵에있는 오브젝트들 지워주기
	Scene* pScene = GET_SINGLETON(SceneManager)->CreateScene<TitleScene>();
	GET_SINGLETON(SceneManager)->SetNextScene(pScene);
	GET_SINGLETON(RenderManager)->SetPostEffectEnable(true);
	GET_SINGLETON(RenderManager)->SetShadowCompute(true);
	GET_SINGLETON(RenderManager)->SetSSAOEnable(false);
}
