#include "KFbxLoader.h"
#include "KFBXManager.h"
//------------------------------------------------------------------------
// FBX 로더 
// 스키닝 애니메이션
//------------------------------------------------------------------------
bool KFbxLoader::Init()
{
	m_pFbxManager = FbxManager::Create(); 
	m_pFbxImporter = FbxImporter::Create(m_pFbxManager, "");
	m_pFbxScene = FbxScene::Create(m_pFbxManager, "");

	FbxAxisSystem	 m_SceneAxisSystem = m_pFbxScene->GetGlobalSettings().GetAxisSystem();
	
	FbxAxisSystem::MayaZUp.ConvertScene(m_pFbxScene); //마야 Z축 버젼 사용
	
	m_SceneAxisSystem = m_pFbxScene->GetGlobalSettings().GetAxisSystem();

	//FBX 파일 마다 단위가 다름, 언리얼 cm, 맥스 M 단위를 cm로 바꾼다.
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
	// Control point 개수는 정점의 개수와 동일함
	int iVertexCount = pFbxMesh->GetControlPointsCount();
	pObject->m_WeightList.resize(iVertexCount);

	//디포머 (스킨 매쉬 개수)
	for (int dwDeformerIndex = 0; dwDeformerIndex < iDeformerCount; dwDeformerIndex++)
	{
		auto pSkin = reinterpret_cast<FbxSkin*>(pFbxMesh->GetDeformer(dwDeformerIndex, FbxDeformer::eSkin));
		DWORD dwClusterCount = pSkin->GetClusterCount();
		//가중치 : 각각의 정점에 저장해서 어떤 행렬에 미치는지 클러스터 별로 구분 
		for (int dwClusterIndex = 0; dwClusterIndex < dwClusterCount; dwClusterIndex++)
		{
			auto pCluster = pSkin->GetCluster(dwClusterIndex);
			//바인드 행렬 계산
			FbxAMatrix matXBindPose; //바인드 포즈 T-POSE와 같은 개념
			FbxAMatrix matReferenceGlobalInitPosition; //기존 첫 위치
			pCluster->GetTransformLinkMatrix(matXBindPose);
			pCluster->GetTransformMatrix(matReferenceGlobalInitPosition);
			FbxMatrix matBindPose = matReferenceGlobalInitPosition.Inverse() * matXBindPose;

			KMatrix matInvBindPos = DxConvertMatrix(ConvertMatrix(matBindPose));
			matInvBindPos = matInvBindPos.Invert();
			
			//인덱스 바인드 포즈 맵 
			int iBoneIdx = m_pFbxNodeMap.find(pCluster->GetLink())->second;
			std::wstring name = m_FBXTreeList[iBoneIdx]->m_ObjName;
			pObject->m_MatrixBindPoseMap.insert(std::make_pair(name, matInvBindPos));

			int  dwClusterSize = pCluster->GetControlPointIndicesCount();

			// 영향을 받는 정점들의 인덱스
			int* pIndices = pCluster->GetControlPointIndices();
			double* pWeights = pCluster->GetControlPointWeights();
			// iBoneIndex의 영향을 받는 정점들이 dwClusterSize개 있다.
			for (int i = 0; i < dwClusterSize; i++)
			{
				// n번 정점(pIndices[i])은 iBoneIndex의 행렬에 
				// pWeights[i]의 가중치로 적용되었다.
				int iVertexIndex = pIndices[i];
				float fWeight = pWeights[i];
				pObject->m_WeightList[iVertexIndex].InsertWeight(iBoneIdx, fWeight);
			}
		}
	}
	return true;
}
//매니져에서 함
bool KFbxLoader::Load(std::wstring filename)
{
	//파일 - > 임포터 -> 씬 (트리) 해석 -> 매쉬 -> 정보 가져오기 pnct -> 출력
	Init();
	std::string temp = to_wm(filename);
	bool bRet = m_pFbxImporter->Initialize(temp.c_str()); //파일명 넘김
	bRet = m_pFbxImporter->Import(m_pFbxScene);
	
	//파일명 임포트 성공
	if (bRet)
	{
		//fbx는 트리 구조로 이어져있음N트리 : 자식 개수가 N개임
		//재귀호출로 전 순회 가능, N 트리여서 자식 수를 알아야함
		//
		m_pRootNode = m_pFbxScene->GetRootNode();

		//전 순회해서 씬에 저장되어 있는 트리구조에서 오브젝트를 찾아옴
		NodeProcess(nullptr, m_pRootNode);
		ParseAnimation(); // 애니메이션 씬 시간 체크 했음

		//N트리에서 찾아낸 오브젝트를 해석함, 단, 그릴수있는 매쉬리스트만,
		for (int iobj = 0; iobj < m_MeshList.size(); iobj++)
		{
			ParseMesh(m_MeshList[iobj]);
		}

		CreateBoneConstantBuffer();
		for (int iobj = 0; iobj < m_MeshList.size(); iobj++)
		{
			//오브젝트 생성
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
		m_FBXTreeList.push_back(fbx); // obj 검색 데이터를 넣을때,
		m_pFbxNodeMap.insert(std::make_pair(pNode, fbx->m_iIndex));
		m_pFbxObjMap.insert(std::make_pair(fbx->m_ObjName, fbx));
		//이름으로 바로 인덱스 접근할 수 있으면 끝.
	}
	// 카메라나 라이트, 헬퍼 오브젝트는 메시타입이 아님
	FbxMesh* pMesh = pNode->GetMesh();
	if (pMesh)
	{
		//매쉬 타입만 라이트, 카메라 제외
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
	//현재 노드의 매쉬를 만듬, 버텍스 PNCT를 채워줘야함
	FbxMesh* pFbxMesh = pObject->m_pFbx_ThisNode->GetMesh();

	pObject->m_bSkinned = ParseMeshSkinning(pFbxMesh, pObject);
	if (pFbxMesh)
	{
		//기하 행렬(FBX 위치 버텍스에서 -> 초기 정점 로컬 위치로 변환)
		FbxAMatrix  mat_Geo;
		FbxVector4	t = pObject->m_pFbx_ThisNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		FbxVector4	r = pObject->m_pFbx_ThisNode->GetGeometricRotation(FbxNode::eSourcePivot);
		FbxVector4	s = pObject->m_pFbx_ThisNode->GetGeometricScaling(FbxNode::eSourcePivot);

		mat_Geo.SetT(t);
		mat_Geo.SetR(r);
		mat_Geo.SetS(s);

		//노말 행렬, 기하행렬의 역행렬의 전치
		//노말 매트릭스
		FbxAMatrix normalMatrix = mat_Geo;
		normalMatrix = normalMatrix.Inverse();
		normalMatrix = normalMatrix.Transpose();


		std::vector<FbxLayerElementMaterial*>	 MaterialSet; // 매터리얼
		std::vector<FbxLayerElementUV*>			 UVSet; // UV
		std::vector<FbxLayerElementVertexColor*> VertexColorSet;//

		//// 노말맵을 위한 노말, 바이노말, 탄젠트
		std::vector<FbxLayerElementNormal*>		 NormalSet;  
		std::vector<FbxLayerElementBinormal*>    BinormalSet;
		std::vector<FbxLayerElementTangent*>     TangentSet; 

		int iLayerCount = pFbxMesh->GetLayerCount(); // 레이어 ( 1번에 랜더링, 여러번에 걸쳐서 랜더링 개념)

		if (iLayerCount == 0 || pFbxMesh->GetLayer(0)->GetNormals() == nullptr)
		{
			pFbxMesh->InitNormals();
			pFbxMesh->GenerateNormals(true);
		}
		//노말 탄젠트 바이노말 없을때 생성해준다.
		if (iLayerCount == 0 || pFbxMesh->GetLayer(0)->GetTangents() == nullptr || 
			pFbxMesh->GetLayer(0)->GetBinormals() == nullptr)
		{
			pFbxMesh->InitTangents();
			pFbxMesh->InitBinormals();
			pFbxMesh->GenerateTangentsData(0, true, false);
		}

		for (int iLayer = 0; iLayer < iLayerCount; iLayer++)
		{
			FbxLayer* pFbxLayer = pFbxMesh->GetLayer(iLayer);// 레이어에 UV 정보가 있음 필수적임

			//매터리얼, UV, 버텍스 컬러
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

		//매터리얼 개수 만큼 돌면서 읽어옴 
		//현재는 저장된 텍스쳐의 이름만 가져옴
		int iNumMtrl = pObject->m_pFbx_ThisNode->GetMaterialCount();
		for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
		{
			FbxSurfaceMaterial* pSurface = pObject->m_pFbx_ThisNode->GetMaterial(iMtrl);
			if (pSurface)
			{
				//메터리얼의 텍스쳐 이름을 가져와서 리스트 추가 및 SRV 생성
				std::wstring strFbxPath = L"../../data/model/Texture/";
				std::wstring strMaterial  = to_mw(pSurface->GetName());
				///std::wstring strFBXTexName = to_mw(ParseMaterial(pSurface));
				//strFbxPath += strFBXTexName;
				std::wstring strTexDefault1 = L"Default_Diffuse.jpg";
				std::wstring strTexDefault2 = L"Default_Specular.jpg";
				std::wstring strTexDefault3 = L"Default_Normal.jpg";
				std::wstring strTexAlpha = L"";
				
				g_FBXManager.GetFBXTexList(strMaterial, strTexDefault1, strTexDefault2, strTexDefault3, strTexAlpha);
				//자동으로 텍스쳐를 만드는데, 실패할 경우
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
		//개수가 1보다 많다면 매터리얼 수만큼 배열 할당해주고 
		// 그외에는 무조건 하나로 할당해준다.
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
		// 폴리곤, 면 개수 만큼 돌면서 위치를 저장
		// 삼각형, 사각형
		int iCurpolyIndex = 0; // 증가되는 폴리곤 인덱스
		int iNumPolyCount = pFbxMesh->GetPolygonCount(); //폴리곤 수
		FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints(); //정점 위치 
		int iNumFace = 0;
		for (int iPoly = 0; iPoly < iNumPolyCount; iPoly++)
		{
			int iPolySize = pFbxMesh->GetPolygonSize(iPoly); //4또는 3 삼각형이나 사각형이냐
			iNumFace = iPolySize - 2; // 한면 구하는 계산

			int iSubMtrl = 0;
			//서브 매터리얼 
			if (iNumMtrl >= 1 && MaterialSet[0] != nullptr)
			{
				iSubMtrl = GetSubMaterialIndex(iPoly, MaterialSet[0]);
			}
			//면 4 - 2는 2개의 트라이앵글
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
					v = mat_Geo.MultT(v); // 로컬 좌표로 행렬 곱 
					pnct_vertex.pos.x = v.mData[0];
					pnct_vertex.pos.y = v.mData[2];
					pnct_vertex.pos.z = v.mData[1];

					// UV
					int u[3];
					u[0] = pFbxMesh->GetTextureUVIndex(iPoly, VertexIndex[0]);
					u[1] = pFbxMesh->GetTextureUVIndex(iPoly, VertexIndex[1]);
					u[2] = pFbxMesh->GetTextureUVIndex(iPoly, VertexIndex[2]);
					//UV 리스트에 값이 있다면
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
					//버텍스 컬러 값이 있다면
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
					pnct_vertex.color.w = pObject->m_iIndex; //버텍스 컬러 값에 인덱스 저장
					//----------------------------------------------------------
					//노말값이 있다면
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
					//애니메이션을 위한 가중치
					//캐릭터 애니메이션이 아닌 오브젝트도 스키닝화 시킨다.
					IW_VERTEX iwVertex;
					if (pObject->m_bSkinned) //캐릭터일 경우
					{
						KWeight* weight = &pObject->m_WeightList[CornerIndex[iIndex]];
						for (int i = 0; i < 4; i++)
						{
							iwVertex.i[i] = weight->Index[i];
							iwVertex.w[i] = weight->Weight[i];
						}
					}
					else//오브젝트 애니메이션인 경우
					{
						// 일반오브젝트 에니메이션을 스키닝 케릭터 화 작업.
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
			//Interpolation 작업을 위해 SRT 행렬을 분해한다.
			D3DKMatrixDecompose(&tTrack.s, &tTrack.r, &tTrack.t, &tTrack.matTrack);
			m_FBXTreeList[iObj]->m_AnimTrack.push_back(tTrack);
		}
	}
}

bool KFbxLoader::CreateBoneConstantBuffer()
{
	//3번 슬롯 뼈 상수 버퍼
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
