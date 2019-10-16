#pragma once
#include "Component.h"

ENGINE_BEGIN

// �ؽ�ó ���÷��ÿ� ����ü
// ���÷��ð� ���õ� �ؽ�ó�� ���� ���ĸ��� Splat�̶�� �Ѵ�.
typedef struct ENGINE_DLL _tagSplatName
{
	std::vector<TCHAR*> vecDiffuseName;
	std::vector<TCHAR*> vecNormalName;
	std::vector<TCHAR*> vecSpecularName;
	std::vector<TCHAR*> vecAlphaName;
	std::vector<std::string> vecPathName;

	~_tagSplatName()
	{
		Safe_Delete_Array_VectorList(vecDiffuseName);
		Safe_Delete_Array_VectorList(vecNormalName);
		Safe_Delete_Array_VectorList(vecSpecularName);
		Safe_Delete_Array_VectorList(vecAlphaName);
		vecPathName.clear();
	}
}SplatName, *PSplatName;

class ENGINE_DLL LandScape : public Component
{
private:
	friend class GameObject;

protected:
	LandScape();
	LandScape(const LandScape& _Com);
	~LandScape();

private:
	class NavigationMesh* m_pNavMesh;
	//
	std::vector<Vertex3D>	m_vecVertex;		// ���� ����
	std::vector<int>		m_vecIndex;			// ���� �ε���

	std::vector<Vector3>	m_vecFaceNormal;	// ��������͵� ��Ƴ���

	// Ÿ�ϸ�ó�� ������� ( ���ڸ�, ���̸� )
	// �ش� ������ xz������ �����ϵ��� ����� y���� �����ؼ� ���̸� ���Ѵ�.
	int						m_iNumX;			// X���� 
	int						m_iNumZ;

	LandScaleCBuffer		m_tCBuffer;			// �����ϸ�(Ÿ�ϸ��� ����)�� ���÷��� ī���� �������

	class Material*			m_pMaterial;

	SplatName				m_tSplatName;		// ���÷��� ������ �ִ� ����ü(�����Ұ͵� -> ������ۿ�)
	class Texture*			m_pSplatDiffTex;
	class Texture*			m_pSplatNormTex;
	class Texture*			m_pSplatSpecTex;
	class Texture*			m_pSplatAlphaTex;

public:
	bool CreateLandScape(const std::string& _strName, 
		const char* _pFileName, const std::string& _strPathName = PATH_TEXTURE);

	void SetDetailLevel(float _fLevelX, float _fLevelZ);

	void ChangeBaseTexture(const TCHAR* _pDiffuseName,
		const TCHAR* _pNormalName,
		const TCHAR* _pSpeuclarName,
		const std::string& _strPathName = PATH_TEXTURE);

	void AddSplatTexture(const TCHAR* _pDiffuseName,
		const TCHAR* _pNormalName,
		const TCHAR* _pSpecualrName,
		const TCHAR* _pAlphaName,
		const std::string& _strPathName = PATH_TEXTURE);

	bool CreateSplatTexture();

public:
	void NavMeshRender(float _fTime);

private:
	void ComputeNormal();	
	void ComputeTangent();

public:
	virtual void Start();		
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);	
	virtual int Render(float _fTime);
	virtual LandScape* Clone()	const;

public:
	void ChangeNavPosEnable(bool _bEnable, Vector3 _vPos);
	void ChangeNavIndexEnable(bool _bEnable, int _Index);

	std::vector<int> GetVecNavEnable();

};

ENGINE_END


#pragma region �ؽ�ó ���÷��� (Texture Splatting)

 //- �ؽ�ó ���÷����̶� ?
	//- �����̽� ���� �ؽ�ó���� ���ĸ��� ����Ͽ� ���� ���� �ϴ� ��� ( ������ ������ �׸��� X )
	//- �̶� �ؽ�ó�� ���� ���ĸ�  -> ���÷�
	//- �ͷ���(����)�� ������ ���� -> ûũ

 //- ���̾� ����
	//- ���ĸ��� �� �ؽ�ó�� �� ����� �����Ͽ� �ϳ��� ���ĸ����� ������ ��.
	//- <�ʿ��Ѱ�>
	//  1. ���ĸ�
	//  2. ������ �� �ؽ�ó��
	//  3. ���̸�
	//- �ȼ����̴����� �����Ѵ�. 
	//-> �������� �ؽ�ó���� �ϳ��� ���´�(���ĸʵ�)->ArrayTexture�ʿ�

// https://m.blog.naver.com/PostView.nhn?blogId=sorkelf&logNo=40151055590&proxyReferer=https%3A%2F%2Fwww.google.com%2F
#pragma endregion
