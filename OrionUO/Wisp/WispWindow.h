//----------------------------------------------------------------------------------
#ifndef WISPWINDOW_H
#define WISPWINDOW_H
//----------------------------------------------------------------------------------
namespace WISP_WINDOW
{
//----------------------------------------------------------------------------------
class CWindow
{
	SETGET(HWND, Handle, 0);
	SETGETE(WISP_GEOMETRY::CSize, Size, WISP_GEOMETRY::CSize());
	SETGET(bool, NoResize, false);
	SETGETE(WISP_GEOMETRY::CSize, MinSize, WISP_GEOMETRY::CSize(100, 100));
	SETGETE(WISP_GEOMETRY::CSize, MaxSize, WISP_GEOMETRY::CSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)));

private:
	deque<WISP_THREADED_TIMER::CThreadedTimer*> m_ThreadedTimersStack;

public:
	CWindow();
	virtual ~CWindow();

	void SetMinSize(const int &width, const int &height) { m_MinSize.Width = width; m_MinSize.Height = height; }
	void SetMaxSize(const int &width, const int &height) { m_MaxSize.Width = width; m_MaxSize.Height = height; }

	bool Create(HINSTANCE hInstance, const wchar_t *className, const wchar_t *title, bool showCursor = false, int width = 800, int height = 600, HICON icon = 0, HCURSOR cursor = 0);
	void Destroy();

	void ShowMessage(const string &text, const string &title, const uint &buttons = MB_OK);
	void ShowMessage(const wstring &text, const wstring &title, const uint &buttons = MB_OK);

	LRESULT OnWindowProc(HWND &hWnd, UINT &message, WPARAM &wParam, LPARAM &lParam);

	void Update() { ::UpdateWindow(m_Handle); }

	bool IsActive() { return (::GetForegroundWindow() == m_Handle); }

	void ShowCursor(const bool &show = true) { ::ShowCursor(show ? TRUE : FALSE); }
	void ShowWindow(bool show) { ::ShowWindow(m_Handle, show ? TRUE : FALSE); }

	bool Zoomed() { return (::IsZoomed(m_Handle) != FALSE); }

	void SetTitle(const string &text) { ::SetWindowTextA(m_Handle, text.c_str()); }
	void SetTitle(const wstring &text) { ::SetWindowTextW(m_Handle, text.c_str()); }

	void CreateTimer(uint id, int delay) { ::SetTimer(m_Handle, id, delay, NULL); }
	void RemoveTimer(uint id) { ::KillTimer(m_Handle, id); }

	void CreateThreadedTimer(uint id, const int &delay, const bool &oneShot = false, const bool &waitForProcessMessage = true, const bool &synchronizedDelay = false);
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
	virtual void OnMidMouseButtonScroll(const bool &up) {}
	virtual void OnXMouseButton(const bool &up) {}
	virtual void OnDragging() {}
	virtual void OnActivate() {}
	virtual void OnDeactivate() {}
	virtual void OnCharPress(const WPARAM &wParam, const LPARAM &lParam) {}
	virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam) {}
	virtual void OnKeyUp(const WPARAM &wParam, const LPARAM &lParam) {}
	virtual HRESULT OnRepaint(const WPARAM &wParam, const LPARAM &lParam) { return (HRESULT)DefWindowProc(m_Handle, WM_NCPAINT, wParam, lParam); }
	virtual void OnShow(const bool &show) {}
	virtual void OnSetText(const LPARAM &lParam) {}
	virtual void OnTimer(uint id) {}
	virtual void OnThreadedTimer(uint nowTime, WISP_THREADED_TIMER::CThreadedTimer *timer) {}
	virtual LRESULT OnUserMessages(const UINT &message, const WPARAM &wParam, const LPARAM &lParam) { return S_OK; }
};
//----------------------------------------------------------------------------------
extern CWindow *g_WispWindow;
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
#endif

