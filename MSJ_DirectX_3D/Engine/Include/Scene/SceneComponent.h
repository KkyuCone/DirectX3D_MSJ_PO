#pragma once
#include "../Reference.h"

ENGINE_BEGIN

// ���� Ŭ���� -> ��ü�� �ȸ��������.
// �ڽ��� ������Ʈ�ϴ� �������� ���� �ִ�. 
// (AddComponent�� �߰�)
class ENGINE_DLL SceneComponent : public Reference
{
private:
	friend class Scene;


protected:
	SceneComponent();
	virtual ~SceneComponent() = 0;

protected:
	class Scene*	m_pScene;			// ������Ʈ���� �ڽ��� � ���� �ִ��� �˰�����

public:
	virtual bool Init() = 0;
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int Render(float _fTime);
};

ENGINE_END