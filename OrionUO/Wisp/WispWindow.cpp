// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//----------------------------------------------------------------------------------
#include "stdafx.h"
#include "WispWindow.h"
#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_timer.h>
namespace WISP_WINDOW
{
CWindow *g_WispWindow = nullptr;
//---------------------------------------------------------------------------
#if USE_WISP
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    WISPFUN_DEBUG("c_ww_wp");
    if (g_WispWindow != NULL)
        return g_WispWindow->OnWindowProc(hWnd, message, wParam, lParam);

    return DefWindowProc(hWnd, message, wParam, lParam);
}
#endif
//----------------------------------------------------------------------------------
CWindow::CWindow()
{
    WISPFUN_DEBUG("c14_f1");
    g_WispWindow = this;
}
//----------------------------------------------------------------------------------
CWindow::~CWindow()
{
}
//----------------------------------------------------------------------------------
void CWindow::SetSize(const WISP_GEOMETRY::CSize &size)
{
#if USE_WISP
    WISPFUN_DEBUG("c14_f2");
    RECT pos = { 0, 0, 0, 0 };
    GetWindowRect(Handle, &pos);

    RECT r = { 0, 0, 0, 0 };
    r.right = size.Width;
    r.bottom = size.Height;
    AdjustWindowRectEx(
        &r, GetWindowLongA(Handle, GWL_STYLE), FALSE, GetWindowLongA(Handle, GWL_EXSTYLE));

    if (r.left < 0)
        r.right += -r.left;

    if (r.top < 0)
        r.bottom += -r.top;

    SetWindowPos(Handle, HWND_TOP, pos.left, pos.top, r.right, r.bottom, 0);
#else
    // SDL_GetWindowPosition
    // SDL_GetWindowSize
    // SDL_SetWindowPosition
    NOT_IMPLEMENTED;
#endif
    m_Size = size;
}
//----------------------------------------------------------------------------------
void CWindow::SetMinSize(const WISP_GEOMETRY::CSize &newMinSize)
{
    WISPFUN_DEBUG("c14_f3");
#if USE_WISP
    if (m_Size.Width < newMinSize.Width || m_Size.Height < newMinSize.Height)
    {
        int width = m_Size.Width;
        int height = m_Size.Height;

        if (width < newMinSize.Width)
            width = newMinSize.Width;

        if (height < newMinSize.Height)
            height = newMinSize.Height;

        RECT pos = { 0, 0, 0, 0 };
        GetWindowRect(Handle, &pos);

        RECT r = { 0, 0, 0, 0 };
        r.right = width;
        r.bottom = height;
        AdjustWindowRectEx(
            &r, GetWindowLongA(Handle, GWL_STYLE), FALSE, GetWindowLongA(Handle, GWL_EXSTYLE));

        if (r.left < 0)
            r.right += -r.left;

        if (r.top < 0)
            r.bottom += -r.top;

        SetWindowPos(Handle, HWND_TOP, pos.left, pos.top, r.right, r.bottom, 0);
    }
#else
    // SDL_GetWindowPosition
    // SDL_GetWindowSize
    NOT_IMPLEMENTED;
#endif
    m_MinSize = newMinSize;
}
//----------------------------------------------------------------------------------
void CWindow::SetMaxSize(const WISP_GEOMETRY::CSize &newMaxSize)
{
    WISPFUN_DEBUG("c14_f4");
#if USE_WISP
    if (m_Size.Width > newMaxSize.Width || m_Size.Height > newMaxSize.Height)
    {
        int width = m_Size.Width;
        int height = m_Size.Height;

        if (width > newMaxSize.Width)
            width = newMaxSize.Width;

        if (height > newMaxSize.Height)
            height = newMaxSize.Height;

        RECT pos = { 0, 0, 0, 0 };
        GetWindowRect(Handle, &pos);

        RECT r = { 0, 0, 0, 0 };
        r.right = width;
        r.bottom = height;
        AdjustWindowRectEx(
            &r, GetWindowLongA(Handle, GWL_STYLE), FALSE, GetWindowLongA(Handle, GWL_EXSTYLE));

        if (r.left < 0)
            r.right += -r.left;

        if (r.top < 0)
            r.bottom += -r.top;

        SetWindowPos(Handle, HWND_TOP, pos.left, pos.top, r.right, r.bottom, 0);
    }
#else
    // SDL_GetWindowPosition
    // SDL_GetWindowSize
    NOT_IMPLEMENTED;
#endif
    m_MaxSize = newMaxSize;
}
//----------------------------------------------------------------------------------

bool CWindow::Create(
    const char *className, const char *title, bool showCursor, int width, int height)
{
    WISPFUN_DEBUG("c14_f5");

#if USE_WISP
    HICON icon = LoadIcon(0, MAKEINTRESOURCE(IDI_ORIONUO));
    HCURSOR cursor = LoadCursor(0, MAKEINTRESOURCE(IDC_CURSOR1));

    static wstring wclassName = ToWString(className);
    static wstring wtitle = ToWString(title);
    WNDCLASSEX wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hCursor = cursor; //LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = wclassName.c_str();
    wcex.hIcon = icon;
    wcex.hIconSm = icon;

    RegisterClassEx(&wcex);

    width += 2 * GetSystemMetrics(SM_CXSIZEFRAME);
    height += GetSystemMetrics(SM_CYCAPTION) + (GetSystemMetrics(SM_CYFRAME) * 2);

    Handle = CreateWindowEx(
        WS_EX_WINDOWEDGE,
        wclassName.c_str(),
        wtitle.c_str(),
        WS_OVERLAPPEDWINDOW,
        0,
        0,
        width,
        height,
        NULL,
        NULL,
        hInstance,
        NULL);
    if (!Handle)
        return false;

    RECT r = { 0, 0, 0, 0 };
    r.right = width;
    r.bottom = height;
    AdjustWindowRectEx(
        &r, GetWindowLongA(Handle, GWL_STYLE), FALSE, GetWindowLongA(Handle, GWL_EXSTYLE));

    if (r.left < 0)
        r.right += -r.left;

    if (r.top < 0)
        r.bottom += -r.top;

    SetWindowPos(Handle, HWND_TOP, 0, 0, r.right, r.bottom, 0);

    srand(unsigned(time(NULL)));

    GetClientRect(Handle, &r);
    m_Size.Width = r.right - r.left;
    m_Size.Height = r.bottom - r.top;

    SDL_ShowCursor(showCursor);

    ::ShowWindow(Handle, FALSE);
    UpdateWindow(Handle);
#else
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
        return false;

    m_Size.Width = width;
    m_Size.Height = height;
    m_window = SDL_CreateWindow(title, 0, 0, width, height, SDL_WINDOW_OPENGL);
    if (!m_window)
    {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Coult not create window: %s\n", SDL_GetError());
        return false;
    }

    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    if (SDL_GetWindowWMInfo(m_window, &info))
    {
        SDL_Log("SDL %d.%d.%d\n", info.version.major, info.version.minor, info.version.patch);

        const char *subsystem = "Unknown";
        switch (info.subsystem)
        {
            case SDL_SYSWM_UNKNOWN:
                break;
            case SDL_SYSWM_WINDOWS:
                subsystem = "Microsoft Windows(TM)";
                break;
            case SDL_SYSWM_X11:
                subsystem = "X Window System";
                break;
#if SDL_VERSION_ATLEAST(2, 0, 3)
            case SDL_SYSWM_WINRT:
                subsystem = "WinRT";
                break;
#endif
            case SDL_SYSWM_DIRECTFB:
                subsystem = "DirectFB";
                break;
            case SDL_SYSWM_COCOA:
                subsystem = "Apple OS X";
                break;
            case SDL_SYSWM_UIKIT:
                subsystem = "UIKit";
                break;
#if SDL_VERSION_ATLEAST(2, 0, 2)
            case SDL_SYSWM_WAYLAND:
                subsystem = "Wayland";
                break;
            case SDL_SYSWM_MIR:
                subsystem = "Mir";
                break;
#endif
#if SDL_VERSION_ATLEAST(2, 0, 4)
            case SDL_SYSWM_ANDROID:
                subsystem = "Android";
                break;
#endif
#if SDL_VERSION_ATLEAST(2, 0, 5)
            case SDL_SYSWM_VIVANTE:
                subsystem = "Vivante";
                break;
#endif
        }

        SDL_Log("System: %s\n", subsystem);
#if defined(ORION_WINDOWS)
        Handle = info.info.win.window;
#endif
    }

    SDL_ShowCursor(showCursor);
#endif // USE_WISP

    return OnCreate();
}

//----------------------------------------------------------------------------------
void CWindow::Destroy()
{
    WISPFUN_DEBUG("c14_f6");
#if USE_WISP
    PostMessage(Handle, WM_CLOSE, 0, 0);
#else
    if (m_window)
        SDL_DestroyWindow(m_window);
#endif
}
//----------------------------------------------------------------------------------
void CWindow::ShowMessage(const string &text, const string &title)
{
    WISPFUN_DEBUG("c14_f7");
#if USE_WISP
    MessageBoxA(Handle, text.c_str(), title.c_str(), MB_OK);
#else
    SDL_Log("%s: %s\n", title.c_str(), text.c_str());
#endif
}
//----------------------------------------------------------------------------------
void CWindow::ShowMessage(const wstring &text, const wstring &title)
{
    WISPFUN_DEBUG("c14_f8");
#if USE_WISP
    MessageBoxW(Handle, text.c_str(), title.c_str(), MB_OK);
#else
    SDL_Log("%s: %s\n", title.c_str(), text.c_str());
#endif
}
//----------------------------------------------------------------------------------
#if USE_WISP
LRESULT CWindow::OnWindowProc(HWND &hWnd, UINT &message, WPARAM &wParam, LPARAM &lParam)
{
    WISPFUN_DEBUG("c14_f9");
    //DebugMsg("m=0x%08X, w0x%08X l0x%08X\n", message, wParam, lParam);
    static bool parse = true;

    if (!parse)
        return DefWindowProc(hWnd, message, wParam, lParam);

    switch (message)
    {
        case WM_GETMINMAXINFO:
        case WM_SIZE:
        {
            if (IsMinimized())
                return DefWindowProc(hWnd, message, wParam, lParam);

            if (message == WM_GETMINMAXINFO)
            {
                MINMAXINFO *pInfo = (MINMAXINFO *)lParam;

                if (NoResize)
                {
                    RECT r = { 0, 0, 0, 0 };
                    r.right = m_Size.Width;
                    r.bottom = m_Size.Height;
                    AdjustWindowRectEx(
                        &r,
                        GetWindowLongA(Handle, GWL_STYLE),
                        FALSE,
                        GetWindowLongA(Handle, GWL_EXSTYLE));

                    if (r.left < 0)
                        r.right -= r.left;

                    if (r.top < 0)
                        r.bottom -= r.top;

                    POINT min = { r.right, r.bottom };
                    POINT max = { r.right, r.bottom };

                    pInfo->ptMinTrackSize = min;
                    pInfo->ptMaxTrackSize = max;
                }
                else
                {
                    RECT r = { 0, 0, 0, 0 };
                    r.right = m_Size.Width;
                    r.bottom = m_Size.Height;
                    AdjustWindowRectEx(
                        &r,
                        GetWindowLongA(Handle, GWL_STYLE),
                        FALSE,
                        GetWindowLongA(Handle, GWL_EXSTYLE));

                    if (r.left < 0)
                        r.right -= r.left;

                    if (r.top < 0)
                        r.bottom -= r.top;

                    POINT min = { m_MinSize.Width, m_MinSize.Height };
                    POINT max = { m_MaxSize.Width, m_MaxSize.Height };
                    pInfo->ptMinTrackSize = min;
                    pInfo->ptMaxTrackSize = max;
                }

                return 0;
            }

            WISP_GEOMETRY::CSize newSize(LOWORD(lParam), HIWORD(lParam));

            OnResize(newSize);
            m_Size = newSize;

            break;
        }
        case WM_CLOSE:
        case WM_NCDESTROY:
        case WM_DESTROY:
        {
            parse = false;

            OnDestroy();

            //ExitProcess(0);
            PostQuitMessage(0);

            return 0;
        }
        case WM_MOUSEMOVE:
        {
            WISP_MOUSE::g_WispMouse->LeftButtonPressed = (bool)(wParam & MK_LBUTTON);
            WISP_MOUSE::g_WispMouse->RightButtonPressed = (bool)(wParam & MK_RBUTTON);
            WISP_MOUSE::g_WispMouse->MidButtonPressed = (bool)(wParam & MK_MBUTTON);
            WISP_MOUSE::g_WispMouse->Update(); // TODO: check if is correct

            if (WISP_MOUSE::g_WispMouse->Dragging)
                OnDragging();

            break;
        }
        case WM_LBUTTONDOWN:
        {
            WISP_MOUSE::g_WispMouse->Capture();

            WISP_MOUSE::g_WispMouse->Update();
            WISP_MOUSE::g_WispMouse->LeftButtonPressed = true;
            WISP_MOUSE::g_WispMouse->LeftDropPosition = WISP_MOUSE::g_WispMouse->Position;
            WISP_MOUSE::g_WispMouse->CancelDoubleClick = false;

            uint ticks = SDL_GetTicks();

            if (WISP_MOUSE::g_WispMouse->LastLeftButtonClickTimer +
                    WISP_MOUSE::g_WispMouse->DoubleClickDelay >=
                ticks)
            {
                WISP_MOUSE::g_WispMouse->LastLeftButtonClickTimer = 0;

                if (!OnLeftMouseButtonDoubleClick())
                    OnLeftMouseButtonDown();
                else
                    WISP_MOUSE::g_WispMouse->LastLeftButtonClickTimer = 0xFFFFFFFF;

                break;
            }

            OnLeftMouseButtonDown();

            if (WISP_MOUSE::g_WispMouse->CancelDoubleClick)
                WISP_MOUSE::g_WispMouse->LastLeftButtonClickTimer = 0;
            else
                WISP_MOUSE::g_WispMouse->LastLeftButtonClickTimer = ticks;

            break;
        }
        case WM_LBUTTONUP:
        {
            WISP_MOUSE::g_WispMouse->Update();

            if (WISP_MOUSE::g_WispMouse->LastLeftButtonClickTimer != 0xFFFFFFFF)
                OnLeftMouseButtonUp();

            WISP_MOUSE::g_WispMouse->LeftButtonPressed = false;
            WISP_MOUSE::g_WispMouse->Release();

            break;
        }
        case WM_RBUTTONDOWN:
        {
            WISP_MOUSE::g_WispMouse->Capture();

            WISP_MOUSE::g_WispMouse->Update();
            WISP_MOUSE::g_WispMouse->RightButtonPressed = true;
            WISP_MOUSE::g_WispMouse->RightDropPosition = WISP_MOUSE::g_WispMouse->Position;
            WISP_MOUSE::g_WispMouse->CancelDoubleClick = false;

            uint ticks = SDL_GetTicks();

            if (WISP_MOUSE::g_WispMouse->LastRightButtonClickTimer +
                    WISP_MOUSE::g_WispMouse->DoubleClickDelay >=
                ticks)
            {
                WISP_MOUSE::g_WispMouse->LastRightButtonClickTimer = 0;

                if (!OnRightMouseButtonDoubleClick())
                    OnRightMouseButtonDown();
                else
                    WISP_MOUSE::g_WispMouse->LastRightButtonClickTimer = 0xFFFFFFFF;

                break;
            }

            OnRightMouseButtonDown();

            if (WISP_MOUSE::g_WispMouse->CancelDoubleClick)
                WISP_MOUSE::g_WispMouse->LastRightButtonClickTimer = 0;
            else
                WISP_MOUSE::g_WispMouse->LastRightButtonClickTimer = ticks;

            break;
        }
        case WM_RBUTTONUP:
        {
            WISP_MOUSE::g_WispMouse->Update();

            if (WISP_MOUSE::g_WispMouse->LastRightButtonClickTimer != 0xFFFFFFFF)
                OnRightMouseButtonUp();

            WISP_MOUSE::g_WispMouse->RightButtonPressed = false;
            WISP_MOUSE::g_WispMouse->Release();

            break;
        }
        //Нажатие на колесико мышки
        case WM_MBUTTONDOWN:
        {
            WISP_MOUSE::g_WispMouse->Capture();

            WISP_MOUSE::g_WispMouse->Update();
            WISP_MOUSE::g_WispMouse->MidButtonPressed = true;
            WISP_MOUSE::g_WispMouse->MidDropPosition = WISP_MOUSE::g_WispMouse->Position;
            WISP_MOUSE::g_WispMouse->CancelDoubleClick = false;

            uint ticks = SDL_GetTicks();

            if (WISP_MOUSE::g_WispMouse->LastMidButtonClickTimer +
                    WISP_MOUSE::g_WispMouse->DoubleClickDelay >=
                ticks)
            {
                if (!OnMidMouseButtonDoubleClick())
                    OnMidMouseButtonDown();

                WISP_MOUSE::g_WispMouse->LastMidButtonClickTimer = 0;

                break;
            }

            OnMidMouseButtonDown();

            if (WISP_MOUSE::g_WispMouse->CancelDoubleClick)
                WISP_MOUSE::g_WispMouse->LastMidButtonClickTimer = 0;
            else
                WISP_MOUSE::g_WispMouse->LastMidButtonClickTimer = ticks;

            break;
        }
        //Отпускание колесика мышки
        case WM_MBUTTONUP:
        {
            WISP_MOUSE::g_WispMouse->Update();
            OnMidMouseButtonUp();

            WISP_MOUSE::g_WispMouse->MidButtonPressed = false;
            WISP_MOUSE::g_WispMouse->Release();

            break;
        }
        //Колесико мышки вверх/вниз
        case WM_MOUSEWHEEL:
        {
            WISP_MOUSE::g_WispMouse->Update();
            OnMidMouseButtonScroll(!(short(HIWORD(wParam)) > 0));

            break;
        }
        //Доп. кнопки мыши
        case WM_XBUTTONDOWN:
        {
            WISP_MOUSE::g_WispMouse->Update();
            OnXMouseButton(!(short(HIWORD(wParam)) > 0));

            break;
        }
        case WM_CHAR:
        {
            OnCharPress(wParam, lParam);

            return 0; //break;
        }
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            OnKeyDown(wParam, lParam);

            if (wParam == VK_F4 && (GetAsyncKeyState(VK_MENU) & 0x80000000)) //Alt + F4
                break;

            return 0; //break;
        }
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            OnKeyUp(wParam, lParam);

            return 0; //break;
        }
        case WM_NCACTIVATE:
        {
            HRESULT res = (HRESULT)DefWindowProc(Handle, WM_NCACTIVATE, wParam, lParam);

            if (wParam == 0)
                OnDeactivate();
            else
                OnActivate();

            return res;
        }
        case WM_NCPAINT:
            return OnRepaint(wParam, lParam);
        case WM_SHOWWINDOW:
        {
            HRESULT res = (HRESULT)DefWindowProc(Handle, WM_SHOWWINDOW, wParam, lParam);

            OnShow(wParam != 0);

            return res;
        }
        case WM_SETTEXT:
        {
            HRESULT res = (HRESULT)DefWindowProc(Handle, WM_SETTEXT, wParam, lParam);

            OnSetText(lParam);

            return res;
        }
        case WM_TIMER:
        {
            OnTimer((uint)wParam);

            break;
        }
        case WISP_THREADED_TIMER::CThreadedTimer::MessageID:
        {
            OnThreadedTimer((uint)wParam, (WISP_THREADED_TIMER::CThreadedTimer *)lParam);

            //DebugMsg("OnThreadedTimer %i, 0x%08X\n", wParam, lParam);

            return 0;
        }
        case WM_SYSCHAR:
        {
            if (wParam == VK_F4 && (GetAsyncKeyState(VK_MENU) & 0x80000000)) //Alt + F4
                break;
            return 0;
        }
        default:
            break;
    }

    if (message >= WM_USER)
        return OnUserMessages(message, wParam, lParam);

    return DefWindowProc(hWnd, message, wParam, lParam);
}
#else
bool CWindow::OnWindowProc(SDL_Event &ev)
{
    switch (ev.type)
    {
        case SDL_WINDOWEVENT_CLOSE:
        case SDL_QUIT:
        {
            OnDestroy();
            return true;
            break;
        }

        case SDL_WINDOWEVENT_SHOWN:
        {
            OnShow(true); // Plugin
            OnActivate(); // Sound + FPS
        }
        break;

        case SDL_WINDOWEVENT_HIDDEN:
        {
            OnShow(false);  // Plugin
            OnDeactivate(); // Sound + FPS
        }
        break;

            // FIXME
            //case WM_GETMINMAXINFO:
            //case WM_SIZE:
            // WISP_THREADED_TIMER::CThreadedTimer::MessageID:
            //case WM_TIMER:

        case SDL_KEYDOWN:
        {
            OnKeyDown(ev.key);
        }
        break;

        case SDL_KEYUP:
        {
            OnKeyUp(ev.key);
        }
        break;

        // https://wiki.libsdl.org/Tutorials/TextInput
        case SDL_TEXTINPUT: // WM_CHAR
        {
            OnTextInput(ev.text);
        }
        break;

        case SDL_MOUSEMOTION:
        {
            WISP_MOUSE::g_WispMouse->Update();
            if (WISP_MOUSE::g_WispMouse->Dragging)
                OnDragging();
        }
        break;

        case SDL_MOUSEWHEEL:
        {
            WISP_MOUSE::g_WispMouse->Update();

            const bool isUp = ev.wheel.y > 0;
            OnMidMouseButtonScroll(isUp);
        }
        break;

        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEBUTTONDOWN:
        {
            WISP_MOUSE::g_WispMouse->Update();

            const bool isDown = ev.type == SDL_MOUSEBUTTONDOWN;
            auto &mouse = ev.button;
            switch (mouse.button)
            {
                case SDL_BUTTON_LEFT:
                    WISP_MOUSE::g_WispMouse->LeftButtonPressed = isDown;
                    if (isDown)
                    {
                        WISP_MOUSE::g_WispMouse->Capture();
                        WISP_MOUSE::g_WispMouse->LeftDropPosition =
                            WISP_MOUSE::g_WispMouse->Position;
                        WISP_MOUSE::g_WispMouse->CancelDoubleClick = false;
                        uint ticks = SDL_GetTicks();
                        if (WISP_MOUSE::g_WispMouse->LastLeftButtonClickTimer +
                                WISP_MOUSE::g_WispMouse->DoubleClickDelay >=
                            ticks)
                        {
                            WISP_MOUSE::g_WispMouse->LastLeftButtonClickTimer = 0;
                            if (!OnLeftMouseButtonDoubleClick())
                                OnLeftMouseButtonDown();
                            else
                                WISP_MOUSE::g_WispMouse->LastLeftButtonClickTimer = 0xFFFFFFFF;
                            break;
                        }

                        OnLeftMouseButtonDown();

                        if (WISP_MOUSE::g_WispMouse->CancelDoubleClick)
                            WISP_MOUSE::g_WispMouse->LastLeftButtonClickTimer = 0;
                        else
                            WISP_MOUSE::g_WispMouse->LastLeftButtonClickTimer = ticks;
                        break;
                    }
                    else
                    {
                        if (WISP_MOUSE::g_WispMouse->LastLeftButtonClickTimer != 0xFFFFFFFF)
                            OnLeftMouseButtonUp();
                        WISP_MOUSE::g_WispMouse->Release();
                    }
                    break;

                case SDL_BUTTON_MIDDLE:
                    WISP_MOUSE::g_WispMouse->MidButtonPressed = isDown;
                    if (isDown)
                    {
                        WISP_MOUSE::g_WispMouse->Capture();
                        WISP_MOUSE::g_WispMouse->MidDropPosition =
                            WISP_MOUSE::g_WispMouse->Position;
                        WISP_MOUSE::g_WispMouse->CancelDoubleClick = false;
                        uint ticks = SDL_GetTicks();
                        if (WISP_MOUSE::g_WispMouse->LastMidButtonClickTimer +
                                WISP_MOUSE::g_WispMouse->DoubleClickDelay >=
                            ticks)
                        {
                            if (!OnMidMouseButtonDoubleClick())
                                OnMidMouseButtonDown();

                            WISP_MOUSE::g_WispMouse->LastMidButtonClickTimer = 0;
                            break;
                        }

                        OnMidMouseButtonDown();

                        if (WISP_MOUSE::g_WispMouse->CancelDoubleClick)
                            WISP_MOUSE::g_WispMouse->LastMidButtonClickTimer = 0;
                        else
                            WISP_MOUSE::g_WispMouse->LastMidButtonClickTimer = ticks;
                    }
                    else
                    {
                        OnMidMouseButtonUp();
                        WISP_MOUSE::g_WispMouse->Release();
                    }
                    break;

                case SDL_BUTTON_RIGHT:
                    WISP_MOUSE::g_WispMouse->RightButtonPressed = isDown;
                    if (isDown)
                    {
                        WISP_MOUSE::g_WispMouse->Capture();
                        WISP_MOUSE::g_WispMouse->RightDropPosition =
                            WISP_MOUSE::g_WispMouse->Position;
                        WISP_MOUSE::g_WispMouse->CancelDoubleClick = false;
                        uint ticks = SDL_GetTicks();
                        if (WISP_MOUSE::g_WispMouse->LastRightButtonClickTimer +
                                WISP_MOUSE::g_WispMouse->DoubleClickDelay >=
                            ticks)
                        {
                            WISP_MOUSE::g_WispMouse->LastRightButtonClickTimer = 0;
                            if (!OnRightMouseButtonDoubleClick())
                                OnRightMouseButtonDown();
                            else
                                WISP_MOUSE::g_WispMouse->LastRightButtonClickTimer = 0xFFFFFFFF;
                            break;
                        }

                        OnRightMouseButtonDown();

                        if (WISP_MOUSE::g_WispMouse->CancelDoubleClick)
                            WISP_MOUSE::g_WispMouse->LastRightButtonClickTimer = 0;
                        else
                            WISP_MOUSE::g_WispMouse->LastRightButtonClickTimer = ticks;
                    }
                    else
                    {
                        if (WISP_MOUSE::g_WispMouse->LastRightButtonClickTimer != 0xFFFFFFFF)
                            OnRightMouseButtonUp();
                        WISP_MOUSE::g_WispMouse->Release();
                    }
                    break;

                case SDL_BUTTON_X1:
                    OnXMouseButton(!isDown);
                    break;

                case SDL_BUTTON_X2:
                    break;
            }
        }
        break;

            // Used by plugins only?
            // - OnShow
            // - OnRepaint
            // - OnSetText
            // - OnXMouseButton

        default:
            break;
    }

    g_Orion.Process(true);
    g_Orion.Process(false);

    return false;
}
#endif
//----------------------------------------------------------------------------------
void CWindow::CreateThreadedTimer(
    uint id, int delay, bool oneShot, bool waitForProcessMessage, bool synchronizedDelay)
{
    WISPFUN_DEBUG("c14_f10");
    for (deque<WISP_THREADED_TIMER::CThreadedTimer *>::iterator i = m_ThreadedTimersStack.begin();
         i != m_ThreadedTimersStack.end();
         ++i)
    {
        if ((*i)->TimerID == id)
            return;
    }

    WISP_THREADED_TIMER::CThreadedTimer *timer =
        new WISP_THREADED_TIMER::CThreadedTimer(id, Handle, waitForProcessMessage);
    m_ThreadedTimersStack.push_back(timer);
    timer->Run(!oneShot, delay, synchronizedDelay);
}
//----------------------------------------------------------------------------------
void CWindow::RemoveThreadedTimer(uint id)
{
    WISPFUN_DEBUG("c14_f11");
    for (deque<WISP_THREADED_TIMER::CThreadedTimer *>::iterator i = m_ThreadedTimersStack.begin();
         i != m_ThreadedTimersStack.end();
         ++i)
    {
        if ((*i)->TimerID == id)
        {
            (*i)->Stop();
            m_ThreadedTimersStack.erase(i);

            break;
        }
    }
}
//----------------------------------------------------------------------------------
WISP_THREADED_TIMER::CThreadedTimer *CWindow::GetThreadedTimer(uint id)
{
    WISPFUN_DEBUG("c14_f12");
    for (deque<WISP_THREADED_TIMER::CThreadedTimer *>::iterator i = m_ThreadedTimersStack.begin();
         i != m_ThreadedTimersStack.end();
         ++i)
    {
        if ((*i)->TimerID == id)
            return *i;
    }

    return 0;
}
//----------------------------------------------------------------------------------
}; // namespace WISP_WINDOW
//----------------------------------------------------------------------------------
