#pragma once
#include "Engine.h"

ENGINE_BEGIN

class ENGINE_DLL Core
{

#pragma region �����뵵
//private:
//	VertexColor TestVColor[3] = {
//		VertexColor(Vector3(0.0f, 0.5f, 0.0f), Vector4::Green),
//		VertexColor(Vector3(0.5f, -0.5f, 0.0f), Vector4::Red),
//		VertexColor(Vector3(-0.5f, -0.5f, 0.0f), Vector4::Blue),
//	};
#pragma endregion

private:
	HINSTANCE	m_hInst;
	HWND		m_hWnd;
	Resolution	m_tResolution;				// �ػ�( _tag, _t�� ����ü )
	static bool m_bLoop;
	float		m_fClearColor[4];
	bool		m_bEditMode;				// Edit ��� ���� -> ��ŷ�뵵üũ

public:
	// ������ Instance, Handle Get�Լ�
	HINSTANCE GetWindowInstance()	const;
	HWND GetWindowHandle()	const;
	bool GetEditMode() const;

public:
	void SetClearColor(unsigned char _r, unsigned char _g,
		unsigned char _b, unsigned char _a);
	void SetEditMode(bool _Value);

public:
	// �ʱ�ȭ
	// _bWindowMode = true�� â���, false�� ��üȭ��
	bool Init(HINSTANCE _hInst, const TCHAR* _pTitle, const TCHAR* _pClassName,
		int _iWidth, int _iHeight, int _iIconID, int _iSmallIcon, 
		bool _bEditMode = false, bool _bWindowMode = true, int _iSamplerCount = 1);

	// �����ε�, �̹� �ʱ�ȭ��(������) �����찡 ���� ��� �߰��� ��
	bool Init(HINSTANCE _hInst, HWND _hWnd, int _iWidth, int _iHeight,
		bool _bEditMode = false, bool _bWindowMode = true, int _iSamplerCount = 1);


	int Run();
	void GameLogic();

private:
	int Input(float _fTime);
	int Update(float _fTime);
	int LateUpdate(float _fTime);
	int Collision(float _fTime);
	int Render(float _fTime);

private:
	void Register(const TCHAR* _pClassName, int _iIconID, int _iSmallIconID);				// ���
	void CreateWnd(const TCHAR* _pCTitle, const TCHAR* _pClassName);						// ������ �����
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_SINGLETON(Core)			// �̱��� �����ϱ�
};

ENGINE_END