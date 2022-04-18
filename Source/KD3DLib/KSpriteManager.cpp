#include "KSpriteManager.h"

void KSprite::SetAnimArray(RECT rt)
{
	m_anim_array.push_back(rt);
}

void KSprite::RunAnim(int speed, int start, int end)
{
	int betw = end - start;
	m_anim_time += betw * g_fSecPerFrame * speed;
	float temp = m_anim_time + start;
	temp = max(temp, start);
	temp = min(temp, end);
	m_AnimIndex = temp;

	if (m_anim_time > betw+1)
	{
		m_anim_time = 0;
	}
}

bool KSprite::Release()
{
	m_anim_array.clear();
	return true;
}

//하나의 스프라이트만 파일 입출력 
bool KSprite::Load(std::wstring filename)
{
	TCHAR buffer[256] = { 0 };
	TCHAR temp[256] = { 0 };
	//타임
	float fAnimTimer;
	FILE* fp_src;
	_wfopen_s(&fp_src, filename.c_str(), _T("rt"));
	if (fp_src == NULL) return false;

	_fgetts(buffer, _countof(buffer), fp_src);
	_stscanf_s(buffer, _T("%s %d"), temp, (unsigned int)_countof(temp));

	TCHAR name[256] = { 0 };
	int iNumFrame = 0;
	_fgetts(buffer, _countof(buffer), fp_src);
	_stscanf_s(buffer, _T("%s %d %f"), name, (unsigned int)_countof(name),
		&iNumFrame,
		&m_anim_time);
	RECT rt;
	for (int iFrame = 0; iFrame < iNumFrame; iFrame++)
	{
		_fgetts(buffer, _countof(buffer), fp_src);
		_stscanf_s(buffer, _T("%s %d %d %d %d"), temp, (unsigned int)_countof(temp),
			&rt.left, &rt.top, &rt.right, &rt.bottom);
		m_anim_array.push_back(rt);
	}
	m_Name = name;
	fclose(fp_src);
	return true;
}
KSprite* KSpriteManager::Load(std::wstring filename)
{
	//중복 처리
	std::wstring name = Splitpath(filename, L"");
	KSprite* pData = CheckLoad(name);
	//기존에 데이터가 있다면 return
	if (pData != nullptr)
	{
		return pData;
	}
	TCHAR buffer[256] = { 0 };
	TCHAR temp[256] = { 0 };

	int num_sprite = 0;
	float anim_time;
	FILE* fp_src;
	_wfopen_s(&fp_src, filename.c_str(), _T("rt"));
	if (fp_src == NULL) return nullptr;

	_fgetts(buffer, _countof(buffer), fp_src);
	_stscanf_s(buffer, _T("%s%d%d"), temp, (unsigned int)_countof(temp),
		&num_sprite);

	for (int iCnt = 0; iCnt < num_sprite; iCnt++)
	{
		TCHAR name[256] = { 0 };
		KSprite* pSprite = new KSprite;
		int iNumFrame = 0;
		_fgetts(buffer, _countof(buffer), fp_src);
		_stscanf_s(buffer, _T("%s %d %d %f"), name, (unsigned int)_countof(name),
			&iNumFrame,
			&pSprite->m_anim_time);

		RECT rt;
		for (int iFrame = 0; iFrame < iNumFrame; iFrame++)
		{
			_fgetts(buffer, _countof(buffer), fp_src);
			_stscanf_s(buffer, _T("%s %d %d %d %d"), temp, (unsigned int)_countof(temp),
				&rt.left, &rt.top, &rt.right, &rt.bottom);
			pSprite->m_anim_array.push_back(rt);
		}

		pSprite->m_Name = name;
		m_list.insert(make_pair(pSprite->m_Name, pSprite));
		m_iIndex++;
	}
	fclose(fp_src);
	return nullptr;
}

KSprite* KSpriteManager::Load(KSprite* sprite)
{
	//중복 처리
	std::wstring name = Splitpath(sprite->m_Name, L"");
	KSprite* pData = CheckLoad(name);
	//기존에 데이터가 있다면 return
	if (pData != nullptr)
	{
		return pData;
	}
	m_list.insert(make_pair(sprite->m_Name, sprite));
	return sprite;
}

