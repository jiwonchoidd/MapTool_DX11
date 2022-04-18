#include "KSound.h"
//loop 플레이 사운드
void KSound::SoundPlay(bool bloop)
{
	if (m_pChannel != nullptr)
	{
		m_pChannel->isPlaying(&m_bPlay);
	}
	if (m_bPlay == false)
	{
		//채널은 플레이 사운드에서 반환이됨
		//FMOD API 에서 플레이 되는 사운드의 제어를 담당함 
		FMOD_RESULT ret = m_pSystem->playSound(m_pSound,
			nullptr, false, &m_pChannel);
		if (ret == FMOD_OK)
		{
			//m_pChannel->setVolume(0.5f);
			if (bloop)
				m_pChannel->setMode(FMOD_LOOP_NORMAL);
			else
				m_pChannel->setMode(FMOD_LOOP_OFF);
		}
	}
}

//채널을 임시적으로 만들어 한번만 사운드
void KSound::SoundPlay_Once()
{
	FMOD::Channel* pChannel = nullptr;
	// 채널은 플레이 되는 사운드의 제어를 담당.
	FMOD_RESULT	ret = m_pSystem->playSound(
		m_pSound, nullptr, false, &pChannel);
	if (ret == FMOD_OK)
	{
	}
}

//사운드 끄기
void KSound::SoundStop()
{
	if (m_pChannel != nullptr)
	{
		m_pChannel->stop();
	}
}

//사운드 멈춤 실행중일때는 멈추고 멈춘상태에서는 재실행
void KSound::SoundPaused()
{
	m_pChannel->isPlaying(&m_bPlay);
	(m_bPlay) ? m_bPlay = false : m_bPlay = true;
	m_pChannel->setPaused(m_bPlay);
}

//볼륨 관련
void KSound::SoundVolumeDown(float vol)
{
	m_fVolume -= vol;
	SoundVolume(m_fVolume);
}
void KSound::SoundVolumeUp(float vol)
{
	m_fVolume += vol;
	SoundVolume(m_fVolume);
}
void KSound::SoundVolume(float vol)
{
	if (m_pChannel != nullptr)
	{
		m_fVolume = max(0.0f, m_fVolume);
		m_fVolume = min(1.0f, m_fVolume);
		m_pChannel->setVolume(m_fVolume);
	}
}
//------------------------------------
bool KSound::Init()
{
	return true;
}

bool KSound::Frame()
{
	if (m_pSound != nullptr || m_pChannel != nullptr) return true;

	//전체 길이
	m_pSound->getLength(&m_size, FMOD_TIMEUNIT_MS);
	//플레이 위치
	m_pChannel->getPosition(&m_pos, FMOD_TIMEUNIT_MS);
	return true;
}

bool KSound::Render()
{
	return true;
}

bool KSound::Release()
{
	if (m_pSound)
	{
		m_pSound->release();
		m_pSound = nullptr;
	}
	return true;
}


KSound::KSound()
{
	m_pSystem = nullptr;
	m_id = 0;
	m_name = L"";
	m_bPlay = false;
	m_fVolume = 1.0f;
	m_size = 0;
	m_pos = 0;
}

KSound::KSound(FMOD::System* system, int index, std::wstring name)
{

	m_pSystem = system;
	m_id = index;
	m_name = name;
	m_bPlay = false;
	m_fVolume = 1.0f;
	m_size = 0;
	m_pos = 0;
}

KSound::~KSound()
{

}
