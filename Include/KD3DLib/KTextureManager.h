#pragma once
#include "KManager.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
#include "ScreenGrab.h"
#include <wincodec.h>
#include <wincodecsdk.h>
#pragma comment	(lib, "d3dcompiler.lib")
class KTexture
{
public:
	int				m_iIndex;
	std::wstring	m_Name;
	std::wstring	m_szMsg;
	TCHAR			m_szBuffer[256];
	wrl::ComPtr <ID3D11Resource>			m_pResourceTexture;
	wrl::ComPtr <ID3D11ShaderResourceView>	m_pSRVTexture;
	D3D11_TEXTURE2D_DESC					m_TextureDesc;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	bool	Load(std::wstring filename);
public:
	KTexture();
	virtual ~KTexture();
};

class KTextureManager : public KManager<KTexture, KTextureManager>
{
	friend class Singleton<KTextureManager>;
private:
	KTextureManager() {};
public:
	void SaveFile(ID3D11DeviceContext* pContext, std::wstring name, ID3D11Texture2D* pRes);
public:
	~KTextureManager() {};
};
#define g_TextureMananger KTextureManager::Get()
