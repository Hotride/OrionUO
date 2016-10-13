/***********************************************************************************
**
** CommonInterfaces.cpp
**
** Copyright (C) September 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "CommonInterfaces.h"
#include "GLEngine/GLEngine.h"
#include "OrionUO.h"
#include "Managers/ClilocManager.h"
#include "Managers/FontsManager.h"
#include "Managers/ColorManager.h"
#include "Walker/PathFinder.h"
#include "Game objects/GamePlayer.h"
//----------------------------------------------------------------------------------
//IGLEngine
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_PushScissor(int x, int y, int width, int height)
{
	g_GL.PushScissor(x, y, width, height);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_PopScissor()
{
	g_GL.PopScissor();
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawLine(unsigned int color, int x, int y, int width, int height)
{
	glColor4ub(GetRValue(color), GetGValue(color), GetBValue(color), (color >> 24) & 0xFF);
	g_GL.DrawLine(x, y, width, height);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawPolygone(unsigned int color, int x, int y, int width, int height)
{
	glColor4ub(GetRValue(color), GetGValue(color), GetBValue(color), (color >> 24) & 0xFF);
	g_GL.DrawPolygone(x, y, width, height);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawCircle(unsigned int color, float x, float y, float radius, int gradientMode)
{
	glColor4ub(GetRValue(color), GetGValue(color), GetBValue(color), (color >> 24) & 0xFF);
	g_GL.DrawCircle(x, y, radius, gradientMode);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawTextA(int x, int y, unsigned char font, const char *text, unsigned short color, int width, TEXT_ALIGN_TYPE align, unsigned short flags)
{
	g_FontManager.DrawA(font, text, color, x, y, width, align, flags);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawTextW(int x, int y, unsigned char font, const wchar_t *text, unsigned short color, int width, TEXT_ALIGN_TYPE align, unsigned short flags)
{
	g_FontManager.DrawW(font, text, color, x, y, 30, width, align, flags);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawArt(int x, int y, unsigned short graphic, unsigned short color)
{
	g_Orion.DrawStaticArt(graphic, color, x, y, 0);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawArtAnimated(int x, int y, unsigned short graphic, unsigned short color)
{
	g_Orion.DrawStaticArtAnimated(graphic, color, x, y, 0);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawResizepicGump(int x, int y, unsigned short graphic, int width, int height)
{
	g_Orion.DrawResizepicGump(graphic, x, y, width, height);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawGump(int x, int y, unsigned short graphic, unsigned short color)
{
	g_Orion.DrawGump(graphic, color, x, y);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawGumppic(int x, int y, unsigned short graphic, unsigned short color, int width, int height)
{
	g_Orion.DrawGump(graphic, color, x, y, width, height);
}
//----------------------------------------------------------------------------------
//IUltimaOnline
//----------------------------------------------------------------------------------
unsigned __int64 __cdecl FUNCBODY_GetLandFlags(unsigned short graphic)
{
	return g_Orion.GetLandFlags(graphic);
}
//----------------------------------------------------------------------------------
unsigned __int64 __cdecl FUNCBODY_GetStaticFlags(unsigned short graphic)
{
	return g_Orion.GetStaticFlags(graphic);
}
//----------------------------------------------------------------------------------
int __cdecl FUNCBODY_GetValueInt(CONFIG_VALUE_KEY key)
{
	return g_Orion.GetValueInt(key);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_SetValueInt(CONFIG_VALUE_KEY key, int value)
{
	g_Orion.GetValueInt(key, value);
}
//----------------------------------------------------------------------------------
std::string __cdecl FUNCBODY_GetValueString(CONFIG_VALUE_KEY key)
{
	return g_Orion.GetValueString(key);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_SetValueString(CONFIG_VALUE_KEY key, std::string value)
{
	g_Orion.GetValueString(key, value);
}
//----------------------------------------------------------------------------------
//IClilocManager
//----------------------------------------------------------------------------------
std::string __cdecl FUNCBODY_GetClilocA(unsigned int clilocID, std::string defaultText)
{
	return g_ClilocManager.Cliloc(g_Language)->GetA(clilocID, defaultText);
}
//----------------------------------------------------------------------------------
std::wstring __cdecl FUNCBODY_GetClilocW(unsigned int clilocID, std::string defaultText)
{
	return g_ClilocManager.Cliloc(g_Language)->GetW(clilocID, defaultText);
}
//----------------------------------------------------------------------------------
std::wstring __cdecl FUNCBODY_GetClilocArguments(unsigned int clilocID, std::wstring args)
{
	return g_ClilocManager.ParseArgumentsToClilocString(clilocID, args);
}
//----------------------------------------------------------------------------------
//IColorManager
//----------------------------------------------------------------------------------
int __cdecl FUNCBODY_GetHuesCount()
{
	return g_ColorManager.GetHuesCount();
}
//----------------------------------------------------------------------------------
unsigned short __cdecl FUNCBODY_GetColor32To16(unsigned int &color)
{
	return g_ColorManager.Color32To16(color);
}
//----------------------------------------------------------------------------------
unsigned int __cdecl FUNCBODY_GetColor16To32(unsigned short &color)
{
	return g_ColorManager.Color16To32(color);
}
//----------------------------------------------------------------------------------
unsigned short __cdecl FUNCBODY_GetColorToGray(unsigned short &color)
{
	return g_ColorManager.ConvertToGray(color);
}
//----------------------------------------------------------------------------------
unsigned int __cdecl FUNCBODY_GetPolygoneColor(unsigned short c, unsigned short color)
{
	return g_ColorManager.GetPolygoneColor(c, color);
}
//----------------------------------------------------------------------------------
unsigned int __cdecl FUNCBODY_GetColor(unsigned short &c, unsigned short color)
{
	return g_ColorManager.GetColor(c, color);
}
//----------------------------------------------------------------------------------
unsigned int __cdecl FUNCBODY_GetHartialHueColor(unsigned short &c, unsigned short color)
{
	return g_ColorManager.GetPartialHueColor(c, color);
}
//----------------------------------------------------------------------------------
//IPathFinder
//----------------------------------------------------------------------------------
bool __cdecl FUNCBODY_GetCanWalk(unsigned char &direction, int &x, int &y, char &z)
{
	return g_PathFinder.CanWalk(direction, x, y, z);
}
//----------------------------------------------------------------------------------
bool __cdecl FUNCBODY_GetWalk(bool run, unsigned char direction)
{
	return g_PathFinder.Walk(run, direction);
}
//----------------------------------------------------------------------------------
bool __cdecl FUNCBODY_GetWalkTo(int x, int y, int z, int distance)
{
	bool result = g_PathFinder.WalkTo(x, y, z, distance);

	if (result)
	{
		while (g_PathFinder.AutoWalking)
			Sleep(100);

		WISP_GEOMETRY::CPoint2Di p(g_Player->X, g_Player->Y);

		CWalkData *wd = g_Player->m_WalkStack.Top();

		if (wd != NULL)
		{
			p.X = wd->X;
			p.Y = wd->Y;
		}

		result = (GetDistance(p, WISP_GEOMETRY::CPoint2Di(x, y)) <= distance);
	}

	return result;
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_GetStopAutowalk()
{
	if (g_PathFinder.PathFindidngCanBeCancelled)
		g_PathFinder.StopAutoWalk();
}
//----------------------------------------------------------------------------------
bool __cdecl FUNCBODY_GetAutowalking()
{
	return g_PathFinder.AutoWalking;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
IGLEngine g_Interface_GL =
{
	FUNCBODY_PushScissor,
	FUNCBODY_PopScissor,
	FUNCBODY_DrawLine,
	FUNCBODY_DrawPolygone,
	FUNCBODY_DrawCircle,
	FUNCBODY_DrawTextA,
	FUNCBODY_DrawTextW,
	FUNCBODY_DrawArt,
	FUNCBODY_DrawArtAnimated,
	FUNCBODY_DrawResizepicGump,
	FUNCBODY_DrawGump,
	FUNCBODY_DrawGumppic
};
//----------------------------------------------------------------------------------
IUltimaOnline g_Interface_UO =
{
	FUNCBODY_GetLandFlags,
	FUNCBODY_GetStaticFlags,
	FUNCBODY_GetValueInt,
	FUNCBODY_SetValueInt,
	FUNCBODY_GetValueString,
	FUNCBODY_SetValueString
};
//----------------------------------------------------------------------------------
IClilocManager g_Interface_ClilocManager =
{
	FUNCBODY_GetClilocA,
	FUNCBODY_GetClilocW,
	FUNCBODY_GetClilocArguments
};
//----------------------------------------------------------------------------------
IColorManager g_Interface_ColorManager =
{
	FUNCBODY_GetHuesCount,
	FUNCBODY_GetColor32To16,
	FUNCBODY_GetColor16To32,
	FUNCBODY_GetColorToGray,
	FUNCBODY_GetPolygoneColor,
	FUNCBODY_GetColor,
	FUNCBODY_GetHartialHueColor
};
//----------------------------------------------------------------------------------
IPathFinder g_Interface_PathFinder =
{
	FUNCBODY_GetCanWalk,
	FUNCBODY_GetWalk,
	FUNCBODY_GetWalkTo,
	FUNCBODY_GetStopAutowalk,
	FUNCBODY_GetAutowalking
};
//----------------------------------------------------------------------------------
