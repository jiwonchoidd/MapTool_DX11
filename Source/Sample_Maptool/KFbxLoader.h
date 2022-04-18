#pragma once
#include "KFBXObj.h"
//g_FBXManager에서 사용하는 객체
//부모매니저 프로토콜 m_Name, Load(filename) 규격에 맞아야함
class KFbxLoader
{
public:
	std::wstring			m_Name;
	KScene_Animation		m_Scene;
public:
	FbxManager*				m_pFbxManager;
	FbxImporter*			m_pFbxImporter;
	FbxScene*				m_pFbxScene; // 내가 원하는 정보를 찾아서 엔진에 맞는 렌더링 데이터로 바꿈
	FbxNode*				m_pRootNode;
	std::vector<KFBXObj*>	m_FBXTreeList;
	std::vector<KFBXObj*>	m_MeshList;
public:
	std::map<std::string, KMatrix>   m_BindPoseMatrixMap; // T-POSE 행렬 원점 행렬, 리타겟팅, 보간, 이름으로 찾는게 편함
	std::map<std::wstring, KFBXObj*> m_pFbxObjMap;
	std::map<FbxNode*, int>			 m_pFbxNodeMap;
public:
	ID3D11Buffer*			m_pBoneCB = nullptr;
public:
	virtual bool			Load(std::wstring filename); //매니져가 불러옴 보통은 직접 호출하지 않음
	virtual void			NodeProcess(KFBXObj* pParentNode, FbxNode* pNode);
	virtual void			ParseMesh(KFBXObj* pObject);
	virtual std::string		ParseMaterial(FbxSurfaceMaterial* pMtrl);
	void					ParseAnimation();
public:
	KMatrix     DxConvertMatrix(KMatrix m);
	KMatrix     ConvertMatrix(FbxMatrix& m);
	KMatrix     ConvertAMatrix(FbxAMatrix& m);
	bool		ParseMeshSkinning(FbxMesh* pFbxMesh, KFBXObj* pObject);
public:
	bool		CreateBoneConstantBuffer();
public:
	void			ReadTextureCoord(FbxMesh* pFbxMesh,
		FbxLayerElementUV* pUVSet,
		int vertexIndex,
		int uvIndex,
		FbxVector2& uv);
	FbxColor		ReadColor(const FbxMesh* mesh,
		DWORD dwVertexColorCount,
		FbxLayerElementVertexColor* pVertexColorSet,
		DWORD dwDCCIndex, DWORD dwVertexIndex);
	FbxVector4		ReadNormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter);
	FbxVector4		ReadTangent(const FbxMesh* mesh, int controlPointIndex, int vertexCounter);
	FbxVector4		ReadBinormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter);
	int				GetSubMaterialIndex(int iPlygon, FbxLayerElementMaterial* pMtrl);
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
};

