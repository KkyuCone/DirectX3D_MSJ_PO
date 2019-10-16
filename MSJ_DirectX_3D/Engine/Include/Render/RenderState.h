#pragma once
#include "..\Reference.h"

ENGINE_BEGIN

class ENGINE_DLL RenderState : public Reference
{ 
	//Raterizer, DepthStencil, Blend ��� DeviceChild�� ��� �޴´�.
	//( �ٿ�ĳ���� ���ٰ��� ������, �̰� �ϳ��θ� �����Ұ�)
	//���� ��� �����ϴ� ����� ���ݾ� �ٸ��� ������ �Ҹ��ڿ� virtual�� �߰����� �ʰ�
	//�����ϴ� �Լ� �κп� virtual�� �߰��Ѵ�. + �ʼ��� �����ؾ��ϱ� ������ ���������Լ��� �����
private:
	friend class RenderManager;

public:
	RenderState();
	~RenderState();

protected:
	ID3D11DeviceChild*	m_pCurState;			// ���� ���� ����
	ID3D11DeviceChild*	m_pOldState;			// ������ ���� ���� ( ���¸� ��� �ٲ��ٶ� ���� ���¸� �˰� �־�� �ϱ� ������ �߰����ذ�)
	RENDER_STATE		m_eType;

public:
	RENDER_STATE GetRenderStateType()	const;

public:
	virtual void SetState() = 0;		// �������� ������Ʈ(�ٲ��ٽ�����Ʈ)
	virtual void ResetState() = 0;		// ���� ������Ʈ�� ���ư�

};

ENGINE_END

