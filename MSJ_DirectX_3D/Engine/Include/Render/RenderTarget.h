#pragma once
#include "..\Engine.h"

ENGINE_BEGIN

class ENGINE_DLL RenderTarget
{
private:
	friend class RenderManager;

public:
	RenderTarget();
	~RenderTarget();

private:
	std::string					m_strName;				// ����Ÿ�� �̸�
	float						m_fCelarColor[4];		// ������ �÷� (�ؽ�ó�� �׷����� �������)

	ID3D11RenderTargetView*		m_pRenderTargetView;	// ����Ÿ�� (�ٲܳ༮)
	ID3D11Texture2D*			m_pRenderTargetTexture;	// �ؽ�ó
	ID3D11ShaderResourceView*	m_pRenderTargetSRV;		// ����Ÿ�ٿ� ���̴����ҽ��� (���̴��� �ѱ�� ���� �ʿ�)

	ID3D11DepthStencilView*		m_pDepthView;			// ���̺�
	ID3D11Texture2D*			m_pDepthTexture;		// ���� �ؽ�ó

private:
	ID3D11RenderTargetView*		m_pOldRenderTargetView;	// ���� ����Ÿ�ٺ�,  ����Ÿ���� �ٲ��Ŀ� ������ ����Ÿ������ �ٲ�����ϱ⶧��
	ID3D11DepthStencilView*		m_pOldDepthView;

public:
	ID3D11RenderTargetView*	GetRenderTargetView()	const;
	ID3D11DepthStencilView*	GetDepthStencilView()	const;
	ID3D11ShaderResourceView* GetShaderResourceView() const;

private:
	Vector3			m_vPosition;
	Vector3			m_vScale;
	bool			m_bDebug;			// ����� ����
	Matrix*			m_pProj;			// �������� ��� (�������)
	class Mesh*		m_pMesh;			// ����׿� ����Ÿ���� ���� ���� �޽�
	class Shader*	m_pShader;			// ����׿� ����� ���̴� 
	ID3D11InputLayout*	m_pLayout;		// ����׿� ���̾ƿ�

public:
	// ����Ÿ���� ���� �Լ� ( ���۵� �������� �� �ʿ��� )
	// MRT�� �ʿ�  ( Multi Render Target )
	bool CreateRenderTarget(const std::string& _strName,
		UINT _iWidth, UINT _iHeight, DXGI_FORMAT _eForamt,
		float _fClearColor[4], int _iSampleCount = 1,
		DXGI_FORMAT _eDepthFormat = DXGI_FORMAT_UNKNOWN);		// ������ ����Ʈ���� ������� �ʰڴٴ¸��� ������

	void OnDebug(const Vector3& _vPos, const Vector3& _vScale, bool _bDebug = true);
	void ClearRenderTarget();				// �����
	void SetRenderTarget();					// ���� Ÿ�� �����ϱ� (�ٲ��ֱ�)
	void ResetRenderTarget();				// ����Ÿ�� ������� �ǵ�����

	void SetShader(int _iRegister);			// ���̴� �������ֱ�
	void SetCShader(int _iRegister);
	void ResetShader(int _iRegister);		// ���̴��� ����� �Ŀ� �ٽ� �ǵ�����
	void ResetCShader(int _iRegister);
	void Render();

};

ENGINE_END

