#pragma once
#include "KStd.h"
#include "KDevice.h"
class KWindow : public KDevice
{
public:
	RECT m_rtWindow;
	RECT m_rtClient;

	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
	bool		m_bGameRun;
public:
	bool   InitWindows(HINSTANCE hInstance,
		int nCmdShow,
		const WCHAR* strWindowTitle=L"Untitled", int width = 800, int height = 600);
	bool	Run();
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT  ExternMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	virtual	bool	ResizeDevice(UINT iWidth, UINT iHeight);
public:
	virtual bool	GameInit();
	virtual bool	GameRun();
	virtual bool	GameRelease();
public:
	KWindow();
	~KWindow();
};

