#include "KInput.h"

INPUT_MAP g_InputData;

bool KInput::Init()
{    
    InitDirectInput();
    return true;
}
bool KInput::InitDirectInput()
{
    HRESULT hr = S_OK;
    if (FAILED(hr = DirectInput8Create(g_hInstance,
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        (void**)&m_pDI, NULL)))
    {
        return false;
    }
    if (FAILED(hr = m_pDI->CreateDevice(GUID_SysKeyboard,
        &m_pKeyDevice, NULL)))
    {
        return false;
    }
    // 장치별 반환 데이터 설정
    m_pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
    // 장치별 독점 및 비독점 설정(협조레벨)
    if (FAILED(hr = m_pKeyDevice->SetCooperativeLevel(
        g_hWnd,
        DISCL_NONEXCLUSIVE |
        DISCL_FOREGROUND |
        DISCL_NOWINKEY)))
    {
        return false;
    }

    while (m_pKeyDevice->Acquire() == DIERR_INPUTLOST);


    if (FAILED(hr = m_pDI->CreateDevice(GUID_SysMouse,
        &m_pMouseDevice, NULL)))
    {
        return false;
    }
    m_pMouseDevice->SetDataFormat(&c_dfDIMouse);

    if (FAILED(hr = m_pMouseDevice->SetCooperativeLevel(
        g_hWnd,
        DISCL_NONEXCLUSIVE |
        DISCL_FOREGROUND)))
    {
        return true;
    }
    while (m_pMouseDevice->Acquire() == DIERR_INPUTLOST);
    return true;
}

bool KInput::ShutDownDirectInput()
{
    if (m_pMouseDevice)
    {
        m_pMouseDevice->Unacquire();
        m_pMouseDevice->Release();
        m_pMouseDevice = NULL;
    }
    if (m_pKeyDevice)
    {
        m_pKeyDevice->Unacquire();
        m_pKeyDevice->Release();
        m_pKeyDevice = NULL;
    }
    if (m_pDI)
    {
        m_pDI->Release();
        m_pDI = NULL;
    }
    return true;
}


bool KInput::Frame()
{
    // 화면좌표
    GetCursorPos(&m_ptPos);
    // 클라이언트 좌표
    ScreenToClient(g_hWnd, &m_ptPos);

    #pragma region 다이렉트 인풋 상태 읽어오기
    HRESULT hr;
    if (m_pMouseDevice == NULL || m_pKeyDevice == NULL) return false;

    if (FAILED(hr = m_pKeyDevice->GetDeviceState(256, m_KeyState)))
    {
        while (m_pKeyDevice->Acquire() == DIERR_INPUTLOST);
    }
    if (FAILED(hr = m_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_DIMouseState)))
    {
        while (m_pMouseDevice->Acquire() == DIERR_INPUTLOST);
    }
    #pragma endregion
    if (m_bInputAvailable)
    {
        #pragma region  마우스 상태
        for (int iButton = 0; iButton < 3; iButton++)
        {
            m_BeforeMouseState[iButton] = m_DIMouseState.rgbButtons[iButton];
        }
        for (int iButton = 0; iButton < 3; iButton++)
        {
            if (m_BeforeMouseState[iButton] & 0x80)
            {
                if (m_MouseState[iButton] == KEY_FREE)
                    m_MouseState[iButton] = KEY_PUSH;
                else
                    m_MouseState[iButton] = KEY_HOLD;
            }
            else
            {
                if (m_MouseState[iButton] == KEY_PUSH ||
                    m_MouseState[iButton] == KEY_HOLD)
                    m_MouseState[iButton] = KEY_UP;
                else
                    m_MouseState[iButton] = KEY_FREE;
            }
        }

        ZeroMemory(&g_InputData, sizeof(INPUT_MAP));

        g_InputData.bMouseState[0] = m_MouseState[0];
        g_InputData.bMouseState[1] = m_MouseState[1];
        g_InputData.bMouseState[2] = m_MouseState[2];

        g_InputData.iMouseValue[0] = m_DIMouseState.lX;
        g_InputData.iMouseValue[1] = m_DIMouseState.lY;
        g_InputData.iMouseValue[2] = m_DIMouseState.lZ;
        g_InputData.iMousePos[0] = m_ptPos.x;
        g_InputData.iMousePos[1] = m_ptPos.y;
#pragma endregion

        g_InputData.bWKey = GetKey(DIK_W);
        g_InputData.bAKey = GetKey(DIK_A);
        g_InputData.bSKey = GetKey(DIK_S);
        g_InputData.bDKey = GetKey(DIK_D);
        g_InputData.bZKey = GetKey(DIK_Z);
        g_InputData.bXKey = GetKey(DIK_X);

        g_InputData.bLShift = GetKey(DIK_LSHIFT);

        g_InputData.bLeftKey = GetKey(DIK_LEFT);
        g_InputData.bRightKey = GetKey(DIK_RIGHT);
        g_InputData.bUpKey = GetKey(DIK_UP);
        g_InputData.bDownKey = GetKey(DIK_DOWN);
       
#ifdef DEBUG
        if (g_Input.GetKey(DIK_F1) == KEY_PUSH)
        {
            g_InputData.bDebugRender = !g_InputData.bDebugRender;
        }
        if (g_Input.GetKey(DIK_F2) == KEY_PUSH)
        {
            g_InputData.bChangeFillMode = !g_InputData.bChangeFillMode;
        }

#endif // DEBUG

        
        if (GetKey(DIK_RETURN) == KEY_UP)
            g_InputData.bEnter= true;
        else
            g_InputData.bEnter = false;;
    }
  

    return true;
}

BYTE KInput::GetKey(BYTE dwKey)
{
    BYTE sKey;
    sKey = m_KeyState[dwKey];
    if (sKey & 0x80)
    {
        if (m_KeyStateOld[dwKey] == KEY_FREE)
            m_KeyStateOld[dwKey] = KEY_PUSH;
        else
            m_KeyStateOld[dwKey] = KEY_HOLD;
    }
    else
    {
        if (m_KeyStateOld[dwKey] == KEY_PUSH ||
            m_KeyStateOld[dwKey] == KEY_HOLD)
            m_KeyStateOld[dwKey] = KEY_UP;
        else
            m_KeyStateOld[dwKey] = KEY_FREE;
    }
    return m_KeyStateOld[dwKey];
}
bool KInput::Render()
{
    return true;
}
bool KInput::Release()
{
    ShutDownDirectInput();
    return true;
}
KInput::KInput() 
{

}
KInput::~KInput() 
{
    //ShutDownDirectInput();
}