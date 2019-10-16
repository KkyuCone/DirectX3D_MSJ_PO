#pragma once
#include "Component.h"
#include "..\SoundManager.h"

ENGINE_BEGIN

class ENGINE_DLL AudioSound : public Component
{
private:
	friend class GameObject;

public:
	AudioSound();
	AudioSound(const AudioSound& _Com);
	virtual ~AudioSound();

private:
	std::string					m_strFileName;
	struct _tagSoundInfo*		m_pSoundInfo;
	Channel*		m_pChannel;
	bool			m_bBGM;
	bool			m_bLoop;
	bool			m_bPlay;
	float			m_fVolume;

public:
	void SetSound(const std::string& _strName);
	void SetSound(const std::string& _strName, class Scene* _pScene,
		bool _bLoop, const char* _pFileName,
		const std::string& _strPathName = PATH_SOUND);

	void EnableBGM(bool _Enable);
	void SetVolume(float _Volume);		// 0.0 ~1.0f

	void Play();
	void Stop();
	void Pause();

public:
	virtual void Start();				// ��ġ�ɷ��� ���̾ ���Եɶ� (��ġ�ɶ�) ȣ��, �� ���̾�� ȣ�����ش�.
	virtual bool Init();
	virtual int Input(float _fTime);
	virtual int Update(float _fTime);
	virtual int LateUpdate(float _fTime);
	virtual int Collision(float _fTime);
	virtual int PrevRender(float _fTime);	// ������ �ϱ� ������ ȣ�����ִ� �Լ��Ĥ���
	virtual int Render(float _fTime);
	virtual AudioSound* Clone()	const;

};

ENGINE_END