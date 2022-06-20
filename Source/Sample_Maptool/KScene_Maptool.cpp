#include "KScene_Maptool.h"
#include "KSceneManager.h"
#include "KState.h"
#include "KFBXManager.h"
#include "ImGuiManager.h"
void KScene_Maptool::ImguiInspector()
{
	#pragma region IMGUI UI <- HERE
	//IMGUI IU 
	ImVec2 ivMin = { static_cast<float>(g_rtClient.right) - static_cast<float>(g_rtClient.right) / 3.5f, 20 };
	ImVec2 ivMax = { static_cast<float>(g_rtClient.right), static_cast<float>(g_rtClient.bottom) };
	if (ImGui::IsMouseHoveringRect(ivMin, ivMax, false))
	{
		m_MousePicker.m_bImgui = true;
	}
	else
	{
		m_MousePicker.m_bImgui = false;
	}

	ImGui::SetNextWindowSize(ImVec2(ivMax.x - ivMin.x, ivMax.y - ivMin.y));
	ImGui::SetNextWindowPos(ivMin);
	if (ImGui::Begin("Inspector"))
	{
		// 맵툴 : 오브젝트 배치, 높이, 텍스쳐--------------------------------------------------------
		if (ImGui::CollapsingHeader("[ MapTool ]"))
		{
			if (ImGui::BeginChild("chlid_terrain", ImVec2(0, 300.0f), true))
			{
				#pragma region 맵툴 IMGUI
				//--------------------------------------------------------
				//라디오 버튼
				if (ImGui::RadioButton("Object", (m_MousePicker.m_iControlState == 0)))
				{
					m_MousePicker.m_iControlState = 0;
				}ImGui::SameLine();
				if (ImGui::RadioButton("Height", (m_MousePicker.m_iControlState == 1)))
				{
					m_MousePicker.m_iControlState = 1;

				}ImGui::SameLine();
				if (ImGui::RadioButton("Texture", (m_MousePicker.m_iControlState == 2)))
				{
					m_MousePicker.m_iControlState = 2;
				}
				//------------------------------------------------------
				//오브젝트
				if (m_MousePicker.m_iControlState == 0)
				{
					#pragma region 오브젝트 IMGUI
					//마우스 선택, 리스트 선택 오브젝트---------------------------------------------------------
					ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), u8"오브젝트");
					if (m_MousePicker.m_pSeletedObj != nullptr)
					{
						bool iChange = 0;
						ImGui::PushItemWidth(50);
						ImGui::TextColored(ImVec4(1, 1, 0, 1), u8"선택된 오브젝트 : "); ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), to_wm(m_MousePicker.m_pSeletedObj->obj_name).c_str());
						ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "Position"); ImGui::SameLine();
						iChange += ImGui::InputFloat("X ##position", &m_MousePicker.m_pSeletedObj->obj_pos.x); ImGui::SameLine();
						iChange += ImGui::InputFloat("Y ##position", &m_MousePicker.m_pSeletedObj->obj_pos.y); ImGui::SameLine();
						iChange += ImGui::InputFloat("Z ##position", &m_MousePicker.m_pSeletedObj->obj_pos.z);
						ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "Rotation"); ImGui::SameLine();
						iChange += ImGui::InputFloat("X ##rotation", &m_MousePicker.m_pSeletedObj->obj_RollPitchYaw.x); ImGui::SameLine();
						iChange += ImGui::InputFloat("Y ##rotation", &m_MousePicker.m_pSeletedObj->obj_RollPitchYaw.y); ImGui::SameLine();
						iChange += ImGui::InputFloat("Z ##rotation", &m_MousePicker.m_pSeletedObj->obj_RollPitchYaw.z);
						ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), "Scale"); ImGui::SameLine();
						iChange += ImGui::InputFloat("X ##scale", &m_MousePicker.m_pSeletedObj->obj_scale.x); ImGui::SameLine();
						iChange += ImGui::InputFloat("Y ##scale", &m_MousePicker.m_pSeletedObj->obj_scale.y); ImGui::SameLine();
						iChange += ImGui::InputFloat("Z ##scale", &m_MousePicker.m_pSeletedObj->obj_scale.z);
						//오브젝트 위치, 회전, 스케일 변경시 적용되는 조건문
						if (iChange > 0)
						{
							m_MousePicker.m_pSeletedObj->UpdateData();
							m_MousePicker.m_pSeletedObj->UpdateCollision();
							//오브젝트 이동시 새로운 노드에 넣는다.
							//기존 노드에 오브젝트 nullptr
							m_Terrian_Space.UpdateObject();
						}
						if (ImGui::Button(u8"선택된 오브젝트 삭제"))
						{
							m_Terrian_Space.DeleteObject(m_MousePicker.m_pSeletedObj->obj_name);
							m_MousePicker.m_pSeletedObj = nullptr;
						}
						ImGui::PopItemWidth();
					}
					else
					{
						ImGui::TextColored(ImVec4(1, 0, 1, 1), u8"[ 현재 선택된 오브젝트가 없습니다.]");
					}
					ImGui::Dummy(ImVec2(0.0f, 5.0f));
					//오브젝트 추가 버튼----------------------------------------------------------------
					if (ImGui::Button(u8"오브젝트 추가"))
					{
						m_MousePicker.m_pSeletedObj = nullptr;
						m_bImguiAddObject = true;
						m_bImguiAddObject_Random = false;
					}
					ImGui::SameLine();
					if (ImGui::Button(u8"오브젝트 추가(랜덤)"))
					{
						m_MousePicker.m_pSeletedObj = nullptr;
						m_bImguiAddObject_Random = true;
						m_bImguiAddObject = false;
					}
					if (m_bImguiAddObject || m_bImguiAddObject_Random)
					{
						ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), u8"추가 할 오브젝트 선택");
						if (ImGui::ListBoxHeader("##ASSETLIST", ImVec2(150, 0)))
						{
							for (auto it : m_Terrian_Space.m_FBXAssetList)
							{
								if (ImGui::Selectable(to_wm(it->m_ObjName).c_str()))
								{
									if (m_bImguiAddObject)
									{
										m_Terrian_Space.SetupObject(it);
										m_bImguiAddObject_Random = false;
										m_bImguiAddObject = false;
									}
									if (m_bImguiAddObject_Random)
									{
										m_Terrian_Space.RandomSetupObject(static_cast<K3DAsset*>(it), 10);
										m_bImguiAddObject_Random = false;
										m_bImguiAddObject = false;
									}
								}
							}
							ImGui::ListBoxFooter();
						}
					}
					else
					{
						ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), u8"맵에 배치된 오브젝트 리스트");
						if (ImGui::ListBoxHeader("##OBJECTLIST", ImVec2(150, 0)))
						{
							for (auto it : m_Terrian_Space.m_ObjectMap)
							{
								if (ImGui::Selectable(to_wm(it.first).c_str()))
								{
									m_MousePicker.m_pSeletedObj = it.second;
								}
							}
							ImGui::ListBoxFooter();
						}
					}
#pragma endregion
				}
				//맵 지형
				else if (m_MousePicker.m_iControlState == 1)
				{
					ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), u8"브러쉬 타입");
					if (ImGui::RadioButton("UP", (m_MousePicker.m_iBrushState == 0)))
					{
						m_MousePicker.m_iBrushState = 0;
					}ImGui::SameLine();
					if (ImGui::RadioButton("DOWN", (m_MousePicker.m_iBrushState == 1)))
					{
						m_MousePicker.m_iBrushState = 1;

					}ImGui::SameLine();
					if (ImGui::RadioButton("SMOOTH", (m_MousePicker.m_iBrushState == 2)))
					{
						m_MousePicker.m_iBrushState = 2;
					}ImGui::SameLine();
					if (ImGui::RadioButton("FLATTEN", (m_MousePicker.m_iBrushState == 3)))
					{
						m_MousePicker.m_iBrushState = 3;
					}
				}
				//텍스쳐
				else if (m_MousePicker.m_iControlState == 2)
				{
#pragma region 텍스쳐 IMGUI
					ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), u8"텍스쳐");

					//마우스 선택, 리스트 선택 오브젝트---------------------------------------------------------
					ImGui::PushItemWidth(50);
					ImGui::TextColored(ImVec4(1, 1, 0, 1), u8"해당 레이어 텍스쳐 : "); ImGui::SameLine();
					ImGui::TextColored(ImVec4(1, 1, 0, 1), to_wm(m_Terrian.m_pSubTextureList[m_Terrian_Sprite.m_Pickbuffer.iIndex]->m_Name).c_str());
					ImGui::PopItemWidth();
					//텍스쳐 4개 중 선택
					ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), u8"레이어 선택");
					if (ImGui::RadioButton("0", m_Terrian_Sprite.m_Pickbuffer.iIndex == 0))
					{
						m_Terrian_Sprite.m_Pickbuffer.iIndex = 0;
					}ImGui::SameLine();
					if (ImGui::RadioButton("1", m_Terrian_Sprite.m_Pickbuffer.iIndex == 1))
					{
						m_Terrian_Sprite.m_Pickbuffer.iIndex = 1;

					}ImGui::SameLine();
					if (ImGui::RadioButton("2", m_Terrian_Sprite.m_Pickbuffer.iIndex == 2))
					{
						m_Terrian_Sprite.m_Pickbuffer.iIndex = 2;
					}ImGui::SameLine();
					if (ImGui::RadioButton("3", m_Terrian_Sprite.m_Pickbuffer.iIndex == 3))
					{
						m_Terrian_Sprite.m_Pickbuffer.iIndex = 3;
					}
					ImGui::Dummy(ImVec2(0.0f, 5.0f));

					ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), u8"텍스쳐 선택");
					if (ImGui::ListBoxHeader("##TEXTURELIST"))
					{
						for (auto it : m_TextureList)
						{
							if (ImGui::Selectable(to_wm(it->m_Name).c_str()))
							{
								m_Terrian.m_pSubTextureList[m_Terrian_Sprite.m_Pickbuffer.iIndex] = it;
							}
						}
						ImGui::ListBoxFooter();
					}
#pragma endregion
				}
				if (m_MousePicker.m_iControlState == 1 || m_MousePicker.m_iControlState == 2)
				{
					ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), u8"브러쉬 조절");
					ImGui::SliderFloat("Brush Size", &m_MousePicker.m_Sel_Brush_Size, 50, 150);
					ImGui::SliderFloat("Brush Strenght", &m_MousePicker.m_Sel_Brush_Strenght, 1, 100);
				}
#pragma endregion
			}
			ImGui::EndChild();
		}
		//----------------------------------------------------------------------------------------
		if (ImGui::CollapsingHeader("[ LOD, Frustum Culling ]"))
		{
			if (ImGui::BeginChild("LOD", ImVec2(0, 150), true))
			{
				#pragma region LOD 프러스텀 컬링 공간분활 IMGUI
				if (ImGui::Button("Terrian Box Enable"))
				{
					m_Terrian_Space.SetDrawDebug();
				}
				ImGui::Text("Detected Nodes: %d", m_Terrian_Space.m_pDrawableLeafList.size());
				ImGui::Text("Detected Objects: %d", m_Terrian_Space.m_ObjectList_Static.size());
				ImGui::Text(u8"LOD Dst"); ImGui::SameLine();
				ImGui::InputFloat("##Distance", &m_Terrian_Space.m_fStartDistance, 2);
				ImGui::Text(u8"LOD Mul"); ImGui::SameLine();
				ImGui::InputFloat("##Multiple", &m_Terrian_Space.m_fDistance_Multiply, 2);
#pragma endregion
			}
			ImGui::EndChild();
		}
		//----------------------------------------------------------------------------------------	
		float* lightColor[3] = { &m_Light.m_vLightColor.x,&m_Light.m_vLightColor.y,&m_Light.m_vLightColor.z };
		if (ImGui::CollapsingHeader("[ Light ]"))
		{
			if (ImGui::BeginChild("light", ImVec2(0, 150), true))
			{
				#pragma region 씬 라이트 IMGUI
				ImGui::SliderFloat("X", &m_Light.m_vPos.x, -200.0f, 200.0f);
				ImGui::SliderFloat("Z", &m_Light.m_vPos.z, -200.0f, 200.0f);
				ImGui::InputFloat3("Color", *lightColor, 2, 0);
#pragma endregion
			}
			ImGui::EndChild();
		}
		//----------------------------------------------------------------------------------------	
	}
	ImGui::End();
#pragma endregion
}
void KScene_Maptool::ImguiSaveLoad()
{
	bool           m_bImguiLoad=false;
	bool           m_bImguiSave=false;
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::MenuItem("Load", NULL))
				m_bImguiLoad = true;
			if (ImGui::MenuItem("Save", NULL))
				m_bImguiSave = true;

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	if (m_bImguiLoad)
		ImGui::OpenPopup("Open File");
	if (m_bImguiSave)
		ImGui::OpenPopup("Save File");
	
	//경로 설정 확인 후 작업 진행, 파일로드
	if (g_ImGui.m_FileDialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".kmap"))
	{
		m_Terrian_Util.LoadKMap(g_ImGui.m_FileDialog.selected_path);
		//초기화
		m_Terrian.Release();m_Terrian_Space.Release();m_Terrian_Sprite.Release();m_MousePicker.Release();
		
		//다시 로드
		m_Terrian.Init(m_pContext, m_Terrian_Util.m_Mapsize[0], m_Terrian_Util.m_Mapsize[1], m_Terrian_Util.m_MapHeight);
		m_Terrian.CreateObject(L"../../data/shader/VSPS_Terrain.hlsl", L"../../data/shader/VSPS_Terrain.hlsl", L"../../data/map/texture/base.jpg",
			L"../../data/map/texture/base_s.jpg", L"../../data/map/texture/base_n.jpg");

		m_Terrian_Sprite.SetAlphaFileName(m_Terrian_Util.m_AlphaTexture);
		m_Terrian_Sprite.Init(m_pContext, &m_Terrian); // 지형 스프라이팅 GPU 컴퓨터 쉐이딩을 사용한다.

		m_Terrian.m_pSubTextureList.push_back(g_TextureMananger.Load(m_Terrian_Util.m_SubTexture[0]));
		m_Terrian.m_pSubTextureList.push_back(g_TextureMananger.Load(m_Terrian_Util.m_SubTexture[1]));
		m_Terrian.m_pSubTextureList.push_back(g_TextureMananger.Load(m_Terrian_Util.m_SubTexture[2]));
		m_Terrian.m_pSubTextureList.push_back(g_TextureMananger.Load(m_Terrian_Util.m_SubTexture[3]));
		m_Terrian_Space.Build(&m_Terrian, g_SceneManager.m_pCamera); // 공간분할
		m_Terrian_Space.DrawDebugInit(m_pContext);

		for (auto pfbx : m_FBXList)
		{
			m_Terrian_Space.m_FBXAssetList.push_back(pfbx);
		}
		//저장된 오브젝트 배치
		for (auto mapObject : m_Terrian_Util.m_MapObject)
		{
			std::size_t pos = mapObject.first.rfind(L"#");
			std::wstring objName = mapObject.first.substr(0, pos);

			for (auto list : m_Terrian_Space.m_FBXAssetList)
			{
				if (list->m_ObjName == objName)
				{
					m_Terrian_Space.SetupObject(list,
						KVector3(mapObject.second._11, mapObject.second._12, mapObject.second._13),
						KVector3(mapObject.second._21, mapObject.second._22, mapObject.second._23), 
						KVector3(mapObject.second._31, mapObject.second._32, mapObject.second._33));
					break;
				}
			}
		}

		//마우스 피커------------------------------------------------------------
		m_MousePicker.Init(m_pContext, &m_Terrian_Space, g_SceneManager.m_pCamera);
	}
	//파일 저장
	if (g_ImGui.m_FileDialog.showFileDialog("Save File", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(700, 310), ".kmap"))
	{
		//저장 구현
		//맵의 높이가 아닌 노드의 높이로 조절중이여서 
		//노드 높이와 맵 높이를 다시 계산해줌.
		m_Terrian_Util.SaveKMap(&m_Terrian_Space, g_ImGui.m_FileDialog.selected_path);
		g_TextureMananger.SaveFile(m_pContext, to_mw(g_ImGui.m_FileDialog.selected_path), m_Terrian_Sprite.m_pTextureCopy.Get());
	}
}
bool KScene_Maptool::Load(std::wstring file)
{
	//todo:파일입출력 맵 구성
	return true;
}

bool KScene_Maptool::Init(ID3D11DeviceContext* context)
{
	KScene::Init(context);
	m_SceneID = S_MapTool;

	//미니맵-------------------------------------------------------------
	m_MiniMap_DebugShadow.Init(KRect(KVector2(-0.25f, -0.75f),0.5f, 0.5f));
	m_MiniMap_DebugCamera.Init(KRect(KVector2(-0.75f, -0.75f), 0.5f, 0.5f));
	
	//미니맵은 키입력으로 보이게 시작부터 보이지 않음
	m_MiniMap_DebugShadow.SwapVisibility();
	m_MiniMap_DebugCamera.SwapVisibility();
	
	//Fbx 파일 로드-------------------------------------------------------------
	g_FBXManager.LoadTextureData(L"../../data/model/FBXData.txt"); //전체 텍스쳐 정보를 가진 파일 파싱
	std::vector<std::wstring> fbx_name_list;
	KDirParser::LoadAllPath(L"../../data/model/Mesh", fbx_name_list); // 해당 경로 FBX 파일 이름 파싱
	m_FBXList.resize(fbx_name_list.size());
	for (int iObj = 0; iObj < m_FBXList.size(); iObj++)
	{
		KFBXAsset* pFbx = new KFBXAsset;
		pFbx->Init();
 		pFbx->m_pLoader = g_FBXManager.Load(fbx_name_list[iObj]);
		pFbx->m_ObjName = pFbx->m_pLoader->m_Name;
		pFbx->m_VertexList = pFbx->m_pLoader->m_MeshList[0]->m_pSubVertexList[0];
		pFbx->GenAABB();
		m_FBXList[iObj]= pFbx;
	}
	//지형 텍스쳐 로드----------------------------------------------------------------
	std::vector<std::wstring> tex_name_list;
	KDirParser::LoadAllPath(L"../../data/map/texture", tex_name_list);
 	m_TextureList.resize(tex_name_list.size());
	for (int iTex = 0; iTex < m_TextureList.size(); iTex++)
	{
		m_TextureList[iTex] = g_TextureMananger.Load(tex_name_list[iTex]);
	}

	//지형-------------------------------------------------------------
	m_Terrian.Init(m_pContext, L"../../data/map/129_heightmap.jpg");
	m_Terrian.CreateObject(L"../../data/shader/VSPS_Terrain.hlsl", L"../../data/shader/VSPS_Terrain.hlsl", L"../../data/map/texture/base.jpg",
		L"../../data/map/texture/base_s.jpg", L"../../data/map/texture/base_n.jpg");
	
	m_Terrian_Sprite.Init(m_pContext, &m_Terrian); // 지형 스프라이팅 GPU 컴퓨터 쉐이딩을 사용한다.
	m_Terrian.m_pSubTextureList.push_back(m_TextureList[0]);
	m_Terrian.m_pSubTextureList.push_back(m_TextureList[1]);
	m_Terrian.m_pSubTextureList.push_back(m_TextureList[2]);
	m_Terrian.m_pSubTextureList.push_back(m_TextureList[3]);

	m_Terrian_Space.Build(&m_Terrian, g_SceneManager.m_pCamera); // 공간분할
	m_Terrian_Space.DrawDebugInit(m_pContext);
	
	for (auto pfbx : m_FBXList)
	{
		m_Terrian_Space.m_FBXAssetList.push_back(pfbx);
	}

	//마우스 피커------------------------------------------------------------
	m_MousePicker.Init(m_pContext, &m_Terrian_Space, g_SceneManager.m_pCamera);

	//라이트 그림자----------------------------------------------------------------
	m_Light.SetLight(KVector3(10.0f,1000.0f,0.0f), KVector3(0.0f, 0.0f, 0.0f));
	m_Shadow.CreateShadow(&m_Light);

	//탑뷰 카메라 ----------------------------------------------------------------
	m_TopView.Init(m_pContext);
	m_TopView.CreateViewMatrix(KVector3(0, 2000.0f, -1),KVector3(0, 0, 0));
	m_TopView.CreateProjMatrix(1.0f, 10000.0f, XM_PI * 0.3f,
		static_cast<float>(g_rtClient.right) / static_cast<float>(g_rtClient.bottom));
	return true;
}

bool KScene_Maptool::Frame()
{
	m_MousePicker.Frame();
	m_Light.Frame();
	m_Shadow.Frame(); // 쉐도우 행렬 계산, 프로젝션 행렬 ,텍스쳐 행렬 곱한것
	m_Terrian_Space.Frame();
	
	ImguiInspector();
	ImguiSaveLoad();
	
	if (m_MousePicker.m_iControlState == m_MousePicker.C_Texture && !m_MousePicker.m_bImgui)
	{
		m_Terrian_Sprite.UpdatePickPos(m_MousePicker.m_vIntersect, m_MousePicker.m_Sel_Brush_Size);
		m_Terrian_Sprite.Frame();
	}
	KScene::Frame();
	return true;
}

bool KScene_Maptool::Render()
{
	//그림자 ------------------------------------------
	float shadow[4] = { 1.0f,1.0f,1.0f,1.0f };
	if (m_Shadow.m_ShadowRT.Begin(m_pContext, shadow))
	{
		//라이트 방향에서 캡쳐
		m_Terrian.SetMatrix(&m_Terrian.m_matWorld,&m_Light.m_matView, &m_Light.m_matProj);
		//쉐이더 셰이더로 교체
		m_Terrian.SwapPSShader(m_Shadow.m_pPSShadow);
		m_Terrian_Space.Render(m_pContext);
		//오브젝트
		for (auto obj : m_Terrian_Space.m_ObjectList_Static)
		{
			obj->obj_pObject->SetMatrix(&obj->obj_matWorld, &m_Light.m_matView, &m_Light.m_matProj);
			obj->obj_pObject->SwapPSShader(m_Shadow.m_pPSShadow);
			obj->obj_pObject->Render(m_pContext);
		}
		//복원 작업
		m_Shadow.m_ShadowRT.End(m_pContext);
	}
	m_Shadow.m_Shadow_cbData.m_matShadow = m_Shadow.m_Shadow_cbData.m_matShadow.Transpose();
	m_pContext->UpdateSubresource(
		m_Shadow.m_pShadowCB.Get(), 0, NULL, &m_Shadow.m_Shadow_cbData, 0, 0);
	//상수버퍼 2번째 슬롯에 넣음
	m_pContext->VSSetConstantBuffers(2, 1, m_Shadow.m_pShadowCB.GetAddressOf());
	//샘플러 상태 : 클램프 -> 그림자용
	ApplySS(m_pContext, KState::g_pClampSS, 1);
	D3DKMatrixInverse(&m_Terrian.m_cbData.matNormal, NULL,
		&m_Terrian.m_matWorld);
	
	//지형 렌더------------------------------------------------------------
	m_Terrian.SetMatrix(&m_Terrian.m_matWorld, &g_SceneManager.m_pCamera->m_matView, &g_SceneManager.m_pCamera->m_matProj);
	m_Terrian.m_cbDataEX.vLightColor = { m_Light.m_vLightColor.x,m_Light.m_vLightColor.y,m_Light.m_vLightColor.z,1.0f };
	m_Terrian.m_cbDataEX.vLightPos = { m_Light.m_vPos.x,m_Light.m_vPos.y,m_Light.m_vPos.z,1.0f};
	m_Terrian.m_cbDataEX.vLightDir =   { m_Light.m_vDir.x,m_Light.m_vDir.y,m_Light.m_vDir.z,1.0f };
	m_Terrian.m_cbDataEX.vCamPos = { g_SceneManager.m_pCamera->GetCameraPos()->x, g_SceneManager.m_pCamera->GetCameraPos()->y, g_SceneManager.m_pCamera->GetCameraPos()->z, 1.0f };
	m_pContext->PSSetShaderResources(3, 1, m_Shadow.m_ShadowRT.m_pTextureSRV.GetAddressOf());
	m_Terrian_Space.Render(m_pContext);

	//지형 오브젝트 렌더-----------------------------------------------------
	for (auto obj : m_Terrian_Space.m_ObjectList_Static)
	{
		D3DKMatrixInverse(&obj->obj_pObject->m_cbData.matNormal, NULL,
			&obj->obj_matWorld);
		obj->obj_pObject->SetMatrix(&obj->obj_matWorld, &g_SceneManager.m_pCamera->m_matView, &g_SceneManager.m_pCamera->m_matProj);
		obj->obj_pObject->m_cbDataEX.vLightColor = { m_Light.m_vLightColor.x,m_Light.m_vLightColor.y,m_Light.m_vLightColor.z,1.0f };
		obj->obj_pObject->m_cbDataEX.vLightPos = { m_Light.m_vPos.x,m_Light.m_vPos.y,m_Light.m_vPos.z,1.0f };
		obj->obj_pObject->m_cbDataEX.vLightDir = { m_Light.m_vDir.x,m_Light.m_vDir.y,m_Light.m_vDir.z,1.0f };
		obj->obj_pObject->m_cbDataEX.vCamPos = { g_SceneManager.m_pCamera->GetCameraPos()->x, g_SceneManager.m_pCamera->GetCameraPos()->y, g_SceneManager.m_pCamera->GetCameraPos()->z, 1.0f };
		m_pContext->PSSetShaderResources(3, 1, m_Shadow.m_ShadowRT.m_pTextureSRV.GetAddressOf());
		obj->obj_pObject->SwapPSShader();
		obj->obj_pObject->Render(m_pContext);
	}

	////미니맵------------------------------------------------
	//if (g_InputData.bDebugRender)
	//{
	//	m_MiniMap_DebugShadow.SwapVisibility();
	//	m_MiniMap_DebugCamera.SwapVisibility();
	//}
	//m_MiniMap_DebugShadow.SetMatrix(nullptr, nullptr, nullptr);
	//m_MiniMap_DebugShadow.PreRender(m_pContext);
	//m_pContext->PSSetShaderResources(0, 1, m_Shadow.m_ShadowRT.m_pTextureSRV.GetAddressOf());
	//m_MiniMap_DebugShadow.PostRender(m_pContext, m_MiniMap_DebugShadow.m_iNumIndex);

	//float color[4] = { 0.2f,0.2f,0.2f,1.0f };
	//if (m_MiniMap_DebugCamera.m_Rt.Begin(m_pContext, color))
	//{
	//	ApplyBS(m_pContext, KState::g_pAlphaBlendState);
	//		m_Terrian.SetMatrix(&m_Terrian.m_matWorld, &m_TopView.m_matView, &m_TopView.m_matProj);
	//		m_Terrian_Space.Render(m_pContext); //맵 렌더
	//		for (auto obj : m_Terrian_Space.m_ObjectList_Static)
	//		{
	//			obj->obj_pObject->SetMatrix(&obj->obj_matWorld, &m_TopView.m_matView, &m_TopView.m_matProj);
	//			obj->obj_pObject->Render(m_pContext);
	//		}
	//		g_SceneManager.m_pCamera->SetMatrix(nullptr, &m_TopView.m_matView,
	//		&m_TopView.m_matProj);
	//		g_SceneManager.m_pCamera->Render(m_pContext); //프러스텀 렌더
	//	m_MiniMap_DebugCamera.m_Rt.End(m_pContext);
	//	KState::g_pCurrentBS = KState::g_pBlendState;
	//}
	//m_MiniMap_DebugCamera.Render(m_pContext);
	

	//마우스 피커--------------------------------------
	m_MousePicker.m_Sel_BoxRender.m_RenderBox.SetMatrix(&m_MousePicker.m_Sel_BoxRender.m_RenderBox.m_matWorld,
		&g_SceneManager.m_pCamera->m_matView, &g_SceneManager.m_pCamera->m_matProj);
	m_MousePicker.Render(m_pContext);
	KScene::Render();
	return true;
}

bool KScene_Maptool::Release()
{
	m_FbxLoader.Release();
	for (auto fbx_object : m_FBXList)
	{
		fbx_object->Release();
		delete fbx_object;
	}
	m_FBXList.clear();
	//
	m_TopView.Release();
	m_Terrian.Release();
	m_Terrian_Space.Release();
	m_Terrian_Sprite.Release();
	m_MousePicker.Release();
	m_MiniMap_DebugCamera.Release();
	m_MiniMap_DebugShadow.Release();
	KScene::Release();
	return true;
}
