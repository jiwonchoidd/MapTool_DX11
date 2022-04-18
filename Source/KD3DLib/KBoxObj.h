#pragma once
#include "K3DAsset.h"
#include "KObjectManager.h"
class KBoxObj : public K3DAsset
{
public:
	bool Init(std::wstring vsfile = L"", std::wstring psfile = L"", std::wstring tex1=L"", std::wstring tex2 = L"", std::wstring tex3 = L"");
	bool Frame();
	bool CreateVertexData()override;
	bool CreateIndexData()override;
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();
public:
	KBoxObj();
	~KBoxObj();
};

