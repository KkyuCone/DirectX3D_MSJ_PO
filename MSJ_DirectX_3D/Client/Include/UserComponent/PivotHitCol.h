#pragma once
#include "Component\Collider.h"
#include "Component\ColliderOBB.h"
#include "Component\ColliderSphere.h"
#include "Base.h"

ENGINE_USING

class PivotHitCol : public Base
{
private:
	bool m_bAttack;
private:
	ColliderSphere* m_pHitCol;

public:
	PivotHitCol();
	PivotHitCol(const PivotHitCol& PivotHitCol);
	~PivotHitCol();

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);
	virtual int Render(float _fTime);
	virtual PivotHitCol* Clone()	const;

private:
	void ComponentInit();

private:
	// 충돌체 함수들
	void HitEnter(Collider * _pSrc, Collider * _pDest, float _fTime);
	void HitStay(Collider * _pSrc, Collider * _pDest, float _fTime);
	void HitExit(Collider * _pSrc, Collider * _pDest, float _fTime);

public:
	void SetbAttack(bool _bAttack);
	bool GetAttack();

public:
	void SetSoketbone(std::string _strBoneName, std::string _strProFile, std::string _strChannel, std::string _strColName = "HitCol", bool _AutoFunc = true);
	void SetHitColInfo(const Vector3 & _vCenter, float _fRadius);

	ColliderSphere* GetHitCollider();
};

