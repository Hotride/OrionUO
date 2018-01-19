/***********************************************************************************
**
** OrionWindow.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef ORIONWINDOW_H
#define ORIONWINDOW_H
//----------------------------------------------------------------------------------
class COrionWindow : public WISP_WINDOW::CWindow
{
	//SETGET(int, RenderTimerDelay, FRAME_DELAY_ACTIVE_WINDOW);

public:
	COrionWindow();
	virtual ~COrionWindow();

	//static const int FASTLOGIN_TIMER_ID = 1;
	static const int RENDER_TIMER_ID = 1;
	static const int UPDATE_TIMER_ID = 2;
	static const int FASTLOGIN_TIMER_ID = 3;

	void SetRenderTimerDelay(const int &delay);

	void EmulateOnLeftMouseButtonDown();

protected:
	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void OnResize(WISP_GEOMETRY::CSize &newSize);
	virtual void OnLeftMouseButtonDown();
	virtual void OnLeftMouseButtonUp();
	virtual bool OnLeftMouseButtonDoubleClick();
	virtual void OnRightMouseButtonDown();
	virtual void OnRightMouseButtonUp();
	virtual bool OnRightMouseButtonDoubleClick();
	virtual void OnMidMouseButtonDown();
	virtual void OnMidMouseButtonUp();
	virtual bool OnMidMouseButtonDoubleClick();
	virtual void OnMidMouseButtonScroll(const bool &up);
	virtual void OnXMouseButton(const bool &up);
	virtual void OnDragging();
	virtual void OnActivate();
	virtual void OnDeactivate();
	virtual void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);
	virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
	virtual void OnKeyUp(const WPARAM &wParam, const LPARAM &lParam);
	virtual HRESULT OnRepaint(const WPARAM &wParam, const LPARAM &lParam);
	virtual void OnShow(const bool &show);
	virtual void OnSetText(const LPARAM &lParam);
	virtual void OnTimer(uint id);
	virtual void OnThreadedTimer(uint nowTime, WISP_THREADED_TIMER::CThreadedTimer *timer);
	virtual LRESULT OnUserMessages(const UINT &message, const WPARAM &wParam, const LPARAM &lParam);
};
//----------------------------------------------------------------------------------
extern COrionWindow g_OrionWindow;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------