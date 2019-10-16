#pragma once
#include "Engine.h"

ENGINE_BEGIN

class ENGINE_DLL Device
{
private:
	HWND					m_hWnd;
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pContext;
	IDXGISwapChain*			m_pSwapChain;				// ������ �� �����͸� ����ϱ� ���� �����ϱ����� �ϳ� �̻��� �����̽� �� �����մϴ�.
	ID3D11RenderTargetView*	m_pRenderTargetView;
	ID3D11DepthStencilView*	m_pDepthStencilView;
	Resolution				m_Resolution;

private:
	// 2D Interface, 2D���̺귯��(��¿�)
	ID2D1RenderTarget*	m_p2DRenderTarget;
	ID2D1Factory*		m_p2DFactory;

public:
	// Get�Լ�
	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetContext() const;
	IDXGISwapChain* GetSwapChain() const;
	ID2D1RenderTarget* Get2DRenderTarget() const;
	Resolution GetResolution() const;
	Vector2 GetWindowToViewportRatio()	const;			// ���콺 ��ǥ�� �˾Ƴ��� ���ؼ� ����ϴ� ������ ũ��� ����Ʈ�� ���� ����

public:
	bool Init(HWND _hWnd, unsigned int _iWidth, unsigned int _iHeight, bool _bWindowMode, int _iSamplerCount/* = 1*/);
	void Clear(float _fCrearColor[4]);
	void Present();

	// �̱����� �Ǹ����
	DECLARE_SINGLETON(Device)
};

ENGINE_END