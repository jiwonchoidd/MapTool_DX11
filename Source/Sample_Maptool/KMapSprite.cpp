#include "KMapSprite.h"
//맵을 뿌릴때, 복사해온 디퓨즈 맵을 SRV로 뿌리고
//CS의 CSSetUnorderedAccessView와 연결을 해제함
void KMapSprite::RunComputeShader(ID3D11DeviceContext* pContext, ID3D11ComputeShader* pComputeShader,
	UINT nNumViews, ID3D11ShaderResourceView** pShaderResourceViews, ID3D11Buffer* pCBCS,
	void* pCSData, DWORD dwNumDataBytes, ID3D11UnorderedAccessView** pUnorderedAccessView, UINT X, UINT Y, UINT Z)
{
	pContext->CSSetShader(pComputeShader, NULL, 0);
	pContext->CSSetShaderResources(0, nNumViews, pShaderResourceViews);
	pContext->CSSetUnorderedAccessViews(0, 1, pUnorderedAccessView, NULL);
	if (pCBCS)
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		pContext->Map(pCBCS, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		memcpy(MappedResource.pData, pCSData, dwNumDataBytes);
		pContext->Unmap(pCBCS, 0);
		ID3D11Buffer* ppCB[1] = { pCBCS };
		pContext->CSSetConstantBuffers(0, 1, ppCB);
	}

	pContext->Dispatch(X, Y, Z);

	pContext->CSSetShader(NULL, NULL, 0);

	//다시 널로 바꿔주는 작업
	ID3D11UnorderedAccessView* ppUAViewNULL[1] = { NULL };
	pContext->CSSetUnorderedAccessViews(0, 1, ppUAViewNULL, NULL);

	ID3D11ShaderResourceView* ppSRVNULL[2] = { NULL, NULL };
	pContext->CSSetShaderResources(0, 2, ppSRVNULL);

	ID3D11Buffer* ppCBNULL[1] = { NULL };
	pContext->CSSetConstantBuffers(0, 1, ppCBNULL);
}

HRESULT KMapSprite::CreateBufferUAV(ID3D11Device* pDevice, int iWidth, int iHeight, ID3D11UnorderedAccessView** ppUAVOut, std::wstring filename)
{
	HRESULT hr = S_OK;
	//before dispatch
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = iWidth;
	textureDesc.Height = iHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	hr = pDevice->CreateTexture2D(&textureDesc, NULL, m_pTexture.GetAddressOf());
	hr = pDevice->CreateShaderResourceView(m_pTexture.Get(), &srvDesc, m_pTextureSRV.GetAddressOf());

	D3D11_UNORDERED_ACCESS_VIEW_DESC viewDescUAV;
	ZeroMemory(&viewDescUAV, sizeof(viewDescUAV));
	viewDescUAV.Format = textureDesc.Format;
	viewDescUAV.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	viewDescUAV.Texture2D.MipSlice = 0;
	hr = pDevice->CreateUnorderedAccessView(m_pTexture.Get(), &viewDescUAV, ppUAVOut);
	// 복사본
	if (filename.empty())
	{
		hr = pDevice->CreateTexture2D(&textureDesc, NULL, m_pTextureCopy.GetAddressOf());
		hr = pDevice->CreateShaderResourceView(m_pTextureCopy.Get(), &srvDesc, m_pTextureCopySRV.GetAddressOf());
	}
	else
	{
		HRESULT hr;
		wrl::ComPtr<ID3D11Resource> pTexture;
		size_t maxsize = 0;
		hr = CreateWICTextureFromFileEx(g_pd3dDevice,
			filename.c_str(),
			maxsize,
			D3D11_USAGE_STAGING,
			NULL,
			D3D11_CPU_ACCESS_READ,
			NULL,
			WIC_LOADER_DEFAULT,
			pTexture.GetAddressOf(), nullptr);

		hr = pDevice->CreateTexture2D(&textureDesc, NULL, m_pTextureCopy.GetAddressOf());
		m_pContext->CopyResource(m_pTextureCopy.Get(), pTexture.Get());
		hr = pDevice->CreateShaderResourceView(m_pTextureCopy.Get(), &srvDesc, m_pTextureCopySRV.GetAddressOf());

		pTexture.Reset();
	}

	return hr;
}

HRESULT KMapSprite::CreateStructuredBuffer(ID3D11Device* pDevice, UINT uElementSize, UINT uCount, VOID* pInitData, ID3D11Buffer** ppBufOut)
{
	*ppBufOut = NULL;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.ByteWidth = uElementSize * uCount;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = uElementSize;

	if (pInitData)
	{
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = pInitData;
		return pDevice->CreateBuffer(&desc, &InitData, ppBufOut);
	}
	else
		return pDevice->CreateBuffer(&desc, NULL, ppBufOut);
}

HRESULT KMapSprite::CreateBufferSRV(ID3D11Device* pDevice, ID3D11Buffer* pBuffer, ID3D11ShaderResourceView** ppSRVOut)
{
	D3D11_BUFFER_DESC descBuf;
	ZeroMemory(&descBuf, sizeof(descBuf));
	pBuffer->GetDesc(&descBuf);

	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	desc.BufferEx.FirstElement = 0;

	//Raw buffer일때
	if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
	{
		desc.Format = DXGI_FORMAT_R32_TYPELESS;
		desc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
		desc.BufferEx.NumElements = descBuf.ByteWidth / 4;
	}
	else
		//structed buffer 일떄
		if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
		{
			desc.Format = DXGI_FORMAT_UNKNOWN;
			desc.BufferEx.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;
		}
		else
		{
			return E_INVALIDARG;
		}
	return pDevice->CreateShaderResourceView(pBuffer, &desc, ppSRVOut);
}
//------------------------------------------------------------------------------------------------------
bool KMapSprite::Init(ID3D11DeviceContext* pContext, KMap* pMap)
{
	if (pMap == nullptr)return false;
	if (pContext == nullptr)return false;
	//브러쉬 버퍼
	m_pMap = pMap;
	m_pContext = pContext;
	m_Pickbuffer.fRadius = 20.0f;
	m_Pickbuffer.iIndex = 0;
	m_Pickbuffer.vPickPos = KVector3(0, 0, 0);
	m_Pickbuffer.vRect[0] = KVector3(-0, 0, 0);
	m_Pickbuffer.vRect[1] = KVector3(0, 0, 0);
	m_Pickbuffer.vRect[2] = KVector3(0, -0, 0);
	m_Pickbuffer.vRect[3] = KVector3(-0, 0, 0);
	//
	//Structed Buffer, SRV 생성 : cs에 보내는 버퍼
	CreateStructuredBuffer(g_pd3dDevice, sizeof(PICKBUFFER), 1, &m_Pickbuffer, m_pPickBuffer.GetAddressOf());
	CreateBufferSRV(g_pd3dDevice, m_pPickBuffer.Get(), m_pPickBufferSRV.GetAddressOf());
	//unorder access view 생성 : SRV 순서와 상관없이 다른 리소스에 대한 읽기 및 쓰기 허용
	CreateBufferUAV(g_pd3dDevice, m_pMap->m_BoxCollision.size.x, m_pMap->m_BoxCollision.size.z, m_pResultUAV.GetAddressOf(), m_strAlphaFile);
	//CS 쉐이더 생성

	KShader* pCS = g_ShaderManager.CreateComputeShader(L"../../data/shader/CS_Terrain.hlsl");
	m_pCS = pCS->m_pComputeShader;

	//기존텍스쳐가 아닌 맵의 텍스쳐를 복사되어지는 텍스쳐로 바꾼다.
	pMap->m_pMapAlphaResultSRV = m_pTextureCopySRV.Get();
	return true;
}

//매 프레임 마다 위치 변경
void KMapSprite::UpdatePickPos(KVector3 vIntersect, float fRadius)
{
	m_Pickbuffer.fRadius = fRadius;
	m_Pickbuffer.vPickPos = KVector3(vIntersect.x, vIntersect.z, 0); // 화면 좌표계임
	m_Pickbuffer.vRect[0] = KVector3(-640, 640, 0);
	m_Pickbuffer.vRect[1] = KVector3(640, 640, 0);
	m_Pickbuffer.vRect[2] = KVector3(640, -640, 0);
	m_Pickbuffer.vRect[3] = KVector3(-640, -640, 0);
}
void KMapSprite::SetAlphaFileName(std::wstring file)
{
	m_strAlphaFile = file;
}
bool KMapSprite::Frame()
{
	if (g_InputData.bMouseState[0])
	{
		m_pContext->UpdateSubresource(m_pPickBuffer.Get(), 0, NULL, &m_Pickbuffer, 0, 0);
		//맵의 텍스쳐를 복사해온다.
		ID3D11ShaderResourceView* aRViews[3] = { m_pMap->m_pTexture_Diffuse->m_pSRVTexture.Get(), m_pTextureCopySRV.Get(), m_pPickBufferSRV.Get()};
		RunComputeShader(m_pContext, m_pCS.Get(), 3, aRViews, NULL, NULL, 0,
			m_pResultUAV.GetAddressOf(),
			m_pMap->m_BoxCollision.size.x/32, m_pMap->m_BoxCollision.size.z / 32, 1);

		m_pContext->CopyResource(m_pTextureCopy.Get(), m_pTexture.Get());
	}
	return true;
}

bool KMapSprite::Release()
{
	m_pCS.Reset();
	m_pTextureCopy.Reset();
	m_pTextureCopySRV.Reset();
	m_pTexture.Reset();
	m_pTextureSRV.Reset();
	m_pPickBuffer.Reset();
	m_pPickBufferSRV.Reset();
	m_pResultUAV.Reset();
	return true;
}

KMapSprite::KMapSprite()
{
	m_pMap = nullptr;
}

KMapSprite::~KMapSprite()
{
	
}
