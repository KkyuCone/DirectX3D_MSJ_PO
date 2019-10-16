#pragma once
#include "UI.h"

ENGINE_BEGIN

// �ٴ� �ܼ��� �� �ؽ�ó�� �� �ؽ�ó�� ������ �ȴ�.
// ���ؽ�ó�� �����Ѵ�. (ũ��)

class ENGINE_DLL UIBar : public UI
{
private:
	friend class GameObject;

protected:
	UIBar();
	UIBar(const UIBar& _Com);
	virtual ~UIBar();

protected:
	float m_fRatio;					// ����
	Vector4 m_vColor;				// ����
	BarCBuffer m_tBarCBuffer;

	class Texture*		m_pTexture;
	class Material*		m_pMaterial;
	class Renderer*		m_pRenderer;

public:
	void SetColor(const Vector4& _vColor);
	void SetRatio(const float& _fRatio);
	void SetTexture(const std::string& _strName, const TCHAR* _pFileName, const std::string& _strPathName = PATH_TEXTURE);
	void SetShader(const std::string& _strKey);
	void SetRenderState(const std::string& _strKey);
	void SetHeightEnable(bool _Enable);

public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��, �� ���̾�� ȣ�����ش�.
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);	// ������ �ϱ� ������ ȣ�����ִ� �Լ��Ĥ���
	virtual int Render(float _fTime);
	virtual UIBar* Clone()	const;

private:
	void DefaultBarInit();

};

ENGINE_END

