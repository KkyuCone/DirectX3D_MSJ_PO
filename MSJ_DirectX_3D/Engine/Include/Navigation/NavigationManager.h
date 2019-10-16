#pragma once
#include "..\Engine.h"

ENGINE_BEGIN

// �׺���̼� �Ŵ�����
// �׺�޽����� �������ִ� ������ ������ �Ѵ�.
// �� ������ �������� �׺�޽��� ������ �� �ִٰ� �����ؼ� �Ʒ��� ���� �����������
// ���� ���� �Ѱ��� �׺�޽��� ���´�.

// �׸��� �̱������� ���������. 
// �ھ��ʿ��� ������ ���Ÿ����ְ�, 
// ������ �����Ҷ� CreateNavMesh�Լ��� ȣ���ؼ� �׺�޽��� �����ϰ� ���� �����ڿ��� �����ְ��Ѵ�.
class ENGINE_DLL NavigationManager
{
private:
	std::unordered_map<class Scene*, std::unordered_map<std::string, class NavigationMesh*>>	m_mapNavMesh;

public:
	bool Init();

public:
	class NavigationMesh* CreateNavMesh(class Scene* _pScene, const std::string& _strName);
	class NavigationMesh* CreateNavMesh(class Scene* _pScene, const std::string& _strName, 
		const char* _pFileName, const std::string& _strPathName = PATH_DATA);
	
	class NavigationMesh* FindNavMesh(class Scene* _pScene, const std::string& _strName);
	class NavigationMesh* FindNavMesh(class Scene* _pScene, const Vector3& _vPos);

	bool EraseNavMesh(class Scene* _pScene, const std::string& _strName);
	bool EraseNavMesh(class Scene* _pScene, const Vector3& _vPos);
	bool EraseNavMesh(class Scene* _pScene);

public:
	int MouseNavSectionCount(class Scene* _pScene, RayInfo _RayInfo);
	Vector3 MouseNavPosition(class Scene* _pSCene, RayInfo _RayInfo);

	DECLARE_SINGLETON(NavigationManager)
};

ENGINE_END
