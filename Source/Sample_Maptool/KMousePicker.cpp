#include "KMousePicker.h"
#include "KState.h"
#include "KCollision.h"

bool KMousePicker::IntersectTriangle(const KVector3& orig, const KVector3& dir, KVector3& v0, KVector3& v1, KVector3& v2, FLOAT* t, FLOAT* u, FLOAT* v)
{
	// �ΰ��� ���� �κ� ã��
	KVector3 edge1 = v1 - v0;
	KVector3 edge2 = v2 - v0;

	//������
	KVector3 pvec;
	D3DXVec3Cross(&pvec, &dir, &edge2);

	// �ﰢ���� ������ ����
	FLOAT det = D3DXVec3Dot(&edge1, &pvec);

	KVector3 tvec; // ������ ����� �� �� �ֵ��� det ������ �����´�.
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

bool KMousePicker::GetIntersectionBox(TRay ray, KBox box)
{
	KVector3 tmin;
	tmin.x = (box.min.x - ray.position.x) / ray.direction.x + 0.001f;
	tmin.y = (box.min.y - ray.position.y) / ray.direction.y + 0.001f;
	tmin.z = (box.min.z - ray.position.z) / ray.direction.z + 0.001f;

	KVector3 tmax;
	tmax.x = (box.max.x - ray.position.x) / ray.direction.x + 0.001f;
	tmax.y = (box.max.y - ray.position.y) / ray.direction.y + 0.001f;
	tmax.z = (box.max.z - ray.position.z) / ray.direction.z + 0.001f;

	KVector3 real_min;
	real_min.x = min(tmin.x, tmax.x);
	real_min.y = min(tmin.y, tmax.y);
	real_min.z = min(tmin.z, tmax.z);
	KVector3 real_max;
	real_max.x = max(tmin.x, tmax.x);
	real_max.y = max(tmin.y, tmax.y);
	real_max.z = max(tmin.z, tmax.z);

	float minmax = min(min(real_max.x, real_max.y), real_max.z);
	float maxmin = max(max(real_min.x, real_min.y), real_min.z);

	if (minmax >= maxmin)
	{
		m_vIntersect = ray.position + ray.direction * maxmin;
		return true;
	}
	return false;
}

bool KMousePicker::Map_HeightBrushType(KVector3 pos, float& height, float scale)
{
	switch (m_iBrushState)
	{
	case B_UpBrush:
	{
		height = pos.y + scale * g_fSecPerFrame;
	}break;
	case B_DownBrush:
	{
		height = pos.y - scale * g_fSecPerFrame;
	}break;
	case B_SmoothBrush:
	{


	}break;
	case B_FlattenBrush:
	{

	}break;
	default:
		break;
	}
	return true;
}

bool KMousePicker::Map_HeightControl(float HeightScale, float BrushSize)
{
#pragma region �� ���� ������

	//�귯�� �ڽ� �ȿ� �ִ� ��� ����, Rect to Rect ���� ������
	for (int i = 0; i < m_pSpace->m_pDrawableLeafList.size(); i++)
	{
		KNode* pNode = m_pSpace->m_pDrawableLeafList[i];
		KRect node_Rect = KRect(KVector2(pNode->m_node_box.min.x, pNode->m_node_box.min.z), KVector2(pNode->m_node_box.max.x, pNode->m_node_box.max.z));
		KRect brush_Rect = KRect(KVector2(m_Sel_Box.min.x, m_Sel_Box.min.z), KVector2(m_Sel_Box.max.x, m_Sel_Box.max.z));
		int result = KCollision::RectToRect(brush_Rect, node_Rect);
		if (result == 0)
		{
			//���Ե��� �ʴ� ���
			continue;
		}
		else if (result == 1)
		{
			//��ģ ���
		}
		else
		{
			//������ ���Ե� ���
			for (int i = 1; i < 20; i++)
			{
				if (i == 5 || i == 10 || i == 15) // �ڳʴ� ��, �ϸ� �ø�
					continue;
				//float newHeight = pos.y + HeightScale * g_fSecPerFrame;
				KVector3 pos = pNode->m_VertexList[i].pos;
				float newHeight =pos.y;
				Map_HeightBrushType(pos, newHeight, HeightScale);
				if (newHeight > -10)
				{
					pNode->m_VertexList[i].pos = KVector3(pos.x, newHeight, pos.z);
					//ī�޶� �������Ҷ����� �ڽ��� �ٽ� �°� �ٿ������
					//-> ���� : ������� �ڽ��� �����ϸ� �Ǵ��� �˾Ҵµ�, ��Ʈ ��� ���� ����� �����������..
					//-> �ذ� : �ش� ������ �θ� ������ ����Լ��� ȣ����, ���� ���� �� �÷���, �ڽ� -> �θ� �Ž��� �ö�
					m_pSpace->UpdateNodeBoundingBox(pNode, newHeight);

				}
			}
			Map_HeightControl_MakeSameHeight(pNode);
		}
	}
	return true;
#pragma endregion
}
//��� ���� ���������� ���� ��� ���� ���� �������� �߻�,
//������ ���� ���̷� �����ϰ� ������Ʈ.
bool KMousePicker::Map_HeightControl_MakeSameHeight(KNode* pNode)
{
	KVector3 temp;
	if (pNode == nullptr)
	{
		return false;
	}
	//���� �������
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
	//��
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
	//��
	for (int south = 20; south <= 24; south += 1)
	{
		if (pNode->m_pNeighborlist[2] != nullptr)
		{
			temp = pNode->m_VertexList[south].pos;
			pNode->m_pNeighborlist[2]->m_VertexList[south - 20].pos = temp;
		}
	}
	//��
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

bool KMousePicker::Map_TextureControl(float HeightScale, float BrushSize)
{
	//�귯�� �ڽ� �ȿ� �ִ� ��� ����, Rect to Rect ���� ������
	for (int i = 0; i < m_pSpace->m_pDrawableLeafList.size(); i++)
	{
		KNode* pNode = m_pSpace->m_pDrawableLeafList[i];
		KRect node_Rect = KRect(KVector2(pNode->m_node_box.min.x, pNode->m_node_box.min.z), KVector2(pNode->m_node_box.max.x, pNode->m_node_box.max.z));
		KRect brush_Rect = KRect(KVector2(m_Sel_Box.min.x, m_Sel_Box.min.z), KVector2(m_Sel_Box.max.x, m_Sel_Box.max.z));
		int result = KCollision::RectToRect(brush_Rect, node_Rect);
		// 0 :  ������ �ִ�.
		// 1 :  �߰��� ���� �ִ�.
		// 2 :  ���� �ȿ� ���ִ�.	
		if (result == 0)
		{
			//���Ե��� �ʴ� ���
			continue;
		}
		else if (result == 1)
		{

		}
		else
		{
			//������ ���Ե� ���
			pNode;
		}
	}
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

	//���� �ڽ� ����� ������ �ڽ��� ����
	m_Sel_BoxRender.DrawDebugInit(pContext);

	return true;
}

bool KMousePicker::Frame()
{
	//���콺 ��ŷ
	//������ ���콺 ��������,
	//ȭ�� ��ǥ���̱⶧����, y�� ����
	//ȭ�� ũ��
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(g_hWnd, &ptCursor);
	KVector3 vView, vProj;

	//Direction ���
	//���� �Ʒ� ������δ� �̴ϸ� Ŭ���� �����, ������ ��ü ȭ�� ũ�� ��������
	vProj.x = (((2.0f * ptCursor.x - 2.0f * g_rtClient.left) / g_rtClient.right) - 1);
	vProj.y = -(((2.0f * ptCursor.y - 2.0f * g_rtClient.top) / g_rtClient.bottom) - 1);
	vProj.z = 1.0f;
	vView.x = vProj.x / m_pCamera->m_matProj._11;
	vView.y = vProj.y / m_pCamera->m_matProj._22;
	vView.z = vProj.z;

	//����ǥ�迡���� ������ ������ 0,0,0
	KMatrix matInverse;
	D3DKMatrixInverse(&matInverse, nullptr, &m_pCamera->m_matView);

	//ī�޶��� ���� ��ǥ�� ���̸� �����.
	TRay ray;
	ray.direction.x = vView.x * matInverse._11 + vView.y * matInverse._21 + vView.z * matInverse._31;
	ray.direction.y = vView.x * matInverse._12 + vView.y * matInverse._22 + vView.z * matInverse._32;
	ray.direction.z = vView.x * matInverse._13 + vView.y * matInverse._23 + vView.z * matInverse._33;

	ray.position.x = matInverse._41;
	ray.position.y = matInverse._42;
	ray.position.z = matInverse._43;
	//����ȭ
	D3DXVec3Normalize(&ray.direction, &ray.direction);
	KVector3  vStart = ray.position;
	KVector3  vEnd = ray.position + ray.direction * m_pCamera->m_fFar;


	if (g_InputData.bMouseState[0] && !m_bImgui)
	{
		switch (m_iControlState)
		{
		case C_None: {
			//������Ʈ ���� NONE
			for (auto pObj : m_pSpace->m_ObjectList_Static)
			{
				if (GetIntersectionBox(ray, pObj->obj_box))
				{
					m_pSeletedObj = pObj;
					break;
				}
				else
				{
					m_pSeletedObj = nullptr;
				}
			}
		}break;
		case C_Height: {
			for (int iNode = 0; iNode < m_pSpace->m_pDrawableLeafList.size(); iNode++)
			{
				KNode* pNode = m_pSpace->m_pDrawableLeafList[iNode];
				if (GetIntersectionBox(ray, pNode->m_node_box))
				{
					Map_HeightControl(m_Sel_Brush_Strenght, m_Sel_Brush_Size);
					m_Sel_Box = KBox(KVector3(-m_Sel_Brush_Size + m_vIntersect.x, -1 * (m_Sel_Brush_Size * 2), -m_Sel_Brush_Size + m_vIntersect.z),
						KVector3(m_Sel_Brush_Size + m_vIntersect.x, m_vIntersect.y + 5, m_Sel_Brush_Size + m_vIntersect.z));
				}
			}
		}break;
		case C_Texture: {
			for (int iNode = 0; iNode < m_pSpace->m_pDrawableLeafList.size(); iNode++)
			{
				KNode* pNode = m_pSpace->m_pDrawableLeafList[iNode];
				if (GetIntersectionBox(ray, pNode->m_node_box))
				{
					Map_HeightControl(m_Sel_Brush_Strenght, m_Sel_Brush_Size);
					m_Sel_Box = KBox(KVector3(-m_Sel_Brush_Size + m_vIntersect.x, -1 * (m_Sel_Brush_Size * 2), -m_Sel_Brush_Size + m_vIntersect.z),
						KVector3(m_Sel_Brush_Size + m_vIntersect.x, m_vIntersect.y + 5, m_Sel_Brush_Size + m_vIntersect.z));
				}
			}
		}break;
		default:
			break;
		}

	}

	//m_bImgui = false;
	return true;
}

bool KMousePicker::Render(ID3D11DeviceContext* pContext)
{
	if (m_iControlState != C_None)
	{
		if (g_InputData.bMouseState[0])
		{
			m_Sel_BoxRender.DrawDebugRender(&m_Sel_Box, pContext, KVector4(0.5f, 0, 0.5f, 0.3f));
		}
		else
		{
			m_Sel_BoxRender.DrawDebugRender(&m_Sel_Box, pContext, KVector4(0, 0, 0.5f, 0.3f));
		}
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
