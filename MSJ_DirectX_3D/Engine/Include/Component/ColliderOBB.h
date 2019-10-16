#pragma once
#include "Collider.h"

ENGINE_BEGIN

class ENGINE_DLL ColliderOBB :
	public Collider
{
private:
	friend class GameObject;

protected:
	ColliderOBB();
	ColliderOBB(const ColliderOBB& _Com);
	virtual ~ColliderOBB();
	
private:
	OBBInfo m_tRelative;
	OBBInfo m_tInfo;

	Vector3 m_vOBBRot;
	Matrix m_matOBBRot;

public:
	void SetOBBInfo(const Vector3& _vCenter, const Vector3& _vHalfLength);
	OBBInfo GetInfo()	const;

	void SetOBBRotation(Vector3 _vRot);

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
	virtual ColliderOBB* Clone()	const;

};

ENGINE_END
