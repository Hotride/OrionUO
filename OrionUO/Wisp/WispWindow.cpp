// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//----------------------------------------------------------------------------------
#include "stdafx.h"

namespace WISP_WINDOW
{
CWindow *g_WispWindow = NULL;
//---------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WISPFUN_DEBUG("c_ww_wp");
	if (g_WispWindow != NULL)
		return g_WispWindow->OnWindowProc(hWnd, message, wParam, lParam);

	return DefWindowProc(hWnd, message, wParam, lParam);
}
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
	WISPFUN_DEBUG("c14_f2");
	RECT pos = { 0, 0, 0, 0 };
	GetWindowRect(m_Handle, &pos);

	RECT r = { 0, 0, 0, 0 };
	r.right = size.Width;
	r.bottom = size.Height;
	AdjustWindowRectEx(&r, GetWindowLongA(m_Handle, GWL_STYLE), FALSE, GetWindowLongA(m_Handle, GWL_EXSTYLE));

	if (r.left < 0)
		r.right += -r.left;

	if (r.top < 0)
		r.bottom += -r.top;

	SetWindowPos(m_Handle, HWND_TOP, pos.left, pos.top, r.right, r.bottom, 0);

	m_Size = size;
}
//----------------------------------------------------------------------------------
void CWindow::SetMinSize(const WISP_GEOMETRY::CSize &newMinSize)
{
	WISPFUN_DEBUG("c14_f3");
	if (m_Size.Width < newMinSize.Width || m_Size.Height < newMinSize.Height)
	{
		int width = m_Size.Width;
		int height = m_Size.Height;

		if (width < newMinSize.Width)
			width = newMinSize.Width;

		if (height < newMinSize.Height)
			height = newMinSize.Height;

		RECT pos = { 0, 0, 0, 0 };
		GetWindowRect(m_Handle, &pos);

		RECT r = { 0, 0, 0, 0 };
		r.right = width;
		r.bottom = height;
		AdjustWindowRectEx(&r, GetWindowLongA(m_Handle, GWL_STYLE), FALSE, GetWindowLongA(m_Handle, GWL_EXSTYLE));

		if (r.left < 0)
			r.right += -r.left;

		if (r.top < 0)
			r.bottom += -r.top;

		SetWindowPos(m_Handle, HWND_TOP, pos.left, pos.top, r.right, r.bottom, 0);
	}

	m_MinSize = newMinSize;
}
//----------------------------------------------------------------------------------
void CWindow::SetMaxSize(const WISP_GEOMETRY::CSize &newMaxSize)
{
	WISPFUN_DEBUG("c14_f4");
	if (m_Size.Width > newMaxSize.Width || m_Size.Height > newMaxSize.Height)
	{
		int width = m_Size.Width;
		int height = m_Size.Height;

		if (width > newMaxSize.Width)
			width = newMaxSize.Width;

		if (height > newMaxSize.Height)
			height = newMaxSize.Height;

		RECT pos = { 0, 0, 0, 0 };
		GetWindowRect(m_Handle, &pos);

		RECT r = { 0, 0, 0, 0 };
		r.right = width;
		r.bottom = height;
		AdjustWindowRectEx(&r, GetWindowLongA(m_Handle, GWL_STYLE), FALSE, GetWindowLongA(m_Handle, GWL_EXSTYLE));

		if (r.left < 0)
			r.right += -r.left;

		if (r.top < 0)
			r.bottom += -r.top;

		SetWindowPos(m_Handle, HWND_TOP, pos.left, pos.top, r.right, r.bottom, 0);
	}

	m_MaxSize = newMaxSize;
}
//----------------------------------------------------------------------------------
bool CWindow::Create(HINSTANCE hInstance, const wchar_t *className, const wchar_t *title, bool showCursor, int width, int height, HICON icon, HCURSOR cursor)
{
	WISPFUN_DEBUG("c14_f5");
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
	wcex.lpszClassName = className;
	wcex.hIcon = icon;
	wcex.hIconSm = icon;

	RegisterClassEx(&wcex);

	width += 2 * GetSystemMetrics(SM_CXSIZEFRAME);
	height += GetSystemMetrics(SM_CYCAPTION) + (GetSystemMetrics(SM_CYFRAME) * 2);

	m_Handle = CreateWindowEx(WS_EX_WINDOWEDGE, className, title, WS_OVERLAPPEDWINDOW, 0, 0, width, height, NULL, NULL, hInstance, NULL);

	if (!m_Handle)
		return false;

	RECT r = { 0, 0, 0, 0 };
	r.right = width;
	r.bottom = height;
	AdjustWindowRectEx(&r, GetWindowLongA(m_Handle, GWL_STYLE), FALSE, GetWindowLongA(m_Handle, GWL_EXSTYLE));

	if (r.left < 0)
		r.right += -r.left;

	if (r.top < 0)
		r.bottom += -r.top;

	SetWindowPos(m_Handle, HWND_TOP, 0, 0, r.right, r.bottom, 0);

	srand(unsigned(time(NULL)));

	GetClientRect(m_Handle, &r);
	m_Size.Width = r.right - r.left;
	m_Size.Height = r.bottom - r.top;

	ShowCursor(showCursor);

	::ShowWindow(m_Handle, FALSE);
	UpdateWindow(m_Handle);

	return OnCreate();
}
//----------------------------------------------------------------------------------
void CWindow::Destroy()
{
	WISPFUN_DEBUG("c14_f6");
	PostMessage(m_Handle, WM_CLOSE, 0, 0);
}
//----------------------------------------------------------------------------------
void CWindow::ShowMessage(const string &text, const string &title, const uint &buttons)
{
	WISPFUN_DEBUG("c14_f7");
	MessageBoxA(m_Handle, text.c_str(), title.c_str(), buttons);
}
//----------------------------------------------------------------------------------
void CWindow::ShowMessage(const wstring &text, const wstring &title, const uint &buttons)
{
	WISPFUN_DEBUG("c14_f8");
	MessageBoxW(m_Handle, text.c_str(), title.c_str(), buttons);
}
//----------------------------------------------------------------------------------
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
			if (IsIconic(m_Handle))
				return DefWindowProc(hWnd, message, wParam, lParam);

			if (message == WM_GETMINMAXINFO)
			{
				MINMAXINFO *pInfo = (MINMAXINFO*)lParam;

				if (m_NoResize)
				{
					RECT r = { 0, 0, 0, 0 };
					r.right = m_Size.Width;
					r.bottom = m_Size.Height;
					AdjustWindowRectEx(&r, GetWindowLongA(m_Handle, GWL_STYLE), FALSE, GetWindowLongA(m_Handle, GWL_EXSTYLE));

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
					AdjustWindowRectEx(&r, GetWindowLongA(m_Handle, GWL_STYLE), FALSE, GetWindowLongA(m_Handle, GWL_EXSTYLE));

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
			WISP_MOUSE::g_WispMouse->Update(wParam, lParam);

			if (WISP_MOUSE::g_WispMouse->Dragging)
				OnDragging();

			break;
		}
		case WM_LBUTTONDOWN:
		{
			SetCapture(m_Handle);

			WISP_MOUSE::g_WispMouse->Update();
			WISP_MOUSE::g_WispMouse->LeftButtonPressed = true;
			WISP_MOUSE::g_WispMouse->LeftDropPosition = WISP_MOUSE::g_WispMouse->Position;
			WISP_MOUSE::g_WispMouse->CancelDoubleClick = false;

			uint ticks = GetTickCount();

			if (WISP_MOUSE::g_WispMouse->LastLeftButtonClickTimer + WISP_MOUSE::g_WispMouse->DoubleClickDelay >= ticks)
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
			WISP_MOUSE::g_WispMouse->ReleaseCapture();

			break;
		}
		case WM_RBUTTONDOWN:
		{
			SetCapture(m_Handle);

			WISP_MOUSE::g_WispMouse->Update();
			WISP_MOUSE::g_WispMouse->RightButtonPressed = true;
			WISP_MOUSE::g_WispMouse->RightDropPosition = WISP_MOUSE::g_WispMouse->Position;
			WISP_MOUSE::g_WispMouse->CancelDoubleClick = false;

			uint ticks = GetTickCount();

			if (WISP_MOUSE::g_WispMouse->LastRightButtonClickTimer + WISP_MOUSE::g_WispMouse->DoubleClickDelay >= ticks)
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
			WISP_MOUSE::g_WispMouse->ReleaseCapture();

			break;
		}
		//Нажатие на колесико мышки
		case WM_MBUTTONDOWN:
		{
			SetCapture(m_Handle);

			WISP_MOUSE::g_WispMouse->Update();
			WISP_MOUSE::g_WispMouse->MidButtonPressed = true;
			WISP_MOUSE::g_WispMouse->MidDropPosition = WISP_MOUSE::g_WispMouse->Position;
			WISP_MOUSE::g_WispMouse->CancelDoubleClick = false;

			uint ticks = GetTickCount();

			if (WISP_MOUSE::g_WispMouse->LastMidButtonClickTimer + WISP_MOUSE::g_WispMouse->DoubleClickDelay >= ticks)
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
			WISP_MOUSE::g_WispMouse->ReleaseCapture();

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
			HRESULT res = (HRESULT)DefWindowProc(m_Handle, WM_NCACTIVATE, wParam, lParam);

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
			HRESULT res = (HRESULT)DefWindowProc(m_Handle, WM_SHOWWINDOW, wParam, lParam);

			OnShow(wParam != 0);

			return res;
		}
		case WM_SETTEXT:
		{
			HRESULT res = (HRESULT)DefWindowProc(m_Handle, WM_SETTEXT, wParam, lParam);

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
			OnThreadedTimer((uint)wParam, (WISP_THREADED_TIMER::CThreadedTimer*)lParam);

			//DebugMsg("OnThreadedTimer %i, 0x%08X\n", wParam, lParam);

			return 0;
		}
		default:
			break;
	}

	if (message >= WM_USER)
		return OnUserMessages(message, wParam, lParam);

	return DefWindowProc(hWnd, message, wParam, lParam);
}
//----------------------------------------------------------------------------------
void CWindow::CreateThreadedTimer(uint id, const int &delay, const bool &oneShot, const bool &waitForProcessMessage, const bool &synchronizedDelay)
{
	WISPFUN_DEBUG("c14_f10");
	for (deque<WISP_THREADED_TIMER::CThreadedTimer*>::iterator i = m_ThreadedTimersStack.begin(); i != m_ThreadedTimersStack.end(); ++i)
	{
		if ((*i)->TimerID == id)
			return;
	}

	WISP_THREADED_TIMER::CThreadedTimer *timer = new WISP_THREADED_TIMER::CThreadedTimer(id, m_Handle, waitForProcessMessage);
	m_ThreadedTimersStack.push_back(timer);
	timer->Run(!oneShot, delay, synchronizedDelay);
}
//----------------------------------------------------------------------------------
void CWindow::RemoveThreadedTimer(uint id)
{
	WISPFUN_DEBUG("c14_f11");
	for (deque<WISP_THREADED_TIMER::CThreadedTimer*>::iterator i = m_ThreadedTimersStack.begin(); i != m_ThreadedTimersStack.end(); ++i)
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
	for (deque<WISP_THREADED_TIMER::CThreadedTimer*>::iterator i = m_ThreadedTimersStack.begin(); i != m_ThreadedTimersStack.end(); ++i)
	{
		if ((*i)->TimerID == id)
			return *i;
	}

	return 0;
}
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
