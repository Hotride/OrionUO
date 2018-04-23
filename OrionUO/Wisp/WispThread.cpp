// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//----------------------------------------------------------------------------------
#include "stdafx.h"

#include <thread>
#include <SDL_timer.h>
#include <SDL_thread.h>
#define THREAD_USE_CLOCK 0

#if USE_WISP
#define THREADCALL __stdcall
#else
#define THREADCALL SDLCALL
#endif

namespace WISP_THREAD
{
//----------------------------------------------------------------------------------
int THREADCALL CThreadLoop(void *arg)
{
	WISPFUN_DEBUG("c_trdlp");
	CThread *parent = (CThread*)arg;

	while (parent->IsActive())
	{
		while (parent->Paused())
			SDL_Delay(1);

#if THREAD_USE_CLOCK == 1
		parent->OnExecute(clock());
#else
		parent->OnExecute(SDL_GetTicks());
#endif

		if (!parent->Cycled())
			break;
		else
		{
			int delay = parent->Delay();

			if (delay > 0)
				SDL_Delay(delay);
		}
	}

	parent->OnDestroy();
	delete parent;

#if USE_WISP
	_endthreadex(0);
#endif

	return 0;
};
//----------------------------------------------------------------------------------
int THREADCALL CThreadLoopSynchronizedDelay(void *arg)
{
	WISPFUN_DEBUG("c_trdlpsd");
	CThread *parent = (CThread*)arg;

	while (parent->IsActive())
	{
		while (parent->Paused())
			SDL_Delay(1);

#if THREAD_USE_CLOCK == 1
		uint nowTime = clock();
#else
		uint nowTime = SDL_GetTicks();
#endif

		parent->OnExecute(nowTime);

		if (!parent->Cycled())
			break;
		else
		{
#if THREAD_USE_CLOCK == 1
			int delay = (int)((nowTime + parent->Delay()) - clock());
#else
			int delay = (int)((nowTime + parent->Delay()) - SDL_GetTicks());
#endif

			SDL_Delay(delay > 0 ? delay : 1);
		}
	}

	parent->OnDestroy();
	delete parent;

#if USE_WISP
	_endthreadex(0);
#endif

	return 0;
};
//----------------------------------------------------------------------------------
CThread::CThread()
{
	WISPFUN_DEBUG("c12_f1");
	//DebugMsg("CThread\n");
	InitializeCriticalSection(&m_CriticalSection);
}
//----------------------------------------------------------------------------------
CThread::~CThread()
{
	WISPFUN_DEBUG("c12_f2");
	//DebugMsg("~CThread\n");
	DeleteCriticalSection(&m_CriticalSection);

#if USE_WISP
	if (m_Handle != 0)
	{
		::CloseHandle(m_Handle);
		m_Handle = 0;
	}
#else
	int ret = 0;
	if (m_Handle)
		SDL_WaitThread(m_Handle, &ret);
	m_Handle = nullptr;
#endif
	ID = 0;
}
//----------------------------------------------------------------------------------
void CThread::Run(bool cycled, int delay, bool synchronizedDelay)
{
	WISPFUN_DEBUG("c12_f3");
	if (!m_Active && m_Handle == 0)
	{
		m_Cycled = cycled;
		m_Delay = delay;
		m_Active = true;

#if USE_WISP
		if (synchronizedDelay)
			m_Handle = (HANDLE)_beginthreadex(NULL, 0, CThreadLoopSynchronizedDelay, this, 0, &ID);
		else
			m_Handle = (HANDLE)_beginthreadex(NULL, 0, CThreadLoop, this, 0, &ID);
#else
		if (synchronizedDelay)
			m_Handle = SDL_CreateThread(CThreadLoopSynchronizedDelay, "CThreadLoopSynchronizedDelay", (void *)this);
		else
			m_Handle = SDL_CreateThread(CThreadLoop, "CThreadLoop", (void *)this);
#endif
	}
}
//----------------------------------------------------------------------------------
bool CThread::IsActive()
{
	WISPFUN_DEBUG("c12_f4");
	EnterCriticalSection(&m_CriticalSection);
	bool result = m_Active;
	LeaveCriticalSection(&m_CriticalSection);

	return result;
}
//----------------------------------------------------------------------------------
void CThread::Stop()
{
	WISPFUN_DEBUG("c12_f5");
	EnterCriticalSection(&m_CriticalSection);
	m_Active = false;
	LeaveCriticalSection(&m_CriticalSection);
}
//----------------------------------------------------------------------------------
bool CThread::Cycled()
{
	WISPFUN_DEBUG("c12_f6");
	EnterCriticalSection(&m_CriticalSection);
	bool result = m_Cycled;
	LeaveCriticalSection(&m_CriticalSection);

	return result;
}
//----------------------------------------------------------------------------------
void CThread::Pause()
{
	WISPFUN_DEBUG("c12_f7");
	EnterCriticalSection(&m_CriticalSection);
	m_Paused = true;
	LeaveCriticalSection(&m_CriticalSection);
}
//----------------------------------------------------------------------------------
void CThread::Resume()
{
	WISPFUN_DEBUG("c12_f8");
	EnterCriticalSection(&m_CriticalSection);
	m_Paused = false;
	LeaveCriticalSection(&m_CriticalSection);
}
//----------------------------------------------------------------------------------
bool CThread::Paused()
{
	WISPFUN_DEBUG("c12_f9");
	EnterCriticalSection(&m_CriticalSection);
	bool result = m_Paused;
	LeaveCriticalSection(&m_CriticalSection);

	return result;
}
//----------------------------------------------------------------------------------
int CThread::Delay()
{
	WISPFUN_DEBUG("c12_f10");
	EnterCriticalSection(&m_CriticalSection);
	int result = m_Delay;
	LeaveCriticalSection(&m_CriticalSection);

	return result;
}
//----------------------------------------------------------------------------------
void CThread::ChangeDelay(int newDelay)
{
	WISPFUN_DEBUG("c12_f11");
	EnterCriticalSection(&m_CriticalSection);
	m_Delay = newDelay;
	LeaveCriticalSection(&m_CriticalSection);
}
//----------------------------------------------------------------------------------
std::thread::id CThread::GetCurrentThreadId()
{
	//return GetCurrentThreadId();
	return std::this_thread::get_id();
}
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
