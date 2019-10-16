#pragma once
#include "../Engine.h"

ENGINE_BEGIN

class ENGINE_DLL Scene
{
private:
	friend class SceneManager;

private:
	Scene();
	~Scene();

private:
	std::list<class SceneComponent*>	m_SceneComponentList;			// ��
	std::list<class Layer*>				m_LayerList;					// ���̾�

private:
	std::unordered_map<std::string, class GameObject*>	m_CameraMap;		// ������ ī�޶� �������ش�.

	// ����ī�޶�
	class GameObject*	m_pMainCameraObject;
	class Camera*		m_pMainCamera;
	class Transform*	m_pMainCameraeTransform;

	// UI ī�޶�
	class GameObject*	m_pUICameraObject;
	class Camera*		m_pUICamera;
	class Transform*	m_pUICameraTransform;

	// ��ī��
	// ��ī�̹ڽ��� Scene���� �ϳ��� ���� �־���Ѵ�.(�ʼ�)
	// ��ī�̰� �� �ʿ��ϴٸ� �ؽ�ó�� ��ü�ϴ� ������� ����Ѵ�.
	class GameObject*	m_pSkyObject;
	class Material*		m_pSkyMaterial;

public:
	// ����ī�޶�
	class GameObject*	GetMainCameraObject() const;
	class Camera*	GetMainCamera() const;
	class Transform* GetMainCameraTransform() const;

	Matrix GetViewMatrix() const;
	Matrix GetProjMatrix() const;
	Vector3 GetMainCameraPos() const;

	// UIī�޶�
	class GameObject*	GetUICameraObject() const;
	class Camera*	GetUICamera() const;
	class Transform* GetUICameraTransform() const;

	Matrix GetUIViewMatrix() const;
	Matrix GetUIProjMatrix() const;
	Vector3 GetUICameraPos() const;

public:
	GameObject* m_pGLDirObject;
	Transform* m_pGLDirTransform;


public:
	bool Init();
	int Input(float _fTime);
	int Update(float _fTime);
	int LateUpdate(float _fTime);
	int Collision(float _fTime);
	int Render(float _fTime);

public:
	void AddLayer(const std::string& _strName, int _Order);
	void SortLayer();
	class Layer* FindLayer(const std::string& _strName);
	//class GameObject* FindGameObejctToLayer(const std::string& _strName, const std::string& _Tag);
	//class GameObject* FindGameObjectFirst(const std::string& _Tag);

public:
	bool AddCamera(const std::string& _strName, const Vector3& _vPos, const Vector3& _vRot,
		CAMERA_TYPE _eType, UINT _iWidth, UINT _iHeight,
		float _fAngle, float _fNear, float _fFar);
	bool ChangeMainCamera(const std::string& _strName);
	class GameObject* FindCamera(const std::string& _strName);

public:
	template <typename T>
	bool AddSceneComponent()
	{
		T* pComponent = new T;

		pComponent->m_pScene = this;

		if (false == pComponent->Init())
		{
			SAFE_DELETE(pComponent);
			return false;
		}

		m_SceneComponentList.push_back(pComponent);
		return true;
	}

private:
	// _pSrcc : ����������, _pDest : ������ ���Ұ� ������ (���ؼ� ���� ������)
	static bool Sort(class Layer* _pSrcc, class Layer* _pDest);
};

ENGINE_END


