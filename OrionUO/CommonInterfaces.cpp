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
void __cdecl FUNCBODY_DrawLine(uint color, int x, int y, int width, int height)
{
	glColor4ub(GetRValue(color), GetGValue(color), GetBValue(color), (color >> 24) & 0xFF);
	g_GL.DrawLine(x, y, width, height);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawPolygone(uint color, int x, int y, int width, int height)
{
	glColor4ub(GetRValue(color), GetGValue(color), GetBValue(color), (color >> 24) & 0xFF);
	g_GL.DrawPolygone(x, y, width, height);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawCircle(uint color, float x, float y, float radius, int gradientMode)
{
	glColor4ub(GetRValue(color), GetGValue(color), GetBValue(color), (color >> 24) & 0xFF);
	g_GL.DrawCircle(x, y, radius, gradientMode);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawTextA(int x, int y, uchar font, const char *text, ushort color, int width, TEXT_ALIGN_TYPE align, ushort flags)
{
	g_FontManager.DrawA(font, text, color, x, y, width, align, flags);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawTextW(int x, int y, uchar font, const wchar_t *text, ushort color, int width, TEXT_ALIGN_TYPE align, ushort flags)
{
	g_FontManager.DrawW(font, text, color, x, y, 30, width, align, flags);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawArt(int x, int y, ushort graphic, ushort color)
{
	g_Orion.DrawStaticArt(graphic, color, x, y, 0);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawArtAnimated(int x, int y, ushort graphic, ushort color)
{
	g_Orion.DrawStaticArtAnimated(graphic, color, x, y, 0);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawResizepicGump(int x, int y, ushort graphic, int width, int height)
{
	g_Orion.DrawResizepicGump(graphic, x, y, width, height);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawGump(int x, int y, ushort graphic, ushort color)
{
	g_Orion.DrawGump(graphic, color, x, y);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawGumppic(int x, int y, ushort graphic, ushort color, int width, int height)
{
	g_Orion.DrawGump(graphic, color, x, y, width, height);
}
//----------------------------------------------------------------------------------
//IUltimaOnline
//----------------------------------------------------------------------------------
uint __cdecl FUNCBODY_GetLandFlags(ushort graphic)
{
	return g_Orion.GetLandFlags(graphic);
}
//----------------------------------------------------------------------------------
uint __cdecl FUNCBODY_GetStaticFlags(ushort graphic)
{
	return g_Orion.GetStaticFlags(graphic);
}
//----------------------------------------------------------------------------------
int __cdecl FUNCBODY_GetConfigValue(const char *option)
{
	return g_Orion.GetConfigValue(option);
}
//----------------------------------------------------------------------------------
void __cdecl FUNCBODY_SetConfigValue(const char *option, int value)
{
	g_Orion.GetConfigValue(option, value);
}
//----------------------------------------------------------------------------------
//IClilocManager
//----------------------------------------------------------------------------------
std::string __cdecl FUNCBODY_GetClilocA(uint clilocID, std::string defaultText)
{
	return g_ClilocManager.Cliloc(g_Language)->GetA(clilocID, defaultText);
}
//----------------------------------------------------------------------------------
std::wstring __cdecl FUNCBODY_GetClilocW(uint clilocID, std::string defaultText)
{
	return g_ClilocManager.Cliloc(g_Language)->GetW(clilocID, defaultText);
}
//----------------------------------------------------------------------------------
std::wstring __cdecl FUNCBODY_GetClilocArguments(uint clilocID, std::wstring args)
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
ushort __cdecl FUNCBODY_GetColor32To16(uint &color)
{
	return g_ColorManager.Color32To16(color);
}
//----------------------------------------------------------------------------------
uint __cdecl FUNCBODY_GetColor16To32(ushort &color)
{
	return g_ColorManager.Color16To32(color);
}
//----------------------------------------------------------------------------------
ushort __cdecl FUNCBODY_GetColorToGray(ushort &color)
{
	return g_ColorManager.ConvertToGray(color);
}
//----------------------------------------------------------------------------------
uint __cdecl FUNCBODY_GetPolygoneColor(ushort c, ushort color)
{
	return g_ColorManager.GetPolygoneColor(c, color);
}
//----------------------------------------------------------------------------------
uint __cdecl FUNCBODY_GetColor(ushort &c, ushort color)
{
	return g_ColorManager.GetColor(c, color);
}
//----------------------------------------------------------------------------------
uint __cdecl FUNCBODY_GetHartialHueColor(ushort &c, ushort color)
{
	return g_ColorManager.GetPartialHueColor(c, color);
}
//----------------------------------------------------------------------------------
//IPathFinder
//----------------------------------------------------------------------------------
bool __cdecl FUNCBODY_GetCanWalk(uchar &direction, int &x, int &y, char &z)
{
	return g_PathFinder.CanWalk(direction, x, y, z);
}
//----------------------------------------------------------------------------------
bool __cdecl FUNCBODY_GetWalk(bool run, uchar direction)
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
	FUNCBODY_GetStaticFlags
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
