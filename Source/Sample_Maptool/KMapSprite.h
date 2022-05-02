#pragma once
#include "KMap.h"
// ComputeShading으로 지형에 스프라이팅 하는 클래스 
struct PICKBUFFER
{
	KVector3 vPickPos;
	KVector3 vRect[4];
	float  fRadius;
	int    iIndex; //4개 텍스쳐 알파값 인덱스 
};
class KMapSprite
{
private:
	KTexture*									m_pBlankTexture;
	wrl::ComPtr<ID3D11Texture2D>				m_pTexture;
	std::wstring								m_strAlphaFile;
public:
	int						m_iWidth;
	int						m_iHeight;
	KMap*					m_pMap;
	ID3D11DeviceContext*	m_pContext;
public:
	wrl::ComPtr<ID3D11ComputeShader>			m_pCS;
	wrl::ComPtr<ID3D11ShaderResourceView>		m_pTextureSRV;
	wrl::ComPtr<ID3D11Texture2D>				m_pTextureCopy;
	wrl::ComPtr<ID3D11ShaderResourceView>		m_pTextureCopySRV;
public:
	PICKBUFFER								m_Pickbuffer;
	wrl::ComPtr<ID3D11Buffer>				m_pPickBuffer;
	wrl::ComPtr<ID3D11ShaderResourceView>	m_pPickBufferSRV;
	wrl::ComPtr<ID3D11UnorderedAccessView>	m_pResultUAV;
public:
	void	RunComputeShader(ID3D11DeviceContext* pd3dImmediateContext,
		ID3D11ComputeShader* pComputeShader,
		UINT nNumViews, ID3D11ShaderResourceView** pShaderResourceViews,
		ID3D11Buffer* pCBCS, void* pCSData, DWORD dwNumDataBytes,
		ID3D11UnorderedAccessView** pUnorderedAccessView,
		UINT X, UINT Y, UINT Z);
	HRESULT CreateStructuredBuffer(ID3D11Device* pDevice, UINT uElementSize, UINT uCount, VOID* pInitData, ID3D11Buffer** ppBufOut);
	HRESULT CreateBufferSRV(ID3D11Device* pDevice, ID3D11Buffer* pBuffer, ID3D11ShaderResourceView** ppSRVOut);
	HRESULT CreateBufferUAV(ID3D11Device* pDevice,
		int iWidth, int iHeight, ID3D11UnorderedAccessView** ppUAVOut, std::wstring filename= L"");
public:
	void UpdatePickPos(KVector3 vIntersect, float fRadius);
	void SetAlphaFileName(std::wstring file);
public:
	bool Init(ID3D11DeviceContext* pContext, KMap* pMap);
	bool Frame();
	bool Release();
public:
	KMapSprite();
	~KMapSprite();
};

