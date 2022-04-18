#pragma once
#include "KSound.h"
class KSoundManager : public Singleton<KSoundManager>
{
private:
	int			m_index;
public:
	friend class Singleton<KSoundManager>;
public:
	std::map<int, KSound*>		m_SoundList;
	FMOD::System*				m_pSystem = nullptr;
public:
	KSound* LoadSound(std::wstring filename);
	KSound* GetSound(int index);
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	KSoundManager();
public:
	~KSoundManager();
};

#define g_SoundManager KSoundManager::Get()
