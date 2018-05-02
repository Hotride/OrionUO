//----------------------------------------------------------------------------------
#pragma once
#include <SDL_video.h>
#include <SDL_events.h>
namespace WISP_WINDOW
{
//----------------------------------------------------------------------------------
class CWindow
{
public:
    HWND Handle = 0;
    bool NoResize = false;

protected:
    WISP_GEOMETRY::CSize m_Size = WISP_GEOMETRY::CSize();
    WISP_GEOMETRY::CSize m_MinSize = WISP_GEOMETRY::CSize(100, 100);
    WISP_GEOMETRY::CSize m_MaxSize =
        WISP_GEOMETRY::CSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

public:
    WISP_GEOMETRY::CSize GetSize() { return m_Size; };
    void SetSize(const WISP_GEOMETRY::CSize &val);

    WISP_GEOMETRY::CSize GetMinSize() { return m_MinSize; };
    void SetMinSize(const WISP_GEOMETRY::CSize &val);

    WISP_GEOMETRY::CSize GetMaxSize() { return m_MaxSize; };
    void SetMaxSize(const WISP_GEOMETRY::CSize &val);

    // GetSystemMetrics -> SDL_GetCurrentDisplayMode
    SDL_Window *m_window = nullptr;

private:
    deque<WISP_THREADED_TIMER::CThreadedTimer *> m_ThreadedTimersStack;

public:
    CWindow();
    virtual ~CWindow();

    void SetMinSize(int width, int height)
    {
        m_MinSize.Width = width;
        m_MinSize.Height = height;
    }
    void SetMaxSize(int width, int height)
    {
        m_MaxSize.Width = width;
        m_MaxSize.Height = height;
    }

    bool Create(
        const char *className,
        const char *title,
        bool showCursor = false,
        int width = 800,
        int height = 600);
    void Destroy();

    void ShowMessage(const string &text, const string &title);
    void ShowMessage(const wstring &text, const wstring &title);

#if USE_WISP
    HINSTANCE hInstance = 0;
    LRESULT OnWindowProc(HWND &hWnd, UINT &message, WPARAM &wParam, LPARAM &lParam);
#else
    bool OnWindowProc(SDL_Event &ev);
#endif

    void Update()
    {
#if USE_WISP
        ::UpdateWindow(Handle);
#endif
    }

    void ShowCursor(bool show = true) { SDL_ShowCursor(show ? SDL_TRUE : SDL_FALSE); }

#if USE_WISP
    bool IsActive() const { return (::GetForegroundWindow() == Handle); }
    void SetTitle(const string &text) const { ::SetWindowTextA(Handle, text.c_str()); }
    void ShowWindow(bool show) const { ::ShowWindow(Handle, show ? TRUE : FALSE); }
    bool IsMinimized() const { return ::IsIconic(Handle); }
    bool IsMaximized() const { return (::IsZoomed(Handle) != FALSE); }
#else
    bool IsActive() const { return SDL_GetGrabbedWindow() == m_window; } // TODO: check
    void SetTitle(const string &text) const { SDL_SetWindowTitle(m_window, text.c_str()); }
    void ShowWindow(bool show) const { show ? SDL_ShowWindow(m_window) : SDL_HideWindow(m_window); }
    bool IsMinimized() const { return (SDL_GetWindowFlags(m_window) & SDL_WINDOW_MINIMIZED) != 0; }
    bool IsMaximized() const { return (SDL_GetWindowFlags(m_window) & SDL_WINDOW_MAXIMIZED) != 0; }
#endif

    // May be done using: SDL_AddTimer / SDL_RemoveTimer
    void CreateTimer(uint id, int delay) { ::SetTimer(Handle, id, delay, NULL); }
    void RemoveTimer(uint id) { ::KillTimer(Handle, id); }

    void CreateThreadedTimer(
        uint id,
        int delay,
        bool oneShot = false,
        bool waitForProcessMessage = true,
        bool synchronizedDelay = false);
    void RemoveThreadedTimer(uint id);
    WISP_THREADED_TIMER::CThreadedTimer *GetThreadedTimer(uint id);

protected:
    virtual bool OnCreate() { return true; }
    virtual void OnDestroy() {}
    virtual void OnResize(WISP_GEOMETRY::CSize &newSize) {}
    virtual void OnLeftMouseButtonDown() {}
    virtual void OnLeftMouseButtonUp() {}
    virtual bool OnLeftMouseButtonDoubleClick() { return false; }
    virtual void OnRightMouseButtonDown() {}
    virtual void OnRightMouseButtonUp() {}
    virtual bool OnRightMouseButtonDoubleClick() { return false; }
    virtual void OnMidMouseButtonDown() {}
    virtual void OnMidMouseButtonUp() {}
    virtual bool OnMidMouseButtonDoubleClick() { return false; }
    virtual void OnMidMouseButtonScroll(bool up) {}
    virtual void OnXMouseButton(bool up) {}
    virtual void OnDragging() {}
    virtual void OnActivate() {}
    virtual void OnDeactivate() {}
    virtual void OnShow(bool show) {}

    virtual void OnTimer(uint id) {}
    virtual void OnThreadedTimer(uint nowTime, WISP_THREADED_TIMER::CThreadedTimer *timer) {}
    virtual void OnSetText(const LPARAM &lParam) {}
    virtual HRESULT OnRepaint(const WPARAM &wParam, const LPARAM &lParam)
    {
        return (HRESULT)DefWindowProc(Handle, WM_NCPAINT, wParam, lParam);
    }
    virtual LRESULT OnUserMessages(int message, const WPARAM &wParam, const LPARAM &lParam)
    {
        return S_OK;
    }
#if USE_WISP
    virtual void OnCharPress(const WPARAM &wParam, const LPARAM &lParam) {}
    virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam) {}
    virtual void OnKeyUp(const WPARAM &wParam, const LPARAM &lParam) {}
#else
    virtual void OnTextInput(const SDL_TextInputEvent &ev) {}
    virtual void OnKeyDown(const SDL_KeyboardEvent &ev) {}
    virtual void OnKeyUp(const SDL_KeyboardEvent &ev) {}
#endif
};
//----------------------------------------------------------------------------------
extern CWindow *g_WispWindow;
//----------------------------------------------------------------------------------
};  // namespace WISP_WINDOW
    //----------------------------------------------------------------------------------