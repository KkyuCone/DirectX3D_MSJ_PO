#pragma once
#include "Component\UserComponent.h"
#include "Component\Renderer.h"
#include "Component\Transform.h"
#include "Component\Material.h"
#include "Component\Animation.h"

ENGINE_USING

class PlayerHair : public UserComponent
{
private:
	// ���
	Transform*	m_pFaceTransform;
	Renderer*	m_pFaceRenderer;
	Material*	m_pFaceMaterial;
	Animation*	m_pFaceAnimation;

public:
	PlayerHair();
	PlayerHair(const PlayerHair& _Face);
	~PlayerHair();

public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��, �� ���̾�� ȣ�����ش�.
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);	// ������ �ϱ� ������ ȣ�����ִ� �Լ��Ĥ���
	virtual int Render(float _fTime);
	virtual PlayerHair* Clone()	const;

private:
	// Init �Լ���
	void ComponentInit();

public:
	void AnimationChange(std::string _strName);
};

