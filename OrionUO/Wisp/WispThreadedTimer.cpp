// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//----------------------------------------------------------------------------------
#include "stdafx.h"
#include <SDL_events.h>

namespace WISP_THREADED_TIMER
{
//----------------------------------------------------------------------------------
CThreadedTimer::CThreadedTimer(uint id, HWND windowHandle, bool waitForProcessMessage)
    : WISP_THREAD::CThread()
    , TimerID(id)
    , WindowHandle(windowHandle)
    , WaitForProcessMessage(waitForProcessMessage)
{
#if !USE_WISP
    EventID = SDL_RegisterEvents(1);
#endif
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
#if USE_WISP
        if (WaitForProcessMessage)
            SendMessage(WindowHandle, MessageID, nowTime, (LPARAM)this);
        else
            PostMessage(WindowHandle, MessageID, nowTime, (LPARAM)this);
#else
        SDL_Event event;
        SDL_zero(event);
        event.type = EventID;
        event.user.code = MessageID;
        event.user.data1 = (void *)nowTime;
        event.user.data2 = this;
        SDL_PushEvent(&event);
#endif
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
}; // namespace WISP_THREADED_TIMER
//----------------------------------------------------------------------------------
