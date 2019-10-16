#pragma once
#include "..\Engine.h"

ENGINE_BEGIN

class ENGINE_DLL ShaderManager
{
private:
	std::unordered_map<std::string, class Shader*>		m_mapShader;
	std::unordered_map<std::string, ID3D11InputLayout*>	m_mapInputLayout;
	std::unordered_map<std::string, PConstBuffer>	m_mapConstBuffer;
	std::vector<D3D11_INPUT_ELEMENT_DESC>	m_vecInputDesc;
	UINT									m_iInputSize;

public:
	bool Init();
	bool LoadShader(const std::string& _strName, const TCHAR* _pFileName,
		std::string _pEntry[ST_END], const std::string& _strPathKey = PATH_SHADER);

	class Shader* FindShader(const std::string& _strName);

	// Desc�߰� (InputLayout)
	void AddInputDesc(const char* _pSemanticName, UINT _iSementicIdx,
		DXGI_FORMAT _eFormat, UINT _iSize, UINT _iInputSlot,
		D3D11_INPUT_CLASSIFICATION _eClass, UINT _iStepRate);

	bool CreateInputLayout(const std::string& _strName, const std::string& _strShaderKey);

	ID3D11InputLayout* FindInputLayout(const std::string& _strName);

	// ������� ����� �� ã��
	bool CreateConstBuffer(const std::string& _strName, int _iSize, int _iRegister, int _iConstantShader);

	// ������� ������Ʈ �Լ�
	// �ι�° ���� ������ void*�� �޴� ������ ��������� ũ�⸦ Ȯ������ �� ���⶧���̴�.
	// ���� �����ش�� ���� �ٲ�ϱ� ������ 
	bool UpdateConstBuffer(const std::string& _strName, void* _pData);
	PConstBuffer FindConstBuffer(const std::string& _strName);

	DECLARE_SINGLETON(ShaderManager)
};


ENGINE_END
