#pragma once
#include "Component\Collider.h"
#include "Base.h"

ENGINE_USING

class Weapon : public Base
{
private:
	Renderer* m_pWeaponRenderer;
	Material* m_pWeaponMaterial;

	bool m_bParentBone;
	bool m_bHit;
	std::string m_strParentBone;

public:
	Weapon();
	Weapon(const Weapon& _Weapon);
	~Weapon();

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);
	virtual int Render(float _fTime);
	virtual Weapon* Clone()	const;

private:
	void ComponentInit();

public:
	void ChangeWeapon(std::string _Name);

};

