/***********************************************************************************
**
** PluginManager.cpp
**
** Copyright (C) September 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "PluginManager.h"
#include "../PluginInterface.h"
#include "../OrionWindow.h"
#include "../OrionUO.h"
#include "PacketManager.h"
#include "FileManager.h"
//----------------------------------------------------------------------------------
CPluginManager g_PluginManager;
//----------------------------------------------------------------------------------
bool __cdecl PluginRecvFunction(puchar buf, const int &size)
{
	SendMessage(g_OrionWindow.Handle, UOMSG_RECV, (WPARAM)buf, size);
	//PacketManager.PluginReceiveHandler(buf, size);

	return true;
}
//----------------------------------------------------------------------------------
bool __cdecl PluginSendFunction(puchar buf, const int &size)
{
	SendMessage(g_OrionWindow.Handle, UOMSG_SEND, (WPARAM)buf, size);
	//ConnectionManager.Send(buf, size);

	return true;
}
//----------------------------------------------------------------------------------
//--------------------------------------CPlugin-------------------------------------
//----------------------------------------------------------------------------------
CPlugin::CPlugin(uint flags)
: CBaseQueueItem(), m_Flags(flags)
{
	m_PPS = new PLUGIN_INTERFACE();
	memset(m_PPS, 0, sizeof(PLUGIN_INTERFACE));

	m_PPS->hWnd = g_OrionWindow.Handle;
	m_PPS->ClientVersion = g_PacketManager.ClientVersion;
	m_PPS->UseVerdata = g_FileManager.UseVerdata;
}
//----------------------------------------------------------------------------------
CPlugin::~CPlugin()
{
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
	QFOR(plugin, m_Items, CPlugin*)
	{
		if (plugin->m_PPS->OnDisconnect != NULL)
			plugin->m_PPS->OnDisconnect();
	}
}
//----------------------------------------------------------------------------------
void CPluginManager::WorldDraw()
{
	QFOR(plugin, m_Items, CPlugin*)
	{
		if (plugin->CanEnterWorldRender() && plugin->m_PPS->OnWorldDraw != NULL)
			plugin->m_PPS->OnWorldDraw();
	}
}
//----------------------------------------------------------------------------------
void CPluginManager::SceneDraw()
{
	QFOR(plugin, m_Items, CPlugin*)
	{
		if (plugin->CanEnterSceneRender() && plugin->m_PPS->OnSceneDraw != NULL)
			plugin->m_PPS->OnSceneDraw();
	}
}
//----------------------------------------------------------------------------------
void CPluginManager::WorldMapDraw()
{
	QFOR(plugin, m_Items, CPlugin*)
	{
		if (plugin->CanEnterWorldMapRender() && plugin->m_PPS->OnWorldMapDraw != NULL)
			plugin->m_PPS->OnWorldMapDraw();
	}
}
//----------------------------------------------------------------------------------
