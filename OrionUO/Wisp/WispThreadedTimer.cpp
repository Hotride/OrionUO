// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//----------------------------------------------------------------------------------
#include "stdafx.h"

namespace WISP_THREADED_TIMER
{
//----------------------------------------------------------------------------------
CThreadedTimer::CThreadedTimer(uint id, HWND windowHandle, bool wairForProcessMessage)
: WISP_THREAD::CThread(), TimerID(id), WindowHandle(windowHandle),
WairForProcessMessage(wairForProcessMessage)
{
}
//----------------------------------------------------------------------------------
CThreadedTimer::~CThreadedTimer()
{
}
//----------------------------------------------------------------------------------
void CThreadedTimer::OnExecute(uint nowTime)
{
	WISPFUN_DEBUG("c13_f1");
	if (IsActive())
	{
		if (WairForProcessMessage)
			SendMessage(WindowHandle, MessageID, nowTime, (LPARAM)this);
		else
			PostMessage(WindowHandle, MessageID, nowTime, (LPARAM)this);
	}
}
//----------------------------------------------------------------------------------
void CThreadedTimer::OnDestroy()
{
	WISPFUN_DEBUG("c13_f2");
	if (WISP_WINDOW::g_WispWindow != NULL)
		WISP_WINDOW::g_WispWindow->RemoveThreadedTimer(TimerID);
}
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
