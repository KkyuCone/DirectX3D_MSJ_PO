#pragma once
#include "Component.h"

ENGINE_BEGIN

// 3��Ī ī�޶� ������Ʈ����
// 1��Ī ó���� ��� �����ϴ�. (�Ÿ� 0���� �����ϸ� ��)

class ENGINE_DLL TPCamera : public Component
{
private:
	friend class GameObject;

protected:
	TPCamera();
	TPCamera(const TPCamera& _Com);
	virtual ~TPCamera();

private:
	class GameObject*	m_pTargetObj;
	class Transform*	m_pTarget;				// Ÿ���� ��ġ
	Vector3				m_vDistance;			// Ÿ���� �ٶ󺸴� ����
	bool				m_bMouseEnable;			// ���콺 ���翩��

	bool				m_bSoketMove;			// ������ Ÿ������ ���� ����
	bool				m_bBaseNoneMove;		// Idle������ ���

	bool				m_bToolCamera;			// ��ī�޶� ����


public:
	void SetTarget(class GameObject* _pTarget);
	void SetTarget(Component* _pTarget);
	void SetDistance(const Vector3& _vDistance);
	void SetMouseEnable(bool _bEnable);
	void SetSoketMoveEnable(bool _bEnable);
	void SetBaseNoneMoveEnable(bool _bEnable);

	void SetToolCamera(bool _bValue);

public:
	GameObject* GetTargetObject();

public:
	virtual void Start();			
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);
	virtual int Render(float _fTime);
	virtual int RenderShadow(float _fTime);
	virtual TPCamera* Clone()	const;

};

ENGINE_END