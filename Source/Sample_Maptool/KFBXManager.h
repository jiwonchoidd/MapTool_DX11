#pragma once
#include "KManager.h"
#include "KFbxLoader.h"
class KFBXManager : public KManager<KFbxLoader, KFBXManager>
{
	friend class Singleton<KFBXManager>;
private:
	KFBXManager();
public:
	virtual ~KFBXManager();
};
#define g_FBXManager KFBXManager::Get()

