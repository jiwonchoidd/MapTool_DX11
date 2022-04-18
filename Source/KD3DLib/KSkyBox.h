#pragma once
#include "KBoxObj.h"
class KSkyBox : public KBoxObj
{
public:
	KTexture* m_pCubeTex;
	KMatrix	  m_matSkyView;
public:
	void SetMatrix(KMatrix* pMatWorld, KMatrix* pMatView, KMatrix* pMatProj);
	bool Init(ID3D11DeviceContext* context, std::wstring shader, std::wstring tex = nullptr);
	bool Frame();
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();
public:
	KSkyBox();
	~KSkyBox();
};