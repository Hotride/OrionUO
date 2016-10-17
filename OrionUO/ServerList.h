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
#include "Globals.h"
#include "GLEngine/GLTextTexture.h"
//----------------------------------------------------------------------------------
class CServer
{
	SETGET(ushort, Index);
	SETGET(string, Name);
	SETGET(uchar, FullPercent);
	SETGET(uchar, Timezone);
	SETGET(uint, IP);
	SETGET(bool, Selected);

public:
	CServer();
	CServer(const ushort &index, const string &name, const uchar &fullPercent, const uchar &timezone, const uint &ip, const bool &selected);
	~CServer();
};
//----------------------------------------------------------------------------------
//Класс для хранения списка серверов
class CServerList
{
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
