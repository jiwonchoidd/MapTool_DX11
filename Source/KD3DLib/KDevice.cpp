#include "KDevice.h"
#include "ImGui\imgui_impl_dx11.h"
ID3D11Device* g_pd3dDevice = nullptr;		// 디바이스 객체
bool	KDevice::SetDevice()
{
	HRESULT hr;
	if (FAILED(CreateGIFactory()))
	{
		return false;
	}
	if (FAILED(CreateDeviceAndSwapChain()))
	{
		return false;
	}
	if (FAILED(SetRenderTargetView()))
	{
		return false;
	}
	if (FAILED(SetDepthStencilView()))
	{
		return false;
	}
	if (FAILED(SetViewPort()))
	{
		return false;
	}
	// imgui d3d impl 초기화
	ImGui_ImplDX11_Init(g_pd3dDevice, m_pImmediateContext.Get());
	return true;
}
HRESULT KDevice::CreateGIFactory()
{
	HRESULT hr = S_OK;
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory),
		(void**)(m_pGIFactory.GetAddressOf()));
	return hr;
}

HRESULT KDevice::CreateDeviceAndSwapChain()
{
	HRESULT hr = S_OK;
	if (m_pGIFactory.Get() == NULL) return S_FALSE;
	//D2DWrite 사용하려면 플래그해줘야함
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
	};
	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_12_0,
	};
	UINT numFeatureLevels = sizeof(featureLevels) / sizeof(featureLevels[0]);
	m_DriverType = driverTypes[0];

	ZeroMemory(&m_SwapChainDesc, sizeof(m_SwapChainDesc));
	m_SwapChainDesc.BufferCount = 1;
	m_SwapChainDesc.BufferDesc.Width = g_rtClient.right;
	m_SwapChainDesc.BufferDesc.Height = g_rtClient.bottom;
	m_SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	m_SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_SwapChainDesc.OutputWindow = g_hWnd;
	//뎁스스텐실 count랑 스왑체인 count랑 맞춰야함,
	m_SwapChainDesc.SampleDesc.Count = 1;
	m_SwapChainDesc.SampleDesc.Quality = 0;
	m_SwapChainDesc.Windowed = true;
	//m_SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	//백버퍼에 가장 적합한 디스플레이 모드로 자동 전환함
	//m_SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	
	hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		m_DriverType,
		NULL,
		createDeviceFlags,
		featureLevels,
		numFeatureLevels,
		D3D11_SDK_VERSION,
		&m_SwapChainDesc,
		m_pSwapChain.GetAddressOf(),
		m_pd3dDevice.GetAddressOf(),
		&m_FeatureLevel,
		m_pImmediateContext.GetAddressOf());

	g_pd3dDevice = m_pd3dDevice.Get();
	return hr;
}

HRESULT KDevice::SetRenderTargetView()
{
	HRESULT hr = S_OK;
	ID3D11Texture2D* pBackBuffer;
	if (FAILED(hr = m_pSwapChain->GetBuffer(
		0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer), hr))
	{
		return hr;
	}
	m_DefaultRT.SetRenderTargetView(pBackBuffer);
	return hr;
}

HRESULT KDevice::SetDepthStencilView()
{
	// 1)텍스처 생성 : 깊이,스텐실 값을 저장하는 버퍼용
	HRESULT hr = S_OK;
	DXGI_SWAP_CHAIN_DESC Desc;
	m_pSwapChain->GetDesc(&Desc);
	hr = m_DefaultRT.m_DepthStencil.CreateDepthStencilView(Desc.BufferDesc.Width,
		Desc.BufferDesc.Height);
	return hr;
}

HRESULT KDevice::SetViewPort()
{
	HRESULT hr = S_OK;
	// Setup the viewport    
	m_ViewPort.Width = m_SwapChainDesc.BufferDesc.Width;
	m_ViewPort.Height = m_SwapChainDesc.BufferDesc.Height;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_pImmediateContext->RSSetViewports(1, &m_ViewPort);
	return hr;
}

bool KDevice::ResizeDevice(UINT width, UINT height)
{
	if (m_pd3dDevice.Get()==nullptr)return false;
	//렌더타겟, 깊이 스텐실 버퍼 해제
	m_pImmediateContext.Get()->OMSetRenderTargets(0, NULL, NULL);
	if (m_DefaultRT.m_pRenderTargetView.Get())m_DefaultRT.m_pRenderTargetView.Get()->Release();
	if (m_DefaultRT.m_DepthStencil.m_pDepthStencilView.Get())m_DefaultRT.m_DepthStencil.m_pDepthStencilView.Get()->Release();

	HRESULT hr = m_pSwapChain.Get()->ResizeBuffers(m_SwapChainDesc.BufferCount,
		width, height,
		m_SwapChainDesc.BufferDesc.Format,
		m_SwapChainDesc.Flags);
	if (SUCCEEDED(hr))
	{
		m_pSwapChain.Get()->GetDesc(&m_SwapChainDesc);
	}
	SetRenderTargetView();
	SetDepthStencilView();
	SetViewPort();
	GetClientRect(g_hWnd, &g_rtClient);
	return true;
}

bool KDevice::CleanupDevice()
{
	//스마트 포인터 사용
	if (m_pImmediateContext.Get()) m_pImmediateContext.Get()->ClearState();
	ImGui_ImplDX11_Shutdown();
	return true;
}

KDevice::KDevice()
{
	m_pd3dDevice = nullptr;
	m_pSwapChain = nullptr;;
	m_pGIFactory = nullptr;
	m_pImmediateContext = nullptr;
}
