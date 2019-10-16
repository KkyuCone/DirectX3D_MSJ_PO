#pragma once
#include "..\Reference.h"
#include "..\Heap.h"

ENGINE_BEGIN

// A��Ÿ�Ҷ� �ʿ�, ���� ����
enum NAVIMESH_CELL_LIST_TYPE
{
	NCLT_NONE,			// X ( �� ù�κ� )
	NCLT_OPEN,			// ���� ���
	NCLT_CLOSE			// ���� ���
};

// �������� (���� ���� �������� �ε���)
// �̵��ϴ� ������Ʈ�� �̵��� ������ �ε������� �˰� �־���Ѵ�. (�������� ���� �����￡��..)
typedef struct ENGINE_DLL _tagAdjInfo
{
	int	iIndex;			// �ش��ϴ� �ε���
	int	iEdgeIndex;		// ���� �ε��� (0, 1, 2)
}ADJINFO, *PADJINFO;

// �׺���̼��� ���� ������ �����Ѵ�.(�ּҴ��� �ﰢ����)
typedef struct ENGINE_DLL _tagNavigationCell
{
	NAVIMESH_CELL_LIST_TYPE	eType;		// A��Ÿ�Ҷ� ���
	Vector3		vPos[3];				// �ﰢ���� ���� ����
	Vector3		vEdge[3];				// ��������
	Vector3		vEdgeCenter[3];			// �� �������� ����
	Vector3		vCenter;				// �ش� �ﰢ���� ����
	std::vector<ADJINFO>	vecAdj;		// �ٸ� ��� ������ �ε�������(����)
	int			iIndex;					// ���� �ε���
	int			iParentIdx;				// A��Ÿ��
	float		fG;						// A��Ÿ�� -  ������� (����ġ ���)
	float		fH;						// A��Ÿ�� - �޸���ƽ(Heuristics) or �߰߹�
	float		fTotal;					// A��Ÿ��
	bool		bEnable;				// �׺�޽� �� ���翩��, �÷��̾ �ٸ� ������Ʈ�� �̵��ÿ� �ش� ���� �����ؾ߸� �̵������ϰ� ��.

	void Clear()
	{
		eType = NCLT_NONE;
		iParentIdx = -1;
		fG = -1.f;
		fH = -1.f;
		fTotal = -1.f;
	}

	_tagNavigationCell()
	{
		eType = NCLT_NONE;
		iParentIdx = -1;
		iIndex = -1;
		fG = -1.f;
		fH = -1.f;
		fTotal = -1.f;
		bEnable = true;
	}
}NavigationCell, *PNavigationCell;

// ������Ʈ ( ���� �Ѱ��� ������������, ������ ���� )
typedef struct ENGINE_DLL _tagCellList
{
	PNavigationCell*	pCellList;
	int			iSize;
	int			iCapacity;

	void Resize()
	{
		if (iSize == iCapacity)
		{
			iCapacity *= 2;
			PNavigationCell*	pList = new PNavigationCell[iCapacity];

			memset(pList, 0, sizeof(PNavigationCell) * iCapacity);

			memcpy(pList, pCellList, sizeof(PNavigationCell) * iSize);

			SAFE_DELETE_ARRAY(pCellList);
			pCellList = pList;
		}
	}

	void Add(PNavigationCell pCell)
	{
		Resize();
		pCellList[iSize] = pCell;
		++iSize;
	}

	_tagCellList()
	{
		iSize = 0;
		iCapacity = 2048;
		pCellList = new PNavigationCell[iCapacity];

		memset(pCellList, 0, sizeof(PNavigationCell) * iCapacity);
	}

	~_tagCellList()
	{
		SAFE_DELETE_ARRAY(pCellList);
	}
}CellList, *PCellList;

// �׺���̼ǵ� �浹ü ó�� ������ ������.
typedef struct ENGINE_DLL _tagNavSection
{
	Vector3		vSize;					// �׺� ���� .. ���° �������� (�浹üó��..)
	CellList	tCellList;				// �Ѱ����� � ���� �����ϴ���.. (����Ʈ���� �������) �Ƹ� ���������� ����������
	Vector3		vMin;					// �׺� ������ �ּڰ�
	Vector3		vMax;					// �׺� ���� �ִ밪

	void Add(PNavigationCell pCell)
	{
		tCellList.Add(pCell);
	}

	_tagNavSection()
	{
	}

	~_tagNavSection()
	{
	}
}NavSection, *PNavSection;

// �츮�� �޸���ƽ �˰����� �̿��� A��Ÿ ������

// �޸���ƽ �˰��� A*�� ���� ���
// 1. Open List			: �������� ���� ������ ��� -> ������ Closed List�� �̵�
// 2. Closed List		: ������ ������ ���  -> Open List�׸� ������ ���⿡ �ִ°���.
// 3. CostFormStart[x]	: ���� ���� ���� ������ġ������ ��� -> g(n) �츮 ������븻�ϴ°��� �װ��� �� �հ�.
// 4. CostFromGoal[x]	: ���� ��ġ���� ��ǥ �������� ��� -> h(n) �޸���ƽ 
// 5. TotalCost[x]		: �� �˰����� ���� �� ����� �����ؾ��� ���� f(n) = g(n) + h(n)

// �켱����ť -> A��Ÿ�Ҷ� ���
class ENGINE_DLL NavigationMesh : public Reference
{
private:
	friend class NavigationManager;

private:
	NavigationMesh();
	~NavigationMesh();

	std::string		m_strName;
	std::vector<PNavigationCell>	m_vecCell;			// �ش� �׺�Ž��� ��ü ������
	PNavigationCell*		m_pCloseCellList;				// A��Ÿ�Ҷ� ��� ( ���λ���) -> �̹� ��θ� �����͵� ����
	int						m_iCloseCellListSize;
	Vector3		m_vOffset;								// �׺���̼��� ��𼭺��� �����ϴ����� ���� ������
	Vector3		m_vOffsetScale;
	Vector3		m_vMin;									// �׺�޽��� �ּڰ�(��ġ)
	Vector3		m_vMax;									// �׺�޽��� �ִ�(��ġ)
	int			m_iLineRectCount;						// ����(�׸���)�� �簢�� ����
	bool		m_bGrid;								// �׸��� ����
	int			m_iSectionX;							// �׺���̼� �������鶧 X, ���η� ��� �������� �������
	int			m_iSectionZ;							// ���� ���� Z, ������
	Vector3		m_vSectionSize;							// �� �׺������ ũ��
	PNavSection	m_pSection;								// �׺���̼� ����

private:
	Heap<PNavigationCell>		m_OpenList;				// A��Ÿ�� - ���� ���
	std::stack<int>				m_FindStack;			// A��Ÿ�� -  
	std::list<Vector3>			m_PathList;				// A��Ÿ�� - ��ζ���ؾ��ϳ� ��ġ����Ʈ��
	bool						m_bFindEnd;

private:
	// ������ (�����ִ� ������, ���� ������ ǥ�ÿ뵵)
//#ifdef _DEBUG
//	std::vector<VertexColor> m_vecDebugVertex;
//	std::vector<int>		m_vecDebugIndex;
//	class Mesh* m_pDebugMesh;			// ������ �޽�
//	class Shader*			m_pShader;			// ���̴�
//	ID3D11InputLayout*		m_pLayout;			// ���̾ƿ�
//	class RenderState*		m_pWireFrame;		// ���̾ƴ� �� �׵θ��� ����ϱ� ���ؼ� RenderState�ʿ�
//	Vector4					m_vColor;			// ��� �׵θ� ����
//	TransformConstBuffer	m_tCBuffer;			// �������
//#endif // _DEBUG
	std::vector<VertexColor> m_vecDebugVertex;
	std::vector<int>		m_vecDebugIndex;
	class Mesh* m_pDebugMesh;			// ������ �޽�
	class Shader*			m_pShader;			// ���̴�
	ID3D11InputLayout*		m_pLayout;			// ���̾ƿ�
	class RenderState*		m_pWireFrame;		// ���̾ƴ� �� �׵θ��� ����ϱ� ���ؼ� RenderState�ʿ�
	Vector4					m_vColor;			// ��� �׵θ� ����
	TransformConstBuffer	m_tCBuffer;			// �������

public:
	Vector3	GetMin()	const;
	Vector3 GetMax()	const;
	Vector3 GetOffset()	const;
	std::list<Vector3> GetPathList()	const;

public:
	void CreateDebugNavMesh(std::string _strNavMesh, std::vector<Vector3> _Vtx, std::vector<int> _Idx);


public:
	void SetOffset(const Vector3& _vOffset);
	void SetOffsetScale(const Vector3& _vOffsetScale);

public:
	void PathListClear();

public:
	bool Init();
	void AddCell(const Vector3 _vPos[3]);				// �׺� �߰�
	void AddAdj(int _iCellIdx, int _iAdjIdx);			// �׺��� �������� �߰� ( ������ ������ �̵��� �����ϴϱ� )
	void CreateGridMapAdj(int _iLineRectCount);			// �׸���� �� ���������߰�
	void CreateAdj();									// �������� ( �׸��� �ƴϴ��� ��밡���ϴ�.)
	bool CheckOnEdge(int _iSrc, int _iDest, const Vector3& _vOrigin1, const Vector3& _vOrigin2,
		const Vector3& _vEdge, float _fEdgeLength, int _iEdge1, int _iEdge2);
	bool CheckPathDir(const Vector3& _vSrc1, const Vector3& _vSrc2,
		const Vector3& _vDest1, const Vector3& _vDest2, Vector3& _vIntersect);
	void FindPath(const Vector3& _vStart, const Vector3& _vEnd);

public:
	float GetY(const Vector3& _vPos);
	bool CheckCell(const Vector3& _vPos);
	bool GetCellEnable(const Vector3& _vPos);
	float GetY(int _iCellIndex, const Vector3& _vPos);
	void CreateSection();

	Vector3 GetCellFaceNormal(const Vector3& _vPos);		// ����� �˾Ƴ���

	PNavigationCell FindCell(const Vector3& _vPos);
	int				FindCellIndex(const Vector3& _vPos);

	float NavigationMesh::GetFaceNormalAngle(Vector3 _vPos);
	                     
private:
	void AddOpenList(PNavigationCell _pCell, PNavigationCell _pEnd,
		const Vector3& _vStart, const Vector3& _vEnd);
	int GetCellIndex(const Vector3& _vPos);
	bool RayIntersectTriangle(Vector3 _rayOrigin, Vector3 _rayDir,
		Vector3 _v0, Vector3 _v1, Vector3 _v2,
		float& _t, Vector3& _vIntersect);
	int GetSectionIndex(Vector3 _vPos);

public:
	void Save(const char* _pFileName, const std::string& _strPathKey = PATH_DATA);
	void SaveFromFullPath(const char* pFullPath);
	void Load(const char* _pFileName, const std::string& _strPathKey = PATH_DATA);
	void LoadFromFullPath(const char* _pFullPath);

private:
	bool OpenListSort(const PNavigationCell& _pSrc,
		const PNavigationCell& _pDest);

public:
	// ������ ���� �׺�޽��� �����浹 -> Ŭ���� ���� ã�� ( Ray, OBB �浹 �̿� )
	std::vector<int> vMouseSectionIndex;				// ���콺�� �浹�� ���� �ε���
	int MouseSectionColCount(RayInfo _RayInfo);			// ���콺�� �ش� �����̶� �浹�� ����
	Vector3 MouseNavCol(RayInfo _RayInfo);				// ���콺�� ���� �浹�� ����

	bool NavigationMesh::CollisionRayToOBB(const RayInfo & _tSrc, const OBBInfo & _tDest);


public:
	// ���� ( �극���� �˰������� ���� Ŭ���� ���� ã��  -> �� �׸��常 �� )
	//  1. �浹�� �׺�޽����� �ε��� ���ϱ�  ( ���� vMouseSectionIndex �̰� ó�� �̿� )
	std::vector<int> vMouseNavIndex;
	Vector3 m_vMouseGridColPos;				// ���콺�� �浹�� ���� ã��
	bool CheckMouseNavIndex(RayInfo _RayInfo);	// �극���� �˰������� �ش� �޽��� ã��
	bool MouseNavColGrid(RayInfo _RayInfo);		// �ش� �޽����� �ﰢ���� ���� �浹�� �ش�Ǵ°� ã�Ƽ� Pos �ֱ�

public:
	// ������ ���� �Լ� + CellEnable
	void NaviMeshRender();
	void UpdateNavVertexBuffer(int _iIndex);

	void CellEnableInit();

	void ChangeCellEnable(bool _bEnable, Vector3 _vPos);
	void ChangeCellEnable(bool _bEnable, int _Index);

	std::vector<int> GetVecCellEnableFalse();
};

ENGINE_END