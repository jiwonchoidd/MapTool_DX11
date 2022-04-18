#pragma once
#include "KFrustum.h"
class KCamera : public KFrustum
{
public:
	float				m_fSpeed;
	float				m_fOriginSpeed;
	float				m_fAspect;
	float				m_fFov;
	float				m_fNear;
	float				m_fFar;
public:
	float				m_fMouseSensitivity; //°¨µµ
	KVector3			m_vCameraPos;
	KVector3			m_vCameraTarget;
	KVector3			m_vLook;
	KVector3			m_vSide;
	KVector3			m_vUp;
public:
	float				m_fYaw;
	float				m_fPitch;
	float				m_fRoll;
	float				m_fRadius;
public:
	KMatrix				m_matWorld;
	KMatrix				m_matView;
	KMatrix				m_matProj;
public:
	KMatrix				OnMouseRotation();
	KVector3*			GetCameraTarget();
	KVector3*			GetCameraPos();
	virtual KMatrix     CreateViewMatrix(KVector3 vPos, KVector3 vTarget, KVector3 vUp= KVector3(0,1,0));
	virtual KMatrix  	CreateProjMatrix(float fNear, float fFar, float fFov, float fAspect);
	void				UpdateCamera();
public:
	virtual	bool		ResizeRatio();
	virtual bool		Init(ID3D11DeviceContext* pContext);
	virtual bool		Frame();
	virtual bool		Render(ID3D11DeviceContext* pContext);
	virtual bool		Release();
	
public:
	KCamera();
	virtual ~KCamera();
};

class KDebugCamera : public KCamera
{
public:
	virtual bool		Frame();
};

