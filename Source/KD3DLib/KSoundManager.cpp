#include "KSoundManager.h"
//인덱스로 사운드 포인터 반환
KSound* KSoundManager::GetSound(int index)
{
	auto iter = m_SoundList.find(index);
	if (iter != m_SoundList.end())
	{
		//사운드 포인터가 있어야만 리턴함
		if ((*iter).second->m_pSound != nullptr)
		{
			return (*iter).second;
		}
	}
	return nullptr;
}
//사운드 메모리 적재하고 사운드 생성, 리스트 추가
KSound* KSoundManager::LoadSound(std::wstring filename)
{
	FMOD_RESULT ret;

	TCHAR szFileName[MAX_PATH] = { 0, };
	TCHAR Drive[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR FileName[MAX_PATH] = { 0, };
	TCHAR FileExt[MAX_PATH] = { 0, };
	std::wstring fullpathname = filename;
	_tsplitpath_s(fullpathname.c_str(), Drive, Dir, FileName, FileExt);

	//중복 방지 처리
	for (auto data : m_SoundList)
	{
		if (data.second->m_name == FileName)
		{
			return data.second;
		}
	}

	//동적 사운드 메모리 로드 생성자
	KSound* sound = new KSound(m_pSystem, m_index, FileName);

	//createSound 사운드 생성
	std::string mutiname = to_wm(filename);
	ret = m_pSystem->createSound(mutiname.c_str(),
		FMOD_DEFAULT, 0, &sound->m_pSound);
	if (ret != FMOD_OK)
	{
		sound->Release();
		return nullptr;
	}
	//createSound 사운드 생성 완료 후 리스트에 추가
	m_SoundList.insert(std::make_pair(m_index++, sound));
	return sound;
}

bool KSoundManager::Init()
{
	FMOD_RESULT ret;
	//FMOD 시스템 디바이스 생성
	ret = FMOD::System_Create(&m_pSystem);
	if (ret != FMOD_OK)
	{
		return false;
	}
	//FMOD 시스템 초기화 채널 32채널을 갖고 사운드 시스템 초기화
	//배경음악도 있고 이펙트도 있음, 중복적으로 실행이 되는것 
	//그 카운터가 32개의 채널이다. 
	ret = m_pSystem->init(32, FMOD_INIT_NORMAL, 0);
	if (ret != FMOD_OK)
	{
		return false;
	}
	return true;
}

bool KSoundManager::Frame()
{
	//프레임에서 반드시 업데이트를 호출시켜야함
	m_pSystem->update();
	return true;
}

bool KSoundManager::Render()
{
	return true;
}

bool KSoundManager::Release()
{
	for (auto data : m_SoundList)
	{
		data.second->Release();
		delete data.second;
	}
	m_SoundList.clear();

	m_pSystem->close();
	m_pSystem->release();
	return true;
}

KSoundManager::KSoundManager()
{
	m_index = 0;
	m_SoundList.clear();
	m_pSystem = nullptr;
}

KSoundManager::~KSoundManager()
{

}
