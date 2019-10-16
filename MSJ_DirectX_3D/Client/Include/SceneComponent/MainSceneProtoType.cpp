#include "MainScene.h"
#include "PathManager.h"
#include "Device.h"

bool MainScene::ProtoTypeInit()
{

	// 총알 프로토타입
	GameObject* pBulletProtoType = GameObject::CreatePrototype("Bullet", m_pScene);
	Bullet* pBulletScriptProtoType = pBulletProtoType->AddComponent<Bullet>("Bullet");

	SAFE_RELEASE(pBulletScriptProtoType);
	SAFE_RELEASE(pBulletProtoType);

	// 파티클 프로토타입
	GameObject* pExplosionProtoType = GameObject::CreatePrototype("Explosion", m_pScene);

	Transform* pExplosionTransform = pExplosionProtoType->GetTransform();
	pExplosionTransform->SetWorldScale(3.0f, 3.0f, 3.0f);

	Renderer*	pExplosionRenderer = pExplosionProtoType->AddComponent<Renderer>("ExplosionRenderer");
	pExplosionRenderer->SetMesh("Particle");
	pExplosionRenderer->SetRenderState("AlphaBlend");

	Particle*	pExplosionParticle = pExplosionProtoType->AddComponent<Particle>("Explosion");

	std::vector<TCHAR*>	vecParticleFileName;				// 파티클 2D애니메이션 하려구 ㅇㅅㅇ 각 텍스처들 불러오기
	vecParticleFileName.reserve(89);

	for (size_t i = 1; i <= 89; ++i)
	{
		TCHAR*	pFileName = new TCHAR[MAX_PATH];
		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(pFileName, TEXT("Explosion/Explosion%d.png"), i);

		vecParticleFileName.push_back(pFileName);
	}

	pExplosionParticle->LoadTextureSet("Explosion", vecParticleFileName);

	AnimationFrame* pExplosionFrame = pExplosionProtoType->AddComponent<AnimationFrame>("ExplosionAnimation");

	std::vector<TextureCoord> vecCoord;
	vecCoord.reserve(89);

	for (int i = 0; i < 89; ++i)
	{
		TextureCoord tCoord = {};
		tCoord.vStart = Vector2(0.0f, 0.0f);
		tCoord.vEnd = Vector2(320.0f, 240.0f);
		vecCoord.push_back(tCoord);
	}

	pExplosionFrame->CreateClip("Idle", AO_LOOP, Vector2(320.0f, 240.0f),
		vecCoord, 1.0f, 10, "Explosion", vecParticleFileName);

	SAFE_RELEASE(pExplosionFrame);
	SAFE_RELEASE(pExplosionParticle);
	SAFE_RELEASE(pExplosionRenderer);
	SAFE_RELEASE(pExplosionTransform);
	SAFE_RELEASE(pExplosionProtoType);
	Safe_Delete_Array_VectorList(vecParticleFileName);

	// HP바 파티클
	GameObject* pMosnterHPProtoType = GameObject::CreatePrototype("MonsterHP", m_pScene);

	Transform* pMosnterHPTransform = pMosnterHPProtoType->GetTransform();
	pMosnterHPTransform->SetWorldPosition(0.0f, 7.0f, 0.0f);
	pMosnterHPTransform->SetWorldScale(5.1f, 0.42f, 1.0f);			// 510, 42
	pMosnterHPTransform->SetParentOnltyPositon(true);

	Renderer*	pHPBarRenderer = pMosnterHPProtoType->AddComponent<Renderer>("MonsterHPRenderer");
	pHPBarRenderer->SetMesh("Particle");
	pHPBarRenderer->SetRenderState("AlphaBlend");

	Particle*	pHPBarParticle = pMosnterHPProtoType->AddComponent<Particle>("MonsterHP");

	pHPBarParticle->LoadTextureSet("MonsterHP", TEXT("MonsterHPBar.png"), PATH_UI_BAR);

	SAFE_RELEASE(pHPBarParticle);
	SAFE_RELEASE(pHPBarRenderer);
	SAFE_RELEASE(pMosnterHPTransform);
	SAFE_RELEASE(pMosnterHPProtoType);

	// HPBar 고양이
	GameObject* pDollMosnterHPProtoType = GameObject::CreatePrototype("DollMonsterHP", m_pScene);

	Transform* pDollMosnterHPTransform = pDollMosnterHPProtoType->GetTransform();
	pDollMosnterHPTransform->SetWorldPosition(0.0f, 7.0f, 0.0f);
	pDollMosnterHPTransform->SetWorldScale(5.1f, 0.42f, 1.0f);			// 510, 42
	pDollMosnterHPTransform->SetParentOnltyPositon(true);

	Renderer*	pDollHPBarRenderer = pDollMosnterHPProtoType->AddComponent<Renderer>("DollMonsterHPRenderer");
	pDollHPBarRenderer->SetMesh("Particle");
	pDollHPBarRenderer->SetRenderState("AlphaBlend");

	Particle*	pDollHPBarParticle = pDollMosnterHPProtoType->AddComponent<Particle>("DollMonsterHP");

	pDollHPBarParticle->LoadTextureSet("DollMonsterHP", TEXT("MonsterHPBar.png"), PATH_UI_BAR);

	SAFE_RELEASE(pDollHPBarParticle);
	SAFE_RELEASE(pDollHPBarRenderer);
	SAFE_RELEASE(pDollMosnterHPTransform);
	SAFE_RELEASE(pDollMosnterHPProtoType);

	// 빗방울 파티클
	GameObject* pRainParticleProtoType = GameObject::CreatePrototype("Rain", m_pScene);

	Transform* pRainParticleTransform = pRainParticleProtoType->GetTransform();
	pRainParticleTransform->SetWorldScale(3.0f, 3.0f, 3.0f);

	Renderer*	pRainRenderer = pRainParticleProtoType->AddComponent<Renderer>("RainRenderer");
	pRainRenderer->SetMesh("Particle");
	pRainRenderer->SetRenderState("AlphaBlend");

	Particle*	pRainParticle = pRainParticleProtoType->AddComponent<Particle>("Rain");

	std::vector<TCHAR*>	vecRainParticleFileName;				// 파티클 2D애니메이션 하려구 ㅇㅅㅇ 각 텍스처들 불러오기
	vecRainParticleFileName.reserve(89);

	for (size_t i = 1; i <= 1; ++i)
	{
		TCHAR*	pFileName = new TCHAR[MAX_PATH];
		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(pFileName, TEXT("RainStreak.dds"), i);

		vecRainParticleFileName.push_back(pFileName);
	}

	pRainParticle->LoadTextureSet("Rain", vecRainParticleFileName);

	AnimationFrame* pRainFrame = pRainParticleProtoType->AddComponent<AnimationFrame>("RainAnimation");

	std::vector<TextureCoord> vecRainCoord;
	vecRainCoord.reserve(1);

	for (int i = 0; i < 1; ++i)
	{
		TextureCoord tCoord = {};
		tCoord.vStart = Vector2(0.0f, 0.0f);
		tCoord.vEnd = Vector2(320.0f, 240.0f);
		vecRainCoord.push_back(tCoord);
	}

	pRainFrame->CreateClip("Idle", AO_LOOP, Vector2(320.0f, 240.0f),
		vecRainCoord, 1.0f, 10, "Rain", vecRainParticleFileName);

	SAFE_RELEASE(pRainFrame);
	SAFE_RELEASE(pRainParticle);
	SAFE_RELEASE(pRainRenderer);
	SAFE_RELEASE(pRainParticleTransform);
	SAFE_RELEASE(pRainParticleProtoType);
	Safe_Delete_Array_VectorList(vecRainParticleFileName);


	// 의자
	GameObject*	pAnjangPrototype = GameObject::CreatePrototype("Anjang", m_pScene);
	pAnjangPrototype->SetRenderGroup(RG_STATIC);
	Renderer*	pAnjangRenderer = pAnjangPrototype->AddComponent<Renderer>("AnjangRenderer");

	pAnjangRenderer->SetMesh("Anjang", TEXT("chair.msh"));

	SAFE_RELEASE(pAnjangRenderer);

	SAFE_RELEASE(pAnjangPrototype);


	// 이름표 
	GameObject* pNPCTagProtoType = GameObject::CreatePrototype("Human_F_Aide_NameTag", m_pScene);

	Transform* pNPCTagTr = pNPCTagProtoType->GetTransform();
	pNPCTagTr->SetWorldPosition(0.0f, 5.0f, 0.0f);
	pNPCTagTr->SetWorldScale(6.0f, 1.0f, 1.0f);			// 510, 42
	pNPCTagTr->SetParentOnltyPositon(true);

	Renderer*	pNPCTagRenderer = pNPCTagProtoType->AddComponent<Renderer>("NPCTagRenderer");
	pNPCTagRenderer->SetMesh("Particle");
	pNPCTagRenderer->SetRenderState("AlphaBlend");

	Particle*	pNPCTagParticle = pNPCTagProtoType->AddComponent<Particle>("NPCNameTag");

	pNPCTagParticle->LoadTextureSet("Human_F_Aide_NameTag", TEXT("Human_F_Aide_NameTag.png"), PATH_UI_NPCNAME);

	SAFE_RELEASE(pNPCTagParticle);
	SAFE_RELEASE(pNPCTagRenderer);
	SAFE_RELEASE(pNPCTagTr);
	SAFE_RELEASE(pNPCTagProtoType);


	pNPCTagProtoType = GameObject::CreatePrototype("Q_Mistel_NameTag", m_pScene);
	pNPCTagTr = pNPCTagProtoType->GetTransform();
	pNPCTagTr->SetWorldPosition(0.0f, 6.0f, 0.0f);
	pNPCTagTr->SetWorldScale(6.0f, 1.0f, 1.0f);			// 510, 42
	pNPCTagTr->SetParentOnltyPositon(true);
	pNPCTagRenderer = pNPCTagProtoType->AddComponent<Renderer>("NPCTagRenderer");
	pNPCTagRenderer->SetMesh("Particle");
	pNPCTagRenderer->SetRenderState("AlphaBlend");
	pNPCTagParticle = pNPCTagProtoType->AddComponent<Particle>("NPCNameTag");
	pNPCTagParticle->LoadTextureSet("Q_Mistel_NameTag", TEXT("Q_Mistel_NameTag.png"), PATH_UI_NPCNAME);

	SAFE_RELEASE(pNPCTagParticle);
	SAFE_RELEASE(pNPCTagRenderer);
	SAFE_RELEASE(pNPCTagTr);
	SAFE_RELEASE(pNPCTagProtoType);

	pNPCTagProtoType = GameObject::CreatePrototype("Highelf_M_Alchemist_NameTag", m_pScene);
	pNPCTagTr = pNPCTagProtoType->GetTransform();
	pNPCTagTr->SetWorldPosition(0.0f, 6.0f, 0.0f);
	pNPCTagTr->SetWorldScale(6.0f, 1.0f, 1.0f);			// 510, 42
	pNPCTagTr->SetParentOnltyPositon(true);
	pNPCTagRenderer = pNPCTagProtoType->AddComponent<Renderer>("NPCTagRenderer");
	pNPCTagRenderer->SetMesh("Particle");
	pNPCTagRenderer->SetRenderState("AlphaBlend");
	pNPCTagParticle = pNPCTagProtoType->AddComponent<Particle>("NPCNameTag");
	pNPCTagParticle->LoadTextureSet("Highelf_M_Alchemist_NameTag", TEXT("Highelf_M_Alchemist_NameTag.png"), PATH_UI_NPCNAME);

	SAFE_RELEASE(pNPCTagParticle);
	SAFE_RELEASE(pNPCTagRenderer);
	SAFE_RELEASE(pNPCTagTr);
	SAFE_RELEASE(pNPCTagProtoType);

	pNPCTagProtoType = GameObject::CreatePrototype("Popori_boy_NameTag", m_pScene);
	pNPCTagTr = pNPCTagProtoType->GetTransform();
	pNPCTagTr->SetWorldPosition(0.0f, 5.0f, 0.0f);
	pNPCTagTr->SetWorldScale(6.0f, 1.0f, 1.0f);			// 510, 42
	pNPCTagTr->SetParentOnltyPositon(true);
	pNPCTagRenderer = pNPCTagProtoType->AddComponent<Renderer>("NPCTagRenderer");
	pNPCTagRenderer->SetMesh("Particle");
	pNPCTagRenderer->SetRenderState("AlphaBlend");
	pNPCTagParticle = pNPCTagProtoType->AddComponent<Particle>("NPCNameTag");
	pNPCTagParticle->LoadTextureSet("Popori_boy_NameTag", TEXT("Popori_boy_NameTag.png"), PATH_UI_NPCNAME);

	SAFE_RELEASE(pNPCTagParticle);
	SAFE_RELEASE(pNPCTagRenderer);
	SAFE_RELEASE(pNPCTagTr);
	SAFE_RELEASE(pNPCTagProtoType);



	//////////////////////////////////////////////////////////////////////////////////////////////////////// Static 프로토타입

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


	//////////////////////////////////////////////////////////////////////////////////////////////////////// Ani 프로토타입

	// 몬스터 프로토타입
	GameObject* pMonsterProtoType = GameObject::CreatePrototype("Monster", m_pScene);
	Monster* pMonsterProtoTypeScript = pMonsterProtoType->AddComponent<Monster>("Monster");

	SAFE_RELEASE(pMonsterProtoTypeScript);
	SAFE_RELEASE(pMonsterProtoType);

	GameObject* pHookaProtoType = GameObject::CreatePrototype("HookaFootman_A", m_pScene);
	HookaFootmanA* pHookaProtoTypeScript = pHookaProtoType->AddComponent<HookaFootmanA>("HookaFootman_A");

	SAFE_RELEASE(pHookaProtoType);
	SAFE_RELEASE(pHookaProtoTypeScript);


	GameObject* pEnchantedDollProtoType = GameObject::CreatePrototype("EnchantedDoll", m_pScene);
	EnchantedDoll* pEnchantProtoTypeScript = pEnchantedDollProtoType->AddComponent<EnchantedDoll>("EnchantedDoll");

	SAFE_RELEASE(pEnchantedDollProtoType);
	SAFE_RELEASE(pEnchantProtoTypeScript);

	GameObject* pMonsterPivotProtoType = GameObject::CreatePrototype("MonsterPivot", m_pScene);
	MovePivot* pMonsterPivotScript = pMonsterPivotProtoType->AddComponent<MovePivot>("MonsterPivot");
	SAFE_RELEASE(pMonsterPivotProtoType);
	SAFE_RELEASE(pMonsterPivotScript);

	//GameObject* pBossMotherProtoType = GameObject::CreatePrototype("Kumas_Mother", m_pScene);
	//BossMonster* pBossMotherProtoTypeScript = pBossMotherProtoType->AddComponent<BossMonster>("Kumas_Mother");
	//SAFE_RELEASE(pBossMotherProtoType);
	//SAFE_RELEASE(pBossMotherProtoTypeScript);

	pNewProtoType = GameObject::CreatePrototype("FloatingCastle", m_pScene);
	pNewTr = pNewProtoType->GetTransform();
	pNewTr->SetWorldScale(0.05f, 0.05f, 0.05f);
	pNewTr->SetWorldPosition(Vector3::Zero);
	pNewRenderer = pNewProtoType->AddComponent<Renderer>("NewRenderer");
	pNewRenderer->SetMesh("FloatingCastle", TEXT("FloatingCastle.msh"), Vector3::Axis[AXIS_Z], PATH_MESH);
	Animation* pNewAnimation = pNewProtoType->AddComponent<Animation>("NewAnimation");
	pNewAnimation->LoadBone("FloatingCastle.bne");
	pNewAnimation->Load("FloatingCastle.anm");
	pNewAnimation->ChangeClip("Idle");

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
	ShopNPC* pShopNPCScript = pNewProtoType->AddComponent<ShopNPC>("Highelf_M_Alchemist");
	SAFE_RELEASE(pShopNPCScript);
	SAFE_RELEASE(pNewProtoType);

	pNewProtoType = GameObject::CreatePrototype("Human_F_Aide", m_pScene);
	PasserbyNPC* pPasserbyNPCScript = pNewProtoType->AddComponent<PasserbyNPC>("Human_F_Aide");
	SAFE_RELEASE(pPasserbyNPCScript);
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
	GoddessNPC* pMistelScript = pNewProtoType->AddComponent<GoddessNPC>("Q_Mistel");
	SAFE_RELEASE(pMistelScript);
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

	return true;
}