#pragma once
#include "Component.h"

ENGINE_BEGIN
#pragma region TextureSet
// �ȼ� ������ ������ �� ���÷��� ����ϴµ� �̶� ������ �������� ������ ���� �־��ش�.
// ������ �������� �������� �������� ���漺���� ��������..
// �������͸����� �������(�ٸ����)�� ��ȭ�� �� �ִ�.  ( ��Ƽ�ٸ����)
// ���漺���δ� �ε巴�� �� �� ������ ���귮�� ������(��Ƽ�ٸ����). ���� �׳� �״�� �������°�
typedef struct ENGINE_DLL _tagTextureSet
{
	class Texture*	pTexture;
	int				iTextureRegister;
	//class Sampler*	pSampler;				// ���ҽ��Ŵ������� �־���⶧���� �Ȱ����ְ� ���̴����� ��� ����
	//int				iSamplerRegister;

	_tagTextureSet()
		: pTexture(nullptr)
		, iTextureRegister(0)
	{

	}
}TextureSet, *PTextureSet;

#pragma endregion

#pragma region MaterialSet

typedef struct ENGINE_DLL _tagMaterialSubSet
{
	MaterialCBuffer	tMaterialData;
	std::unordered_map<std::string, PTextureSet>	mapTextureSet;			// ��� ����Ұ���
	//PTextureSet	pDiffuse;
	//PTextureSet pNormal;
	//PTextureSet pSpecular;
}MaterialSubSet, *PMaterialSubSet;

#pragma endregion

#pragma region Material Container

typedef struct ENGINE_DLL _tagMaterialContainer
{
	std::vector<PMaterialSubSet> vecSubSet;
}MaterialContainer, *PMaterialContainer;

#pragma endregion

// ����(Material) : ���� ��ü�� ǥ��� ��ȣ�ۿ��ϴ� ����� �����ϴ� �Ӽ����� ����
class ENGINE_DLL Material : public Component
{
private:
	friend class GameObject;
	friend class Mesh;

public:
	Material();
	Material(const Material& _Component);
	virtual ~Material();

private:
	std::vector<PMaterialContainer>	m_vecMaterialContainer;

public:
	void SetColor(const Vector4& _vDiffuse, const Vector4& _vAmbient
		, const Vector4& _vSpecualr, const Vector4& _vEmissive, float _fSpecularPower, UINT _iContainer = 0, UINT _iSubSet = 0);

	// EmissiveColor ����
	void SetEmissiveColor(float _fEmissive);		// ����(����)
	void SetEmissiveColor(int _iContainer, int _iSubSet, float _fEmissive);

	// �ؽ�ó ���� -  Texture, ArrayTexture
	void AddTextureSet(int _iContainer, int _iSubSet,
		int _iTextureRegister, const std::string& _strTextureName, 
		const TCHAR* _pFileName = nullptr,
		const std::string& _strPathName = PATH_TEXTURE);

	void AddTextureSet(int _iContainer, int _iSubSet,
		int _iTextureRegister,class Texture* _pTexture);			//�ִϸ��̼� ����

	void AddTextureSetFromFullPath(int _iContainer, int _iSubSet,
		int _iTextureRegister, const std::string& _strTextureName,
		const TCHAR* _pFullPath = nullptr);

	void AddTextureSetArray(int _iContainer, int _iSubSet,
		int _iTextureRegister, const std::string& _strTextureName,
		const std::vector<TCHAR*>* _vecFileName = nullptr,
		const std::string& _strPathName = PATH_TEXTURE);

	void AddTextureSetArrayFromFullPath(int _iContainer, int _iSubSet,
		int _iTextureRegister, const std::string& _strTextureName,
		const std::vector<TCHAR*>* _vecFullPath = nullptr);

	// ����
	void DeleteTextureSet(int _iContainer, int _iSubSet, const std::string& _strName);
	void DeleteTextureSet(int _iContainer, int _iSubSet);			// �ִϸ��̼ǰ���  -> ��ü ������

	// �ؽ�ó �ٲٱ� -  Texture, ArrayTexture
	void ChangeTextureSet(int _iContainer, int _iSubSet,
		int _iTextureRegister, const std::string& _strOriTextuerName, 
		const std::string& _strChangeTextureName,
		const TCHAR* _pFileName = nullptr,
		const std::string& _strPathName = PATH_TEXTURE);

	void ChangeTextureSet(int _iContainer, int _iSubSet,
		int _iTextureRegister, const std::string& _strOriTextuerName,
		class Texture* _pTexture);						// �ִϸ��̼� ����

	void ChangeTextureSetFromFullPath(int _iContainer, int _iSubSet,
		int _iTextureRegister, const std::string& _strOriTextuerName,
		const std::string& _strChangeTextureName,
		const TCHAR* _pFullPath = nullptr);

	void ChangeTextureSetArray(int _iContainer, int _iSubSet,
		int _iTextureRegister, const std::string& _strOriTextuerName,
		const std::string& _strChangeTextureName,
		const std::vector<TCHAR*>* _vecFullPath = nullptr,
		const std::string& _strPathname = PATH_TEXTURE);

	void ChangeTextureSetArrayFromFullPath(int _iContainer, int _iSubSet,
		int _iTextureRegister, const std::string& _strOriTextuerName,
		const std::string& _strChangeTextureName,
		const std::vector<TCHAR*>* _vecFullPath = nullptr);


	// Bump(Normal) �ؽ�ó, Sepcular�ؽ�ó 
	void BumpTextureEnable(int _iContainer, int _iSubSet);
	void SepcularTextureEnable(int _iContainer, int _iSubSet);

private:
	PTextureSet FindTextureSet(int _iConatienr, int _iSubSet,
		const std::string& _strName);

public:
	// ��Ű��
	void Skinning(int _iContainer, int _iSubSet);

public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��, �� ���̾�� ȣ�����ش�.
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);	// ������ �ϱ� ������ ȣ�����ִ� �Լ��Ĥ���
	virtual int Render(float _fTime);
	virtual int RenderShadow(float _fTime);
	virtual Material* Clone()	const;

public:
	// ��Ƽ���󿡼� ���̴� �����ϱ�
	void SetShader(int _iContainer, int _iSubSet);
	void ReSetShader(int _iContainer, int _iSubSet);

	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);
	virtual void SaveFromPath(const char* _pFileName, const std::string& _strPathKey = PATH_DATA);
	virtual void LoadFromPath(const char* _pFileName, const std::string& _strPathKey = PATH_DATA);

private:
	void SaveTextureSet(FILE* _pFile, PTextureSet _pTexture);
	void LoadTextureSet(FILE* _pFile, PTextureSet* _ppTexture);
};

ENGINE_END
