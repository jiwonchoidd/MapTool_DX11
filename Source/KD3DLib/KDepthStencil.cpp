#include "KDepthStencil.h"

wrl::ComPtr<ID3D11Texture2D> KDepthStencil::CreateTexture(UINT Width, UINT Height)
{
	HRESULT hr = S_OK;
	// 1)텍스처 생성 : 깊이,스텐실 값을 저장하는 버퍼용
	wrl::ComPtr<ID3D11Texture2D> pDSTexture = nullptr;
	D3D11_TEXTURE2D_DESC DescDepth;
	DescDepth.Width = Width;
	DescDepth.Height = Height;
	DescDepth.MipLevels = 1;
	DescDepth.ArraySize = 1;
	//RGB는 텍스쳐 리소스 D24는 뎁스
	DescDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DescDepth.SampleDesc.Count = 1;
	DescDepth.SampleDesc.Quality = 0;
	DescDepth.Usage = D3D11_USAGE_DEFAULT;

	// 백 버퍼 깊이 및 스텐실 버퍼 생성
	if (DescDepth.Format == DXGI_FORMAT_D24_UNORM_S8_UINT)
		DescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	else // 깊이맵 전용 깊이맵 생성
		DescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	DescDepth.CPUAccessFlags = 0;
	DescDepth.MiscFlags = 0;
	if (FAILED(hr = g_pd3dDevice->CreateTexture2D(&DescDepth, NULL, pDSTexture.GetAddressOf())))
	{
		return nullptr;
	}
	return pDSTexture;
}

HRESULT KDepthStencil::CreateDepthStencilView(UINT Width, UINT Height)
{
	HRESULT hr = S_OK;
	// 1)텍스처 생성 : 깊이,스텐실 값을 저장하는 버퍼용
	m_pTexture = CreateTexture(Width, Height);
	if (m_pTexture.Get() == nullptr)
	{
		return E_FAIL;
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC svd;
	ZeroMemory(&svd, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	svd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	svd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	svd.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView(m_pTexture.Get(), &svd,
		m_pDepthStencilView.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
	D3D11_SHADER_RESOURCE_VIEW_DESC Desc;
	ZeroMemory(&Desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	Desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	Desc.Texture2D.MipLevels = 1;
	hr = g_pd3dDevice->CreateShaderResourceView(m_pTexture.Get(),
		&Desc, m_pTextureSRV.GetAddressOf());
	if (FAILED(hr))
	{
		m_pTexture.Get()->Release();
		return hr;
	}
	return hr;
}

bool KDepthStencil::Release()
{
	//m_pTexture.Get()->Release();
	//m_pDepthStencilView.Get()->Release();
    return true;
}

KDepthStencil::KDepthStencil()
{
}

KDepthStencil::~KDepthStencil()
{
}
