#pragma once
#include "KDepthStencil.h"
#include "KRenderTarget.h"
class KDevice
{
public:
	wrl::ComPtr <ID3D11Device>				m_pd3dDevice;		// ����̽� ��ü
	wrl::ComPtr <ID3D11DeviceContext>		m_pImmediateContext;// ����̽� ���ؽ�Ʈ
	wrl::ComPtr <IDXGISwapChain>			m_pSwapChain;		// ����ü�� ��ü
	wrl::ComPtr <IDXGIFactory>				m_pGIFactory;		// DXGI ��ü
public:
	KRenderTarget			m_DefaultRT; // DefaultDS �������ٽ� ���� �� ������ ��
public:
	DXGI_SWAP_CHAIN_DESC	m_SwapChainDesc;	// ����ü�� �Ӽ���
	D3D_DRIVER_TYPE         m_driverType;		// ����̽� Ÿ��( ����Ʈ:�ϵ���� ���� )
	D3D11_VIEWPORT			m_ViewPort;			// ����Ʈ 
	D3D_DRIVER_TYPE			m_DriverType;		// ����̽� Ÿ��
	D3D_FEATURE_LEVEL       m_FeatureLevel;		// DirectX�� ��ɼ���
public:
	bool	SetDevice();
	//--------------------------------------------------------------------------------------
	// ����̽� �� ����ü�� ���� : 
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

