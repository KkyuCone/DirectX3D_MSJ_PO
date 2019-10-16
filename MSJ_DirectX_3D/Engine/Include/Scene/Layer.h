#pragma once
#include "../Reference.h"

ENGINE_BEGIN

// ���������� ������Ʈ(�Ǵ� ��ü)�� ��ġ�Ǵ°��� Layer��� ���� �ȴ�. ������
// ���̾�� � ������ ������Ʈ�� �������� �߿��ϴ�.
// ���⼭ ������ m_iOrder�� �з��ϸ� ������������ ������Ʈ�� ������
class ENGINE_DLL Layer : public Reference
{
private:
	friend class Scene;

private:
	Layer();
	~Layer();

private:
	class Scene*	m_pScene;
	int				m_iOrder;			// ���̾� ������ ���ϱ� ���� ���� (������������ �Ұ�)

	// ������ ���̾���� �ڽŵ��� ���� ������Ʈ�� �ִ��� �˰�����.
	// vector�� ������� �ʰ� list�� ����ϴ� ������ ���� ���� ������ ����ϰ� (������ �������)�Ͼ ��� list�� ����� ������
	// ���⼱ ���ӿ�����Ʈ���� ����, ���� ���� ���� �𸣱� ������ list�� ���
	std::list<class GameObject*>	m_GameObjectList;

	std::list<class GameObject*>	m_StartList;

	// + STL��ü�� ������� (�Ű�������..�޴°��) �����ƴ� �����ͷ� �־���Ѵ�.
	// -> ������ ���� �Ϲݺ���ó�� �ְԵǸ� �׶����� �� ���簡 �ƴ� �޸𸮸� �Ҵ��ϱ� �����̴�. (�޸� ����)

public:
	int GetOrder()	const;
	void SetOrder(int _Order);
	void Start();

	void SetScene(Scene* _pScene);
	Scene* GetScene();

public:
	bool Init();
	int Input(float _fTime);
	int Update(float _fTime);
	int LateUpdate(float _fTime);
	int Collision(float _fTime);
	int Render(float _fTime);

	void AddGameObject(class GameObject* _GameObject);
	class GameObject* FindGameObject(std::string _Tag);
	
	void DeleteGameObjectToTag(class GameObject* _Object);					// Delete�� �ƿ� ���̾�� ���縦 ����°���
	void DeleteGameObject(class GameObject* _Object);
	void DeleteGameObject(std::string _Tag);

	void DeleteAllGameObject();

public:
	
};

ENGINE_END

