#pragma once
#include "Component\UserComponent.h"
#include "Component\Renderer.h"
#include "Component\Transform.h"
#include "Component\Material.h"
#include "Component\Animation.h"
#include "Component\UIButton.h"
#include "Component\UIBar.h"
#include "Component\UIText.h"
#include "Component\LandScape.h"

ENGINE_USING

#define	MAX_PAGE 6

#define MESH_SEL_X_MAX 12
#define MESH_SEL_Y_MAX 4

#define MESH_SEL_BT_X_LEN 145
#define MESH_SEL_BT_Y_LEN 150

enum FOLDER_TYPE
{
	FOLDER_NONE = 0,
	FOLDER_NAV,
	FOLDER_ANI,
	FOLDER_STATIC,
	FOLDER_LAND,
};

struct SelectKindBt
{
	GameObject*		m_pSelectKindBtObj;
	Transform*		m_pSelectKindBtTr;
	UIButton*		m_pSelectKindBt;

	SelectKindBt()
	{
		m_pSelectKindBtObj = nullptr;
		m_pSelectKindBtTr = nullptr;
		m_pSelectKindBt = nullptr;
	}

};

struct MeshSelectBt
{
	// 오브젝트 선택 버튼
	GameObject*		pMeshSelBtObj;
	Transform*		pMeshSelBtTr;
	UIButton*		pMeshSelBt;

	MeshSelectBt()
	{
		pMeshSelBtObj = nullptr;
		pMeshSelBtTr = nullptr;
		pMeshSelBt = nullptr;
	}

};


#pragma region 오브젝트 종류 enum들

enum MapTool_Type_Static
{
	MTS_EX01_Adventure_Floor = 0,
	MTS_EX01_Door_01_SM,
	MTS_EX01_Foliage_JUG_Mush_02_SM,
	MTS_EX01_Foliage_JUG_Mush_03_SM,
	MTS_EX01_Foliage_JUG_Small_02_SM,
	MTS_Grass1,
	MTS_GWall0,
	MTS_GWall1,
	MTS_GWall2,
	MTS_GWall3,
	MTS_GWall4,
	MTS_GWall5,
	MTS_GWall6,
	MTS_RockB1,
	MTS_RockB12,
	MTS_RockB2,
	MTS_ST_Angel,
	MTS_Tree0,
	MTS_Tree1,
	MTS_Tree4,
	MTS_EX01_Balder_Floor01,
	MTS_EX01_Balder_Floor02,
	MTS_EX01_Balder_Gate02,
	MTS_EX01_Balder_Gate03,
	MTS_Ex01_Balder_Maze01,
	MTS_Ex01_Balder_Maze02,
	MTS_Ex01_Balder_Maze03,
	MTS_Ex01_Balder_Maze04,
	MTS_EX01_Balder_Obj01,
	MTS_EX01_Balder_Obj02,
	MTS_EX01_Balder_Obj03,
	MTS_EX01_Balder_Pillar01,
	MTS_EX01_Balder_Wall01,
	MTS_EX01_Balder_Wall02,
	MTS_EX01_Balder_Wall04,
	MTS_EX01_Balder_Wall05,
	MTS_Z_VD_Ev_01,
	MTS_Z_VD_F1_01,
	MTS_Z_VD_F2_01,
	MTS_Z_VD_F3_01,
	MTS_Z_VD_F4_01,
	MTS_Z_VD_Roof_01,
	MTS_Z_WhiteTree01,
	MTS_Z_WhiteTree02,
	MTS_MAX
};

enum MapTool_Type_Ani
{
	MTA_Arcdeva_Gate = 0,
	MTA_Kumas_Mother,
	MTA_EnchantedDoll,
	MTA_HookaFootman_A,
	MTA_FloatingCastle,
	MTA_Kumas_Baby_Blue,
	MTA_Kumas_Baby_Pink,
	MTA_Kumas_Baby_Red,
	MTA_Goat,
	MTA_Highelf_M_Alchemist,
	MTA_Human_F_Aide,
	MTA_Popori_boy,
	MTA_Popori_boy02,
	MTA_Popori_boy03,
	MTA_Q_Mistel,
	MTA_Rabbit,

	MTA_MAX
};

enum MapTool_Type_Nav
{
	MTN_FALSE = 0,
	MTN_TRUE,
};

#pragma endregion


class EnvSelectManager : public UserComponent
{
private:
	// 배경
	GameObject*		m_pSelectTopBGObj;
	Transform*		m_pSelectTopBGTr;
	UIBar*			m_pSelectTopBGImage;

	GameObject*		m_pSelectMdBGObj;
	Transform*		m_pSelectMdBGTr;
	UIBar*			m_pSelectMdBGImage;

	GameObject*		m_pSelectBotBGObj;
	Transform*		m_pSelectBotBGTr;
	UIBar*			m_pSelectBotBGImage;

	GameObject*		m_pKindTopLineObj;
	Transform*		m_pKindTopLineTr;
	UIBar*			m_pKindTopLineImage;

	GameObject*		m_pKindBotLineObj;
	Transform*		m_pKindBotLineTr;
	UIBar*			m_pKindBotLineImage;

	// 버튼들
	GameObject*		m_pCloseBtObj;
	Transform*		m_pCloseBtTr;
	UIButton*		m_pCloseBt;

	GameObject*		m_pPrevPageBtObj;
	Transform*		m_pPrevPageBtTr;
	UIButton*		m_pPrevPageBt;

	GameObject*		m_pNextPageBtObj;
	Transform*		m_pNextPageBtTr;
	UIButton*		m_pNextPageBt;

	// 지형
	LandScape*		m_pCurSceneLand;

	//std::vector<SelectKindBt*> m_vecSelectKindBts;
	std::vector<MeshSelectBt*> m_vecMeshSelBts;

	std::vector<std::string>	m_vecDirectoryStaticName;
	std::vector<std::string>	m_vecDirectoryAniName;

	std::vector<std::string>	m_vecDirectoryNavName;
	std::vector<std::string>	m_vecDirectoryLandName;

	// 몇페이지인지 확인할 텍스트
	int				m_iMaxPage;
	int				m_iCurPage;

	// 클릭하면 ( 선택 가능하도록.. )
	bool		m_bClickPickPossible;
	GameObject*	m_pSelectObject;

	bool		m_bClickPickCol;			// 충돌체 여부

	// 선택된 오브젝트 버튼 인덱스
	int				m_iSelectBtIndex;

	GameObject*		m_pCurPageTextObj;
	Transform*		m_pCurPageTextTr;
	UIText*			m_pCurPageText;

	GameObject*		m_pMaxPageTextObj;
	Transform*		m_pMaxPageTextTr;
	UIText*			m_pMaxPageText;

	// 선택된 폴더 타입
	FOLDER_TYPE m_eFolderType;

	// 플레이어 스크립트 관련 (오브젝트 생성때문)
	class Mover*	m_pMoverScript;

	// 생성된 맵 게임오브젝트들 관리
	std::list<GameObject*> m_StaticObjList;
	std::list<GameObject*> m_AniObjList;
	std::list<int>			m_AniObjEnumList;

	bool		m_bSaveALoad;					// Save, Load 선택화면인지 여부 (이때는 생성 X)

private:
	class MapToolButtons*		m_pMapToolBtScript;

public:
	EnvSelectManager();
	EnvSelectManager(const EnvSelectManager& _SelectEnvInfo);
	~EnvSelectManager();

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);
	virtual int Render(float _fTime);
	virtual EnvSelectManager* Clone()	const;

public:
	void SetMoverScript(class Mover* _MoverScript);
	void SetMapToolBtScript(class MapToolButtons* _Script);
	void SetLandScape(LandScape* _LandScape);

private:
	void InputInit();

	void CreateSelectBG();			// 배경
	void CreateButtons();			// 버튼들
	void CreateTexts();				// 텍스트들

	void CreateEnvObject(Vector3 _Pos);
	std::vector<std::string> GetFileDirectory(std::string _Path, std::string _Filter, FOLDER_TYPE _eType);

	void MeshSelectBtMoveEnable();				// 이동시(다음, 이전페이지) Enable여부

	// 네비관련 함수(못가는 네비, 갈수있는 네비 관련)
	void ChangeNavEnable(Vector3 _vPos);

public:
	// 열기, 닫기
	void EnvSelectBtOn(float _fTime);
	void EnvSelectBtOff(float _fTime);

	void SetEnvSelectBtStatic(float _fTime);
	void SetEnvSelectBtStaticImage();

	void SetEnvSelectBtAni(float _fTime);
	void SetEnvSelectBtAniImage();

	void SetEnvSelectBtNav(float _fTime);
	void SetEnvSelectBtNavImage();

	void SetEnvSelectBtLand(float _fTime);
	void SetEnvSelectBtLandImage();

	void SetEnvSelectBtBaseImage(FOLDER_TYPE _eType);

public:
	// 버튼 눌렀을때 상호작용함수 (이전, 다음)
	void PrevBtClick(float _fTime);
	void NextBtClick(float _fTime);

public:
	// 오브젝트 종류 선택 버튼 눌렀을때
	void SetSelectIndex(float _fTime);
	void SetbSaveALoad(bool _Enable);
	//
	void DeleteSelObject(float _fTime);

private:
	void RotateX(float _fScale, float _fTime);
	void RotateY(float _fScale, float _fTime);
	void RotateZ(float _fScale, float _fTime);

	void MoveFront(float _fTime);
	void MoveBack(float _fTime);
	void MoveLeft(float _fTime);
	void MoveRight(float _fTime);
	void MoveUp(float _fTime);
	void MoveDown(float _fTime);

	void ScaleUp(float _fTime);
	void ScaleDown(float _fTime);

	void SelectObj_None(float _fTime);
	void SelectObj_ON(float _fTime);

public:
	void SaveStatic(const char * _pFullPath);
	void LoadStatic(const char * _pFullPath);

	void SaveAni(const char * _pFullPath);
	void LoadAni(const char * _pFullPath);

	void SaveNav(const char * _pFullPath);
	void LoadNav(const char * _pFullPath);

};

