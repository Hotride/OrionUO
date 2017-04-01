/***********************************************************************************
**
** ServerList.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "ServerList.h"
#include "Managers/FontsManager.h"

CServerList g_ServerList;
//----------------------------------------------------------------------------------
//---------------------------------------CServer------------------------------------
//----------------------------------------------------------------------------------
CServer::CServer()
: m_Index(0), m_Name(""), m_FullPercent(0), m_Timezone(0), m_IP(0), m_Selected(false)
{
}
//----------------------------------------------------------------------------------
CServer::CServer(const ushort &index, const string &name, const uchar &fullPercent, const uchar &timezone, const uint &ip, const bool &selected)
: m_Index(index), m_Name(name), m_FullPercent(fullPercent), m_Timezone(timezone),
m_IP(ip), m_Selected(selected)
{
}
//----------------------------------------------------------------------------------
CServer::~CServer()
{
}
//----------------------------------------------------------------------------------
//-------------------------------------CServerList----------------------------------
//----------------------------------------------------------------------------------
CServerList::CServerList()
{
}
//----------------------------------------------------------------------------------
CServerList::~CServerList()
{
	Clear();
}
//----------------------------------------------------------------------------------
void CServerList::Clear()
{
	WISPFUN_DEBUG("c206_f1");
	for (deque<CServer*>::iterator i = m_Servers.begin(); i != m_Servers.end(); i++)
		delete *i;

	m_Servers.clear();
}
//----------------------------------------------------------------------------------
void CServerList::AddServer(CServer *server)
{
	WISPFUN_DEBUG("c206_f2");
	m_Servers.push_back(server);
}
//----------------------------------------------------------------------------------
CServer *CServerList::GetServer(int index)
{
	WISPFUN_DEBUG("c206_f3");
	for (deque<CServer*>::iterator i = m_Servers.begin(); i != m_Servers.end(); i++, index--)
	{
		if (!index)
			return *i;
	}

	return NULL;
}
//----------------------------------------------------------------------------------
CServer *CServerList::GetSelectedServer()
{
	WISPFUN_DEBUG("c206_f4");
	for (deque<CServer*>::iterator i = m_Servers.begin(); i != m_Servers.end(); i++)
	{
		if ((*i)->Selected)
			return *i;
	}

	return NULL;
}
//----------------------------------------------------------------------------------
CServer *CServerList::Select(int index)
{
	WISPFUN_DEBUG("c206_f5");
	CServer *server = NULL;

	for (deque<CServer*>::iterator i = m_Servers.begin(); i != m_Servers.end(); i++, index--)
	{
		(*i)->Selected = false;

		if (!index)
		{
			server = *i;
			server->Selected = true;
		}
	}

	return server;
}
//----------------------------------------------------------------------------------
