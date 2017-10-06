/***********************************************************************************
**
** PluginManager.cpp
**
** Copyright (C) September 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CPluginManager g_PluginManager;
//----------------------------------------------------------------------------------
bool __cdecl PluginRecvFunction(puchar buf, const int &size)
{
	WISPFUN_DEBUG("c_plgrcvfnc");
	SendMessage(g_OrionWindow.Handle, UOMSG_RECV, (WPARAM)buf, size);
	
	//g_PacketManager.SavePluginReceivePacket(buf, size);

	return true;
}
//----------------------------------------------------------------------------------
bool __cdecl PluginSendFunction(puchar buf, const int &size)
{
	WISPFUN_DEBUG("c_plgsndfnc");
	SendMessage(g_OrionWindow.Handle, UOMSG_SEND, (WPARAM)buf, size);

	/*uint ticks = g_Ticks;
	g_TotalSendSize += size;

	CPacketInfo &type = g_PacketManager.GetInfo(*buf);

	LOG("--- ^(%d) s(+%d => %d) Plugin->Server:: %s\n", ticks - g_LastPacketTime, size, g_TotalSendSize, type.Name);

	g_LastPacketTime = ticks;
	g_LastSendTime = ticks;

	if (*buf == 0x80 || *buf == 0x91)
	{
		LOG_DUMP(buf, 1);
		LOG("**** ACCOUNT AND PASSWORD CENSORED ****\n");
	}
	else
		LOG_DUMP(buf, size);

	g_ConnectionManager.Send(buf, size);*/

	return true;
}
//----------------------------------------------------------------------------------
//--------------------------------------CPlugin-------------------------------------
//----------------------------------------------------------------------------------
CPlugin::CPlugin(uint flags)
: CBaseQueueItem(), m_Flags(flags)
{
	WISPFUN_DEBUG("c151_f1");
	m_PPS = new PLUGIN_INTERFACE();
	memset(m_PPS, 0, sizeof(PLUGIN_INTERFACE));

	m_PPS->hWnd = g_OrionWindow.Handle;
	m_PPS->ClientVersion = g_PacketManager.ClientVersion;
	m_PPS->UseVerdata = g_FileManager.UseVerdata;
}
//----------------------------------------------------------------------------------
CPlugin::~CPlugin()
{
	WISPFUN_DEBUG("c151_f2");
	if (m_PPS != NULL)
	{
		delete m_PPS;
		m_PPS = NULL;
	}
}
//----------------------------------------------------------------------------------
//-----------------------------------CPluginManager---------------------------------
//----------------------------------------------------------------------------------
CPluginManager::CPluginManager()
: CBaseQueue()
{
}
//----------------------------------------------------------------------------------
LRESULT CPluginManager::WindowProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	WISPFUN_DEBUG("c152_f1");
	LRESULT result = 0;

	QFOR(plugin, m_Items, CPlugin*)
	{
		if (plugin->CanWindowProc() && plugin->m_PPS->WindowProc != NULL)
		{
			result = plugin->m_PPS->WindowProc(hWnd, msg, wparam, lparam);
			/*bool funRet = plugin->m_PPS->WindowProc(hWnd, msg, wparam, lparam);

			if (result)
				result = funRet;*/
		}
	}

	return result;
}
//----------------------------------------------------------------------------------
bool CPluginManager::PacketRecv(puchar buf, const int &size)
{
	WISPFUN_DEBUG("c152_f2");
	bool result = true;

	QFOR(plugin, m_Items, CPlugin*)
	{
		if (plugin->CanParseRecv() && plugin->m_PPS->OnRecv != NULL)
		{
			bool funRet = plugin->m_PPS->OnRecv(buf, size);

			if (result)
				result = funRet;
		}
	}

	return result;
}
//----------------------------------------------------------------------------------
bool CPluginManager::PacketSend(puchar buf, const int &size)
{
	WISPFUN_DEBUG("c152_f3");
	bool result = true;

	QFOR(plugin, m_Items, CPlugin*)
	{
		if (plugin->CanParseSend() && plugin->m_PPS->OnSend != NULL)
		{
			bool funRet = plugin->m_PPS->OnSend(buf, size);

			if (result)
				result = funRet;
		}
	}

	return result;
}
//----------------------------------------------------------------------------------
void CPluginManager::Disconnect()
{
	WISPFUN_DEBUG("c152_f4");
	QFOR(plugin, m_Items, CPlugin*)
	{
		if (plugin->m_PPS->OnDisconnect != NULL)
			plugin->m_PPS->OnDisconnect();
	}
}
//----------------------------------------------------------------------------------
void CPluginManager::WorldDraw()
{
	WISPFUN_DEBUG("c152_f5");
	QFOR(plugin, m_Items, CPlugin*)
	{
		if (plugin->CanEnterWorldRender() && plugin->m_PPS->OnWorldDraw != NULL)
			plugin->m_PPS->OnWorldDraw();
	}
}
//----------------------------------------------------------------------------------
void CPluginManager::SceneDraw()
{
	WISPFUN_DEBUG("c152_f6");
	QFOR(plugin, m_Items, CPlugin*)
	{
		if (plugin->CanEnterSceneRender() && plugin->m_PPS->OnSceneDraw != NULL)
			plugin->m_PPS->OnSceneDraw();
	}
}
//----------------------------------------------------------------------------------
void CPluginManager::WorldMapDraw()
{
	WISPFUN_DEBUG("c152_f7");
	QFOR(plugin, m_Items, CPlugin*)
	{
		if (plugin->CanEnterWorldMapRender() && plugin->m_PPS->OnWorldMapDraw != NULL)
			plugin->m_PPS->OnWorldMapDraw();
	}
}
//----------------------------------------------------------------------------------
