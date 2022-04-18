#pragma once
#include "K3DAsset.h"
class KPlaneObj : public K3DAsset
{
public:
	virtual bool Init(std::wstring vs , std::wstring ps , std::wstring tex =L"");
	virtual bool Frame();
	virtual bool Render(ID3D11DeviceContext* pContext);
	virtual bool Release();
public:
	KPlaneObj();
	virtual ~KPlaneObj();
};

