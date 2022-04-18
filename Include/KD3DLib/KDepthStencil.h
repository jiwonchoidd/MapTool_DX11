#pragma once
#include "KStd.h"
class KDepthStencil
{
public:
	wrl::ComPtr <ID3D11Texture2D> m_pTexture;
	wrl::ComPtr <ID3D11ShaderResourceView> m_pTextureSRV;
	wrl::ComPtr <ID3D11DepthStencilView> m_pDepthStencilView;
public:
	wrl::ComPtr<ID3D11Texture2D> CreateTexture(UINT Width, UINT Height);
	HRESULT CreateDepthStencilView(UINT Width, UINT Height);
	bool	Release();
public:
	KDepthStencil();
	virtual ~KDepthStencil();
};
