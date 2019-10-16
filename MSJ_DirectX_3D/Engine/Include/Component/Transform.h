#pragma once
#include "Component.h"

ENGINE_BEGIN

// Ŭ������ �����Ҵ��ϸ� 4����Ʈ ������ ó���ϰԵȴ�.
// �׷��� Transform���� ���� ��Ʈ���� �� 16����Ʈ ������ ��������� �����ִµ�
// __declspec(align(16))�� ���� �ʴ� ���·� ����ϰԵȴٸ�
// 4����Ʈ ������ ó���ϰԵǴϱ� ������ �ȵȴ�!
// �׷��ϱ� 16����Ʈ�� ó���ϱ� ���ؼ� __declspec(align(16))�� ����Ѵ�.
class ENGINE_DLL __declspec(align(16)) Transform : public Component
{
private:
	friend class GameObject;

private:
	Transform();
	Transform(const Transform& _Component);
	~Transform();

private:
	bool	m_bStatic;			// �����̴� ������Ʈ���� �ƴ��� �Ǻ��ϱ� ���� ���� (false�� �����̴� ������Ʈ, true�� �������� �ʴ� ������Ʈ)
	bool	m_bUpdate;			// ������ �ѹ��� Update�ؾ��Ѵ�. m_bUpdate�� ���� ����� �ٽ� ����Ұ����� �����̴�. Transform�� ��� ��ġ, ũ��, ȸ���� �ٲ𶧸��� ����� �ٽ� ����ؾ��Ѵ�.
	bool	m_bUI;				// UI ������Ʈ ���� 
	bool	m_bParentOnlyPosition;	// �θ� ������ �ִ� ������Ʈ�� �߿��� ������ ���� �������� ����
	TransformConstBuffer	m_tConstBuffer;		// ������� ( ����, ��, �������� ����� �������� )
	TransformConstBuffer	m_tShadowCBuffer;

public:
	bool GetIsStatic()	const;
	bool GetIsUpdate()	const;
	bool GetUIEnable()	const;
	bool GetParentOnlyPosition() const;

public:
	void SetUIEnable(bool _Enable);
	void SetParentOnltyPositon(bool _Enable);

	/////////////////////////////////////////////////////////////////// ����
private:
	Vector3 m_vLocalPosition;
	Vector3 m_vLocalRotation;
	Vector3	m_vLocalSacle;

	// ���� ȸ���� ���ú信�� ������ ��ģ��.( ���� �ٲ� )
	Vector3 m_vLocalRelativeView;
	Vector3 m_vLocalView;

public:
	Vector3 GetLocalPosition()	const;
	Vector3	GetLocalRotation()	const;
	Vector3	GetLocalScale()		const;
	Vector3 GetLocalView()		const;

public:
	void SetLocalPosition(float _x, float _y, float _z);
	void SetLocalPosition(const Vector3& _vPosition);

	void SetLocalRotation(float _x, float _y, float _z);
	void SetLocalRotation(const Vector3& _vRotation);
	void SetLocalRotateX(float _x);
	void SetLocalRotateY(float _y);
	void SetLocalRotateZ(float _z);

	void SetLocalScale(float _x, float _y, float _z);
	void SetLocalScale(const Vector3& _vScale);

	void SetLocalRelativeView(float _x, float _y, float _z);
	void SetLocalRelativeView(const Vector3& _vView);

private:
	Matrix	m_MatrixLocalPosition;
	Matrix	m_MatrixLocalRotation;
	Matrix	m_MatrixLocalScale;
	Matrix	m_MatrixLocal;

public:
	Matrix GetLocalRotationMatrix() const;
	Matrix GetLocalPositionMatrix() const;
	Matrix GetLocalMatrix()	const;
	Matrix GetLocalRotPos() const;

	///////////////////////////////////////////////////////////////// ����
private:
	Vector3 m_vWorldPosition;
	Vector3 m_vWorldRotation;
	Vector3 m_vWorldScale;

	// �� ������Ʈ���� �ڽŸ��� ������ ���� �����ִ�. (Axis) �������̾ƴ� �ڽ��� ������ ����
	// ������ ���� ĳ���Ͱ� �̵�, ȸ���� ���ϵ����� (������ ���ٴ���, �ڷΰ��ٴ��� ��..)
	Vector3 m_vWorldAxis[AXIS_END];
	Vector3 m_vWorldView;
	Vector3 m_vMove;
	bool m_bLookAt;
	Transform* m_pLookAt;			// �ٶ󺸴� ����
	LOOKAT_AXIS m_eLookAxis;

public:
	void SetLookAtActive(bool _Active);

public:
	Vector3 GetWorldPosition()	const;
	Vector3 GetPrevWorldPosiitonAtMatrix() const;
	Vector3 GetWorldPositionAtMatrix() const;
	Vector3 GetWorldRotation()	const;
	Vector3 GetWorldScale()		const;
	Vector3 GetWorldAxis(AXIS _Axis)	const;
	Vector3 GetWorldView()	const;
	Vector3 GetMove()	const;

public:
	void SetWorldPosition(float _x, float _y, float _z);
	void SetWorldPosition(const Vector3& _Position);

	void SetWorldRotation(float _x, float _y, float _z);
	void SetWorldRotation(const Vector3& _Rotation);
	void SetWorldRotateX(float _x);
	void SetWorldRotateY(float _y);
	void SetWorldRotateZ(float _z);

	void SetWorldScale(float _x, float _y, float _z);
	void SetWorldScaleX(float _x);
	void SetWorldScaleY(float _y);
	void SetWorldScaleZ(float _z);
	void SetWorldScale(const Vector3& _Scale);

	void SetLookAtAxis(LOOKAT_AXIS _eAxis);

private:
	// ũ��(ȸ��)�̰���
	// ��������� �������� �θ𿡼� ���带 ����Ҷ� �־��ٰű⶧�� ������
	Matrix	m_MatrixWorldScale;				// ũ��
	Matrix	m_MatrixWorldRotation;			// ȸ��
	Matrix	m_MatrixWorldPosition;			// �̵�(��ġ)
	Matrix	m_MatrixWorldParent;			// �θ�

	Matrix m_PrevMatrixWolrd;
	Matrix m_MatrixWorld;					// �������

public:
	void SetParentMatrix(const Matrix& _MatrixParent);

public:
	Matrix GetWorldRotationMatrix() const;
	Matrix GetWorldPositionMatrix() const;
	Matrix GetWorldMatrix()	const;

	Matrix GetWorldRotPosMatrix() const;

public:
	void Move(AXIS _eAxis, float _fSpeed, float _fTime);
	void Move(const Vector3& _vDir, float _fSpeed, float _fTime);
	void Move(const Vector3& _vMove);
	void MoveBack();

	void Rotate(const Vector3& _vRot, float _fTime);
	void Rotate(const Vector3& _vRot);
	void RotateX(float _fSpeed, float _fTime);
	void RotateX(float _fSpeed);
	void RotateY(float _fSpeed, float _fTime);
	void RotateY(float _fSpeed);
	void RotateZ(float _fSpeed, float _fTime);
	void RotateZ(float _fSpeed);

	void LookAt(class GameObject* _pObject);
	void LookAt(Component* _pComponent);
	void RemoveLookAt();
	void RotationLookAt();
	void RotationLookAt(Vector3 _vLookAt);
private:
	// ��ȸ��
	void ComputeAxis();

public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);	// ������ �ϱ� ������ ȣ�����ִ� �Լ��Ĥ���
	virtual int PrevShadowRender(Matrix _matLightView, Matrix _matLightProj, float _fTime);
	virtual int Render(float _fTime);
	virtual Component* Clone()	const;

	void UpdateCBuffer();

};

ENGINE_END
