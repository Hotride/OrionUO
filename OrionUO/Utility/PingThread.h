/***********************************************************************************
**
** PingThread.h
**
** Copyright (C) December 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef PINGTHREAD_H
#define PINGTHREAD_H
//----------------------------------------------------------------------------------
struct PING_INFO_DATA
{
    uint ServerID;
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
    ushort CalculateChecksum(pushort addr, int count);

    int CalculatePing();

public:
    CPingThread(int serverID, const string &serverIP, int requestsCount);
    virtual ~CPingThread();

    static const uint MessageID = WM_USER + 401;

    virtual void OnExecute(uint nowTime);
};
//----------------------------------------------------------------------------------
#endif //PINGTHREAD_H
//----------------------------------------------------------------------------------
