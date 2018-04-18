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
public:
	ushort Index = 0;
	string Name = "";
	uchar FullPercent = 0;
	uchar Timezone = 0;
	uint IP = 0;
	bool Selected = false;
	int Ping = -1;
	int PacketsLoss = -1;

	CServer();
	CServer(ushort index, const string &name, uchar fullPercent, uchar timezone, int ip, bool selected);
	~CServer();
};
//----------------------------------------------------------------------------------
//Класс для хранения списка серверов
class CServerList
{
public:
	string LastServerName = "";
	uint LastServerIndex = 0;

private:
	vector<CServer> m_Servers;

public:
	CServerList();
	~CServerList();

	CServer *GetServer(int index);
	CServer *GetSelectedServer();
	int ServersCount() { return (int)m_Servers.size(); }

	CServer *Select(int index);

	void ParsePacket(WISP_DATASTREAM::CDataReader &reader);
};
//----------------------------------------------------------------------------------
extern CServerList g_ServerList;
//----------------------------------------------------------------------------------
#endif
