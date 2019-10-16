#pragma once
#include "Component.h"

ENGINE_BEGIN

class ENGINE_DLL Particle : public Component
{
private:
	friend class GameObject;

protected:
	Particle();
	Particle(const Particle& _Com);
	~Particle();

private:
	ParticleCBuffer m_tCBuffer;
	class Material*	 m_pMaterial;
	std::string		m_strTextureName;

public:
	bool LoadTextureSet(const std::string& _strTextureName,
		const TCHAR* _pFileName = nullptr,
		const std::string& _strPathName = PATH_TEXTURE);

	bool LoadTextureSetFromFullPath(const std::string& _strTextureName,
		const TCHAR* _pFullPath = nullptr);

	bool LoadTextureSet(const std::string& _strTextureName,
		const std::vector<TCHAR*>& _vecFileName,
		const std::string& _strPathKey = PATH_TEXTURE);

	bool LoadTextureSetFromFullPath(const std::string& _strTextureName,
		const std::vector<TCHAR*>& _vecFullPath);
	
public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��, �� ���̾�� ȣ�����ش�.
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);	// ������ �ϱ� ������ ȣ�����ִ� �Լ��Ĥ���
	virtual int Render(float _fTime);
	virtual Particle* Clone()	const;

public:
	void UpdatePercent(float _fPercent);

};

ENGINE_END
