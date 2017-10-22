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
	deque<CServer*> m_Servers;

public:
	CServerList();
	~CServerList();

	//Очистить списки серверов
	void Clear();

	void AddServer(CServer *server);
	CServer *GetServer(int index);
	CServer *GetSelectedServer();
	int ServersCount() { return (int)m_Servers.size(); }

	CServer *Select(int index);
};
//----------------------------------------------------------------------------------
extern CServerList g_ServerList;
//----------------------------------------------------------------------------------
#endif
