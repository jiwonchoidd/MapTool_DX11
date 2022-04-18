#include "KFrustum.h"

bool KFrustum::Init()
{
	//���� �Ʒ����� 
	m_Frustum[0] = KVector3(-1.0f, -1.0f, 0.0f);
	m_Frustum[1] = KVector3(1.0f, -1.0f, 0.0f);
	m_Frustum[2] = KVector3(-1.0f, 1.0f, 0.0f);
	m_Frustum[3] = KVector3(1.0f, 1.0f, 0.0f);

	m_Frustum[4] = KVector3(-1.0f, -1.0f, 1.0f);
	m_Frustum[5] = KVector3(1.0f, -1.0f, 1.0f);
	m_Frustum[6] = KVector3(-1.0f, 1.0f, 1.0f);
	m_Frustum[7] = KVector3(1.0f, 1.0f, 1.0f);

	KBoxObj::Init(L"", L"");

	//���� �ٲٱ�
	int color = 0;

	// ����
	m_VertexList[color].color =		KVector4(1.0f, 1.0f, 1.0f, 0.3f);
	m_VertexList[color += 1].color = KVector4(1.0f, 1.0f, 1.0f, 0.3f);
	m_VertexList[color += 1].color = KVector4(1.0f, 1.0f, 1.0f, 0.3f);
	m_VertexList[color += 1].color = KVector4(1.0f, 1.0f, 1.0f, 0.3f);

	m_VertexList[color += 1].color = KVector4(0.1f, 0.5f, 0.5f, 0.3f);
	m_VertexList[color += 1].color = KVector4(0.1f, 0.5f, 0.5f, 0.3f);
	m_VertexList[color += 1].color = KVector4(0.1f, 0.5f, 0.5f, 0.3f);
	m_VertexList[color += 1].color = KVector4(0.1f, 0.5f, 0.5f, 0.3f);

	m_VertexList[color += 1].color = KVector4(0.1f, 0.0f, 1.0f, 0.3f);//������
	m_VertexList[color += 1].color = KVector4(0.1f, 0.0f, 1.0f, 0.3f);
	m_VertexList[color += 1].color = KVector4(0.1f, 0.0f, 1.0f, 0.3f);
	m_VertexList[color += 1].color = KVector4(0.1f, 0.0f, 1.0f, 0.3f);

	m_VertexList[color += 1].color = KVector4(1.0f, 1.0f, 0.0f, 0.3f);//����
	m_VertexList[color += 1].color = KVector4(1.0f, 1.0f, 0.0f, 0.3f);
	m_VertexList[color += 1].color = KVector4(1.0f, 1.0f, 0.0f, 0.3f);
	m_VertexList[color += 1].color = KVector4(1.0f, 1.0f, 0.0f, 0.3f);

	m_VertexList[color += 1].color = KVector4(0.5f, 0.2f, 0.5f, 0.3f);//����
	m_VertexList[color += 1].color = KVector4(0.5f, 0.2f, 0.5f, 0.3f);
	m_VertexList[color += 1].color = KVector4(0.5f, 0.2f, 0.5f, 0.3f);
	m_VertexList[color += 1].color = KVector4(0.5f, 0.2f, 0.5f, 0.3f);

	m_VertexList[color += 1].color = KVector4(1.0f, 0.0f, 0.0f, 0.3f);//�Ʒ�
	m_VertexList[color += 1].color = KVector4(1.0f, 0.0f, 0.0f, 0.3f);
	m_VertexList[color += 1].color = KVector4(1.0f, 0.0f, 0.0f, 0.3f);
	m_VertexList[color += 1].color = KVector4(1.0f, 0.0f, 0.0f, 0.3f);
	
	return true;
}

//���ؽ� ���۸� �������� ũ�⿡ �°� �����ؼ� ������Ʈ
void KFrustum::CreateFrustum(KMatrix& matView, KMatrix& matProj)
{
	m_Frustum[0] = KVector3(-1.0f, -1.0f, 0.0f);
	m_Frustum[1] = KVector3(1.0f, -1.0f, 0.0f);
	m_Frustum[2] = KVector3(-1.0f, 1.0f, 0.0f);
	m_Frustum[3] = KVector3(1.0f, 1.0f, 0.0f);

	m_Frustum[4] = KVector3(-1.0f, -1.0f, 1.0f);
	m_Frustum[5] = KVector3(1.0f, -1.0f, 1.0f);
	m_Frustum[6] = KVector3(-1.0f, 1.0f, 1.0f);
	m_Frustum[7] = KVector3(1.0f, 1.0f, 1.0f);

	KMatrix matInverse;
	D3DKMatrixMultiply(&matInverse, &matView, &matProj);
	D3DKMatrixInverse(&matInverse, NULL, &matInverse);

	//�������ҿ� �°� ������Ŵ
	for (int iVer = 0; iVer < 8; iVer++)
	{
		D3DXVec3TransformCoord(
			&m_Frustum[iVer], &m_Frustum[iVer], &matInverse);
	}
	if (m_VertexList.size() > 0)
	{
		// ����
		m_VertexList[0].pos = m_Frustum[0];
		m_VertexList[1].pos = m_Frustum[1];
		m_VertexList[2].pos = m_Frustum[2];
		m_VertexList[3].pos = m_Frustum[3];
		// �޸�
		m_VertexList[4].pos = m_Frustum[5];
		m_VertexList[5].pos = m_Frustum[4];
		m_VertexList[6].pos = m_Frustum[7];
		m_VertexList[7].pos = m_Frustum[6];
		// �����ʸ�
		m_VertexList[8].pos = m_Frustum[1];
		m_VertexList[9].pos = m_Frustum[5];
		m_VertexList[10].pos = m_Frustum[3];
		m_VertexList[11].pos = m_Frustum[7];
		// ���ʸ�
		m_VertexList[12].pos = m_Frustum[4];
		m_VertexList[13].pos = m_Frustum[0];
		m_VertexList[14].pos = m_Frustum[6];
		m_VertexList[15].pos = m_Frustum[2];
		// ���ʸ�
		m_VertexList[16].pos = m_Frustum[2];
		m_VertexList[17].pos = m_Frustum[3];
		m_VertexList[18].pos = m_Frustum[6];
		m_VertexList[19].pos = m_Frustum[7];
		// �Ʒ���
		m_VertexList[20].pos = m_Frustum[4];
		m_VertexList[21].pos = m_Frustum[5];
		m_VertexList[22].pos = m_Frustum[0];
		m_VertexList[23].pos = m_Frustum[1];
		m_pContext->UpdateSubresource(
			m_pVertexBuffer.Get(), 0, NULL, &m_VertexList.at(0), 0, 0);
	}
	
	m_Plane[0] = KPlane(m_Frustum[0], m_Frustum[2], m_Frustum[3]);  //-z ����

	m_Plane[1] = KPlane(m_Frustum[5], m_Frustum[7], m_Frustum[6]);	//+z �޸�

	m_Plane[2] = KPlane(m_Frustum[4], m_Frustum[6], m_Frustum[2]);	//-x ����

	m_Plane[3] = KPlane(m_Frustum[1], m_Frustum[3], m_Frustum[7]);	//+x ������

	m_Plane[4] = KPlane(m_Frustum[4], m_Frustum[0], m_Frustum[1]);	//-y �Ʒ���

	m_Plane[5] = KPlane(m_Frustum[2], m_Frustum[6], m_Frustum[7]);	//+y ����

}

BOOL KFrustum::ClassifyPoint(KVector3* v)
{
	for (int i = 0; i < 6; i++)
	{
		float pToc = m_Plane[i].x * v->x +
			m_Plane[i].y * v->y +
			m_Plane[i].z * v->z +
			m_Plane[i].w;
		if (pToc > 0.0f)
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL KFrustum::ClassifySphere(KSphere* v)
{
	for (int i = 0; i < 6; i++)
	{
		float pToc = m_Plane[i].x * v->vCenter.x +
			m_Plane[i].y * v->vCenter.y +
			m_Plane[i].z * v->vCenter.z + m_Plane[i].w;
		if (pToc >= v->fRadius)
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL KFrustum::ClassifyOBB(KBox* box)
{
	float sum = 0.0f;
	KVector3 vDir;
	for (int i = 0; i < 6; i++)
	{
		vDir = box->Axis[0] * box->size.x;
		sum = fabs(m_Plane[i].x * vDir.x +
			m_Plane[i].y * vDir.y +
			m_Plane[i].z * vDir.z);
		vDir = box->Axis[1] * box->size.y;
		sum += fabs(m_Plane[i].x * vDir.x +
			m_Plane[i].y * vDir.y +
			m_Plane[i].z * vDir.z);
		vDir = box->Axis[2] * box->size.z;
		sum += fabs(m_Plane[i].x * vDir.x +
			m_Plane[i].y * vDir.y +
			m_Plane[i].z * vDir.z);

		float pToc = m_Plane[i].x * box->middle.x +
			m_Plane[i].y * box->middle.y +
			m_Plane[i].z * box->middle.z + m_Plane[i].w;
		if (pToc >= sum)
		{
			return FALSE;
		}
	}
	return TRUE;
}
