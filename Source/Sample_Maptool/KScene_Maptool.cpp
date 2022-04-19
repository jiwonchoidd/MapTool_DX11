#include "KScene_Maptool.h"
#include "KSceneManager.h"
#include "KState.h"
#include "KFBXManager.h"
#include "ImGuiManager.h"
#include "FileDirParser.h"
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
	std::vector<std::wstring> fbx_name_list;
	FileDirParser::LoadAllPath(L"../../data/model/Map/Mesh", fbx_name_list);
	m_FBXList.resize(fbx_name_list.size());
	for (int iObj = 0; iObj < m_FBXList.size(); iObj++)
	{
		std::shared_ptr<KFBXAsset> pFbx = std::make_shared<KFBXAsset>();
		pFbx->Init();
		//float fHeight = m_Terrian.GetHeight(3, 3);
		pFbx->SetPosition(KVector3(3 * iObj * 10, 3, 3 * iObj));
 		pFbx->m_pLoader = g_FBXManager.Load(fbx_name_list[iObj]);
		m_FBXList[iObj]= pFbx;
	}
	//�ؽ��� �ε�----------------------------------------------------------------
	std::vector<std::wstring> tex_name_list;
	FileDirParser::LoadAllPath(L"../../data/texture/Map", tex_name_list);
	m_TextureList.resize(tex_name_list.size());
	for (int iTex = 0; iTex < m_TextureList.size(); iTex++)
	{
		m_TextureList[iTex] = g_TextureMananger.Load(tex_name_list[iTex]);
	}

	//����-------------------------------------------------------------
	m_Terrian.Init(m_pContext, L"../../data/map/129_heightmap.jpg");
	m_Terrian.CreateObject(L"../../data/shader/VSPS_DepthShadow.hlsl", L"../../data/shader/VSPS_DepthShadow.hlsl", L"../../data/map/Ground_Grass_001_COLOR.jpg",
		L"../../data/map/Ground_Grass_001_ROUGH.jpg", L"../../data/map/Ground_Grass_001_NORM.jpg");

	m_Terrian_Space.Build(&m_Terrian, g_SceneManager.m_pCamera);
	m_Terrian_Space.DrawDebugInit(m_pContext);

	KBoxObj* tempBox = new KBoxObj();
	tempBox->m_ObjName = L"textbox";
	tempBox->Init(L"../../data/shader/VSPS_DepthShadow.hlsl", L"../../data/shader/VSPS_DepthShadow.hlsl", 
		L"../../data/texture/brick.jpg", L"../../data/texture/brick.jpg", L"../../data/texture/brick_normal.jpg");
	m_Terrian_Space.RandomSetupObject(tempBox,10);
	
	m_TopView.Init(m_pContext);
	m_TopView.CreateViewMatrix(KVector3(0, 1000.0f, -1),KVector3(0, 0, 0));
	m_TopView.CreateProjMatrix(1.0f, 10000.0f, XM_PI * 0.3f,
		static_cast<float>(g_rtClient.right) / static_cast<float>(g_rtClient.bottom));

	//����Ʈ �׸���----------------------------------------------------------------
	m_Light.SetLight(KVector3(-200.0f,1000.0f,0.0f), KVector3(0.0f, 0.0f, 0.0f));
	m_Shadow.CreateShadow(&m_Light);

	//���콺 ��Ŀ------------------------------------------------------------
	m_MousePicker.Init(m_pContext, &m_Terrian_Space, g_SceneManager.m_pCamera);

	return true;
}

bool KScene_Maptool::Frame()
{
	m_MousePicker.Frame();
	m_Light.Frame();
	m_Shadow.Frame(); // ������ ��� ���, �������� ��� ,�ؽ��� ��� ���Ѱ�

	#pragma region IMGUI UI <- HERE
	//IMGUI IU 
	ImVec2 ivMin = { static_cast<float>(g_rtClient.right)- static_cast<float>(g_rtClient.right) / 3.5f, 0 };
	ImVec2 ivMax = { static_cast<float>(g_rtClient.right), static_cast<float>(g_rtClient.bottom) };
	if (ImGui::IsMouseHoveringRect(ivMin, ivMax, false))
	{
		m_MousePicker.m_bImgui = true;
	}
	else
	{
		m_MousePicker.m_bImgui = false;
	}
	
	ImGui::SetNextWindowSize(ImVec2(ivMax.x -ivMin.x, ivMax.y - ivMin.y));
	ImGui::SetNextWindowPos(ivMin);
	if (ImGui::Begin("Inspector"))
	{
		//----------------------------------------------------------------------------------------
		// ���� : ������Ʈ ��ġ, ����, �ؽ��� 
		if (ImGui::CollapsingHeader("[ MapTool ]"))
		{
			if (ImGui::BeginChild("chlid_terrain", ImVec2(0, 300.0f), true))
			{
				#pragma region ���� IMGUI
				//--------------------------------------------------------
				//���� ��ư
				if (ImGui::RadioButton("None", (m_MousePicker.m_iControlState == 0)))
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
						ImGui::PushItemWidth(50);
						ImGui::TextColored(ImVec4(1, 1, 0, 1), u8"���õ� ������Ʈ : "); ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), to_wm(m_MousePicker.m_pSeletedObj->obj_name).c_str());
						ImGui::InputFloat("X", &m_MousePicker.m_pSeletedObj->obj_pos.x); ImGui::SameLine();
						ImGui::InputFloat("Y", &m_MousePicker.m_pSeletedObj->obj_pos.y); ImGui::SameLine();
						ImGui::InputFloat("Z", &m_MousePicker.m_pSeletedObj->obj_pos.z);
						ImGui::PopItemWidth();
					}
					else
					{
						ImGui::TextColored(ImVec4(1, 0, 1, 1), u8"[ ���� ���õ� ������Ʈ�� �����ϴ�.]");
					}
					ImGui::Dummy(ImVec2(0.0f, 5.0f));

					ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1), u8"��ġ�� ������Ʈ ����Ʈ");
					if (ImGui::ListBoxHeader("##OBJECTLIST"))
					{
						m_MousePicker.m_pSeletedObj->obj_name;
						for (auto it : m_Terrian_Space.m_ObjectMap)
						{
							if (ImGui::Selectable(to_wm(it.first).c_str()))
							{
								m_MousePicker.m_pSeletedObj = it.second;
							}
						}
						ImGui::ListBoxFooter();
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
					if (m_MousePicker.m_pSeletedTex != nullptr)
					{
						ImGui::PushItemWidth(50);
						ImGui::TextColored(ImVec4(1, 1, 0, 1), u8"���õ� �ؽ��� : "); ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 0, 1), to_wm(m_MousePicker.m_pSeletedTex->m_Name).c_str());
						ImGui::PopItemWidth();
					}
					else
					{
						ImGui::TextColored(ImVec4(1, 0, 1, 1), u8"[ ���� ���õ� �ؽ��İ� �����ϴ�.]");
					}
					ImGui::Dummy(ImVec2(0.0f, 5.0f));

					if (ImGui::ListBoxHeader("##TEXTURELIST"))
					{
						for (auto it : m_TextureList)
						{
							if (ImGui::Selectable(to_wm(it->m_Name).c_str()))
							{
								m_MousePicker.m_pSeletedTex = it;
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
					ImGui::SliderFloat("Brush Strenght", &m_MousePicker.m_Sel_Brush_Strenght, 50, 150);
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

	m_Terrian_Space.Frame();
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
			obj->obj_pObject->PreRender(m_pContext);
			m_pContext->PSSetShader(m_Shadow.m_pPSShadow->m_pPixelShader.Get(), NULL, 0);
			obj->obj_pObject->PostRender(m_pContext,
				obj->obj_pObject->m_iNumIndex);
		}
		//FBX OBJ Render------------------------------------------
		/*for (int iObj = 0; iObj < m_Scene_FBXList.size(); iObj++)
		{
			m_Scene_FBXList[iObj]->SetMatrix(&m_Scene_FBXList[iObj]->m_matWorld, &m_Light.m_matView, &m_Light.m_matProj);
			m_Scene_FBXList[iObj]->SwapPSShader(m_Shadow.m_pPSShadow);
			m_Scene_FBXList[iObj]->Render(m_pContext);
		}*/
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
	}
	m_pContext->PSSetShaderResources(3, 1, m_Shadow.m_ShadowRT.m_pTextureSRV.GetAddressOf());
	m_Terrian_Space.Render_MapObject(m_pContext);

	//FBX OBJ Render------------------------------------------
	/*for (int iObj = 0; iObj < m_Scene_FBXList.size(); iObj++)
	{
		m_Scene_FBXList[iObj]->SetMatrix(&m_Scene_FBXList[iObj]->m_matWorld, &g_SceneManager.m_pCamera->m_matView, &g_SceneManager.m_pCamera->m_matProj);
		m_Scene_FBXList[iObj]-> m_cbDataEX.vLightColor = { m_Light.m_vLightColor.x,m_Light.m_vLightColor.y,m_Light.m_vLightColor.z,1.0f };
		m_Scene_FBXList[iObj]-> m_cbDataEX.vLightPos = { m_Light.m_vPos.x,m_Light.m_vPos.y,m_Light.m_vPos.z,1.0f };
		m_Scene_FBXList[iObj]-> m_cbDataEX.vLightDir = { m_Light.m_vDir.x,m_Light.m_vDir.y,m_Light.m_vDir.z,1.0f };
		m_Scene_FBXList[iObj]->	m_cbDataEX.vCamPos = { g_SceneManager.m_pCamera->GetCameraPos()->x, g_SceneManager.m_pCamera->GetCameraPos()->y, g_SceneManager.m_pCamera->GetCameraPos()->z, 1.0f };
		m_pContext->PSSetShaderResources(3, 1, m_Shadow.m_ShadowRT.m_pTextureSRV.GetAddressOf());
		m_Scene_FBXList[iObj]->SwapPSShader();
		m_Scene_FBXList[iObj]->Render(m_pContext);
	}*/

	////�̴ϸ�------------------------------------------------
	if (g_InputData.bDebugRender)
	{
		m_MiniMap_DebugShadow.SwapVisibility();
		m_MiniMap_DebugCamera.SwapVisibility();
	}
	m_MiniMap_DebugShadow.SetMatrix(nullptr, nullptr, nullptr);
	m_MiniMap_DebugShadow.PreRender(m_pContext);
	m_pContext->PSSetShaderResources(0, 1, m_Shadow.m_ShadowRT.m_pTextureSRV.GetAddressOf());
	m_MiniMap_DebugShadow.PostRender(m_pContext, m_MiniMap_DebugShadow.m_iNumIndex);

	float color[4] = { 0.2f,0.2f,0.2f,1.0f };
	if (m_MiniMap_DebugCamera.m_Rt.Begin(m_pContext, color))
	{
		ApplyBS(m_pContext, KState::g_pAlphaBlendState);
			m_Terrian.SetMatrix(&m_Terrian.m_matWorld, &m_TopView.m_matView, &m_TopView.m_matProj);
			m_Terrian_Space.Render(m_pContext); //�� ����
			m_Terrian_Space.Render_MapObject(m_pContext); //�� ������Ʈ
			g_SceneManager.m_pCamera->SetMatrix(nullptr, &m_TopView.m_matView,
			&m_TopView.m_matProj);
			g_SceneManager.m_pCamera->Render(m_pContext); //�������� ����
		m_MiniMap_DebugCamera.m_Rt.End(m_pContext);
		KState::g_pCurrentBS = KState::g_pBlendState;
	}
	m_MiniMap_DebugCamera.Render(m_pContext);
	

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
	m_Terrian_Space.Release();
	m_Terrian.Release();
	m_TopView.Release();
	m_MiniMap_DebugCamera.Release();
	m_MiniMap_DebugShadow.Release();
	m_MousePicker.Release();
	KScene::Release();
	return true;
}
