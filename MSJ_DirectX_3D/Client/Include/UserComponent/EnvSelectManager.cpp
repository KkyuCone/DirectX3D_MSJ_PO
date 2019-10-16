#include "EnvSelectManager.h"
#include "Scene\Scene.h"
#include "Scene\Layer.h"
#include "Resource\Mesh.h"
#include "Component\ColliderSphere.h"
#include "Component\ColliderOBB.h"
#include "Device.h"
#include "Time.h"
#include "GameObject.h"
#include "InputManager.h"
#include "CollisionManager.h"
#include "PathManager.h"
#include "Mover.h"
#include "MapToolButtons.h"
#include <io.h>

EnvSelectManager::EnvSelectManager() :
	m_pSelectTopBGObj(nullptr), m_pSelectTopBGTr(nullptr), m_pSelectTopBGImage(nullptr)
	, m_pSelectMdBGObj(nullptr), m_pSelectMdBGTr(nullptr), m_pSelectMdBGImage(nullptr)
	, m_pSelectBotBGObj(nullptr), m_pSelectBotBGTr(nullptr), m_pSelectBotBGImage(nullptr)
	, m_pKindTopLineObj(nullptr), m_pKindTopLineTr(nullptr), m_pKindTopLineImage(nullptr)
	, m_pKindBotLineObj(nullptr), m_pKindBotLineTr(nullptr), m_pKindBotLineImage(nullptr)
	, m_pCloseBtObj(nullptr), m_pCloseBtTr(nullptr), m_pCloseBt(nullptr)
	, m_pPrevPageBtObj(nullptr), m_pPrevPageBtTr(nullptr), m_pPrevPageBt(nullptr)
	, m_pNextPageBtObj(nullptr), m_pNextPageBtTr(nullptr), m_pNextPageBt(nullptr)
	, m_pCurPageTextObj(nullptr), m_pCurPageTextTr(nullptr), m_pCurPageText(nullptr)
	, m_pMaxPageTextObj(nullptr), m_pMaxPageTextTr(nullptr), m_pMaxPageText(nullptr)
	, m_pMoverScript(nullptr)
	, m_bClickPickPossible(false), m_bClickPickCol(false)
	, m_pSelectObject(nullptr)
	, m_pMapToolBtScript(nullptr)
	, m_bSaveALoad(false), m_pCurSceneLand(nullptr)
{
	m_iCurPage = 0;
	m_iMaxPage = MAX_PAGE;
	m_iSelectBtIndex = -1;
	m_eFolderType = FOLDER_TYPE::FOLDER_NONE;
}

EnvSelectManager::EnvSelectManager(const EnvSelectManager & _EnvSelectManager)
{
	*this = _EnvSelectManager;
	m_iReferenceCount = 1;
	m_pLayer = _EnvSelectManager.m_pLayer;
}


EnvSelectManager::~EnvSelectManager()
{
	// 배경
	SAFE_RELEASE(m_pSelectTopBGImage);
	SAFE_RELEASE(m_pSelectTopBGTr);
	SAFE_RELEASE(m_pSelectTopBGObj);

	SAFE_RELEASE(m_pSelectMdBGImage);
	SAFE_RELEASE(m_pSelectMdBGTr);
	SAFE_RELEASE(m_pSelectMdBGObj);

	SAFE_RELEASE(m_pSelectBotBGObj);
	SAFE_RELEASE(m_pSelectBotBGTr);
	SAFE_RELEASE(m_pSelectBotBGImage);

	SAFE_RELEASE(m_pKindTopLineImage);
	SAFE_RELEASE(m_pKindTopLineTr);
	SAFE_RELEASE(m_pKindTopLineObj);

	SAFE_RELEASE(m_pKindBotLineImage);
	SAFE_RELEASE(m_pKindBotLineTr);
	SAFE_RELEASE(m_pKindBotLineObj);

	// 버튼들
	SAFE_RELEASE(m_pCloseBt);
	SAFE_RELEASE(m_pCloseBtTr);
	SAFE_RELEASE(m_pCloseBtObj);

	SAFE_RELEASE(m_pPrevPageBt);
	SAFE_RELEASE(m_pPrevPageBtTr);
	SAFE_RELEASE(m_pPrevPageBtObj);

	SAFE_RELEASE(m_pNextPageBt);
	SAFE_RELEASE(m_pNextPageBtTr);
	SAFE_RELEASE(m_pNextPageBtObj);

	/*for (int i = 0; i < m_vecSelectKindBts.size(); ++i)
	{
		SAFE_RELEASE(m_vecSelectKindBts[i]->m_pSelectKindBt);
		SAFE_RELEASE(m_vecSelectKindBts[i]->m_pSelectKindBtTr);
		SAFE_RELEASE(m_vecSelectKindBts[i]->m_pSelectKindBtObj);
	}*/

	for (int i = 0; i < m_vecMeshSelBts.size(); ++i)
	{
		SAFE_RELEASE(m_vecMeshSelBts[i]->pMeshSelBt);
		SAFE_RELEASE(m_vecMeshSelBts[i]->pMeshSelBtTr);
		SAFE_RELEASE(m_vecMeshSelBts[i]->pMeshSelBtObj);
		SAFE_DELETE(m_vecMeshSelBts[i]);
	}

	m_vecMeshSelBts.clear();

	// 오브젝트 리스트들 지워주기
	std::list<GameObject*>::iterator StartStaticIter = m_StaticObjList.begin();
	std::list<GameObject*>::iterator EndStaticIter = m_StaticObjList.end();

	for (; StartStaticIter != EndStaticIter; ++StartStaticIter)
	{
		SAFE_RELEASE((*StartStaticIter));
	}

	m_StaticObjList.clear();

	std::list<GameObject*>::iterator StartAniIter = m_AniObjList.begin();
	std::list<GameObject*>::iterator EndAniIter = m_AniObjList.end();

	for (; StartAniIter != EndAniIter; ++StartAniIter)
	{
		SAFE_RELEASE((*StartAniIter));
	}

	m_AniObjList.clear();
	m_AniObjEnumList.clear();


	// Axis도 지운다.
	GET_SINGLETON(InputManager)->DeleteAxisKey("Pick_Rot_X", DIK_1);
	GET_SINGLETON(InputManager)->DeleteAxisKey("Pick_Rot_Y", DIK_2);
	GET_SINGLETON(InputManager)->DeleteAxisKey("Pick_Rot_Z", DIK_3);
}

void EnvSelectManager::Start()
{
	CreateSelectBG();
	CreateButtons();
	CreateTexts();

	GetFileDirectory(PATH_UI_MAPTOOLSCENE_STATIC, "*.*", FOLDER_STATIC);
	GetFileDirectory(PATH_UI_MAPTOOLSCENE_ANI, "*.*", FOLDER_ANI);
	GetFileDirectory(PATH_UI_MAPTOOLSCENE_NAV, "*.*", FOLDER_NAV);
	GetFileDirectory(PATH_UI_MAPTOOLSCENE_LAND, "*.*", FOLDER_LAND);
}

bool EnvSelectManager::Init()
{
	InputInit();
	return true;
}

int EnvSelectManager::Input(float _fTime)
{
	return 0;
}

int EnvSelectManager::Update(float _fTime)
{

	return 0;
}

int EnvSelectManager::LateUpdate(float _fTime)
{
	// 클릭 했을때만 생성하도록 한다.
	if (true == GET_SINGLETON(InputManager)->MousePress(MOUSE_BUTTON::MB_LBUTTON))
	{
		if (true == m_bSaveALoad)
		{
			return 0;
		}

		if (nullptr == m_pMoverScript)
		{
			m_bClickPickPossible = false;
			return 0;
		}

		Vector2 vUIMousePos = GET_SINGLETON(InputManager)->GetUIMouseViewportPos();
		float fReturnPos = _RESOLUTION.iWidth * 0.5f + 650.0f;

		// UI와 근접한 지형일 경우엔 지형 위치를 찾지 않기.
		if (fReturnPos < vUIMousePos.x)
		{
			m_bClickPickPossible = false;
			return 0;
		}

		if (true == m_bClickPickPossible)
		{
			// 오른쪽 마우스 클릭 -> 클릭 (피킹가능한 오브젝트 가져오기)
			GameObject* pClickPickObj = GET_SINGLETON(CollisionManager)->GetPickColliderObject();
			m_pSelectObject = pClickPickObj;
			SAFE_RELEASE(pClickPickObj);
			m_bClickPickPossible = false;
		}
		else
		{
			if (-1 == m_iSelectBtIndex)
			{
				return 0;
			}


			// 월드상의 마우스 위치 가져오기
			Vector3 vPos = m_pMoverScript->GetCurWorldMousePos();

			// 클릭된 버튼 종류에 따라 처리
			switch (m_eFolderType)
			{
			case FOLDER_NAV:
				ChangeNavEnable(vPos);					// 플레이어가 갈 수 있는 지형인지 아닌지 지정.
				break;
			case FOLDER_ANI:
			case FOLDER_STATIC:
				CreateEnvObject(vPos);					// FBX 메쉬 생성
				break;
			case FOLDER_LAND:
				break;
			default:
				break;
			}
		}

	}

	return 0;
}

int EnvSelectManager::Collision(float _fTime)
{
	return 0;
}

int EnvSelectManager::PrevRender(float _fTime)
{
	return 0;
}

int EnvSelectManager::Render(float _fTime)
{
	return 0;
}

EnvSelectManager * EnvSelectManager::Clone() const
{
	return new EnvSelectManager(*this);
}

void EnvSelectManager::SetMoverScript(class Mover * _MoverScript)
{
	m_pMoverScript = _MoverScript;
}

void EnvSelectManager::SetMapToolBtScript(class MapToolButtons* _Script)
{
	m_pMapToolBtScript = _Script;
}

void EnvSelectManager::SetLandScape(class LandScape* _LandScape)
{
	m_pCurSceneLand = _LandScape;
}

void EnvSelectManager::InputInit()
{
	// Axis
	GET_SINGLETON(InputManager)->AddAxisKey("Pick_Rot_X", DIK_1, 1.0f);		// 회전 - X
	GET_SINGLETON(InputManager)->AddAxisKey("Pick_Rot_Y", DIK_2, 1.0f);		// 회전 - Y
	GET_SINGLETON(InputManager)->AddAxisKey("Pick_Rot_Z", DIK_3, 1.0f);		// 회전 - Z

	GET_SINGLETON(InputManager)->BindAxis("Pick_Rot_X", this, &EnvSelectManager::RotateX);
	GET_SINGLETON(InputManager)->BindAxis("Pick_Rot_Y", this, &EnvSelectManager::RotateY);
	GET_SINGLETON(InputManager)->BindAxis("Pick_Rot_Z", this, &EnvSelectManager::RotateZ);

	// Action
	GET_SINGLETON(InputManager)->AddActionKey("Pick_Pos_Front", DIK_UP);		// 오브젝트들 위치
	GET_SINGLETON(InputManager)->AddActionKey("Pick_Pos_Back", DIK_DOWN);	
	GET_SINGLETON(InputManager)->AddActionKey("Pick_Pos_Left", DIK_LEFT);	
	GET_SINGLETON(InputManager)->AddActionKey("Pick_Pos_Right", DIK_RIGHT);	
	GET_SINGLETON(InputManager)->AddActionKey("Pick_Pos_Up", DIK_K);
	GET_SINGLETON(InputManager)->AddActionKey("Pick_Pos_Down", DIK_L);

	GET_SINGLETON(InputManager)->AddActionKey("Pick_Scale_Plus", DIK_4);		// 크기 - 전체(크게)
	GET_SINGLETON(InputManager)->AddActionKey("Pick_Scale_Mius", DIK_5);		// 크기 - 전체(작게)

	GET_SINGLETON(InputManager)->AddActionKey("Pick_None", DIK_N);		// 
	GET_SINGLETON(InputManager)->AddActionKey("Pick_On", DIK_M);		// 

	GET_SINGLETON(InputManager)->BindAction("Pick_Pos_Front", KS_PUSH, this, &EnvSelectManager::MoveFront);
	GET_SINGLETON(InputManager)->BindAction("Pick_Pos_Back", KS_PUSH, this, &EnvSelectManager::MoveBack);
	GET_SINGLETON(InputManager)->BindAction("Pick_Pos_Left", KS_PUSH, this, &EnvSelectManager::MoveLeft);
	GET_SINGLETON(InputManager)->BindAction("Pick_Pos_Right", KS_PUSH, this, &EnvSelectManager::MoveRight);
	GET_SINGLETON(InputManager)->BindAction("Pick_Pos_Up", KS_PUSH, this, &EnvSelectManager::MoveUp);
	GET_SINGLETON(InputManager)->BindAction("Pick_Pos_Down", KS_PUSH, this, &EnvSelectManager::MoveDown);

	GET_SINGLETON(InputManager)->BindAction("Pick_Scale_Plus", KS_PUSH, this, &EnvSelectManager::ScaleUp);
	GET_SINGLETON(InputManager)->BindAction("Pick_Scale_Mius", KS_PUSH, this, &EnvSelectManager::ScaleDown);

	GET_SINGLETON(InputManager)->BindAction("Pick_None", KS_PRESS, this, &EnvSelectManager::SelectObj_None);
	GET_SINGLETON(InputManager)->BindAction("Pick_On", KS_PRESS, this, &EnvSelectManager::SelectObj_ON);


}

// 배경 생성
void EnvSelectManager::CreateSelectBG()
{
	// 상단
	m_pSelectTopBGObj = GameObject::CreateObject("MapTool_SelectUpBG", m_pLayer);
	m_pSelectTopBGImage = m_pSelectTopBGObj->AddComponent<UIBar>("MapTool_SelectUpBG");
	m_pSelectTopBGImage->SetTexture("MapTool_SelectUpBG_NORMAL", TEXT("Select_BG_Up.png"), PATH_UI_MAPTOOLSCENE);
	m_pSelectTopBGImage->SetShader(SHADER_UI_ONLY_IMAGE);

	Transform* pSelectUpBGTR = m_pSelectTopBGObj->GetTransform();
	pSelectUpBGTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f + 650.0f, _RESOLUTION.iHeight * 0.5f + 465.0f, 2.0f);
	pSelectUpBGTR->SetWorldScale(300.0f, 67.0f, 1.0f);
	m_pSelectTopBGTr = pSelectUpBGTR;
	SAFE_RELEASE(pSelectUpBGTR);

	// 중간
	m_pSelectMdBGObj = GameObject::CreateObject("MapTool_SelectMdBG", m_pLayer);
	m_pSelectMdBGImage = m_pSelectMdBGObj->AddComponent<UIBar>("MapTool_SelectMdBG");
	m_pSelectMdBGImage->SetTexture("MapTool_SelectMdBG_NORMAL", TEXT("Select_BG_Middle.png"), PATH_UI_MAPTOOLSCENE);
	m_pSelectMdBGImage->SetShader(SHADER_UI_ONLY_IMAGE);

	Transform* pSelectMdBGTR = m_pSelectMdBGObj->GetTransform();
	pSelectMdBGTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f + 650.0f, _RESOLUTION.iHeight * 0.5f - 200.0f, 2.0f);
	pSelectMdBGTR->SetWorldScale(300.0f, 700.0f, 1.0f);
	m_pSelectMdBGTr = pSelectMdBGTR;
	SAFE_RELEASE(pSelectMdBGTR);

	// 하단
	m_pSelectBotBGObj = GameObject::CreateObject("MapTool_SelectDownBG", m_pLayer);
	m_pSelectBotBGImage = m_pSelectBotBGObj->AddComponent<UIBar>("MapTool_SelectDownBG");
	m_pSelectBotBGImage->SetTexture("MapTool_SelectDownBG_NORMAL", TEXT("Select_BG_Down.png"), PATH_UI_MAPTOOLSCENE);
	m_pSelectBotBGImage->SetShader(SHADER_UI_ONLY_IMAGE);

	Transform* pSelectDownBGTR = m_pSelectBotBGObj->GetTransform();
	pSelectDownBGTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f + 650.0f, _RESOLUTION.iHeight * 0.5f - 250.0f, 2.0f);
	pSelectDownBGTR->SetWorldScale(300.0f, 67.0f, 1.0f);
	m_pSelectBotBGTr = pSelectDownBGTR;
	SAFE_RELEASE(pSelectDownBGTR);

	// 상단 라인
	m_pKindTopLineObj = GameObject::CreateObject("MapTool_KindTopLine", m_pLayer);
	m_pKindTopLineImage = m_pKindTopLineObj->AddComponent<UIBar>("MapTool_KindTopLine");
	m_pKindTopLineImage->SetTexture("MapTool_KindTopLine_NORMAL", TEXT("Select_BG_Line.png"), PATH_UI_MAPTOOLSCENE);
	m_pKindTopLineImage->SetShader(SHADER_UI_ONLY_IMAGE);

	Transform* pKindTopTR = m_pKindTopLineObj->GetTransform();
	pKindTopTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f + 653.0f, _RESOLUTION.iHeight * 0.5f + 465.0f, 1.9f);
	pKindTopTR->SetWorldScale(295.0f, 4.0f, 1.0f);
	m_pKindTopLineTr = pKindTopTR;
	SAFE_RELEASE(pKindTopTR);

	// 하단 라인
	m_pKindBotLineObj = GameObject::CreateObject("MapTool_KindBotLine", m_pLayer);
	m_pKindBotLineImage = m_pKindBotLineObj->AddComponent<UIBar>("MapTool_KindBotLine");
	m_pKindBotLineImage->SetTexture("MapTool_KindBotLine_NORMAL", TEXT("Select_BG_Line.png"), PATH_UI_MAPTOOLSCENE);
	m_pKindBotLineImage->SetShader(SHADER_UI_ONLY_IMAGE);

	Transform* pKindBotTR = m_pKindBotLineObj->GetTransform();
	pKindBotTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f + 653.0f, _RESOLUTION.iHeight * 0.5f - 170.0f, 1.9f);
	pKindBotTR->SetWorldScale(295.0f, 4.0f, 1.0f);
	m_pKindBotLineTr = pKindBotTR;
	SAFE_RELEASE(pKindBotTR);
}

void EnvSelectManager::CreateButtons()
{
	// 닫기 버튼
	m_pCloseBtObj = GameObject::CreateObject("MapTool_Select_CloseBt", m_pLayer);
	m_pCloseBt = m_pCloseBtObj->AddComponent<UIButton>("MapTool_Select_CloseBt");
	m_pCloseBt->SetStateTexture(BS_NORMAL,"MapTool_Select_CloseBt_NORMAL", TEXT("button_theme16_round11.png"), PATH_UI_MAPTOOLSCENE);
	m_pCloseBt->SetStateColor(BS_CLICK, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	m_pCloseBt->SetStateColor(BS_PUSH, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	m_pCloseBt->SetCallBackFunc(BS_CLICK, this, &EnvSelectManager::EnvSelectBtOff);
	m_pCloseBt->SetPushNoneOnlyMouseON(true);

	Transform* pCloseBtTR = m_pCloseBtObj->GetTransform();
	pCloseBtTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f + 890.0f, _RESOLUTION.iHeight * 0.5f + 473.0f, 1.9f);
	pCloseBtTR->SetWorldScale(47.2f, 45.6f, 1.0f);
	m_pCloseBtTr = pCloseBtTR;
	SAFE_RELEASE(pCloseBtTR);
	
	// 다음창, 이전창 버튼 
	m_pPrevPageBtObj = GameObject::CreateObject("MapTool_Select_PrevBt", m_pLayer);
	m_pPrevPageBt = m_pPrevPageBtObj->AddComponent<UIButton>("MapTool_Select_PrevBt");
	m_pPrevPageBt->SetStateTexture(BS_NORMAL, "MapTool_Select_PrevBt_NORMAL", TEXT("button_theme16_square16.png"), PATH_UI_MAPTOOLSCENE);
	m_pPrevPageBt->SetStateColor(BS_CLICK, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	m_pPrevPageBt->SetStateColor(BS_PUSH, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	m_pPrevPageBt->SetCallBackFunc(BS_CLICK, this, &EnvSelectManager::PrevBtClick);
	m_pPrevPageBt->SetPushNoneOnlyMouseON(true);

	Transform* pPrevBtTR = m_pPrevPageBtObj->GetTransform();
	pPrevBtTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f + 685.0f, _RESOLUTION.iHeight * 0.5f - 225.0f, 1.9f);
	pPrevBtTR->SetWorldScale(47.2f, 45.6f, 1.0f);
	m_pPrevPageBtTr = pPrevBtTR;
	SAFE_RELEASE(pPrevBtTR);

	m_pNextPageBtObj = GameObject::CreateObject("MapTool_Select_NextBt", m_pLayer);
	m_pNextPageBt = m_pNextPageBtObj->AddComponent<UIButton>("MapTool_Select_NextBt");
	m_pNextPageBt->SetStateTexture(BS_NORMAL, "MapTool_Select_NextBt_NORMAL", TEXT("button_theme16_square17.png"), PATH_UI_MAPTOOLSCENE);
	m_pNextPageBt->SetStateColor(BS_CLICK, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	m_pNextPageBt->SetStateColor(BS_PUSH, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	m_pNextPageBt->SetCallBackFunc(BS_CLICK, this, &EnvSelectManager::NextBtClick);
	m_pNextPageBt->SetPushNoneOnlyMouseON(true);

	Transform* pNextBtTR = m_pNextPageBtObj->GetTransform();
	pNextBtTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f + 870.0f, _RESOLUTION.iHeight * 0.5f - 225.0f, 1.9f);
	pNextBtTR->SetWorldScale(47.2f, 45.6f, 1.0f);
	m_pNextPageBtTr = pNextBtTR;
	SAFE_RELEASE(pNextBtTR);

	// 선택 버튼들
	int iMeshSelBtSize = MESH_SEL_X_MAX * MESH_SEL_Y_MAX;
	m_vecMeshSelBts.resize(iMeshSelBtSize);

	Vector3 vMeshSelBtBasePos = Vector3(_RESOLUTION.iWidth * 0.5f + 670.0f, _RESOLUTION.iHeight * 0.5f + 320.0f, 1.8f);

	for (int x = 0; x < MESH_SEL_X_MAX; ++x)
	{
		Vector3 vBtPos = vMeshSelBtBasePos;
		vBtPos.x += (MESH_SEL_BT_X_LEN* x);

		for (int y = 0; y < MESH_SEL_Y_MAX; ++y)
		{
			Vector3 vSelBtPos = vBtPos;
			vSelBtPos.y -= (MESH_SEL_BT_Y_LEN * y);

			int iElement = (MESH_SEL_Y_MAX * x) + y;
			MeshSelectBt* pNewMeshSelBt = new MeshSelectBt;
			m_vecMeshSelBts[iElement] = pNewMeshSelBt;

			GameObject* pBtObj = GameObject::CreateObject("MapTool_Select_MeshEmptyBt", m_pLayer);
			UIButton* pUIBt = pBtObj->AddComponent<UIButton>("MapTool_Select_MeshEmptyBt");
			pUIBt->SetStateTexture(BS_NORMAL, "MapTool_Select_MeshEmptyBt_NORMAL", TEXT("MeshSelBt_Base.png"), PATH_UI_MAPTOOLSCENE);
			//pUIBt->SetStateTexture(BS_NORMAL, "MapTool_Select_MeshEmptyBt_NORMAL", TEXT("MeshSelBt_Arcdeva_Gate.png"), PATH_UI_MAPTOOLSCENE_ANI);
			pUIBt->SetStateColor(BS_NORMAL, Vector4(0.95f, 0.95f, 0.95f, 0.95f));
			pUIBt->SetStateColor(BS_CLICK, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
			pUIBt->SetStateColor(BS_PUSH, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
			pUIBt->SetCallBackFunc(BS_CLICK, this, &EnvSelectManager::SetSelectIndex);
			pUIBt->SetPushNoneOnlyMouseON(true);

			Transform* pBtTr = pBtObj->GetTransform();
			pBtTr->SetWorldPosition(vSelBtPos.x, vSelBtPos.y, vSelBtPos.z);
			pBtTr->SetWorldScale(118.0f, 114.0f, 1.0f);

			pNewMeshSelBt->pMeshSelBtObj = pBtObj;
			pNewMeshSelBt->pMeshSelBt = pUIBt;
			pNewMeshSelBt->pMeshSelBtTr = pBtTr;

			SAFE_RELEASE(pBtTr);
		}
	}

}

void EnvSelectManager::CreateTexts()
{
}

void EnvSelectManager::CreateEnvObject(Vector3 _Pos)
{
	// 스태틱과 애니메이션 경우일때만 맞는 인덱스에 따른 오브젝트 생성
	// 선택된 경우가 없다면 return + UI위치도 

	std::string strFileName = "";
	GameObject*	pNewGameObject = nullptr;
	Renderer*	pNewObjectRenderer = nullptr;
	Transform*	pNewObjectTr = nullptr;
	ColliderSphere* pPickCol = nullptr;
	ColliderSphere*	 pNewObjCol = nullptr;
	SphereInfo tGetInfo = {};

	if (FOLDER_TYPE::FOLDER_STATIC == m_eFolderType)
	{
		// Static
		if (m_iSelectBtIndex >= m_vecDirectoryStaticName.size())
			return;

		MapTool_Type_Static eMTS = (MapTool_Type_Static)m_iSelectBtIndex;

		switch (eMTS)
		{
		case MTS_EX01_Adventure_Floor:
			// 바닥
			pNewGameObject = GameObject::CreateClone("EX01_Adventure_Floor", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_EX01_Door_01_SM:
			pNewGameObject = GameObject::CreateClone("EX01_Door_01_SM", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_EX01_Foliage_JUG_Mush_02_SM:
			pNewGameObject = GameObject::CreateClone("EX01_Foliage_JUG_Mush_02_SM", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_EX01_Foliage_JUG_Mush_03_SM:
			pNewGameObject = GameObject::CreateClone("EX01_Foliage_JUG_Mush_03_SM", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_EX01_Foliage_JUG_Small_02_SM:
			// 클로버
			pNewGameObject = GameObject::CreateClone("EX01_Foliage_JUG_Small_02_SM", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_Grass1:
			pNewGameObject = GameObject::CreateClone("Grass1", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_GWall0:
			pNewGameObject = GameObject::CreateClone("GWall0", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_GWall1:
			pNewGameObject = GameObject::CreateClone("GWall1", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_GWall2:
			// 벽
			pNewGameObject = GameObject::CreateClone("GWall2", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_GWall3:
			pNewGameObject = GameObject::CreateClone("GWall3", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_GWall4:
			pNewGameObject = GameObject::CreateClone("GWall4", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_GWall5:
			pNewGameObject = GameObject::CreateClone("GWall5", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_GWall6:
			// 기둥
			pNewGameObject = GameObject::CreateClone("GWall6", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_RockB1:
			pNewGameObject = GameObject::CreateClone("RockB1", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_RockB12:
			pNewGameObject = GameObject::CreateClone("RockB12", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_RockB2:
			pNewGameObject = GameObject::CreateClone("RockB2", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_ST_Angel:
			// 조각상
			pNewGameObject = GameObject::CreateClone("ST_Angel", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_Tree0:
			pNewGameObject = GameObject::CreateClone("Tree0", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_Tree1:
			pNewGameObject = GameObject::CreateClone("Tree1", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_Tree4:
			pNewGameObject = GameObject::CreateClone("Tree4", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_EX01_Balder_Floor01:
			pNewGameObject = GameObject::CreateClone("EX01_Balder_Floor01", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_EX01_Balder_Floor02:
			pNewGameObject = GameObject::CreateClone("EX01_Balder_Floor02", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_EX01_Balder_Gate02:
			pNewGameObject = GameObject::CreateClone("EX01_Balder_Gate02", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_EX01_Balder_Gate03:
			pNewGameObject = GameObject::CreateClone("EX01_Balder_Gate03", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_Ex01_Balder_Maze01:
			pNewGameObject = GameObject::CreateClone("Ex01_Balder_Maze01", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_Ex01_Balder_Maze02:
			pNewGameObject = GameObject::CreateClone("Ex01_Balder_Maze02", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_Ex01_Balder_Maze03:
			pNewGameObject = GameObject::CreateClone("Ex01_Balder_Maze03", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_Ex01_Balder_Maze04:
			pNewGameObject = GameObject::CreateClone("Ex01_Balder_Maze04", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_EX01_Balder_Obj01:
			pNewGameObject = GameObject::CreateClone("EX01_Balder_Obj01", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_EX01_Balder_Obj02:
			pNewGameObject = GameObject::CreateClone("EX01_Balder_Obj02", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_EX01_Balder_Obj03:
			pNewGameObject = GameObject::CreateClone("EX01_Balder_Obj03", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_EX01_Balder_Pillar01:
			pNewGameObject = GameObject::CreateClone("EX01_Balder_Pillar01", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_EX01_Balder_Wall01:
			pNewGameObject = GameObject::CreateClone("EX01_Balder_Wall01", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_EX01_Balder_Wall02:
			pNewGameObject = GameObject::CreateClone("EX01_Balder_Wall02", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_EX01_Balder_Wall04:
			pNewGameObject = GameObject::CreateClone("EX01_Balder_Wall04", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_EX01_Balder_Wall05:
			pNewGameObject = GameObject::CreateClone("EX01_Balder_Wall05", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_Z_VD_Ev_01:
			pNewGameObject = GameObject::CreateClone("VD_Ev_01", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_Z_VD_F1_01:
			pNewGameObject = GameObject::CreateClone("VD_F1_01", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_Z_VD_F2_01:
			pNewGameObject = GameObject::CreateClone("VD_F2_01", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_Z_VD_F3_01:
			pNewGameObject = GameObject::CreateClone("VD_F3_01", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_Z_VD_F4_01:
			pNewGameObject = GameObject::CreateClone("VD_F4_01", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_Z_VD_Roof_01:
			pNewGameObject = GameObject::CreateClone("VD_Roof_01", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_Z_WhiteTree01:
			pNewGameObject = GameObject::CreateClone("WhiteTree01", m_pLayer->GetScene(), m_pLayer);
			break;
		case MTS_Z_WhiteTree02:
			pNewGameObject = GameObject::CreateClone("WhiteTree02", m_pLayer->GetScene(), m_pLayer);
			break;
		default:
			break;
		}

		if (nullptr == pNewGameObject)
		{
			m_pSelectObject = nullptr;
			return;
		}

		pNewObjectTr = pNewGameObject->GetTransform();
		pNewObjectTr->SetWorldPosition(_Pos);

		switch (eMTS)
		{
		case MTS_Ex01_Balder_Maze03:
			pNewObjectTr->SetWorldScale(0.01f, 0.01f, 0.01f);
			break;
		case MTS_Ex01_Balder_Maze04:
			pNewObjectTr->SetWorldScale(0.01f, 0.01f, 0.01f);
			pNewObjectTr->SetWorldRotateY(90.0f);
			break;
		default:
			break;
		}

		pNewObjectRenderer = pNewGameObject->FindComponentFromType<Renderer>(CT_RENDERER);
		pPickCol = pNewGameObject->FindComponentFromTag<ColliderSphere>("PickSphere");
		pNewObjCol = pNewGameObject->AddComponent<ColliderSphere>("Col_SphereNewObj");
		pNewObjCol->SetProfile("WorldStatic");
		pNewObjCol->SetChannel("WorldStatic");
		tGetInfo = pPickCol->GetInfo();
		tGetInfo = pNewObjectRenderer->GetPickSphereInfo();
		pNewObjCol->SetSphereInfo(tGetInfo.vCenter, tGetInfo.fRadius);

		m_StaticObjList.push_back(pNewGameObject);

	}
	else if (FOLDER_TYPE::FOLDER_ANI == m_eFolderType)
	{
		// Ani
		if (m_iSelectBtIndex >= m_vecDirectoryAniName.size())
			return;

		MapTool_Type_Ani eMTA = (MapTool_Type_Ani)m_iSelectBtIndex;
		Animation* pNewObjectAni = nullptr;

		switch (eMTA)
		{
		case MTA_Arcdeva_Gate:
			// 문
			pNewGameObject = GameObject::CreateClone("Arcdeva_Gate", m_pLayer->GetScene(), m_pLayer);
			pNewObjectAni = pNewGameObject->FindComponentFromType<Animation>(CT_ANIMATION);
			pNewObjectAni->ChangeClip("Open");
			break;
		case MTA_Kumas_Mother:
			pNewGameObject = GameObject::CreateClone("Kumas_Mother", m_pLayer->GetScene(), m_pLayer);
			pNewObjectAni = pNewGameObject->FindComponentFromType<Animation>(CT_ANIMATION);
			pNewObjectAni->ChangeClip("Groggy");
			break;
		case MTA_EnchantedDoll:
			pNewGameObject = GameObject::CreateClone("EnchantedDoll", m_pLayer->GetScene(), m_pLayer);
			pNewObjectAni = pNewGameObject->FindComponentFromType<Animation>(CT_ANIMATION);
			pNewObjectAni->ChangeClip("Wait");
			break;
		case MTA_HookaFootman_A:
			pNewGameObject = GameObject::CreateClone("HookaFootman_A", m_pLayer->GetScene(), m_pLayer);
			pNewObjectAni = pNewGameObject->FindComponentFromType<Animation>(CT_ANIMATION);
			pNewObjectAni->ChangeClip("Wait");
			break;
		case MTA_FloatingCastle:
			// 성
			pNewGameObject = GameObject::CreateClone("FloatingCastle", m_pLayer->GetScene(), m_pLayer);
			pNewObjectAni = pNewGameObject->FindComponentFromType<Animation>(CT_ANIMATION);
			pNewObjectAni->ChangeClip("Idle");
			break;
		case MTA_Kumas_Baby_Blue:
			pNewGameObject = GameObject::CreateClone("Kumas_Baby_Blue", m_pLayer->GetScene(), m_pLayer);
			pNewObjectAni = pNewGameObject->FindComponentFromType<Animation>(CT_ANIMATION);
			pNewObjectAni->ChangeClip("Groggy");
			break;
		case MTA_Kumas_Baby_Pink:
			pNewGameObject = GameObject::CreateClone("Kumas_Baby_Pink", m_pLayer->GetScene(), m_pLayer);
			pNewObjectAni = pNewGameObject->FindComponentFromType<Animation>(CT_ANIMATION);
			pNewObjectAni->ChangeClip("Groggy");
			break;
		case MTA_Kumas_Baby_Red:
			pNewGameObject = GameObject::CreateClone("Kumas_Baby_Red", m_pLayer->GetScene(), m_pLayer);
			pNewObjectAni = pNewGameObject->FindComponentFromType<Animation>(CT_ANIMATION);
			pNewObjectAni->ChangeClip("Groggy");
			break;
		case MTA_Goat:
			//염소
			pNewGameObject = GameObject::CreateClone("Goat", m_pLayer->GetScene(), m_pLayer);
			pNewObjectAni = pNewGameObject->FindComponentFromType<Animation>(CT_ANIMATION);
			pNewObjectAni->ChangeClip("Idle01");
			break;
		case MTA_Highelf_M_Alchemist:
			pNewGameObject = GameObject::CreateClone("Highelf_M_Alchemist", m_pLayer->GetScene(), m_pLayer);
			pNewObjectAni = pNewGameObject->FindComponentFromType<Animation>(CT_ANIMATION);
			pNewObjectAni->ChangeClip("Wait");
			break;
		case MTA_Human_F_Aide:
			pNewGameObject = GameObject::CreateClone("Human_F_Aide", m_pLayer->GetScene(), m_pLayer);
			pNewObjectAni = pNewGameObject->FindComponentFromType<Animation>(CT_ANIMATION);
			pNewObjectAni->ChangeClip("Wait");
			break;
		case MTA_Popori_boy:
			pNewGameObject = GameObject::CreateClone("Popori_boy", m_pLayer->GetScene(), m_pLayer);
			pNewObjectAni = pNewGameObject->FindComponentFromType<Animation>(CT_ANIMATION);
			pNewObjectAni->ChangeClip("Idle");
			break;
		case MTA_Popori_boy02:
			// 애기포포리2
			pNewGameObject = GameObject::CreateClone("Popori_boy02", m_pLayer->GetScene(), m_pLayer);
			pNewObjectAni = pNewGameObject->FindComponentFromType<Animation>(CT_ANIMATION);
			pNewObjectAni->ChangeClip("Idle");
			break;
		case MTA_Popori_boy03:
			pNewGameObject = GameObject::CreateClone("Popori_boy03", m_pLayer->GetScene(), m_pLayer);
			pNewObjectAni = pNewGameObject->FindComponentFromType<Animation>(CT_ANIMATION);
			pNewObjectAni->ChangeClip("Idle");
			break;
		case MTA_Q_Mistel:
			pNewGameObject = GameObject::CreateClone("Q_Mistel", m_pLayer->GetScene(), m_pLayer);
			pNewObjectAni = pNewGameObject->FindComponentFromType<Animation>(CT_ANIMATION);
			pNewObjectAni->ChangeClip("Talk");
			break;
		case MTA_Rabbit:
			pNewGameObject = GameObject::CreateClone("Rabbit", m_pLayer->GetScene(), m_pLayer);
			pNewObjectAni = pNewGameObject->FindComponentFromType<Animation>(CT_ANIMATION);
			pNewObjectAni->ChangeClip("Idle01");
			break;
		}

		if (nullptr == pNewGameObject)
		{
			m_pSelectObject = nullptr;
			return;
		}

		pNewObjectTr = pNewGameObject->GetTransform();
		pNewObjectTr->SetWorldPosition(_Pos);
		pNewObjectRenderer = pNewGameObject->FindComponentFromType<Renderer>(CT_RENDERER);
		pPickCol = pNewGameObject->FindComponentFromTag<ColliderSphere>("PickSphere");
		tGetInfo = pNewObjectRenderer->GetPickSphereInfo();

		m_AniObjList.push_back(pNewGameObject);
		m_AniObjEnumList.push_back((int)(eMTA));

		SAFE_RELEASE(pNewObjectAni);
	}

	m_pSelectObject = pNewGameObject;
	SAFE_RELEASE(pPickCol);
	SAFE_RELEASE(pNewObjCol);
	SAFE_RELEASE(pNewObjectTr);
	SAFE_RELEASE(pNewObjectRenderer);
}

std::vector<std::string> EnvSelectManager::GetFileDirectory(std::string _Path, std::string _Filter, FOLDER_TYPE _eType)
{
	// 해당 폴더내 전체 파일 명 가져오기
	std::string PathFilter = _Path;

	const TCHAR* FullPath = GET_SINGLETON(PathManager)->FindPath(PathFilter);
	std::string searching = TCHARToString(FullPath) + _Filter;
	std::vector<std::string> return_;

	_finddata_t fd;
	intptr_t handle = _findfirst(searching.c_str(), &fd);  //현재 폴더 내 모든 파일을 찾는다.

	if (handle == -1)    return return_;

	int result = 0;
	do
	{
		std::string strFileName = fd.name;
		if (strFileName == "." || strFileName == "..")
		{

		}
		else
		{
			return_.push_back(fd.name);
		}
		result = _findnext(handle, &fd);
	} while (result != -1);

	switch (_eType)
	{
	case FOLDER_NAV:
		m_vecDirectoryNavName.clear();
		m_vecDirectoryNavName.resize(return_.size());

		for (int i = 0; i < return_.size(); ++i)
		{
			m_vecDirectoryNavName[i] = return_[i];
		}
		break;
	case FOLDER_ANI:
		m_vecDirectoryAniName.clear();
		m_vecDirectoryAniName.resize(return_.size());

		for (int i = 0; i < return_.size(); ++i)
		{
			m_vecDirectoryAniName[i] = return_[i];
		}
		break;
	case FOLDER_STATIC:
		m_vecDirectoryStaticName.clear();
		m_vecDirectoryStaticName.resize(return_.size());

		for (int i = 0; i < return_.size(); ++i)
		{
			m_vecDirectoryStaticName[i] = return_[i];
		}
		break;
	case FOLDER_LAND:
		m_vecDirectoryLandName.clear();
		m_vecDirectoryLandName.resize(return_.size());

		for (int i = 0; i < return_.size(); ++i)
		{
			m_vecDirectoryLandName[i] = return_[i];
		}
		break;
	default:
		break;
	}

	_findclose(handle);

	return return_;
}

void EnvSelectManager::MeshSelectBtMoveEnable()
{
	for (int i = 0; i < m_vecMeshSelBts.size(); ++i)
	{
		Vector3 vCurPos = m_vecMeshSelBts[i]->pMeshSelBtTr->GetWorldPosition();
		float vMinXPos = (_RESOLUTION.iWidth * 0.5f + 670.0f);
		float vMaxXPos = (_RESOLUTION.iWidth * 0.5f + 670.0f) + MESH_SEL_BT_X_LEN;

		if (vCurPos.x < vMinXPos || vCurPos.x > vMaxXPos)
		{
			m_vecMeshSelBts[i]->pMeshSelBtObj->Enable(false);
		}
		else
		{
			m_vecMeshSelBts[i]->pMeshSelBtObj->Enable(true);
		}
	}
}

void EnvSelectManager::ChangeNavEnable(Vector3 _vPos)
{
	// 지형이 생성되어있지 않을 경우 return.
	if (nullptr == m_pCurSceneLand)
	{
		return;
	}

	if (0 == m_iSelectBtIndex)
	{
		// False, 이동 불가능한 지형 설정
		m_pCurSceneLand->ChangeNavPosEnable(false, _vPos);
	}
	else if (1 == m_iSelectBtIndex)
	{
		// True, 이동 가능한 지형 설정
		m_pCurSceneLand->ChangeNavPosEnable(true, _vPos);
	}
}

void EnvSelectManager::EnvSelectBtOn(float _fTime)
{
	// 켜기
	m_pSelectTopBGObj->Enable(true);
	m_pSelectMdBGObj->Enable(true);
	m_pSelectBotBGObj->Enable(true);

	m_pKindTopLineObj->Enable(true);
	m_pKindBotLineObj->Enable(true);

	// 버튼들
	m_pCloseBtObj->Enable(true);
	m_pPrevPageBtObj->Enable(true);
	m_pNextPageBtObj->Enable(true);

	for (int i = 0; i < m_vecMeshSelBts.size(); ++i)
	{
		m_vecMeshSelBts[i]->pMeshSelBtObj->Enable(true);
	}
}

void EnvSelectManager::EnvSelectBtOff(float _fTime)
{
	// 끄기
	m_eFolderType = FOLDER_TYPE::FOLDER_NONE;
	m_iSelectBtIndex = -1;

	// 배경
	m_pSelectTopBGObj->Enable(false);
	m_pSelectMdBGObj->Enable(false);
	m_pSelectBotBGObj->Enable(false);

	m_pKindTopLineObj->Enable(false);
	m_pKindBotLineObj->Enable(false);

	// 버튼들
	m_pCloseBtObj->Enable(false);
	m_pPrevPageBtObj->Enable(false);
	m_pNextPageBtObj->Enable(false);

	for (int i = 0; i < m_vecMeshSelBts.size(); ++i)
	{
		m_vecMeshSelBts[i]->pMeshSelBtObj->Enable(false);
	}

	//
	if (nullptr != m_pMapToolBtScript)
	{
		m_pMapToolBtScript->SelDeleteBtEnable(false);
	}
}

void EnvSelectManager::SetEnvSelectBtNav(float _fTime)
{
	EnvSelectBtOn(_fTime);
	SetEnvSelectBtNavImage();
}

void EnvSelectManager::SetEnvSelectBtNavImage()
{
	m_eFolderType = FOLDER_NAV;
	SetEnvSelectBtBaseImage(FOLDER_NAV);
}

void EnvSelectManager::SetEnvSelectBtLand(float _fTime)
{
	EnvSelectBtOn(_fTime);
	SetEnvSelectBtLandImage();
}

void EnvSelectManager::SetEnvSelectBtLandImage()
{
	m_eFolderType = FOLDER_LAND;
	SetEnvSelectBtBaseImage(FOLDER_LAND);
}

void EnvSelectManager::SetEnvSelectBtStatic(float _fTime)
{
	EnvSelectBtOn(_fTime);
	SetEnvSelectBtStaticImage();
}

void EnvSelectManager::SetEnvSelectBtStaticImage()
{
	m_eFolderType = FOLDER_STATIC;
	SetEnvSelectBtBaseImage(FOLDER_STATIC);
}

void EnvSelectManager::SetEnvSelectBtAni(float _fTime)
{
	EnvSelectBtOn(_fTime);
	SetEnvSelectBtAniImage();
}

void EnvSelectManager::SetEnvSelectBtAniImage()
{
	m_eFolderType = FOLDER_ANI;
	SetEnvSelectBtBaseImage(FOLDER_ANI);
}

void EnvSelectManager::SetEnvSelectBtBaseImage(FOLDER_TYPE _eType)
{
	// 기본 이미지들로 셋팅
	int iRemainder = (int)(m_vecMeshSelBts.size());

	for (int i = 0; i < m_vecMeshSelBts.size(); ++i)
	{
		m_vecMeshSelBts[i]->pMeshSelBtObj->Enable(true);
		m_vecMeshSelBts[i]->pMeshSelBt->SetStateTexture(BS_NORMAL, "MapTool_Select_MeshEmptyBt_NORMAL", TEXT("MeshSelBt_Base.png"), PATH_UI_MAPTOOLSCENE);
	}

	switch (_eType)
	{
	case FOLDER_NAV:
		iRemainder -= (int)(m_vecDirectoryNavName.size());

		for (int i = 0; i < m_vecDirectoryNavName.size(); ++i)
		{
			std::string strFileName = m_vecDirectoryNavName[i];
			std::wstring wstrFileName = StringToLPWSTR(strFileName);

			m_vecMeshSelBts[i]->pMeshSelBt->SetStateTexture(BS_NORMAL,
				strFileName, wstrFileName.c_str(), PATH_UI_MAPTOOLSCENE_NAV);
		}
		break;
	case FOLDER_ANI:
		iRemainder -= (int)(m_vecDirectoryAniName.size());

		for (int i = 0; i < m_vecDirectoryAniName.size(); ++i)
		{
			std::string strFileName = m_vecDirectoryAniName[i];
			std::wstring wstrFileName = StringToLPWSTR(strFileName);

			m_vecMeshSelBts[i]->pMeshSelBt->SetStateTexture(BS_NORMAL,
				strFileName, wstrFileName.c_str(), PATH_UI_MAPTOOLSCENE_ANI);
		}
		break;
	case FOLDER_STATIC:
		iRemainder -= (int)(m_vecDirectoryStaticName.size());

		for (int i = 0; i < m_vecDirectoryStaticName.size(); ++i)
		{
			std::string strFileName = m_vecDirectoryStaticName[i];
			std::wstring wstrFileName = StringToLPWSTR(strFileName);

			m_vecMeshSelBts[i]->pMeshSelBt->SetStateTexture(BS_NORMAL,
				strFileName, wstrFileName.c_str(), PATH_UI_MAPTOOLSCENE_STATIC);
		}
		break;
	case FOLDER_LAND:
		iRemainder -= (int)(m_vecDirectoryLandName.size());

		for (int i = 0; i < m_vecDirectoryLandName.size(); ++i)
		{
			std::string strFileName = m_vecDirectoryLandName[i];
			std::wstring wstrFileName = StringToLPWSTR(strFileName);

			m_vecMeshSelBts[i]->pMeshSelBt->SetStateTexture(BS_NORMAL,
				strFileName, wstrFileName.c_str(), PATH_UI_MAPTOOLSCENE_NAV);
		}
		break;
	default:
		break;
	}
	//iRemainder
	int iStart = (int)(m_vecMeshSelBts.size()) - iRemainder;

	for (int i = iStart; i < m_vecMeshSelBts.size(); ++i)
	{
		m_vecMeshSelBts[i]->pMeshSelBt->SetStateTexture(BS_NORMAL, "MapTool_Select_MeshEmptyBt_NORMAL", TEXT("MeshSelBt_Base.png"), PATH_UI_MAPTOOLSCENE);
	}

	MeshSelectBtMoveEnable();

}

void EnvSelectManager::PrevBtClick(float _fTime)
{
	--m_iCurPage;

	if (m_iCurPage <= 0)
	{
		m_iCurPage = 1;
		return;
	}

	for (int i = 0; i < m_vecMeshSelBts.size(); ++i)
	{
		Vector3 vMove = Vector3::Zero;
		//vMove.x -= (118.0f + 145.0f);
		vMove.x += 290.0f;
		m_vecMeshSelBts[i]->pMeshSelBtObj->Enable(true);
		m_vecMeshSelBts[i]->pMeshSelBtTr->Move(vMove);
	}

	MeshSelectBtMoveEnable();
}

void EnvSelectManager::NextBtClick(float _fTime)
{
	++m_iCurPage;

	if (m_iCurPage > MAX_PAGE)
	{
		m_iCurPage = MAX_PAGE;
		return;
	}

	for (int i = 0; i < m_vecMeshSelBts.size(); ++i)
	{
		Vector3 vMove = Vector3::Zero;
		//vMove.x += (118.0f + 145.0f);
		vMove.x -= 290.0f;
		m_vecMeshSelBts[i]->pMeshSelBtObj->Enable(true);
		m_vecMeshSelBts[i]->pMeshSelBtTr->Move(vMove);
	}

	MeshSelectBtMoveEnable();
}

void EnvSelectManager::SetSelectIndex(float _fTime)
{
	// 선택된 버튼 인덱스 찾기
	for (int i = 0; i < m_vecMeshSelBts.size(); ++i)
	{
		if (BS_PUSH == m_vecMeshSelBts[i]->pMeshSelBt->GetButtonState())
		{
			m_iSelectBtIndex = i;
			return;
		}
	}

	m_iSelectBtIndex = -1;
	return;
}

void EnvSelectManager::SetbSaveALoad(bool _Enable)
{
	m_bSaveALoad = _Enable;
}

void EnvSelectManager::DeleteSelObject(float _fTime)
{
	if (nullptr == m_pSelectObject)
	{
		return;
	}

	// 스태틱 먼저 찾기
	std::list<GameObject*>::iterator StartIter = m_StaticObjList.begin();
	std::list<GameObject*>::iterator EndIter = m_StaticObjList.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		// 만약 해당 오브젝트를 찾았다면 지워주기
		if ((*StartIter) == m_pSelectObject)
		{
			GameObject* pFindObj = (*StartIter);
			m_StaticObjList.erase(StartIter);
			m_pLayer->DeleteGameObject(pFindObj);
			SAFE_RELEASE(pFindObj);
			m_pSelectObject = nullptr;
			return;
		}
	}

	// 애니 m_AniObjEnumList
	StartIter = m_AniObjList.begin();
	EndIter = m_AniObjList.end();

	std::list<int>::iterator eIter = m_AniObjEnumList.begin();

	for (; StartIter != EndIter; ++StartIter, ++eIter)
	{
		// 만약 해당 오브젝트를 찾았다면 지워주기
		if ((*StartIter) == m_pSelectObject)
		{
			GameObject* pFindObj = (*StartIter);
			m_AniObjList.erase(StartIter);
			m_AniObjEnumList.erase(eIter);
			SAFE_RELEASE(pFindObj);
			m_pSelectObject = nullptr;
			return;
		}
	}
}

void EnvSelectManager::RotateX(float _fScale, float _fTime)
{
	int a = 0;
	if (nullptr == m_pSelectObject)
	{
		return;
	}
	Transform* pTr = m_pSelectObject->GetTransform();
	pTr->RotateX(180.0f * _fScale, _fTime);
	SAFE_RELEASE(pTr);
}

void EnvSelectManager::RotateY(float _fScale, float _fTime)
{
	if (nullptr == m_pSelectObject)
	{
		return;
	}
	Transform* pTr = m_pSelectObject->GetTransform();
	pTr->RotateY(180.0f * _fScale, _fTime);
	SAFE_RELEASE(pTr);
}

void EnvSelectManager::RotateZ(float _fScale, float _fTime)
{
	if (nullptr == m_pSelectObject)
	{
		return;
	}

	Transform* pTr = m_pSelectObject->GetTransform();
	pTr->RotateZ(180.0f * _fScale, _fTime);
	SAFE_RELEASE(pTr);
}

void EnvSelectManager::MoveFront(float _fTime)
{
	if (nullptr == m_pSelectObject)
	{
		return;
	}

	Transform* pTr = m_pSelectObject->GetTransform();
	pTr->Move(AXIS_Z, 5.0f * 1.0f, _fTime);
	SAFE_RELEASE(pTr);
}

void EnvSelectManager::MoveBack(float _fTime)
{
	if (nullptr == m_pSelectObject)
	{
		return;
	}

	Transform* pTr = m_pSelectObject->GetTransform();
	pTr->Move(AXIS_Z, 5.0f * -1.0f, _fTime);
	SAFE_RELEASE(pTr);
}

void EnvSelectManager::MoveLeft(float _fTime)
{
	if (nullptr == m_pSelectObject)
	{
		return;
	}

	Transform* pTr = m_pSelectObject->GetTransform();
	pTr->Move(AXIS_X, 5.0f * -1.0f, _fTime);
	SAFE_RELEASE(pTr);
}

void EnvSelectManager::MoveRight(float _fTime)
{
	if (nullptr == m_pSelectObject)
	{
		return;
	}

	Transform* pTr = m_pSelectObject->GetTransform();
	pTr->Move(AXIS_X, 5.0f * 1.0f, _fTime);
	SAFE_RELEASE(pTr);
}

void EnvSelectManager::MoveUp(float _fTime)
{
	if (nullptr == m_pSelectObject)
	{
		return;
	}

	Transform* pTr = m_pSelectObject->GetTransform();
	pTr->Move(AXIS_Y, 10.0f * 1.0f, _fTime);
	SAFE_RELEASE(pTr);
}

void EnvSelectManager::MoveDown(float _fTime)
{
	if (nullptr == m_pSelectObject)
	{
		return;
	}

	Transform* pTr = m_pSelectObject->GetTransform();
	pTr->Move(AXIS_Y, 10.0f * -1.0f, _fTime);
	SAFE_RELEASE(pTr);
}

void EnvSelectManager::ScaleUp(float _fTime)
{
	if (nullptr == m_pSelectObject)
	{
		return;
	}

	Transform* pTr = m_pSelectObject->GetTransform();
	Vector3 vScale = pTr->GetWorldScale();
	vScale += (vScale * 2.0f * _fTime);
	pTr->SetWorldScale(vScale);
	SAFE_RELEASE(pTr);

}

void EnvSelectManager::ScaleDown(float _fTime)
{
	if (nullptr == m_pSelectObject)
	{
		return;
	}

	Transform* pTr = m_pSelectObject->GetTransform();
	Vector3 vScale = pTr->GetWorldScale();
	vScale -= (vScale * 2.0f * _fTime);
	pTr->SetWorldScale(vScale);
	SAFE_RELEASE(pTr);
}

void EnvSelectManager::SelectObj_None(float _fTime)
{
	m_bClickPickPossible = false;
	m_pSelectObject = nullptr;
}

void EnvSelectManager::SelectObj_ON(float _fTime)
{
	m_iSelectBtIndex = -1;
	m_bClickPickPossible = true;
}

// Save & Load
void EnvSelectManager::SaveStatic(const char * _pFullPath)
{
	// 자체포맷
	FILE*	pFile = nullptr;

	// write
	fopen_s(&pFile, _pFullPath, "wb");

	if (nullptr == pFile)
	{
		fclose(pFile);
		return;
	}

	// 저장할 총 개수 저장
	int iStaticCount = (int)(m_StaticObjList.size());
	fwrite(&iStaticCount, sizeof(int), 1, pFile);

	int iLength = 0;
	// 각 오브젝트들 메쉬이름, 위치, 크기, 회전값 저장
	std::list<GameObject*>::iterator StartIter = m_StaticObjList.begin();
	std::list<GameObject*>::iterator EndIter = m_StaticObjList.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		std::string pStaticTag = (*StartIter)->GetTag();
		Transform* pStaticTr = (*StartIter)->GetTransform();

		// 메쉬이름
		iLength = (int)(pStaticTag.length());
		fwrite(&iLength, sizeof(int), 1, pFile);
		fwrite(pStaticTag.c_str(), 1, iLength, pFile);

		// 위치, 회전, 크기값 저장 (Local, World)
		fwrite(&(pStaticTr->GetLocalPosition()), sizeof(Vector3), 1, pFile);
		fwrite(&(pStaticTr->GetLocalRotation()), sizeof(Vector3), 1, pFile);
		fwrite(&(pStaticTr->GetLocalScale()), sizeof(Vector3), 1, pFile);

		fwrite(&(pStaticTr->GetWorldPosition()), sizeof(Vector3), 1, pFile);
		fwrite(&(pStaticTr->GetWorldRotation()), sizeof(Vector3), 1, pFile);
		fwrite(&(pStaticTr->GetWorldScale()), sizeof(Vector3), 1, pFile);

		SAFE_RELEASE(pStaticTr);
	}

	fclose(pFile);
}

void EnvSelectManager::LoadStatic(const char * _pFullPath)
{
	// 기존 스태틱오브젝트들 삭제
	std::list<GameObject*>::iterator StartStaticIter = m_StaticObjList.begin();
	std::list<GameObject*>::iterator EndStaticIter = m_StaticObjList.end();

	for (; StartStaticIter != EndStaticIter; ++StartStaticIter)
	{
		SAFE_RELEASE((*StartStaticIter));
	}
	m_StaticObjList.clear();

	//
	FILE*	pFile = nullptr;

	fopen_s(&pFile, _pFullPath, "rb");

	if (nullptr == pFile)
	{
		fclose(pFile);
		return;
	}
	
	// 읽어오기
	// 총 개수
	int iStaticCount = 0;
	fread(&iStaticCount, sizeof(int), 1, pFile);

	// 각 오브젝트들 생성하기
	int iLength = 0;
	for (int i = 0; i < iStaticCount; i++)
	{

		// 생성할 오브젝트 메쉬 이름
		fread(&iLength, sizeof(int), 1, pFile);
		char strMeshName[256] = {};
		fread(strMeshName, 1, iLength, pFile);

		GameObject* pNewObject = GameObject::CreateClone(strMeshName, m_pLayer->GetScene(), m_pLayer);
		Transform*	pNewTr = pNewObject->GetTransform();

		// 여기서 오브젝트 생성도 같이해야함 (클론으로 만들어두자)
		Vector3 vLocal = Vector3::Zero;
		fread(&vLocal, sizeof(Vector3), 1, pFile);
		pNewTr->SetLocalPosition(vLocal);
		fread(&vLocal, sizeof(Vector3), 1, pFile);
		pNewTr->SetLocalRotation(vLocal);
		fread(&vLocal, sizeof(Vector3), 1, pFile);
		pNewTr->SetLocalScale(vLocal);

		Vector3 vWorld = Vector3::Zero;
		fread(&vWorld, sizeof(Vector3), 1, pFile);
		pNewTr->SetWorldPosition(vWorld);
		fread(&vWorld, sizeof(Vector3), 1, pFile);
		pNewTr->SetWorldRotation(vWorld);
		fread(&vWorld, sizeof(Vector3), 1, pFile);
		pNewTr->SetWorldScale(vWorld);

		m_StaticObjList.push_back(pNewObject);
		SAFE_RELEASE(pNewTr);
	}
	fclose(pFile);
}

void EnvSelectManager::SaveAni(const char * _pFullPath)
{
	// 자체포맷
	FILE*	pFile = nullptr;

	// write
	fopen_s(&pFile, _pFullPath, "wb");

	if (nullptr == pFile)
	{
		fclose(pFile);
		return;
	}

	// 저장할 총 개수 저장
	int iAniCount = (int)(m_AniObjList.size());
	fwrite(&iAniCount, sizeof(int), 1, pFile);

	int iLength = 0;
	// 각 오브젝트들 메쉬이름, 위치, 크기, 회전값 저장
	std::list<GameObject*>::iterator StartIter = m_AniObjList.begin();
	std::list<GameObject*>::iterator EndIter = m_AniObjList.end();

	std::list<int>::iterator eIter = m_AniObjEnumList.begin();
	int ieAniObj = -1;
	for (; StartIter != EndIter; ++StartIter, ++eIter)
	{
		std::string strAniTag = (*StartIter)->GetTag();
		Transform* pAniTr = (*StartIter)->GetTransform();
		Animation* pAniAnimation = (*StartIter)->FindComponentFromType<Animation>(CT_ANIMATION);
		PANIMATIONCLIP CurAniClip = pAniAnimation->GetCurrentClip();
		std::string strCurClipName = CurAniClip->strName;

		// enum값 저장
		ieAniObj = *eIter;
		fwrite(&ieAniObj, sizeof(int), 1, pFile);

		// 메쉬이름
		iLength = (int)(strAniTag.length());
		fwrite(&iLength, sizeof(int), 1, pFile);
		fwrite(strAniTag.c_str(), 1, iLength, pFile);

		// 애니메이션 이름
		iLength = (int)(strCurClipName.length());
		fwrite(&iLength, sizeof(int), 1, pFile);
		fwrite(strCurClipName.c_str(), 1, iLength, pFile);

		// 위치, 회전, 크기값 저장 (Local, World)
		fwrite(&(pAniTr->GetLocalPosition()), sizeof(Vector3), 1, pFile);
		fwrite(&(pAniTr->GetLocalRotation()), sizeof(Vector3), 1, pFile);
		fwrite(&(pAniTr->GetLocalScale()), sizeof(Vector3), 1, pFile);

		fwrite(&(pAniTr->GetWorldPosition()), sizeof(Vector3), 1, pFile);
		fwrite(&(pAniTr->GetWorldRotation()), sizeof(Vector3), 1, pFile);
		fwrite(&(pAniTr->GetWorldScale()), sizeof(Vector3), 1, pFile);

		SAFE_RELEASE(pAniAnimation);
		SAFE_RELEASE(pAniTr);
	}

	fclose(pFile);
}

void EnvSelectManager::LoadAni(const char * _pFullPath)
{
	// 기존 스태틱오브젝트들 삭제
	std::list<GameObject*>::iterator StartStaticIter = m_AniObjList.begin();
	std::list<GameObject*>::iterator EndStaticIter = m_AniObjList.end();

	for (; StartStaticIter != EndStaticIter; ++StartStaticIter)
	{
		SAFE_RELEASE((*StartStaticIter));
	}
	m_AniObjList.clear();

	//
	FILE*	pFile = nullptr;

	fopen_s(&pFile, _pFullPath, "rb");

	if (nullptr == pFile)
	{
		fclose(pFile);
		return;
	}

	// 읽어오기
	// 총 개수
	int iAniCount = 0;
	fread(&iAniCount, sizeof(int), 1, pFile);

	// 각 오브젝트들 생성하기
	int iLength = 0;
	for (int i = 0; i < iAniCount; i++)
	{
		// enum값
		int ieAni = -1;
		fread(&ieAni, sizeof(int), 1, pFile);

		// 생성할 오브젝트 메쉬 이름
		fread(&iLength, sizeof(int), 1, pFile);
		char strMeshName[256] = {};
		fread(strMeshName, 1, iLength, pFile);

		GameObject* pNewObject = GameObject::CreateClone(strMeshName, m_pLayer->GetScene(), m_pLayer);
		Transform*	pNewTr = pNewObject->GetTransform();

		// 해당 애니메이션 오브젝트 기본 애니메이션 클립명가져오기
		fread(&iLength, sizeof(int), 1, pFile);
		char ClipName[256] = {};
		fread(ClipName, 1, iLength, pFile);

		std::string strClipName = ClipName;
		Animation* pNewObjAni = pNewObject->FindComponentFromType<Animation>(CT_ANIMATION);
		pNewObjAni->ChangeClip(strClipName);

		// 여기서 오브젝트 생성도 같이해야함 (클론으로 만들어두자)
		Vector3 vLocal = Vector3::Zero;
		fread(&vLocal, sizeof(Vector3), 1, pFile);
		pNewTr->SetLocalPosition(vLocal);
		fread(&vLocal, sizeof(Vector3), 1, pFile);
		pNewTr->SetLocalRotation(vLocal);
		fread(&vLocal, sizeof(Vector3), 1, pFile);
		pNewTr->SetLocalScale(vLocal);

		Vector3 vWorld = Vector3::Zero;
		fread(&vWorld, sizeof(Vector3), 1, pFile);
		pNewTr->SetWorldPosition(vWorld);
		fread(&vWorld, sizeof(Vector3), 1, pFile);
		pNewTr->SetWorldRotation(vWorld);
		fread(&vWorld, sizeof(Vector3), 1, pFile);
		pNewTr->SetWorldScale(vWorld);

		m_AniObjList.push_back(pNewObject);
		m_AniObjEnumList.push_back(ieAni);
		SAFE_RELEASE(pNewObjAni);
		SAFE_RELEASE(pNewTr);
	}
	fclose(pFile);
}

void EnvSelectManager::SaveNav(const char * _pFullPath)
{
	if (nullptr == m_pCurSceneLand)
	{
		return;
	}

	// 네비 저장
	FILE*	pFile = nullptr;

	fopen_s(&pFile, _pFullPath, "wb");

	if (nullptr == pFile)
	{
		fclose(pFile);
		return;
	}

	int iLength = 0;
	std::vector<int> vecSaveIndex = m_pCurSceneLand->GetVecNavEnable();

	iLength = (int)(vecSaveIndex.size());
	fwrite(&iLength, sizeof(int), 1, pFile);

	for (int i = 0; i < vecSaveIndex.size(); ++i)
	{
		fwrite(&vecSaveIndex[i], sizeof(int), 1, pFile);
	}

	fclose(pFile);
}

void EnvSelectManager::LoadNav(const char * _pFullPath)
{
	if (nullptr == m_pCurSceneLand)
	{
		return;
	}

	FILE*	pFile = nullptr;

	fopen_s(&pFile, _pFullPath, "rb");

	if (nullptr == pFile)
	{
		fclose(pFile);
		return;
	}

	// 읽어오기
	// 총 개수
	int iIndex = 0;
	int iLength = 0;
	fread(&iLength, sizeof(int), 1, pFile);

	// 각 오브젝트들 생성하기
	for (int i = 0; i < iLength; i++)
	{

		// 생성할 오브젝트 메쉬 이름
		fread(&iIndex, sizeof(int), 1, pFile);
		m_pCurSceneLand->ChangeNavIndexEnable(false, iIndex);
	}

	fclose(pFile);
}

