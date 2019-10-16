#pragma once
#include "Engine.h"

ENGINE_BEGIN

class ENGINE_DLL Reference
{
protected:
	Reference();
	virtual ~Reference();

protected:
	int			m_iReferenceCount;
	std::string m_strTag;
	bool		m_bActive;				// �ش� ��ü�� ����ִ��� ����
	bool		m_bEnable;				// �ش� ��ü�� Ȱ��ȭ �Ǿ��ִ��� ����
	unsigned int m_iSerialNumber;		// �ĺ� ��ȣ

public:
	unsigned int GetSerialNumber() const;
	void SetSerialNumber(unsigned int _iNumber);


public:
	void Release();
	void AddReference();

	void SetTag(const std::string& _pTagName);
	std::string GetTag() const;

	bool IsActive()	const;
	bool IsEnable() const;

	void Active(bool _bActive);
	void Enable(bool _bEnable);
};

ENGINE_END