#pragma once
#include "Engine.h"

ENGINE_BEGIN

class ENGINE_DLL SerialNumber
{
public:
	SerialNumber();
	~SerialNumber();

private:
	unsigned int	m_iNumber;			// �갡 �ĺ���ȣ
	unsigned int*	m_pValidNumber;		// ������ �ĺ���ȣ ����
	unsigned int	m_iSize;			// ������ �ĺ���ȣ ����
	unsigned int	m_iCapacity;		// ����
	unsigned int	m_iMin;				// �ν��Ͻ� �ּҰ�
	unsigned int	m_iMax;				// �ν��Ͻ� �ִ밪..?

public:
	void			SetMinMax(unsigned int _iMin, unsigned int _iMax);
	unsigned int	GetSerialNumber();
	void			AddValidNumber(unsigned int _iNumber);
};

ENGINE_END
