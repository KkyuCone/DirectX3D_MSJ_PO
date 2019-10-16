#pragma once
#include "UI.h"

ENGINE_BEGIN

class UIImage : public UI
{
private:
	friend class GameObject;

protected:
	UIImage();
	UIImage(const UIImage& _Com);
	virtual ~UIImage();

protected:
	class Texture*		m_pTexture;			// �ش� �̹��� �ؽ�ó  ( �̹����ϱ� �ؽ�ó 1�� )
	Vector4				m_vColor;			// ����
	ImageCBuffer		m_tImageCBuffer;	// ���� ��� ����
	class Material*		m_pMaterial;
	class Renderer*		m_pRenderer;

	//std::function<void(float)> m_CallBackFunc;		// �Լ�

public:
	// �̹��� ���� ����
	void SetColor(const Vector4& _vColor);

	// �̹��� �ؽ�ó ����
	void SetTexture(const std::string& _strName, const TCHAR* _pFileName, const std::string& _strPathName = PATH_TEXTURE);

	// ���̴� ����, ���� ������Ʈ ����
	void SetShader(const std::string& _strKey = SHADER_UI_IMAGE);
	void SetBlendState(const std::string& _strKey = "AlphaBlend");

public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��, �� ���̾�� ȣ�����ش�.
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);	// ������ �ϱ� ������ ȣ�����ִ� �Լ��Ĥ�
	virtual int Render(float _fTime);
	virtual UIImage* Clone()	const;

private:
	void DefaultInit();			// ����Ʈ��
//public:
//	void SetCallBackFunc(void(*_pFunc)(float));
//
//	template <typename FuncType>
//	void SetCallBackFunc(FuncType* _pObejct, void(FuncType::*_pFunc)(float))
//	{
//		m_CallBackFunc = std::binary_function(_pFunc, _pObejct, std::placeholders::_1);
//	}
};

ENGINE_END