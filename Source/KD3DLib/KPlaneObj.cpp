#include "KPlaneObj.h"

bool KPlaneObj::Init(std::wstring vsfile, std::wstring psfile, std::wstring texturefile)
{
	const KVector3 vertices[] =
	{
		{-1.0f,1.0f,1.0f},
		{ 1.0f,1.0f,1.0f},
		{ -1.0f,-1.0f,1.0f},
		{1.0f,-1.0f,1.0f},
	};
	const KVector2 uv[] =
	{
		{0.0f,0.0f},
		{ 1.0f,0.0f},
		{ 0.0f,1.0f},
		{1.0f,1.0f},
	};
	const unsigned short indexes[] =
	{
		0,1,2,
		2,1,3,
	};

	for (int num = 0; num < sizeof(vertices)/sizeof(KVector3); num++)
	{
		PNCT_VERTEX pnct;
		pnct.pos = vertices[num];
		pnct.color = { 1.0f,1.0f,1.0f,1.0f };
		pnct.tex = uv[num];
		m_VertexList.push_back(pnct);
	}

	for (int num = 0; num < 6; num++)
	{
		m_IndexList.push_back(indexes[num]);
	}

	if (CreateObject(vsfile,
		psfile,
		texturefile))
	{
		return true;
	}

	return false;
}

bool KPlaneObj::Frame()
{
	return true;
}

bool KPlaneObj::Render(ID3D11DeviceContext* pContext)
{
	K3DAsset::Render(pContext);
	return true;
}

bool KPlaneObj::Release()
{
	return true;
}

KPlaneObj::KPlaneObj()
{
}

KPlaneObj::~KPlaneObj()
{
}
