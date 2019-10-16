#pragma once
#include "Reference.h"

ENGINE_BEGIN

class ENGINE_DLL GameObject : public Reference
{
private:
	GameObject();
	GameObject(const GameObject& _Obj);
	~GameObject();

private:
	// ������Ÿ�� ��������� ���� �츮 �����ӿ�ũ����
	// ���� ������Ÿ���� ���� �װ� �����ؼ� (Clon����) �����ؼ� ����Ѵ�.  ( ������ü�� �����ؼ� ���, �޸𸮿� �̸� �־�ΰ� �װ� ���� ������ ) 
	// ������Ÿ���� �����ϱ� ���� map�� ����ߴµ� �̰� Ž���� �����ϱ⋚���� ����Ѱ��� ������
	// ���簴ü�� �����ϴ��Լ� -> CreateClone()
	static std::unordered_map<class Scene*, std::unordered_map<std::string, GameObject*>>	m_PrototypeMap;		// ������Ÿ��

public:
	bool m_bUpdateInstancing;					// �ν��Ͻ� �״�� ������ ���� ���� ����

public:
	void SetUpdateInstancing(bool _Enable);
	bool GetUpdateInstancing();

public:
	static GameObject* CreateObject(const std::string& _NameTag = "", class Layer* _pLayer = nullptr);
	static GameObject* CreatePrototype(const std::string& _NameTag = "", class Scene* _pScene = nullptr);
	static GameObject* CreateClone(const std::string& _NameTag, class Scene* _pScene, class Layer* _pLayer);

	// ���� ���������� �ƴϸ� �ش������ �ش� ������Ÿ�Ը� ���������� .. ��츦 2���� ������ �����.
	static void RemoveProtoType(class Scene* _pScene);
	static void RemoveProtoType(class Scene* _pScene, const std::string& _strProtoType);

private:
	static GameObject* FindPrototype(const std::string& _NameTag, class Scene* _pScene = nullptr);

private:
	class Scene* m_pScene;						// �ش� ������Ʈ�� ����� �ִ��� �˰�����
	class Layer* m_pLayer;						// �ش� ������Ʈ�� ����� ���̾ �ִ��� �˰�����
	class Transform* m_pTransform;				// ���ӿ�����Ʈ�� Ʈ�������� �ʼ���.
	RENDER_GROUP	m_eRenderGroup;				//  �����׷� ���� ������ ���ٷ��� �߰���.( ��ƼŬ, UI ���� ���� ������ ���� �ʿ䰡 ��� �����׷��� ���� ������ �Ŀ� �����Ŵ������� �����ٰ���)
	float			m_fLifeTime;
	bool			m_bInstancing;				// �ν��Ͻ� ����
	bool			m_bFrustum;					// �������� �ø� ������ ����

public:
	class Scene* GetScene()	const;
	class Layer* GetLayer()	const;
	class Transform* GetTransform() const;
	RENDER_GROUP	GetRenderGroup() const;
	float			GetLifeTime()	const;
	bool			 IsIntancing() const;
	bool			IsFrustumCulling() const;

	void SetScene(class Scene* _pScene);
	void SetLayer(class Layer* _pLayer);
	void SetRenderGroup(RENDER_GROUP _RenderGroup);
	void SetLifeTime(float _fTime);
	void SetInstancingEnable(bool _bEnable);
	void SetFrustumCulling(bool _bCulling);

private:
	std::list<class Component*>	m_ComponentList;		// �ش� ������Ʈ�� �ڽ��� �����ִ� ��� ������Ʈ���� �˰��ִ�.
	std::list<class Collider*>	m_ColliderList;			// �ݶ��̴� ����Ʈ��
	std::list<class Component*>	m_StartList;			// Start()�� ȣ�����ʿ��� ������Ʈ���� ����, ����� Add�� ��� ������Ʈ���� ������ �ϰ� ������ ���Ŀ� ���� �ʿ��Ѿֵ鸸 ���� �и��ص���

public:
	const std::list<class Collider*>* GetColliderList() const;		// Get Collider List

private:
	// �θ�, �ڽ� ���� �߰�
	bool m_bParent;
	GameObject* m_pParent;
	std::list<GameObject*> m_pChildList;					// �ڽ��� �������� �� ���ְ�

public:
	int GetChildCount();
	bool ParentEnable() const;

public:
	void SetParent(GameObject* _Object);

	void DeleteParent();

	void AddChild(GameObject* _Object);

	void DeleteChild(GameObject* _Object);

	GameObject* GetParent();
	std::list<GameObject*> GetChildren();
	GameObject* FindChild(std::string _strName);

private:
	// ���ⰰ�� ������ ��� (�ִϸ��̼� ���� X ������ �θ������Ʈ�� ������� �ʿ��� ���
	bool m_bSoket;
	std::string m_SoketBoneName;
	int m_SoketBoneNum;

public:
	void SetBoneSoket(std::string _strBoneName);
	std::string GetParentBoneSoketName();
	int GetParentBoneSoketIndex();
	void DeleteBoneSoket();

	bool GetSoketEnable();
	int GetSoketBoneNum();

public:
	void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��
	bool Init();
	int Input(float _fTime);
	int Update(float _fTime);
	int LateUpdate(float _fTime);
	int Collision(float _fTime);
	int PrevRender(float _fTime);
	int PrevShadowRender(Matrix _matLightView, Matrix _matLightProj, float _fTime);
	int Render(float _fTime);
	int RenderShadow(class Shader* _pShader, Matrix _matLightView, Matrix _matLightProj, float _fTime);
	GameObject* Clone()	const;

public:
	// �ش� ������Ʈ�� �����ִ����� ����
	bool CheckComponentFromTag(const std::string& _strTag);
	bool CheckComponentFromType(COMPONENT_TYPE _eType);

public:
	// ������Ʈ ����
	void EraseComponentFromTag(const std::string& _strTag);
	void EraseComponentFromType(COMPONENT_TYPE _eType);
	void EraseComponent(class Component* _pComponent);

public:
	// ������Ʈ �߰�
	Component* AddComponent(class Component* _pComponent);

	template <typename T>
	T* AddComponent(const std::string& _strTag = "")
	{
		T* pComponent = new T;

		pComponent->SetTag(_strTag.c_str());
		pComponent->SetScene(m_pScene);
		pComponent->SetLayer(m_pLayer);
		pComponent->SetGameObject(this);
		pComponent->m_pTransform = m_pTransform;

		if (false == pComponent->Init())
		{
			SAFE_RELEASE(pComponent);
			return nullptr;
		}

		return (T*)AddComponent(pComponent);
	}

public:
	// ������Ʈ - �±׷� ã��
	template <typename T>
	T* FindComponentFromTag(const std::string& _strTag)
	{
		std::list<Component*>::iterator StartIter = m_ComponentList.begin();
		std::list<Component*>::iterator EndIter = m_ComponentList.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			if ((*StartIter)->GetTag() == _strTag)
			{
				(*StartIter)->AddReference();
				return (T*)*StartIter;
			}
		}
		return nullptr;
	}

	// ������Ʈ - ������ƮŸ������ ã��
	template <typename T>
	T* FindComponentFromType(COMPONENT_TYPE _eType)
	{
		std::list<Component*>::iterator StartIter = m_ComponentList.begin();
		std::list<Component*>::iterator EndIter = m_ComponentList.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			if ((*StartIter)->GetComponentType() == _eType)
			{
				(*StartIter)->AddReference();
				return (T*)*StartIter;
			}
		}

		return nullptr;
	}
};

ENGINE_END

