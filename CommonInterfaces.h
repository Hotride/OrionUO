/****************************************************************************
**
** CommonInterfaces.h
**
** Copyright (C) December 2015 Hotride
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
#ifndef CommonInterfacesH
#define CommonInterfacesH
//----------------------------------------------------------------------------
#ifndef UOInterface
#define UOInterface struct
#endif
//----------------------------------------------------------------------------
//IGLEngine
typedef void __cdecl FUNCDEF_SCISSOR(int, int, int, int);
typedef void __cdecl FUNCDEF_DRAW_LINE(DWORD, int, int, int, int);
typedef void __cdecl FUNCDEF_DRAW_POLYGONE(DWORD, int, int, int, int);
typedef void __cdecl FUNCDEF_DRAW_CIRCLE(DWORD, float, float, float, int);

//IUltimaOnline
typedef DWORD __cdecl FUNCDEF_GET_LAND_FLAGS(WORD);
typedef DWORD __cdecl FUNCDEF_GET_STATIC_FLAGS(WORD);

//IClilocManager
typedef string __cdecl FUNCDEF_GET_CLILOC_A(DWORD, string);
typedef wstring __cdecl FUNCDEF_GET_CLILOC_W(DWORD, string);
typedef wstring __cdecl FUNCDEF_GET_CLILOC_ARGUMENTS(DWORD, wstring);

//IColorManager
typedef int __cdecl FUNCDEF_GET_HUES_COUNT();
typedef WORD __cdecl FUNCDEF_GET_COLOR32TO16(DWORD&);
typedef DWORD __cdecl FUNCDEF_GET_COLOR16TO32(WORD&);
typedef WORD __cdecl FUNCDEF_GET_COLOR_TO_GRAY(WORD&);
typedef DWORD __cdecl FUNCDEF_GET_POLYGONE_COLOR(WORD, WORD);
typedef DWORD __cdecl FUNCDEF_GET_COLOR(WORD&, WORD);
typedef DWORD __cdecl FUNCDEF_GET_PRTIAL_HUE_COLOR(WORD&, WORD);

//IPathFinder
typedef bool __cdecl FUNCDEF_GET_CAN_WALK(BYTE&, int&, int&, char&);
typedef bool __cdecl FUNCDEF_GET_WALK(bool, BYTE);
typedef bool __cdecl FUNCDEF_GET_WALK_TO(int, int, int, int);
typedef void __cdecl FUNCDEF_GET_STOP_AUTOWALK();
//----------------------------------------------------------------------------
UOInterface IGLEngine
{
	FUNCDEF_SCISSOR *Scissor;
	FUNCDEF_DRAW_LINE *DrawLine;
	FUNCDEF_DRAW_POLYGONE *DrawPolygone;
	FUNCDEF_DRAW_CIRCLE *DrawCircle;
};
//----------------------------------------------------------------------------
UOInterface IUltimaOnline
{
	FUNCDEF_GET_LAND_FLAGS *GetLandFlags;
	FUNCDEF_GET_STATIC_FLAGS *GetStaticFlags;
};
//----------------------------------------------------------------------------
UOInterface IClilocManager
{
	FUNCDEF_GET_CLILOC_A *GetClilocA;
	FUNCDEF_GET_CLILOC_W *GetClilocW;

	FUNCDEF_GET_CLILOC_ARGUMENTS *GetClilocArguments;
};
//----------------------------------------------------------------------------
UOInterface IColorManager
{
	FUNCDEF_GET_HUES_COUNT *GetHuesCount;

	FUNCDEF_GET_COLOR32TO16 *Color32To16;
	FUNCDEF_GET_COLOR16TO32 *Color16To32;
	FUNCDEF_GET_COLOR_TO_GRAY *ConvertToGray;

	FUNCDEF_GET_POLYGONE_COLOR *GetPolygoneColor;
	FUNCDEF_GET_COLOR *GetColor;
	FUNCDEF_GET_PRTIAL_HUE_COLOR *GetPartialHueColor;
};
//----------------------------------------------------------------------------
UOInterface IPathFinder
{
	FUNCDEF_GET_CAN_WALK *CanWalk;
	FUNCDEF_GET_WALK *Walk;
	FUNCDEF_GET_WALK_TO *WalkTo;
	FUNCDEF_GET_STOP_AUTOWALK *StopAutowalk;
};
//---------------------------------------------------------------------------
extern IGLEngine g_Interface_GL;
extern IUltimaOnline g_Interface_UO;
extern IClilocManager g_Interface_ClilocManager;
extern IColorManager g_Interface_ColorManager;
extern IPathFinder g_Interface_PathFinder;
//---------------------------------------------------------------------------
typedef struct PLUGIN_CLIENT_INTERFACE
{
	IGLEngine *GL;
	IUltimaOnline *UO;
	IClilocManager *ClilocManager;
	IColorManager *ColorManager;
	IPathFinder *PathFinder;
} *PPLUGIN_CLIENT_INTERFACE;
//----------------------------------------------------------------------------
extern PLUGIN_CLIENT_INTERFACE PluginClientInterface;
//----------------------------------------------------------------------------
#endif