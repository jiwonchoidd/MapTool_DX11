#include "KObject.h"
void KObject::SetMatrix(KMatrix* pMatWorld, KMatrix* pMatView, KMatrix* pMatProj)
{
    if (pMatWorld != nullptr)
    {
        m_matWorld = *pMatWorld;
        m_cbData.matWorld = pMatWorld->Transpose();
    }
    if (pMatView != nullptr)
    {
        m_matView = *pMatView;
        m_cbData.matView = pMatView->Transpose();
    }
    if (pMatProj != nullptr)
    {
        m_matProj = *pMatProj;
        m_cbData.matProj = pMatProj->Transpose();
    }
    //m_cbData.matNormal = m_cbData.matNormal.Transpose();
}

void KObject::SetMatrix_Billboard(KMatrix* pMatWorld, KMatrix* pMatView, KMatrix* pMatProj)
{
    if (pMatWorld != nullptr)
    {
        KMatrix billboard = pMatView->Transpose();
        pMatWorld->_11 = billboard._11;
        pMatWorld->_13 = billboard._13;
        pMatWorld->_31 = billboard._31;
        pMatWorld->_33 = billboard._33;

        pMatWorld->_22 = billboard._22;
        pMatWorld->_23 = billboard._23;
        pMatWorld->_32 = billboard._32;
        pMatWorld->_33 = billboard._33;
        m_cbData.matWorld = pMatWorld->Transpose();
    }
    if (pMatView != nullptr)
    {
        m_cbData.matView = pMatView->Transpose();
    }
    if (pMatProj != nullptr)
    {
        m_cbData.matProj = pMatProj->Transpose();
    }
    //m_cbData.matNormal = m_cbData.matNormal.Transpose();
}

bool KObject::Init()
{
    return true;
}

bool KObject::Frame()
{
    return true;
}

bool KObject::PreRender(ID3D11DeviceContext* pContext)
{
    if (!m_bVisibility)return false;

    if (m_VertexList.size() <= 0) return true;
    //리소스 업데이트 데이터와 리소스 버퍼의 저장
    pContext->UpdateSubresource(
        m_pConstantBuffer.Get(), 0, NULL, &m_cbData, 0, 0);

    pContext->UpdateSubresource(
        m_pConstantBuffer_EX.Get(), 0, NULL, &m_cbDataEX, 0, 0);

    pContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
    pContext->PSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());

    pContext->VSSetConstantBuffers(3, 1, m_pConstantBuffer_EX.GetAddressOf());
    pContext->PSSetConstantBuffers(3, 1, m_pConstantBuffer_EX.GetAddressOf());

    //텍스쳐 리소스를 0번 슬롯 - 디퓨즈 //1번 슬롯 - 스페큘러 //2번 슬롯 - 노말
    if(m_pTexture_Diffuse !=nullptr)
    pContext->PSSetShaderResources(0, 1, m_pTexture_Diffuse->m_pSRVTexture.GetAddressOf());

    if (m_pTexture_Specular != nullptr)
    pContext->PSSetShaderResources(1, 1, m_pTexture_Specular->m_pSRVTexture.GetAddressOf());

    if (m_pTexture_Normal != nullptr)
     pContext->PSSetShaderResources(2, 1, m_pTexture_Normal->m_pSRVTexture.GetAddressOf());

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
    UINT pStrides[2] = { sizeof(PNCT_VERTEX), sizeof(BT_VERTEX) };
    UINT pOffsets[2] = {0,};

    //정점버퍼 바인딩 인덱스버퍼 바인딩 0번슬롯
    //배열로 0~1 번슬롯 할당
    ID3D11Buffer* buffer[2] = { m_pVertexBuffer.Get(), m_pVertexBTBuffer.Get()};
    pContext->IASetVertexBuffers(0, 2, buffer,
        pStrides, pOffsets);

    pContext->IASetIndexBuffer(m_pIndexBuffer.Get(),DXGI_FORMAT_R32_UINT, 0);
    return true;
}

bool KObject::Render(ID3D11DeviceContext* pContext)
{
    if (!m_bVisibility)return false;
    if (PreRender(pContext) == false) return false;
    if (PostRender(pContext, m_iNumIndex) == false) return false;
    return true;
}

bool KObject::PostRender(ID3D11DeviceContext* pContext, UINT iNumIndex)
{
    if (!m_bVisibility)return false;

    if (iNumIndex > 0)
    {
        pContext->DrawIndexed(iNumIndex, 0, 0);
        return true;
    }
    else
    {
        pContext->Draw(m_VertexList.size(), 0);
        return true;
    }
    return false;
}

//매니져를 사용해 쉐이더 로드
bool KObject::LoadShader(std::wstring vsFile, std::wstring psFile)
{
    if (m_pVS != nullptr || m_pPS != nullptr)
    {
        return true;
    }
    else
    {
        if (!vsFile.empty() || !psFile.empty())
        {
            m_pVS = g_ShaderManager.CreateVertexShader(vsFile, "VS");
            m_pPS = g_ShaderManager.CreatePixelShader(psFile, "PS");
            return true;
        }
        return false;
    }
}

//딱 한 프레임 구간에서만 바꿔치고 다시 원래 PS로 바꾸기 위해 Bool 값 사용
bool KObject::SwapPSShader(KShader* pShader)
{
    m_pPS_Swaped = pShader;
    if (pShader == nullptr)
    {
        return false;
    }
    //바꿀 쉐이더
    return true;
}

//매니져를 사용해 텍스쳐 로드
bool KObject::LoadTexture(std::wstring tex1, std::wstring tex2, std::wstring tex3)
{
    if (!tex1.empty())
    {
        m_pTexture_Diffuse = g_TextureMananger.Load(tex1);
        m_TextureDesc = m_pTexture_Diffuse->m_TextureDesc;
    }
    if (!tex2.empty())
    {
        m_pTexture_Specular = g_TextureMananger.Load(tex2);
    }
    if (!tex3.empty())
    {
        m_pTexture_Normal = g_TextureMananger.Load(tex3);
    }
    return true;
}

bool KObject::SwapVisibility()
{
    m_bVisibility = !m_bVisibility;
    return m_bVisibility;
}

bool KObject::CheckVertexData()
{
    if (m_VertexList.size() > 0)
    {
        return true;
    }
    return false;
}

bool KObject::CheckIndexData()
{
    if (m_IndexList.size() > 0)
    {
        return true;
    }
    return false;
}

bool KObject::CreateVertexData()
{
    return true;
}

bool KObject::CreateIndexData()
{
    return true;
}

HRESULT KObject::CreateConstantBuffer()
{
    HRESULT hr = S_OK;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = sizeof(CB_DATA);
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem = &m_cbData;
    hr = g_pd3dDevice->CreateBuffer(&bd, &data, m_pConstantBuffer.GetAddressOf());
    if (FAILED(hr)) return hr;

    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = sizeof(CB_DATA_EX);
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem = &m_cbDataEX;
    hr = g_pd3dDevice->CreateBuffer(&bd, &data, m_pConstantBuffer_EX.GetAddressOf());
    if (FAILED(hr)) return hr;
    return hr;
}

HRESULT KObject::CreateVertexBuffer()
{
    HRESULT hr = S_OK;
    if (m_VertexList.size() <= 0) return hr;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = sizeof(PNCT_VERTEX) * m_VertexList.size();
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem = &m_VertexList.at(0);
    hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pVertexBuffer);
    if (FAILED(hr)) return hr;

    if (m_BTList.size() > 0)
    {
        HRESULT hr = S_OK;
        if (m_BTList.size() <= 0) return hr;
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
        bd.ByteWidth = sizeof(BT_VERTEX) * m_BTList.size();
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA data;
        ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
        data.pSysMem = &m_BTList.at(0);
        hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pVertexBTBuffer);
       /* if (FAILED(hr)) return hr;*/
    }
    return hr;
}

HRESULT KObject::CreateIndexBuffer()
{
    HRESULT hr = S_OK;
    //if (m_IndexList.size() <= 0) return hr;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = sizeof(DWORD) * m_IndexList.size();
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem = &m_IndexList.at(0);
    hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pIndexBuffer);
    if (FAILED(hr)) return hr;

    m_iNumIndex = m_IndexList.size();
    return hr;
}

HRESULT KObject::CreateVertexLayout()
{
    HRESULT hr = S_OK;
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,        0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },

        { "TANGENT",   0, DXGI_FORMAT_R32G32B32_FLOAT,   1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BINORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,  1, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    hr = g_pd3dDevice->CreateInputLayout(layout, _countof(layout),
        m_pVS->m_pVSCodeResult.Get()->GetBufferPointer(),
        m_pVS->m_pVSCodeResult.Get()->GetBufferSize(),
        &m_pVertexLayout);
    if (FAILED(hr)) return hr;

    return hr;
}


bool KObject::CreateObject(std::wstring vsFile,
    std::wstring psFile, std::wstring tex1, std::wstring tex2, std::wstring tex3)
{
    //버텍스 데이터 생성
    if (!CreateVertexData())
    {
        return false;
    }
    if (!CreateIndexData())
    {
        return false;
    }
    if (!LoadTexture(tex1, tex2, tex3))
    {
        return false;
    }

    if (CheckVertexData())
    {
        CreateVertexBuffer();
        if (CheckIndexData())
        {
            CreateIndexBuffer();
        }
        if ((LoadShader(vsFile, psFile)))
        {
            if (SUCCEEDED(CreateVertexLayout()))
            {
                CreateConstantBuffer();
                return true;
            }
        }
    }
    return false;
}


bool KObject::Release()
{
    m_pVertexBuffer.Reset();
    m_pVertexBTBuffer.Reset();
    m_pIndexBuffer.Reset();
    m_pConstantBuffer.Reset();
    m_pVertexLayout.Reset();
    m_pConstantBuffer_EX.Reset();
    m_VertexList.clear();
    m_BTList.clear();
    m_IndexList.clear();
    return true;
}

KObject::KObject()
{
    m_iVertexSize = sizeof(PNCT_VERTEX);
    m_iNumIndex = 0;
    m_bVisibility=true;
}