#include "MapToolScene.h"
#include "Device.h"

void MapToolScene::CreateProtoType()
{
	// Static
	GameObject* pNewProtoType = GameObject::CreatePrototype("EX01_Adventure_Floor", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	Transform*	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	Renderer* pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("EX01_Adventure_Floor", TEXT("EX01_Adventure_Floor.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("EX01_Door_01_SM", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("EX01_Door_01_SM", TEXT("EX01_Door_01_SM.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("EX01_Foliage_JUG_Mush_02_SM", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("EX01_Foliage_JUG_Mush_02_SM", TEXT("EX01_Foliage_JUG_Mush_02_SM.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("EX01_Foliage_JUG_Mush_03_SM", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("EX01_Foliage_JUG_Mush_03_SM", TEXT("EX01_Foliage_JUG_Mush_03_SM.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("EX01_Foliage_JUG_Small_02_SM", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("EX01_Foliage_JUG_Small_02_SM", TEXT("EX01_Foliage_JUG_Small_02_SM.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("Grass1", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("Grass1", TEXT("Grass1.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("GWall0", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("GWall0", TEXT("GWall0.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("GWall1", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("GWall1", TEXT("GWall1.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("GWall2", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("GWall2", TEXT("GWall2.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("GWall3", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("GWall3", TEXT("GWall3.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("GWall4", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("GWall4", TEXT("GWall4.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("GWall5", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("GWall5", TEXT("GWall5.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("GWall6", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("GWall6", TEXT("GWall6.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("RockB1", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("RockB1", TEXT("RockB1.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("RockB2", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("RockB2", TEXT("RockB2.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("RockB12", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("RockB12", TEXT("RockB12.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("ST_Angel", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("ST_Angel", TEXT("ST_Angel.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("Tree0", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("Tree0", TEXT("Tree0.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("Tree1", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("Tree1", TEXT("Tree1.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("Tree4", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("Tree4", TEXT("Tree4.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("EX01_Balder_Floor01", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("EX01_Balder_Floor01", TEXT("EX01_Balder_Floor01.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("EX01_Balder_Floor02", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("EX01_Balder_Floor02", TEXT("EX01_Balder_Floor02.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("EX01_Balder_Gate02", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("EX01_Balder_Gate02", TEXT("EX01_Balder_Gate02.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("EX01_Balder_Gate03", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("EX01_Balder_Gate03", TEXT("EX01_Balder_Gate03.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("Ex01_Balder_Maze01", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("Ex01_Balder_Maze01", TEXT("Ex01_Balder_Maze01.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("Ex01_Balder_Maze02", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("Ex01_Balder_Maze02", TEXT("Ex01_Balder_Maze02.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("Ex01_Balder_Maze03", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("Ex01_Balder_Maze03", TEXT("Ex01_Balder_Maze03.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("Ex01_Balder_Maze04", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("Ex01_Balder_Maze04", TEXT("Ex01_Balder_Maze04.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("EX01_Balder_Obj01", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("EX01_Balder_Obj01", TEXT("EX01_Balder_Obj01.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("EX01_Balder_Obj02", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("EX01_Balder_Obj02", TEXT("EX01_Balder_Obj02.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("EX01_Balder_Obj03", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("EX01_Balder_Obj03", TEXT("EX01_Balder_Obj03.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("EX01_Balder_Pillar01", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("EX01_Balder_Pillar01", TEXT("EX01_Balder_Pillar01.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("EX01_Balder_Wall01", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("EX01_Balder_Wall01", TEXT("EX01_Balder_Wall01.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("EX01_Balder_Wall02", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("EX01_Balder_Wall02", TEXT("EX01_Balder_Wall02.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("EX01_Balder_Wall04", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("EX01_Balder_Wall04", TEXT("EX01_Balder_Wall04.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("EX01_Balder_Wall05", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("EX01_Balder_Wall05", TEXT("EX01_Balder_Wall05.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("VD_Ev_01", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("VD_Ev_01", TEXT("VD_Ev_01.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("VD_F1_01", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("VD_F1_01", TEXT("VD_F1_01.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("VD_F2_01", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("VD_F2_01", TEXT("VD_F2_01.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("VD_F3_01", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("VD_F3_01", TEXT("VD_F3_01.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("VD_F4_01", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("VD_F4_01", TEXT("VD_F4_01.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("VD_Roof_01", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("VD_Roof_01", TEXT("VD_Roof_01.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("WhiteTree01", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("WhiteTree01", TEXT("WhiteTree01.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("WhiteTree02", m_pScene);
	pNewProtoType->SetRenderGroup(RG_STATIC);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("WhiteTree02", TEXT("WhiteTree02.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);

	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);
	////////////////////////////////////////////////////////////////////////////////////////////////// Animation ( 애니까지 존재 )
	pNewProtoType = GameObject::CreatePrototype("Arcdeva_Gate", m_pScene);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("Arcdeva_Gate", TEXT("Arcdeva_Gate.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);
	Animation* pNewAnimation = pNewProtoType->AddComponent<Animation>("NewAnimation");
	pNewAnimation->LoadBone("Arcdeva_Gate.bne");
	pNewAnimation->Load("Arcdeva_Gate.anm");
	pNewAnimation->ChangeClip("Open");

	SAFE_RELEASE(pNewAnimation);
	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("Kumas_Mother", m_pScene);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("Kumas_Mother", TEXT("Kumas_Mother.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);
	pNewAnimation = pNewProtoType->AddComponent<Animation>("NewAnimation");
	pNewAnimation->LoadBone("Kumas_Mother.bne");
	pNewAnimation->Load("Kumas_Mother.anm");
	pNewAnimation->ChangeClip("Groggy");

	SAFE_RELEASE(pNewAnimation);
	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("EnchantedDoll", m_pScene);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("EnchantedDoll", TEXT("EnchantedDoll.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);
	pNewAnimation = pNewProtoType->AddComponent<Animation>("NewAnimation");
	pNewAnimation->LoadBone("EnchantedDoll.bne");
	pNewAnimation->Load("EnchantedDoll.anm");
	pNewAnimation->ChangeClip("Wait");

	SAFE_RELEASE(pNewAnimation);
	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("HookaFootman_A", m_pScene);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("HookaFootman_A", TEXT("HookaFootman_A.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);
	pNewAnimation = pNewProtoType->AddComponent<Animation>("NewAnimation");
	pNewAnimation->LoadBone("HookaFootman_A.bne");
	pNewAnimation->Load("HookaFootman_A.anm");
	pNewAnimation->ChangeClip("Wait");

	SAFE_RELEASE(pNewAnimation);
	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("FloatingCastle", m_pScene);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("FloatingCastle", TEXT("FloatingCastle.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);
	pNewAnimation = pNewProtoType->AddComponent<Animation>("NewAnimation");
	pNewAnimation->LoadBone("FloatingCastle.bne");
	pNewAnimation->Load("FloatingCastle.anm");
	pNewAnimation->ChangeClip("Idle");

	SAFE_RELEASE(pNewAnimation);
	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("Kumas_Baby_Blue", m_pScene);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("Kumas_Baby_Blue", TEXT("Kumas_Baby_Blue.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);
	pNewAnimation = pNewProtoType->AddComponent<Animation>("NewAnimation");
	pNewAnimation->LoadBone("Kumas_Baby_Blue.bne");
	pNewAnimation->Load("Kumas_Baby_Blue.anm");
	pNewAnimation->ChangeClip("Groggy");

	SAFE_RELEASE(pNewAnimation);
	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("Kumas_Baby_Pink", m_pScene);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("Kumas_Baby_Pink", TEXT("Kumas_Baby_Pink.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);
	pNewAnimation = pNewProtoType->AddComponent<Animation>("NewAnimation");
	pNewAnimation->LoadBone("Kumas_Baby_Pink.bne");
	pNewAnimation->Load("Kumas_Baby_Pink.anm");
	pNewAnimation->ChangeClip("Groggy");

	SAFE_RELEASE(pNewAnimation);
	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("Kumas_Baby_Red", m_pScene);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("Kumas_Baby_Red", TEXT("Kumas_Baby_Red.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);
	pNewAnimation = pNewProtoType->AddComponent<Animation>("NewAnimation");
	pNewAnimation->LoadBone("Kumas_Baby_Red.bne");
	pNewAnimation->Load("Kumas_Baby_Red.anm");
	pNewAnimation->ChangeClip("Groggy");

	SAFE_RELEASE(pNewAnimation);
	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("Goat", m_pScene);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("Goat", TEXT("Goat.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);
	pNewAnimation = pNewProtoType->AddComponent<Animation>("NewAnimation");
	pNewAnimation->LoadBone("Goat.bne");
	pNewAnimation->Load("Goat.anm");
	pNewAnimation->ChangeClip("Idle01");

	SAFE_RELEASE(pNewAnimation);
	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("Highelf_M_Alchemist", m_pScene);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("Highelf_M_Alchemist", TEXT("Highelf_M_Alchemist.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);
	pNewAnimation = pNewProtoType->AddComponent<Animation>("NewAnimation");
	pNewAnimation->LoadBone("Highelf_M_Alchemist.bne");
	pNewAnimation->Load("Highelf_M_Alchemist.anm");
	pNewAnimation->ChangeClip("Wait");

	SAFE_RELEASE(pNewAnimation);
	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("Human_F_Aide", m_pScene);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("Human_F_Aide", TEXT("Human_F_Aide.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);
	pNewAnimation = pNewProtoType->AddComponent<Animation>("NewAnimation");
	pNewAnimation->LoadBone("Human_F_Aide.bne");
	pNewAnimation->Load("Human_F_Aide.anm");
	pNewAnimation->ChangeClip("Wait");

	SAFE_RELEASE(pNewAnimation);
	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("Popori_boy", m_pScene);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("Popori_boy", TEXT("Popori_boy.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);
	pNewAnimation = pNewProtoType->AddComponent<Animation>("NewAnimation");
	pNewAnimation->LoadBone("Popori_boy.bne");
	pNewAnimation->Load("Popori_boy.anm");
	pNewAnimation->ChangeClip("Idle");

	SAFE_RELEASE(pNewAnimation);
	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("Popori_boy02", m_pScene);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("Popori_boy02", TEXT("Popori_boy02.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);
	pNewAnimation = pNewProtoType->AddComponent<Animation>("NewAnimation");
	pNewAnimation->LoadBone("Popori_boy02.bne");
	pNewAnimation->Load("Popori_boy02.anm");
	pNewAnimation->ChangeClip("Idle");

	SAFE_RELEASE(pNewAnimation);
	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("Popori_boy03", m_pScene);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("Popori_boy03", TEXT("Popori_boy03.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);
	pNewAnimation = pNewProtoType->AddComponent<Animation>("NewAnimation");
	pNewAnimation->LoadBone("Popori_boy03.bne");
	pNewAnimation->Load("Popori_boy03.anm");
	pNewAnimation->ChangeClip("Idle");

	SAFE_RELEASE(pNewAnimation);
	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("Q_Mistel", m_pScene);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("Q_Mistel", TEXT("Q_Mistel.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);
	pNewAnimation = pNewProtoType->AddComponent<Animation>("NewAnimation");
	pNewAnimation->LoadBone("Q_Mistel.bne");
	pNewAnimation->Load("Q_Mistel.anm");
	pNewAnimation->ChangeClip("Talk");

	SAFE_RELEASE(pNewAnimation);
	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("Rabbit", m_pScene);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("Rabbit", TEXT("Rabbit.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);
	pNewAnimation = pNewProtoType->AddComponent<Animation>("NewAnimation");
	pNewAnimation->LoadBone("Rabbit.bne");
	pNewAnimation->Load("Rabbit.anm");
	pNewAnimation->ChangeClip("Idle01");

	SAFE_RELEASE(pNewAnimation);
	SAFE_RELEASE(pNewRenderer);
	SAFE_RELEASE(pNewTr);
	SAFE_RELEASE(pNewProtoType);

}
