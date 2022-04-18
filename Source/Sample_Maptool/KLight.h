#pragma once
#include "KStd.h"
//방향성라이트 : 위치가 없고 방향만 있음
//포인트라이트 : 방향이 없고 위치만 있음
//스포트 라이트 : 사이각을 가짐
//엠비언트라이트 : 주변광원
class KLight
{
public:
    KVector3    m_vPos;
    KVector3    m_vTarget;
    KVector3    m_vDir;
    KVector3    m_vRotation;
    KVector4    m_vLightColor;
public:
    KMatrix     m_matView;
    KMatrix     m_matProj;
    KMatrix     m_matWorld;
public:
    virtual void  SetLight(KVector3 vPos, KVector3 vTarget)
    {
        //색깔 기본
        m_vLightColor = { 0.9f,0.85f,0.85f,1.0f};
        m_vPos = vPos;
        m_vTarget = vTarget;
        m_vDir = m_vTarget - m_vPos;
        D3DXVec3Normalize(&m_vDir, &m_vDir);
        //월드행렬 만들기 ----------------------
        m_matWorld._41 = m_vPos.x;
        m_matWorld._42 = m_vPos.y;
        m_matWorld._43 = m_vPos.z;
        //뷰행렬만들기---------------------------
        KVector3 vUp(0, 1, 0);
        D3DKMatrixLookAtLH(&m_matView, &m_vPos, &m_vTarget, &vUp);
        //원근투영 투영행렬 만들기---------------
        D3DKMatrixPerspectiveFovLH(&m_matProj, XM_PI * 0.45f, 1.0f, 1.0f, 10000.0f);
    }
    virtual bool    Frame()
    {
        KMatrix mRotation;
        D3DKMatrixRotationY(&mRotation, g_fSecPerFrame * 0.1f);
        D3DKMatrixMultiply(&m_matWorld, &m_matWorld, &mRotation);

        m_vPos.x = m_matWorld._41;
        m_vPos.y = m_matWorld._42;
        m_vPos.z = m_matWorld._43;

        //뷰행렬만들기---------------------------
        KVector3 vUp(0, 1, 0);
        D3DKMatrixLookAtLH(&m_matView, &m_vPos, &m_vTarget, &vUp);
        return true;
    }
public:
    KLight()
    {
        D3DKMatrixIdentity(&m_matWorld);
        m_vPos = KVector3(0.0f,10.0f,0.0f);
        m_vTarget = KVector3(0.0f, 0.0f, 0.0f);
        m_vDir = KVector3(0.0f, 1.0f, 0.0f);
        m_vRotation = KVector3(0.0f, 0.0f, 0.0f);
        m_vLightColor= KVector4(1.0f, 1.0f, 1.0f, 1.0f);
    };
    ~KLight()
    {
    };
};
