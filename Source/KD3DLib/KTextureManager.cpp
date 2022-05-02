#include "KTextureManager.h"

bool KTexture::Init()
{
    return true;
}

bool KTexture::Frame()
{
    return true;
}

bool KTexture::Render()
{
    return true;
}

bool KTexture::Release()
{
    return true;
}

bool KTexture::Load(std::wstring filename)
{
    HRESULT hr = DirectX::CreateWICTextureFromFile(
        g_pd3dDevice,
        filename.c_str(),
        m_pResourceTexture.GetAddressOf(),
        m_pSRVTexture.GetAddressOf());
    if (FAILED(hr))
    {
        hr = DirectX::CreateDDSTextureFromFile(
            g_pd3dDevice,
            filename.c_str(),
            m_pResourceTexture.GetAddressOf(),
            m_pSRVTexture.GetAddressOf());
    }
    if (FAILED(hr))
    {
        return false;
    }
    static_cast<ID3D11Texture2D*>(m_pResourceTexture.Get())->GetDesc(&m_TextureDesc);
    return true;
}

void KTextureManager::SaveFile(ID3D11DeviceContext* pContext, std::wstring name, ID3D11Texture2D* pRes)
{
	HRESULT hr = S_OK;
	wrl::ComPtr<ID3D11Texture2D> tex;
	if (pRes == nullptr)
	{
		return;
	}
	else
	{
		tex = pRes;
	}
	if (SUCCEEDED(hr))
	{
		hr = DirectX::SaveWICTextureToFile(pContext, tex.Get(), GUID_ContainerFormatBmp, (name + L".bmp").c_str());
	}
	if (pRes == nullptr)
	{
		tex->Release();
	}
}

KTexture::KTexture()
{
    m_pResourceTexture.Reset();
    m_pSRVTexture.Reset();
    m_pResourceTexture = nullptr;
    m_pSRVTexture = nullptr;
}

KTexture::~KTexture()
{
    m_pResourceTexture.Reset();
    m_pSRVTexture.Reset();
    m_pResourceTexture = nullptr;
    m_pSRVTexture = nullptr;
}

