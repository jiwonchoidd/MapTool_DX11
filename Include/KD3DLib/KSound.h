#pragma once
#include "KStd.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#pragma comment(lib, "fmod_vc.lib")

//사운드 클래스
class KSound
{
private:
	int				m_id;
	bool			m_bPlay;
	float			m_fVolume;
	unsigned int    m_size;
	unsigned int	m_pos;
public:
	std::wstring	m_name;
	FMOD::System*	m_pSystem = nullptr;
	FMOD::Sound*	m_pSound = nullptr;
	FMOD::Channel*	m_pChannel = nullptr;
public:
	void		SoundPlay(bool bloop);
	void		SoundPlay_Once();
	void		SoundStop();
	void		SoundPaused();
	void		SoundVolumeDown(float vol);
	void		SoundVolumeUp(float vol);
private:
	void		SoundVolume(float vol);
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	KSound();
	KSound(FMOD::System* system, int index, std::wstring name);
	virtual ~KSound();
};

