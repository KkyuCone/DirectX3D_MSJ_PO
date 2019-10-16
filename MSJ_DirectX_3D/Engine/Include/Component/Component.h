#pragma once
#include "..\Reference.h"

ENGINE_BEGIN

class ENGINE_DLL Component : public Reference
{
private:
	friend class GameObject;

protected:
	Component();
	Component(const Component& _Component);
	virtual ~Component() = 0;

protected:
	class Scene*	m_pScene;
	class Layer*	m_pLayer;
	class GameObject*	m_pGameObject;
	class Transform*	m_pTransform;

public:
	class Scene*	GetScene()	const;
	class Layer*	GetLayer()	const;
	class GameObject*	GetGameObject() const;
	class Transform*	GetTransform()	const;			// SetTransform�� �ʿ���� �����Ҷ� �����ڿ��� ������ٰ��Ӥ� ����


public:
	void SetScene(class Scene* _pScene);
	void SetLayer(class Layer* _pLayer);
	void SetGameObject(class GameObject* _pGameObject);
	void SetTransform(Transform* _Transform);

	bool IsObjectEnable()	const;
	bool IsObjectActive()	const;


protected:
	COMPONENT_TYPE m_eComponentType;

public:
	COMPONENT_TYPE GetComponentType() const;

public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��, �� ���̾�� ȣ�����ش�.
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);	// ������ �ϱ� ������ ȣ�����ִ� �Լ��Ĥ���
	virtual int PrevShadowRender(Matrix _matLightView, Matrix _matLightProj, float _fTime);
	virtual int Render(float _fTime);
	virtual int RenderShadow(class Shader* _pShader, float _fTime);
	virtual Component* Clone()	const;

public:
	// ������Ʈ �����
	void EraseComponentFromTag(const std::string& _strTag);
	void EraseComponentFromType(COMPONENT_TYPE _eType);
	void EraseComponent(class Component* _pComponent);

public:
	template <typename T>
	T* FindComponentFromTag(const std::string& _strTag)
	{
		return m_pGameObject->FindComponentFromTag<T>(_strTag);
	}

	template <typename T>
	T* FindComponentFromType(COMPONENT_TYPE _eType)
	{
		return m_pGameObject->FindComponentFromType<T>(_eType);
	}

	template <typename T>
	T* AddComponent(const std::string& _strTag)
	{
		return m_pGameObject->AddComponent<T>(_strTag);
	}
};

ENGINE_END