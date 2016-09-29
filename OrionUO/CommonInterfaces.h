/***********************************************************************************
**
** CommonInterfaces.h
**
** Copyright (C) September 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef COMMONINTERFACES_H
#define COMMONINTERFACES_H
//----------------------------------------------------------------------------------
#include "EnumList.h"
#include "Wisp/WispDefinitions.h"
#include <string>
//----------------------------------------------------------------------------------
#ifndef UOInterface
#define UOInterface struct
#endif
//----------------------------------------------------------------------------------
//IGLEngine
typedef void __cdecl FUNCDEF_PUSH_SCISSOR(int, int, int, int);
typedef void __cdecl FUNCDEF_POP_SCISSOR();
typedef void __cdecl FUNCDEF_DRAW_LINE(uint, int, int, int, int);
typedef void __cdecl FUNCDEF_DRAW_POLYGONE(uint, int, int, int, int);
typedef void __cdecl FUNCDEF_DRAW_CIRCLE(uint, float, float, float, int);
typedef void __cdecl FUNCDEF_DRAW_TEXT_A(int, int, uchar, const char*, ushort, int, TEXT_ALIGN_TYPE, ushort);
typedef void __cdecl FUNCDEF_DRAW_TEXT_W(int, int, uchar, const wchar_t*, ushort, int, TEXT_ALIGN_TYPE, ushort);
typedef void __cdecl FUNCDEF_DRAW_ART(int, int, ushort, ushort);
typedef void __cdecl FUNCDEF_DRAW_ART_ANIMATED(int, int, ushort, ushort);
typedef void __cdecl FUNCDEF_DRAW_RESIZEPIC_GUMP(int, int, ushort, int, int);
typedef void __cdecl FUNCDEF_DRAW_GUMP(int, int, ushort, ushort);
typedef void __cdecl FUNCDEF_DRAW_GUMPPIC(int, int, ushort, ushort, int, int);

//IUltimaOnline
typedef uint __cdecl FUNCDEF_GET_LAND_FLAGS(ushort);
typedef uint __cdecl FUNCDEF_GET_STATIC_FLAGS(ushort);
typedef int __cdecl FUNCDEF_GET_CONFIG_VALUE(const char*);
typedef void __cdecl FUNCDEF_SET_CONFIG_VALUE(const char*, int);

//IClilocManager
typedef std::string __cdecl FUNCDEF_GET_CLILOC_A(uint, std::string);
typedef std::wstring __cdecl FUNCDEF_GET_CLILOC_W(uint, std::string);
typedef std::wstring __cdecl FUNCDEF_GET_CLILOC_ARGUMENTS(uint, std::wstring);

//IColorManager
typedef int __cdecl FUNCDEF_GET_HUES_COUNT();
typedef ushort __cdecl FUNCDEF_GET_COLOR32TO16(uint&);
typedef uint __cdecl FUNCDEF_GET_COLOR16TO32(ushort&);
typedef ushort __cdecl FUNCDEF_GET_COLOR_TO_GRAY(ushort&);
typedef uint __cdecl FUNCDEF_GET_POLYGONE_COLOR(ushort, ushort);
typedef uint __cdecl FUNCDEF_GET_COLOR(ushort&, ushort);
typedef uint __cdecl FUNCDEF_GET_PRTIAL_HUE_COLOR(ushort&, ushort);

//IPathFinder
typedef bool __cdecl FUNCDEF_GET_CAN_WALK(uchar&, int&, int&, char&);
typedef bool __cdecl FUNCDEF_GET_WALK(bool, uchar);
typedef bool __cdecl FUNCDEF_GET_WALK_TO(int, int, int, int);
typedef void __cdecl FUNCDEF_GET_STOP_AUTOWALK();
typedef bool __cdecl FUNCDEF_GET_AUTOWALKING();
//----------------------------------------------------------------------------------
UOInterface IGLEngine
{
	FUNCDEF_PUSH_SCISSOR *PushScissor;
	FUNCDEF_POP_SCISSOR *PopScissor;
	FUNCDEF_DRAW_LINE *DrawLine;
	FUNCDEF_DRAW_POLYGONE *DrawPolygone;
	FUNCDEF_DRAW_CIRCLE *DrawCircle;
	FUNCDEF_DRAW_TEXT_A *DrawTextA;
	FUNCDEF_DRAW_TEXT_W *DrawTextW;
	FUNCDEF_DRAW_ART *DrawArt;
	FUNCDEF_DRAW_ART_ANIMATED *DrawArtAnimated;
	FUNCDEF_DRAW_RESIZEPIC_GUMP *DrawResizepicGump;
	FUNCDEF_DRAW_GUMP *DrawGump;
	FUNCDEF_DRAW_GUMPPIC *DrawGumppic;
};
//----------------------------------------------------------------------------------
UOInterface IUltimaOnline
{
	FUNCDEF_GET_LAND_FLAGS *GetLandFlags;
	FUNCDEF_GET_STATIC_FLAGS *GetStaticFlags;
	FUNCDEF_GET_CONFIG_VALUE *GetConfigValue;
	FUNCDEF_SET_CONFIG_VALUE *SetConfigValue;
};
//----------------------------------------------------------------------------------
UOInterface IClilocManager
{
	FUNCDEF_GET_CLILOC_A *GetClilocA;
	FUNCDEF_GET_CLILOC_W *GetClilocW;

	FUNCDEF_GET_CLILOC_ARGUMENTS *GetClilocArguments;
};
//----------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------
UOInterface IPathFinder
{
	FUNCDEF_GET_CAN_WALK *CanWalk;
	FUNCDEF_GET_WALK *Walk;
	FUNCDEF_GET_WALK_TO *WalkTo;
	FUNCDEF_GET_STOP_AUTOWALK *StopAutowalk;
	FUNCDEF_GET_AUTOWALKING *GetAutowalking;
};
//----------------------------------------------------------------------------------
extern IGLEngine g_Interface_GL;
extern IUltimaOnline g_Interface_UO;
extern IClilocManager g_Interface_ClilocManager;
extern IColorManager g_Interface_ColorManager;
extern IPathFinder g_Interface_PathFinder;
//----------------------------------------------------------------------------------
typedef struct PLUGIN_CLIENT_INTERFACE
{
	IGLEngine *GL;
	IUltimaOnline *UO;
	IClilocManager *ClilocManager;
	IColorManager *ColorManager;
	IPathFinder *PathFinder;
} *PPLUGIN_CLIENT_INTERFACE;
//----------------------------------------------------------------------------------
extern PLUGIN_CLIENT_INTERFACE g_PluginClientInterface;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
