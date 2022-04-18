#pragma once
#include "KFbxLoader.h"
//KFBXOBJ을 모아서 트리 구조를 이루는 하나의 FBX 오브젝트
class KFBXAsset : public K3DAsset
{
private:
	bool			m_bAnimPlay = false;
public:
	KFbxLoader*		m_pLoader;
	KFbxLoader*		m_pAnimLoader;
	float			m_fAnimDir = 1.0f;
	float			m_fAnimTime = 0.0f;
	float			m_fAnimSpeed = 1.0f;
	KBoneWorld		m_matBoneArray;
	//애니메이션 보간은 다른 애니메이션 변환시에만 사용함(프레임 저하)
public:
	virtual bool	Init()override;
	virtual bool	Frame()override;
	virtual bool	Render(ID3D11DeviceContext* pContext)override;
	virtual bool    Release() override;
public:
	//애니메이션 SRT 행렬로 분해해서 보간, 단 회전은 쿼터니언 구면보간 SLERP 사용
	bool			PlayAnimation();
	bool			StopAnimation();
	KMatrix			Interpolation(KFbxLoader* pAnimLoader, KFBXObj* pFbxObj, float fTime);
};

