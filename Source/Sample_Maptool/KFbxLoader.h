#pragma once
#include "KFBXObj.h"
//g_FBXManager���� ����ϴ� ��ü
//�θ�Ŵ��� �������� m_Name, Load(filename) �԰ݿ� �¾ƾ���
class KFbxLoader
{
public:
	std::wstring			m_Name;
	KScene_Animation		m_Scene;
public:
	FbxManager*				m_pFbxManager;
	FbxImporter*			m_pFbxImporter;
	FbxScene*				m_pFbxScene; // ���� ���ϴ� ������ ã�Ƽ� ������ �´� ������ �����ͷ� �ٲ�
	FbxNode*				m_pRootNode;
	std::vector<KFBXObj*>	m_FBXTreeList;
	std::vector<KFBXObj*>	m_MeshList;
public:
	std::map<std::string, KMatrix>   m_BindPoseMatrixMap; // T-POSE ��� ���� ���, ��Ÿ����, ����, �̸����� ã�°� ����
	std::map<std::wstring, KFBXObj*> m_pFbxObjMap;
	std::map<FbxNode*, int>			 m_pFbxNodeMap;
public:
	ID3D11Buffer*			m_pBoneCB = nullptr;
public:
	virtual bool			Load(std::wstring filename); //�Ŵ����� �ҷ��� ������ ���� ȣ������ ����
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

