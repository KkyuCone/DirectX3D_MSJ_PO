#pragma once
#include "Component.h"

ENGINE_BEGIN

// �����(View Space)�� ����İ� ��������ķ� �̷����ִ�.
// ������������ ��Ⱦ��, �þ߰�, �����Ÿ�, �����Ÿ��� �ʿ��ϴ�.
class ENGINE_DLL __declspec(align(16)) Camera : public Component
{
private:
	friend class GameObject;

private:
	Camera();
	Camera(const Camera& _Camera);
	~Camera();

private:
	Matrix	m_ViewMatrix;				// �����
	Matrix	m_ProjectionMatrix;			// �������
	CAMERA_TYPE m_eCameraType;
	UINT	m_iWidth;
	UINT	m_iHeight;
	float	m_fAngle;					// �þ߰�
	float	m_fNear;
	float	m_fFar;

	bool	m_bUpdate;					// ������Ʈ �ʿ� ����

	class Frustum* m_pFrustum;			// �������� �ø��� ���ؼ� ������

public:
	void SetCameraInfo(CAMERA_TYPE _eCameraType, UINT _iWidth, UINT _iHeight,
		float _fAngle, float _fNear, float _fFar);
	void SetCameraType(CAMERA_TYPE _eCameraType);

public:
	bool GetUpdate()	const;
	Matrix GetViewMatrix()	const;
	Matrix GetProjectionMatrix()	const;
	float GetNear() const;
	float GetFar() const;

public:
	// �ø��� ������Ʈ�� ����Ұ���
	bool FrustumInPoint(const Vector3& _vPos);
	bool FrustumInSphere(const Vector3& _vCenter, float _fRadius);

public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��, �� ���̾�� ȣ�����ش�.
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);	// ������ �ϱ� ������ ȣ�����ִ� �Լ��Ĥ���
	virtual int Render(float _fTime);
	virtual Camera* Clone()	const;

};

ENGINE_END

