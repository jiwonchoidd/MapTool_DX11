#pragma once
#include "KRenderTarget.h"
#include "KShaderManager.h"
#include "KLight.h"
struct cbDataShadow
{
	KMatrix m_matShadow; // Lw * Lv * Lp	
};
class KDepthShadow
{
public:
	KLight*		  m_pLight=nullptr;
	cbDataShadow  m_Shadow_cbData; // 라이트 뷰행렬 라이트투영행렬 텍스쳐 행렬 곱한, 쉐도우 행렬
	KMatrix		  m_matTexture;
	KRenderTarget m_ShadowRT;
	KShader*      m_pPSShadow = nullptr;
	KShader*	  m_pPSShadowFBX = nullptr;
	wrl::ComPtr<ID3D11Buffer> m_pShadowCB = nullptr;
public:
	bool CreateShadow(KLight* plight)
	{
		//텍스쳐 변환행렬
		m_pLight = plight;
		m_matTexture._11 = 0.5f; m_matTexture._22 = -0.5f;
		m_matTexture._41 = 0.5f; m_matTexture._42 = 0.5f;
		m_ShadowRT.Create(1024, 1024);

		m_pPSShadowFBX = g_ShaderManager.CreatePixelShader(L"../../data/shader/VSPS_FBX.hlsl", "PSDepth");//맵
		m_pPSShadow = g_ShaderManager.CreatePixelShader(L"../../data/shader/VSPS_Default.hlsl", "PSDepth");//오브젝트
		if (!CreateConstantBuffer())
		{
			return false;
		}
		return true;
	};
	bool CreateConstantBuffer()
	{
		HRESULT hr = S_OK;
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.ByteWidth = sizeof(cbDataShadow);
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = &m_Shadow_cbData;
		hr = g_pd3dDevice->CreateBuffer(&bd, &data, m_pShadowCB.GetAddressOf());
		if (FAILED(hr)) return false;
		return true;
	};
	bool Frame()
	{
		m_Shadow_cbData.m_matShadow = m_pLight->m_matView * m_pLight->m_matProj * m_matTexture;
		return true;
	};
	bool Release()
	{
		m_ShadowRT.Release();
		return true;
	};
};

