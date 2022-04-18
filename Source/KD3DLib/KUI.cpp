#include "KUI.h"
#include "ImGuiManager.h"
#pragma region 기본 UI
bool KUI::Init(ID3D11DeviceContext* context, std::wstring vs, std::wstring ps, std::wstring tex, std::wstring mask)
{
	m_pContext = context;
	//m_cbData.vLightDir = { 1,1,1,1};
	K2DAsset::CreateObject_Mask(vs, ps, tex, mask);
	return true;
}

bool KUI::Frame()
{
	KObject::Frame();
	return true;
}

bool KUI::Render(ID3D11DeviceContext* pContext)
{
	KObject::Render(pContext);
	return true;
}

// 9등분, 한 이미지를 안깨지게 활용하기 위함
bool KUI::SetVertexData()
{
	if (m_rtSource.left != 0)
	{
		K2DAsset::SetVertexData();
		return true;
	}
	KVector2 left_top = { (float)m_rtOffset.left,  (float)m_rtOffset.top };
	KVector2 right_top ={ (float)m_rtOffset.right, (float)m_rtOffset.top };
	KVector2 left_bottom = { (float)m_rtOffset.left, (float)m_rtOffset.bottom };
	KVector2 right_bottom = { (float)m_rtOffset.right, (float)m_rtOffset.bottom };
	float width = 0.0f;
	float height = 0.0f;
	if (m_pTexture_Diffuse != nullptr)
	{
		width = m_pTexture_Diffuse->m_TextureDesc.Width;
		height = m_pTexture_Diffuse->m_TextureDesc.Height;
	}
	else
	{
		width = m_rtSize.width;
		height = m_rtSize.height;
	}
	KVector2 tLT = { left_top.x / width, left_top.y / height };
	KVector2 tRT = { right_top.x / width, right_top.y / height };
	KVector2 tLB = { left_bottom.x / width, left_bottom.y / height };
	KVector2 tRB = { right_top.x / width, right_top.y / height };


	std::vector<PNCT_VERTEX> list(16);
	float halfWidth = m_rtSize.width / 2.0f;
	float halfHeight = m_rtSize.height / 2.0f;
	list[0].pos = { m_pos.x - halfWidth, m_pos.y - halfHeight };
	list[0].tex = { (float)m_rtOffsetTex.left,(float)m_rtOffsetTex.top};
	list[3].pos = { m_pos.x + halfWidth, m_pos.y - halfHeight };
	list[3].tex = { (float)m_rtOffsetTex.right,(float)m_rtOffsetTex.top };
	list[12].pos = { m_pos.x - halfWidth, m_pos.y + halfHeight };
	list[12].tex = { (float)m_rtOffsetTex.left,(float)m_rtOffsetTex.bottom };
	list[15].pos = { m_pos.x + halfWidth, m_pos.y + halfHeight };
	list[15].tex = { (float)m_rtOffsetTex.right,(float)m_rtOffsetTex.bottom };
	list[5].pos = { list[0].pos.x + left_top.x, list[0].pos.y + left_top.y };
	list[5].tex = { list[0].tex.x + tLT.x, list[0].tex.y + tLT.y };
	list[6].pos = { list[3].pos.x - right_top.x, list[3].pos.y + right_top.y };
	list[6].tex = { list[3].tex.x - tRT.x, list[3].tex.y + tRT.y };
	list[9].pos = { list[12].pos.x + left_bottom.x, list[12].pos.y - left_bottom.y };
	list[9].tex = { list[12].tex.x + tLB.x, list[12].tex.y - tLB.y };
	list[10].pos = { list[15].pos.x - right_bottom.x, list[15].pos.y - right_bottom.y };
	list[10].tex = { list[15].tex.x - tRB.x, list[15].tex.y - tRB.y };

	list[1].pos = { list[5].pos.x, list[0].pos.y };
	list[1].tex = { list[5].tex.x, list[0].tex.y };
	list[2].pos = { list[6].pos.x, list[3].pos.y };
	list[2].tex = { list[6].tex.x, list[3].tex.y };

	list[4].pos = { list[0].pos.x, list[5].pos.y };
	list[4].tex = { list[0].tex.x, list[5].tex.y };
	list[7].pos = { list[3].pos.x, list[6].pos.y };
	list[7].tex = { list[3].tex.x, list[6].tex.y };

	list[8].pos = { list[12].pos.x, list[9].pos.y };
	list[8].tex = { list[12].tex.x, list[9].tex.y };
	list[11].pos = { list[15].pos.x, list[10].pos.y };
	list[11].tex = { list[15].tex.x, list[10].tex.y };

	list[13].pos = { list[9].pos.x, list[12].pos.y };
	list[13].tex = { list[9].tex.x, list[12].tex.y };
	list[14].pos = { list[10].pos.x, list[15].pos.y };
	list[14].tex = { list[10].tex.x, list[15].tex.y };

	for (int i = 0; i < list.size(); i++)
	{
		list[i].pos.x = list[i].pos.x / g_rtClient.right;
		list[i].pos.y = list[i].pos.y / g_rtClient.bottom;
		list[i].pos.x = list[i].pos.x * 2.0f - 1.0f;
		list[i].pos.y = -1.0f * (list[i].pos.y * 2.0f - 1.0f);
	}
	m_VertexList = list;
	return true;
}

bool KUI::SetIndexData()
{
	if (m_rtSource.left != 0)
	{
		K2DAsset::SetIndexData();
		return true;
	}
	m_IndexList.push_back(0); m_IndexList.push_back(1); m_IndexList.push_back(4);
	m_IndexList.push_back(4); m_IndexList.push_back(1); m_IndexList.push_back(5);
	m_IndexList.push_back(1); m_IndexList.push_back(2); m_IndexList.push_back(5);
	m_IndexList.push_back(5); m_IndexList.push_back(2); m_IndexList.push_back(6);
	m_IndexList.push_back(2); m_IndexList.push_back(3); m_IndexList.push_back(6);
	m_IndexList.push_back(6); m_IndexList.push_back(3); m_IndexList.push_back(7);

	m_IndexList.push_back(4); m_IndexList.push_back(5); m_IndexList.push_back(8);
	m_IndexList.push_back(8); m_IndexList.push_back(5); m_IndexList.push_back(9);
	m_IndexList.push_back(5); m_IndexList.push_back(6); m_IndexList.push_back(9);
	m_IndexList.push_back(9); m_IndexList.push_back(6); m_IndexList.push_back(10);
	m_IndexList.push_back(6); m_IndexList.push_back(7); m_IndexList.push_back(10);
	m_IndexList.push_back(10); m_IndexList.push_back(7); m_IndexList.push_back(11);


	m_IndexList.push_back(8); m_IndexList.push_back(9); m_IndexList.push_back(12);
	m_IndexList.push_back(12); m_IndexList.push_back(9); m_IndexList.push_back(13);
	m_IndexList.push_back(9); m_IndexList.push_back(10); m_IndexList.push_back(13);
	m_IndexList.push_back(13); m_IndexList.push_back(10); m_IndexList.push_back(14);
	m_IndexList.push_back(10); m_IndexList.push_back(11); m_IndexList.push_back(14);
	m_IndexList.push_back(14); m_IndexList.push_back(11); m_IndexList.push_back(15);
	return true;
}

KUI::KUI()
{
	m_rtOffsetTex.left = 0;
	m_rtOffsetTex.top = 0;
	m_rtOffsetTex.right = 1;
	m_rtOffsetTex.bottom = 1;
}

KUI::~KUI()
{
}
#pragma endregion

#pragma region 이미지
bool KImage::Frame()
{
	if (m_bFadeIn)	FadeIn();
	if (m_bFadeOut)	FadeOut();
	if (m_bMoveImg) MoveIMG();
	//m_cbData.vLightDir.x = m_fAlpha;
	//m_cbData.vLightDir.y = m_fAlpha;
	//m_cbData.vLightDir.z = m_fAlpha;
	//m_cbData.vLightDir.w = 1.0f;
	return true;
}

void KImage::FadeIn()
{
	m_fAlpha += g_fSecPerFrame;
	m_fAlpha = min(m_fAlpha, 1.0f);
	if (m_fAlpha >= 1.0f)
	{
		m_bFadeIn = false;
	}
}

void KImage::FadeOut()
{
	m_fAlpha -= g_fSecPerFrame*0.5f;
	m_fAlpha = max(m_fAlpha, 0.0f);
	if (m_fAlpha <= 0.0f)
	{
		m_bFadeOut = false;
	}
}
void KImage::MoveIMG()
{
	if (m_cbDataEX.vValue.x > 1.5f)
	{
		m_bMoveImg = false;
		m_Timer = 0.0f;
	}
	else
	{
		m_cbDataEX.vValue.x += g_fSecPerFrame;
	}
}

void KImage::SelectOverlap(KCollider* pObj, DWORD dwState)
{
	if (m_bVisibility && m_SelectType != Select_Ignore)
	{
		if (m_PreSelectState == m_SelectState)
		{
			return;
		}
		switch (m_SelectState)
		{

		case KSelectState::S_DEFAULT:
		{
			m_bImgHover = false;
		}break;
		case KSelectState::S_SELECTED:
		{
		}break;
		case KSelectState::S_HOVER:
		{
			m_bImgHover = true;
		}break;
		case KSelectState::S_ACTIVE:
		{
		}break;
		case KSelectState::S_FOCUS:
		{
		}break;
		default:
		{
		}
		m_PreSelectState = m_SelectState;
		}

	}
}
//버튼 클릭
void KButton::SelectOverlap(KCollider* pObj, DWORD dwState)
{
	if (m_bVisibility && m_SelectType!=Select_Ignore)
	{
		
	}
}

bool KButton::Frame()
{

	if (m_bVisibility && m_SelectType != Select_Ignore)
	{
		if (m_PreSelectState == m_SelectState)
		{
			return false;
		}
		if (m_datalist.size() <= 0) return false;

		switch (m_SelectState)
		{
		case KSelectState::S_DEFAULT:
		{
			if (m_datalist[0].pTex != nullptr)
				m_pTexture_Diffuse = m_datalist[0].pTex;
			m_bSelect = false;
		}break;
		case KSelectState::S_SELECTED:
		{
			//클릭 활성화
			m_bSelect = true;
			if (m_datalist[2].pSound != nullptr)
				m_datalist[2].pSound->SoundPlay_Once();
			if (m_datalist[0].pTex != nullptr)
				m_pTexture_Diffuse = m_datalist[0].pTex;
		}break;
		case KSelectState::S_HOVER:
		{
			if (!m_bSelect)
			{
				if (m_datalist[1].pSound != nullptr)
					m_datalist[1].pSound->SoundPlay_Once();
				if (m_datalist[1].pTex != nullptr)
					m_pTexture_Diffuse = m_datalist[1].pTex;

			}
		}break;
		case KSelectState::S_ACTIVE:
		{
			if (m_datalist[0].pTex != nullptr)
				m_pTexture_Diffuse = m_datalist[0].pTex;
		}break;
		case KSelectState::S_FOCUS:
		{
			if (m_datalist[0].pTex != nullptr)
				m_pTexture_Diffuse = m_datalist[0].pTex;
		}break;
		default:
		{
			m_bSelect = false;
		}
		}
		m_PreSelectState = m_SelectState;
	}

	return true;
}

bool KListCtrlObject::Create(int xCount, int yCount , std::wstring ui_name)
{
	UpdateData();

	int iHalfWidth = m_rtSize.width / xCount;
	int iHalfHeight = m_rtSize.height/ yCount;
	KVector2 pStart = { (float)m_rtDraw.left, (float)m_rtDraw.top };
	pStart.x += m_rtSize.width / xCount / 2.0f;
	pStart.y += m_rtSize.height / yCount / 2.0f;
	for (int iCol = 0; iCol < xCount; iCol++)
	{
		for (int iRow = 0; iRow < yCount; iRow++)
		{
			KUIModel* pNewBtn = g_UIModelManager.GetPtr(ui_name)->Clone();
			pNewBtn->m_pParent = this;
			pNewBtn->m_Name = L"Btn";
			pNewBtn->m_Name += std::to_wstring(iRow * yCount + iCol);
			pNewBtn->SetRectDraw({ 0,0, iHalfWidth,iHalfHeight });
			pNewBtn->SetPosition(KVector2(
				pStart.x + iHalfWidth * iCol,
				pStart.y + iHalfHeight * iRow));
			pNewBtn->UpdateData();
			Add(pNewBtn);
		}
	}
	return true;
}

void KListCtrlObject::SelectOverlap(KCollider* pObj, DWORD dwState)
{
	if (m_PreSelectState == m_SelectState)
	{
		return;
	}
	m_PreSelectState = m_SelectState;
}
