#pragma once
#include "Base.h"
#include "Component\Renderer.h"
#include "Component\Animation.h"
#include "Component\Transform.h"
#include "Component\Material.h"
#include "Component\Collider.h"
#include "Component\ColliderRay.h"
#include "Component\TPCamera.h"
#include "Component\Navigation.h"

ENGINE_USING

class Mover : public Base
{
private:
	float	m_pSpeed;
private:
	Renderer* m_pMoverRenderer;
	Material* m_pMoverMaterial;
	Animation*	m_pMoverAnimation;
	Navigation*	m_pMoverNavigation;

public:
	Mover();
	Mover(const Mover& _Mover);
	~Mover();

public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��, �� ���̾�� ȣ�����ش�.
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);	// ������ �ϱ� ������ ȣ�����ִ� �Լ��Ĥ���
	virtual int Render(float _fTime);
	virtual Mover* Clone()	const;

private:
	void ComponentInit();
	void InputInit();

public:
	TPCamera*	m_pTPCamera;
	void SetTPCam(TPCamera* _Camera);

public:
	Vector3 GetCurWorldMousePos();

public:
	void MoveFront(float _fTime);
	void MoveBack(float _fTime);
	void MoveLeft(float _fTime);
	void MoveRight(float _fTime);

	void MoveUp(float _fTime);
	void MoveDown(float _fTime);

	// Axis
	void Rotate(float _fScale, float _fTime);

public:
	void MoveZero(float _fTime);

public:
	// �ӵ�����
	void SpeedMultiple(float _fTime);		// �ӵ� * 2.0f
	void SpeedDivision(float _fTime);		// �ӵ� * 0.5f

};

