#include "KSoundManager.h"
//�ε����� ���� ������ ��ȯ
KSound* KSoundManager::GetSound(int index)
{
	auto iter = m_SoundList.find(index);
	if (iter != m_SoundList.end())
	{
		//���� �����Ͱ� �־�߸� ������
		if ((*iter).second->m_pSound != nullptr)
		{
			return (*iter).second;
		}
	}
	return nullptr;
}
//���� �޸� �����ϰ� ���� ����, ����Ʈ �߰�
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

	//�ߺ� ���� ó��
	for (auto data : m_SoundList)
	{
		if (data.second->m_name == FileName)
		{
			return data.second;
		}
	}

	//���� ���� �޸� �ε� ������
	KSound* sound = new KSound(m_pSystem, m_index, FileName);

	//createSound ���� ����
	std::string mutiname = to_wm(filename);
	ret = m_pSystem->createSound(mutiname.c_str(),
		FMOD_DEFAULT, 0, &sound->m_pSound);
	if (ret != FMOD_OK)
	{
		sound->Release();
		return nullptr;
	}
	//createSound ���� ���� �Ϸ� �� ����Ʈ�� �߰�
	m_SoundList.insert(std::make_pair(m_index++, sound));
	return sound;
}

bool KSoundManager::Init()
{
	FMOD_RESULT ret;
	//FMOD �ý��� ����̽� ����
	ret = FMOD::System_Create(&m_pSystem);
	if (ret != FMOD_OK)
	{
		return false;
	}
	//FMOD �ý��� �ʱ�ȭ ä�� 32ä���� ���� ���� �ý��� �ʱ�ȭ
	//������ǵ� �ְ� ����Ʈ�� ����, �ߺ������� ������ �Ǵ°� 
	//�� ī���Ͱ� 32���� ä���̴�. 
	ret = m_pSystem->init(32, FMOD_INIT_NORMAL, 0);
	if (ret != FMOD_OK)
	{
		return false;
	}
	return true;
}

bool KSoundManager::Frame()
{
	//�����ӿ��� �ݵ�� ������Ʈ�� ȣ����Ѿ���
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
