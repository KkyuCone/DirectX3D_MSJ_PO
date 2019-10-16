#pragma once
#include "Component\UserComponent.h"
#include "Component\Transform.h"
#include "Component\Animation.h"
#include "Component\Camera.h"
#include "Component\TPCamera.h"

ENGINE_USING

class PlayerCameraPivot : public UserComponent
{
private:
	class GameObject* m_pParentObj;
	class Player* m_pParentScript;
	Animation* m_pParentAni;
	TPCamera* m_pTPCamera;
	Transform* m_pTPCamTR;

public:
	PlayerCameraPivot();
	PlayerCameraPivot(const PlayerCameraPivot& _PlayerCameraPivot);
	~PlayerCameraPivot();

public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��, �� ���̾�� ȣ�����ش�.
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);	// ������ �ϱ� ������ ȣ�����ִ� �Լ��Ĥ���
	virtual int Render(float _fTime);
	virtual PlayerCameraPivot* Clone()	const;

};

