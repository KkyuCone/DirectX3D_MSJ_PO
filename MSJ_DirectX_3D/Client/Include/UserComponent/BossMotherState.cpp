#include "BossMonster.h"
#include "Device.h"
#include "Scene\Scene.h"
#include "Scene\Layer.h"
#include "Time.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Component\ColliderSphere.h"
#include "Component\ColliderOBB.h"
#include "Component\Navigation.h"
#include "Component\Camera.h"
#include "Component\TPCamera.h"
#include "Component\Particle.h"
#include "Render\RenderManager.h"
#include "Navigation\NavigationManager.h"
#include "Navigation\NavigationMesh.h"
#include "PivotHitCol.h"
#include "BossUIState.h"
#include "Player.h"


void BossMonster::Idle(float _fTime)
{
	// Idle -> Walk, Wait
	ChangeAnimation("Idle");

	if (true == m_pBossAnimation->IsAnimationEnd())
	{
		bool bReWalk = RandomCheck(0.5f);

		if (true == bReWalk)
		{
			// Walk
			m_eState = BaseState::WALK;
		}
		else
		{
			// Wait
			m_eState = BaseState::WAIT;
		}
	}

}

void BossMonster::Walk(float _fTime)
{
	ChangeAnimation("Walk");

	if (m_fCurWalkTime >= m_fWalkTime)
	{
		bool bReWalk = RandomCheck(0.7f);

		if (true == bReWalk)
		{
			m_eState = BaseState::WALK;
			UpdateWalkDir();
		}
		else
		{
			m_eState = BaseState::IDLE;
			m_fCurWalkTime = 0.0f;
		}
	}
	else
	{
		m_fCurWalkTime += _fTime;
		m_pTransform->Move(AXIS_X, 1.0f, _fTime);
	}
}

void BossMonster::Run(float _fTime)
{
	ChangeAnimation("Run");
}

void BossMonster::Wait(float _fTime)
{
	ChangeAnimation("Wait");
	ChangeAnimationToIdle(_fTime);
}

void BossMonster::Death(float _fTime)
{
	ChangeAnimation("Death");
	m_bTrace = false;
	m_pTransform->SetLookAtActive(false);
	m_pBossNavigation->SetAIFindPath(false);

	if (true == m_pBossAnimation->IsAnimationEnd())
	{
		m_pUIBossScript->BossUIOnOff(false);
		m_pGameObject->Active(false);
	}
}

void BossMonster::Groggy(float _fTime)
{
	ChangeAnimation("Groggy");
	ChangeAnimationToIdle(_fTime);
}

void BossMonster::Attack01(float _fTime)
{
	PANIMATIONCLIP pCurClip = m_pBossAnimation->GetCurrentClip();

	if ("Atk01" == pCurClip->strName)
	{
		//pCurClip->iChangeFrame
		// 이미 돌리고 있는 애니메이션이라면
		if (pCurClip->iChangeFrame >= 31 && pCurClip->iChangeFrame < 37)
		{
			m_pHitSound->Play();
		}

		if (pCurClip->iChangeFrame >= 56 && pCurClip->iChangeFrame < 63)
		{
			m_pHitSound->Play();
		}
	}


	ChangeAnimation("Atk01");
	ChangeAnimationToIdle(_fTime);
}

void BossMonster::Attack02(float _fTime)
{
	PANIMATIONCLIP pCurClip = m_pBossAnimation->GetCurrentClip();

	if ("Atk02" == pCurClip->strName)
	{
		//pCurClip->iChangeFrame
		// 이미 돌리고 있는 애니메이션이라면
		if (pCurClip->iChangeFrame >= 70 && pCurClip->iChangeFrame < 80)
		{
			m_pHitSound->Play();
		}
	}
	ChangeAnimation("Atk02");
	ChangeAnimationToIdle(_fTime);
}

void BossMonster::Attack03(float _fTime)
{
	PANIMATIONCLIP pCurClip = m_pBossAnimation->GetCurrentClip();
	if ("HeavyAtk" == pCurClip->strName)
	{
		//pCurClip->iChangeFrame
		// 이미 돌리고 있는 애니메이션이라면
		if (pCurClip->iChangeFrame >= 51 && pCurClip->iChangeFrame < 58)
		{
			m_pHitSound->Play();
		}
		else if (pCurClip->iChangeFrame >= 75 && pCurClip->iChangeFrame < 80)
		{
			m_pHitSound->Play();
		}

		else if (pCurClip->iChangeFrame >= 120 && pCurClip->iChangeFrame < 125)
		{
			m_pHitSound->Play();
		}
	}
	ChangeAnimation("HeavyAtk");
	ChangeAnimationToIdle(_fTime);
}

void BossMonster::Attack04(float _fTime)
{
	PANIMATIONCLIP pCurClip = m_pBossAnimation->GetCurrentClip();
	if ("HeavyAtk02" == pCurClip->strName)
	{
		//pCurClip->iChangeFrame
		// 이미 돌리고 있는 애니메이션이라면
		if (pCurClip->iChangeFrame >= 23 && pCurClip->iChangeFrame < 30)
		{
			m_pHitSound->Play();
		}
		else if (pCurClip->iChangeFrame >= 70 && pCurClip->iChangeFrame < 78)
		{
			m_pHitSound->Play();
		}
		else if (pCurClip->iChangeFrame >= 92 && pCurClip->iChangeFrame < 97)
		{
			m_pHitSound->Play();
		}
		else if (pCurClip->iChangeFrame >= 103 && pCurClip->iChangeFrame < 108)
		{
			m_pHitSound->Play();
		}
		else if (pCurClip->iChangeFrame >= 115 && pCurClip->iChangeFrame < 120)
		{
			m_pHitSound->Play();
		}
		else if (pCurClip->iChangeFrame >= 115 && pCurClip->iChangeFrame < 120)
		{
			m_pHitSound->Play();
		}
		else if (pCurClip->iChangeFrame >= 139 && pCurClip->iChangeFrame < 144)
		{
			m_pHitSound->Play();
		}
	}
	ChangeAnimation("HeavyAtk02");
	ChangeAnimationToIdle(_fTime);
}

void BossMonster::Attack05(float _fTime)
{
	PANIMATIONCLIP pCurClip = m_pBossAnimation->GetCurrentClip();
	if ("HeavyAtk03" == pCurClip->strName)
	{
		//pCurClip->iChangeFrame
		// 이미 돌리고 있는 애니메이션이라면0
		if (pCurClip->iChangeFrame >= 32 && pCurClip->iChangeFrame < 41)
		{
			m_pHitSound->Play();
		}
		else if (pCurClip->iChangeFrame >= 48 && pCurClip->iChangeFrame < 56)
		{
			m_pHitSound->Play();
		}
		else if (pCurClip->iChangeFrame >= 62 && pCurClip->iChangeFrame < 70)
		{
			m_pHitSound->Play();
		}
		else if (pCurClip->iChangeFrame >= 78 && pCurClip->iChangeFrame < 85)
		{
			m_pHitSound->Play();
		}
	}
	ChangeAnimation("HeavyAtk03");
	ChangeAnimationToIdle(_fTime);
}

void BossMonster::Shot(float _fTime)
{
	ChangeAnimation("Flinch");
	if (ChangeAnimationToIdle(_fTime))
	{
		Vector4 vColor = Vector4(10.0f, 10.0f, 10.0f, 1.0f);
		//m_pBossMaterial->SetColor(vColor, vColor, vColor, Vector4::One, 5.0f);
		m_bShot = false;
	}
}

void BossMonster::HealStart(float _fTime)
{
	m_bAttack = false;
	ChangeAnimation("SleepStart");
	PANIMATIONCLIP pCurClip = m_pBossAnimation->GetCurrentClip();
	if (true == m_pBossAnimation->IsAnimationEnd() && pCurClip->strName == "SleepStart")
	{
		m_eState = BaseState::BOSS_HEAL_LOOP;
	}
}

void BossMonster::HealLoop(float _fTime)
{
	ChangeAnimation("SleepLoop");

	PANIMATIONCLIP pCurClip = m_pBossAnimation->GetCurrentClip();
	if (true == m_pBossAnimation->IsAnimationEnd() && pCurClip->strName == "SleepLoop")
	{
		m_eState = BaseState::BOSS_HEAL_END;
	}
}

void BossMonster::HealEnd(float _fTime)
{
	ChangeAnimation("SleepEnd");

	PANIMATIONCLIP pCurClip = m_pBossAnimation->GetCurrentClip();
	if (true == m_pBossAnimation->IsAnimationEnd() && pCurClip->strName == "SleepEnd")
	{
		m_bDotHeal = false;
		m_bAttack = false;
		m_eState = BaseState::IDLE;
	}
}

void BossMonster::MoveAttack(float _fTime)
{
	ChangeAnimation("MoveAtk");
	if (ChangeAnimationToIdle(_fTime))
	{
		MovePivotPos(_fTime);
	}
}

void BossMonster::RollingAttackStart(float _fTime)
{
	ChangeAnimation("RollingFStart");
	if (true == m_pBossAnimation->IsAnimationEnd())
	{
		m_eState = BaseState::BOSS_ROLLING_ATK_LOOP;
	}
}

void BossMonster::RollingAttackLoop(float _fTime)
{
	ChangeAnimation("RollingFLoop");
	if (true == m_pBossAnimation->IsAnimationEnd())
	{
		m_eState = BaseState::BOSS_ROLLING_ATK_END;
		MovePivotPos(_fTime);
	}
}

void BossMonster::RollingAttackEnd(float _fTime)
{
	ChangeAnimation("RollingFEnd");
	if (ChangeAnimationToIdle(_fTime))
	{
		MovePivotPos(_fTime);
	}
}

void BossMonster::ChangeAnimation(std::string _strAniName)
{
	PANIMATIONCLIP pCurAniClip = m_pBossAnimation->GetCurrentClip();

	if (pCurAniClip->strName != _strAniName)
	{
		m_pBossAnimation->ChangeClip(_strAniName);
	}
}

bool BossMonster::ChangeAnimationToIdle(float _fTime)
{
	// 해당 애니메이션 실행이 끝나면 바로 Idle모드 변경
	if (true == m_pBossAnimation->IsAnimationEnd())
	{
		m_eState = BaseState::IDLE;
		MovePivotPos(_fTime);
		m_bAttack = false;
		m_bTrace = true;
		return true;
	}
	return false;
}

