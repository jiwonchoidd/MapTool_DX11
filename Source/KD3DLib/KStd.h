#pragma once
#include <winsock2.h>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <d3d11.h>
#include <dxgi.h>
#include <assert.h>
#include <wrl.h>
#include <DirectXmath.h>
#include <iostream>
#include <functional>
#include "KMath.h"
#include "KCollision.h"
#include <atlconv.h> // A2W

class KWrite;
namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

#pragma comment	(lib, "d3d11.lib")
#pragma comment	(lib, "dxgi.lib")
#pragma comment	(lib, "DirectXTK.lib")

#ifdef _DEBUG
#pragma comment	(lib, "KD3DLib.lib")
#else
#pragma comment	(lib, "KD3DLib.lib")
#endif
#pragma comment	(lib, "ws2_32.lib")
extern float g_fSecPerFrame;
extern float g_fSecTimer;
extern HWND  g_hWnd;
extern RECT  g_rtClient;
extern ID3D11Device* g_pd3dDevice;
extern HINSTANCE g_hInstance;
extern KWrite*	g_Write;
//템플릿 싱글톤
template<class T>
class Singleton
{
public:
	static T& Get()
	{
		static T theSingle;
		return theSingle;
	}
};

struct INPUT_MAP
{
	bool bUpKey;
	bool bDownKey;
	bool bLeftKey;
	bool bRightKey;

	bool bWKey;
	bool bSKey;
	bool bAKey;
	bool bDKey;
	bool bQKey;
	bool bEKey;
	bool bZKey;//확인
	bool bXKey;//취소
	bool bCKey;


	bool bLShift;
	bool bExit;
	bool bSpace;
	bool bEnter;

	//마우스 
	int  iMouseValue[3];
	int  iMousePos[2];
	BYTE bMouseState[3];
	//디버그
	bool bFullScreen;
	bool bChangePrimitive;
	bool bChangeCameraType;
	bool bChangeCullMode;
	bool bChangeFillMode;
	bool bDebugRender;
};

extern INPUT_MAP g_InputData;

static std::wstring to_mw(const std::string& _src)
{
	USES_CONVERSION;
	return std::wstring(A2W(_src.c_str()));
};

static std::string to_wm(const std::wstring& _src)
{
	USES_CONVERSION;
	return std::string(W2A(_src.c_str()));
};

#define randf(x) (x*rand()/(float)RAND_MAX)
#define randf2(x,off) (off+x*rand()/(float)RAND_MAX)
#define randstep(fMin,fMax) (fMin+((float)fMax-(float)fMin)*rand()/(float)RAND_MAX)
#define clamp(x,MinX,MaxX) if (x>MaxX) x=MaxX; else if (x<MinX) x=MinX;

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(A)				{ if(A) { (A)->Release(); (A)=NULL; } }
#endif 

#pragma region wWinMain 매크로

//기본 매크로
#define WinSTART int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow){

//스트링화연산자를 이용한 매크로
#define WinRUN_OPT(NAME, WIDTH, HEIGHT) Sample g_Sample; g_Sample.InitWindows(hInstance,nCmdShow,L#NAME, WIDTH, HEIGHT); g_Sample.Run();}
#define WinMain_OPT(NAME, WIDTH, HEIGHT) WinSTART WinRUN_OPT(NAME, WIDTH, HEIGHT)

#pragma endregion