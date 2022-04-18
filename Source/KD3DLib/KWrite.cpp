#include "KWrite.h"
HRESULT KWrite::CreateDeviceResources(IDXGISurface1* pSurface)
{
    HRESULT hr = S_OK;

    UINT dpi = GetDpiForWindow(g_hWnd);
    D2D1_RENDER_TARGET_PROPERTIES props;
    props.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
    props.pixelFormat.format= DXGI_FORMAT_UNKNOWN;
    props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
    props.dpiX = dpi;
    props.dpiY = dpi;
    props.usage= D2D1_RENDER_TARGET_USAGE_NONE;
    props.minLevel= D2D1_FEATURE_LEVEL_DEFAULT;

    hr = m_pd2dFactory->CreateDxgiSurfaceRenderTarget(
        pSurface, 
        &props, 
        &m_pRT);
    if (FAILED(hr)) return hr;

    m_pRT->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::Yellow), 
        &m_pTextBrush);
    if (FAILED(hr)) return hr;
    return S_OK;
}
bool KWrite::Init()
{
    HRESULT hr;
    hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pd2dFactory);
    if (FAILED(hr)) return false;
    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory), 
        reinterpret_cast<IUnknown**>(&m_pdWriteFactory));
    if (FAILED(hr)) return false;

    AddFontResourceEx(L"../../data/font/PFStardust.ttf", FR_PRIVATE, 0);

     hr = m_pdWriteFactory->CreateTextFormat(
            L"궁서",
            NULL,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            20,
            L"ko-kr",//L"en-us",//L"ko-kr",
            &m_pTextFormat
        );
     if (FAILED(hr)) return false;
     hr = m_pdWriteFactory->CreateTextFormat(
         L"PFStardust",
         NULL,
         DWRITE_FONT_WEIGHT_BOLD,
         DWRITE_FONT_STYLE_NORMAL,
         DWRITE_FONT_STRETCH_EXTRA_CONDENSED,
         40,
         L"ko-kr",//L"en-us",//L"ko-kr",
         &m_pTextGame40
     );
    if (FAILED(hr)) return false;
    hr = m_pdWriteFactory->CreateTextFormat(
        L"PFStardust",
        NULL,
        DWRITE_FONT_WEIGHT_BOLD,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_EXTRA_CONDENSED,
        20,
        L"ko-kr",//L"en-us",//L"ko-kr",
        &m_pTextGame15
    );
    if (FAILED(hr)) return false;
     m_pTextGame40->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_UNIFORM, 50.0f, 40.0f);
    return true;
}

bool KWrite::Frame()
{
    return true;
}

bool KWrite::Render()
{
    return true;
}
bool KWrite::RenderText(RECT rt, 
    const TCHAR* data, D2D1::ColorF color,
    IDWriteTextFormat* pTextFormat)
{
    if (m_pRT)
    {
        m_pRT->BeginDraw();
        m_pRT->SetTransform(D2D1::IdentityMatrix());
        D2D1_RECT_F rect = D2D1::RectF(rt.left, rt.top, rt.right, rt.bottom);
        m_pTextBrush->SetColor(color);
        if (pTextFormat == nullptr)
        {
            m_pRT->DrawText(data, wcslen(data),
                m_pTextGame40, rect, m_pTextBrush);
        }
        else
        {
            m_pRT->DrawText(data, wcslen(data),
                pTextFormat, rect, m_pTextBrush);
        }
        m_pRT->EndDraw();
    }
    return true;
}
bool KWrite::RenderText_Sequence(RECT rt, std::wstring text, D2D1::ColorF color, IDWriteTextFormat* pTextFormat)
{
    if (m_SeqText == text)
    {
        m_SeqTimer = 0.0f;
        m_SeqText = text;
    }
    else
    {
        m_SeqTimer += g_fSecPerFrame*15;
        m_SeqTimer = min(m_SeqTimer, text.length());
        m_SeqText =text.substr(0,m_SeqTimer);
    }
    RenderText(rt, m_SeqText.c_str(), color);
    return true;
}
void KWrite::DeleteDeviceResize()
{
    if (m_pTextBrush)m_pTextBrush->Release();
    m_pTextBrush = nullptr;
    if (m_pRT)m_pRT->Release();
    m_pRT = nullptr;
}
bool KWrite::Release()
{
    if (m_pd2dFactory) m_pd2dFactory->Release();
    if (m_pdWriteFactory) m_pdWriteFactory->Release();
    if (m_pRT) m_pRT->Release();
    if (m_pTextFormat) m_pTextFormat->Release();
    if (m_pTextBrush) m_pTextBrush->Release();
    if (m_pTextGame40)m_pTextGame40->Release();
    if (m_pTextGame15)m_pTextGame15->Release();
    //ex버젼은 자동으로 해제해준다
    RemoveFontResourceEx(L"../../data/font/PFStardust.ttf", FR_PRIVATE, 0);
    return true;
}
