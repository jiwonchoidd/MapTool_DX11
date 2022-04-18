#pragma once
#include "KDepthStencil.h"
class KRenderTarget
{
public:
	KDepthStencil			m_DepthStencil;
	// Store the old render targets
	D3D11_VIEWPORT			m_vpOld[D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX];
	UINT					m_nViewPorts;
	ID3D11RenderTargetView* m_pOldRTV;
	ID3D11DepthStencilView* m_pOldDSV;
	D3D11_VIEWPORT			m_ViewPort;
	KMatrix					m_matProj;
	HRESULT		SetViewPort(UINT Width, UINT Height);
	KMatrix  	CreateProjMatrix(
		float fNear, float fFar, float fFov, float fAspect);
public:
	wrl::ComPtr <ID3D11Texture2D>			m_pTexture;
	wrl::ComPtr <ID3D11ShaderResourceView>  m_pTextureSRV;
	wrl::ComPtr <ID3D11RenderTargetView>	m_pRenderTargetView;// 메인 랜더타켓 뷰
public:
	bool	Create(UINT Width, UINT Height);
	bool    Begin(ID3D11DeviceContext* pContext, float color[4]);
	bool    End(ID3D11DeviceContext* pContext);
	bool	Release();
	void	Save(ID3D11DeviceContext* pContext, std::wstring saveFileName);
	wrl::ComPtr <ID3D11Texture2D> CreateTexture(UINT Width, UINT Height);
	HRESULT				SetRenderTargetView(ID3D11Texture2D* pTexture);
	HRESULT CreateRenderTargetView(UINT Width, UINT Height);
};

