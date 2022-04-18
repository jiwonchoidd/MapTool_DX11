#pragma once
#include "KBoxObj.h"
class KDebugBoxRenderer
{
public:
	KBoxObj	m_RenderBox;
public:
	void DrawDebugInit(ID3D11DeviceContext* pContext);
	void DrawDebugRender(KBox* pBox, ID3D11DeviceContext* pContext, KVector4 color);
	void Release();
public:
	KDebugBoxRenderer() {};
	~KDebugBoxRenderer() {};
};

