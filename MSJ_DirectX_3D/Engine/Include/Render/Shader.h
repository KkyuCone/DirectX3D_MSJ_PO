#pragma once
#include "../Reference.h"

ENGINE_BEGIN

class ENGINE_DLL Shader : public Reference
{
private:
	friend class ShaderManager;

private:
	Shader();
	~Shader();

private:
	ID3D11VertexShader*		m_pVS;				// ���ؽ����̴�
	ID3DBlob*				m_pVSBlob;

	ID3D11PixelShader*		m_pPS;				// �ȼ����̴�
	ID3DBlob*				m_pPSBlob;

	ID3D11GeometryShader*	m_pGS;				// ���� ���̴�
	ID3DBlob*				m_pGSBlob;		

	ID3D11ComputeShader*	m_pCS;				// �����̴�
	ID3DBlob*				m_pCSBlob;

public:
	const void* GetVSCode() const;
	size_t GetVSCodeSize() const;

public:
	bool LoadShader(const std::string& _strName, const TCHAR* _pFileName
		, std::string _pEntry[ST_END], const std::string& _strPathKey = PATH_SHADER);

	void SetShader();

private:
	bool LoadVertexShader(const TCHAR* _pFullPath, const char* _pEntry);
	bool LoadPixelShader(const TCHAR* _pFullPath, const char* _pEntry);
	bool LoadGeometryShader(const TCHAR* _pFullPath, const char* _pEntry);
	bool LoadComputeShader(const TCHAR* _pFullPath, const char* _pEntry);
};

ENGINE_END

