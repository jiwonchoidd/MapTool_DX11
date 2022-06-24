#pragma once
#include "ImGui\imgui.h"
#include "ImGui\ImGuiFileBrowser.h"
#include "ImGui\imgui_impl_dx11.h"
#include "ImGui\imgui_impl_win32.h"
//https://github.com/gallickgunner/ImGui-Addons
//dirent, gallickgunner ImGui - Addons, Public
class ImGuiManager : public Singleton <ImGuiManager>
{
private:
	friend class Singleton<ImGuiManager>;
	bool m_bImguiEnable = true;
	bool m_bCreated = false;
public:
	imgui_addons::ImGuiFileBrowser m_FileDialog; //��������� ������
public:
	void OnOffImgui();
	bool isImguiEnable();
public:
	void Init_DX(ID3D11Device* pdevice, ID3D11DeviceContext* pcontext); //���̷�Ʈ ���������� �ʱ�ȭ
	void Init_Win(HWND hwnd); // ������ Winapi �ʱ�ȭ
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

