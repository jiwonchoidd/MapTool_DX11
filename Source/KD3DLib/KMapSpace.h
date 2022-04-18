#pragma once
#include "KQuadTree.h"
#include "KMap.h"
#include "KCamera.h"
struct KLodPatch
{
	UINT   iLodLevel;
	std::vector<DWORD>	IndexList[16];
	std::vector<KNode*> g_DrawLeafNodes;
	wrl::ComPtr<ID3D11Buffer> IndexBufferList[16] = { nullptr, };
	void Release()
	{
		for (int iBuffer = 0; iBuffer < 16; iBuffer++)
		{
			if (IndexBufferList[iBuffer].Get())
			{
				IndexBufferList[iBuffer].Reset();
				IndexBufferList[iBuffer] = nullptr;
			}
		}
	}
	KLodPatch() {}
	~KLodPatch()
	{
	}
};
class KMapSpace : public KQuadTree
{
private:
	bool						m_bDebug= false;
public:
	float						m_fDistance_Multiply;
	float						m_fStartDistance;
public:
	KMap*						m_pMap;
	KCamera*					m_pCamera;
public:
	int							m_iNumCell;
	int							m_iPatchLodCount;
	std::vector<KLodPatch>		m_LodPatchList;
	wrl::ComPtr<ID3D11Buffer>	m_pLodIndexBuffer;
	std::vector<DWORD>			m_IndexList;
public:								//������Ʈ ����Ʈ
	std::list<KMapObject*>	m_ObjectList_Static; //�������ҿ� ��� ������Ʈ ����Ʈ 
	std::list<KMapObject*>	m_ObjectList_Dynamic;
	std::vector<K3DAsset*>  m_ObjectItemList; // �ߺ����� �ʴ� ������Ʈ ����Ʈ
	std::map<std::wstring, KMapObject*> m_ObjectMap;
public:
	std::vector<KNode*>		m_pDrawableLeafList;//���ν��ҿ� ���̴� �������
public:
	KBoxObj					m_Debug_Box;// Temporary debug object
public:
	KBox   CreateNodeBoundingBox(KNode* pNode);
	KVector2 GetHeightFromNode(DWORD TL, DWORD TR, DWORD BL, DWORD BR);
	void   DrawableUpdate();
	void   RenderTile(KNode* pNode);
public:
	//�� ������Ʈ �߰� ���� �Լ�
	bool   RandomSetupObject(K3DAsset* obj, int amount); // ������Ʈ�� Kmapobject ����ü�� ��ȯ
	bool   AddObject(KMapObject* obj); // ���� ������Ʈ�� �߰��ϴ� �Լ�
	bool   AddDynamicObject(KMapObject* obj);
	void	NameDeduplicator(std::wstring* name);
public:
	//LOD �Ÿ��� ���� �޶����� ����, 
	virtual bool	Build(KMap* pmap, KCamera* pCamera);
	virtual bool	SetLOD(KVector3* vCamera);
	virtual DWORD	SetLODType(KNode* pNode);
	virtual bool	ComputeStaticLodIndex(int numcell);
public:
	//LOD ���� ������ ���� ����� �Լ�
	bool LoadLODFile(std::wstring filename);
	template <typename OutputIterator>
	void Tokenize(const std::wstring& text, const std::wstring& delimiters, OutputIterator first);
public:
	virtual KNode*	  CreateNode(KNode* pParent, float x, float y, float w, float h)override;
	virtual bool      UpdateVertexList(KNode* pNode)override;
	virtual HRESULT   CreateVertexBuffer(KNode* pNode)override;
	virtual bool      UpdateIndexList(KNode* pNode);
	virtual HRESULT   CreateIndexBuffer(KLodPatch& patch, int iCode);
	virtual HRESULT   CreateIndexBuffer(KNode* pNode);
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render(ID3D11DeviceContext* pContext);
	virtual bool	Render_MapObject(ID3D11DeviceContext* pContext);
	virtual bool	Release();
public:
	//����� �Լ�
	void DrawDebugInit(ID3D11DeviceContext* pContext);
	void DrawDebugRender(KBox* pBox, ID3D11DeviceContext* pContext, float alpha = 1.0f);
	void SetDrawDebug();
public:
	KMapSpace();
	virtual ~KMapSpace();
};

