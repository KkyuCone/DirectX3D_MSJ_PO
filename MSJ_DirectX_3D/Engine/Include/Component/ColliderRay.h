#pragma once
#include "Collider.h"

ENGINE_BEGIN

class ENGINE_DLL ColliderRay :
	public Collider
{
private:
	friend class GameObject;

protected:
	ColliderRay();
	ColliderRay(const ColliderRay& _Com);
	virtual ~ColliderRay();

private:
	RayInfo m_tRelative;
	RayInfo m_tInfo;
	bool m_bMouse;						// ���콺 ������ �������� �ƴ��� ����

public:
	void MouseEnable(bool _Value);
	RayInfo GetInfo() const;

public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��, �� ���̾�� ȣ�����ش�.
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual bool Collision(Collider* _pDest);
	virtual int PrevRender(float _fTime);	// ������ �ϱ� ������ ȣ�����ִ� �Լ�
	virtual int Render(float _fTime);
	virtual ColliderRay* Clone()	const;
};

ENGINE_END