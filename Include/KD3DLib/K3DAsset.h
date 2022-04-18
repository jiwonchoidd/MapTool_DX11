#pragma once
#include "KCollider.h"
class K3DAsset : public KObject
{
public:
	std::wstring m_ObjName;
	KBox		 m_BoxCollision;
public:
	KVector3	m_pos;
	KVector3	m_rot;
	KVector3	m_dir;
	KVector3	m_vRight;
	KVector3	m_vUp;
	KVector3	m_vLook;
public:
	virtual void		SetMatrix(KMatrix* matWorld,
		KMatrix* matView, KMatrix* matProj)override;
	virtual void		AddPosition(KVector3 vPos);
	virtual void		SetPosition(KVector3 vPos);
	virtual void		SetRotation(KVector3 vRot);
	// 화면좌표 위치를 중점으로 NDC 변환
	virtual void	Convert(
		KVector2 center, float fWidth, float fHeight,
		std::vector<PNCT_VERTEX>& retList);
	// 화면좌표계를 NDC 변환
	virtual void	Convert(std::vector<PNCT_VERTEX>& list,
		std::vector<PNCT_VERTEX>& retList);
	virtual void		SetUVcoord(std::vector<PNCT_VERTEX>& retList);
public:
	virtual void		UpdateData();
	virtual void		UpdateCollision();
public:
	virtual void        GenAABB();
public:
	virtual bool		CreateVertexData()override;
	virtual bool		CreateIndexData()override;
	virtual bool		CreateTangentSpace(KVector3* v1, KVector3* v2, KVector3* v3, KVector2* uv1,
		KVector2* uv2, KVector2* uv3, KVector3* normal, KVector3* tangent, KVector3* binormal);
public:
	K3DAsset();
	~K3DAsset();
};

