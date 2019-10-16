#pragma once
#include "MapToolInfo.h"
#include "GameObject.h"
#include "Scene\Scene.h"
#include "Scene\Layer.h"
#include "Component\Renderer.h"
#include "Component\TPCamera.h"
#include "Component\Particle.h"
#include "Component\AnimationFrame.h"
#include "Component\LandScape.h"
#include "Component\UIBar.h"
#include "Component\UIText.h"
#include "CollisionManager.h"

#include "..\UserComponent\Mover.h"
#include "..\UserComponent\EnvSelectManager.h"
#include "..\UserComponent\MapToolButtons.h"

ENGINE_USING

struct InvironmentStatic
{
	GameObject* pEnvStaticObject;
	ENV_STATIC eEnvStatic;

	InvironmentStatic()
	{
		pEnvStaticObject = nullptr;
		eEnvStatic = STATIC_NONE;
	}
};

struct InvironmentAni
{
	GameObject* pEnvAniObject;
	ENV_ANI eEnvAni;

	InvironmentAni()
	{
		pEnvAniObject = nullptr;
		eEnvAni = ENVANI_NONE;
	}
};

class MapToolScene : public MapToolInfo
{
private:
	Layer*		pDefaultLayer;
	Layer*		pUILayer;

private:
	//  지형
	GameObject* pLandScapeObject;
	GameObject* pMainCameraObject;
	GameObject* pMoverCameraObject;

	// 그냥 단순 움직이는 용도의 오브젝트
	GameObject* pMoverObject;
	Transform*	pMoverTr;
	Mover*		pMoverScript;

	// 인벤토리처럼 선택한 메쉬 처리할 오브젝트 ( 지형 포함 )
	GameObject*			pEnvsSelObject;
	Transform*			pEnvsSelTr;
	EnvSelectManager*	pEnvsSelScript;
	

	// 맵 아래 버튼들 관리할 오브젝트 ( 네비, 스태틱, 애니, 세이브, 로드 )
	GameObject*		pMapToolButtonsObj;
	Transform*		pMapToolButtonsTr;
	MapToolButtons*	pMapToolButtonsScript;

private:
	std::list<InvironmentStatic*>	m_EnvStaticObjectList;
	std::list<InvironmentAni*>		m_EnvAniObjectList;

public:
	MapToolScene();
	~MapToolScene();

public:
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int Render(float _fTime);

public:
	void LayerInit();
	void TPCameraInit();
	void MapToolObjInit();
	void LandScapeInit();
	
	void CreateProtoType();
};

