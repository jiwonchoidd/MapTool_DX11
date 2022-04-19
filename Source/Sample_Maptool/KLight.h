#pragma once
#include "KStd.h"
//���⼺����Ʈ : ��ġ�� ���� ���⸸ ����
//����Ʈ����Ʈ : ������ ���� ��ġ�� ����
//����Ʈ ����Ʈ : ���̰��� ����
//�����Ʈ����Ʈ : �ֺ�����
class KLight
{
public:
    float				m_fYaw;
    float				m_fPitch;
    float				m_fRoll;
public:
    KVector3    m_vPos;
    KVector3    m_vTarget;
    KVector3    m_vDir;
    KVector3    m_vPreviousDir;
    KVector3    m_vPreviousPos;
    KVector3    m_vRotation;
    KVector4    m_vLightColor;
public:
    KMatrix     m_matView;
    KMatrix     m_matProj;
    KMatrix     m_matWorld;
public:
    virtual void  SetLight(KVector3 vPos, KVector3 vTarget);
    virtual bool    Frame();
    virtual bool    Render();
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
