#pragma once
#include "Scene\SceneComponent.h"
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
#include "Component\AudioSound.h"
#include "..\UserComponent\Player.h"
#include "..\UserComponent\PlayerFace.h"
#include "..\UserComponent\PlayerHair.h"
#include "..\UserComponent\Weapon.h"
#include "..\UserComponent\PivotHitCol.h"
#include "..\UserComponent\Monster.h"
#include "..\UserComponent\HookaFootmanA.h"
#include "..\UserComponent\BossMonster.h"
#include "..\UserComponent\EnchantedDoll.h"
#include "..\UserComponent\Bullet.h"
#include "..\UserComponent\StateBar.h"
#include "..\UserComponent\PlayerCameraPivot.h"
#include "..\UserComponent\Inventory.h"
#include "..\UserComponent\QuickSlot.h"
#include "..\UserComponent\MouseItemSkill.h"
#include "..\UserComponent\MovePivot.h"
#include "..\UserComponent\BossUIState.h"
#include "..\UserComponent\ShopManager.h"
#include "..\UserComponent\ShopNPC.h"
#include "..\UserComponent\GoddessNPC.h"
#include "..\UserComponent\PoporiBoyNPC.h"
#include "..\UserComponent\PasserbyNPC.h"
#include "..\UserComponent\NPCTalkBtManager.h"
#include "..\UserComponent\DialogueManager.h"
#include "..\UserComponent\QuestManager.h"
#include "..\UserComponent\QuestAngelStatue.h"
#include "CollisionManager.h"

ENGINE_USING

enum LOAD_ANI_TYPE
{
	LAT_Arcdeva_Gate = 0,
	LAT_Kumas_Mother,
	LAT_EnchantedDoll,
	LAT_HookaFootman_A,
	LAT_FloatingCastle,
	LAT_Kumas_Baby_Blue,
	LAT_Kumas_Baby_Pink,
	LAT_Kumas_Baby_Red,
	LAT_Goat,
	LAT_Highelf_M_Alchemist,
	LAT_Human_F_Aide,
	LAT_Popori_boy,
	LAT_Popori_boy02,
	LAT_Popori_boy03,
	LAT_Q_Mistel,
	LAT_Rabbit,

	LAT_MAX
};

class MainScene : public SceneComponent
{
private:
	Layer* pDefaultLayer;

private:
	GameObject* pPlayerObject;
	GameObject* pPlayerFaceObject;
	GameObject*	pPlayerHairObject;
	GameObject*	pPlayerWeaponObject;
	GameObject* pPlayerWeaponColHitStartObject;
	GameObject* pPlayerWeaponColHitMidObject;
	GameObject* pPlayerCameraPivotObject;

	GameObject* pPlayerShotObject;
	PivotHitCol* pPlayerShotScript;

	GameObject* pMonsterObject;
	GameObject* pParticleObject;
	GameObject* pLandScapeObject;
	GameObject* pMainCameraObject;

	GameObject* pHookaObject;
	GameObject* pHookaPivotObject;

	// UI
	// UI - State ���
	GameObject* pHPBarBGObj;
	GameObject* pHPBarBGObj02;
	GameObject* pEXPBarBGObj;

	// UI - State Bar
	GameObject* pHPBarUIObj;
	GameObject* pMPBarUIObj;
	GameObject* pEXPBarUIObj;

	GameObject* pHPText;
	GameObject* pMPText;
	GameObject* pEXPText;

	UIBar* m_pHPUIBar;
	UIBar* m_pMPUIBar;
	UIBar* m_pEXPUIBar;

	// UI - ������
	GameObject* pQuickSlotBGObj;
	QuickSlot*	pQuickSlotScript;

	// UI - �κ��丮
	GameObject* pInventoryObj;
	Inventory* pInventoryScript;

	// ���콺 ��ų, ������ �̹������� ������Ʈ
	GameObject*	pMouseISObject;
	MouseItemSkill* pMouseISScript;

	// ����  UI
	GameObject*		pBossUIObject;
	BossUIState*	pBossUIScript;

	// UI - ����
	GameObject*		pShopManagerObject;
	ShopManager*	pShopManagerScript;

	// UI - NPC ��ư
	GameObject*			pNPCBtManagerObject;
	NPCTalkBtManager*	pNPCBtManagerScript;

	// UI - ��ȭâ
	GameObject*			pDialogueManagerObject;
	DialogueManager*	pDialogueManagerScript;

	// UI - �̴� ����Ʈâ
	GameObject*			pMiniQuestManagerObject;
	QuestManager*		pMiniQuestManagerScript;

	// ������ ����Ʈ��
	GameObject*			pAngelQuestObject;
	QuestAngelStatue*	pAngelQuestScript;

private:
	// ��ũ��Ʈ
	Player*				pPlayerScript;
	PlayerFace*			pPlayerFaceScript;
	PlayerHair*			pPlayerHairScript;
	Weapon*				pPlayerWeaponScript;
	PivotHitCol*	pPlayerWeaponHitColStartScript;
	PivotHitCol* pPlayerWeaponHitColMidScript;
	PlayerCameraPivot*	pPlayerCameraPivotScript;

	StateBar* pHpBarScript;
	StateBar* pMPBarScript;
	StateBar* pEXPBarScript;

private:
	// ����ƽ ������Ʈ��
	std::list<GameObject*> m_StaticObjList;

	std::list<GameObject*> m_MoveFunList;
	std::list<GameObject*> m_HookaMonsterList;
	std::list<GameObject*> m_HookaMonsterAttackColList;
	std::list<GameObject*> m_EnchantMonsterList;
	std::list<GameObject*> m_BossMosnterList;
	std::list<GameObject*> m_NPCList;

	// ���͵� �̵��ϴ� �ִϸ��̼Ƕ����� ����� �Ǻ�������Ʈ��
	std::list<GameObject*> m_HookaMonsterPivotList;
	std::list<GameObject*> m_EnchantMonsterPivotList;

	std::list<GameObject*> m_MonsterPivotList;

private:
	// ��������
	GameObject*		m_pMotherBossMonster;
	BossMonster*	m_pMotehrBossScript;

	GameObject*	m_pMotherLeftHand;
	GameObject*	m_pMotherRightHand;
	GameObject* m_pMotherShotObject;

	PivotHitCol* m_pMotherLeftHandScript;
	PivotHitCol* m_pMotherRightHandScript;
	PivotHitCol* m_pMotherShotColScript;
public:
	MainScene();
	~MainScene();

public:
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);

private:
	void CollisionChannelInit();
	void CollisionProfileInit();
	void CollisionProfileChannelStateInit();

	bool LayerInit();
	bool ProtoTypeInit();
	bool PlayerInit();
	bool MonsterInit();
	bool ParticleInit();
	bool LandScapeInit();
	bool TPCameraInit();
	bool UIInit();
	bool PlayerStateInit(Layer* _pLayer);
	bool ShopInit(Layer* _pLayer);
	bool NPCBtInit(Layer* _pLayer);
	bool DialogueInit(Layer* _pLayer);
	void MiniQuestInit(Layer* _pLayer);
	void InstancingTestInit();
	void BGMInit();

	// �ε�
	void LoadStaticMesh();
	void LoadAniMesh();
	void LoadNavMesh();

	//
	bool CheckNameTagNPC(GameObject* _pObject);

};

