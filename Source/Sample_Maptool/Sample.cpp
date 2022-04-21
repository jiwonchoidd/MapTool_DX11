#include "Sample.h"

bool Sample::Init()
{
	//사운드 초기화 
	g_SoundManager.Init();
	//씬 매니져
	g_SceneManager.m_pCamera = &m_Camera;
	g_SceneManager.Init(m_pImmediateContext.Get());
    return true;
}
// 프레임 함수
bool Sample::Frame()
{
	g_SceneManager.Frame();
    return true;
}
bool Sample::Render()
{
	g_SceneManager.Render();
    return true;
}
bool Sample::Release()
{
	g_SceneManager.Release();
    return true;
}

Sample::Sample()
{
}

Sample::~Sample()
{
}

WinMain_OPT(DirectX11 맵툴 [최지원], 1280, 720);
