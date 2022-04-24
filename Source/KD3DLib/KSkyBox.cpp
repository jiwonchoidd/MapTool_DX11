#include "KSkyBox.h"
#include "KState.h"
void KSkyBox::SetMatrix(KMatrix* pMatWorld, KMatrix* pMatView, KMatrix* pMatProj)
{
    if (pMatWorld != nullptr)
    {
        pMatWorld->_11 = 5.0f;
        pMatWorld->_22 = 5.0f;
        pMatWorld->_33 = 5.0f;
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
    m_cbData.matNormal = m_cbData.matNormal.Transpose();
}
//��ī�� �ڽ� 3�� ���, 
//1. 6�� ���� �ϳ��� �Ѹ��� ��� �������� for�� ���ƾ���, 
//2. �ؽ��� SRV�� �迭�� �޾Ƽ� �ѹ��� �Ѹ��� ���, 
//3. ť��� cubemap�� ��� ť��� ���ҽ� �ʿ���. (�� ���)
bool KSkyBox::Init(ID3D11DeviceContext* context,std::wstring shader, std::wstring tex)
{
    m_pContext = context;
    KBoxObj::Init(shader, shader, tex, L"", L"");
    m_pCubeTex = m_pTexture_Diffuse;
    return true;
}

bool KSkyBox::Frame()
{
    return true;
}

bool KSkyBox::Render(ID3D11DeviceContext* pContext)
{
    if (KState::g_pCurrentRS != KState::g_pRSWireFrame)
        ApplyRS(pContext, KState::g_pRSBackface);
    //��� ���̴� 5���� ��ī�� �ؽ��ĸ� �ѱ��.
    pContext->PSSetShaderResources(6, 1, m_pCubeTex->m_pSRVTexture.GetAddressOf());
    ApplyDSS(pContext, KState::g_pDSS_Disabled);
    KObject::Render(pContext);
    ApplyRS(pContext, KState::g_pCurrentRS);
    ApplyDSS(pContext, KState::g_pCurrentDSS);
    return true;
}

bool KSkyBox::Release()
{
    return true;
}

KSkyBox::KSkyBox()
{
    D3DKMatrixIdentity(&m_matSkyView);
    m_matSkyView._11 = 10.0f;
    m_matSkyView._22 = 10.0f;
    m_matSkyView._33 = 10.0f;
}

KSkyBox::~KSkyBox()
{
}
