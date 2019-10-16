#pragma once
#include "..\Engine.h"

ENGINE_BEGIN

// ����ü ����
enum FRUSTUM_DIR
{
	FD_LEFT,
	FD_RIGHT,
	FD_TOP,
	FD_BOTTOM,
	FD_NEAR,
	FD_FAR,
	FD_END
};

// �������� �ø��� ���� Ŭ����
// ���� ��� �������� �ø��� �� ����, ����� ������ �̿�
class ENGINE_DLL Frustum
{
private:
	friend class Camera;

public:
	Frustum();
	~Frustum();

private:
	Vector3 m_vPos[8];
	Vector4 m_vPlane[FD_END];

public:
	void Update(const Matrix& _matInvVP);
	bool FrustumInPoint(const Vector3& _vPos);
	bool FrustumInSphere(const Vector3& _vCenter, float _fRadius);
};

ENGINE_END
