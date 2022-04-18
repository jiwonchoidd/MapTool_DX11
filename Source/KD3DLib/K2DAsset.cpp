#include "K2DAsset.h"
#include "KObjectManager.h"
void K2DAsset::RegisterOverlap()
{
	g_ObjManager.AddCollisionExecute(this,
		std::bind(&KCollider::ObjectOverlap, this,
			std::placeholders::_1,
			std::placeholders::_2));
	g_ObjManager.AddSelectExecute(this,
		std::bind(&KCollider::SelectOverlap, this,
			std::placeholders::_1,
			std::placeholders::_2));
}
//오브젝트 충돌콜백
void K2DAsset::ObjectOverlap(KCollider* pObj, DWORD dwState)
{
	if (dwState == KCollisionType::Overlap)
	{
		int kkk = 0;
	}
}
//마우스 선택콜백
void K2DAsset::SelectOverlap(KCollider* pObj, DWORD dwState)
{
	if (dwState == KSelectType::Select_Overlap)
	{
		int kkk = 0;
	}
}

void K2DAsset::SetRectSource(RECT rt)
{
	m_rtSource = rt;
}

void K2DAsset::SetRectDraw(RECT rt)
{
	m_rtDraw = rt;
	m_rtSize.width = rt.right;
	m_rtSize.height = rt.bottom;
	m_rtColl = KRect(m_pos, m_rtSize.width, m_rtSize.height);
}

void K2DAsset::UpdateRectDraw(RECT rt)
{
	m_rtSize.width = rt.right;
	m_rtSize.height = rt.bottom;
}

//생성 함수 마스킹 텍스쳐까지 불러옴
//마스킹 텍스쳐 없을 경우 하나의 텍스쳐만 불러옴
bool K2DAsset::CreateObject_Mask(std::wstring vsFile, std::wstring psFile, std::wstring tex, std::wstring mask)
{
	//KUI Model에서 똑같은거 쓰고 있음 수정하면 거기도 수정해야함
	m_rtColl = KRect(m_pos, m_rtSize.width, m_rtSize.height);
	m_matWorld._41 = m_pos.x;
	m_matWorld._42 = m_pos.y;
	
	RegisterOverlap();

	CreateObject(vsFile, psFile, tex, mask);
	return true;
}

void K2DAsset::AddPosition(KVector2 vPos)
{
	m_matWorld._41 += vPos.x;
	m_matWorld._42 += vPos.y;
	m_pos = { m_matWorld._41, m_matWorld._42 };
	m_rtColl = KRect(m_pos, m_rtSize.width, m_rtSize.height);
	SetUVcoord(m_VertexList);
	if (m_pContext != nullptr)
	{
		m_pContext->UpdateSubresource(m_pVertexBuffer.Get(), 0, NULL,
			&m_VertexList.at(0), 0, 0);
	}
}

void K2DAsset::SetPosition(KVector2 vPos)
{
	// 현재위치
	m_pos = vPos;
	m_matWorld._41 = m_pos.x;
	m_matWorld._42 = m_pos.y;

	SetVertexData();
	SetIndexData();
	if (m_pContext != nullptr)
	{
		m_pContext->UpdateSubresource(m_pVertexBuffer.Get(), 0, NULL,
			&m_VertexList.at(0), 0, 0);
	}
}

//현재 위치, 크기, 
//구버젼, 화면 좌표계 기준임.
void K2DAsset::Convert(KVector2 center, float fWidth, float fHeight,
						std::vector<PNCT_VERTEX>& retList)
{
	std::vector<PNCT_VERTEX> list(6);
	float halfWidth = fWidth / 2.0f;
	float halfHeight = fHeight / 2.0f;
	list[0].pos = { center.x - halfWidth, center.y - halfHeight,0.0f};
	list[1].pos = { center.x + halfWidth, center.y - halfHeight,0.0f};
	list[2].pos = { center.x - halfWidth, center.y + halfHeight,0.0f };
	list[3].pos = list[2].pos;
	list[4].pos = list[1].pos;
	list[5].pos = { center.x + halfWidth, center.y + halfHeight, 0.0f};

	Convert(list, retList);
}
//구버젼, 화면 좌표계 기준임.
void K2DAsset::Convert(std::vector<PNCT_VERTEX>& list,
						std::vector<PNCT_VERTEX>& retList)
{
	retList.resize(list.size());
	for (int i = 0; i < list.size(); i++)
	{
		// 0 ~ 800 -> 0 ~ 1 -> -1 ~ +1
		retList[i].pos.x = list[i].pos.x / g_rtClient.right;
		retList[i].pos.y = list[i].pos.y / g_rtClient.bottom;
		// 0 ~ 1 -> -1 ~ +1 :::: -1 ~ +1 -> 0 ~ 1
		// x = x * 2 + -1;  ::::  x= x * 0.5f + 0.5f;
		retList[i].pos.x = retList[i].pos.x * 2.0f - 1.0f;
		retList[i].pos.y = -1.0f * (retList[i].pos.y * 2.0f - 1.0f);
	}
	// 91,1, 42, 56 => 0 ~ 1
	m_rtSource.left = 0; m_rtSource.right = 0;
	m_rtSource.top = 0; m_rtSource.bottom = 0;
	if (m_rtSource.left == 0 && m_rtSource.right == 0 &&
		m_rtSource.top == 0 && m_rtSource.bottom == 0)
	{
		retList[0].tex.x = 0.0f; retList[0].tex.y = 0.0f; // v0
		retList[1].tex.x = 1.0f; retList[1].tex.y = 0.0f; // v1
		retList[2].tex.x = 0.0f; retList[2].tex.y = 1.0f;
		retList[3].tex = retList[2].tex;
		retList[4].tex = retList[1].tex;
		retList[5].tex.x = 1.0f; retList[5].tex.y = 1.0f;
	}
	else
	{
		float u = m_rtSource.left / (float)m_pTexture_Diffuse->m_TextureDesc.Width;
		float pos = m_rtSource.top / (float)m_pTexture_Diffuse->m_TextureDesc.Height;
		float w = m_rtSource.right / (float)m_pTexture_Diffuse->m_TextureDesc.Width;
		float h = m_rtSource.bottom / (float)m_pTexture_Diffuse->m_TextureDesc.Height;
		retList[0].tex.x = u; retList[0].tex.y = pos; // v0
		retList[1].tex.x = u + w; retList[1].tex.y = pos; // v1
		retList[2].tex.x = u; retList[2].tex.y = pos + h;
		retList[3].tex = retList[2].tex;
		retList[4].tex = retList[1].tex;
		retList[5].tex.x = u + w; retList[5].tex.y = pos + h;
	}
}
//화면좌표계-> 데카르트 좌표계로 변경했음
void K2DAsset::ConvertIndex(KVector2 center, float fWidth, float fHeight, std::vector<PNCT_VERTEX>& retList)
{
	std::vector<PNCT_VERTEX> list(4);
	float halfWidth = fWidth / 2.0f;
	float halfHeight = fHeight / 2.0f;
	list[0].pos = { center.x - halfWidth, center.y + halfHeight };
	list[1].pos = { center.x + halfWidth, center.y + halfHeight };
	list[2].pos = { center.x - halfWidth, center.y - halfHeight };
	list[3].pos = { center.x + halfWidth, center.y - halfHeight };
	ConvertIndex(list, retList);
}
//화면좌표계-> 데카르트 좌표계로 변경했음
void K2DAsset::ConvertIndex(std::vector<PNCT_VERTEX>& list, std::vector<PNCT_VERTEX>& retList)
{
	retList.resize(list.size());
	for (int i = 0; i < list.size(); i++)
	{
		retList[i].pos.x = list[i].pos.x;
		retList[i].pos.y = list[i].pos.y;
	}
	SetUVcoord(retList);
}

void K2DAsset::SetUVcoord(std::vector<PNCT_VERTEX>& retList)
{
	if (m_rtSource.left == 0 && m_rtSource.right == 0 &&
		m_rtSource.top == 0 && m_rtSource.bottom == 0)
	{
		retList[0].tex.x = 0.0f; retList[0].tex.y = 0.0f; // v0
		retList[1].tex.x = 1.0f; retList[1].tex.y = 0.0f; // v1
		retList[2].tex.x = 0.0f; retList[2].tex.y = 1.0f;
		retList[3].tex.x = 1.0f; retList[3].tex.y = 1.0f;
	}
	else
	{
		float u = m_rtSource.left / (float)m_TextureDesc.Width;
		float pos = m_rtSource.top / (float)m_TextureDesc.Height;
		float w = m_rtSource.right / (float)m_TextureDesc.Width;
		float h = m_rtSource.bottom / (float)m_TextureDesc.Height;
		retList[0].tex.x = u; retList[0].tex.y = pos; // v0
		retList[1].tex.x = u + w; retList[1].tex.y = pos; // v1
		retList[2].tex.x = u; retList[2].tex.y = pos + h;
		retList[3].tex.x = u + w; retList[3].tex.y = pos + h;
	}
}

bool K2DAsset::SetVertexData()
{
	ConvertIndex(m_pos, m_rtSize.width, m_rtSize.height, m_VertexList);
	m_matWorld._41 = m_pos.x;
	m_matWorld._42 = m_pos.y;
	return true;
}

bool K2DAsset::SetIndexData()
{
	m_IndexList.clear();
	m_IndexList.push_back(0); m_IndexList.push_back(1); m_IndexList.push_back(2);
	m_IndexList.push_back(2); m_IndexList.push_back(1); m_IndexList.push_back(3);
	return true;
}

bool K2DAsset::CheckVertexData()
{
	SetVertexData();
	SetIndexData();
	return true;
}

K2DAsset::K2DAsset()
{
	m_Speed = 2.0f;
	m_rtSource.left = 0; m_rtSource.right = 0;
	m_rtSource.top = 0; m_rtSource.bottom = 0;
	m_rtDraw.left = 0; m_rtDraw.right = g_rtClient.right;
	m_rtDraw.top = 0; m_rtDraw.bottom = g_rtClient.bottom;
}
//소멸자에서 콜라이더 삭제
K2DAsset::~K2DAsset()
{
	g_ObjManager.DeleteExecute(this,
		std::bind(&KCollider::ObjectOverlap, this,
			std::placeholders::_1,
			std::placeholders::_2));
	g_ObjManager.DeleteSelectExecute(this,
		std::bind(&KCollider::SelectOverlap, this,
			std::placeholders::_1,
			std::placeholders::_2));
}