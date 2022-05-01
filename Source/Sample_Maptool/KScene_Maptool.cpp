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
		// ���� : ������Ʈ ��ġ, ����, �ؽ���--------------------------------------------------------
		if (ImGui::CollapsingHeader("[ MapTool ]"))
		{
			if (ImGui::BeginChild("chlid_terrain", ImVec2(0, 300.0f), true))
			{
#pragma region ���� IMGUI
				//--------------------------------------------------------
				//���� ��ư
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
				//������Ʈ
				if (m_MousePicker.m_iControlState == 0)
				{
#pragma region ������Ʈ IMGUI
					//���콺 ����, ����Ʈ ���� ������Ʈ---------------------------------------------------------
					ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), u8"������Ʈ");
					if (m_MousePicker.m_pSeletedObj != nullptr)
					{
						int iChange = 0;
						ImGui::PushItemWidth(50);
						ImGui::TextColored(ImVec4(1, 1, 0, 1), u8"���õ� ������Ʈ : "); ImGui::SameLine();
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
						//������Ʈ ��ġ, ȸ��, ������ ����� ����Ǵ� ���ǹ�
						if (iChange > 0)
						{
							m_MousePicker.m_pSeletedObj->UpdateData();
							m_MousePicker.m_pSeletedObj->UpdateCollision();
							//������Ʈ �̵��� ���ο� ��忡 �ִ´�.
							//���� ��忡 ������Ʈ nullptr
							m_Terrian_Space.UpdateObject();
						}
						if (ImGui::Button(u8"���õ� ������Ʈ ����"))
						{
							m_Terrian_Space.DeleteObject(m_MousePicker.m_pSeletedObj->obj_name);
							m_MousePicker.m_pSeletedObj = nullptr;
						}
						ImGui::PopItemWidth();
					}
					else
					{
						ImGui::TextColored(ImVec4(1, 0, 1, 1), u8"[ ���� ���õ� ������Ʈ�� �����ϴ�.]");
					}
					ImGui::Dummy(ImVec2(0.0f, 5.0f));
					//������Ʈ �߰� ��ư----------------------------------------------------------------
					if (ImGui::Button(u8"������Ʈ �߰�"))
					{
						m_MousePicker.m_pSeletedObj = nullptr;
						m_bImguiAddObject = true;
						m_bImguiAddObject_Random = false;
					}
					ImGui::SameLine();
					if (ImGui::Button(u8"������Ʈ �߰�(����)"))
					{
						m_MousePicker.m_pSeletedObj = nullptr;
						m_bImguiAddObject_Random = true;
						m_bImguiAddObject = false;
					}
					if (m_bImguiAddObject || m_bImguiAddObject_Random)
					{
						ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), u8"�߰� �� ������Ʈ ����");
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
						ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), u8"�ʿ� ��ġ�� ������Ʈ ����Ʈ");
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
				//�� ����
				else if (m_MousePicker.m_iControlState == 1)
				{
					ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), u8"�귯�� Ÿ��");
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
				//�ؽ���
				else if (m_MousePicker.m_iControlState == 2)
				{
#pragma region �ؽ��� IMGUI
					ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), u8"�ؽ���");

					//���콺 ����, ����Ʈ ���� ������Ʈ---------------------------------------------------------
					ImGui::PushItemWidth(50);
					ImGui::TextColored(ImVec4(1, 1, 0, 1), u8"�ش� ���̾� �ؽ��� : "); ImGui::SameLine();
					ImGui::TextColored(ImVec4(1, 1, 0, 1), to_wm(m_Terrian.m_pSubTextureList[m_Terrian_Sprite.m_Pickbuffer.iIndex]->m_Name).c_str());
					ImGui::PopItemWidth();
					//�ؽ��� 4�� �� ����
					ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), u8"���̾� ����");
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

					ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), u8"�ؽ��� ����");
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
					ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), u8"�귯�� ����");
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
#pragma region LOD �������� �ø� ������Ȱ IMGUI
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
#pragma region �� ����Ʈ IMGUI
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
	
	//��� ���� Ȯ�� �� �۾� ����
	if (g_ImGui.m_FileDialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".kmap"))
	{
		m_Terrian_Util.LoadKMap(g_ImGui.m_FileDialog.selected_path);

		//�ʱ�ȭ
		m_Terrian.Release();
		m_Terrian_Space.Release();
		m_Terrian_Sprite.Release();
		m_MousePicker.Release();
		
		//�ٽ� �ε�
		m_Terrian.Init(m_pContext, m_Terrian_Util.m_Mapsize[0], m_Terrian_Util.m_Mapsize[1], m_Terrian_Util.m_MapHeight);
		m_Terrian.CreateObject(L"../../data/shader/VSPS_Terrain.hlsl", L"../../data/shader/VSPS_Terrain.hlsl", L"../../data/map/texture/base.jpg",
			L"../../data/map/texture/base_s.jpg", L"../../data/map/texture/base_n.jpg");

		m_Terrian_Sprite.Init(m_pContext, &m_Terrian); // ���� ���������� GPU ��ǻ�� ���̵��� ����Ѵ�.
		m_Terrian.m_pSubTextureList.push_back(m_TextureList[0]);
		m_Terrian.m_pSubTextureList.push_back(m_TextureList[1]);
		m_Terrian.m_pSubTextureList.push_back(m_TextureList[2]);
		m_Terrian.m_pSubTextureList.push_back(m_TextureList[3]);

		m_Terrian_Space.Build(&m_Terrian, g_SceneManager.m_pCamera); // ��������
		m_Terrian_Space.DrawDebugInit(m_pContext);

		for (auto pfbx : m_FBXList)
		{
			m_Terrian_Space.m_FBXAssetList.push_back(pfbx);
		}

		//���콺 ��Ŀ------------------------------------------------------------
		m_MousePicker.Init(m_pContext, &m_Terrian_Space, g_SceneManager.m_pCamera);
	}
	if (g_ImGui.m_FileDialog.showFileDialog("Save File", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(700, 310), ".kmap"))
	{
		m_Terrian_Util.SaveKMap(&m_Terrian_Space, g_ImGui.m_FileDialog.selected_path);
		//Do writing of files based on extension here
	}
}
bool KScene_Maptool::Load(std::wstring file)
{
	//todo:��������� �� ����
	return true;
}

bool KScene_Maptool::Init(ID3D11DeviceContext* context)
{
	KScene::Init(context);
	m_SceneID = S_MapTool;

	//�̴ϸ�-------------------------------------------------------------
	m_MiniMap_DebugShadow.Init(KRect(KVector2(-0.25f, -0.75f),0.5f, 0.5f));
	m_MiniMap_DebugCamera.Init(KRect(KVector2(-0.75f, -0.75f), 0.5f, 0.5f));
	
	//�̴ϸ��� Ű�Է����� ���̰� ���ۺ��� ������ ����
	m_MiniMap_DebugShadow.SwapVisibility();
	m_MiniMap_DebugCamera.SwapVisibility();
	
	//Fbx ���� �ε�-------------------------------------------------------------
	g_FBXManager.LoadTextureData(L"../../data/model/FBXData.txt"); //��ü �ؽ��� ������ ���� ���� �Ľ�
	std::vector<std::wstring> fbx_name_list;
	KDirParser::LoadAllPath(L"../../data/model/Mesh", fbx_name_list); // �ش� ��� FBX ���� �̸� �Ľ�
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
	//���� �ؽ��� �ε�----------------------------------------------------------------
	std::vector<std::wstring> tex_name_list;
	KDirParser::LoadAllPath(L"../../data/map/texture", tex_name_list);
 	m_TextureList.resize(tex_name_list.size());
	for (int iTex = 0; iTex < m_TextureList.size(); iTex++)
	{
		m_TextureList[iTex] = g_TextureMananger.Load(tex_name_list[iTex]);
	}

	//����-------------------------------------------------------------
	m_Terrian.Init(m_pContext, L"../../data/map/129_heightmap.jpg");
	m_Terrian.CreateObject(L"../../data/shader/VSPS_Terrain.hlsl", L"../../data/shader/VSPS_Terrain.hlsl", L"../../data/map/texture/base.jpg",
		L"../../data/map/texture/base_s.jpg", L"../../data/map/texture/base_n.jpg");
	
	m_Terrian_Sprite.Init(m_pContext, &m_Terrian); // ���� ���������� GPU ��ǻ�� ���̵��� ����Ѵ�.
	m_Terrian.m_pSubTextureList.push_back(m_TextureList[0]);
	m_Terrian.m_pSubTextureList.push_back(m_TextureList[1]);
	m_Terrian.m_pSubTextureList.push_back(m_TextureList[2]);
	m_Terrian.m_pSubTextureList.push_back(m_TextureList[3]);

	m_Terrian_Space.Build(&m_Terrian, g_SceneManager.m_pCamera); // ��������
	m_Terrian_Space.DrawDebugInit(m_pContext);
	
	for (auto pfbx : m_FBXList)
	{
		m_Terrian_Space.m_FBXAssetList.push_back(pfbx);
	}

	//���콺 ��Ŀ------------------------------------------------------------
	m_MousePicker.Init(m_pContext, &m_Terrian_Space, g_SceneManager.m_pCamera);

	//����Ʈ �׸���----------------------------------------------------------------
	m_Light.SetLight(KVector3(10.0f,1000.0f,0.0f), KVector3(0.0f, 0.0f, 0.0f));
	m_Shadow.CreateShadow(&m_Light);

	//ž�� ī�޶� ----------------------------------------------------------------
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
	m_Shadow.Frame(); // ������ ��� ���, �������� ��� ,�ؽ��� ��� ���Ѱ�
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
	//�׸��� ------------------------------------------
	float shadow[4] = { 1.0f,1.0f,1.0f,1.0f };
	if (m_Shadow.m_ShadowRT.Begin(m_pContext, shadow))
	{
		//����Ʈ ���⿡�� ĸ��
		m_Terrian.SetMatrix(&m_Terrian.m_matWorld,&m_Light.m_matView, &m_Light.m_matProj);
		//���̴� ���̴��� ��ü
		m_Terrian.SwapPSShader(m_Shadow.m_pPSShadow);
		m_Terrian_Space.Render(m_pContext);
		//������Ʈ
		for (auto obj : m_Terrian_Space.m_ObjectList_Static)
		{
			obj->obj_pObject->SetMatrix(&obj->obj_matWorld, &m_Light.m_matView, &m_Light.m_matProj);
			obj->obj_pObject->SwapPSShader(m_Shadow.m_pPSShadow);
			obj->obj_pObject->Render(m_pContext);
		}
		//���� �۾�
		m_Shadow.m_ShadowRT.End(m_pContext);
	}
	m_Shadow.m_Shadow_cbData.m_matShadow = m_Shadow.m_Shadow_cbData.m_matShadow.Transpose();
	m_pContext->UpdateSubresource(
		m_Shadow.m_pShadowCB.Get(), 0, NULL, &m_Shadow.m_Shadow_cbData, 0, 0);
	//������� 2��° ���Կ� ����
	m_pContext->VSSetConstantBuffers(2, 1, m_Shadow.m_pShadowCB.GetAddressOf());
	//���÷� ���� : Ŭ���� -> �׸��ڿ�
	ApplySS(m_pContext, KState::g_pClampSS, 1);
	D3DKMatrixInverse(&m_Terrian.m_cbData.matNormal, NULL,
		&m_Terrian.m_matWorld);
	
	//���� ����------------------------------------------------------------
	m_Terrian.SetMatrix(&m_Terrian.m_matWorld, &g_SceneManager.m_pCamera->m_matView, &g_SceneManager.m_pCamera->m_matProj);
	m_Terrian.m_cbDataEX.vLightColor = { m_Light.m_vLightColor.x,m_Light.m_vLightColor.y,m_Light.m_vLightColor.z,1.0f };
	m_Terrian.m_cbDataEX.vLightPos = { m_Light.m_vPos.x,m_Light.m_vPos.y,m_Light.m_vPos.z,1.0f};
	m_Terrian.m_cbDataEX.vLightDir =   { m_Light.m_vDir.x,m_Light.m_vDir.y,m_Light.m_vDir.z,1.0f };
	m_Terrian.m_cbDataEX.vCamPos = { g_SceneManager.m_pCamera->GetCameraPos()->x, g_SceneManager.m_pCamera->GetCameraPos()->y, g_SceneManager.m_pCamera->GetCameraPos()->z, 1.0f };
	m_pContext->PSSetShaderResources(3, 1, m_Shadow.m_ShadowRT.m_pTextureSRV.GetAddressOf());
	m_Terrian_Space.Render(m_pContext);

	//���� ������Ʈ ����-----------------------------------------------------
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

	////�̴ϸ�------------------------------------------------
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
	//		m_Terrian_Space.Render(m_pContext); //�� ����
	//		for (auto obj : m_Terrian_Space.m_ObjectList_Static)
	//		{
	//			obj->obj_pObject->SetMatrix(&obj->obj_matWorld, &m_TopView.m_matView, &m_TopView.m_matProj);
	//			obj->obj_pObject->Render(m_pContext);
	//		}
	//		g_SceneManager.m_pCamera->SetMatrix(nullptr, &m_TopView.m_matView,
	//		&m_TopView.m_matProj);
	//		g_SceneManager.m_pCamera->Render(m_pContext); //�������� ����
	//	m_MiniMap_DebugCamera.m_Rt.End(m_pContext);
	//	KState::g_pCurrentBS = KState::g_pBlendState;
	//}
	//m_MiniMap_DebugCamera.Render(m_pContext);
	

	//���콺 ��Ŀ--------------------------------------
	m_MousePicker.m_Sel_BoxRender.m_RenderBox.SetMatrix(&m_MousePicker.m_Sel_BoxRender.m_RenderBox.m_matWorld,
		&g_SceneManager.m_pCamera->m_matView, &g_SceneManager.m_pCamera->m_matProj);
	m_MousePicker.Render(m_pContext);
	KScene::Render();
	return true;
}

bool KScene_Maptool::Release()
{
	m_FbxLoader.Release();
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
