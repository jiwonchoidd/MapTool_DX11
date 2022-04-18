#include "KScene_Maptool.h"
#include "KSceneManager.h"
#include "KState.h"
#include "KFBXManager.h"
#include "ImGuiManager.h"
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
	
	std::vector<std::wstring> listname;
	listname.push_back(L"../../data/model/SM_Rock.FBX");
	//listname.push_back(L"../../data/model/SM_Barrel.FBX");
	m_Scene_FBXList.resize(listname.size());
	for (int iObj = 0; iObj < m_Scene_FBXList.size(); iObj++)
	{
		std::shared_ptr<KFBXAsset> pFbx = std::make_shared<KFBXAsset>();
		pFbx->Init();
		pFbx->m_matWorld._11 = 0.2f;
		pFbx->m_matWorld._22 = 0.2f;
		pFbx->m_matWorld._33 = 0.2f;
		pFbx->m_matWorld._42 = 0.0f;
 		pFbx->m_pLoader = g_FBXManager.Load(listname[iObj]);
		m_Scene_FBXList[iObj]= pFbx;
		
	}

	//지형-------------------------------------------------------------
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

	//라이트 그림자----------------------------------------------------------------
	m_Light.SetLight(KVector3(200.0f,1000.0f,0.0f), KVector3(0.0f, 0.0f, 0.0f));
	m_Shadow.CreateShadow(&m_Light);

	//마우스 피커------------------------------------------------------------
	m_MousePicker.Init(m_pContext, &m_Terrian_Space, g_SceneManager.m_pCamera);

	return true;
}

bool KScene_Maptool::Frame()
{
	m_MousePicker.Frame();
	m_Light.Frame();
	m_Shadow.Frame(); // 쉐도우 행렬 계산, 프로젝션 행렬 ,텍스쳐 행렬 곱한것

	#pragma region IMGUI UI <- HERE
	//IMGUI IU 
	if (ImGui::Begin("Inspector"))
	{
		//----------------------------------------------------------------------------------------
		float* lightDir[3] = { &m_Light.m_vDir.x,&m_Light.m_vDir.y,&m_Light.m_vDir.z };
		float* lightColor[3] = { &m_Light.m_vLightColor.x,&m_Light.m_vLightColor.y,&m_Light.m_vLightColor.z };
		ImGui::Text("[ LIGHT ]");
		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		ImGui::BeginChild("light", ImVec2(0, 50));
		ImGui::SliderFloat3("Dir", *lightDir, -1.0f, 1.0f);
		ImGui::InputFloat3("Color", *lightColor, 2, 0);
		ImGui::EndChild();
		//----------------------------------------------------------------------------------------
		//지형
		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		ImGui::Text("[ Terrian ]");
		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		ImGui::BeginChild("terrian", ImVec2(0, 100));
		// 툴링 선택 라디오 버튼
		if (ImGui::RadioButton("None", (m_iImguiSelected == 0)))
		{
			m_iImguiSelected = 0;
		}ImGui::SameLine();
		if (ImGui::RadioButton("Height", (m_iImguiSelected == 1)))
		{
			m_iImguiSelected = 1;

		}ImGui::SameLine();
		if (ImGui::RadioButton("Texture", (m_iImguiSelected == 2)))
		{
			m_iImguiSelected = 2;
		}
		if (m_iImguiSelected == 1)
		{
			if (ImGui::SliderFloat("Brush Size", &m_MousePicker.m_Sel_Brush_Size, 30, 150))
			{
				m_MousePicker.m_bImgui = true;
			}
			else
			{
				m_MousePicker.m_bImgui = false;
			}
			if(ImGui::SliderFloat("Brush Strenght", &m_MousePicker.m_Sel_Brush_Strenght, -500, 500))
			{
				m_MousePicker.m_bImgui = true;
			}
			else
			{
				m_MousePicker.m_bImgui = false;
			}
		}
		ImGui::EndChild();
		//----------------------------------------------------------------------------------------
		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		ImGui::Text("[ LOD, Frustum Culling ]");
		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		ImGui::BeginChild("LOD", ImVec2(0, 150));
		// 디버깅 박스 렌더
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
		ImGui::EndChild();
		//----------------------------------------------------------------------------------------	
		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		ImGui::Text("[ Object ]");
		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		ImGui::BeginChild("object", ImVec2(0, 150));
		if (ImGui::ListBoxHeader("List"))
		{
			for (auto it : m_Terrian_Space.m_ObjectMap)
			{
				if (ImGui::Selectable(to_wm(it.first).c_str()))
				{
					int k = 0;
				}
			}
			ImGui::ListBoxFooter();
		}
		ImGui::EndChild();
		//----------------------------------------------------------------------------------------	
	}
	ImGui::End();
#pragma endregion

	m_MousePicker.m_iState = m_iImguiSelected;

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
			obj->obj_pObject->PreRender(m_pContext);
			m_pContext->PSSetShader(m_Shadow.m_pPSShadow->m_pPixelShader.Get(), NULL, 0);
			obj->obj_pObject->PostRender(m_pContext,
				obj->obj_pObject->m_iNumIndex);
		}
		//FBX OBJ Render------------------------------------------
		for (int iObj = 0; iObj < m_Scene_FBXList.size(); iObj++)
		{
			m_Scene_FBXList[iObj]->SetMatrix(&m_Scene_FBXList[iObj]->m_matWorld, &m_Light.m_matView, &m_Light.m_matProj);
			m_Scene_FBXList[iObj]->SwapPSShader(m_Shadow.m_pPSShadow);
			m_Scene_FBXList[iObj]->Render(m_pContext);
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
	}
	m_pContext->PSSetShaderResources(3, 1, m_Shadow.m_ShadowRT.m_pTextureSRV.GetAddressOf());
	m_Terrian_Space.Render_MapObject(m_pContext);

	//FBX OBJ Render------------------------------------------
	for (int iObj = 0; iObj < m_Scene_FBXList.size(); iObj++)
	{
		m_Scene_FBXList[iObj]->SetMatrix(&m_Scene_FBXList[iObj]->m_matWorld, &g_SceneManager.m_pCamera->m_matView, &g_SceneManager.m_pCamera->m_matProj);
		m_Scene_FBXList[iObj]-> m_cbDataEX.vLightColor = { m_Light.m_vLightColor.x,m_Light.m_vLightColor.y,m_Light.m_vLightColor.z,1.0f };
		m_Scene_FBXList[iObj]-> m_cbDataEX.vLightPos = { m_Light.m_vPos.x,m_Light.m_vPos.y,m_Light.m_vPos.z,1.0f };
		m_Scene_FBXList[iObj]-> m_cbDataEX.vLightDir = { m_Light.m_vDir.x,m_Light.m_vDir.y,m_Light.m_vDir.z,1.0f };
		m_Scene_FBXList[iObj]->	m_cbDataEX.vCamPos = { g_SceneManager.m_pCamera->GetCameraPos()->x, g_SceneManager.m_pCamera->GetCameraPos()->y, g_SceneManager.m_pCamera->GetCameraPos()->z, 1.0f };
		m_pContext->PSSetShaderResources(3, 1, m_Shadow.m_ShadowRT.m_pTextureSRV.GetAddressOf());
		m_Scene_FBXList[iObj]->SwapPSShader();
		m_Scene_FBXList[iObj]->Render(m_pContext);
	}

	////미니맵------------------------------------------------
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
			m_Terrian_Space.Render(m_pContext); //맵 렌더
			m_Terrian_Space.Render_MapObject(m_pContext); //맵 오브젝트
			g_SceneManager.m_pCamera->SetMatrix(nullptr, &m_TopView.m_matView,
			&m_TopView.m_matProj);
			g_SceneManager.m_pCamera->Render(m_pContext); //프러스텀 렌더
		m_MiniMap_DebugCamera.m_Rt.End(m_pContext);
		KState::g_pCurrentBS = KState::g_pBlendState;
	}
	m_MiniMap_DebugCamera.Render(m_pContext);
	

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
	m_Terrian_Space.Release();
	m_Terrian.Release();
	m_TopView.Release();
	m_MiniMap_DebugCamera.Release();
	m_MiniMap_DebugShadow.Release();
	m_MousePicker.Release();
	KScene::Release();
	return true;
}
