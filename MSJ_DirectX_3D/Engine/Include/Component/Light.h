#pragma once
#include "Component.h"

ENGINE_BEGIN

class ENGINE_DLL Light : public Component
{
private:
	friend class GameObject;

protected:
	Light();
	Light(const Light& _Component);
	~Light();

private:
	LightInfo	m_tInfo;
	float		m_fInAngle;			// ���� �ݰ� ���� (��������Ʈ)
	float		m_fOutAngle;		// �ٱ��� �ݰ� ���� (��������Ʈ)

	Matrix		m_matLightVeiw;
	Matrix		m_matLightProj;

public:
	LightInfo GetLightInfo()		const;

	float GetInAngle()	const;
	float GetOutAngle()	const;
	Vector4 GetDiffuseColor() const;
	Vector4 GetAmbientColor() const;

	Matrix GetLightViewMatrix() const;
	Matrix GetLightProjMatrix() const;

public:
	void SetLightType(LIGHT_TYPE _eType);
	void SetLightColor(const Vector4& _vDiffuse, const Vector4& _vAmbient, const Vector4& _vSpecular);
	void SetLightDistance(float _fDistance);
	void SetLightAngle(float _fInAngle, float _fOutAngle);

	void SetLightVP(Matrix _VP);
	void SetBias(float _fBias);

public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��, �� ���̾�� ȣ�����ش�.
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);	// ������ �ϱ� ������ ȣ�����ִ� �Լ��Ĥ���
	virtual int Render(float _fTime);
	virtual Light* Clone()	const;

public:
	void SetShader();

};

ENGINE_END
