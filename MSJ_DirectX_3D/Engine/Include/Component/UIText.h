#pragma once
#include "Component.h"

ENGINE_BEGIN

// ���� ����
enum TEXT_ALIGN_H
{
	TAH_LEFT,
	TAH_CENTER,
	TAH_RIGHT
};

// ���� ����
enum TEXT_ALIGN_V
{
	TAV_TOP,
	TAV_MID,
	TAV_BOTTOM
};

enum TEXT_RENDER_TYPE
{
	TRT_UI,
	TRT_2D,
	TRT_3D
};

class ENGINE_DLL UIText :
	public Component
{
private:
	friend class GameObject;

protected:
	UIText();
	UIText(const UIText& _Com);
	virtual ~UIText();

private:
	IDWriteTextFormat* m_pTextFormat;
	IDWriteTextLayout* m_pTextLayout;

	ID2D1SolidColorBrush* m_pColor;
	ID2D1SolidColorBrush* m_pShadowColor; // �׸��ڿ�

										  // �ؽ�Ʈ ������ (YDJ)
	Vector3 m_vOffset;

	// ���� Ÿ��
	ID2D1RenderTarget* m_2DTarget;

	// �׸��� ��뿩��
	bool	m_bShadow;

	// �ؽ�Ʈ
	TCHAR* m_pText;

	// �ؽ�Ʈ �� ����
	int m_iMaxCount;

	// ��Ʈ ��
	TCHAR* m_pFont;

	// ��Ʈ ũ��
	float m_fSize;

	// ���� ���� ����
	TEXT_ALIGN_H m_eAlignH;
	// ���� ���� ����
	TEXT_ALIGN_V m_eAlignV;

	// ���� Ÿ��(2D, 3D, UI) 
	TEXT_RENDER_TYPE m_eRenderType;

	// ���� ���� ����
	bool m_bAlpha;
	// �׸��� ���� ���� ����
	bool m_bAlphaShadow;

	// ����
	float m_fOpacity;

	// �׸��� ����
	float m_fShadowOpacity;

	// ��Ʈ ����
	Vector4 m_vColor;
	// �׸��� ����
	Vector4 m_vShadowColor;

	// �׸��� ������
	Vector3 m_vShadowOffset;

	// ��Ʈ�� �׷��� ����
	D2D1_RECT_F m_tRenderArea;

public:
	void SetRenderType(TEXT_RENDER_TYPE _eType);
	void SetTextCount(int _iMaxCount);
	void SetText(const TCHAR* _pText);
	void AddText(const TCHAR* _pText);

	void SetFont(const std::string& _strName, const TCHAR* _pFontName,
		int _iWeight, int _iStyle, int _iStretch, float _fSize,
		const TCHAR* _pLocalName);

	void SetFont(const std::string& _strName);
	void SetSize(float _fSize);
	void SetAlignH(TEXT_ALIGN_H _eAlign);
	void SetAlignV(TEXT_ALIGN_V _eAlign);
	void Shadow(bool _bShadow);
	void SetOffset(const Vector3& _vOffset); // YDJ
	void SetShadowOffset(const Vector3& _vShadowOffset);
	void SetShadowColor(float _r, float _g, float _b, float _a);
	void SetShadowColor(unsigned char _r, unsigned char _g,
		unsigned char _b, unsigned char _a);
	void SetShadowColor(const Vector4& _vColor);
	void SetShadowColor(unsigned int _iColor);
	void AlphaBlend(bool _bAlpha);
	void ShadowAlphaBlend(bool _bAlpha);
	void SetOpacity(float _fOpacity);
	void SetShadowOpacity(float _fOpacity);
	void SetColor(float _r, float _g, float _b, float _a);
	void SetColor(unsigned char _r, unsigned char _g,
		unsigned char _b, unsigned char _a);
	void SetColor(const Vector4& _vColor);
	void SetColor(unsigned int _iColor);
	void SetRenderArea(float _l, float _t, float _r, float _b);

private:
	void CreateTextLayout();

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);
	virtual int Render(float _fTime);
	virtual UIText* Clone()	const;
};

ENGINE_END
