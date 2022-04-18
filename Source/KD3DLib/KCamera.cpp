#include "KCamera.h"
#include "KInput.h"
#include "ImGuiManager.h"
#pragma region Camera
bool KCamera::ResizeRatio()
{
    float new_Aspect = static_cast<float>(g_rtClient.right) / static_cast<float>(g_rtClient.bottom);
    if (m_fAspect != new_Aspect)
    {
        CreateProjMatrix(m_fNear, m_fFar, m_fFov, new_Aspect);
        return true;
    }
    return false;
}

KMatrix KCamera::OnMouseRotation()
{
    if (g_Input.m_DIMouseState.rgbButtons[1])
    {
        m_fYaw += XMConvertToRadians((g_InputData.iMouseValue[0] * g_fSecPerFrame) * m_fMouseSensitivity);
        m_fPitch += XMConvertToRadians((g_InputData.iMouseValue[1] * g_fSecPerFrame) * m_fMouseSensitivity);
    }
    m_fRoll += 0;
    m_fRadius += 0;
    KQuaternion q;
    //사원수를 행렬로 변환하고 역행렬로 카메라
    D3DXQuaternionRotationYawPitchRoll(&q, m_fYaw, m_fPitch, m_fRoll);
    KMatrix matRotation;
    D3DKMatrixAffineTransformation(&matRotation, 1.0f, NULL, &q, &m_vCameraPos);
    D3DKMatrixInverse(&m_matView, NULL, &matRotation);

    return matRotation;
}
void KCamera::UpdateCamera()
{
    //매 프레임 마다 뷰행렬을 만든다.
    m_vSide.x = m_matView._11;
    m_vSide.y = m_matView._21;
    m_vSide.z = m_matView._31;
    m_vUp.x = m_matView._12;
    m_vUp.y = m_matView._22;
    m_vUp.z = m_matView._32;
    m_vLook.x = m_matView._13;
    m_vLook.y = m_matView._23;
    m_vLook.z = m_matView._33;
    CreateFrustum(m_matView, m_matProj);
}
bool KCamera::Init(ID3D11DeviceContext* pContext)
{
    m_pContext = pContext;
    KFrustum::Init();
    return true;
}
bool KCamera::Frame()
{
    return true;
}
//None
bool KCamera::Render(ID3D11DeviceContext* pContext)
{
    K3DAsset::Render(pContext);
    return true;
}
bool KCamera::Release()
{
    return true;
}
KVector3*   KCamera::GetCameraPos()
{
    return &m_vCameraPos;
}
KVector3* KCamera::GetCameraTarget()
{
    return &m_vCameraTarget;
}
//뷰행렬 만듬
KMatrix   KCamera::CreateViewMatrix(KVector3 vPos, KVector3 vTarget, KVector3 vUp)
{
    m_vCameraPos = vPos;
    m_vCameraTarget = vTarget;
    D3DKMatrixIdentity(&m_matWorld);
    m_matWorld._41 = m_vCameraPos.x;
    m_matWorld._42 = m_vCameraPos.y;
    m_matWorld._43 = m_vCameraPos.z;
    D3DKMatrixLookAtLH(&m_matView, &m_vCameraPos, &m_vCameraTarget, &vUp);
    m_vSide.x = m_matView._11;
    m_vSide.y = m_matView._21;
    m_vSide.z = m_matView._31;

    m_vUp.x = m_matView._12;
    m_vUp.y = m_matView._22;
    m_vUp.z = m_matView._32;

    m_vLook.x = m_matView._13;
    m_vLook.y = m_matView._23;
    m_vLook.z = m_matView._33;

    m_fYaw = m_vLook.x;
    m_fPitch = -m_vLook.y;
    return m_matView;
}
//투영행렬 만듬
KMatrix     KCamera::CreateProjMatrix(
    float fNear, float fFar, float fFov, float fAspect)
{
    m_fAspect = fAspect;
    m_fFov = fFov;
    m_fNear = fNear;
    m_fFar = fFar;
    D3DKMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
    return m_matProj;
}

KCamera::KCamera()
{
    D3DKMatrixIdentity(&m_matWorld);
    m_vCameraPos = { 0, 0, -10.0f };
    m_vCameraTarget = { 0, 0, 0.0f };
    m_fSpeed = 25.0f;
    m_fMouseSensitivity = 30;
    m_fOriginSpeed = m_fSpeed;
}
KCamera::~KCamera()
{
}
#pragma endregion

#pragma region Camera>>DebugCamera
bool KDebugCamera::Frame()
{
    ResizeRatio();
    UpdateCamera();
    OnMouseRotation();
    if (g_InputData.bWKey)
    {
        m_vCameraPos = m_vCameraPos + m_vLook * m_fSpeed * g_fSecPerFrame;
    }
    if (g_InputData.bSKey)
    {
        m_vCameraPos = m_vCameraPos + m_vLook * -m_fSpeed * g_fSecPerFrame;
    }
    if (g_InputData.bDKey)
    {
        m_vCameraPos = m_vCameraPos + m_vSide * m_fSpeed * g_fSecPerFrame;
        m_vCameraTarget.x += m_fSpeed * g_fSecPerFrame;
    }
    if (g_InputData.bAKey)
    {
        m_vCameraPos = m_vCameraPos + m_vSide * -m_fSpeed * g_fSecPerFrame;
        m_vCameraTarget.x -= m_fSpeed * g_fSecPerFrame;
    }

    g_InputData.bLShift ?
        m_fSpeed += 300.0f * g_fSecPerFrame : m_fSpeed -= 300.0f * g_fSecPerFrame;

    if (m_fSpeed >= 300.0f)
    {
        m_fSpeed = 300.0f;
    }

    if (m_fSpeed <= m_fOriginSpeed)
    {
        m_fSpeed = m_fOriginSpeed;
    }
    m_matWorld._41 = m_vCameraPos.x;
    m_matWorld._42 = m_vCameraPos.y;
    m_matWorld._43 = m_vCameraPos.z;
    return true;
}
#pragma endregion




