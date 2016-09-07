//----------------------------------------------------------------------------------
#include "WispThread.h"
#include <process.h>

#define THREAD_USE_CLOCK 0

#if THREAD_USE_CLOCK == 1
#include <ctime>
#else
#include <Mmsystem.h>

#pragma comment(lib, "Winmm.lib")
#endif

namespace WISP_THREAD
{
//----------------------------------------------------------------------------------
unsigned __stdcall CThreadLoop(void *arg)
{
	CThread *parent = (CThread*)arg;

	while (parent->IsActive())
	{
		while (parent->Paused())
			Sleep(1);

#if THREAD_USE_CLOCK == 1
		parent->OnExecute(clock());
#else
		parent->OnExecute(timeGetTime());
#endif

		if (!parent->Cycled())
			break;
		else
		{
			int delay = parent->Delay();

			if (delay > 0)
				Sleep(delay);
		}
	}

	parent->OnDestroy();
	delete parent;

	_endthreadex(0);

	return 0;
};
//----------------------------------------------------------------------------------
unsigned __stdcall CThreadLoopSynchronizedDelay(void *arg)
{
	CThread *parent = (CThread*)arg;
	//timeBeginPeriod(1);

	while (parent->IsActive())
	{
		while (parent->Paused())
			Sleep(1);

#if THREAD_USE_CLOCK == 1
		uint nowTime = clock();
#else
		uint nowTime = timeGetTime();
#endif

		parent->OnExecute(nowTime);

		if (!parent->Cycled())
			break;
		else
		{
#if THREAD_USE_CLOCK == 1
			int delay = (int)((nowTime + parent->Delay()) - clock());
#else
			int delay = (int)((nowTime + parent->Delay()) - timeGetTime());
#endif

			if (delay > 0)
				Sleep(delay);
			else
				Sleep(1);
		}
	}

	//timeEndPeriod(1);

	parent->OnDestroy();
	delete parent;

	_endthreadex(0);

	return 0;
};
//----------------------------------------------------------------------------------
CThread::CThread()
: m_Handle(0), m_ID(0), m_Paused(false), m_Cycled(false), m_Delay(1), m_Active(false)
{
	//DebugMsg("CThread\n");
	InitializeCriticalSection(&m_CriticalSection);
}
//----------------------------------------------------------------------------------
CThread::~CThread()
{
	//DebugMsg("~CThread\n");
	DeleteCriticalSection(&m_CriticalSection);

	if (m_Handle != 0)
	{
		::CloseHandle(m_Handle);
		m_Handle = 0;
	}

	m_ID = 0;
}
//----------------------------------------------------------------------------------
void CThread::Run(bool cycled, int delay, bool synchronizedDelay)
{
	if (!m_Active && m_Handle == 0)
	{
		m_Cycled = cycled;
		m_Delay = delay;
		m_Active = true;

		if (synchronizedDelay)
			m_Handle = (HANDLE)_beginthreadex(NULL, 0, CThreadLoopSynchronizedDelay, this, 0, &m_ID);
		else
			m_Handle = (HANDLE)_beginthreadex(NULL, 0, CThreadLoop, this, 0, &m_ID);
	}
}
//----------------------------------------------------------------------------------
bool CThread::IsActive()
{
	EnterCriticalSection(&m_CriticalSection);
	bool result = m_Active;
	LeaveCriticalSection(&m_CriticalSection);

	return result;
}
//----------------------------------------------------------------------------------
void CThread::Stop()
{
	EnterCriticalSection(&m_CriticalSection);
	m_Active = false;
	LeaveCriticalSection(&m_CriticalSection);
}
//----------------------------------------------------------------------------------
bool CThread::Cycled()
{
	EnterCriticalSection(&m_CriticalSection);
	bool result = m_Cycled;
	LeaveCriticalSection(&m_CriticalSection);

	return result;
}
//----------------------------------------------------------------------------------
void CThread::Pause()
{
	EnterCriticalSection(&m_CriticalSection);
	m_Paused = true;
	LeaveCriticalSection(&m_CriticalSection);
}
//----------------------------------------------------------------------------------
void CThread::Resume()
{
	EnterCriticalSection(&m_CriticalSection);
	m_Paused = false;
	LeaveCriticalSection(&m_CriticalSection);
}
//----------------------------------------------------------------------------------
bool CThread::Paused()
{
	EnterCriticalSection(&m_CriticalSection);
	bool result = m_Paused;
	LeaveCriticalSection(&m_CriticalSection);

	return result;
}
//----------------------------------------------------------------------------------
int CThread::Delay()
{
	EnterCriticalSection(&m_CriticalSection);
	int result = m_Delay;
	LeaveCriticalSection(&m_CriticalSection);

	return result;
}
//----------------------------------------------------------------------------------
void CThread::ChangeDelay(int newDelay)
{
	EnterCriticalSection(&m_CriticalSection);
	m_Delay = newDelay;
	LeaveCriticalSection(&m_CriticalSection);
}
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
