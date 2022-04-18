#include "KBoxObj.h"

bool KBoxObj::Init(std::wstring vsfile, std::wstring psfile, std::wstring texturefile,
	std::wstring specularfile, std::wstring normalfile)
{
	const KVector3 vertices[] =
	{
		{-1.0f,-1.0f,-1.0f},
		{ 1.0f,-1.0f,-1.0f},
		{-1.0f,1.0f,-1.0f},
		{ 1.0f,1.0f,-1.0f},//Á¤¸é3
		{1.0f,-1.0f,1.0f},
		{ -1.0f,-1.0f,1.0f},
		{1.0f,1.0f,1.0f},
		{-1.0f,1.0f,1.0f},//µÞ¸é7
		{1.0f,-1.0f,-1.0f},
		{ 1.0f,-1.0f,1.0f},
		{1.0f,1.0f,-1.0f},
		{ 1.0f,1.0f,1.0f},//¿À¸¥ÂÊ¸é11
		{-1.0f,-1.0f,1.0f},
		{ -1.0f,-1.0f,-1.0f},
		{-1.0f,1.0f,1.0f},
		{ -1.0f,1.0f,-1.0f},//¿ÞÂÊ¸é15
		{-1.0f,1.0f,-1.0f},
		{ 1.0f,1.0f,-1.0f},
		{-1.0f,1.0f,1.0f},
		{ 1.0f,1.0f,1.0f},//À­ÂÊ¸é19
		{-1.0f,-1.0f,1.0f},
		{ 1.0f,-1.0f,1.0f},
		{-1.0f,-1.0f,-1.0f},
		{ 1.0f,-1.0f,-1.0f},//¾Æ·§¸é23
	};
	const KVector3 normal[] =
	{
		{0.0f,0.0f,-1.0f},
		{ 0.0f,0.0f,-1.0f},
		{0.0f,0.0f,-1.0f},
		{ 0.0f,0.0f,-1.0f},//Á¤¸é3
		{0.0f,0.0f,1.0f},
		{ 0.0f,0.0f,1.0f},
		{0.0f,0.0f,1.0f},
		{0.0f,0.0f,1.0f},//µÞ¸é7
		{1.0f,0.0f,0.0f},
		{ 1.0f,0.0f,0.0f},
		{1.0f,0.0f,0.0f},
		{ 1.0f,0.0f,0.0f},//¿À¸¥ÂÊ¸é11
		{-1.0f,0.0f,0.0f},
		{ -1.0f,0.0f,0.0f},
		{-1.0f,0.0f,0.0f},
		{ -1.0f,0.0f,0.0f},//¿ÞÂÊ¸é15
		{0.0f,1.0f,0.0f},
		{ 0.0f,1.0f,0.0f},
		{0.0f,1.0f,0.0f},
		{ 0.0f,1.0f,0.0f},//À­ÂÊ¸é19
		{0.0f,-1.0f,0.0f},
		{ 0.0f,-1.0f,0.0f},
		{0.0f,-1.0f,0.0f},
		{ 0.0f,-1.0f,0.0f},//¾Æ·§¸é23
	};
	const KVector2 uv[] =
	{
		{0.0f,1.0f},
		{1.0f,1.0f},
		{0.0f,0.0f},
		{1.0f,0.0f},//Á¤¸é
		{0.0f,1.0f},
		{1.0f,1.0f},
		{0.0f,0.0f},
		{1.0f,0.0f},//µÞ¸é
		{0.0f,1.0f},
		{1.0f,1.0f},
		{0.0f,0.0f},
		{1.0f,0.0f},//¿À¸¥¸é
		{0.0f,1.0f},
		{1.0f,1.0f},
		{0.0f,0.0f},
		{1.0f,0.0f},//¿Þ¸é
		{0.0f,1.0f},
		{1.0f,1.0f},
		{0.0f,0.0f},
		{1.0f,0.0f},//À­¸é
		{0.0f,1.0f},
		{1.0f,1.0f},
		{0.0f,0.0f},
		{1.0f,0.0f},//¾Æ·§¸é
	};
	const unsigned short indices[] =
	{
		2,1,0,	 2,3,1,//Á¤¸é
		6,5,4,	 6,7,5,//µÞ¸é
		10,9,8,  10,11,9,//¿À¸¥ÂÊ¸é
		14,13,12, 14,15,13,//¿ÞÂÊ¸é
		18,17,16, 18,19,17, //À­ÂÊ¸é
		22,21,20, 22,23,21,//¾Æ·§¸é
		//0,2,1, 2,3,1,
		//1,3,5, 3,7,5,
		//2,6,3, 3,6,7,
		//4,5,7, 4,7,6,
		//0,4,2, 2,4,6,
		//0,1,4, 1,5,4,
	};
	for (int num = 0; num < sizeof(vertices)/sizeof(KVector3); num+=4)
	{
		PNCT_VERTEX pnct1;
		PNCT_VERTEX pnct2;
		PNCT_VERTEX pnct3;
		PNCT_VERTEX pnct4;
		BT_VERTEX bt1;
		BT_VERTEX bt2;
		BT_VERTEX bt3;
		BT_VERTEX bt4;
		pnct1.pos = vertices[num];
		pnct1.tex = uv[num];
		pnct1.color = { 1.0f,1.0f,1.0f,1.0f };

		pnct2.pos = vertices[num+1];
		pnct2.tex = uv[num+1];
		pnct2.color = { 1.0f,1.0f,1.0f,1.0f };

		pnct3.pos = vertices[num+2];
		pnct3.tex = uv[num+2];
		pnct3.color = { 1.0f,1.0f,1.0f,1.0f };

		pnct4.pos = vertices[num + 3];
		pnct4.tex = uv[num + 3];
		pnct4.color = { 1.0f,1.0f,1.0f,1.0f };
		KVector3 t, b, n;
		CreateTangentSpace(&pnct1.pos, &pnct2.pos, &pnct3.pos, 
			&pnct1.tex, &pnct2.tex, &pnct3.tex,&n, &t, &b);
		bt1.tangent = t;
		bt1.binormal = b;
		pnct1.normal = n;
		CreateTangentSpace(&pnct2.pos, &pnct3.pos, &pnct1.pos,
			&pnct2.tex, &pnct3.tex, &pnct1.tex, &n, &t, &b);
		bt2.tangent = t;
		bt2.binormal = b;
		pnct2.normal = n;
		CreateTangentSpace(&pnct3.pos, &pnct1.pos, &pnct2.pos,
			&pnct3.tex, &pnct1.tex, &pnct2.tex, &n, &t, &b);
		bt3.tangent = t;
		bt3.binormal = b;
		pnct3.normal = n;
		CreateTangentSpace(&pnct4.pos, &pnct3.pos, &pnct2.pos,
			&pnct4.tex, &pnct3.tex, &pnct2.tex, &n, &t, &b);
		bt4.tangent = t;
		bt4.binormal = b;
		pnct4.normal = n;
		m_VertexList.push_back(pnct1);
		m_VertexList.push_back(pnct2);
		m_VertexList.push_back(pnct3);
		m_VertexList.push_back(pnct4);
		m_BTList.push_back(bt1);
		m_BTList.push_back(bt2);
		m_BTList.push_back(bt3);
		m_BTList.push_back(bt4);
	}

	for (int num = 0; num < sizeof(indices)/ sizeof(short); num++)
	{ 
		m_IndexList.push_back(indices[num]);
	}

	if (CreateObject(vsfile,psfile,texturefile, specularfile, normalfile))
	{
		GenAABB();
		return true;
	}
	return false;
}

bool KBoxObj::Frame()
{
	return true;
}

bool KBoxObj::CreateVertexData()
{
	return true;
}

bool KBoxObj::CreateIndexData()
{
	return true;
}


bool KBoxObj::Render(ID3D11DeviceContext* pContext)
{
	
	KObject::Render(pContext);
	return true;
}

bool KBoxObj::Release()
{
	K3DAsset::Release();
	return true;
}

KBoxObj::KBoxObj()
{
}

KBoxObj::~KBoxObj()
{
}
