// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** PingThread.cpp
**
** Copyright (C) December 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
#include <SDL_events.h>
#include <SDL_timer.h>
//----------------------------------------------------------------------------------
#if !USE_WISP
uint32_t CPingThread::m_PingEvent = 0;
#endif
//----------------------------------------------------------------------------------
CPingThread::CPingThread(int serverID, const string &serverIP, int requestsCount)
    : WISP_THREAD::CThread()
    , ServerID(serverID)
    , ServerIP(serverIP)
    , RequestsCount(requestsCount)
{
#if !USE_WISP
    if (!m_PingEvent)
    {
        m_PingEvent = SDL_RegisterEvents(1);
    }
#endif

    LOG("CPingThread => %s\n", serverIP.c_str());
    WISPFUN_DEBUG("");
}
//----------------------------------------------------------------------------------
CPingThread::~CPingThread()
{
    WISPFUN_DEBUG("");
}
//----------------------------------------------------------------------------------
int CPingThread::CalculatePing()
{
    auto handle = icmp_open();
    if (!handle)
        return -4;

    uint32_t timems = SDL_GetTicks();
    int result = icmp_query(handle, ServerIP.c_str(), &timems);
    if (!result)
        result = (SDL_GetTicks() - timems);

    icmp_close(handle);

    return result;
}
//----------------------------------------------------------------------------------
void CPingThread::OnExecute(uint32_t nowTime)
{
    WISPFUN_DEBUG("");

    if (ServerIP.empty() || RequestsCount < 1)
        return;

    PING_INFO_DATA info = { ServerID, 9999, 0, 0, 0 };

    IFOR (i, 0, RequestsCount)
    {
        int ping = CalculatePing();

        if (ping < 0)
        {
            if (ping == -1)
                info.Lost++;

            continue;
        }

        info.Min = min(info.Min, ping);
        info.Max = max(info.Max, ping);

        info.Average += (info.Max - info.Min);
    }

    info.Average = info.Min + (info.Average / RequestsCount);

#if USE_WISP
    SendMessage(g_OrionWindow.Handle, MessageID, (WPARAM)&info, 0);
#else
    SDL_Event event;
    SDL_memset(&event, 0, sizeof(event));
    event.type = m_PingEvent;
    event.user.data1 = (void *)&info;
    SDL_PushEvent(&event);
#endif
}
//----------------------------------------------------------------------------------
