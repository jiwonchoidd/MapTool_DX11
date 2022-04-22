#include "KFBXManager.h"
bool KFBXManager::LoadTextureData(std::wstring filename)
{
	FILE* fp = nullptr;
	_tfopen_s(&fp, filename.c_str(), _T("rt"));
	if (fp == NULL)
	{
		//파일이 없음
		return false;
	}
	TCHAR buffer[256] = { 0, };

	//실패인 경우 0 리턴 EOF은 -1
	//한줄을 읽어옴
	while (_fgetts(buffer, _countof(buffer), fp) != 0)
	{
		TCHAR fbxName[256] = { 0, };
		_stscanf_s(buffer, _T("%s"), fbxName, (unsigned int)_countof(fbxName));

		TCHAR fbxmodel[256] = { 0, };
		TCHAR texname_diffuse[256] = { 0, };
		TCHAR texname_specualr[256] = { 0, };
		TCHAR texname_normal[256] = { 0, };
		TCHAR texname_alpha[256] = { 0, };
		_stscanf_s(buffer, _T("%s %s %s %s %s"), fbxmodel, (unsigned int)_countof(fbxmodel),
			texname_diffuse, (unsigned int)_countof(texname_diffuse),
			texname_specualr, (unsigned int)_countof(texname_specualr), 
			texname_normal, (unsigned int)_countof(texname_normal),
			texname_alpha, (unsigned int)_countof(texname_alpha));

		//알베도(디퓨즈), 스페큘러맵, 노말맵, 알파맵 까지 추가
		//보통 알파맵은 없음 공백상태임, 현재 나무잎만 알파맵이 들어감
		std::vector<std::wstring> pack;
		pack.push_back(texname_diffuse);
		pack.push_back(texname_specualr);
		pack.push_back(texname_normal);
		pack.push_back(texname_alpha);
		pack.resize(4);
		m_TexDataList.insert(std::make_pair(fbxmodel, pack));
		
	}
	fclose(fp);
}
bool KFBXManager::GetFBXTexList(std::wstring fbxName, std::wstring& diffuse, std::wstring& specular, std::wstring& normal, std::wstring& alpha)
{
	if (!m_TexDataList.count(fbxName)) return false;
	std::map<std::wstring, std::vector<std::wstring>> ::iterator iter;

	//맵에서 해당 텍스쳐 빼옴
	iter = m_TexDataList.find(fbxName);
	
	diffuse = iter->second.at(0);
	specular = iter->second.at(1);
	normal = iter->second.at(2);
	alpha = iter->second.at(3);
	return true;
}
KFBXManager::KFBXManager()
{
}
KFBXManager::~KFBXManager()
{
}
