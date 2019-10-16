#include "MapToolScene.h"
#include "Device.h"


MapToolScene::MapToolScene() :
	pMoverObject(nullptr)
	, pMoverTr(nullptr)
	, pMoverScript(nullptr)
	, pEnvsSelObject(nullptr)
	, pEnvsSelTr(nullptr)
	, pEnvsSelScript(nullptr)
	, pMapToolButtonsObj(nullptr)
	, pMapToolButtonsTr(nullptr)
	, pMapToolButtonsScript(nullptr)
	, pLandScapeObject(nullptr)
	, pDefaultLayer(nullptr)
	, pUILayer(nullptr)
	, pMainCameraObject(nullptr)
	, pMoverCameraObject(nullptr)
{
}


MapToolScene::~MapToolScene()
{
	SAFE_RELEASE(pMoverTr);
	SAFE_RELEASE(pMoverScript);
	SAFE_RELEASE(pMoverObject);

	SAFE_RELEASE(pEnvsSelTr);
	SAFE_RELEASE(pEnvsSelScript);
	SAFE_RELEASE(pEnvsSelObject);

	SAFE_RELEASE(pMapToolButtonsTr);
	SAFE_RELEASE(pMapToolButtonsScript);
	SAFE_RELEASE(pMapToolButtonsObj);

	SAFE_RELEASE(pLandScapeObject);
	SAFE_RELEASE(pMainCameraObject);
	SAFE_RELEASE(pMoverCameraObject)
}

bool MapToolScene::Init()
{
	LayerInit();
	LandScapeInit();
	MapToolObjInit();
	TPCameraInit();
	CreateProtoType();
	return true;
}

int MapToolScene::Input(float _fTime)
{
	return 0;
}

int MapToolScene::Update(float _fTime)
{
	return 0;
}

int MapToolScene::LateUpdate(float _fTime)
{
	return 0;
}

int MapToolScene::Collision(float _fTime)
{
	return 0;
}

int MapToolScene::Render(float _fTime)
{
	return 0;
}

void MapToolScene::LayerInit()
{
	pDefaultLayer = m_pScene->FindLayer("Default");
	pUILayer = m_pScene->FindLayer("UI");
}

void MapToolScene::TPCameraInit()
{
	pMainCameraObject = m_pScene->GetMainCameraObject();

	// 플레이어
	//pMoverCameraObject = GameObject::CreateObject("PlayerPivot", pDefaultLayer);
	//pMoverCameraObject->SetParent(pMoverObject);

	TPCamera* pTPCamera = pMainCameraObject->AddComponent<TPCamera>("ThirdPersonCamera");
	pTPCamera->SetMouseEnable(true);
	pTPCamera->SetTarget(pMoverObject);

	pMoverScript->SetTPCam(pTPCamera);

	SAFE_RELEASE(pTPCamera);
}

void MapToolScene::MapToolObjInit()
{
	// Mover
	pMoverObject = GameObject::CreateObject("MapTool_MoverObj", pDefaultLayer);
	pMoverScript = pMoverObject->AddComponent<Mover>("MapTool_MoverScript");
	pMoverTr = pMoverObject->GetTransform();
	pMoverTr->SetWorldPosition(_RESOLUTION.iWidth / 2.f, _RESOLUTION.iHeight, 1.0f);

	// Select & Buttons
	pEnvsSelObject = GameObject::CreateObject("MapTool_EnvSelectManager", pUILayer);
	pEnvsSelScript = pEnvsSelObject->AddComponent<EnvSelectManager>("MapTool_EnvSelectScript");
	pEnvsSelScript->SetMoverScript(pMoverScript);
	pEnvsSelTr = pEnvsSelObject->GetTransform();
	pEnvsSelTr->SetWorldPosition(_RESOLUTION.iWidth / 2.f, _RESOLUTION.iHeight, 1.0f);

	LandScape* pFindLandScape = pLandScapeObject->FindComponentFromType<LandScape>(CT_LANDSCAPE);
	pEnvsSelScript->SetLandScape(pFindLandScape);
	SAFE_RELEASE(pFindLandScape);

	// Buttons
	pMapToolButtonsObj = GameObject::CreateObject("MapTool_ButtonsManager", pUILayer);
	pMapToolButtonsScript = pMapToolButtonsObj->AddComponent<MapToolButtons>("MapTool_ButtonsScript");
	pMapToolButtonsScript->SetEnvSelScript(pEnvsSelScript);
	pMapToolButtonsTr = pMapToolButtonsObj->GetTransform();
	pMapToolButtonsTr->SetWorldPosition(_RESOLUTION.iWidth / 2.f, _RESOLUTION.iHeight, 1.0f);

	pEnvsSelScript->SetMapToolBtScript(pMapToolButtonsScript);

	SAFE_RELEASE(pMoverTr);
	SAFE_RELEASE(pEnvsSelTr);
	SAFE_RELEASE(pMapToolButtonsTr);
}

void MapToolScene::LandScapeInit()
{
	pLandScapeObject = GameObject::CreateObject("LandScape_MapTool", pDefaultLayer);
	pLandScapeObject->SetRenderGroup(RG_LANDSCAPE);

	LandScape*	pLandScape = pLandScapeObject->AddComponent<LandScape>("LandScape");

	pLandScape->CreateLandScape("LandScape", "LandScape/MyHeightMap02.bmp");			// 높이맵 설정


	pLandScape->AddSplatTexture(TEXT("LandScape/BD_Terrain_Cliff05.dds"),
		TEXT("LandScape/BD_Terrain_Cliff05_NRM.bmp"),
		TEXT("LandScape/BD_Terrain_Cliff05_SPEC.bmp"),
		TEXT("LandScape/RoadAlpha1.bmp"));
	pLandScape->AddSplatTexture(TEXT("LandScape/Terrain_Cliff_15_Large.dds"),
		TEXT("LandScape/Terrain_Cliff_15_Large_NRM.bmp"),
		TEXT("LandScape/Terrain_Cliff_15_Large_SPEC.bmp"),
		TEXT("LandScape/SandBaseAlpha.bmp"));
	pLandScape->AddSplatTexture(TEXT("LandScape/Terrain_Pebbles_01.dds"),
		TEXT("LandScape/Terrain_Pebbles_01_NRM.bmp"),
		TEXT("LandScape/Terrain_Pebbles_01_SPEC.bmp"),
		TEXT("LandScape/WaterBaseAlpha.bmp"));

	pLandScape->CreateSplatTexture();

	SAFE_RELEASE(pLandScape);

	return;
}

