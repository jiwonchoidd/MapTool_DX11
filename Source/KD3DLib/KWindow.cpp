#include "KWindow.h"
#include <assert.h>
#include <sstream>
#include "resource.h"
#include "ImGui\imgui_impl_win32.h"
KWindow*    g_pWindow = nullptr;
HINSTANCE	g_hInstance;
HWND        g_hWnd;
RECT        g_rtClient;

//RESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // �޼��� �ڵ鸵
    assert(g_pWindow);
    g_pWindow->ExternMsgProc(hWnd, message, wParam, lParam);
    return g_pWindow->MsgProc(hWnd, message, wParam, lParam);    
}
LRESULT  KWindow::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    {
        return true;
    }
    switch (msg)
    {
    //WM_KEYDOWN, WM_KEYUP, WM_CHAR, WM_LBUTTONDOWN
    case WM_DESTROY:
    {
        PostQuitMessage(0);
    }break;
    case WM_SIZE:
    {
        UINT iWidth = LOWORD(lParam);
        UINT iHeight = HIWORD(lParam);
        g_pWindow->ResizeDevice(iWidth, iHeight);
    }break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

LRESULT KWindow::ExternMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return 0;
}

// 1. ������ Ŭ���� ���
// 2. ������ ����
bool   KWindow::InitWindows(
	HINSTANCE hInstance,
	int nCmdShow,
	const WCHAR* strWindowTitle, int width, int height)
{
	m_hInstance = hInstance;
    g_hInstance = m_hInstance;
    WNDCLASSEXW wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEXW));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc; //�޽��� �ڵ鷯 ���
    wcex.hInstance = hInstance;
    wcex.hbrBackground = CreateSolidBrush(RGB(0,0,0));
    wcex.lpszClassName = L"dd_ch";
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hIcon = static_cast<HICON>(LoadImage(m_hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
    wcex.hIconSm = static_cast<HICON>(LoadImage(m_hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));

    if (!RegisterClassExW(&wcex))
    {
        return false;
    }
    RECT rc = { 0, 0, width, height };
    // �۾�����(  Ÿ��Ʋ ��/��輱/�޴�/��ũ�� �� ���� ������ ������ ����), ������ ��Ÿ��, �޴�����
    // Chilil�� �̰� ����Ʈ �����̶�� ��
    //AdjustWindowRect(&rc, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
    //WS_OVERLAPPEDWINDOW
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	// ������ ���� �Լ�
	m_hWnd = CreateWindowExW(
        0,
        L"dd_ch",
        strWindowTitle,
        WS_OVERLAPPEDWINDOW,
        0,
        0,
        rc.right-rc.left,
        rc.bottom-rc.top,
        NULL,
        NULL,
        hInstance,
        NULL);
    if (m_hWnd == NULL)
    {
        return false;
    }
    GetWindowRect(m_hWnd, &m_rtWindow);
    GetClientRect(m_hWnd, &m_rtClient);
    g_hWnd = m_hWnd;
    g_rtClient = m_rtClient;
    m_hInstance = hInstance;
    g_hInstance = m_hInstance;
    // SW_SHOW, nCmdShow
    
    ShowWindow(m_hWnd, SW_SHOW);
    
    ImGui_ImplWin32_Init(m_hWnd);

	return true;
}

bool	KWindow::Run()
{
    //���̷�Ʈ �ʱ�ȭ
    GameInit();
    //�޼��� const
    MSG msg;
    while (m_bGameRun)
    {
        //
        if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            m_bGameRun = GameRun();            
        }
    }
    GameRelease();
	return true;
}

bool	KWindow::GameRun()
{
    return true;
}
bool KWindow::ResizeDevice(UINT iWidth, UINT iHeight)
{
    GetWindowRect(m_hWnd, &m_rtWindow);
    GetClientRect(m_hWnd, &m_rtClient);
    g_hWnd = m_hWnd;
    g_rtClient = m_rtClient;
    g_hInstance = m_hInstance;
    return true;
}
bool	KWindow::GameInit()
{
    return true;
}
bool	KWindow::GameRelease() 
{
    return true;
}
KWindow::KWindow() : m_bGameRun(true)
{
    g_pWindow = this;
}


KWindow::~KWindow()
{
    ImGui_ImplWin32_Shutdown();
}
   