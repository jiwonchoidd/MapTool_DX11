#pragma once
#include "KNode.h"
#include <queue>
class KQuadTree
{
public:
	int					m_width;
	int					m_height;
	int					m_maxDepth;
public:
	KNode*				m_pRootNode;
	std::map<int, KNode*> m_pLeafList;
	std::queue<KNode*>	m_queue;
public:
	bool	LoadLeafData(std::wstring data);
public:
	void    Buildtree(KNode* pNode);
	void	SetNeighborNode();
	KNode*	FindLeafNode(KVector2 pos);
	KNode*  FindNode(KNode* pNode, KVector2 pos);
	KNode*  FindNode(KNode* pNode, KBox& box);
	bool	SubDivide(KNode* pNode);
public:
	
	bool CheckBox(KBox& boxA, KBox& boxB);
public:
	virtual bool	UpdateVertexList(KNode* pNode); // 가상 함수
	virtual HRESULT	CreateVertexBuffer(KNode* pNode); // 가상 함수
	virtual KNode*  CreateNode(KNode* pParent, float x, float y, float w, float h); // 가상 함수
public:
	virtual bool	Init(float width, float height);
	virtual bool	Frame();
	virtual bool	Render(ID3D11DeviceContext* pContext);
	virtual bool	Release();
public:
	KQuadTree();
	virtual ~KQuadTree();
};

