#include "Player.h"
#include "Scene\Scene.h"
#include "Scene\Layer.h"
#include "Time.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Bullet.h"
#include "Component\ColliderSphere.h"
#include "Component\ColliderOBB.h"
#include "Component\Navigation.h"
#include "Navigation\NavigationManager.h"
#include "Navigation\NavigationMesh.h"
#include "Render\RenderManager.h"
#include "PlayerFace.h"
#include "PlayerHair.h"
#include "Weapon.h"

/////////////////////////////////////////////////////////////////// �⺻ ����

void Player::StateChangeIdle(float _fTime)
{
}

void Player::StateChangeRun(float _fTime)
{
}

void Player::StateChangeWait(float _fTime)
{
}

void Player::StateChangeDeath(float _fTime)
{
	m_ePlayerState = BaseState::DEATH;
	AnimationChange("Death");
}

void Player::StateChangeDeathWait(float _fTime)
{
}

void Player::StateChangeNockBack(float _fTime)
{
}

void Player::StateChangeGroggy(float _fTime)
{
}

void Player::StateChangeTumbling(float _fTime)
{
}

/////////////////////////////////////////////////////////////////// ��ų
// ��ų ���´�, Idle, Run, �⺻ ������ ��쿡�� ��ų���·� �ٲ��ش�.
// Death, DeathWait, NockBack, Groggy, Tumbling + �ٸ� ��ų ��� ���� ��쿡�� ��� �Ұ����ϴ�.

void Player::StateChangeSkill01(float _fTime)
{
	if (!CheckChangeSkill(BaseState::SKILL01))
	{
		return;
	}
	
	if (true == AnimationChange("CuttingSlash"))
	{
		UseMP(80);
	}
}

void Player::StateChangeSkill02(float _fTime)
{
	if (!CheckChangeSkill(BaseState::SKILL02))
	{
		return;
	}
	if (true == AnimationChange("StingerBlade"))
	{
		UseMP(80);
	}
}

void Player::StateChangeSkill03(float _fTime)
{
	if (!CheckChangeSkill(BaseState::SKILL03))
	{
		return;
	}
	if (true == AnimationChange("RisingAttack"))
	{
		UseMP(80);
	}
}

void Player::StateChangeSkill04(float _fTime)
{
	if (!CheckChangeSkill(BaseState::SKILL04))
	{
		return;
	}
	if (true == AnimationChange("WindCutter1R"))
	{
		UseMP(80);
	}
}

void Player::StateChangeSkill05(float _fTime)
{
	if (!CheckChangeSkill(BaseState::SKILL05))
	{
		return;
	}
	if (true == AnimationChange("WindCutter3"))
	{
		UseMP(80);
	}
}

/////////////////////////////////////////////////////////////////// �Ϲ� ����

void Player::StateChangeCombo1(float _fTime)
{
}

void Player::StateChangeCombo2(float _fTime)
{
}

void Player::StateChangeCombo3(float _fTime)
{
}

void Player::StateChangeCombo4(float _fTime)
{
}