#include "KMapSprite.h"

void KMapSprite::RunComputeShader(ID3D11DeviceContext* pd3dImmediateContext, ID3D11ComputeShader* pComputeShader, UINT nNumViews, ID3D11ShaderResourceView** pShaderResourceViews, ID3D11Buffer* pCBCS, void* pCSData, DWORD dwNumDataBytes, ID3D11UnorderedAccessView** pUnorderedAccessView, UINT X, UINT Y, UINT Z)
{
}

HRESULT KMapSprite::CreateBufferUAV(ID3D11Device* pDevice, int iWidth, int iHeight, ID3D11UnorderedAccessView** ppUAVOut)
{
	return E_NOTIMPL;
}

bool KMapSprite::Init(KMap* pMap)
{
	m_pMap = pMap;
	return false;
}

bool KMapSprite::Frame()
{
	return false;
}

bool KMapSprite::Release()
{
	return false;
}

KMapSprite::KMapSprite()
{
	m_pMap = nullptr;
}

KMapSprite::~KMapSprite()
{
	m_pCS.Reset();
	m_pTexureCopy.Reset();
	m_pTexureCopySRV.Reset();
	m_pPickBuffer.Reset();
	m_pPickBufferSRV.Reset();
}
