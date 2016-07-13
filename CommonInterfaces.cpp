/****************************************************************************
**
** CommonInterfaces.cpp
**
** Copyright (C) February 2016 Hotride
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
//---------------------------------------------------------------------------
//IGLEngine
//---------------------------------------------------------------------------
void __cdecl FUNCBODY_Scissor(int x, int y, int width, int height)
{
	g_GL.Scissor(x, y, width, height);
}
//---------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawLine(DWORD color, int x, int y, int width, int height)
{
	glColor4ub(GetRValue(color), GetGValue(color), GetBValue(color), (color >> 24) & 0xFF);
	g_GL.DrawLine(x, y, width, height);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
//---------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawPolygone(DWORD color, int x, int y, int width, int height)
{
	glColor4ub(GetRValue(color), GetGValue(color), GetBValue(color), (color >> 24) & 0xFF);
	g_GL.DrawPolygone(x, y, width, height);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
//---------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawCircle(DWORD color, float x, float y, float radius, int gradientMode)
{
	glColor4ub(GetRValue(color), GetGValue(color), GetBValue(color), (color >> 24) & 0xFF);
	g_GL.DrawCircle(x, y, radius, gradientMode);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
//---------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawTextA(int x, int y, BYTE font, const char *text, WORD color, int width, TEXT_ALIGN_TYPE align, WORD flags)
{
	TTextTexture th;
	FontManager->GenerateA(font, th, text, color, width, align, flags);
	th.Draw(x, y);
	th.Clear();
}
//---------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawTextW(int x, int y, BYTE font, const wchar_t *text, WORD color, int width, TEXT_ALIGN_TYPE align, WORD flags)
{
	TTextTexture th;
	FontManager->GenerateW(font, th, text, color, 30, width, align, flags);
	th.Draw(x, y);
	th.Clear();
}
//---------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawArt(int x, int y, WORD graphic, WORD color)
{
	Orion->DrawStaticArt(graphic, color, x, y, 0);
}
//---------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawArtAnimated(int x, int y, WORD graphic, WORD color)
{
	Orion->DrawStaticArtAnimated(graphic, color, x, y, 0);
}
//---------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawResizepicGump(int x, int y, WORD graphic, int width, int height)
{
	Orion->DrawResizepicGump(graphic, x, y, width, height);
}
//---------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawGump(int x, int y, WORD graphic, WORD color)
{
	Orion->DrawGump(graphic, color, x, y);
}
//---------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawGumppic(int x, int y, WORD graphic, WORD color, int width, int height)
{
	Orion->DrawGump(graphic, color, x, y, width, height);
}
//---------------------------------------------------------------------------
//IUltimaOnline
//---------------------------------------------------------------------------
DWORD __cdecl FUNCBODY_GetLandFlags(WORD graphic)
{
	return Orion->GetLandFlags(graphic);
}
//---------------------------------------------------------------------------
DWORD __cdecl FUNCBODY_GetStaticFlags(WORD graphic)
{
	return Orion->GetStaticFlags(graphic);
}
//---------------------------------------------------------------------------
int __cdecl FUNCBODY_GetConfigValue(const char *option)
{
	return Orion->GetConfigValue(option);
}
//---------------------------------------------------------------------------
void __cdecl FUNCBODY_SetConfigValue(const char *option, int value)
{
	Orion->GetConfigValue(option, value);
}
//---------------------------------------------------------------------------
//IClilocManager
//---------------------------------------------------------------------------
string __cdecl FUNCBODY_GetClilocA(DWORD clilocID, string defaultText)
{
	return ClilocManager->Cliloc(g_Language)->GetA(clilocID, defaultText);
}
//---------------------------------------------------------------------------
wstring __cdecl FUNCBODY_GetClilocW(DWORD clilocID, string defaultText)
{
	return ClilocManager->Cliloc(g_Language)->GetW(clilocID, defaultText);
}
//---------------------------------------------------------------------------
wstring __cdecl FUNCBODY_GetClilocArguments(DWORD clilocID, wstring args)
{
	return ClilocManager->ParseArgumentsToClilocString(clilocID, args);
}
//---------------------------------------------------------------------------
//IColorManager
//---------------------------------------------------------------------------
int __cdecl FUNCBODY_GetHuesCount()
{
	return ColorManager->GetHuesCount();
}
//---------------------------------------------------------------------------
WORD __cdecl FUNCBODY_GetColor32To16(DWORD &color)
{
	return ColorManager->Color32To16(color);
}
//---------------------------------------------------------------------------
DWORD __cdecl FUNCBODY_GetColor16To32(WORD &color)
{
	return ColorManager->Color16To32(color);
}
//---------------------------------------------------------------------------
WORD __cdecl FUNCBODY_GetColorToGray(WORD &color)
{
	return ColorManager->ConvertToGray(color);
}
//---------------------------------------------------------------------------
DWORD __cdecl FUNCBODY_GetPolygoneColor(WORD c, WORD color)
{
	return ColorManager->GetPolygoneColor(c, color);
}
//---------------------------------------------------------------------------
DWORD __cdecl FUNCBODY_GetColor(WORD &c, WORD color)
{
	return ColorManager->GetColor(c, color);
}
//---------------------------------------------------------------------------
DWORD __cdecl FUNCBODY_GetHartialHueColor(WORD &c, WORD color)
{
	return ColorManager->GetPartialHueColor(c, color);
}
//---------------------------------------------------------------------------
//IPathFinder
//---------------------------------------------------------------------------
bool __cdecl FUNCBODY_GetCanWalk(BYTE &direction, int &x, int &y, char &z)
{
	return PathFinder->CanWalk(direction, x, y, z);
}
//---------------------------------------------------------------------------
bool __cdecl FUNCBODY_GetWalk(bool run, BYTE direction)
{
	return PathFinder->Walk(run, direction);
}
//---------------------------------------------------------------------------
bool __cdecl FUNCBODY_GetWalkTo(int x, int y, int z, int distance)
{
	bool result = PathFinder->WalkTo(x, y, z, distance);

	if (result)
	{
		while (PathFinder->AutoWalking)
			Sleep(100);

		POINT p = { g_Player->X, g_Player->Y };

		TWalkData *wd = g_Player->m_WalkStack.Top();

		if (wd != NULL)
		{
			p.x = wd->X;
			p.y = wd->Y;
		}

		POINT startPoint = { x, y };

		result = (GetDistance(p, startPoint) <= distance);
	}

	return result;
}
//---------------------------------------------------------------------------
void __cdecl FUNCBODY_GetStopAutowalk()
{
	if (PathFinder->PathFindidngCanBeCancelled)
		PathFinder->StopAutoWalk();
}
//---------------------------------------------------------------------------
bool __cdecl FUNCBODY_GetAutowalking()
{
	return PathFinder->AutoWalking;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
IGLEngine g_Interface_GL =
{
	FUNCBODY_Scissor,
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
//---------------------------------------------------------------------------
IUltimaOnline g_Interface_UO =
{
	FUNCBODY_GetLandFlags,
	FUNCBODY_GetStaticFlags
};
//---------------------------------------------------------------------------
IClilocManager g_Interface_ClilocManager =
{
	FUNCBODY_GetClilocA,
	FUNCBODY_GetClilocW,
	FUNCBODY_GetClilocArguments
};
//---------------------------------------------------------------------------
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
//---------------------------------------------------------------------------
IPathFinder g_Interface_PathFinder =
{
	FUNCBODY_GetCanWalk,
	FUNCBODY_GetWalk,
	FUNCBODY_GetWalkTo,
	FUNCBODY_GetStopAutowalk,
	FUNCBODY_GetAutowalking
};
//---------------------------------------------------------------------------