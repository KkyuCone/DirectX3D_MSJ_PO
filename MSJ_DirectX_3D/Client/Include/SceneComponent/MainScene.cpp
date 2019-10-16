#include "MainScene.h"
#include "PathManager.h"
#include "Device.h"

MainScene::MainScene() :
	pDefaultLayer(nullptr),
	pPlayerObject(nullptr),
	pPlayerFaceObject(nullptr), pPlayerHairObject(nullptr), pPlayerFaceScript(nullptr), pPlayerHairScript(nullptr),
	pPlayerWeaponColHitStartObject(nullptr), pPlayerWeaponHitColStartScript(nullptr),
	pPlayerWeaponColHitMidObject(nullptr), pPlayerWeaponHitColMidScript(nullptr),
	pPlayerCameraPivotObject(nullptr), pPlayerShotObject(nullptr), pPlayerShotScript(nullptr),
	pMonsterObject(nullptr),
	pHookaObject(nullptr),
	pHookaPivotObject(nullptr),
	pParticleObject(nullptr),
	pPlayerScript(nullptr),
	pMainCameraObject(nullptr),
	pHPBarBGObj(nullptr), pHPBarBGObj02(nullptr), pEXPBarBGObj(nullptr),
	pHPBarUIObj(nullptr), pMPBarUIObj(nullptr), pEXPBarUIObj(nullptr),
	pHpBarScript(nullptr), pMPBarScript(nullptr), pEXPBarScript(nullptr),
	pPlayerWeaponObject(nullptr),
	pPlayerWeaponScript(nullptr),
	pPlayerCameraPivotScript(nullptr),
	pQuickSlotBGObj(nullptr),
	pQuickSlotScript(nullptr),
	pInventoryObj(nullptr),
	pInventoryScript(nullptr),
	pMouseISObject(nullptr),
	pMouseISScript(nullptr),
	pBossUIObject(nullptr), pBossUIScript(nullptr), m_pMotherBossMonster(nullptr), m_pMotehrBossScript(nullptr),
	m_pMotherLeftHand(nullptr), m_pMotherRightHand(nullptr),
	m_pMotherLeftHandScript(nullptr), m_pMotherRightHandScript(nullptr), m_pMotherShotObject(nullptr), m_pMotherShotColScript(nullptr),
	pShopManagerObject(nullptr), pShopManagerScript(nullptr), pNPCBtManagerObject(nullptr), pNPCBtManagerScript(nullptr),
	pDialogueManagerObject(nullptr), pDialogueManagerScript(nullptr),
	pMiniQuestManagerObject(nullptr), pMiniQuestManagerScript(nullptr),
	pAngelQuestObject(nullptr), pAngelQuestScript(nullptr)
{
	pHPText = nullptr;
	pMPText = nullptr;
	pEXPText = nullptr;

	m_pHPUIBar = nullptr;
	m_pMPUIBar = nullptr;
	m_pEXPUIBar = nullptr;
}


MainScene::~MainScene()
{
	SAFE_RELEASE(pPlayerScript);
	SAFE_RELEASE(pPlayerObject);
	SAFE_RELEASE(pPlayerFaceScript);
	SAFE_RELEASE(pPlayerHairScript);
	SAFE_RELEASE(pPlayerFaceObject);
	SAFE_RELEASE(pPlayerHairObject);
	SAFE_RELEASE(pPlayerWeaponObject);
	SAFE_RELEASE(pPlayerCameraPivotObject);
	SAFE_RELEASE(pPlayerWeaponScript);
	SAFE_RELEASE(pPlayerShotScript);
	SAFE_RELEASE(pPlayerShotObject);

	SAFE_RELEASE(pMonsterObject);
	SAFE_RELEASE(pHookaObject);
	SAFE_RELEASE(pHookaPivotObject);
	SAFE_RELEASE(pParticleObject);
	SAFE_RELEASE(pLandScapeObject);

	SAFE_RELEASE(pMainCameraObject);
	SAFE_RELEASE(pPlayerCameraPivotScript);

	SAFE_RELEASE(pPlayerWeaponHitColStartScript);
	SAFE_RELEASE(pPlayerWeaponColHitStartObject);
	SAFE_RELEASE(pPlayerWeaponHitColMidScript);
	SAFE_RELEASE(pPlayerWeaponColHitMidObject);


	// UI
	SAFE_RELEASE(pHPBarBGObj);
	SAFE_RELEASE(pHPBarBGObj02);
	SAFE_RELEASE(pEXPBarBGObj);

	SAFE_RELEASE(pHPText);
	SAFE_RELEASE(pMPText);
	SAFE_RELEASE(pEXPText);

	SAFE_RELEASE(pHpBarScript);
	SAFE_RELEASE(pMPBarScript);
	SAFE_RELEASE(pEXPBarScript);

	SAFE_RELEASE(m_pHPUIBar);
	SAFE_RELEASE(m_pMPUIBar);
	SAFE_RELEASE(m_pEXPUIBar);

	SAFE_RELEASE(pHPBarUIObj);
	SAFE_RELEASE(pMPBarUIObj);
	SAFE_RELEASE(pEXPBarUIObj);

	SAFE_RELEASE(pQuickSlotBGObj);
	SAFE_RELEASE(pQuickSlotScript);

	SAFE_RELEASE(pInventoryObj);
	SAFE_RELEASE(pInventoryScript);

	SAFE_RELEASE(pMouseISScript);
	SAFE_RELEASE(pMouseISObject);

	SAFE_RELEASE(pBossUIScript);
	SAFE_RELEASE(pBossUIObject);

	SAFE_RELEASE(pShopManagerScript);
	SAFE_RELEASE(pShopManagerObject);

	SAFE_RELEASE(pNPCBtManagerScript);
	SAFE_RELEASE(pNPCBtManagerObject);

	SAFE_RELEASE(pDialogueManagerScript);
	SAFE_RELEASE(pDialogueManagerObject);

	SAFE_RELEASE(pMiniQuestManagerScript);
	SAFE_RELEASE(pMiniQuestManagerObject);

	SAFE_RELEASE(pAngelQuestScript);
	SAFE_RELEASE(pAngelQuestObject);

	// 오브젝트 리스트들 지워주기
	// Static
	std::list<GameObject*>::iterator StartObjIter = m_StaticObjList.begin();
	std::list<GameObject*>::iterator EndObjIter = m_StaticObjList.end();

	for (; StartObjIter != EndObjIter; ++StartObjIter)
	{
		SAFE_RELEASE((*StartObjIter));
	}

	m_StaticObjList.clear();

	// Fun
	StartObjIter = m_MoveFunList.begin();
	EndObjIter = m_MoveFunList.end();

	for (; StartObjIter != EndObjIter; ++StartObjIter)
	{
		SAFE_RELEASE((*StartObjIter));
	}

	m_MoveFunList.clear();

	// Base Monster   m_EnchantMonsterList
	StartObjIter = m_HookaMonsterList.begin();
	EndObjIter = m_HookaMonsterList.end();

	for (; StartObjIter != EndObjIter; ++StartObjIter)
	{
		SAFE_RELEASE((*StartObjIter));
	}

	m_HookaMonsterList.clear();

	StartObjIter = m_HookaMonsterAttackColList.begin();
	EndObjIter = m_HookaMonsterAttackColList.end();

	for (; StartObjIter != EndObjIter; ++StartObjIter)
	{
		SAFE_RELEASE((*StartObjIter));
	}

	m_HookaMonsterAttackColList.clear();

	StartObjIter = m_EnchantMonsterList.begin();
	EndObjIter = m_EnchantMonsterList.end();

	for (; StartObjIter != EndObjIter; ++StartObjIter)
	{
		SAFE_RELEASE((*StartObjIter));
	}

	m_EnchantMonsterList.clear();


	// Boss
	StartObjIter = m_BossMosnterList.begin();
	EndObjIter = m_BossMosnterList.end();

	for (; StartObjIter != EndObjIter; ++StartObjIter)
	{
		SAFE_RELEASE((*StartObjIter));
	}

	m_BossMosnterList.clear();

	// NPC
	StartObjIter = m_NPCList.begin();
	EndObjIter = m_NPCList.end();

	for (; StartObjIter != EndObjIter; ++StartObjIter)
	{
		SAFE_RELEASE((*StartObjIter));
	}

	m_NPCList.clear();


	// 몬스터 피봇 오브젝트들 삭제
	StartObjIter = m_HookaMonsterPivotList.begin();
	EndObjIter = m_HookaMonsterPivotList.end();

	for (; StartObjIter != EndObjIter; ++StartObjIter)
	{
		SAFE_RELEASE((*StartObjIter));
	}

	m_HookaMonsterPivotList.clear();

	StartObjIter = m_EnchantMonsterPivotList.begin();
	EndObjIter = m_EnchantMonsterPivotList.end();

	for (; StartObjIter != EndObjIter; ++StartObjIter)
	{
		SAFE_RELEASE((*StartObjIter));
	}

	m_EnchantMonsterPivotList.clear();

	StartObjIter = m_MonsterPivotList.begin();
	EndObjIter = m_MonsterPivotList.end();

	for (; StartObjIter != EndObjIter; ++StartObjIter)
	{
		SAFE_RELEASE((*StartObjIter));
	}

	m_MonsterPivotList.clear();

	SAFE_RELEASE(m_pMotherLeftHand);
	SAFE_RELEASE(m_pMotherRightHand);
	SAFE_RELEASE(m_pMotherLeftHandScript);
	SAFE_RELEASE(m_pMotherRightHandScript);
	SAFE_RELEASE(m_pMotherShotObject);
	SAFE_RELEASE(m_pMotherShotColScript);
	SAFE_RELEASE(m_pMotherBossMonster);
	SAFE_RELEASE(m_pMotehrBossScript);
}

bool MainScene::Init()
{
	// Collision - Channel
	CollisionChannelInit();

	// Collision - Profile
	CollisionProfileInit();
	CollisionProfileChannelStateInit();


	// 레이어
	LayerInit();

	// 프로토타입
	ProtoTypeInit();
	LoadStaticMesh();

	// 플레이어
	PlayerInit();

	LoadAniMesh();

	// 몬스터
	MonsterInit();

	// 파티클
	ParticleInit();

	// 지형
	if (true == LandScapeInit())
	{
		LoadNavMesh();
	}

	// 3인칭 카메라
	TPCameraInit();

	// 인스턴싱 테스트
	//InstancingTestInit();

	// UI  & Sound
	UIInit();
	BGMInit();

	return true;
}

int MainScene::Input(float _fTime)
{
	return 0;
}

int MainScene::Update(float _fTime)
{
	return 0;
}

int MainScene::LateUpdate(float _fTime)
{
	return 0;
}

void MainScene::CollisionChannelInit()
{
	// Channel 생성
	GET_SINGLETON(CollisionManager)->AddChannel("Player");
	GET_SINGLETON(CollisionManager)->AddChannel("Enemy");
	GET_SINGLETON(CollisionManager)->AddChannel("PlayerAttack");
	GET_SINGLETON(CollisionManager)->AddChannel("EnemyAttack");
	GET_SINGLETON(CollisionManager)->AddChannel("PickSphere");
	GET_SINGLETON(CollisionManager)->AddChannel("Trace");
	GET_SINGLETON(CollisionManager)->AddChannel("NPC");
}

void MainScene::CollisionProfileInit()
{
	//  Profile 생성
	GET_SINGLETON(CollisionManager)->CreateProfile("Player");
	GET_SINGLETON(CollisionManager)->CreateProfile("Enemy");
	GET_SINGLETON(CollisionManager)->CreateProfile("Trace");
	GET_SINGLETON(CollisionManager)->CreateProfile("NPC");
}

void MainScene::CollisionProfileChannelStateInit()
{
	// SetProfileChannelState(프로파일명, 채널명, 채널상태)
	GET_SINGLETON(CollisionManager)->SetProfileChannelState("Player", "Player", CCS_BLOCK);
	GET_SINGLETON(CollisionManager)->SetProfileChannelState("Player", "WorldStatic", CCS_BLOCK);
	GET_SINGLETON(CollisionManager)->SetProfileChannelState("Player", "WorldDynamic", CCS_BLOCK);
	GET_SINGLETON(CollisionManager)->SetProfileChannelState("Player", "Enemy", CCS_BLOCK);
	GET_SINGLETON(CollisionManager)->SetProfileChannelState("Player", "EneymyAttack", CCS_BLOCK);
	GET_SINGLETON(CollisionManager)->SetProfileChannelState("Player", "PickSphere", CCS_IGNORE);
	GET_SINGLETON(CollisionManager)->SetProfileChannelState("Player", "Trace", CCS_BLOCK);
	GET_SINGLETON(CollisionManager)->SetProfileChannelState("Player", "NPC", CCS_BLOCK);

	GET_SINGLETON(CollisionManager)->SetProfileChannelState("Enemy", "Enemy", CCS_BLOCK);
	GET_SINGLETON(CollisionManager)->SetProfileChannelState("Enemy", "WorldStatic", CCS_BLOCK);
	GET_SINGLETON(CollisionManager)->SetProfileChannelState("Enemy", "WorldDynamic", CCS_BLOCK);
	GET_SINGLETON(CollisionManager)->SetProfileChannelState("Enemy", "Player", CCS_BLOCK);
	GET_SINGLETON(CollisionManager)->SetProfileChannelState("Enemy", "PlayerAttack", CCS_BLOCK);
	GET_SINGLETON(CollisionManager)->SetProfileChannelState("Enemy", "PickSphere", CCS_IGNORE);

	GET_SINGLETON(CollisionManager)->SetProfileChannelState("Trace", "Player", CCS_BLOCK);
	GET_SINGLETON(CollisionManager)->SetProfileChannelState("Trace", "Trace", CCS_IGNORE);

	GET_SINGLETON(CollisionManager)->SetProfileChannelState("NPC", "NPC", CCS_IGNORE);
	GET_SINGLETON(CollisionManager)->SetProfileChannelState("NPC", "Player", CCS_OVERLAP);
	GET_SINGLETON(CollisionManager)->SetProfileChannelState("NPC", "WorldStatic", CCS_IGNORE);
	GET_SINGLETON(CollisionManager)->SetProfileChannelState("NPC", "WorldDynamic", CCS_IGNORE);
	GET_SINGLETON(CollisionManager)->SetProfileChannelState("NPC", "Enemy", CCS_IGNORE);
	GET_SINGLETON(CollisionManager)->SetProfileChannelState("NPC", "EneymyAttack", CCS_IGNORE);
	GET_SINGLETON(CollisionManager)->SetProfileChannelState("NPC", "PickSphere", CCS_IGNORE);
	GET_SINGLETON(CollisionManager)->SetProfileChannelState("NPC", "Trace", CCS_IGNORE);
}

bool MainScene::LayerInit()
{
	pDefaultLayer = m_pScene->FindLayer("Default");
	return true;
}

bool MainScene::PlayerInit()
{
	pPlayerObject = GameObject::CreateObject("Player", pDefaultLayer);
	pPlayerScript = pPlayerObject->AddComponent<Player>("Player");

	pPlayerFaceObject = GameObject::CreateObject("PlayerFace", pDefaultLayer);
	pPlayerFaceObject->SetParent(pPlayerObject);
	pPlayerFaceScript = pPlayerFaceObject->AddComponent<PlayerFace>("PlayerFace");

	pPlayerScript->SetFaceObject(pPlayerFaceObject);
	pPlayerScript->SetFaceScript(pPlayerFaceScript);

	pPlayerHairObject = GameObject::CreateObject("PlayerHair", pDefaultLayer);
	pPlayerHairObject->SetParent(pPlayerObject);
	pPlayerHairScript = pPlayerHairObject->AddComponent<PlayerHair>("PlayerHair");

	pPlayerScript->SetHairObject(pPlayerHairObject);
	pPlayerScript->SetHairScript(pPlayerHairScript);

	pPlayerWeaponObject = GameObject::CreateObject("PlayerWeapon", pDefaultLayer);
	pPlayerWeaponObject->SetParent(pPlayerObject);
	pPlayerWeaponScript = pPlayerWeaponObject->AddComponent<Weapon>("PlayerWeapon");

	pPlayerWeaponColHitStartObject = GameObject::CreateObject("PlayerWeaponHitCol", pDefaultLayer);
	pPlayerWeaponColHitStartObject->SetParent(pPlayerObject);
	pPlayerWeaponHitColStartScript = pPlayerWeaponColHitStartObject->AddComponent<PivotHitCol>("PlayerWeaponHitCol");
	pPlayerWeaponHitColStartScript->SetSoketbone("Bip01-R-Hand", "Player", "Player");
	Transform* pHitColStartTr = pPlayerWeaponColHitStartObject->GetTransform();
	pHitColStartTr->SetLocalPosition(0.0f, 35.0f, 0.0f);

	pPlayerWeaponColHitMidObject = GameObject::CreateObject("PlayerWeaponHitCol", pDefaultLayer);
	pPlayerWeaponColHitMidObject->SetParent(pPlayerObject);
	pPlayerWeaponHitColMidScript = pPlayerWeaponColHitMidObject->AddComponent<PivotHitCol>("PlayerWeaponHitCol");
	pPlayerWeaponHitColMidScript->SetSoketbone("Bip01-R-Hand", "Player", "Player");
	Transform* pHitColMidTr = pPlayerWeaponColHitMidObject->GetTransform();
	pHitColMidTr->SetLocalPosition(0.0f, 50.0f, 0.0f);

	pPlayerScript->SetFirstHitCol(pPlayerWeaponColHitStartObject);
	pPlayerScript->SetFirstHitColScript(pPlayerWeaponHitColStartScript);
	pPlayerScript->SetSecondHitCol(pPlayerWeaponColHitMidObject);
	pPlayerScript->SetSceondHitColScript(pPlayerWeaponHitColMidScript);

	SAFE_RELEASE(pHitColStartTr);
	SAFE_RELEASE(pHitColMidTr);

	pPlayerShotObject = GameObject::CreateObject("PlayerShotCol", pDefaultLayer);
	pPlayerShotObject->SetParent(pPlayerObject);
	pPlayerShotScript = pPlayerShotObject->AddComponent<PivotHitCol>("PlayerShotCol");
	pPlayerShotScript->SetSoketbone("Bip01-Spine3", "Player", "Player", "PlayerShotCol");
	pPlayerShotScript->SetHitColInfo(Vector3::Zero, 1.0f);

	pPlayerScript->SetShotHitCol(pPlayerShotObject);
	pPlayerScript->SetShotHitColScript(pPlayerShotScript);


	// 조각상
	pAngelQuestObject = GameObject::CreateObject("AngelStatue", pDefaultLayer);
	pAngelQuestScript = pAngelQuestObject->AddComponent<QuestAngelStatue>("AngelStatue");
	return true;
}

bool MainScene::MonsterInit()
{
	//pMonsterObject = GameObject::CreateClone("Monster", m_pScene, pDefaultLayer);

	//Transform* pMonTransform = pMonsterObject->GetTransform();
	//pMonTransform->SetWorldPosition(15.0f, 1.0f, 30.0f);
	//pMonTransform->LookAt(pPlayerObject);

	//SAFE_RELEASE(pMonTransform);
	//SAFE_RELEASE(pMonsterObject);

	// Hooka
	//pHookaObject = GameObject::CreateClone("HookaFootman_A", m_pScene, pDefaultLayer);

	//Transform* pHookaTransform = pHookaObject->GetTransform();
	//pHookaTransform->SetWorldPosition(15.0f, 1.0f, 60.0f);
	//pHookaTransform->LookAt(pPlayerObject);

	////pHookaPivotObject = GameObject::CreateClone("MosnterPivot", m_pScene, pDefaultLayer);
	////pHookaPivotObject->SetParent(pHookaObject);
	////pHookaPivotObject->SetBoneSoket("Bone05");

	//HookaFootmanA* pHookaObjectScript = pHookaObject->FindComponentFromType<HookaFootmanA>(CT_USERCOMPONENT);
	////pHookaObjectScript->SetMonsterPivotObject(pHookaPivotObject);

	//SAFE_RELEASE(pHookaObjectScript);
	//SAFE_RELEASE(pHookaTransform);
	//SAFE_RELEASE(pHookaObject);


	// 보스몬스터 Hit Col 충돌체 생성 및 셋팅
	if (nullptr != m_pMotherBossMonster)
	{
		// 피봇이동하는 애니메이션때문에 이동용 오브젝트 추가
		GameObject* pMonsterPivotObj = GameObject::CreateClone("MonsterPivot", m_pScene, pDefaultLayer);
		pMonsterPivotObj->SetParent(m_pMotherBossMonster);
		pMonsterPivotObj->SetBoneSoket("Bip01-Spine1");

		BossMonster* pScript = m_pMotherBossMonster->FindComponentFromTag<BossMonster>("Kumas_Mother");
		pScript->SetPivotObject(pMonsterPivotObj);

		// 왼손
		m_pMotherLeftHand = GameObject::CreateObject("Boss_LeftHandHitCol", pDefaultLayer);
		m_pMotherLeftHand->SetParent(m_pMotherBossMonster);
		m_pMotherLeftHandScript = m_pMotherLeftHand->AddComponent<PivotHitCol>("Boss_LeftHandHitCol");
		//m_pMotherLeftHandScript->SetSoketbone("Bone01", "WorldDynamic", "WorldDynamic");
		m_pMotherLeftHandScript->SetSoketbone("Bone032", "Enemy", "Enemy", "Boss_HitCollider", false);
		m_pMotherLeftHandScript->SetHitColInfo(Vector3::Zero, 4.5f);
		pScript->SetHitLHandCollider(m_pMotherLeftHand, m_pMotherLeftHandScript);

		// 오른손
		m_pMotherRightHand = GameObject::CreateObject("Boss_RightHandHitCol", pDefaultLayer);
		m_pMotherRightHand->SetParent(m_pMotherBossMonster);
		m_pMotherRightHandScript = m_pMotherRightHand->AddComponent<PivotHitCol>("Boss_RightHandHitCol");
		//m_pMotherRightHandScript->SetSoketbone("Bone666777", "WorldDynamic", "WorldDynamic");
		m_pMotherRightHandScript->SetSoketbone("Bone04m", "Enemy", "Enemy", "Boss_HitCollider", false);
		m_pMotherRightHandScript->SetHitColInfo(Vector3::Zero, 4.5f);
		pScript->SetHitRHandCollider(m_pMotherRightHand, m_pMotherRightHandScript);

		// 피격용 만들어주기
		m_pMotherShotObject = GameObject::CreateObject("Boss_ShotCol", pDefaultLayer);
		m_pMotherShotObject->SetParent(m_pMotherBossMonster);
		m_pMotherShotColScript = m_pMotherShotObject->AddComponent<PivotHitCol>("Boss_ShotCol");
		m_pMotherShotColScript->SetSoketbone("Bip01-Spine1", "Enemy", "Enemy");
		m_pMotherShotColScript->SetHitColInfo(Vector3::Zero, 10.0f);
		pScript->SetShotObject(m_pMotherShotObject);
		SAFE_RELEASE(m_pMotherShotColScript);

		SAFE_RELEASE(pScript);
		SAFE_RELEASE(pMonsterPivotObj);
	}


	// 후카 몬스터 공격 충돌체
	std::list<GameObject*>::iterator Iter = m_HookaMonsterList.begin();
	std::list<GameObject*>::iterator EndIter = m_HookaMonsterList.end();
	m_HookaMonsterAttackColList.resize(m_HookaMonsterList.size());
	for (; Iter != EndIter; ++Iter)
	{
		HookaFootmanA* pScript = (*Iter)->FindComponentFromTag<HookaFootmanA>("HookaFootman_A");

		GameObject* pMonAtkObject = GameObject::CreateObject("Hooka_HitCol", pDefaultLayer);
		pMonAtkObject->SetParent(*Iter);
		Transform* pMonAtkTr = pMonAtkObject->GetTransform();
		pMonAtkTr->SetLocalPosition(15.0f, 0.0f, 0.0f);
		PivotHitCol* pMonAtkScript = pMonAtkObject->AddComponent<PivotHitCol>("Hooka_HitCol");
		pMonAtkScript->SetSoketbone("FxCenter", "Enemy", "Enemy", "Hooka_HitCollider", false);
		pMonAtkScript->SetHitColInfo(Vector3::Zero, 2.5f);
		pScript->SetHitCollider(pMonAtkScript);

		m_HookaMonsterAttackColList.push_back(pMonAtkObject);
		SAFE_RELEASE(pScript);
		SAFE_RELEASE(pMonAtkTr);
		SAFE_RELEASE(pMonAtkScript);
	}

	return true;
}

bool MainScene::ParticleInit()
{
	pParticleObject = GameObject::CreateClone("Explosion", m_pScene, pDefaultLayer);
	Transform* pTr = pParticleObject->GetTransform();
	pTr->SetWorldPosition(0.0f, 30.0f, 0.0f);
	SAFE_RELEASE(pTr);
	return true;
}

bool MainScene::LandScapeInit()
{
	pLandScapeObject = GameObject::CreateObject("LandScape", pDefaultLayer);
	pLandScapeObject->SetRenderGroup(RG_LANDSCAPE);
	LandScape*	pLandScape = pLandScapeObject->AddComponent<LandScape>("LandScape");

	pLandScape->CreateLandScape("LandScape", "LandScape/MyHeightMap02.bmp");
	//pLandScape->CreateLandScape("LandScape", "LandScape/height3.bmp");


	pLandScape->AddSplatTexture(TEXT("LandScape/BD_Terrain_Cliff05.dds"),
		TEXT("LandScape/BD_Terrain_Cliff05_NRM.bmp"),
		TEXT("LandScape/BD_Terrain_Cliff05_SPEC.bmp"),
		TEXT("LandScape/RoadAlpha.bmp"));
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

	return true;
}

bool MainScene::TPCameraInit()
{
	pMainCameraObject = m_pScene->GetMainCameraObject();

	// 플레이어
	pPlayerCameraPivotObject = GameObject::CreateObject("PlayerPivot", pDefaultLayer);
	pPlayerCameraPivotObject->SetParent(pPlayerObject);

	TPCamera* pTPCamera = pMainCameraObject->AddComponent<TPCamera>("ThirdPersonCamera");
	pTPCamera->SetMouseEnable(true);
	pTPCamera->SetTarget(pPlayerCameraPivotObject);

	pPlayerCameraPivotScript = pPlayerCameraPivotObject->AddComponent<PlayerCameraPivot>("PlayerPivot");
	pPlayerScript->SetMoveAniChildObject(pPlayerCameraPivotObject);
	pPlayerScript->SetMoveAniChildScript(pPlayerCameraPivotScript);

	pPlayerScript->SetTPCam(pTPCamera);

	SAFE_RELEASE(pTPCamera);

	return true;
}

bool MainScene::UIInit()
{
	Layer*	pUILayer = m_pScene->FindLayer("UI");
	PlayerStateInit(pUILayer);
	ShopInit(pUILayer);
	NPCBtInit(pUILayer);
	DialogueInit(pUILayer);
	MiniQuestInit(pUILayer);
	return true;
}

bool MainScene::PlayerStateInit(Layer* _pLayer)
{
	// 마우스 스킬, 아이템 이미지 관련 오브젝트
	pMouseISObject = GameObject::CreateObject("Mouse_ItemSkillObj", _pLayer);
	pMouseISScript = pMouseISObject->AddComponent<MouseItemSkill>("MouseItemSkillScript");

	// UI - 상태바
	// HP,MP 배경 오브젝트
	pHPBarBGObj = GameObject::CreateObject("StateBar_BG", _pLayer);
	UIBar* pHPBarBG = pHPBarBGObj->AddComponent<UIBar>("StateBar_BG");
	pHPBarBG->SetTexture("StateBar_BG", TEXT("ab2_bonus_frame02.png"), PATH_UI_GAGEBAR);
	pHPBarBG->SetShader(SHADER_UI_BAR);

	Transform* pHPBGTr = pHPBarBGObj->GetTransform();
	pHPBGTr->SetWorldScale(738.0f, 184.8f, 1.0f);
	pHPBGTr->SetWorldPosition(_RESOLUTION.iWidth / 2.f - 369.0f, _RESOLUTION.iHeight / 2.f - 410.f, 2.0f);

	SAFE_RELEASE(pHPBarBG);
	SAFE_RELEASE(pHPBGTr);

	pHPBarBGObj02 = GameObject::CreateObject("StateBar_BG02", _pLayer);
	UIBar* pHPBarBG02 = pHPBarBGObj02->AddComponent<UIBar>("StateBar_BG02");
	pHPBarBG02->SetTexture("StateBar_BG02", TEXT("ab2_bonus_frame_globe_borders.png"), PATH_UI_GAGEBAR);
	pHPBarBG02->SetShader(SHADER_UI_BAR);

	Transform* pHPBGTr02 = pHPBarBGObj02->GetTransform();
	pHPBGTr02->SetWorldScale(166.8f, 166.8f, 1.0f);
	pHPBGTr02->SetWorldPosition(_RESOLUTION.iWidth * 0.5f - 82.0f, _RESOLUTION.iHeight * 0.5f - 390.0f, 1.3f);

	SAFE_RELEASE(pHPBarBG02);
	SAFE_RELEASE(pHPBGTr02);

	// HP 오브젝트
	pHPBarUIObj = GameObject::CreateObject("HPBar", _pLayer);
	m_pHPUIBar = pHPBarUIObj->AddComponent<UIBar>("HPBar");
	m_pHPUIBar->SetTexture("HPBarGage", TEXT("ab2_bonus_frame_globe_fill_red.png"), PATH_UI_GAGEBAR);
	m_pHPUIBar->SetShader(SHADER_UI_BAR);
	m_pHPUIBar->SetHeightEnable(true);
	pHpBarScript = pHPBarUIObj->AddComponent<StateBar>("UI_StateBar_HP");
	//pHpBarScript->SetMin(0);
	//pHpBarScript->SetMax(10000);
	pHpBarScript->SetUIBar(m_pHPUIBar);
	pPlayerScript->SetHPObject(pHPBarUIObj);
	pPlayerScript->SetHPScript(pHpBarScript);

	Transform* pHPTr = pHPBarUIObj->GetTransform();
	pHPTr->SetWorldScale(81.0f, 162.0f, 1.0f);
	pHPTr->SetWorldPosition(_RESOLUTION.iWidth * 0.5f - 80.0f, _RESOLUTION.iHeight * 0.5f - 389.f, 1.5f);

	SAFE_RELEASE(m_pHPUIBar);
	SAFE_RELEASE(pHpBarScript);
	SAFE_RELEASE(pHPTr);


	// MP
	pMPBarUIObj = GameObject::CreateObject("MPBar", _pLayer);
	m_pMPUIBar = pMPBarUIObj->AddComponent<UIBar>("MPBar");
	m_pMPUIBar->SetTexture("MPBarGage", TEXT("ab2_bonus_frame_globe_fill_blue.png"), PATH_UI_GAGEBAR);
	m_pMPUIBar->SetShader(SHADER_UI_BAR);
	m_pMPUIBar->SetHeightEnable(true);
	pMPBarScript = pMPBarUIObj->AddComponent<StateBar>("UI_StateBar_MP");
	//pMPBarScript->SetMin(0);
	//pMPBarScript->SetMax(10000);
	pMPBarScript->SetUIBar(m_pMPUIBar);
	pPlayerScript->SetMPObject(pMPBarUIObj);
	pPlayerScript->SetMPScript(pMPBarScript);

	Transform* pMPTr = pMPBarUIObj->GetTransform();
	pMPTr->SetWorldScale(81.0f, 162.0f, 1.0f);
	pMPTr->SetWorldPosition(_RESOLUTION.iWidth * 0.5f - 1.0f, _RESOLUTION.iHeight * 0.5f - 389.f, 1.5f);

	SAFE_RELEASE(m_pMPUIBar);
	SAFE_RELEASE(pMPBarScript);
	SAFE_RELEASE(pMPTr);


	////  UI - 텍스트
	//// 상태바
	//pHPText = GameObject::CreateObject("HPText", pDefaultLayer);
	//Transform* pHPTextTr = pHPText->GetTransform();
	//pHPTextTr->SetWorldPosition(pHPTr->GetWorldPosition());

	//UIText* pHPUIText = pHPText->AddComponent<UIText>("Text");
	//pHPUIText->SetText(TEXT("HP"));
	//pHPUIText->Shadow(true);
	//pHPUIText->SetShadowColor(1.0f, 0.0f, 1.0f, 1.0f);
	//pHPUIText->SetShadowOffset(Vector3(2.f, -2.f, 0.f));
	//pHPUIText->SetShadowOpacity(1.f);
	//pHPUIText->SetFont("Gungseo");
	//pHPUIText->SetColor(1.f, 1.f, 0.f, 1.f);
	//pHPUIText->SetOpacity(0.5f);
	//pHPUIText->AlphaBlend(true);
	//pHPUIText->SetRenderArea(0.f, 0.f, 300.f, 50.f);

	//pHpBarScript->SetUIText(pHPUIText);			// 연결

	//pMPText = GameObject::CreateObject("MPText", pDefaultLayer);
	//Transform* pMPTextTr = pMPText->GetTransform();
	//pMPTextTr->SetWorldPosition(pHPTr->GetWorldPosition());
	//UIText* pMPUIText = pMPText->AddComponent<UIText>("Text");
	//pMPUIText->SetText(TEXT("MP"));
	//pMPUIText->Shadow(true);
	//pMPUIText->SetShadowColor(1.0f, 0.0f, 1.0f, 1.0f);
	//pMPUIText->SetShadowOffset(Vector3(2.f, -2.f, 0.f));
	//pMPUIText->SetShadowOpacity(1.f);
	//pMPUIText->SetFont("Gungseo");
	//pMPUIText->SetColor(1.f, 1.f, 0.f, 1.f);
	//pMPUIText->SetOpacity(0.5f);
	//pMPUIText->AlphaBlend(true);
	//pMPUIText->SetRenderArea(0.f, 0.f, 300.f, 50.f);

	//pMPBarScript->SetUIText(pMPUIText);			// 연결

	// Release
	/*SAFE_RELEASE(pHPTr);
	SAFE_RELEASE(pMPTr);

	SAFE_RELEASE(pHPUIText);

	SAFE_RELEASE(pHPTextTr);*/

	//SAFE_RELEASE(pMPUIText);


	// EXP 바
	// HP,MP 배경 오브젝트
	pEXPBarBGObj = GameObject::CreateObject("EXPBar_BG", _pLayer);
	UIBar* pEXPBarBG = pEXPBarBGObj->AddComponent<UIBar>("EXPBar_BG");
	pEXPBarBG->SetTexture("EXPBar_BG", TEXT("ab2_xp_empty_frame.png"), PATH_UI_GAGEBAR);
	pEXPBarBG->SetShader(SHADER_UI_BAR);

	Transform* pEXPBGTr = pEXPBarBGObj->GetTransform();
	pEXPBGTr->SetWorldScale(1711.2f, 43.2f, 1.0f);
	pEXPBGTr->SetWorldPosition(_RESOLUTION.iWidth * 0.5f - 855.0f, _RESOLUTION.iHeight * 0.5f - 540.f, 1.5f);

	SAFE_RELEASE(pEXPBarBG);
	SAFE_RELEASE(pEXPBGTr);

	// EXP 오브젝트
	pEXPBarUIObj = GameObject::CreateObject("EXPBar", _pLayer);
	m_pEXPUIBar = pEXPBarUIObj->AddComponent<UIBar>("EXPBar");
	m_pEXPUIBar->SetTexture("EXPBarGage", TEXT("ab2_xp_fill.png"), PATH_UI_GAGEBAR);
	m_pEXPUIBar->SetShader(SHADER_UI_BAR);
	pEXPBarScript = pEXPBarUIObj->AddComponent<StateBar>("UI_StateBar_EXP");
	pEXPBarScript->SetMin(0);
	pEXPBarScript->SetMax(10000);
	pEXPBarScript->SetUIBar(m_pEXPUIBar);
	pPlayerScript->SetEXPObject(pEXPBarUIObj);
	pPlayerScript->SetEXPScript(pEXPBarScript);

	Transform* pEXPTr = pEXPBarUIObj->GetTransform();
	pEXPTr->SetWorldScale(1678.4f, 21.6f, 1.0f);
	pEXPTr->SetWorldPosition(_RESOLUTION.iWidth * 0.5f - 841.5f, _RESOLUTION.iHeight * 0.5f - 533.f, 1.4f);

	SAFE_RELEASE(m_pEXPUIBar);
	SAFE_RELEASE(pHpBarScript);
	SAFE_RELEASE(pEXPTr);


	// 퀵슬롯
	pQuickSlotBGObj = GameObject::CreateObject("QuickSlot", _pLayer);
	pQuickSlotScript = pQuickSlotBGObj->AddComponent<QuickSlot>("QuickSlot");
	pMouseISScript->SetQuickslotScript(pQuickSlotScript);
	Transform* pQuickSlotTR = pQuickSlotBGObj->GetTransform();
	pQuickSlotTR->SetWorldScale(1162.2f, 139.8f, 1.0f);
	pQuickSlotTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f - 590.0f, _RESOLUTION.iHeight * 0.5f - 528.f, 1.4f);
	SAFE_RELEASE(pQuickSlotTR);


	// 인벤토리
	pInventoryObj = GameObject::CreateObject("Inventory", _pLayer);
	pInventoryScript = pInventoryObj->AddComponent<Inventory>("Inventory");
	pMouseISScript->SetInventoryScript(pInventoryScript);
	pInventoryScript->SetPlayerWeaponScript(pPlayerWeaponScript);
	Transform* pInventoryObjTR = pInventoryObj->GetTransform();
	pInventoryObjTR->SetWorldPosition(_RESOLUTION.iWidth * 0.5f, _RESOLUTION.iHeight * 0.5f, 1.0f);
	pInventoryObjTR->SetWorldScale(1.0f, 1.0f, 1.0f);

	SAFE_RELEASE(pInventoryObjTR);

	pPlayerScript->SetInventoryObject(pInventoryObj);
	pPlayerScript->SetInventoryScript(pInventoryScript);
	pInventoryScript->SetMouseItemSkillScript(pMouseISScript);
	pQuickSlotScript->SetMouseItemSkillScript(pMouseISScript);


	// 보스 몬스터 UI
	pBossUIObject = GameObject::CreateObject("BossStateUIManager", _pLayer);
	pBossUIScript = pBossUIObject->AddComponent<BossUIState>("BossStateUIManager");

	m_pMotehrBossScript->SetUIBossObject(pBossUIObject);
	m_pMotehrBossScript->SetUIBossScript(pBossUIScript);
	return true;
}

bool MainScene::ShopInit(Layer* _pLayer)
{
	pShopManagerObject = GameObject::CreateObject("ShopManager", _pLayer);
	pShopManagerScript = pShopManagerObject->AddComponent<ShopManager>("ShopManager");
	pShopManagerScript->ShopOff();
	pShopManagerScript->SetInventory(pInventoryScript);
	return true;
}

bool MainScene::NPCBtInit(Layer* _pLayer)
{
	pNPCBtManagerObject = GameObject::CreateObject("NPCButtonManager", _pLayer);
	pNPCBtManagerScript = pNPCBtManagerObject->AddComponent<NPCTalkBtManager>("NPCButtonManager");
	pNPCBtManagerScript->SetShopManager(pShopManagerScript);
	pNPCBtManagerScript->SetInventory(pInventoryScript);

	// 각 NPC에 맞는 버튼 스크립트 Set
	// 상점, 여신, 포포리들
	std::list<GameObject*>::iterator Iter = m_NPCList.begin();
	std::list<GameObject*>::iterator EndIter = m_NPCList.end();

	for (; Iter != EndIter; ++Iter)
	{
		std::string strObjTag = (*Iter)->GetTag();
		if (strObjTag == "Highelf_M_Alchemist")
		{
			// 상점 NPC
			ShopNPC* pScript = (*Iter)->FindComponentFromTag<ShopNPC>("Highelf_M_Alchemist");
			if (nullptr != pScript)
			{
				pScript->SetNPCBtScript(pNPCBtManagerScript);
			}
			SAFE_RELEASE(pScript);
		}
		else if (strObjTag == "Popori_boy")
		{
			// 포포리
			//PoporiBoyNPC* pScript = (*Iter)->FindComponentFromTag<PoporiBoyNPC>("Highelf_M_Alchemist");
			//if (nullptr != pScript)
			//{
			//	pScript->SetNPCBtScript(pNPCBtManagerScript);
			//}
			//SAFE_RELEASE(pScript);
		}
		else if (strObjTag == "Q_Mistel")
		{
			// 여신
			GoddessNPC* pScript = (*Iter)->FindComponentFromTag<GoddessNPC>("Q_Mistel");
			if (nullptr != pScript)
			{
				pScript->SetNPCBtScript(pNPCBtManagerScript);
			}
			SAFE_RELEASE(pScript);
		}
		else if (strObjTag == "Human_F_Aide")
		{
			// 지나가는 행인
			PasserbyNPC* pScript = (*Iter)->FindComponentFromTag<PasserbyNPC>("Human_F_Aide");
			if (nullptr != pScript)
			{
				pScript->SetNPCBtScript(pNPCBtManagerScript);
			}
			SAFE_RELEASE(pScript);
		}
	}
	return true;
}

bool MainScene::DialogueInit(Layer* _pLayer)
{
	pDialogueManagerObject = GameObject::CreateObject("DialogueManager", _pLayer);
	pDialogueManagerScript = pDialogueManagerObject->AddComponent<DialogueManager>("DialogueManager");
	pDialogueManagerScript->SetQuickSlotScript(pQuickSlotScript);

	pDialogueManagerScript->PushPalyerObject(pHPBarBGObj);
	pDialogueManagerScript->PushPalyerObject(pHPBarBGObj02);
	pDialogueManagerScript->PushPalyerObject(pEXPBarBGObj);
	pDialogueManagerScript->PushPalyerObject(pHPBarUIObj);
	pDialogueManagerScript->PushPalyerObject(pMPBarUIObj);
	pDialogueManagerScript->PushPalyerObject(pEXPBarUIObj);
	//pDialogueManagerScript->PushPalyerObject(pHPText);
	//pDialogueManagerScript->PushPalyerObject(pMPText);
	//pDialogueManagerScript->PushPalyerObject(pEXPText);
	//pDialogueManagerScript->DialogueOff();

	pNPCBtManagerScript->SetDialogueManager(pDialogueManagerScript);
	return true;
}

void MainScene::MiniQuestInit(Layer * _pLayer)
{
	pMiniQuestManagerObject = GameObject::CreateObject("MiniQuestManager", _pLayer);
	pMiniQuestManagerScript = pMiniQuestManagerObject->AddComponent<QuestManager>("MiniQuestManager");
	pDialogueManagerScript->SetQuestManagerScript(pMiniQuestManagerScript);
	pPlayerScript->SetQuestManagerScript(pMiniQuestManagerScript);
}

void MainScene::InstancingTestInit()
{
	return;
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j < 25; ++j)
		{
			// 스태틱 인스턴싱
			//GameObject*	pAnjangObj = GameObject::CreateClone("Anjang",
			//	m_pScene, pDefaultLayer);

			//Transform*	pAnjangTr = pAnjangObj->GetTransform();

			//pAnjangTr->SetWorldScale(0.02f, 0.02f, 0.02f);
			//pAnjangTr->SetWorldPosition(j * 2.f, 6.f, 7.f + i * 2.f);

			//SAFE_RELEASE(pAnjangTr);
			//SAFE_RELEASE(pAnjangObj);

			// 2D애니메이션 , 프레임 애니메이션 인스턴싱Rain
			GameObject* pParticleObj = GameObject::CreateClone("Explosion", m_pScene, pDefaultLayer);
			//GameObject* pParticleObj = GameObject::CreateClone("Rain", m_pScene, pDefaultLayer);
			Transform*	pParticleTr = pParticleObj->GetTransform();
			pParticleTr->SetWorldPosition(j * 2.f, 0.0f, i * 2.f);

			SAFE_RELEASE(pParticleTr);
			SAFE_RELEASE(pParticleObj);

		}
	}

}

void MainScene::BGMInit()
{
	// 배경음
	GameObject* pBGMObject = GameObject::CreateObject("MainBGM", pDefaultLayer);
	AudioSound* pBGM = pBGMObject->AddComponent<AudioSound>("MainBGM");
	pBGM->SetSound("MainBGM", m_pScene, true, "rpg_02_loop.wav", PATH_SOUND_BGM);
	pBGM->SetVolume(0.05f);
	pBGM->Play();

	SAFE_RELEASE(pBGM);
	SAFE_RELEASE(pBGMObject);
}

void MainScene::LoadStaticMesh()
{
	// 경로 찾기
	const char*		pPath = GET_SINGLETON(PathManager)->FindPathMultibyte(PATH_DATA_MAPINFO);
	char		strFullPath[MAX_PATH] = {};

	if (nullptr != pPath)
	{
		strcpy_s(strFullPath, pPath);
	}

	char strStaticFullPath[MAX_PATH] = {};
	strcpy_s(strStaticFullPath, strFullPath);

	const char * pFileName = "MainSceneObjInfo_Static.minfo";
	strcat_s(strStaticFullPath, pFileName);

	// 파일 로드
	FILE*	pFile = nullptr;

	fopen_s(&pFile, strStaticFullPath, "rb");

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

		GameObject* pNewObject = GameObject::CreateClone(strMeshName, pDefaultLayer->GetScene(), pDefaultLayer);
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

void MainScene::LoadAniMesh()
{
	// 경로 찾기
	const char*		pPath = GET_SINGLETON(PathManager)->FindPathMultibyte(PATH_DATA_MAPINFO);
	char		strFullPath[MAX_PATH] = {};

	if (nullptr != pPath)
	{
		strcpy_s(strFullPath, pPath);
	}

	char strStaticFullPath[MAX_PATH] = {};
	strcpy_s(strStaticFullPath, strFullPath);

	const char * pFileName = "MainSceneObjInfo_Ani.minfo";
	strcat_s(strStaticFullPath, pFileName);

	// 파일 로드
	FILE*	pFile = nullptr;

	fopen_s(&pFile, strStaticFullPath, "rb");

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

		std::string strMother = strMeshName;
		if (strMother == "Kumas_Mother")
		{
			GameObject* pNewObject = GameObject::CreateObject(strMeshName, pDefaultLayer);
			m_pMotehrBossScript = pNewObject->AddComponent<BossMonster>(strMother);
			Transform*	pNewTr = pNewObject->GetTransform();
			fread(&iLength, sizeof(int), 1, pFile);
			char ClipName[256] = {};
			fread(ClipName, 1, iLength, pFile);

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

			m_pMotherBossMonster = pNewObject;
			SAFE_RELEASE(pNewTr);
			continue;
		}

		GameObject* pNewObject = GameObject::CreateClone(strMeshName, pDefaultLayer->GetScene(), pDefaultLayer);
		Transform*	pNewTr = pNewObject->GetTransform();

		// 해당 애니메이션 오브젝트 기본 애니메이션 클립명가져오기
		fread(&iLength, sizeof(int), 1, pFile);
		char ClipName[256] = {};
		fread(ClipName, 1, iLength, pFile);

		std::string strClipName = ClipName;
		Animation* pNewObjAni = pNewObject->FindComponentFromType<Animation>(CT_ANIMATION);
		if (nullptr != pNewObjAni)
		{
			pNewObjAni->ChangeClip(strClipName);
		}

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

		// 몬스터들 피봇 오브젝트들
		//GameObject* pMonsterPivotObj = GameObject::CreateObject("MonsterPivot", pDefaultLayer);
		//pMonsterPivotObj->SetParent(pNewObject);
		//MovePivot* pMonsterPivotScript = pMonsterPivotObj->AddComponent<MovePivot>("MonsterPivot");

		LOAD_ANI_TYPE eAni = (LOAD_ANI_TYPE)ieAni;
		switch (eAni)
		{
		case LAT_Kumas_Mother:
			m_pMotherBossMonster = pNewObject;
			break;
		case LAT_Kumas_Baby_Blue:
		case LAT_Kumas_Baby_Pink:
		case LAT_Kumas_Baby_Red:
			m_BossMosnterList.push_back(pNewObject);
			break;
		case LAT_EnchantedDoll:
			pNewObject->Active(false);
			m_EnchantMonsterList.push_back(pNewObject);
			break;
		case LAT_HookaFootman_A:
			pNewTr->SetLocalRotation(Vector3(0.0f, 0.0f, 0.0f));
			pNewTr->SetWorldRotation(Vector3(0.0f, 0.0f, 0.0f));
			pNewTr->SetWorldScale(0.2f, 0.2f, 0.2f);
			pNewTr->LookAt(pPlayerObject);
			m_HookaMonsterList.push_back(pNewObject);
			break;
		case LAT_Arcdeva_Gate:
		case LAT_FloatingCastle:
			m_MoveFunList.push_back(pNewObject);
			break;
		case LAT_Goat:
		case LAT_Highelf_M_Alchemist:
		case LAT_Human_F_Aide:
		case LAT_Popori_boy:
		case LAT_Popori_boy02:
		case LAT_Popori_boy03:
		case LAT_Q_Mistel:
		case LAT_Rabbit:
			m_NPCList.push_back(pNewObject);
			break;
		default:
			assert(false);
			break;
		}

		SAFE_RELEASE(pNewObjAni);
		SAFE_RELEASE(pNewTr);
	}
	fclose(pFile);

	// 몬스터 HP
	//  Hooka HP
	std::list<GameObject*>::iterator Iter = m_HookaMonsterList.begin();
	std::list<GameObject*>::iterator EndIter = m_HookaMonsterList.end();
	for (; Iter != EndIter;)
	{
		GameObject* pParentObj = (*Iter);
		GameObject* pMonsterPivotObj = GameObject::CreateClone("MonsterPivot", m_pScene, pDefaultLayer);
		pMonsterPivotObj->SetParent(pParentObj);
		pMonsterPivotObj->SetBoneSoket("Bip01-Pelvis");
		//MovePivot* pMonsterPivotScript = pMonsterPivotObj->AddComponent<MovePivot>("MonsterPivot");
		//MovePivot* pMonsterPivotScript = pMonsterPivotObj->FindComponentFromTag<MovePivot>("MonsterPivot");

		HookaFootmanA* pScript = pParentObj->FindComponentFromTag<HookaFootmanA>("HookaFootman_A");
		pScript->SetPivotObject(pMonsterPivotObj);

		Transform* pHookaTr = pParentObj->GetTransform();

		GameObject* pHpBarObj = GameObject::CreateClone("MonsterHP", m_pScene, pDefaultLayer);
		pHpBarObj->SetParent(pParentObj);
		pScript->SetHPBarObject(pHpBarObj);

		SAFE_RELEASE(pMonsterPivotObj);
		//SAFE_RELEASE(pMonsterPivotScript);
		SAFE_RELEASE(pScript);

		SAFE_RELEASE(pHpBarObj);
		SAFE_RELEASE(pHookaTr);
		++Iter;
	}

	// 고양이
	//Iter = m_EnchantMonsterList.begin();
	//EndIter = m_EnchantMonsterList.end();
	//for (; Iter != EndIter; ++Iter)
	//{
	//	GameObject* pMonsterPivotObj = GameObject::CreateObject("MonsterPivot", pDefaultLayer);
	//	pMonsterPivotObj->SetParent((*Iter));
	//	pMonsterPivotObj->SetBoneSoket("Bip01-Pelvis");
	//	MovePivot* pMonsterPivotScript = pMonsterPivotObj->AddComponent<MovePivot>("MonsterPivot");

	//	HookaFootmanA* pScript = (*Iter)->FindComponentFromTag<HookaFootmanA>("HookaFootman_A2");
	//	pScript->SetPivotObject(pMonsterPivotObj);

	//	Transform* pDollTr = (*Iter)->GetTransform();

	//	GameObject* pHpBarObj = GameObject::CreateClone("DollMonsterHP", m_pScene, pDefaultLayer);
	//	pHpBarObj->SetParent((*Iter));
	//	pScript->SetHPBarObject(pHpBarObj);

	//	SAFE_RELEASE(pMonsterPivotObj);
	//	SAFE_RELEASE(pMonsterPivotScript);
	//	SAFE_RELEASE(pScript);

	//	//SAFE_RELEASE(pHpBarObj);
	//	SAFE_RELEASE(pDollTr);
	//}


	// NPC 이름표
	Iter = m_NPCList.begin();
	EndIter = m_NPCList.end();

	for (; Iter != EndIter; ++Iter)
	{
		GameObject* pParentObj = (*Iter);
		std::string pParentTag = pParentObj->GetTag();

		if (CheckNameTagNPC(pParentObj))
		{
			std::string strNameTagImage = pParentTag;
			strNameTagImage += "_NameTag";
			std::wstring wstrImageName = StringToLPWSTR(strNameTagImage);
			GameObject* pNPCTagObj = GameObject::CreateClone(strNameTagImage, m_pScene, pDefaultLayer);
			pNPCTagObj->SetParent(pParentObj);
			SAFE_RELEASE(pNPCTagObj);
		}
	}

}

void MainScene::LoadNavMesh()
{
	if (nullptr == pLandScapeObject)
	{
		return;
	}

	LandScape* pLandScape = pLandScapeObject->FindComponentFromType<LandScape>(CT_LANDSCAPE);

	if (nullptr == pLandScape)
	{
		return;
	}

	// 경로 찾기
	const char*		pPath = GET_SINGLETON(PathManager)->FindPathMultibyte(PATH_DATA_MAPINFO);
	char		strFullPath[MAX_PATH] = {};

	if (nullptr != pPath)
	{
		strcpy_s(strFullPath, pPath);
	}

	char strStaticFullPath[MAX_PATH] = {};
	strcpy_s(strStaticFullPath, strFullPath);

	const char * pFileName = "MainSceneObjInfo_Nav.minfo";
	strcat_s(strStaticFullPath, pFileName);


	// 로드
	FILE*	pFile = nullptr;

	fopen_s(&pFile, strStaticFullPath, "rb");

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
		pLandScape->ChangeNavIndexEnable(false, iIndex);
	}

	fclose(pFile);

	SAFE_RELEASE(pLandScape);
}

bool MainScene::CheckNameTagNPC(GameObject * _pObject)
{
	std::string strName = _pObject->GetTag();
	if (strName == "Q_Mistel" || strName == "Human_F_Aide"
		|| strName == "Highelf_M_Alchemist" || strName == "Popori_boy")
	{
		return true;
	}
	return false;
}
