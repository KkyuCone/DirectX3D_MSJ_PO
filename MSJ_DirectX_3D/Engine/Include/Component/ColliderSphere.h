#pragma once
#include "Collider.h"

ENGINE_BEGIN

class ENGINE_DLL ColliderSphere :
	public Collider
{
private:
	friend class GameObject;

protected:
	ColliderSphere();
	ColliderSphere(const ColliderSphere& _Com);
	virtual ~ColliderSphere();

private:
	SphereInfo	m_tRelative;			// ������� OffSet
	SphereInfo	m_tInfo;				// �� �浹ü ����
	bool		m_bScaleEnable;			// �� ������ ������ ������ ���������� ���� bool��

public:
	SphereInfo GetInfo()	const;

public:
	void SetSphereInfo(const Vector3& _vCenter, float _fRadius);
	void SetSphereInfo(const Vector3& _vCenter);
	void SetSphereInfo(float _fRadius);
	void SetScaleEnable(bool _bEnable);

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
	virtual ColliderSphere* Clone()	const;
};

ENGINE_END