#include "KLight.h"
#include "ImGuiManager.h"
void KLight::SetLight(KVector3 vPos, KVector3 vTarget)
{
    //���� �⺻
    m_vLightColor = { 0.9f,0.85f,0.85f,1.0f };
    m_vPos = vPos;
    m_vTarget = vTarget;
    m_vDir = m_vTarget - m_vPos;
    D3DXVec3Normalize(&m_vDir, &m_vDir);
    //������� ����� ----------------------
    m_matWorld._41 = m_vPos.x;
    m_matWorld._42 = m_vPos.y;
    m_matWorld._43 = m_vPos.z;
    //����ĸ����---------------------------
    KVector3 vUp(0, 1, 0);
    D3DKMatrixLookAtLH(&m_matView, &m_vPos, &m_vTarget, &vUp);
    //�������� ������� �����---------------
    D3DKMatrixPerspectiveFovLH(&m_matProj, XM_PI * 0.45f, 1.0f, 1.0f, 10000.0f);

}

bool KLight::Frame()
{
    if (m_vPreviousDir != m_vDir)
    {
        KQuaternion q;
        float angle = 0.0f;
        KVector3 vUp(0, 1, 0);
        KVector3 vPlane(0, 0, 1);
        KVector3 vRight;
        vRight = m_vDir.Cross(vUp);
        angle = acosf(vRight.x * vPlane.x + vRight.y * vPlane.y + vRight.z * vPlane.z);
        D3DXQuaternionRotationAxis(&q, &m_vDir, angle * 180.0f/ XM_PI);
        KMatrix matRotation;
        D3DKMatrixAffineTransformation(&matRotation, 1.0f, NULL, &q, &m_vPos);
        D3DKMatrixInverse(&m_matWorld, NULL, &matRotation);
    }
    m_vPos.x = m_matWorld._41;
    m_vPos.y = m_matWorld._42;
    m_vPos.z = m_matWorld._43;
    

    //����ĸ����---------------------------
    KVector3 vUp(0, 1, 0);
    D3DKMatrixLookAtLH(&m_matView, &m_vPos, &m_vTarget, &vUp);

    //����ķ� YAW PITCH ROLL ���� ���� �� ����
    m_fYaw = atan2(m_matView._31, m_matView._33);
    m_fPitch = asin(-m_matView._32);
    m_fRoll = atan2(m_matView._11, m_matView._12);

    m_vPreviousDir = m_vDir;
    return true;
}

bool KLight::Render()
{
    return true;
}
