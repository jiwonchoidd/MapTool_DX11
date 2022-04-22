#include "KState.h"
ID3D11DepthStencilState* KState::g_pCurrentDSS = nullptr;
ID3D11SamplerState* KState::g_pCurrentSS[3] = { nullptr,nullptr,nullptr };
ID3D11RasterizerState* KState::g_pCurrentRS = nullptr;
ID3D11BlendState* KState::g_pCurrentBS = nullptr;

ID3D11BlendState* KState::g_pBlendState = nullptr;
ID3D11BlendState* KState::g_pAlphaBlendState = nullptr;
ID3D11DepthStencilState* KState::g_pDSS = nullptr;
ID3D11DepthStencilState* KState::g_pDSS_Disabled = nullptr;
ID3D11SamplerState* KState::g_pClampSS = nullptr;
ID3D11SamplerState* KState::g_pWrapSS = nullptr;
ID3D11SamplerState* KState::g_pMirrorSS = nullptr;
ID3D11SamplerState* KState::g_pNoFilterSS =nullptr;
ID3D11RasterizerState* KState::g_pRSSolid = nullptr;
ID3D11RasterizerState* KState::g_pRSWireFrame = nullptr;
ID3D11RasterizerState* KState::g_pRSBackface = nullptr;
ID3D11RasterizerState* KState::g_pRSAllface = nullptr;

bool KState::SetState()
{
    CreateDepthStenState();
    CreateSamplerState();
    CreateRasterizeState();
    CreateBlendState();

    //초기 설정
     g_pCurrentDSS = g_pDSS;
     g_pCurrentSS[0] = g_pClampSS;
     g_pCurrentSS[1] = g_pWrapSS;
     g_pCurrentRS = g_pRSSolid;
     g_pCurrentBS = g_pBlendState;
	return true;
}
HRESULT KState::CreateDepthStenState()
{
    HRESULT hr = S_OK;
    D3D11_DEPTH_STENCIL_DESC dsd;
    ZeroMemory(&dsd, sizeof(D3D11_DEPTH_STENCIL_DESC));
    dsd.DepthEnable = TRUE;
    dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    //1.0, 0.6과 0.5를 비교해서 크면 실패
    //제일 앞에 있는 면이 뿌려지면 뒤에있는애들은 렌더 안됨
    dsd.DepthFunc = D3D11_COMPARISON_LESS;
    dsd.StencilEnable = TRUE;
    dsd.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    dsd.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
    dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    hr = g_pd3dDevice->CreateDepthStencilState(&dsd, &g_pDSS);

    ZeroMemory(&dsd, sizeof(D3D11_DEPTH_STENCIL_DESC));
    dsd.DepthEnable = FALSE;
    dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    //1.0, 0.6과 0.5를 비교해서 크면 실패
    //제일 앞에 있는 면이 뿌려지면 뒤에있는애들은 렌더 안됨
    dsd.DepthFunc = D3D11_COMPARISON_ALWAYS;
    dsd.StencilEnable = TRUE;
    dsd.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    dsd.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
    dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    hr = g_pd3dDevice->CreateDepthStencilState(&dsd, &g_pDSS_Disabled);

    if (FAILED(hr))
    {
        return hr;
    }
    //깊이 스텐실 제일 마지막 결과 기반으로 렌더하는 것이기에 OM
    return hr;
}

HRESULT KState::CreateBlendState()
{
    HRESULT hr = S_OK;
    D3D11_BLEND_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));

    bd.AlphaToCoverageEnable = false;
    bd.IndependentBlendEnable = false;
    bd.RenderTarget[0].BlendEnable = TRUE;
    bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    //// A 연산 저장
    bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    hr = g_pd3dDevice->CreateBlendState(&bd, &g_pBlendState);
    if (FAILED(hr))
    {
        return hr;
    }
    ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
    bd.RenderTarget[0].BlendEnable = TRUE;
    bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    //// A 연산 저장
    bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
    bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    hr = g_pd3dDevice->CreateBlendState(&bd, &g_pAlphaBlendState);
    if (FAILED(hr))
    {
        return hr;
    }
}

HRESULT KState::CreateRasterizeState()
{
    HRESULT hr = S_OK;
    D3D11_RASTERIZER_DESC rd;
    ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
    rd.FillMode = D3D11_FILL_WIREFRAME;
    rd.CullMode = D3D11_CULL_NONE;
    hr = g_pd3dDevice->CreateRasterizerState(&rd, &g_pRSWireFrame);

    ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
    rd.FillMode = D3D11_FILL_SOLID;
    rd.CullMode = D3D11_CULL_BACK;
    rd.DepthClipEnable = TRUE;
    hr = g_pd3dDevice->CreateRasterizerState(&rd, &g_pRSSolid);

    ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
    rd.FillMode = D3D11_FILL_SOLID;
    rd.CullMode = D3D11_CULL_FRONT;
    rd.DepthClipEnable = TRUE; // Clipping 효과 기본이 False임
    hr = g_pd3dDevice->CreateRasterizerState(&rd, &g_pRSBackface);

    ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
    rd.FillMode = D3D11_FILL_SOLID;
    rd.CullMode = D3D11_CULL_NONE;
    rd.DepthClipEnable = TRUE; // Clipping 효과 기본이 False임
    hr = g_pd3dDevice->CreateRasterizerState(&rd, &g_pRSAllface);
    return hr;
}

HRESULT KState::CreateSamplerState()
{
    D3D11_SAMPLER_DESC sd;
    ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.MinLOD = FLT_MAX;
    sd.MaxLOD = FLT_MIN;
    HRESULT hr = g_pd3dDevice->CreateSamplerState(&sd,
        &g_pClampSS);

    ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.MinLOD = FLT_MAX;
    sd.MaxLOD = FLT_MIN;
    hr = g_pd3dDevice->CreateSamplerState(&sd, &g_pWrapSS);

    ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
    sd.MinLOD = FLT_MAX;
    sd.MaxLOD = FLT_MIN;
    hr = g_pd3dDevice->CreateSamplerState(&sd,
        &g_pMirrorSS);

    ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.MinLOD = FLT_MAX;
    sd.MaxLOD = FLT_MIN;
    hr = g_pd3dDevice->CreateSamplerState(&sd,
        &g_pNoFilterSS);

    return hr;
}


bool KState::ReleaseState()
{
    if (g_pBlendState)g_pBlendState->Release();
    if (g_pDSS)g_pDSS->Release();
    if (g_pDSS_Disabled)g_pDSS_Disabled->Release();
    if (g_pClampSS)g_pClampSS->Release();
    if (g_pWrapSS)g_pWrapSS->Release();
    if (g_pMirrorSS)g_pMirrorSS->Release();
    if (g_pNoFilterSS)g_pNoFilterSS->Release();
    if (g_pRSSolid)g_pRSSolid->Release();
    if (g_pRSBackface)g_pRSBackface->Release();
    if (g_pRSWireFrame)g_pRSWireFrame->Release();
    if (g_pAlphaBlendState)g_pAlphaBlendState->Release();
    if (g_pRSAllface) g_pRSAllface->Release();
    g_pRSAllface = nullptr;
    g_pAlphaBlendState = nullptr;
    g_pBlendState = nullptr;
    g_pDSS = nullptr;
    g_pDSS_Disabled = nullptr;
    g_pClampSS = nullptr;
    g_pWrapSS = nullptr;
    g_pMirrorSS = nullptr;
    g_pNoFilterSS = nullptr;
    g_pRSSolid = nullptr;
    g_pRSWireFrame = nullptr;
    g_pRSBackface = nullptr;
   
    return true;
}