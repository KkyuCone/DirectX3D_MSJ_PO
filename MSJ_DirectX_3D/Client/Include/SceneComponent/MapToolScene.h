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
	//  ����
	GameObject* pLandScapeObject;
	GameObject* pMainCameraObject;
	GameObject* pMoverCameraObject;

	// �׳� �ܼ� �����̴� �뵵�� ������Ʈ
	GameObject* pMoverObject;
	Transform*	pMoverTr;
	Mover*		pMoverScript;

	// �κ��丮ó�� ������ �޽� ó���� ������Ʈ ( ���� ���� )
	GameObject*			pEnvsSelObject;
	Transform*			pEnvsSelTr;
	EnvSelectManager*	pEnvsSelScript;
	

	// �� �Ʒ� ��ư�� ������ ������Ʈ ( �׺�, ����ƽ, �ִ�, ���̺�, �ε� )
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

