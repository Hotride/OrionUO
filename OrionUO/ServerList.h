/***********************************************************************************
**
** ServerList.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef SERVERLIST_H
#define SERVERLIST_H
//----------------------------------------------------------------------------------
class CServer
{
	SETGET(ushort, Index, 0);
	SETGET(string, Name, "");
	SETGET(uchar, FullPercent, 0);
	SETGET(uchar, Timezone, 0);
	SETGET(uint, IP, 0);
	SETGET(bool, Selected, false);
	SETGET(int, Ping, -1);
	SETGET(int, PacketsLoss, -1);

public:
	CServer();
	CServer(const ushort &index, const string &name, const uchar &fullPercent, const uchar &timezone, const uint &ip, const bool &selected);
	~CServer();
};
//----------------------------------------------------------------------------------
//Класс для хранения списка серверов
class CServerList
{
	SETGET(string, LastServerName, "");
	SETGET(uint, LastServerIndex, 0);

private:
	vector<CServer> m_Servers;

public:
	CServerList();
	~CServerList();

	CServer *GetServer(const uint &index);
	CServer *GetSelectedServer();
	int ServersCount() { return (int)m_Servers.size(); }

	CServer *Select(int index);

	void ParsePacket(WISP_DATASTREAM::CDataReader &reader);
};
//----------------------------------------------------------------------------------
extern CServerList g_ServerList;
//----------------------------------------------------------------------------------
#endif
