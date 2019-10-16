#pragma once
#include "Component.h"

ENGINE_BEGIN

class ENGINE_DLL Navigation : public Component
{
private:
	friend class GameObject;

protected:
	Navigation();
	Navigation(const Navigation& _Com);
	~Navigation();

private:
	Vector3 m_vMousePos;				// ���콺�� �浹�� ���� ��ġ
	bool m_bMousePosCheck;
	bool m_bNavMove;
	class NavigationMesh* m_pNavMesh;


private:
	bool m_bOnCellYPosition;			// �ش� �׺���̼� �޽��� ���� Y������
	bool m_bMouseMove;					// ���콺 �̵����� (���콺�� �̵��Ұ� �����ؼ�)
	bool m_bMapToolMousePos;

	bool m_bAI;							// AI ��������
	std::list<Vector3> m_PathList;		// ��� (��ã������ ã�� ��ε��� ��Ƶд�.)
	class Transform* m_pTarget;			// AI�� ��� ����ٴ� Ÿ�� ����
	Vector3 m_vTargetPos;				// Ÿ�� ��ġ
	Vector3 m_vWay;						// �� ���
	float m_fTargetDetectTime;			// Ÿ�� ��ġ�� üũ�ϴ� �ð�
	float m_fTargetCheckTime;			// �갡 n�� üũ�ϴ¾�

public:
	Vector3 GetMousePos() const;		// ������ �浹�� ����

public:
	void SetCellYPosition(bool _bEnable);
	void SetMouseMove(bool _bEnable);
	void SetAIFindPath(bool _bEnable);
	void SetTarget(class GameObject* _pTarget);
	void SetTarget(class Component* _pTarget);
	void SetTargetDetectTime(float _fTime);

	void SetMapToolOnlyMousePos(bool _bEnable);

	Transform* GetTarget();
	bool GetAIFindPath();

private:
	void CheckMousePos();				// ���콺 ��ġ�Ǻ� �Լ�

	void CheckAIPos(Vector3 _vPos, float _fTime);
	void MoveAI(float _fTime);
	bool FindPath();

public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��, �� ���̾�� ȣ�����ش�.
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);
	virtual int Render(float _fTime);
	virtual Navigation* Clone()	const;
};

ENGINE_END