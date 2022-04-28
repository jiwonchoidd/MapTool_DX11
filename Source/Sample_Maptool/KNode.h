#pragma once
#include "K3DAsset.h"
//�� ������Ʈ�� 
//��� ������Ʈ�� ���� �� �ְ�
//�ݶ��̴� ��Ȱ�� �ϴ� KBOX Ŭ������
//��ü �浹, �������� ���� ������ Ž���� �����ϴ�.
//��ü�� �߰��ϸ�, KBOX�� ������Ʈ�ؼ� ����� �°� ���
class KNode;
struct KMapObject
{
public:
	KNode*			obj_node;
	std::wstring	obj_name;
	KMatrix			obj_matWorld;//������Ʈ���� ��ġ�� �ٸ�
	KVector3		obj_scale;
	KVector3		obj_rot;
	KVector3		obj_pos;
	KBox			obj_box; // ������Ʈ ��ġ OBB AABB ����
	K3DAsset*		obj_pObject;
public:
	KVector3		m_vRight;
	KVector3		m_vUp;
	KVector3		m_vLook;
	void		UpdateData()
	{
		//s
		obj_matWorld._11 = obj_scale.x;
		obj_matWorld._22 = obj_scale.y;
		obj_matWorld._33 = obj_scale.z;
		//r
		//t
		m_vRight.x = obj_matWorld._11;
		m_vRight.y = obj_matWorld._12;
		m_vRight.z = obj_matWorld._13;
		m_vUp.x = obj_matWorld._21;
		m_vUp.y = obj_matWorld._22;
		m_vUp.z = obj_matWorld._23;
		m_vLook.x = obj_matWorld._31;
		m_vLook.y = obj_matWorld._32;
		m_vLook.z = obj_matWorld._33;

		D3DXVec3Normalize(&m_vRight, &m_vRight);
		D3DXVec3Normalize(&m_vUp, &m_vUp);
		D3DXVec3Normalize(&m_vLook, &m_vLook);
	}
	void		UpdateCollision()
	{
		obj_box.Axis[0] = m_vRight;
		obj_box.Axis[1] = m_vUp;
		obj_box.Axis[2] = m_vLook;
		obj_box.min = KVector3(100000, 100000, 100000);
		obj_box.max = KVector3(-100000, -100000, -100000);
		for (int iV = 0; iV < 8; iV++)
		{
			KVector3 pos;
			D3DXVec3TransformCoord(&pos, &obj_box.List[iV], &obj_matWorld);
			if (obj_box.min.x > pos.x)
			{
				obj_box.min.x = pos.x;
			}
			if (obj_box.min.y > pos.y)
			{
				obj_box.min.y = pos.y;
			}
			if (obj_box.min.z > pos.z)
			{
				obj_box.min.z = pos.z;
			}
			if (obj_box.max.x < pos.x)
			{
				obj_box.max.x = pos.x;
			}
			if (obj_box.max.y < pos.y)
			{
				obj_box.max.y = pos.y;
			}
			if (obj_box.max.z < pos.z)
			{
				obj_box.max.z = pos.z;
			}
		}
		obj_box.middle = (obj_box.min + obj_box.max);
		obj_box.middle /= 2.0f;
		KVector3 vHalf = obj_box.max - obj_box.middle;
		obj_box.size.x = fabs(D3DXVec3Dot(&obj_box.Axis[0], &vHalf));
		obj_box.size.y = fabs(D3DXVec3Dot(&obj_box.Axis[1], &vHalf));
		obj_box.size.z = fabs(D3DXVec3Dot(&obj_box.Axis[2], &vHalf));
	}
	KMapObject()
	{
		obj_pos = { 0,0,0 };
		obj_scale = { 1,1,1 };
	}
};
class KNode
{
public:
	KNode*  m_pParent;
	KRect	m_rect;//����
	KBox    m_node_box;//�ݸ���
	int		m_index;
	int		m_data;
	int		m_depth;
public:
	std::vector<KNode*>     m_pChildlist; //�ڽ� ���
	std::vector<KNode*>   	m_pNeighborlist;// �̿� ���
	std::vector<DWORD>		m_CornerList;// ����� �ڳ�
	KVector3				m_Center;
	UINT					m_LodLevel;
	UINT					m_LodType;
	POINT					m_Element;				// ��ġ ����Ʈ
	bool					m_bLeaf;
	std::list<KMapObject*>  m_StaticObjectList;
	std::list<KMapObject*>  m_DynamicObjectList;
public:
	std::vector <PNCT_VERTEX>	m_VertexList;
	wrl::ComPtr<ID3D11Buffer>	m_pVertexBuffer;
public:
	void   AddObject(KMapObject* obj);
	void   AddDynamicObject(KMapObject* obj);
	void   DeleteObject(KMapObject* obj);
public:
	bool isRect(KVector2 pos)
	{
		if (this->m_rect.min.x <= pos.x &&
			this->m_rect.max.x >= pos.x &&
			this->m_rect.min.y <= pos.y &&
			this->m_rect.max.y >= pos.y)
		{
			return true;
		}
		return false;
	}
	void SetRect(float x, float y, float z, float w, float h)
	{
		m_Center.x = x + (w / 2.0f);
		m_Center.y = y/2.0f;
		m_Center.z = z - (h / 2.0f);
		this->m_rect.size = KVector2(w, h);
		this->m_rect.min = KVector2(x, z);
		this->m_rect.max.x = this->m_rect.min.x + w;
		//����
		this->m_rect.max.y = this->m_rect.min.y - h;
	}
public:
	KNode()
	{
		m_pChildlist.resize(4);
		m_pChildlist[0] = nullptr;
		m_pChildlist[1] = nullptr;
		m_pChildlist[2] = nullptr;
		m_pChildlist[3] = nullptr;
		m_pNeighborlist.resize(4);
		m_pParent = nullptr;
		m_index = 0;
		m_data = 0;
		m_depth = 0;
		m_Element = { 0,0 };				// ��ġ ����Ʈ
		m_bLeaf = false;
	}
	KNode(float x, float y, float w, float h)
	{
		m_pChildlist.resize(4);
		m_pChildlist[0] = nullptr;
		m_pChildlist[1] = nullptr;
		m_pChildlist[2] = nullptr;
		m_pChildlist[3] = nullptr;
		m_pNeighborlist.resize(4);
		m_pNeighborlist[0] = nullptr;
		m_pNeighborlist[1] = nullptr;
		m_pNeighborlist[2] = nullptr;
		m_pNeighborlist[3] = nullptr;

		m_pParent = nullptr; m_Element = { 0,0 };
		m_bLeaf = false; m_data = 0;
		m_index = 0; m_depth = 0;
		m_CornerList.push_back(x);
		m_CornerList.push_back(y);
		m_CornerList.push_back(w);
		m_CornerList.push_back(h);
	}
	~KNode()
	{
		for (std::list<KMapObject*>::iterator iter = m_StaticObjectList.begin();
			iter != m_StaticObjectList.end();
			iter++)
		{
			KMapObject* pObj = *iter;
			delete pObj;
		}
		m_StaticObjectList.clear();
		for (int iChild = 0; iChild < 4; iChild++)
		{
			if (m_pChildlist[iChild] != nullptr)
			{
				delete m_pChildlist[iChild];
				m_pChildlist[iChild] = nullptr;
			}
		}

	}
};