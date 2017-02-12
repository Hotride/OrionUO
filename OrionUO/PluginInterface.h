/***********************************************************************************
**
** PluginInterface.h
**
** Copyright (C) September 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H
//----------------------------------------------------------------------------------
#include <Windows.h>
//----------------------------------------------------------------------------------
typedef LRESULT WINDOW_PROC(HWND, UINT, WPARAM, LPARAM);
typedef bool __cdecl PACKET_PROC(unsigned char *, const int&);
typedef void __cdecl VOID_PROC();
typedef bool __cdecl WORLD_MAP_DRAW_PROC();
//----------------------------------------------------------------------------------
#define UOMSG_SET_SERVER_NAME		WM_USER + 660
#define UOMSG_SET_PLAYER_NAME		WM_USER + 661
#define UOMSG_UPDATE_PLAYER_XYZ		WM_USER + 662
#define UOMSG_UPDATE_PLAYER_DIR		WM_USER + 663
#define UOMSG_RECV					WM_USER + 664
#define UOMSG_SEND					WM_USER + 665
#define UOMSG_IP_SEND				WM_USER + 666
#define UOMSG_PATHFINDING			WM_USER + 667
//----------------------------------------------------------------------------------
#pragma pack (push,1)
typedef struct PLAYER_XYZ_DATA
{
	int X;
	int Y;
	int Z;
} *PPLAYER_XYZ_DATA;
#pragma pack (pop)
//----------------------------------------------------------------------------------
bool __cdecl PluginRecvFunction(unsigned char *buf, const int &size);
bool __cdecl PluginSendFunction(unsigned char *buf, const int &size);
//----------------------------------------------------------------------------------
typedef struct PLUGIN_INTERFACE
{
	struct PLUGIN_CLIENT_INTERFACE *Client;

	HWND hWnd;
	CLIENT_VERSION ClientVersion;
	bool UseVerdata;

	PACKET_PROC *Recv;
	PACKET_PROC *Send;

	class CPlugin *Owner;

	WINDOW_PROC *WindowProc;
	PACKET_PROC *OnRecv;
	PACKET_PROC *OnSend;
	VOID_PROC *OnDisconnect;
	VOID_PROC *OnWorldDraw;
	VOID_PROC *OnSceneDraw;
	WORLD_MAP_DRAW_PROC *OnWorldMapDraw;
} *PPLUGIN_INTERFACE;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
