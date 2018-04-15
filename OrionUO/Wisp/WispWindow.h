//----------------------------------------------------------------------------------
#ifndef WISPWINDOW_H
#define WISPWINDOW_H
//----------------------------------------------------------------------------------
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
	WISP_GEOMETRY::CSize m_MaxSize = WISP_GEOMETRY::CSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

public:
    WISP_GEOMETRY::CSize GetSize() { return m_Size; };
    void SetSize(const WISP_GEOMETRY::CSize& val);

	WISP_GEOMETRY::CSize GetMinSize() { return m_MinSize; };
	void SetMinSize(const WISP_GEOMETRY::CSize& val);

	WISP_GEOMETRY::CSize GetMaxSize() { return m_MaxSize; };
	void SetMaxSize(const WISP_GEOMETRY::CSize& val);


private:
	deque<WISP_THREADED_TIMER::CThreadedTimer*> m_ThreadedTimersStack;

public:
	CWindow();
	virtual ~CWindow();

	void SetMinSize(int width, int height) { m_MinSize.Width = width; m_MinSize.Height = height; }
	void SetMaxSize(int width, int height) { m_MaxSize.Width = width; m_MaxSize.Height = height; }

	bool Create(HINSTANCE hInstance, const wchar_t *className, const wchar_t *title, bool showCursor = false, int width = 800, int height = 600, HICON icon = 0, HCURSOR cursor = 0);
	void Destroy();

	void ShowMessage(const string &text, const string &title, int buttons = MB_OK);
	void ShowMessage(const wstring &text, const wstring &title, int buttons = MB_OK);

	LRESULT OnWindowProc(HWND &hWnd, UINT &message, WPARAM &wParam, LPARAM &lParam);

	void Update() { ::UpdateWindow(Handle); }

	bool IsActive() { return (::GetForegroundWindow() == Handle); }

	void ShowCursor(bool show = true) { ::ShowCursor(show ? TRUE : FALSE); }
	void ShowWindow(bool show) { ::ShowWindow(Handle, show ? TRUE : FALSE); }

	bool Zoomed() { return (::IsZoomed(Handle) != FALSE); }

	void SetTitle(const string &text) { ::SetWindowTextA(Handle, text.c_str()); }
	void SetTitle(const wstring &text) { ::SetWindowTextW(Handle, text.c_str()); }

	void CreateTimer(uint id, int delay) { ::SetTimer(Handle, id, delay, NULL); }
	void RemoveTimer(uint id) { ::KillTimer(Handle, id); }

	void CreateThreadedTimer(uint id, int delay, bool oneShot = false, bool waitForProcessMessage = true, bool synchronizedDelay = false);
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
	virtual void OnCharPress(const WPARAM &wParam, const LPARAM &lParam) {}
	virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam) {}
	virtual void OnKeyUp(const WPARAM &wParam, const LPARAM &lParam) {}
	virtual HRESULT OnRepaint(const WPARAM &wParam, const LPARAM &lParam) { return (HRESULT)DefWindowProc(Handle, WM_NCPAINT, wParam, lParam); }
	virtual void OnShow(bool show) {}
	virtual void OnSetText(const LPARAM &lParam) {}
	virtual void OnTimer(uint id) {}
	virtual void OnThreadedTimer(uint nowTime, WISP_THREADED_TIMER::CThreadedTimer *timer) {}
	virtual LRESULT OnUserMessages(int message, const WPARAM &wParam, const LPARAM &lParam) { return S_OK; }
};
//----------------------------------------------------------------------------------
extern CWindow *g_WispWindow;
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
#endif

