#pragma once
#include "KManager.h"
#include "KFbxLoader.h"
class KFBXManager : public KManager<KFbxLoader, KFBXManager>
{
	friend class Singleton<KFBXManager>;
public:
	std::map<std::wstring, std::vector<std::wstring>> m_TexDataList;
public:
	bool	LoadTextureData(std::wstring filename);
	bool	GetFBXTexList(std::wstring fbxName, std::wstring &diffuse, std::wstring& specular, std::wstring& normal, std::wstring& alpha);
private:
	KFBXManager();
public:
	virtual ~KFBXManager();
};
#define g_FBXManager KFBXManager::Get()

