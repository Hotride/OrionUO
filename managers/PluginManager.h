/****************************************************************************
**
** PluginManager.h
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
#ifndef PluginManagerH
#define PluginManagerH
//---------------------------------------------------------------------------
class TPlugin : public TBaseQueueItem
{
private:
	DWORD m_Flags;
public:
	TPlugin(DWORD flags);
	virtual ~TPlugin();

	bool CanParseRecv() {return (m_Flags & PLUGIN_FLAGS_PARSE_RECV);}
	bool CanParseSend() {return (m_Flags & PLUGIN_FLAGS_PARSE_SEND);}
	bool CanRecv() {return (m_Flags & PLUGIN_FLAGS_RECV);}
	bool CanSend() {return (m_Flags & PLUGIN_FLAGS_SEND);}
	bool CanWindowProc() {return (m_Flags & PLUGIN_FLAGS_WINDOW_PROC);}
	bool CanClientAccess() {return (m_Flags & PLUGIN_FLAGS_CLIENT_ACCESS);}
	bool CanEnterWorldRender() {return (m_Flags & PLUGIN_FLAGS_GAME_WORLD_DRAW);}
	bool CanEnterSceneRender() {return (m_Flags & PLUGIN_FLAGS_SCENE_DRAW);}

	PPLUGIN_INTERFACE m_PPS;
};
//---------------------------------------------------------------------------
class TPluginManager : public TBaseQueue
{
private:
public:
	TPluginManager();
	virtual ~TPluginManager() {}

	bool WindowProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
	bool PacketRecv(PBYTE buf, int size);
	bool PacketSend(PBYTE buf, int size);
	void Disconnect();
	void WorldDraw();
	void SceneDraw();
};
//---------------------------------------------------------------------------
extern TPluginManager *PluginManager;
//---------------------------------------------------------------------------
#endif