#include "ImGuiManager.h"
void ImGuiManager::Init()
{
    if (!m_bCreated)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 15.0f, NULL, io.Fonts->GetGlyphRangesKorean());
    }
}
void ImGuiManager::Frame()
{
    if (m_bImguiEnable)
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }
}
void ImGuiManager::Render()
{
    if (m_bImguiEnable)
    {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
}

void ImGuiManager::Release()
{
    
}

void ImGuiManager::OnOffImgui()
{
    m_bImguiEnable = !m_bImguiEnable;
}

bool ImGuiManager::isImguiEnable()
{
    return m_bImguiEnable;
}
void ImGuiManager::Init_DX(ID3D11Device* pdevice, ID3D11DeviceContext* pcontext)
{
    ImGui_ImplDX11_Init(pdevice, pcontext);
}
void ImGuiManager::Init_Win(HWND hwnd)
{
    ImGui_ImplWin32_Init(hwnd);
}
ImGuiManager::ImGuiManager()
{
    Init();
}

ImGuiManager::~ImGuiManager()
{
    ImGui::DestroyContext();
}
