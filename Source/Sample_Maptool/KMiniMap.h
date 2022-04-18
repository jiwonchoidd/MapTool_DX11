#pragma once
#include "KPlaneObj.h"
#include "KRenderTarget.h"
class KMiniMap : public KPlaneObj
{
public:
	KRect			m_rtMiniMap;
	KRenderTarget	m_Rt;
public:
	bool Init(KRect pos);
	bool Render(ID3D11DeviceContext* m_pContext);
public:
	bool CreateVertexData() override;
	bool CreateIndexData() override;
public:
	bool Release();
};

