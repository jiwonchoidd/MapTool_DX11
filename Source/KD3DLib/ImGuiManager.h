#pragma once
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_dx11.h"
#include "ImGui\imgui_impl_win32.h"
class ImGuiManager : public Singleton <ImGuiManager>
{
private:
	friend class Singleton<ImGuiManager>;
	bool m_bImguiEnable = true;
	bool m_bCreated = false;
public:
	void OnOffImgui();
	bool isImguiEnable();
public:
	void Init();
	void Frame();
	void Render();
	void Release();
public:
	ImGuiManager();
	~ImGuiManager();
};
#define g_ImGui ImGuiManager::Get()

