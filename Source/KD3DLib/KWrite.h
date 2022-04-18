#pragma once
#include "KStd.h"
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
// 원하는 위치, 색, 크기, 폰트, 스트링 출력
class KWrite 
{
private:
	float				m_SeqTimer;
	std::wstring		m_SeqText;
public:
	ID2D1Factory*		m_pd2dFactory;
	IDWriteFactory*		m_pdWriteFactory;
	ID2D1RenderTarget*	m_pRT; // 3d 연동
	IDWriteTextFormat*	m_pTextFormat;
	IDWriteTextFormat*  m_pTextGame40;
	IDWriteTextFormat*	m_pTextGame15;
	ID2D1SolidColorBrush* m_pTextBrush;
public:
	HRESULT CreateDeviceResources(IDXGISurface1* pSurface);
	bool	RenderText(RECT rt, const TCHAR* text,
		D2D1::ColorF color, IDWriteTextFormat* pTextFormat=nullptr);
	bool	RenderText_Sequence(RECT rt, std::wstring text,
		D2D1::ColorF color, IDWriteTextFormat* pTextFormat = nullptr);
	void	DeleteDeviceResize();
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
};

