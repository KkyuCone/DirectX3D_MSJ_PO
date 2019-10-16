#pragma once
#include "Component.h"

ENGINE_BEGIN

class ENGINE_DLL Renderer : public Component
{
private:
	friend class GameObject;

private:
	Renderer();
	Renderer(const Renderer& _Component);
	~Renderer();

private:
	class Shader*	m_pShader;
	class Mesh*		m_pMesh;
	ID3D11InputLayout*	m_pInputLayout;
	class Material*	m_pMaterial;
	class RenderState*	m_pRenderState[RS_END];

public:
	class Mesh* GetMesh() const;

public:
	void SetMesh(const std::string& _strKey);
	void SetMesh(const std::string& _strKey, const TCHAR* _pFileName,
		const Vector3& _vView = Vector3::Axis[AXIS_Z],
		const std::string& _strPathKey = PATH_MESH);
	void SetMeshFromFullPath(const std::string& _strKey, const TCHAR* _pFileName,
		const Vector3& _vView = Vector3::Axis[AXIS_Z]);
	void SetShader(const std::string& _strKey);
	void SetInputLayout(const std::string& _strKey);
	void SetRenderState(const std::string& _strName);

	void SetMaterialColor(const Vector4 & _vDiffuse, const Vector4 & _vAmbient
		, const Vector4 & _vSpecualr, const Vector4 & _vEmissive
		, float _fSpecularPower, UINT _iContainer = 0, UINT _iSubSet = 0);

public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��, �� ���忡 ��ġ�ɶ� ȣ���ϴ� �Լ�
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime); // Render() �Լ� ȣ�� ������ ȣ�����ִ� �Լ�
	virtual int PrevShadowRender(Matrix _matLightView, Matrix _matLightProj, float _fTime);
	virtual int Render(float _fTime);
	virtual int RenderShadow(class Shader* _pShader, float _fTime);
	virtual Renderer* Clone()	const;

public:
	SphereInfo GetPickSphereInfo();


public:
	void RenderInstancing(PInstancingBuffer _pBuffer,
		class Shader* _pShader, ID3D11InputLayout* _pLayout,
		int _iInstancingCount, float _fTime);

	void RenderInstancingShadow(PInstancingBuffer _pBuffer,
		class Shader* _pShader, ID3D11InputLayout* _pLayout,
		int _iInstancingCount, float _fTime);
};

ENGINE_END

