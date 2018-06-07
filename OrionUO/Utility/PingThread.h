/***********************************************************************************
**
** PingThread.h
**
** Copyright (C) December 2017 Hotride
**
************************************************************************************
*/
#pragma once

struct PING_INFO_DATA
{
    uint32_t ServerID;
    int Min;
    int Max;
    int Average;
    int Lost;
};
//----------------------------------------------------------------------------------
class CPingThread : public WISP_THREAD::CThread
{
    uint ServerID = 0xFFFFFFFF;
    string ServerIP = "";
    int RequestsCount = 10;

private:
    int CalculatePing();

public:
    CPingThread(int serverID, const string &serverIP, int requestsCount);
    virtual ~CPingThread();

    static const uint MessageID = WM_USER + 401;
#if !USE_WISP
    static uint32_t m_PingEvent;
#endif

    virtual void OnExecute(uint32_t nowTime);
};