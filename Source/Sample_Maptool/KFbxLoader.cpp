#include "KFbxLoader.h"
#include "KFBXManager.h"
//------------------------------------------------------------------------
// FBX �δ� 
// ��Ű�� �ִϸ��̼�
//------------------------------------------------------------------------
bool KFbxLoader::Init()
{
	m_pFbxManager = FbxManager::Create(); 
	m_pFbxImporter = FbxImporter::Create(m_pFbxManager, "");
	m_pFbxScene = FbxScene::Create(m_pFbxManager, "");

	FbxAxisSystem	 m_SceneAxisSystem = m_pFbxScene->GetGlobalSettings().GetAxisSystem();
	
	FbxAxisSystem::MayaZUp.ConvertScene(m_pFbxScene); //���� Z�� ���� ���
	
	m_SceneAxisSystem = m_pFbxScene->GetGlobalSettings().GetAxisSystem();

	//FBX ���� ���� ������ �ٸ�, �𸮾� cm, �ƽ� M ������ cm�� �ٲ۴�.
	FbxSystemUnit	m_SceneSystemUnit = m_pFbxScene->GetGlobalSettings().GetSystemUnit();
	if (m_SceneSystemUnit.GetScaleFactor() != 1.0f)
	{
		FbxSystemUnit::cm.ConvertScene(m_pFbxScene);
	}
	return true;
}
bool KFbxLoader::ParseMeshSkinning(FbxMesh* pFbxMesh, KFBXObj* pObject)
{
	int iDeformerCount = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (iDeformerCount == 0)
	{
		return false;
	}
	// Control point ������ ������ ������ ������
	int iVertexCount = pFbxMesh->GetControlPointsCount();
	pObject->m_WeightList.resize(iVertexCount);

	//������ (��Ų �Ž� ����)
	for (int dwDeformerIndex = 0; dwDeformerIndex < iDeformerCount; dwDeformerIndex++)
	{
		auto pSkin = reinterpret_cast<FbxSkin*>(pFbxMesh->GetDeformer(dwDeformerIndex, FbxDeformer::eSkin));
		DWORD dwClusterCount = pSkin->GetClusterCount();
		//����ġ : ������ ������ �����ؼ� � ��Ŀ� ��ġ���� Ŭ������ ���� ���� 
		for (int dwClusterIndex = 0; dwClusterIndex < dwClusterCount; dwClusterIndex++)
		{
			auto pCluster = pSkin->GetCluster(dwClusterIndex);
			//���ε� ��� ���
			FbxAMatrix matXBindPose; //���ε� ���� T-POSE�� ���� ����
			FbxAMatrix matReferenceGlobalInitPosition; //���� ù ��ġ
			pCluster->GetTransformLinkMatrix(matXBindPose);
			pCluster->GetTransformMatrix(matReferenceGlobalInitPosition);
			FbxMatrix matBindPose = matReferenceGlobalInitPosition.Inverse() * matXBindPose;

			KMatrix matInvBindPos = DxConvertMatrix(ConvertMatrix(matBindPose));
			matInvBindPos = matInvBindPos.Invert();
			
			//�ε��� ���ε� ���� �� 
			int iBoneIdx = m_pFbxNodeMap.find(pCluster->GetLink())->second;
			std::wstring name = m_FBXTreeList[iBoneIdx]->m_ObjName;
			pObject->m_MatrixBindPoseMap.insert(std::make_pair(name, matInvBindPos));

			int  dwClusterSize = pCluster->GetControlPointIndicesCount();

			// ������ �޴� �������� �ε���
			int* pIndices = pCluster->GetControlPointIndices();
			double* pWeights = pCluster->GetControlPointWeights();
			// iBoneIndex�� ������ �޴� �������� dwClusterSize�� �ִ�.
			for (int i = 0; i < dwClusterSize; i++)
			{
				// n�� ����(pIndices[i])�� iBoneIndex�� ��Ŀ� 
				// pWeights[i]�� ����ġ�� ����Ǿ���.
				int iVertexIndex = pIndices[i];
				float fWeight = pWeights[i];
				pObject->m_WeightList[iVertexIndex].InsertWeight(iBoneIdx, fWeight);
			}
		}
	}
	return true;
}
//�Ŵ������� ��
bool KFbxLoader::Load(std::wstring filename)
{
	//���� - > ������ -> �� (Ʈ��) �ؼ� -> �Ž� -> ���� �������� pnct -> ���
	Init();
	std::string temp = to_wm(filename);
	bool bRet = m_pFbxImporter->Initialize(temp.c_str()); //���ϸ� �ѱ�
	bRet = m_pFbxImporter->Import(m_pFbxScene);
	
	//���ϸ� ����Ʈ ����
	if (bRet)
	{
		//fbx�� Ʈ�� ������ �̾�������NƮ�� : �ڽ� ������ N����
		//���ȣ��� �� ��ȸ ����, N Ʈ������ �ڽ� ���� �˾ƾ���
		//
		m_pRootNode = m_pFbxScene->GetRootNode();

		//�� ��ȸ�ؼ� ���� ����Ǿ� �ִ� Ʈ���������� ������Ʈ�� ã�ƿ�
		NodeProcess(nullptr, m_pRootNode);
		ParseAnimation(); // �ִϸ��̼� �� �ð� üũ ����

		//NƮ������ ã�Ƴ� ������Ʈ�� �ؼ���, ��, �׸����ִ� �Ž�����Ʈ��,
		for (int iobj = 0; iobj < m_MeshList.size(); iobj++)
		{
			ParseMesh(m_MeshList[iobj]);
		}

		CreateBoneConstantBuffer();
		for (int iobj = 0; iobj < m_MeshList.size(); iobj++)
		{
			//������Ʈ ����
			if (!m_MeshList[iobj]->CreateObject(L"../../data/shader/VSPS_FBX.hlsl", L"../../data/shader/VSPS_FBX.hlsl"))
			{
				return false;
			}
		}
		return true;
	}
	return false;
}


void KFbxLoader::NodeProcess(KFBXObj* pParentObj, FbxNode* pNode)
{
	KFBXObj* fbx = nullptr;
	if (pNode!=nullptr)
	{
		fbx = new KFBXObj;
		fbx->m_pFbx_ThisNode = pNode;
		fbx->m_pFbx_ParentNode = pNode->GetParent();
		fbx->m_pFbx_ParentObj = pParentObj;
		fbx->m_iIndex = m_FBXTreeList.size();
		fbx->m_ObjName = to_mw(pNode->GetName());
		m_FBXTreeList.push_back(fbx); // obj �˻� �����͸� ������,
		m_pFbxNodeMap.insert(std::make_pair(pNode, fbx->m_iIndex));
		m_pFbxObjMap.insert(std::make_pair(fbx->m_ObjName, fbx));
		//�̸����� �ٷ� �ε��� ������ �� ������ ��.
	}
	// ī�޶� ����Ʈ, ���� ������Ʈ�� �޽�Ÿ���� �ƴ�
	FbxMesh* pMesh = pNode->GetMesh();
	if (pMesh)
	{
		//�Ž� Ÿ�Ը� ����Ʈ, ī�޶� ����
		m_MeshList.push_back(fbx);
	}
	int iNumChild = pNode->GetChildCount();
	for (int iNode = 0; iNode < iNumChild; iNode++)
	{
		FbxNode* child = pNode->GetChild(iNode);
		NodeProcess(fbx, child);
	}
}

void KFbxLoader::ParseMesh(KFBXObj* pObject)
{
	//���� ����� �Ž��� ����, ���ؽ� PNCT�� ä�������
	FbxMesh* pFbxMesh = pObject->m_pFbx_ThisNode->GetMesh();

	pObject->m_bSkinned = ParseMeshSkinning(pFbxMesh, pObject);
	if (pFbxMesh)
	{
		//���� ���(FBX ��ġ ���ؽ����� -> �ʱ� ���� ���� ��ġ�� ��ȯ)
		FbxAMatrix  mat_Geo;
		FbxVector4	t = pObject->m_pFbx_ThisNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		FbxVector4	r = pObject->m_pFbx_ThisNode->GetGeometricRotation(FbxNode::eSourcePivot);
		FbxVector4	s = pObject->m_pFbx_ThisNode->GetGeometricScaling(FbxNode::eSourcePivot);

		mat_Geo.SetT(t);
		mat_Geo.SetR(r);
		mat_Geo.SetS(s);

		//�븻 ���, ��������� ������� ��ġ
		//�븻 ��Ʈ����
		FbxAMatrix normalMatrix = mat_Geo;
		normalMatrix = normalMatrix.Inverse();
		normalMatrix = normalMatrix.Transpose();


		std::vector<FbxLayerElementMaterial*>	 MaterialSet; // ���͸���
		std::vector<FbxLayerElementUV*>			 UVSet; // UV
		std::vector<FbxLayerElementVertexColor*> VertexColorSet;//

		//// �븻���� ���� �븻, ���̳븻, ź��Ʈ
		std::vector<FbxLayerElementNormal*>		 NormalSet;  
		std::vector<FbxLayerElementBinormal*>    BinormalSet;
		std::vector<FbxLayerElementTangent*>     TangentSet; 

		int iLayerCount = pFbxMesh->GetLayerCount(); // ���̾� ( 1���� ������, �������� ���ļ� ������ ����)

		if (iLayerCount == 0 || pFbxMesh->GetLayer(0)->GetNormals() == nullptr)
		{
			pFbxMesh->InitNormals();
			pFbxMesh->GenerateNormals(true);
		}
		//�븻 ź��Ʈ ���̳븻 ������ �������ش�.
		if (iLayerCount == 0 || pFbxMesh->GetLayer(0)->GetTangents() == nullptr || 
			pFbxMesh->GetLayer(0)->GetBinormals() == nullptr)
		{
			pFbxMesh->InitTangents();
			pFbxMesh->InitBinormals();
			pFbxMesh->GenerateTangentsData(0, true, false);
		}

		for (int iLayer = 0; iLayer < iLayerCount; iLayer++)
		{
			FbxLayer* pFbxLayer = pFbxMesh->GetLayer(iLayer);// ���̾ UV ������ ���� �ʼ�����

			//���͸���, UV, ���ؽ� �÷�
			if (pFbxLayer->GetMaterials() != nullptr)
			{
				MaterialSet.push_back(pFbxLayer->GetMaterials());
			}
			if (pFbxLayer->GetUVs() != nullptr)
			{
				UVSet.push_back(pFbxLayer->GetUVs());
			}
			if (pFbxLayer->GetVertexColors() != nullptr)
			{
				VertexColorSet.push_back(pFbxLayer->GetVertexColors());
			}
			if (pFbxLayer->GetNormals() != nullptr)
			{
				NormalSet.push_back(pFbxLayer->GetNormals());
			}
			if (pFbxLayer->GetBinormals() != nullptr) 
			{
				BinormalSet.push_back(pFbxLayer->GetBinormals());
			}
			if (pFbxLayer->GetTangents() != nullptr)
			{
				TangentSet.push_back(pFbxLayer->GetTangents());
			}
		}

		//���͸��� ���� ��ŭ ���鼭 �о�� 
		//����� ����� �ؽ����� �̸��� ������
		int iNumMtrl = pObject->m_pFbx_ThisNode->GetMaterialCount();
		for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
		{
			FbxSurfaceMaterial* pSurface = pObject->m_pFbx_ThisNode->GetMaterial(iMtrl);
			if (pSurface)
			{
				//���͸����� �ؽ��� �̸��� �����ͼ� ����Ʈ �߰� �� SRV ����
				std::wstring strFbxPath = L"../../data/model/Texture/";
				std::wstring strMaterial  = to_mw(pSurface->GetName());
				///std::wstring strFBXTexName = to_mw(ParseMaterial(pSurface));
				//strFbxPath += strFBXTexName;
				std::wstring strTexDefault1 = L"Default_Diffuse.jpg";
				std::wstring strTexDefault2 = L"Default_Specular.jpg";
				std::wstring strTexDefault3 = L"Default_Normal.jpg";
				std::wstring strTexAlpha = L"";
				
				g_FBXManager.GetFBXTexList(strMaterial, strTexDefault1, strTexDefault2, strTexDefault3, strTexAlpha);
				//�ڵ����� �ؽ��ĸ� ����µ�, ������ ���
				KTexture* pTex1 = g_TextureMananger.Load(strFbxPath + strTexDefault1);
				KTexture* pTex2 = g_TextureMananger.Load(strFbxPath + strTexDefault2);
				KTexture* pTex3 = g_TextureMananger.Load(strFbxPath + strTexDefault3);
				KTexture* pTexAlpha = nullptr;
				//
				if (strTexAlpha != L"")
				{
					pTexAlpha = g_TextureMananger.Load(strFbxPath + strTexAlpha);
				}
				KTexturePack pack = { pTex1 ,pTex2, pTex3, pTexAlpha};
				pObject->m_pTextureList.push_back(pack);
			}
		}
		//������ 1���� ���ٸ� ���͸��� ����ŭ �迭 �Ҵ����ְ� 
		// �׿ܿ��� ������ �ϳ��� �Ҵ����ش�.
		if (iNumMtrl > 0)
		{
			pObject->m_pSubBTList.resize(iNumMtrl);
			pObject->m_pSubVertexList.resize(iNumMtrl);
			pObject->m_pSubIWVertexList.resize(iNumMtrl);
		}
		else
		{
			pObject->m_pSubBTList.resize(1);
			pObject->m_pSubVertexList.resize(1);
			pObject->m_pSubIWVertexList.resize(1);
		}
		//----------------------------------------------------------
		// ������, �� ���� ��ŭ ���鼭 ��ġ�� ����
		// �ﰢ��, �簢��
		int iCurpolyIndex = 0; // �����Ǵ� ������ �ε���
		int iNumPolyCount = pFbxMesh->GetPolygonCount(); //������ ��
		FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints(); //���� ��ġ 
		int iNumFace = 0;
		for (int iPoly = 0; iPoly < iNumPolyCount; iPoly++)
		{
			int iPolySize = pFbxMesh->GetPolygonSize(iPoly); //4�Ǵ� 3 �ﰢ���̳� �簢���̳�
			iNumFace = iPolySize - 2; // �Ѹ� ���ϴ� ���

			int iSubMtrl = 0;
			//���� ���͸��� 
			if (iNumMtrl >= 1 && MaterialSet[0] != nullptr)
			{
				iSubMtrl = GetSubMaterialIndex(iPoly, MaterialSet[0]);
			}
			//�� 4 - 2�� 2���� Ʈ���̾ޱ�
			for (int iFace = 0; iFace < iNumFace; iFace++)
			{
				int VertexIndex[3] = { 0, iFace + 2, iFace + 1 };
				int CornerIndex[3];
				CornerIndex[0] = pFbxMesh->GetPolygonVertex(iPoly, VertexIndex[0]);
				CornerIndex[1] = pFbxMesh->GetPolygonVertex(iPoly, VertexIndex[1]);
				CornerIndex[2] = pFbxMesh->GetPolygonVertex(iPoly, VertexIndex[2]);
				for (int iIndex = 0; iIndex < 3; iIndex++)
				{
					PNCT_VERTEX pnct_vertex;
					BT_VERTEX bt_vertex;
					// Max(x,z,y) ->(dx)x,y,z    
					FbxVector4 v = pVertexPositions[CornerIndex[iIndex]];
					v = mat_Geo.MultT(v); // ���� ��ǥ�� ��� �� 
					pnct_vertex.pos.x = v.mData[0];
					pnct_vertex.pos.y = v.mData[2];
					pnct_vertex.pos.z = v.mData[1];

					// UV
					int u[3];
					u[0] = pFbxMesh->GetTextureUVIndex(iPoly, VertexIndex[0]);
					u[1] = pFbxMesh->GetTextureUVIndex(iPoly, VertexIndex[1]);
					u[2] = pFbxMesh->GetTextureUVIndex(iPoly, VertexIndex[2]);
					//UV ����Ʈ�� ���� �ִٸ�
					if (UVSet.size() > 0)
					{
						FbxLayerElementUV* pUVSet = UVSet[0];
						FbxVector2 uv;
						ReadTextureCoord(
							pFbxMesh,
							pUVSet,
							CornerIndex[iIndex],
							u[iIndex],
							uv);
						pnct_vertex.tex.x = uv.mData[0];
						pnct_vertex.tex.y = 1.0f - uv.mData[1];
					}
					//----------------------------------------------------------
					//���ؽ� �÷� ���� �ִٸ�
					FbxColor color = FbxColor(1, 1, 1, 1);
					if (VertexColorSet.size() > 0)
					{
						color = ReadColor(pFbxMesh,
							VertexColorSet.size(),
							VertexColorSet[0],
							CornerIndex[iIndex],
							iCurpolyIndex + VertexIndex[iIndex]);
					}
					pnct_vertex.color.x = color.mRed;
					pnct_vertex.color.y = color.mGreen;
					pnct_vertex.color.z = color.mBlue;
					pnct_vertex.color.w = pObject->m_iIndex; //���ؽ� �÷� ���� �ε��� ����
					//----------------------------------------------------------
					//�븻���� �ִٸ�
					if (NormalSet.size() > 0)
					{
						FbxVector4 normal = ReadNormal(pFbxMesh,
							CornerIndex[iIndex],
							iCurpolyIndex + VertexIndex[iIndex]);
						normal = normalMatrix.MultT(normal);
						pnct_vertex.normal.x = normal.mData[0]; // x
						pnct_vertex.normal.y = normal.mData[2]; // z
						pnct_vertex.normal.z = normal.mData[1]; // y
						D3DXVec3Normalize(&pnct_vertex.normal, &pnct_vertex.normal);
					}

					if (BinormalSet.size() > 0)
					{
						FbxVector4 binormal = ReadBinormal(pFbxMesh,
							CornerIndex[iIndex],
							iCurpolyIndex + VertexIndex[iIndex]);
						binormal = normalMatrix.MultT(binormal);
						bt_vertex.binormal.x = binormal.mData[0]; // x
						bt_vertex.binormal.y = binormal.mData[2]; // z
						bt_vertex.binormal.z = binormal.mData[1]; // y
						D3DXVec3Normalize(&bt_vertex.binormal, &bt_vertex.binormal);
					}

					if (TangentSet.size() > 0)
					{
						FbxVector4 tangent = ReadTangent(pFbxMesh,
							CornerIndex[iIndex],
							iCurpolyIndex + VertexIndex[iIndex]);
						tangent = normalMatrix.MultT(tangent);
						bt_vertex.tangent.x = tangent.mData[0]; // x
						bt_vertex.tangent.y = tangent.mData[2]; // z
						bt_vertex.tangent.z = tangent.mData[1]; // y
						D3DXVec3Normalize(&bt_vertex.tangent, &bt_vertex.tangent);
					}
					//�ִϸ��̼��� ���� ����ġ
					//ĳ���� �ִϸ��̼��� �ƴ� ������Ʈ�� ��Ű��ȭ ��Ų��.
					IW_VERTEX iwVertex;
					if (pObject->m_bSkinned) //ĳ������ ���
					{
						KWeight* weight = &pObject->m_WeightList[CornerIndex[iIndex]];
						for (int i = 0; i < 4; i++)
						{
							iwVertex.i[i] = weight->Index[i];
							iwVertex.w[i] = weight->Weight[i];
						}
					}
					else//������Ʈ �ִϸ��̼��� ���
					{
						// �Ϲݿ�����Ʈ ���ϸ��̼��� ��Ű�� �ɸ��� ȭ �۾�.
						iwVertex.i[0] = pObject->m_iIndex;
						iwVertex.w[0] = 1.0f;
					}

					//pObject->m_VertexList.push_back(Vertex);//
					pObject->m_pSubVertexList[iSubMtrl].push_back(pnct_vertex);
					pObject->m_pSubIWVertexList[iSubMtrl].push_back(iwVertex);
					pObject->m_pSubBTList[iSubMtrl].push_back(bt_vertex);
				}
			}
			iCurpolyIndex += iPolySize;
		}

	}
}

std::string KFbxLoader::ParseMaterial(FbxSurfaceMaterial* pMtrl)
{
	std::string mtl_name = pMtrl->GetName();
	auto dProperty = pMtrl->FindProperty(FbxSurfaceMaterial::sDiffuse);
	auto sProperty = pMtrl->FindProperty(FbxSurfaceMaterial::sSpecular);
	auto nProperty = pMtrl->FindProperty(FbxSurfaceMaterial::sNormalMap);
	if (dProperty.IsValid())
	{
		const FBXSDK_DLL::FbxFileTexture* tex = dProperty.GetSrcObject<FBXSDK_DLL::FbxFileTexture>(0);
		if (tex != nullptr)
		{
			const CHAR* szFileName = tex->GetFileName();
			CHAR Drive[MAX_PATH];
			CHAR Dir[MAX_PATH];
			CHAR FName[MAX_PATH];
			CHAR Ext[MAX_PATH];
			_splitpath_s(szFileName, Drive, Dir, FName, Ext);
			std::string texName = FName;
			std::string ext = Ext;
			if (ext == ".tga" || ext == ".TGA")
			{
				ext.clear();
				ext = ".dds";
			}
			texName += ext;
			return texName;
		}
	}
	return std::string("");
}

KMatrix KFbxLoader::DxConvertMatrix(KMatrix m)
{
	KMatrix mat;
	mat._11 = m._11; mat._12 = m._13; mat._13 = m._12;
	mat._21 = m._31; mat._22 = m._33; mat._23 = m._32;
	mat._31 = m._21; mat._32 = m._23; mat._33 = m._22;
	mat._41 = m._41; mat._42 = m._43; mat._43 = m._42;
	mat._14 = mat._24 = mat._34 = 0.0f;
	mat._44 = 1.0f;
	return mat;
}

KMatrix KFbxLoader::ConvertMatrix(FbxMatrix& m)
{
	KMatrix mat;
	float* pMatArray = reinterpret_cast<float*>(&mat);
	double* pSrcArray = reinterpret_cast<double*>(&m);
	for (int i = 0; i < 16; i++)
	{
		pMatArray[i] = pSrcArray[i];
	}
	return mat;
}

KMatrix KFbxLoader::ConvertAMatrix(FbxAMatrix& m)
{
	KMatrix mat;
	float* pMatArray = reinterpret_cast<float*>(&mat);
	double* pSrcArray = reinterpret_cast<double*>(&m);
	for (int i = 0; i < 16; i++)
	{
		pMatArray[i] = pSrcArray[i];
	}
	return mat;
}

void KFbxLoader::ParseAnimation()
{
	FbxTime::SetGlobalTimeMode(FbxTime::eFrames30);
	FbxAnimStack* stack = m_pFbxScene->GetSrcObject<FbxAnimStack>(0);
	if (stack == nullptr) return;

	FbxString TakeName = stack->GetName();
	FbxTakeInfo* TakeInfo = m_pFbxScene->GetTakeInfo(TakeName);
	FbxTimeSpan LocalTimeSpan = TakeInfo->mLocalTimeSpan;
	FbxTime start	 = LocalTimeSpan.GetStart();
	FbxTime end		 = LocalTimeSpan.GetStop();
	FbxTime Duration = LocalTimeSpan.GetDuration();

	FbxTime::EMode TimeMode = FbxTime::GetGlobalTimeMode();
	FbxLongLong s = start.GetFrameCount(TimeMode);
	FbxLongLong n = end.GetFrameCount(TimeMode);

	m_Scene.iStart = s;
	m_Scene.iEnd = n;
	m_Scene.iFrameSpeed = 30;

	FbxTime time;
	KTrack tTrack;
	for (FbxLongLong t = s; t <= n; t++)
	{
		time.SetFrame(t, TimeMode);
		for (int iObj = 0; iObj < m_FBXTreeList.size(); iObj++)
		{
			FbxAMatrix matGlobal = m_FBXTreeList[iObj]->m_pFbx_ThisNode->EvaluateGlobalTransform(time);
			tTrack.iFrame = t;
			tTrack.matTrack = DxConvertMatrix(ConvertAMatrix(matGlobal));
			//Interpolation �۾��� ���� SRT ����� �����Ѵ�.
			D3DKMatrixDecompose(&tTrack.s, &tTrack.r, &tTrack.t, &tTrack.matTrack);
			m_FBXTreeList[iObj]->m_AnimTrack.push_back(tTrack);
		}
	}
}

bool KFbxLoader::CreateBoneConstantBuffer()
{
	//3�� ���� �� ��� ����
	HRESULT hr;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(KBoneWorld);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	if (FAILED(hr = g_pd3dDevice->CreateBuffer(&bd, NULL,
		&m_pBoneCB)))
	{
		return false;
	}
	return true;
}


bool KFbxLoader::Frame()
{
	return true;
}

bool KFbxLoader::Render()
{
	return true;
}

bool KFbxLoader::Release()
{
	for (int iObj = 0; iObj < m_MeshList.size(); iObj++)
	{
		m_MeshList[iObj]->Release();
	}
	if (m_pBoneCB != nullptr)
	{
		m_pBoneCB->Release();
	}
	if (m_pFbxScene != nullptr)
	{
		m_pFbxScene->Destroy();
	}
	if (m_pFbxImporter != nullptr)
	{
		m_pFbxImporter->Destroy();
	}
	if (m_pFbxManager != nullptr)
	{
		m_pFbxManager->Destroy();
	}
	return true;
}
