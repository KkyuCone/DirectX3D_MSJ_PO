#pragma once
#include "Component\Collider.h"
#include "Component\ColliderRay.h"
#include "Component\TPCamera.h"
#include "Component\AudioSound.h"
#include "Base.h"
#include "StateBar.h"

ENGINE_USING

class Player : public Base
{
private:
	BaseState		m_ePlayerState;					// �ִϸ��̼� �� ���� ����
	BaseUIState		m_tPlayerUIState;				// HP, MP, EXP ����
	float			m_fSpeed;
	bool			m_bNPCBtClick;

private:
	// ��ų ��Ÿ�� ����
	bool			m_bComputeCoolTime[5];
	float			m_fArrSkillCollTime[5];
	float			m_fArrCurCoolTime[5];
	BaseState		m_iBeforeSkill;					// ��ų ��Ÿ�� - ���� ��ų üũ��
	BaseState		m_iCurSKillNumber;				// ��ų ��Ÿ�� - ���� ��ų üũ��


private:
	// ��
	Transform* m_pBodyTransform;
	Renderer* m_pBodyRenderer;
	Material* m_pBodyMaterial;
	Animation*	m_pBodyAnimation;
	ColliderRay* m_pMouseRay;				// ���콺����, �̵��Ҷ� �ʿ�

public:
	void ReNavLateUpdate(float _fTime);

private:
	class GameObject*	m_pFaceObject;
	class GameObject*	m_pHairObject;
	class GameObject*	m_pWeaponObject;

	class PlayerFace*	m_pFaceScript;
	class PlayerHair*	m_pHairScript;
	class Weapon*		m_pWeaponScript;

	// �̵��ϴ� �ִϸ��̼��� �� �ʿ��� �ڽ� ������Ʈ
	class GameObject*			m_pMoveAniChildObj;
	class PlayerCameraPivot*	m_pMoveAniChildScript;
	Transform*					m_pMoveAniChildTR;

	bool m_bMoveAni;
	Vector3 m_vMoveAniChild;

private:
	// UI
	class GameObject*	m_pInventoryObject;
	class Inventory*	m_pInventoryScript;
	class QuestManager*	m_pQuestManagerScript;

private:
	TPCamera*			m_pTPCam;

private:
	class GameObject*	m_pFirstHitCol;
	class GameObject*	m_pSecondHitCol;
	class GameObject*	m_pShotCol;

	class PivotHitCol* m_pFirstHitColScript;
	class PivotHitCol* m_pSecondHitColScript;
	class PivotHitCol* m_pShotColScript;

private:
	// ����
	class GameObject*	m_pEffectSoundObject;
	AudioSound*			m_pEffectSound;

	class GameObject*	m_pSkillSoundObject[5];
	AudioSound*			m_pSkillSound[5];
public:
	bool GetMoveAni();

private:
	// ����� �ܼ��� ������
	// ���ξ����� �����ǰ� ����
	GameObject* m_pHPObject;
	StateBar*	m_pHPScript;

	GameObject* m_pMPObject;
	StateBar*	m_pMPScript;

	GameObject*	m_pEXPObject;
	StateBar*	m_pEXPScript;

public:
	Player();
	Player(const Player& _Player);
	~Player();

public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��, �� ���̾�� ȣ�����ش�.
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);	// ������ �ϱ� ������ ȣ�����ִ� �Լ��Ĥ���
	virtual int Render(float _fTime);
	virtual Player* Clone()	const;

	// Get, Set ����
public:
	void SetFaceObject(class GameObject* _Object);
	void SetHairObject(class GameObject* _Object);
	void SetWeaponObject(class GameObject* _Object);

	void SetFaceScript(class PlayerFace* _Script);
	void SetHairScript(class PlayerHair* _Script);
	void SetWeaponScript(class Weapon* _Script);

	void SetMoveAniChildObject(class GameObject* _Object);
	void SetMoveAniChildScript(class PlayerCameraPivot* _Script);

	// UI
	void SetInventoryObject(class GameObject* _Object);
	void SetInventoryScript(class Inventory* _Script);

	void SetHPObject(GameObject* _Object);
	void SetHPScript(StateBar* _Script);

	void SetMPObject(GameObject* _Object);
	void SetMPScript(StateBar* _Script);

	void SetEXPObject(GameObject* _Object);
	void SetEXPScript(StateBar* _Script);

	void SetQuestManagerScript(QuestManager* _Script);

	// TPCam
	void SetTPCam(TPCamera* _TPCam);

public:
	BaseState GetCurrentState();
	bool GetFButtonClick();

public:
	// �浹ü ����
	void SetFirstHitCol(GameObject* _pHitCol);
	void SetSecondHitCol(GameObject* _pHitCol);
	void SetShotHitCol(GameObject* _pHitCol);

	void SetFirstHitColScript(class PivotHitCol* _Script);
	void SetSceondHitColScript(class PivotHitCol* _Script);
	void SetShotHitColScript(class PivotHitCol* _Script);

private:
	// Init �Լ���
	void InputInit();
	void ComponentInit();

public:
	void MoveFront(float _fTime);
	void MoveBack(float _fTime);
	void MoveNone(float _fTime);
	void MoveRight(float _fScale, float _fTime);
	void Rotate(float _fScale, float _fTime);
	void Fire(float _fTime);
	void Test(float _fTime);
	void FButtonClick(float _fScale, float _fTime);

public:
	// ���� ĳ���� ���� �Լ� (�̵�, ���� ��)
	bool AnimationChange(std::string _strName);

public:
	void HDR_ON(float _fScale, float _fTime);
	void HDR_OFF(float _fScale, float _fTime);

	void SSAO_ON(float _fScale, float _fTime);
	void SSAO_OFF(float _fScale, float _fTime);

	void DepthFog_ON(float _fScale, float _fTime);
	void DepthFog_OFF(float _fScale, float _fTime);

	void Shadow_ON(float _fScale, float _fTime);
	void Shadow_OFF(float _fScale, float _fTime);


private:
	void TestHitEnter(Collider* _pSrc, Collider* _pDest, float _fTime);
	void TestHitStay(Collider* _pSrc, Collider* _pDest, float _fTime);
	void TestHitExit(Collider* _pSrc, Collider* _pDest, float _fTime);

	void HitPick(Collider* _pSrc, Collider* _pDest, float _fTime);
	void HitPickLeave(Collider* _pSrc, Collider* _pDest, float _fTime);

private:
	// UI ����
	void InventoryOnOff(float _fTime);

private:
	// ���� �ٲ� �Լ�
	void StateChangeIdle(float _fTime);
	void StateChangeRun(float _fTime);
	void StateChangeWait(float _fTime);
	void StateChangeDeath(float _fTime);
	void StateChangeDeathWait(float _fTime);
	void StateChangeNockBack(float _fTime);
	void StateChangeGroggy(float _fTime);
	void StateChangeTumbling(float _fTime);

	void StateChangeSkill01(float _fTime);
	void StateChangeSkill02(float _fTime);
	void StateChangeSkill03(float _fTime);
	void StateChangeSkill04(float _fTime);
	void StateChangeSkill05(float _fTime);

	void StateChangeCombo1(float _fTime);
	void StateChangeCombo2(float _fTime);
	void StateChangeCombo3(float _fTime);
	void StateChangeCombo4(float _fTime);

private:
	bool ChangeableSkill(float _fTime);				// ��ų ���¸� �ٲܼ� �ִ��� ���� �Ǵ�
	bool ComputeSkillCoolTime(float _fTime);		// ��ų ��Ÿ�� ���
	void ChangeSkillToIdle();

	bool CheckChangeSkill(BaseState _State);
	bool UseSkill();

private:
	// ���¿� ���� �Լ�
	void Idle(float _fTime);
	void Run(float _fTime);
	void Wait(float _fTime);
	void Death(float _fTime);
	void DeathWait(float _fTime);
	void NockBack(float _fTime);
	void Groggy(float _fTime);
	void Tumbling(float _fTime);

	void Skill01(float _fTime);
	void Skill02(float _fTime);
	void Skill03(float _fTime);
	void Skill04(float _fTime);
	void Skill05(float _fTime);

	void Combo1(float _fTime);
	void Combo2(float _fTime);
	void Combo3(float _fTime);
	void Combo4(float _fTime);

	void Shot(float _fTime);

	// �Ǻ� ��ȭ�� ���� 3��Ī ī�޶� ��ġ �̵� (������ �������϶� �̵� )
	void AnimationPivotMove();

public:
	// �׽�Ʈ���Լ�
	void DecHP(float _fTime);
	void TestAddItem_PotionHP(float _fTime);
	void TestAddItem_PotionMP(float _fTime);
	void Damage(int _Value, bool _bGroggy = false);
	void Heal(int _Value);
	void UseMP(int _Value);			// ���� ���
	void FillMP(int _Value);
	void AddEXP(int _Value);

	public:
		void TPCameraOnOff();
		void HitColOnOff(bool _Value);
};

