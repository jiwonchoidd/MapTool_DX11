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
public:								//오브젝트 리스트
	std::list<KMapObject*>	m_ObjectList_Static; //프러스텀에 담긴 오브젝트 리스트 
	std::list<KMapObject*>	m_ObjectList_Dynamic;
	std::vector<K3DAsset*>  m_ObjectAssetList; // 중복되지 않는 오브젝트 리스트
	std::map<std::wstring, KMapObject*> m_ObjectMap;
public:
	std::vector<KNode*>		m_pDrawableLeafList;//프로스텀에 보이는 리프노드
public:
	KBoxObj					m_Debug_Box;// Temporary debug object
public:
	KBox   CreateNodeBoundingBox(KNode* pNode);
	void   UpdateNodeBoundingBox(KNode* pNode, float height);
	KVector2 GetHeightFromNode(DWORD TL, DWORD TR, DWORD BL, DWORD BR);
	void   DrawableUpdate();
	void   RenderTile(KNode* pNode); //재귀 함수 버젼
	void   RenderTile_OnlyLeaf(); //리프노드만 도는 버젼
public:
	//맵 오브젝트 추가 관련 함수
	bool   RandomSetupObject(K3DAsset* obj, int amount); // 오브젝트를 Kmapobject 구조체로 변환
	bool   SetupObject(K3DAsset* obj); // 오브젝트를 Kmapobject 구조체로 변환
	bool   AddAsset(K3DAsset* obj); // 
	bool   AddObject(KMapObject* obj); // 실제 오브젝트를 추가하는 함수
	bool   UpdateObject(); // 실제 오브젝트를 추가하는 함수
	bool   AddDynamicObject(KMapObject* obj);
	void	NameDeduplicator(std::wstring* name);
public:
	//LOD 거리에 따라 달라지는 정점, 
	virtual bool	Build(KMap* pmap, KCamera* pCamera);
	virtual bool	SetLOD(KVector3* vCamera);
	virtual DWORD	SetLODType(KNode* pNode);
	virtual bool	ComputeStaticLodIndex(int numcell);
public:
	//LOD 정적 데이터 파일 입출력 함수
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
	//디버깅 함수
	void DrawDebugInit(ID3D11DeviceContext* pContext);
	void DrawDebugRender(KBox* pBox, ID3D11DeviceContext* pContext, float alpha = 1.0f);
	void SetDrawDebug();
public:
	KMapSpace();
	virtual ~KMapSpace();
};

