#include "KFBXObj.h"
#include "KState.h"
bool KFBXObj::PreRender(ID3D11DeviceContext* pContext)
{
	//if (m_VertexList.size() <= 0) return true;
	//리소스 업데이트 데이터와 리소스 버퍼의 저장
	pContext->UpdateSubresource(
		m_pConstantBuffer.Get(), 0, NULL, &m_cbData, 0, 0);
	pContext->UpdateSubresource(
		m_pConstantBuffer_EX.Get(), 0, NULL, &m_cbDataEX, 0, 0);

	pContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	pContext->PSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());

	pContext->VSSetConstantBuffers(3, 1, m_pConstantBuffer_EX.GetAddressOf());
	pContext->PSSetConstantBuffers(3, 1, m_pConstantBuffer_EX.GetAddressOf());

	////텍스쳐 리소스를 0번 슬롯 - 디퓨즈 //1번 슬롯 - 스페큘러 //2번 슬롯 - 노말
	//쉐이더
	pContext->VSSetShader(m_pVS->m_pVertexShader.Get(), NULL, 0);
	if (m_pPS_Swaped!=nullptr)
	{
		pContext->PSSetShader(m_pPS_Swaped->m_pPixelShader.Get(), NULL, 0);
		m_pPS_Swaped = nullptr;
	}
	else
	{
		pContext->PSSetShader(m_pPS->m_pPixelShader.Get(), NULL, 0);
	}

	pContext->IASetInputLayout(m_pVertexLayout.Get());
	//UINT pStrides = m_iVertexSize;
	//UINT pOffsets = 0;

	////정점버퍼 바인딩 인덱스버퍼 바인딩 
	//pContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(),
	//	&pStrides, &pOffsets);
	//pContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	return true;
}

bool KFBXObj::Render(ID3D11DeviceContext* context)
{
	if (PreRender(context) == false) return false;
	if (PostRender(context, m_iNumIndex) == false) return false;
	return true;
}

bool KFBXObj::PostRender(ID3D11DeviceContext* pContext, UINT iNumIndex)
{
	UINT Strides[3] = { sizeof(PNCT_VERTEX) ,sizeof(BT_VERTEX), sizeof(IW_VERTEX)};
	UINT Offsets[3] = { 0, };

	for (int index = 0; index < m_pSubVertexList.size(); index++)
	{
		//텍스쳐 리소스를 0번 슬롯 - 디퓨즈 //1번 슬롯 - 스페큘러 //2번 슬롯 - 노말
		if (m_pTextureList[index].texAlbedo!= nullptr)
		pContext->PSSetShaderResources(0, 1, m_pTextureList[index].texAlbedo->m_pSRVTexture.GetAddressOf());

		if (m_pTextureList[index].texSpecular != nullptr)
			pContext->PSSetShaderResources(1, 1, m_pTextureList[index].texSpecular->m_pSRVTexture.GetAddressOf());

		if (m_pTextureList[index].texNormal != nullptr)
			pContext->PSSetShaderResources(2, 1, m_pTextureList[index].texNormal->m_pSRVTexture.GetAddressOf());

		if (m_pTextureList[index].texAlpha != nullptr)
		{
			pContext->PSSetShaderResources(4, 1, m_pTextureList[index].texAlpha->m_pSRVTexture.GetAddressOf());
			ApplyRS(pContext, KState::g_pRSAllface);
		}
		else
		{
			ID3D11ShaderResourceView* nullSRV[1] = { nullptr };//알파맵 없으면 리소스 바인딩 해제
			pContext->PSSetShaderResources(4, 1, nullSRV);
		}

		ID3D11Buffer* buffer[3] = { m_pVBList[index], m_pVBBTList[index], m_pVBWeightList[index] };

		pContext->IASetVertexBuffers(0, 3, buffer,Strides, Offsets);

		if (m_IndexList.size() <= 0)
			pContext->Draw(m_pSubVertexList[index].size(), 0);
		else
			pContext->DrawIndexed(m_IndexList.size(), 0, 0);
	}
	ApplyRS(pContext, KState::g_pCurrentRS);
	return true;
}

bool KFBXObj::Release()
{
	K3DAsset::Release();
	for (int ivb = 0; ivb < m_pVBList.size(); ivb++)
	{
		if (m_pVBList[ivb] != nullptr)
		{
			m_pVBList[ivb]->Release();
		}
	}
	for (int ivb = 0; ivb < m_pVBBTList.size(); ivb++)
	{
		if (m_pVBBTList[ivb] != nullptr)
		{
			m_pVBBTList[ivb]->Release();
		}
	}
	for (int ivb = 0; ivb < m_pVBWeightList.size(); ivb++)
	{
		if (m_pVBWeightList[ivb] != nullptr)
		{
			m_pVBWeightList[ivb]->Release();
		}
	}
	return true;
}

bool KFBXObj::CheckVertexData()
{
	return true;
}


bool KFBXObj::CreateVertexData()
{
	if (m_pSubVertexList.size() > 0)
	{
		m_pVBList.resize(m_pSubVertexList.size());
	}
	if (m_pSubIWVertexList.size() > 0)
	{
		m_pVBWeightList.resize(m_pSubIWVertexList.size());
	}
	if (m_pSubBTList.size() > 0)
	{
		m_pVBBTList.resize(m_pSubBTList.size());
	}
	return true;
}

bool KFBXObj::CreateIndexData()
{
	return true;
}

HRESULT KFBXObj::CreateVertexLayout()
{
	//애니메이션을 위한 3번 슬롯 INDEX, WEIGHT
	HRESULT hr = S_OK;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,        0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "TANGENT",   0, DXGI_FORMAT_R32G32B32_FLOAT,   1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,  1, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "INDEX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 16,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = g_pd3dDevice->CreateInputLayout(layout, _countof(layout),
		m_pVS->m_pVSCodeResult.Get()->GetBufferPointer(),
		m_pVS->m_pVSCodeResult.Get()->GetBufferSize(),
		&m_pVertexLayout);
	if (FAILED(hr)) return hr;
	return hr;
}

HRESULT KFBXObj::CreateVertexBuffer()
{
	//서브 버텍스 리스트 생성
	HRESULT hr = S_OK;
	for (int index = 0; index < m_pSubVertexList.size(); index++)
	{
		if (m_pSubVertexList[index].size() <= 0) return hr;
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.ByteWidth = sizeof(PNCT_VERTEX) * m_pSubVertexList[index].size();
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
		sd.pSysMem = &m_pSubVertexList[index].at(0);

		hr = g_pd3dDevice->CreateBuffer(&bd, &sd, &m_pVBList[index]);
		if (FAILED(hr))return hr;
	}
	//서브 바이노말 탄젠트 버퍼 생성
	for (int index = 0; index < m_pSubBTList.size(); index++)
	{
		HRESULT hr = S_OK;
		if (m_pSubBTList[index].size() <= 0) return hr;
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.ByteWidth = sizeof(BT_VERTEX) * m_pSubBTList[index].size();
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = &m_pSubBTList[index].at(0);
		hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pVBBTList[index]);
		if (FAILED(hr)) return hr;
	}
	//추가적인 Vertexlist 가중치 값
	for (int iWeight = 0; iWeight < m_pSubIWVertexList.size(); iWeight++)
	{
		if (m_pSubIWVertexList[iWeight].size() <= 0) return hr;
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.ByteWidth = sizeof(IW_VERTEX) * m_pSubIWVertexList[iWeight].size();
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
		sd.pSysMem = &m_pSubIWVertexList[iWeight].at(0);
		hr = g_pd3dDevice->CreateBuffer(&bd, &sd, &m_pVBWeightList[iWeight]);
		if (FAILED(hr))return hr;
	}

	return hr;
}

KFBXObj::KFBXObj()
{
}

KFBXObj::~KFBXObj()
{
}
