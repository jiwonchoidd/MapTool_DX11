#include "KFBXManager.h"
bool KFBXManager::LoadTextureData(std::wstring filename)
{
	FILE* fp = nullptr;
	_tfopen_s(&fp, filename.c_str(), _T("rt"));
	if (fp == NULL)
	{
		//������ ����
		return false;
	}
	TCHAR buffer[256] = { 0, };

	//������ ��� 0 ���� EOF�� -1
	//������ �о��
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

		//�˺���(��ǻ��), ����ŧ����, �븻��, ���ĸ� ���� �߰�
		//���� ���ĸ��� ���� ���������, ���� �����ٸ� ���ĸ��� ��
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

	//�ʿ��� �ش� �ؽ��� ����
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
