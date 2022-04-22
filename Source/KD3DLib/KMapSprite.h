#pragma once
#include "KMap.h"
// ComputeShading으로 지형에 스프라이팅 하는 클래스 
struct PICKBUFFER
{
	KVector3 vPickPos;
	KVector3 vRect[4];
	float  fRadius;
	int    iIndex;
};
class KMapSprite
{
public:
	KMap* m_pMap;
public:
	wrl::ComPtr<ID3D11ComputeShader>			m_pCS;
	wrl::ComPtr<ID3D11Texture2D>				m_pTexureCopy;
	wrl::ComPtr<ID3D11ShaderResourceView>		m_pTexureCopySRV;
public:
	PICKBUFFER								m_Pickbuffer;
	wrl::ComPtr<ID3D11Buffer>				m_pPickBuffer;
	wrl::ComPtr<ID3D11ShaderResourceView>	m_pPickBufferSRV;
public:
	void	RunComputeShader(ID3D11DeviceContext* pd3dImmediateContext,
		ID3D11ComputeShader* pComputeShader,
		UINT nNumViews, ID3D11ShaderResourceView** pShaderResourceViews,
		ID3D11Buffer* pCBCS, void* pCSData, DWORD dwNumDataBytes,
		ID3D11UnorderedAccessView** pUnorderedAccessView,
		UINT X, UINT Y, UINT Z);
	HRESULT CreateBufferUAV(ID3D11Device* pDevice,
		int iWidth, int iHeight, ID3D11UnorderedAccessView** ppUAVOut);
public:
	bool Init(KMap* pMap);
	bool Frame();
	bool Release();
public:
	KMapSprite();
	~KMapSprite();
};

