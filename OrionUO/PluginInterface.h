﻿/***********************************************************************************
**
** PluginInterface.h
**
** Copyright (C) September 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#pragma once
//----------------------------------------------------------------------------------
#if defined(ORION_LINUX)
#include "stdafx.h"
#define CDECL
#else
#include <Windows.h>
#endif
//----------------------------------------------------------------------------------
#pragma pack (push,1)
typedef struct PLUGIN_INFO
{
	char FileName[MAX_PATH];
	char FunctionName[100];
	uint64_t Flags;
} *PPLUGIN_INFO;

#pragma pack (pop)
//----------------------------------------------------------------------------------
typedef LRESULT WINDOW_PROC(HWND, UINT, WPARAM, LPARAM);
typedef bool CDECL PACKET_PROC(unsigned char *, const int&);
typedef void CDECL VOID_PROC();
typedef bool CDECL WORLD_MAP_DRAW_PROC();
//----------------------------------------------------------------------------------
#define UOMSG_SET_SERVER_NAME		WM_USER + 660
#define UOMSG_SET_PLAYER_NAME		WM_USER + 661
#define UOMSG_UPDATE_PLAYER_XYZ		WM_USER + 662
#define UOMSG_UPDATE_PLAYER_DIR		WM_USER + 663
#define UOMSG_RECV					WM_USER + 664
#define UOMSG_SEND					WM_USER + 665
#define UOMSG_IP_SEND				WM_USER + 666
#define UOMSG_PATHFINDING			WM_USER + 667
#define UOMSG_WALK					WM_USER + 668
#define UOMSG_MENU_RESPONSE			WM_USER + 669
#define UOMSG_STATUS_REQUEST		WM_USER + 670
#define UOMSG_SELECTED_TILE			WM_USER + 671
#define UOMSG_END_MACRO_PAYING		WM_USER + 672
#define UOMSG_UPDATE_REMOVE_POS		WM_USER + 673
//----------------------------------------------------------------------------------
#pragma pack (push,1)
typedef struct UOI_PLAYER_XYZ_DATA
{
	int X;
	int Y;
	int Z;
} *PUOI_PLAYER_XYZ_DATA;

typedef struct UOI_SELECTED_TILE
{
	unsigned int Serial;
	unsigned short Graphic;
	unsigned short Color;
	int X;
	int Y;
	int Z;
	unsigned short LandGraphic;
	int LandX;
	int LandY;
	int LandZ;
} *PUOI_SELECTED_TILE;

typedef struct UOI_MENU_RESPONSE
{
	unsigned int Serial;
	unsigned int ID;
	int Code;
} *PUOI_MENU_RESPONSE;
//----------------------------------------------------------------------------------
bool CDECL PluginRecvFunction(unsigned char *buf, const int &size);
bool CDECL PluginSendFunction(unsigned char *buf, const int &size);
//----------------------------------------------------------------------------------
typedef struct PLUGIN_INTERFACE
{
	int InterfaceVersion;
	int Size;
	struct PLUGIN_CLIENT_INTERFACE *Client;

	HWND WindowHandle;
	int ClientVersion;
	int ClientFlags;

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
#pragma pack (pop)
//----------------------------------------------------------------------------------
