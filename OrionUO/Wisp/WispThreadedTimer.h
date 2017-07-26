//----------------------------------------------------------------------------------
#ifndef WISPTHREADEDTIMER_H
#define WISPTHREADEDTIMER_H
#include "WispThread.h"
//----------------------------------------------------------------------------------
namespace WISP_THREADED_TIMER
{
//----------------------------------------------------------------------------------
class CThreadedTimer : public WISP_THREAD::CThread
{
	SETGET(uint, TimerID, 0);
	SETGET(HWND, WindowHandle, 0);
	SETGET(bool, WairForProcessMessage, false);

public:
	CThreadedTimer(uint id, HWND windowHandle, bool wairForProcessMessage = false);
	virtual ~CThreadedTimer();

	static const uint MessageID = WM_USER + 400;

	virtual void OnExecute(uint nowTime);
	virtual void OnDestroy();
};
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
#endif

