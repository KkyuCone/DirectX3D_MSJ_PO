#pragma once
#include "Component.h"

ENGINE_BEGIN

#pragma region 2DAnimation�� ���� ����ü��

// �ִϸ��̼��� ���� �ؽ�ó UV ��ǥ
typedef struct ENGINE_DLL _tagTextureCoord
{
	Vector2 vStart;
	Vector2 vEnd;
}TextureCoord, *PTextureCoord;


// �ִϸ��̼� ������ Ŭ��
typedef struct ENGINE_DLL _tagAnimationFrameClip
{
	std::string					strName;
	ANIMATION_FRAME_TYPE		eType;
	ANIMATION_OPTION			eOption;
	int							iFrame;
	Vector2						vTextureSize;
	std::vector<TextureCoord>	vecCoord;
	float						fPlayTime;
	float						fPlayLimitTime;
	float						fFrameTime;
	class Texture*				pTexture;
	int							iRegister;

}AnimationFrameClip, *PAnimationFramClip;

#pragma endregion


// Ŭ����
class ENGINE_DLL AnimationFrame : public Component
{
private:
	friend class GameObject;

protected:
	AnimationFrame();
	AnimationFrame(const AnimationFrame& _Com);
	~AnimationFrame();

private:
	std::unordered_map<std::string, PAnimationFramClip>	m_mapClip;			// �ִϸ��̼ǵ� ����
	PAnimationFramClip		m_pCurClip;						// ���� �ִϸ��̼� Ŭ��
	PAnimationFramClip		m_pDefaultClip;					// �⺻ Ŭ��
	AnimationFrameCBuffer	m_tCBuffer;						// �ִϸ��̼� �������
	class Material*			m_pMaterial;

public:
	// ��Ʋ�� �ؽ�ó �����
	bool CreateClip(const std::string& _strName,				// �ִϸ��̼� Ŭ�� �̸�
		ANIMATION_OPTION _eOption,								// �ִϸ��̼� �ɼ�
		const Vector2& _vTextureSize,							// �ؽ�ó ������
		const std::vector<TextureCoord>& _vecCoord,				// �ؽ�ó UV ��ǥ
		float _fLimitTime,										// ���ѽð�
		int _iTextureRegister,									// �ش� �ؽ�ó ��������(���̴��� �Ѱ���..)
		const std::string& _strTextureName,						// ����� ��Ʋ�� �ؽ�ó�̸�
		const TCHAR*	_pFileName,								// ���� �̸�
		const std::string& _strPathName = PATH_TEXTURE);		// �ش� ���� ���

	// ������ �ؽ�ó ( �������� �ؽ�ó�� �� �ֵ� ) �����
	bool CreateClip(const std::string& _strName,				// �ִϸ��̼� Ŭ�� �̸�
		ANIMATION_OPTION _eOption,								// �ִϸ��̼� �ɼ�
		const Vector2& _vTextureSize,							// �ؽ�ó ������
		const std::vector<TextureCoord>& _vecCoord,				// �ؽ�ó UV ��ǥ
		float _fLimitTime,										// ���ѽð�
		int _iTextureRegister,									// �ش� �ؽ�ó ��������(���̴��� �Ѱ���..)
		const std::string& _strTextureName,						// ����� ������ �ؽ�ó�̸� -> �ؽ�ó�� �ε�� �ֵ�
		const std::vector<TCHAR*>&	_vecFileName,				// ���� �̸���.. - ����
		const std::string& _strPathName = PATH_TEXTURE);		// �ش� ���� ���

public:
	bool SetCurrentClip(const std::string& _strName);
	bool SetDefaultClip(const std::string& _strName);
	bool ChangeClip(const std::string& _strName);

public:
	PAnimationFramClip GetCurClip();

private:
	PAnimationFramClip FindClip(const std::string& _strName);

public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��, �� ���̾�� ȣ�����ش�.
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);	// ������ �ϱ� ������ ȣ�����ִ� �Լ��Ĥ���
	virtual int Render(float _fTime);
	virtual Component* Clone()	const;
};

ENGINE_END