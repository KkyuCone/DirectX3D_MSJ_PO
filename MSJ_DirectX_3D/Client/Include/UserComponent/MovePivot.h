#pragma once
#include "Component\Collider.h"
#include "Component\ColliderOBB.h"
#include "Component\ColliderSphere.h"
#include "Base.h"

ENGINE_USING

class MovePivot : public Base
{
private:
	bool m_bParentBone;
	std::string m_strParentBone;

private:
	ColliderSphere* m_pHitCol;

public:
	MovePivot();
	MovePivot(const MovePivot& _MovePivot);
	~MovePivot();

public:
	virtual void Start();	
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);
	virtual int Render(float _fTime);
	virtual MovePivot* Clone()	const;

private:
	void ComponentInit();
};

