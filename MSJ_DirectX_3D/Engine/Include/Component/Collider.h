#pragma once
#include "Component.h"

ENGINE_BEGIN

class ENGINE_DLL Collider :
	public Component
{
private:
	friend class GameObject;

protected:
	Collider();
	Collider(const Collider& _Com);
	virtual ~Collider() = 0;

	// ����� ����϶��� ȭ�鿡 ������ ������ֱ� ���� �޽�, ���̴�, ���̾ƿ��� �־��ش�.
#ifdef _DEBUG
protected:
	class Mesh*				m_pMesh;			// ������ �޽�
	class Shader*			m_pShader;			// ���̴�
	ID3D11InputLayout*		m_pLayout;			// ���̾ƿ�
	class RenderState*		m_pWireFrame;		// ���̾ƴ� �� �׵θ��� ����ϱ� ���ؼ� RenderState�ʿ�
	Vector4					m_vColor;			// ��� �׵θ� ����
	TransformConstBuffer	m_tCBuffer;			// �������
#endif // _DEBUG

protected:
	// ���� �ĺ���ȣ�� CollisionManager class���� �����Ѵ�.
	// CollisionManager�� �ش� �ݶ��̴� ��ϵ��� ���� �־���
	// ���ӿ�����Ʈ�� Collider ������ ���� �ĺ� ��ȣ�� �ο��Ѵ�.
	COLLIDER_TYPE		m_eCollType;			// �ݶ��̴� Ÿ�� (��������)
	//unsigned int		m_iSerialNumber;		// �ش� �ݶ��̴��� ���� �ĺ� ��ȣ

	Vector3				m_vSectionMin;			// �ڽ��� ���� ���ҵ� ������(�������߿� 1��) �ּڰ��� �˾Ƴ��� ���� �ڽŸ��� ������ �ּڰ�
	Vector3				m_vSectionMax;			// �ڽ��� ���� ������ �ִ�  ( �Ŀ� �ڽ��� ���Ұ����� ��� ���Ұ����� �˱� ���� �ڽŸ��� ������ �ִ�)
	struct _tagCollisionProfile*	m_pProfile;	// �ڽ��� ���� ������ ��������(�� �������� ���������� �Ѱ��� �����ϴµ� ���������� ä�� ������ ���� ����)
	unsigned int		m_iChannelIndex;		// ä�� �ε���

	unsigned int*		m_pPrevNumber;			// �浹�� �ݶ��̴��� ����(�ĺ���ȣ���� ����)
	unsigned int		m_iPrevSize;
	unsigned int		m_iPrevCapacity;

	unsigned int*		m_pCollisionList;		// �浹�� �ݶ��̴� ���� ( �ݶ��̴� �� ��ü�� ��Ƶ� )
	unsigned int		m_iCollisionSize;
	unsigned int		m_iCollisionCapacity;

	std::function<void(Collider*, Collider*, float)>		m_CallBackFunc[CCBS_END];		// �ݹ� �Լ� ������ ������
	bool			m_bCallBack[CCBS_END];				// �ݹ� ����
	bool			m_bPick;							// ��ŷ�� �ݶ��̴� ����
	bool			m_bAddCollisionManager;				// �ݸ����Ŵ����� �������� ����( ������ �浹ü �Ѱ��� �����Ұ�� �浹�Լ��� ȣ������ X )


public:
	COLLIDER_TYPE	GetColliderType()		const;
	//unsigned int	GetSerialNumber()		const;			// CollisionManager���� �����ĺ���ȣ �޾ƿ���

	Vector3 GetSectionMin()	const;
	Vector3 GetSectionMax()	const;

	struct _tagCollisionProfile* GetCollisionProfile()	const;
	unsigned int GetCollisionChannelIndex()	const;

	bool GetPickEnable()	const;

public:
	void SetSerialNumber(unsigned int _iSerialNumber);		// �ݶ��̴����� �����ĺ���ȣ�� ���µ� �̰� Set���ִ� �Լ�
	void SetChannel(const std::string& _strChannel);
	void SetProfile(const std::string& _strProfile);
	void PickEnable(bool _Value);
	void CollisionManagerEnable(bool _Value);

public:
	void ClearState();

	void AddPrevSerialNumber(unsigned int _iSerialNumber);			// �浹�� �����ĺ���ȣ�� �߰��ϱ�
	void DeletePrevSerialNumber(unsigned int _iSerialNumber);		// �浹�� �����ĺ���ȣ ����

	void AddCollisionList(unsigned int _iSerialNumber);
	void DeleteCollisionList(unsigned int _ISerialNumber);
	bool CheckCollisionList(unsigned int _iSerialNumber);
	bool CheckCollisionList();
	void ClearCollisionList();

	void Call(COLLISION_CALLBACK_STATE _eState, Collider* pDest, float _fTime);			// �ݹ��Լ�

public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��, �� ���̾�� ȣ�����ش�.
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);	// ������ �ϱ� ������ ȣ�����ִ� �Լ�
	virtual int Render(float _fTime);
	virtual Collider* Clone()	const;
	virtual bool Collision(Collider* _pDest) = 0;

public:
	// �浹 �Լ� 
	bool CollisionSphereToSphere(const SphereInfo& _tSrc, const SphereInfo& _tDest);		// Sphere to Sphere
	bool CollisionSphereToRay(const SphereInfo& _tSrc, const RayInfo& _tDest);				// Sphere to Ray
	bool CollisionSphereToOBB(const SphereInfo& _tSrc, const OBBInfo& _tDest);				// Sphere to OBB
	bool CollisionOBBToOBB(const OBBInfo& _tSrc, const OBBInfo& _tDest);					// OBB to OBB
	bool CollisionRayToOBB(const RayInfo& _tSrc, const OBBInfo& _tDest);					// Ray to OBB
public:
	// �ݹ��Լ� �������ֱ� ( �浹ü ���¿� ����.. Enter, Stay, Exit )
	void SetCallback(COLLISION_CALLBACK_STATE _eState,
		void(*pFunc)(Collider*, Collider*, float));

	template <typename T>
	void SetCallback(COLLISION_CALLBACK_STATE _eState,
		T* _pObject, void(T::*pFunc)(Collider*, Collider*, float))
	{
		m_bCallBack[_eState] = true;
		m_CallBackFunc[_eState] = bind(pFunc, _pObject, std::placeholders::_1, 
			std::placeholders::_2, std::placeholders::_3);
	}
};

ENGINE_END
