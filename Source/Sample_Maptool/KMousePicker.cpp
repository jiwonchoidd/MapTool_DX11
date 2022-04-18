#include "KMousePicker.h"
#include "KState.h"
#include "KCollision.h"

bool KMousePicker::IntersectTriangle(const KVector3& orig, const KVector3& dir, KVector3& v0, KVector3& v1, KVector3& v2, FLOAT* t, FLOAT* u, FLOAT* v)
{
	// 두개의 엣지 부분 찾기
	KVector3 edge1 = v1 - v0;
	KVector3 edge2 = v2 - v0;

	//결정자
	KVector3 pvec;
	D3DXVec3Cross(&pvec, &dir, &edge2);

	// 삼각형의 접선을 구함
	FLOAT det = D3DXVec3Dot(&edge1, &pvec);

	KVector3 tvec; // 내적이 양수가 될 수 있도록 det 방향을 뒤집는다.
	if (det > 0)
	{
		tvec = orig - v0;
	}
	else
	{
		tvec = v0 - orig;
		det = -det;
	}

	if (det < 0.0001f)
		return false;

	// Calculate U parameter and test bounds
	*u = D3DXVec3Dot(&tvec, &pvec);
	if (*u < 0.0f || *u > det)
		return false;

	// Prepare to test V parameter
	KVector3 qvec;
	D3DXVec3Cross(&qvec, &tvec, &edge1);

	// Calculate V parameter and test bounds
	*v = D3DXVec3Dot(&dir, &qvec);
	if (*v < 0.0f || *u + *v > det)
		return false;

	// Calculate t, scale parameters, ray intersects triangle
	*t = D3DXVec3Dot(&edge2, &qvec);
	FLOAT fInvDet = 1.0f / det;
	*t *= fInvDet;
	*u *= fInvDet;
	*v *= fInvDet;
	return true;
}

bool KMousePicker::Map_HeightControl(float HeightScale, float BrushSize)
{
#pragma region 맵 지형 높낮이

	//브러쉬 박스 안에 있는 노드 검출, Rect to Rect 으로 검출함
	for (int i = 0; i < m_pSpace->m_pDrawableLeafList.size(); i++)
	{
		KNode* pNode = m_pSpace->m_pDrawableLeafList[i];
		KRect node_Rect = KRect(KVector2(pNode->m_node_box.min.x, pNode->m_node_box.min.z), KVector2(pNode->m_node_box.max.x, pNode->m_node_box.max.z));
		KRect brush_Rect = KRect(KVector2(m_Sel_Box.min.x, m_Sel_Box.min.z), KVector2(m_Sel_Box.max.x, m_Sel_Box.max.z));
		int result = KCollision::RectToRect(brush_Rect, node_Rect);
		// 0 :  떨어져 있다.
		// 1 :  중간만 걸쳐 있다.
		// 2 :  완전 안에 들어가있다.	
		if (result == 0)
		{
			//포함되지 않는 노드
			continue;
		}
		else if (result == 1)
		{

		}
		else
		{
			for (int i = 1; i < 20; i++)
			{
				if (i == 5 || i == 10 || i == 15)
					continue;
				KVector3 pos = pNode->m_VertexList[i].pos;
				float newHeight = pos.y + HeightScale * g_fSecPerFrame;
				pNode->m_VertexList[i].pos = KVector3(pos.x, newHeight, pos.z);
				pNode->m_node_box = KBox(KVector3(pNode->m_node_box.min.x, pNode->m_node_box.min.y, pNode->m_node_box.min.z),
					KVector3(pNode->m_node_box.max.x, newHeight, pNode->m_node_box.max.z)); 
			}
			Map_HeightControl_MakeSameHeight(pNode);

		}
	}
	return true;
#pragma endregion
}
//노드 단위 렌더링으로 리프 노드 사이 사이 찢어짐이 발생,
//강제로 같은 높이로 기입하고 업데이트.
bool KMousePicker::Map_HeightControl_MakeSameHeight(KNode* pNode)
{
	KVector3 temp;
	if (pNode == nullptr)
	{
		return false;
	}
	//동쪽 리프노드
	for (int east = 4; east <= 24; east += 5)
	{
		if (pNode->m_pNeighborlist[0] != nullptr)
		{
			temp = pNode->m_VertexList[east].pos;
			pNode->m_pNeighborlist[0]->m_VertexList[east - 4].pos = temp;
			if (east == 4 && pNode->m_pNeighborlist[0]->m_pNeighborlist[3] != nullptr)
			{
				pNode->m_pNeighborlist[0]->m_pNeighborlist[3]->m_VertexList[20].pos = temp;
				m_pContext->UpdateSubresource(
					pNode->m_pNeighborlist[0]->m_pNeighborlist[3]->m_pVertexBuffer.Get(), 0, NULL,
					&pNode->m_pNeighborlist[0]->m_pNeighborlist[3]->m_VertexList.at(0), 0, 0);
			}
			if (east == 24 && pNode->m_pNeighborlist[0]->m_pNeighborlist[2] != nullptr)
			{
				pNode->m_pNeighborlist[0]->m_pNeighborlist[2]->m_VertexList[0].pos = temp;
				m_pContext->UpdateSubresource(
					pNode->m_pNeighborlist[0]->m_pNeighborlist[2]->m_pVertexBuffer.Get(), 0, NULL,
					&pNode->m_pNeighborlist[0]->m_pNeighborlist[2]->m_VertexList.at(0), 0, 0);
			}
		}
	}
	//서
	for (int west = 0; west <= 20; west += 5)
	{
		if (pNode->m_pNeighborlist[1] != nullptr)
		{
			temp = pNode->m_VertexList[west].pos;
			pNode->m_pNeighborlist[1]->m_VertexList[west + 4].pos = temp;
			if (west == 0 && pNode->m_pNeighborlist[1]->m_pNeighborlist[3] != nullptr)
			{
				pNode->m_pNeighborlist[1]->m_pNeighborlist[3]->m_VertexList[24].pos = temp;
				m_pContext->UpdateSubresource(
					pNode->m_pNeighborlist[1]->m_pNeighborlist[3]->m_pVertexBuffer.Get(), 0, NULL,
					&pNode->m_pNeighborlist[1]->m_pNeighborlist[3]->m_VertexList.at(0), 0, 0);
			}
			if (west == 20 && pNode->m_pNeighborlist[1]->m_pNeighborlist[2] != nullptr)
			{
				pNode->m_pNeighborlist[1]->m_pNeighborlist[2]->m_VertexList[4].pos = temp;
				m_pContext->UpdateSubresource(
					pNode->m_pNeighborlist[1]->m_pNeighborlist[2]->m_pVertexBuffer.Get(), 0, NULL,
					&pNode->m_pNeighborlist[1]->m_pNeighborlist[2]->m_VertexList.at(0), 0, 0);
			}
		}
	}
	//남
	for (int south = 20; south <= 24; south += 1)
	{
		if (pNode->m_pNeighborlist[2] != nullptr)
		{
			temp = pNode->m_VertexList[south].pos;
			pNode->m_pNeighborlist[2]->m_VertexList[south - 20].pos = temp;
		}
	}
	//북
	for (int north = 0; north <= 4; north += 1)
	{
		if (pNode->m_pNeighborlist[3] != nullptr)
		{
			temp = pNode->m_VertexList[north].pos;
			pNode->m_pNeighborlist[3]->m_VertexList[north + 20].pos = temp;
		}
	}
	for (int iNei = 0; iNei < 4; iNei++)
	{
		if (pNode->m_pNeighborlist[iNei] != nullptr)
		{
			m_pContext->UpdateSubresource(
				pNode->m_pNeighborlist[iNei]->m_pVertexBuffer.Get(), 0, NULL,
				&pNode->m_pNeighborlist[iNei]->m_VertexList.at(0), 0, 0);
		}
	}
	m_pContext->UpdateSubresource(
		pNode->m_pVertexBuffer.Get(), 0, NULL,
		&pNode->m_VertexList.at(0), 0, 0);
	return true;
}
bool KMousePicker::Init(ID3D11DeviceContext* pContext, KMapSpace* pSpace, KCamera* pCam)
{
	m_pContext = pContext;
	m_pSpace = pSpace;
	m_pCamera = pCam;
	m_vIntersect = KVector3(0, -20, 0);
	m_Sel_Box = KBox(KVector3(-m_Sel_Brush_Size + m_vIntersect.x, -1 * (m_Sel_Brush_Size * 2), -m_Sel_Brush_Size + m_vIntersect.z),
		KVector3(m_Sel_Brush_Size + m_vIntersect.x, m_vIntersect.y + 5, m_Sel_Brush_Size + m_vIntersect.z));
	m_Sel_Box.Axis[0] = KVector3(1, 0, 0);
	m_Sel_Box.Axis[1] = KVector3(0, 1, 0);
	m_Sel_Box.Axis[2] = KVector3(0, 0, 1);

	//선택 박스 디버깅 렌더링 박스로 구현
	m_Sel_BoxRender.DrawDebugInit(pContext);

	return true;
}

bool KMousePicker::Frame()
{
	//마우스 피킹
	//오른쪽 마우스 눌렀을때,
	if (m_iState == C_Height || m_iState == C_Texture)
	{
		//화면 좌표계이기때문에, y를 음수
		//화면 크기
		POINT ptCursor;
		GetCursorPos(&ptCursor);
		ScreenToClient(g_hWnd, &ptCursor);
		KVector3 vView, vProj;

		//Direction 계산
		//현재 아래 계산으로는 미니맵 클릭시 어려움, 무조건 전체 화면 크기 기준으로
		vProj.x = (((2.0f * ptCursor.x - 2.0f * g_rtClient.left) / g_rtClient.right) - 1);
		vProj.y = -(((2.0f * ptCursor.y - 2.0f * g_rtClient.top) / g_rtClient.bottom) - 1);
		vProj.z = 1.0f;
		vView.x = vProj.x / m_pCamera->m_matProj._11;
		vView.y = vProj.y / m_pCamera->m_matProj._22;
		vView.z = vProj.z;

		//뷰좌표계에서는 시작은 무조건 0,0,0
		KMatrix matInverse;
		D3DKMatrixInverse(&matInverse, nullptr, &m_pCamera->m_matView);

		//카메라의 월드 좌표의 레이를 만든다.
		TRay ray;
		ray.direction.x = vView.x * matInverse._11 + vView.y * matInverse._21 + vView.z * matInverse._31;
		ray.direction.y = vView.x * matInverse._12 + vView.y * matInverse._22 + vView.z * matInverse._32;
		ray.direction.z = vView.x * matInverse._13 + vView.y * matInverse._23 + vView.z * matInverse._33;

		ray.position.x = matInverse._41;
		ray.position.y = matInverse._42;
		ray.position.z = matInverse._43;
		//정규화
		D3DXVec3Normalize(&ray.direction, &ray.direction);
		KVector3  vStart = ray.position;
		KVector3  vEnd = ray.position + ray.direction * m_pCamera->m_fFar;

		//전체 보여지는 노드 돈다.
		for (int iNode = 0; iNode < m_pSpace->m_pDrawableLeafList.size(); iNode++)
		{
			KNode* pNode = m_pSpace->m_pDrawableLeafList[iNode];

			for (int i = 0; i < m_pSpace->m_IndexList.size(); i += 6)
			{
				KVector3 v0, v1, v2;
				DWORD i0 = m_pSpace->m_IndexList[i + 0];
				DWORD i1 = m_pSpace->m_IndexList[i + 1];
				DWORD i2 = m_pSpace->m_IndexList[i + 2];
				v0 = pNode->m_VertexList[i0].pos;
				v1 = pNode->m_VertexList[i1].pos;
				v2 = pNode->m_VertexList[i2].pos;

				//2번 교점
				float t, u, v;
			
				if (IntersectTriangle(ray.position, ray.direction,
					v0, v1, v2, &t, &u, &v))
				{
					m_vIntersect = ray.position + ray.direction * t;
 					
					if (g_InputData.bMouseState[0] && !m_bImgui)
					{
						switch (m_iState)
						{
						case 0: {

						}break;
						case 1: {
							Map_HeightControl(m_Sel_Brush_Strenght, m_Sel_Brush_Size);
						}break;
						case 2: {

						}break;
						default:
							break;
						}

					}
				}
				
			}
		}
	}
	m_Sel_Box = KBox(KVector3(-m_Sel_Brush_Size + m_vIntersect.x, -1 * (m_Sel_Brush_Size * 2), -m_Sel_Brush_Size + m_vIntersect.z),
		KVector3(m_Sel_Brush_Size + m_vIntersect.x, m_vIntersect.y + 5, m_Sel_Brush_Size + m_vIntersect.z));

	//m_bImgui = false;
	return true;
}

bool KMousePicker::Render(ID3D11DeviceContext* pContext)
{
	//m_Sel_BoxRender.m_RenderBox.m_matWorld._41 = m_vIntersect.x;
	//m_Sel_BoxRender.m_RenderBox.m_matWorld._42 = m_vIntersect.y;
	//m_Sel_BoxRender.m_RenderBox.m_matWorld._43 = m_vIntersect.z;
	if (g_InputData.bMouseState[0])
	{
		m_Sel_BoxRender.DrawDebugRender(&m_Sel_Box, pContext, KVector4(0.5f, 0, 0.5f, 0.3f));
	}
	else
	{
		m_Sel_BoxRender.DrawDebugRender(&m_Sel_Box,pContext, KVector4(0,0,0.5f,0.3f));
	}
	return true;
}


bool KMousePicker::Release()
{
	m_Sel_BoxRender.m_RenderBox.Release();
	return true;
}


KMousePicker::KMousePicker()
{
}

KMousePicker::~KMousePicker()
{
}
