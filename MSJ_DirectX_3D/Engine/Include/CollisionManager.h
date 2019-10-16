#pragma once

#include "Component\Collider.h"

ENGINE_BEGIN

#define MAX_COLLISION_CHANNEL	32				// ���� �浹ü�� ä���� �� 32���� ���Ѵ�. (�� �̻��� ��� ȿ���� ���� ������ )

// ������ �����ؼ� �浹 ó���� �� �ǵ�
// ���� ������ �׸���� �ɰ�����.(���ڹ��)
// �׸��� �浹ü���� �����ɶ� �����ĺ���ȣ(Serial Number)�� �ο��Ǹ�, �Ѱ�� 2^N ������.. 4096����
// �浹�� ������� ������� ó���� ���̴�.
//
//   0 1 2 3 4 5 
// 0 0 1 0 0 0 0
// 1 0 0 0 0 0 0
// 2 0 0 0 0 0 0
// 3 0 0 0 0 0 0
// 4 0 0 1 0 0 0
// 5 0 0 0 0 0 0				// 1�� �浹 0 �� �浹 X �׸��� �����ڽŵ��� �浹���� �����Ƿ� �� �밢������ ������ 0 �̴�.


// ó�� _tagColliderInfo�� �����ڿ���
//   0 1 2 3 4 5 . . . . . 127     <- ������ ��� �Ѱ��� int->4����Ʈ�� -> ��Ʈ������ �ɰ��� 32���� ������� 32 * 128 = 4096  �� 4096 * 4096 ��������� ���´�.
// 0 0
// 1   0
// 2     0
// 3       0
// 4         0
// ...         0
// 4096                     0

// �ݶ��̴� ����
typedef struct ENGINE_DLL _tagColliderInfo
{
	Collider**		pColliderList;		// �浹�� �ݶ��̴��� ( ����ʿ� ��ġ�� ��� �ݶ��̴���)
	unsigned int**	pAdjMatrix;			// ������� ( �浹 üũ �뵵 )
	unsigned int	iSize;				// pColliderList ����
	unsigned int	iCapacity;

	// ������ �Ҵ�Ȱͺ��� �� �߰��Ϸ���������
	void Resize()
	{
		iCapacity *= 2;

		Collider** pList = new Collider*[iCapacity];

		memcpy(pList, pColliderList, sizeof(Collider*) * iSize);

		SAFE_DELETE_ARRAY(pColliderList);

		pColliderList = pList;

		unsigned int** pMatrix = new unsigned int*[iCapacity];

		unsigned int	iWidth = iCapacity / 32;					// 128  -> 32(int�� ��Ʈ������..) * 128 = 4096
		
		for (unsigned int i = 0; i < iCapacity; ++i)
		{
			pMatrix[i] = new unsigned int[iWidth];
			memset(pMatrix[i], 0, sizeof(unsigned int) * iWidth);
			memcpy(pMatrix[i], pAdjMatrix[i], sizeof(unsigned int) * (iWidth / 2));		// ������ �ִ��͵��� �־������
			SAFE_DELETE_ARRAY(pAdjMatrix[i]);
		}

		SAFE_DELETE_ARRAY(pAdjMatrix);
		pAdjMatrix = pMatrix;
	}

	_tagColliderInfo()
	{
		iSize = 0;
		iCapacity = 4096;
		pColliderList = new Collider*[iCapacity];
		pAdjMatrix = new unsigned int*[iCapacity];

		unsigned int	iWidth = iCapacity / 32;			// ���η� 32���� �����ٰ��� = 128
		
		for (unsigned int i = 0; i < iCapacity; ++i)
		{
			pAdjMatrix[i] = new unsigned int[iWidth];
			memset(pAdjMatrix[i], 0, sizeof(unsigned int) * iWidth);
		}
	}

	~_tagColliderInfo()
	{
		SAFE_DELETE_ARRAY(pColliderList);

		for (unsigned int i = 0; i < iCapacity; ++i)
		{
			SAFE_DELETE_ARRAY(pAdjMatrix[i]);
		}

		SAFE_DELETE_ARRAY(pAdjMatrix);
	}

}ColliderInfo, *PColliderInfo;

// �ݶ��̴� ����Ʈ
typedef struct ENGINE_DLL _tagColliderList
{
	Collider**	pList;
	int		iSize;
	int		iCapacity;

	_tagColliderList()
	{
		iSize = 0;
		iCapacity = 100;
		pList = new Collider*[iCapacity];
		memset(pList, 0, sizeof(Collider*) * iCapacity);
	}

	~_tagColliderList()
	{
		SAFE_DELETE_ARRAY(pList);
	}

	// resize()
	void Resize()
	{
		iCapacity *= 2;
		Collider** pNewList = new Collider*[iCapacity];
		memset(pNewList, 0, sizeof(Collider*) * iCapacity);		// �ϴ� 0���� ���� �ʱ�ȭ
		memcpy(pNewList, pList, sizeof(Collider*) * iSize);

		SAFE_DELETE_ARRAY(pList);			// ������ ������ ���� �߰�(�ٽ� �־��ִ°���)
		pList = pNewList;
	}

	// �ݶ��̴� �߰��Լ�
	void Add(class Collider* _pCollider)
	{
		// �޸𸮰����� ���� á���� ���� �÷��ֱ�
		if (iSize == iCapacity)
		{
			Resize();
		}

		pList[iSize] = _pCollider;
		++iSize;
	}

	// Ŭ�����Լ�
	void Clear()
	{
		iSize = 0;
	}

}ColliderList, *PColliderList;

// �ݸ��� ä��
typedef struct ENGINE_DLL _tagCollisionChannel
{
	std::string strName;
	COLLISION_CHANNEL_STATE eState;

	_tagCollisionChannel()
	{
		strName = "";
		eState = CCS_IGNORE;			// �⺻��
	}
}CollisionChannel, *PCollisionChannel;

// �ݸ��� �������� -> �� ������Ʈ���� �浹ü�� ���� ��� �ݸ��� ���������� Init()���� ����Ǿ���Ѵ�.
typedef struct ENGINE_DLL _tagCollisionProfile
{
	std::string strName;
	PCollisionChannel	pChannelList;				// ���������� ä�θ���Ʈ���� ���� �ִ�.
	unsigned int		iChannelSize;

	PCollisionChannel FindChannel(const std::string& _strName)
	{
		for (unsigned int i = 0; i < iChannelSize; ++i)
		{
			if (pChannelList[i].strName == _strName)
			{
				return &pChannelList[i];
			}
		}

		return nullptr;
	}

	_tagCollisionProfile()
	{
		strName == "";
		iChannelSize = 0;
		pChannelList = new CollisionChannel[MAX_COLLISION_CHANNEL];
	}

	~_tagCollisionProfile()
	{
		SAFE_DELETE_ARRAY(pChannelList);
	}

}CollisionProfile, *PCollisionProfile;

// �ݸ��� ���� ( ���ҵ� ����)
typedef struct ENGINE_DLL _tagCollisionSection
{
	PColliderList	pSectionList;			// ���ҵ� ������
	int		iNumX;							// X������ �󸶳� �ɰ�����
	int		iNumY;
	int		iNumZ;
	Vector3	vMin;							// ������ �ּڰ�
	Vector3	vMax;							// ������ �ִ�
	Vector3	vCellSize;						// �� ������ ������
	Vector3	vLength;						// ������ �� ũ��

	_tagCollisionSection() :
		iNumX(1),
		iNumY(1),
		iNumZ(1),
		pSectionList(nullptr)
	{
	}

	~_tagCollisionSection()
	{
		SAFE_DELETE_ARRAY(pSectionList);
	}
}CollisionSection, *PCollisionSection;

class ENGINE_DLL CollisionManager
{
private:
	ColliderInfo	m_tColliderInfo;
	unsigned int	m_iSerialNumber;			// ���� �ĺ���ȣ�� �� ��� ����������??;;

#pragma region Vaild ����
	// Collider�� ���� �ĺ���ȣ�� ���������� ���� �ű�� �۾��� ���� �ʰ�
	// ������ �ֵ��� ��Ƶд�. -> m_pValidNumber
	// �̴� �ű�� �۾����� ȿ�����̱� ����
	// �׳� �θ� ���Ѵ�� �޸𸮰����� ���⶧���� �׿� �޸𸮰� ����. �̰� �����ϱ� ���ؼ� ����Ѵ�.
#pragma endregion
	unsigned int*	m_pValidNumber;				// ������ Collider�� �����ĺ���ȣ ����
	unsigned int	m_iValidSize;				// ������ Collider�� �����ĺ���ȣ ����
	unsigned int	m_iValidCapacity;

private:
	PCollisionSection	m_pSection;				// ���ҵ� ���� ( �Ϲ� ��� �浹 ���� )
	PCollisionSection	m_pPickSection;			// ��ŷ�� �浹 ���� ( ���⼭ ���콺 ���̿� �´� �����鸸 ���߿� ó�� ������ !! (�� ����;;)
	PCollisionSection	m_pUISection;			// UI �浹 ����

	PColliderList		m_pMouseCollisionList;	// ���콺 �浹 ����Ʈ (���� ��Ƶּ� �����ؾ���)

	Vector3				m_vPos;					// ������ ������ ( Main camera�� ��ġ )
	Vector3				m_vPivot;				// 0~1.0f�����ǰ�, �� �ۼ�Ʈ�� �̿��

	class Collider* m_pPrevMousePick;			// ���� �����ӿ� ���콺�� �浹�� ��ŷ�� �ݶ��̴� ����
	class Collider* m_pPrevMouseCollider;		// ���� �����ӿ� ���콺�� �浹�� ���� ������Ʈ �ݶ��̴� ����

private:
	PCollisionProfile	m_pProfileList;			// Profile List 
	unsigned int		m_iProfileSize;
	unsigned int		m_iProfileCapacity;

	PCollisionChannel	m_pChannelList;			// Channel List -> ��ü ä�� ����Ʈ
	unsigned int		m_iChannelSize;

public:
	unsigned int GetSerialNumber();								// �ĺ� ��ȣ�� �ο��ϴ� Get�Լ�
	void AddValidSerialNumber(unsigned int _iNumber);			// �ݶ��̴��� �����ɶ����� �ش� �ĺ���ȣ�� ������ �Լ�(��ư ������ �ݶ��̴��� �ĺ���ȣ�� ��Ƽ� ���߿� �ݶ��̴��� �����Ҷ� ������ �ݶ��̴��� �ĺ���ȣ�� ���� ��� �� �ĺ���ȣ�� �ο��� ����)

	// �浹�Ѿֵ��� ��Ƶΰų� ����, üũ�� �Լ�
	void AddPrevCollider(unsigned int _iSrc, unsigned int _iDest);
	void DeletePrevCollider(unsigned int _iSrc, unsigned int _iDest);
	bool CheckPrevCollider(unsigned int _iSrc, unsigned int _iDest);

private:
	class GameObject*	m_pPickObject;
public:
	class GameObject* GetPickColliderObject();

public:
	// ���� ����
	void CreateSection(int _iNumX, int _iNumY, int _iNumZ,
		const Vector3& _vCellSize, const Vector3& _vPos = Vector3::Zero,
		const Vector3& _vPivot = Vector3::Zero);

	bool AddChannel(const std::string& _strName, COLLISION_CHANNEL_STATE eState = CCS_IGNORE);	// ä�� �߰�, ����� 3���� ���� ���߿� ä�ΰ����� �߰��ϰ� ������ �߰����ָ� �ȴ�.
	bool DeleteChannel(const std::string& _strName);											// ä�� ����
	bool CheckChannel(const std::string& _strName);
	unsigned int GetChannelIndex(const std::string& _strName);				// ä�� �ε��� ã��

	bool CreateProfile(const std::string& _strName);						// �������� ���� 
	bool SetProfileChannelState(const std::string& _strProfile,
		const std::string& _strChannel, COLLISION_CHANNEL_STATE _eState);	// �������Ͼ��� ä�� �������ٲܼ��ִ� �Լ�
	PCollisionProfile FindProfile(const std::string& _strName);				// �������� ã��

	void AddCollider(class GameObject* _pObject);							// �ݶ��̴� �߰��ϱ�(�����Ǹ� �߰��ϴµ�) -> ������ �����ɶ����� ȣ���Ҿ�
	void AddCollider(class Collider* pCollider);

	class Collider* FindCollider(unsigned int _iSerialNumber);				// �ݶ��̴� ã�� ( ���� �ĺ� ��ȣ�� )
	void DeleteCollider(unsigned int _iSerialNumber);						// �ݶ��̴� ����
	void ComputeSection();													// ���� ���

public:
	bool Init();
	void Collision(float _fTime);
	void Render(float _fTime);

public:
	void DeletePrevMouse(Collider* _pCollider);

public:
	void AddColliderSize();

private:
	bool CollisionMouseUI(float _fTime);			// ���콺�� UI �浹
	bool CollisionMouseWorld(float _fTime);			// ���콺�� ���� ������Ʈ�� �浹
	bool CollisionWorld(float _fTime);				// ���� ������Ʈ�鳢�� �浹(�����浹)
	bool Collision(class Collider* _pSrc, class Collider* _pDest);	// �浹

public:
	static int SortZ(const void* _pSrc, const void* _pDest);

	DECLARE_SINGLETON(CollisionManager)
};

ENGINE_END
