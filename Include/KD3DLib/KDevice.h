#pragma once
#include "KDepthStencil.h"
#include "KRenderTarget.h"
class KDevice
{
public:
	wrl::ComPtr <ID3D11Device>				m_pd3dDevice;		// 디바이스 객체
	wrl::ComPtr <ID3D11DeviceContext>		m_pImmediateContext;// 디바이스 컨텍스트
	wrl::ComPtr <IDXGISwapChain>			m_pSwapChain;		// 스왑체인 객체
	wrl::ComPtr <IDXGIFactory>				m_pGIFactory;		// DXGI 객체
public:
	KRenderTarget			m_DefaultRT; // DefaultDS 뎁스스텐실 같이 한 쌍으로 둠
public:
	DXGI_SWAP_CHAIN_DESC	m_SwapChainDesc;	// 스왑체인 속성값
	D3D_DRIVER_TYPE         m_driverType;		// 디바이스 타입( 디폴트:하드웨어 가속 )
	D3D11_VIEWPORT			m_ViewPort;			// 뷰포트 
	D3D_DRIVER_TYPE			m_DriverType;		// 디바이스 타입
	D3D_FEATURE_LEVEL       m_FeatureLevel;		// DirectX의 기능수준
public:
	bool	SetDevice();
	//--------------------------------------------------------------------------------------
	// 디바이스 및 스왑체인 생성 : 
	// 1, CreateDevice()
	// 2, CreateGIFactory()
	// 3, CreateSwapChain()
	// 4, SetRenderTargetView()
	// 5, SetViewPort()
	//--------------------------------------------------------------------------------------
	HRESULT		CreateDeviceAndSwapChain();
	HRESULT		CreateGIFactory();
	HRESULT		SetRenderTargetView();
	HRESULT		SetDepthStencilView();
	HRESULT		SetViewPort();
	bool		CleanupDevice();
public:
	bool		ResizeDevice(UINT width, UINT height);
	KDevice();
};

