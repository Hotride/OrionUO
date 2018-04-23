﻿//----------------------------------------------------------------------------------
#pragma once
#include <SDL_thread.h>
//----------------------------------------------------------------------------------
namespace WISP_THREAD
{
//----------------------------------------------------------------------------------
class CThread
{
private:
#if USE_WISP
	HANDLE m_Handle{ 0 };
#else
	SDL_Thread *m_Handle = nullptr;
#endif
	UINT ID{ 0 };
	CRITICAL_SECTION m_CriticalSection;
	bool m_Paused{ false };
	bool m_Cycled{ false };
	int m_Delay{ 1 };
	bool m_Active{ false };

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

	static std::thread::id GetCurrentThreadId();
};
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
