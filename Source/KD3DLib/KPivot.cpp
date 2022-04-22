#include "KPivot.h"
#include "KState.h"
bool KPivot::Init(ID3D11DeviceContext* pContext)
{
	//x, y, z
	m_BoxAxis[0] = KBox(KVector3(0, 0, 0), KVector3(10000, 1, 1));
	m_BoxAxis[1] = KBox(KVector3(0, 0, 0), KVector3(1, 10000, 1));
	m_BoxAxis[2] = KBox(KVector3(0, 0, 0), KVector3(1, 1, 10000));

	m_BoxAxis[0].Axis[0] = KVector3(1, 0, 0);
	m_BoxAxis[0].Axis[1] = KVector3(0, 1, 0);
	m_BoxAxis[0].Axis[2] = KVector3(0, 0, 1);

	m_BoxAxis[1].Axis[0] = KVector3(1, 0, 0);
	m_BoxAxis[1].Axis[1] = KVector3(0, 1, 0);
	m_BoxAxis[1].Axis[2] = KVector3(0, 0, 1);

	m_BoxAxis[2].Axis[0] = KVector3(1, 0, 0);
	m_BoxAxis[2].Axis[1] = KVector3(0, 1, 0);
	m_BoxAxis[2].Axis[2] = KVector3(0, 0, 1);


	m_PivotBox.DrawDebugInit(pContext);
	return true;
}

bool KPivot::Frame()
{
	return false;
}

bool KPivot::Render(ID3D11DeviceContext* pContext)
{
	for (int i = 0; i < 3; i++)
	{
		KVector4 color;
		if (i == 0)color = { 1, 0, 0, 1 };
		if (i == 1)color = { 0, 1, 0, 1 };
		if (i == 2)color = { 0, 0, 1, 1 };
		m_PivotBox.DrawDebugRender(&m_BoxAxis[i], pContext, color);
	}
	return true;
}

bool KPivot::Release()
{
	m_PivotBox.Release();
	return true;
}


KPivot::KPivot()
{
}

KPivot::~KPivot()
{
}
