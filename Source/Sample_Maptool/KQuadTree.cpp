#include "KQuadTree.h"
#include "KState.h"

bool KQuadTree::Init(float width, float height)
{
	return true;
}

void KQuadTree::Buildtree(KNode* pNode)
{
	if (SubDivide(pNode)) // 재귀호출, 더 나눠 질 수 있는지?
	{
		int center = (pNode->m_CornerList[3] + pNode->m_CornerList[0]) / 2;
		int mt = (pNode->m_CornerList[0] + pNode->m_CornerList[1]) / 2;
		int ml = (pNode->m_CornerList[2] + pNode->m_CornerList[0]) / 2;
		int mr = (pNode->m_CornerList[3] + pNode->m_CornerList[1]) / 2;
		int mb = (pNode->m_CornerList[2] + pNode->m_CornerList[3]) / 2;

		pNode->m_pChildlist[0] = CreateNode(pNode, pNode->m_CornerList[0],
			mt,
			ml,
			center);
		Buildtree(pNode->m_pChildlist[0]);

		pNode->m_pChildlist[1] = CreateNode(pNode, mt,
			pNode->m_CornerList[1],
			center,
			mr);
		Buildtree(pNode->m_pChildlist[1]);

		pNode->m_pChildlist[2] = CreateNode(pNode, ml,
			center,
			pNode->m_CornerList[2],
			mb);
		Buildtree(pNode->m_pChildlist[2]);

		pNode->m_pChildlist[3] = CreateNode(pNode, center,
			mr,
			mb,
			pNode->m_CornerList[3]);
		Buildtree(pNode->m_pChildlist[3]);
	}
	else
	{
		//pNode->m_index = m_pLeafList.size();
		pNode->m_bLeaf = true;
		if (m_maxDepth < pNode->m_depth)
		{
			m_maxDepth = pNode->m_depth;
		}
		// 공유 버택스버퍼용(정점버퍼 리프노드 당), 
		// 맵의 버텍스 버퍼, 노드의 버텍스 버퍼 동기화
		if (UpdateVertexList(pNode))
		{
			CreateVertexBuffer(pNode);
		}
		m_pLeafList.insert(std::make_pair(pNode->m_index, pNode));
	}
}

void KQuadTree::SetNeighborNode()
{
	//최하단 LowQuality 패치의 기준
	for (int iNode = 0; iNode < m_pLeafList.size(); iNode++)
	{
		auto iter = m_pLeafList.find(iNode);
		_ASSERT(iter != m_pLeafList.end());
		KNode* pNode = iter->second;
		DWORD dwNumPatchCount = (DWORD)pow(2.0f, (float)pNode->m_depth);
		DWORD dwNeighborCol = 0;
		DWORD dwNeighborRow = 0;

		if (pNode->m_Element.y > 0)  //상
		{
			dwNeighborCol = pNode->m_Element.x;
			dwNeighborRow = (pNode->m_Element.y - 1) * dwNumPatchCount;
			auto iter = m_pLeafList.find(dwNeighborRow + dwNeighborCol);
			_ASSERT(iter != m_pLeafList.end());
			pNode->m_pNeighborlist[3] = iter->second;
		}
		if (pNode->m_Element.y < dwNumPatchCount - 1) // 하
		{
			dwNeighborCol = pNode->m_Element.x;
			dwNeighborRow = (pNode->m_Element.y + 1) * dwNumPatchCount;
			auto iter = m_pLeafList.find(dwNeighborRow + dwNeighborCol);
			_ASSERT(iter != m_pLeafList.end());
			pNode->m_pNeighborlist[2] = iter->second;
		}
		if (pNode->m_Element.x > 0) // 좌
		{
			dwNeighborCol = pNode->m_Element.x - 1;
			dwNeighborRow = pNode->m_Element.y * dwNumPatchCount;
			auto iter = m_pLeafList.find(dwNeighborRow + dwNeighborCol);
			_ASSERT(iter != m_pLeafList.end());
			pNode->m_pNeighborlist[1] = iter->second;
		}
		if (pNode->m_Element.x < dwNumPatchCount - 1) // 우
		{
			dwNeighborCol = pNode->m_Element.x + 1;
			dwNeighborRow = pNode->m_Element.y * dwNumPatchCount;
			auto iter = m_pLeafList.find(dwNeighborRow + dwNeighborCol);
			_ASSERT(iter != m_pLeafList.end());
			pNode->m_pNeighborlist[0] = iter->second;
		}
	}
}

bool KQuadTree::Release()
{
	m_pLeafList.clear();
	if (m_pRootNode != nullptr)
	{
		delete m_pRootNode;
		m_pRootNode = nullptr;
	}
	return true;
}

KNode* KQuadTree::CreateNode(KNode* pParent, float x, float y, float w, float h)
{
	KNode* pNode = new KNode(x, y, w, h);

	//초기에 부모가 없으므로 예외처리
	if (pParent != nullptr)
	{
		pNode->m_depth = pParent->m_depth + 1;
		pNode->m_pParent = pParent;
	}

	return pNode;
}

KNode* KQuadTree::FindNode(KNode* pNode, KVector2 pos)
{
	do {
		for (int iNode = 0; iNode < 4; iNode++)
		{
			//위치에 포함된 노드라면?
			if (pNode->m_pChildlist[iNode] != nullptr &&
				pNode->m_pChildlist[iNode]->isRect(pos))
			{
				m_queue.push(pNode->m_pChildlist[iNode]);
				break;
			}
		}
		if (m_queue.empty())break;
		pNode = m_queue.front();
		m_queue.pop();
	} while (pNode);
	return pNode;
}
KNode* KQuadTree::FindNode(KNode* pNode, KBox& box)
{
	do {
		for (int iNode = 0; iNode < 4; iNode++)
		{
			if (pNode->m_pChildlist[iNode] != nullptr)
			{
				if (CheckBox(pNode->m_pChildlist[iNode]->m_node_box,
					box))
				{
					m_queue.push(pNode->m_pChildlist[iNode]);
					break;
				}
			}
		}
		if (m_queue.empty()) break;
		pNode = m_queue.front();
		m_queue.pop();
	} while (pNode);
	return pNode;
}

bool KQuadTree::CheckBox(KBox& boxA, KBox& boxB)
{
	if (boxA.min.x <= boxB.min.x && boxA.min.y <= boxB.min.y &&
		boxA.min.z <= boxB.min.z)
	{
		if (boxA.max.x >= boxB.max.x && boxA.max.y >= boxB.max.y &&
			boxA.max.z >= boxB.max.z)
		{
			return true;
		}
	}
	return false;
}

bool KQuadTree::SubDivide(KNode* pNode)
{
	if ((pNode->m_CornerList[1] - pNode->m_CornerList[0]) > 4)
	{
		return true;
	}
	return false;
}

KNode* KQuadTree::FindLeafNode(KVector2 pos)
{
	for (int iNode = 0; iNode < m_pLeafList.size(); iNode++)
	{
		if (m_pLeafList[iNode]->isRect(pos))
		{
			return m_pLeafList[iNode];
		}
	}
	return nullptr;
}
//파일 로드
bool KQuadTree::LoadLeafData(std::wstring data)
{
	//8,4,2,1
	//위 아래 왼 오른
	//
	FILE* fp = nullptr;
	_tfopen_s(&fp, data.c_str(), _T("rt"));
	if (fp == NULL)
	{
		//파일이 없음
		return false;
	}
	TCHAR buffer[256] = { 0, };
	_fgetts(buffer, _countof(buffer), fp);
	TCHAR filename[32] = {0,};
	_stscanf_s(buffer, _T("%s %d"), filename, (unsigned int)_countof(filename));

	while (_fgetts(buffer, _countof(buffer), fp) != 0)
	{
		int index = 0;
		int data = 0;
		_stscanf_s(buffer, _T("%d %d \n"),&index, &data);
		m_pLeafList[index]->m_data = data;
	}
	fclose(fp);
	return true;
}

bool KQuadTree::Frame()
{
	return true;
}

bool KQuadTree::Render(ID3D11DeviceContext* pContext)
{
	return true;
}

//가상함수----------------------------------------------------
bool KQuadTree::UpdateVertexList(KNode* pNode)
{
	return true;
}
HRESULT KQuadTree::CreateVertexBuffer(KNode* pNode)
{
	return E_NOTIMPL;
}
//------------------------------------------------------------

KQuadTree::KQuadTree()
{
	m_height = 0;
	m_width = 0;
	m_pRootNode = nullptr;
}

KQuadTree::~KQuadTree()
{
}


