#define DIRECTINPUT_VERSION 0x0800
#pragma once
#include "KStd.h"
#include <dinput.h>

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
//#pragma comment (lib, "uuid.lib")

enum  KeyState
{
	KEY_FREE = 0,
	KEY_UP,
	KEY_PUSH,
	KEY_HOLD,
};
class KInput : public Singleton<KInput>
{
	friend class Singleton<KInput>;
private:
	int		m_iWheel = 0;
public:
	POINT   m_ptPos;
	POINT	m_pMouseMove;
	POINT	m_pPrevMouse;
	bool	m_bMouseEnabled=true;
	bool	m_bDrag = false;
	bool	m_bInputAvailable = true;
public:
	LPDIRECTINPUT8		  m_pDI;
	LPDIRECTINPUTDEVICE8  m_pKeyDevice; // 키보드
	LPDIRECTINPUTDEVICE8  m_pMouseDevice;// 마우스
	DIMOUSESTATE		  m_DIMouseState;
	BYTE				  m_MouseState[3];
	BYTE				  m_BeforeMouseState[3];
	BYTE				  m_KeyState[256];
	BYTE				  m_KeyStateOld[256];
public:
	BYTE	GetKey(BYTE dwKey);
public:
	bool	InitDirectInput();
	bool	ShutDownDirectInput();
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
private:
	KInput();
public:
	~KInput();
};

#define g_Input KInput::Get()