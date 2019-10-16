#pragma once
#include "Engine.h"

ENGINE_BEGIN

template <typename T>
class ENGINE_DLL Heap
{
public:
	Heap()
	{
		m_iSize = 0;
		m_iCapacity = 100;
		m_pArray = new T[m_iCapacity];

		SetSortFunc(Heap<T>::Sort);
	}

	~Heap()
	{
		SAFE_DELETE_ARRAY(m_pArray);
	}

private:
	T* m_pArray;
	int m_iSize;		// ��, ������ ���� ����
	int m_iCapacity;	// ũ��
	std::function<bool(const T&, const T&)> m_CmpFunc;			// �� ��

public:
	// ����
	void SetSortFunc(bool(*_pFunc)(const T&, const T&))
	{
		m_CmpFunc = bind(_pFunc, std::placeholders::_1, std::placeholders::_2);
	}

	template <typename ClassType>
	void SetSortFunc(ClassType* _pObj, bool(ClassType::*_pFunc)(const T&, const T&))
	{
		m_CmpFunc = bind(_pFunc, _pObj, std::placeholders::_1, std::placeholders::_2);
	}

private:
	// ������ üũ�ؼ� �ø���
	void Resize()
	{
		if (m_iCapacity == m_iSize)
		{
			m_iCapacity *= 2;
			T*	pList = new T[m_iCapacity];
			memset(pList, 0, sizeof(T) * m_iCapacity);
			memcpy(pList, m_pArray, sizeof(T) * m_iSize);
			SAFE_DELETE_ARRAY(m_pArray);
			m_pArray = pList;
		}
	}

public:
	int Size()
	{
		return m_iSize;
	}

public:

	// �̰� ���� ���ϴ¸�ŭ �뷮�� �ַ���������
	void Resize(int _iCapacity)
	{
		m_iCapacity = _iCapacity;

		T*	pList = new T[m_iCapacity];
		memset(pList, 0, sizeof(T) * m_iCapacity);

		SAFE_DELETE_ARRAY(m_pArray);

		m_pArray = pList;
	}

	void Insert(const T& _Data)
	{
		Resize();

		m_pArray[m_iSize] = _Data;
		++m_iSize;

		_Insert(m_iSize - 1);
	}

	// ���� ���� �����̴�. (FIFO)
	bool Pop(T& _Data)
	{
		if (m_iSize == 0)
			return false;

		_Data = m_pArray[0];

		m_pArray[0] = m_pArray[m_iSize - 1];
		--m_iSize;

		_Pop(0);

		return true;
	}

	void Clear()
	{
		m_iSize = 0;
	}

	bool Empty() const
	{
		return m_iSize == 0;
	}

	// ���� �ִ°� ������ �����ϴ°��� (������)
	void Sort()
	{
		int	iIndex = m_iSize / 2 - 1;			//������ ����� �θ� ã�ƿ���

		// �θ��ε����⿡.. ��Ʈ��带 ������ ��尡 ������ �������ϴ� while����
		while (iIndex >= 0)
		{
			int	iBuildCount = 1;
			int	iBuildIndex = iIndex + 1;

			while (iBuildIndex /= 2)
			{
				iBuildCount *= 2;
			}

			--iBuildCount;

			iBuildCount = iIndex - iBuildCount;

			for (int i = 0; i <= iBuildCount; ++i)
			{
				_Pop(iIndex - i);
			}

			iIndex = iIndex - iBuildCount - 1;
		}
	}

	private:
		void _Insert(int iIndex)
		{
			if (iIndex == 0)
				return;

			int	iParent = (iIndex - 1) / 2;

			// �־��� ���� �θ� �� ���� ���ؼ� ������ �Ŀ� �־��ش�. (����Լ� ȣ��)
			if (m_CmpFunc(m_pArray[iParent], m_pArray[iIndex]))
			{
				T	data = m_pArray[iParent];
				m_pArray[iParent] = m_pArray[iIndex];
				m_pArray[iIndex] = data;

				_Insert(iParent);
			}
		}

		void _Pop(int iIndex)
		{
			// ���� �ڽ��� �ε����� ���Ѵ�.
			int	iLeftChild = iIndex * 2 + 1;

			if (iLeftChild >= m_iSize)
				return;

			int	iRightChild = iLeftChild + 1;
			int	iChildIndex = iLeftChild;

			if (iRightChild < m_iSize)
			{
				if (m_CmpFunc(m_pArray[iLeftChild], m_pArray[iRightChild]))
					iChildIndex = iRightChild;
			}

			if (m_CmpFunc(m_pArray[iIndex], m_pArray[iChildIndex]))
			{
				T	data = m_pArray[iChildIndex];
				m_pArray[iChildIndex] = m_pArray[iIndex];
				m_pArray[iIndex] = data;

				_Pop(iChildIndex);
			}
		}

private:
	// �� �� ����
	static bool Sort(const T& src, const T& dest)
	{
		return src > dest;
	}
};

ENGINE_END