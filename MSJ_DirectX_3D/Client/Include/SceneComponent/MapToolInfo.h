#pragma once
#include "Scene/SceneComponent.h"

ENGINE_USING

enum ENV_STATIC
{
	STATIC_NONE = 0,
};

enum ENV_ANI
{
	ENVANI_NONE = 0,
};


class MapToolInfo : public SceneComponent
{
public:
	MapToolInfo();
	~MapToolInfo();

public:
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int Render(float _fTime);
};

