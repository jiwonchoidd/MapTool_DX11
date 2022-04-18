#include "KSound.h"
//loop �÷��� ����
void KSound::SoundPlay(bool bloop)
{
	if (m_pChannel != nullptr)
	{
		m_pChannel->isPlaying(&m_bPlay);
	}
	if (m_bPlay == false)
	{
		//ä���� �÷��� ���忡�� ��ȯ�̵�
		//FMOD API ���� �÷��� �Ǵ� ������ ��� ����� 
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

//ä���� �ӽ������� ����� �ѹ��� ����
void KSound::SoundPlay_Once()
{
	FMOD::Channel* pChannel = nullptr;
	// ä���� �÷��� �Ǵ� ������ ��� ���.
	FMOD_RESULT	ret = m_pSystem->playSound(
		m_pSound, nullptr, false, &pChannel);
	if (ret == FMOD_OK)
	{
	}
}

//���� ����
void KSound::SoundStop()
{
	if (m_pChannel != nullptr)
	{
		m_pChannel->stop();
	}
}

//���� ���� �������϶��� ���߰� ������¿����� �����
void KSound::SoundPaused()
{
	m_pChannel->isPlaying(&m_bPlay);
	(m_bPlay) ? m_bPlay = false : m_bPlay = true;
	m_pChannel->setPaused(m_bPlay);
}

//���� ����
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

	//��ü ����
	m_pSound->getLength(&m_size, FMOD_TIMEUNIT_MS);
	//�÷��� ��ġ
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
