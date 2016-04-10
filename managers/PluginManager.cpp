/****************************************************************************
**
** PluginManager.cpp
**
** Copyright (C) November 2015 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//---------------------------------------------------------------------------
#include "stdafx.h"

TPluginManager *PluginManager = NULL;
//---------------------------------------------------------------------------
bool __cdecl PluginRecvFunction(PBYTE buf, int size)
{
	SendMessage(g_hWnd, UOMSG_RECV, (WPARAM)buf, size);
	//PacketManager.PluginReceiveHandler(buf, size);

	return true;
}
//---------------------------------------------------------------------------
bool __cdecl PluginSendFunction(PBYTE buf, int size)
{
	SendMessage(g_hWnd, UOMSG_SEND, (WPARAM)buf, size);
	//ConnectionManager.Send(buf, size);

	return true;
}
//---------------------------------------------------------------------------
//-----------------------------------TPlugin---------------------------------
//---------------------------------------------------------------------------
TPlugin::TPlugin(DWORD flags)
: TBaseQueueItem(), m_Flags(flags)
{
	m_PPS = new PLUGIN_INTERFACE();

	memset(m_PPS, 0, sizeof(PLUGIN_INTERFACE));

	m_PPS->hWnd = g_hWnd;
	m_PPS->ClientVersion = PacketManager.GetClientVersion();
	m_PPS->InverseBuylist = g_InverseBuylist;
	m_PPS->UseVerdata = FileManager.UseVerdata;
}
//---------------------------------------------------------------------------
TPlugin::~TPlugin()
{
	if (m_PPS != NULL)
	{
		delete m_PPS;
		m_PPS = NULL;
	}
}
//---------------------------------------------------------------------------
//--------------------------------TPluginManager-----------------------------
//---------------------------------------------------------------------------
TPluginManager::TPluginManager()
: TBaseQueue()
{
}
//---------------------------------------------------------------------------
bool TPluginManager::WindowProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	bool result = true;

	QFOR(plugin, m_Items, TPlugin*)
	{
		if (plugin->CanWindowProc() && plugin->m_PPS->WindowProc != NULL)
		{
			bool funRet = plugin->m_PPS->WindowProc(hWnd, msg, wparam, lparam);

			if (result)
				result = funRet;
		}
	}

	return result;
}
//---------------------------------------------------------------------------
bool TPluginManager::PacketRecv(PBYTE buf, int size)
{
	bool result = true;

	QFOR(plugin, m_Items, TPlugin*)
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
//---------------------------------------------------------------------------
bool TPluginManager::PacketSend(PBYTE buf, int size)
{
	bool result = true;

	QFOR(plugin, m_Items, TPlugin*)
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
//---------------------------------------------------------------------------
void TPluginManager::Disconnect()
{
	QFOR(plugin, m_Items, TPlugin*)
	{
		if (plugin->m_PPS->OnDisconnect != NULL)
			plugin->m_PPS->OnDisconnect();
	}
}
//---------------------------------------------------------------------------
void TPluginManager::WorldDraw()
{
	QFOR(plugin, m_Items, TPlugin*)
	{
		if (plugin->CanEnterWorldRender() && plugin->m_PPS->OnWorldDraw != NULL)
			plugin->m_PPS->OnWorldDraw();
	}
}
//---------------------------------------------------------------------------
void TPluginManager::SceneDraw()
{
	QFOR(plugin, m_Items, TPlugin*)
	{
		if (plugin->CanEnterSceneRender() && plugin->m_PPS->OnSceneDraw != NULL)
			plugin->m_PPS->OnSceneDraw();
	}
}
//---------------------------------------------------------------------------