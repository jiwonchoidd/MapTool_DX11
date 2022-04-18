#pragma once
#include "KDebugBoxRenderer.h"
class KPivot
{
public:
	KBox	m_BoxAxis[3];
	KDebugBoxRenderer m_PivotBox;
public:
	bool Init(ID3D11DeviceContext* pContext);
	bool Frame();
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();
public:
	KPivot();
	~KPivot();
};

