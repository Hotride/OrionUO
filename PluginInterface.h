/****************************************************************************
**
** PluginInterface.h
**
** Copyright (C) April 2016 Hotride
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
#ifndef PluginInterfaceH
#define PluginInterfaceH
//---------------------------------------------------------------------------
typedef bool WINDOW_PROC(HWND, UINT, WPARAM, LPARAM);
typedef bool __cdecl PACKET_PROC(PBYTE, int);
typedef void __cdecl VOID_PROC();
//---------------------------------------------------------------------------
#define UOMSG_SET_ACCOUNT			WM_USER + 660
#define UOMSG_SET_SERVER_NAME		WM_USER + 661
#define UOMSG_SET_PLAYER_NAME		WM_USER + 662
#define UOMSG_UPDATE_PLAYER_X		WM_USER + 663
#define UOMSG_UPDATE_PLAYER_Y		WM_USER + 664
#define UOMSG_UPDATE_PLAYER_Z		WM_USER + 665
#define UOMSG_UPDATE_PLAYER_DIR		WM_USER + 666
//---------------------------------------------------------------------------
bool __cdecl PluginRecvFunction(PBYTE buf, int size);
bool __cdecl PluginSendFunction(PBYTE buf, int size);
//---------------------------------------------------------------------------
typedef struct PLUGIN_INTERFACE
{
	struct PLUGIN_CLIENT_INTERFACE *Client;

	HWND hWnd;
	CLIENT_VERSION ClientVersion;
	bool InverseBuylist;
	bool UseVerdata;

	PACKET_PROC *Recv;
	PACKET_PROC *Send;

	class TPlugin *Owner;

	WINDOW_PROC *WindowProc;
	PACKET_PROC *OnRecv;
	PACKET_PROC *OnSend;
	VOID_PROC *OnDisconnect;
	VOID_PROC *OnWorldDraw;
	VOID_PROC *OnSceneDraw;
} *PPLUGIN_INTERFACE;
//---------------------------------------------------------------------------
#endif