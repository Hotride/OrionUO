//----------------------------------------------------------------------------------
#ifndef WISPTHREAD_H
#define WISPTHREAD_H
#include "WispGlobal.h"
//----------------------------------------------------------------------------------
namespace WISP_THREAD
{
//----------------------------------------------------------------------------------
class CThread
{
private:
	HANDLE m_Handle;
	UINT m_ID;
	CRITICAL_SECTION m_CriticalSection;
	bool m_Paused;
	bool m_Cycled;
	int m_Delay;
	bool m_Active;

public:
	CThread();
	virtual ~CThread();

	void Run(bool cycled = false, int delay = 1, bool synchronizedDelay = false);

	bool IsActive();
	void Stop();
	bool Cycled();

	void Pause();
	void Resume();
	bool Paused();

	int Delay();
	void ChangeDelay(int newDelay);

	virtual void OnExecute(uint nowTime) {}
	virtual void OnDestroy() {}
};
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
#endif

