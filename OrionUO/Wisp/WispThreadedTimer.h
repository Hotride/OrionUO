//----------------------------------------------------------------------------------
#ifndef WISPTHREADEDTIMER_H
#define WISPTHREADEDTIMER_H
//----------------------------------------------------------------------------------
namespace WISP_THREADED_TIMER
{
//----------------------------------------------------------------------------------
class CThreadedTimer : public WISP_THREAD::CThread
{
public:
	uint TimerID = 0;
	HWND WindowHandle = 0;
	bool WairForProcessMessage = false;

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

