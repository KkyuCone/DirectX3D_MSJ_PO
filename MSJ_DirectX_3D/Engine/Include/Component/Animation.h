#pragma once
#include "Component.h"

ENGINE_BEGIN

// ��
typedef struct ENGINE_DLL _tagBone
{
	std::string		strName;			// �� �̸�
	int				iDepth;				// �� ����(����, Ʈ��������)
	int				iParentIndex;		// �θ� �ε���
	PMatrix			matOffset;			// ������ ���
	PMatrix			matBone;			// ���� ��� (��ȯ ���� 3D�ƽ����� ���� �����_
	std::list<class CBoneSocket*>	SocketList;	// ���Ŀ� ���ⰰ���� ��� ���� ��� �ʿ�
	int				iRefCount;			// ���� ���� ��ü�� ���� ��� ���� �ϳ��� �����Ѵ�. ��� �� ��ü���� ��� �־���ϹǷ� ī���͸� �����ش�.

	_tagBone() :
		iRefCount(1)
	{
	}
}BONE, *PBONE;

// Ű������
typedef struct ENGINE_DLL _tagKeyFrame
{
	double	dTime;				// �� Ű�����ӿ� �ش��ϴ� �ð���
	Vector3	vPos;
	Vector3	vScale;
	Vector4	vRot;
}KEYFRAME, *PKEYFRAME;

// �ִϸ��̼� Ű������
typedef struct ENGINE_DLL _tagBoneKeyFrame
{
	int		iBoneIndex;
	std::vector<PKEYFRAME>	vecKeyFrame;
	int			iRefCount;

	_tagBoneKeyFrame() :
		iRefCount(1)
	{
	}

	~_tagBoneKeyFrame()
	{
		Safe_Delete_VectorList(vecKeyFrame);
	}
}BONEKEYFRAME, *PBONEKEYFRAME;

typedef struct ENGINE_DLL _tagAnimationCallback
{
	int		iAnimationProgress;
	float	fAnimationProgress;
	std::function<void(float)> func;
	bool	bCall;
}ANIMATIONCALLBACK, *PANIMATIONCALLBACK;

typedef struct ENGINE_DLL _tagAnimationClip
{
	ANIMATION_OPTION	eOption;
	std::string				strName;
	float				fStartTime;
	float				fEndTime;
	float				fTimeLength;			// �ִϸ��̼� ����(�ϳ��� ��� Idle..Run����..) �ð� ���� ����
	float				fFrameTime;				// ���ʸ��� �ִϸ��̼� �������� ���ŵǾ�� �ϴ����� ���� �ð���
	float				fPlayTime;
	int					iStartFrame;
	int					iEndFrame;
	int					iFrameLength;
	int					iFrameMode;				// ���� 30������
	int					iChangeFrame;			// �ٸ� �ִϸ��̼����� �ٲٱ� �ٷ� ������ ���� �ִϸ��̼� ������
	std::vector<PBONEKEYFRAME>		vecKeyFrame;
	std::vector<PANIMATIONCALLBACK>	vecCallback;

	_tagAnimationClip() :
		eOption(AO_LOOP),
		strName(""),
		iFrameMode(0),
		fStartTime(0),
		fEndTime(0),
		fTimeLength(0),
		iStartFrame(0),
		iEndFrame(0),
		iFrameLength(0),
		fPlayTime(1.0f)
	{
	}

	~_tagAnimationClip()
	{
		for (size_t i = 0; i < vecKeyFrame.size(); ++i)
		{
			--(vecKeyFrame[i]->iRefCount);

			if (vecKeyFrame[i]->iRefCount == 0)
			{
				SAFE_DELETE(vecKeyFrame[i]);
			}
		}

		vecKeyFrame.clear();
		//Safe_Delete_VecList(vecCallback);
	}
}ANIMATIONCLIP, *PANIMATIONCLIP;

typedef struct ENGINE_DLL _tagBlendInfo
{
	Vector3	vScale;
	Vector3	vPos;
	Vector4	vRot;
}BlendInfo, *PBlendInfo;

//
class ENGINE_DLL Animation : public Component
{
private:
	friend class GameObject;
	friend class Mesh;

protected:
	Animation();
	Animation(const Animation& _Com);
	virtual ~Animation();

private:
	// ���̴��� ��� �������ʹ� ���� �Ѱ谡 �ִ�.
	// ��, �ʹ� ���� ������ ���̴��� ���� ���� ����.
	// �̸� �����ϱ� ���� 2���� ����� �ִµ�
	// 1. ���������� �ݾ� �ɰ��� �ִ� ���
	// 2. VTF(Vertex Texture Fatch)
	// �츮�� ���߿��� VTF�� ����Ұǵ�, ���������� �ؽ�ó�� �ְ�
	// ���ؽ����̴����� �ȼ� ������ �����ͼ� �ִϸ��̼� �� �������� ó���� ���̴�.
	// �̴� ���̴� 3.0���� �����ϸ�, �ؽ�ó�� ����ó�� ����� ���̱� ������
	// Texture2D�� ShaderResourceView�� �ʿ��ϴ�. (���̴��� �Ѱ��ٷ���)
	std::vector<PBONE>				m_vecBones;					// ���� ����
	ID3D11Texture2D*				m_pBoneTexture;				// ����ĵ� ������ ���� �ִ� �ؽ�ó, �̰� ���̴��� �Ѱ��༭ Skinning�Ҷ� ����� ����	
	ID3D11ShaderResourceView*		m_pBoneResourceView;		// ���� �ؽ�ó�� ���̴��� �ѱ� Shader Resource View
	std::vector<PMatrix>			m_vecBoneMatrix;			// ����ĵ� ����
	std::unordered_map<std::string, PANIMATIONCLIP>	m_mapClip;	// �ִϸ��̼� Ŭ���� ��Ƶ�. �ؽ�ó�� �ٷλ��� ����Ұű⶧���� unordered_map���

	PANIMATIONCLIP			m_pDefaultClip;						// Default �ִϸ��̼� ���� ( �ٸ��ִϸ��̼��� ���� �Ŀ� Default �ִϸ��̼����� ���ƿ� ����. (Idle����..))
	PANIMATIONCLIP			m_pCurClip;							// ���� �ִϸ��̼�
	PANIMATIONCLIP			m_pNextClip;						// ���� �ִϸ��̼�

	std::list<std::string>	m_AddClipNameList;				// �ִϸ��̼� Ŭ�� �̸� ��� ����

	bool					m_bEnd;							// �ִϸ��̼��� �������� ����
	float					m_fAnimationGlobalTime;			// �ִϸ��̼� �����ð�
	float					m_fClipProgress;				// ���� �ִϸ��̼� ����� (������� �ִϸ��̼��� ����Ǿ�����)
	float					m_fChangeTime;					// �ٸ��ִϸ��̼����� �ִϸ��̼��� �ٲܶ��� �ð� ( �ð���� ���� )
	float					m_fChangeLimitTime;				// �ٸ��ִϸ��̼����� �ٲܶ��� ���ѽð� A�ִϸ��̼� -> B�ִϸ��̼����� ����ɶ� ���ʸ��� �����ؾ��ϴ���..

	// ��� ����
	std::vector<BlendInfo>		m_vecBlendInfo;

public:
	// Get �Լ�
	const std::list<std::string>*	GetAddClipNameList()		const;		// �ִϸ��̼� Ŭ�� �̸����� ��Ƶ� ����Ʈ Get�Լ�
	const std::unordered_map<std::string, PANIMATIONCLIP>* GetClips()	const;
	ID3D11ShaderResourceView** GetBoneTexture();
	Matrix GetBoneMatrix(const std::string& strBoneName);
	PANIMATIONCLIP GetCurrentClip()	const;					// ���� �ִϸ��̼� Ŭ�� ��������

public:
	// Find �Լ�
	PBONE			FindBone(const std::string& strBoneName);
	PBONE			FindBone(int _Index);
	int				FindBoneIndex(const std::string& strBoneName);
	PANIMATIONCLIP	FindClip(const std::string& strName);

	Matrix			FindIndexBoneMatrix(int _Index);

public:
	void AddBone(PBONE pBone);
	bool CreateBoneTexture();

	void AddClip(ANIMATION_OPTION eOption,
		struct _tagFbxAnimationClip* pClip);
	void AddClip(const std::string& _strName, ANIMATION_OPTION _eOption,
		int _iStartFrame, int _iEndFrame, float _fPlayTime,
		const std::vector<PBONEKEYFRAME>& _vecFrame);
	void AddClip(const TCHAR* _pFullPath);

	void AddClipFromMultibyte(const char* _pFullPath);

	void GetCurrentKeyFrame(std::vector<PBONEKEYFRAME>& _vecKeyFrame);

	bool IsAnimationEnd()	const;

public:
	void ChangeClipKey(const std::string& strOrigin, const std::string& strChange);
	bool ChangeClip(const std::string& strClip);

public:
	bool ModifyClip(const std::string& strKey, const std::string& strChangeKey,
		ANIMATION_OPTION eOption, int iStartFrame, int iEndFrame, float _fPlayTime,
		const std::vector<PBONEKEYFRAME>& _vecFrame);
	bool DeleteClip(const std::string& strKey);
	bool ReturnDefaultClip();

public:
	bool Save(const TCHAR* pFileName, const std::string& strPathKey = PATH_MESH);
	bool Save(const char* pFileName, const std::string& strPathKey = PATH_MESH);
	bool SaveFromFullPath(const TCHAR* pFullPath);
	bool SaveFromFullPath(const char* pFullPath);
	bool Load(const TCHAR* pFileName, const std::string& strPathKey = PATH_MESH);
	bool Load(const char* pFileName, const std::string& strPathKey = PATH_MESH);
	bool LoadFromFullPath(const TCHAR* pFullPath);
	bool LoadFromFullPath(const char* pFullPath);
	bool SaveBone(const TCHAR* pFileName, const std::string& strPathKey = PATH_MESH);
	bool SaveBone(const char* pFileName, const std::string& strPathKey = PATH_MESH);
	bool SaveBoneFromFullPath(const TCHAR* pFullPath);
	bool SaveBoneFromFullPath(const char* pFullPath);
	bool LoadBone(const TCHAR* pFileName, const std::string& strPathKey = PATH_MESH);
	bool LoadBone(const char* pFileName, const std::string& strPathKey = PATH_MESH);
	bool LoadBoneFromFullPath(const TCHAR* pFullPath);
	bool LoadBoneFromFullPath(const char* pFullPath);

private:
	void LoadFBXAnimation(const char* _pFullPath);

public:
	virtual void Start();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual int Collision(float fTime);
	virtual int PrevRender(float fTime);
	virtual int PrevShadowRender(Matrix _matLightView, Matrix _matLightProj, float _fTime);
	virtual int Render(float fTime);
	virtual Animation* Clone()	const;

	private:
		// �θ� ����
		bool m_bParentBone;
		int  m_iParentBoneNum;
		std::string m_strParentBone;

		// �Ǻ� ��ġ���涧���� �ʿ��� �� 
		bool m_bChangePivot;
		int m_iChangePivotBoneNum;
		std::string m_strChangePivotBone;

public:
	int GetParentBoneIndex();
	void SetParentBoneName(std::string _ParentBone);
	void DeleteParentBone();
	bool ParentBoneEnable();

	void SetChangePivotBone(std::string _PivotBone);
	void MovePivotBone();
	void DeleteChangePivotBone();

};

ENGINE_END