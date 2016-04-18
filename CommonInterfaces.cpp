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
	glColor4b(GetRValue(color), GetGValue(color), GetBValue(color), (color >> 24) & 0xFF);
	g_GL.DrawLine(x, y, width, height);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
//---------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawPolygone(DWORD color, int x, int y, int width, int height)
{
	glColor4b(GetRValue(color), GetGValue(color), GetBValue(color), (color >> 24) & 0xFF);
	g_GL.DrawPolygone(x, y, width, height);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
//---------------------------------------------------------------------------
void __cdecl FUNCBODY_DrawCircle(DWORD color, float x, float y, float radius, int gradientMode)
{
	glColor4b(GetRValue(color), GetGValue(color), GetBValue(color), (color >> 24) & 0xFF);
	g_GL.DrawCircle(x, y, radius, gradientMode);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
//---------------------------------------------------------------------------
//IUltimaOnline
//---------------------------------------------------------------------------
DWORD __cdecl FUNCBODY_GetLandFlags(WORD graphic)
{
	return UO->GetLandFlags(graphic);
}
//---------------------------------------------------------------------------
DWORD __cdecl FUNCBODY_GetStaticFlags(WORD graphic)
{
	return UO->GetStaticFlags(graphic);
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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
IGLEngine g_Interface_GL =
{
	FUNCBODY_Scissor,
	FUNCBODY_DrawLine,
	FUNCBODY_DrawPolygone,
	FUNCBODY_DrawCircle
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
	FUNCBODY_GetWalk
};
//---------------------------------------------------------------------------