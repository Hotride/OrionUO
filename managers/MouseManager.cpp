/****************************************************************************
**
** MouseManager.cpp
**
** Copyright (C) September 2015 Hotride
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

TMouseManager MouseManager;
//---------------------------------------------------------------------------
WORD g_CursorData[2][16] =
{
	{
		0x206A, 0x206B, 0x206C, 0x206D, 0x206E, 0x206F, 0x2070, 0x2071,
		0x2072, 0x2073, 0x2074, 0x2075, 0x2076, 0x2077, 0x2078, 0x2079
	},
	{
		0x2053, 0x2054, 0x2055, 0x2056, 0x2057, 0x2058, 0x2059, 0x205A,
		0x205B, 0x205C, 0x205D, 0x205E, 0x205F, 0x2060, 0x2061, 0x2062
	}
};
//---------------------------------------------------------------------------
void TMouseManager::UpdateMouse()
{
	POINT ptMouse;
	GetCursorPos(&ptMouse);

	ScreenToClient(g_hWnd, &ptMouse);

	g_MouseX = ptMouse.x;
	g_MouseY = ptMouse.y;
	
	/*if (g_MouseX > GetSystemMetrics(SM_CXSIZEFRAME) && g_MouseX < g_ClientWidth - GetSystemMetrics(SM_CXSIZEFRAME) &&
		g_MouseY > (GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME)) && g_MouseY < g_ClientHeight - GetSystemMetrics(SM_CYFRAME))*/
	/*if (g_MouseX > 0 && g_MouseX < g_ClientWidth &&
		g_MouseY > 0 && g_MouseY < g_ClientHeight)
	{
		ShowCursor(GetFocus() != g_hWnd);
	}
	else
		ShowCursor(false);*/
}
//---------------------------------------------------------------------------
int TMouseManager::Sgn(int val)
{
	return (0 < val) - (val < 0);
}
//---------------------------------------------------------------------------
int TMouseManager::GetFacing(int x1, int  y1, int  to_x, int to_y, int current_facing)
{
	int shiftX = to_x - x1;
	int shiftY = to_y - y1;

	int hashf = 100 * (Sgn(shiftX) + 2) + 10 * (Sgn(shiftY) + 2);

	if ((shiftX != 0) && (shiftY != 0))
	{
        shiftX = std::abs(shiftX);
        shiftY = std::abs(shiftY);

        if ((shiftY * 5) <= (shiftX * 2))
            hashf = hashf + 1;
        else if ((shiftY * 2) >= (shiftX * 5))
            hashf = hashf + 3;
        else
            hashf = hashf + 2;

	}
	else if (!shiftX)
	{
		if (!shiftY)
			return current_facing;
	}

	switch (hashf)
	{
		case 111:
			return DT_W; // W
		case 112:
			return DT_NW; // NW
		case 113:
			return DT_N; // N
		case 120:
			return DT_W; // W
		case 131:
			return DT_W; // W
		case 132:
			return DT_SW; // SW
		case 133:
			return DT_S; // S
		case 210:
			return DT_N; // N
		case 230:
			return DT_S; // S
		case 311:
			return DT_E; // E
		case 312:
			return DT_NE; // NE
		case 313:
			return DT_N; // N
		case 320:
			return DT_E; // E
		case 331:
			return DT_E; // E
		case 332:
			return DT_SE; // SE
		case 333:
			return DT_S; // S
		default:
			break;
	}

	return current_facing;
}
//---------------------------------------------------------------------------
WORD TMouseManager::GetGameCursor()
{
	int war = (int)(g_Player != NULL && g_Player->Warmode);
	WORD result = g_CursorData[war][9]; //Main Gump mouse cursor
	
	int GameWindowCenterX = g_GameWindowPosX + (g_GameWindowSizeX / 2);
	int GameWindowCenterY = g_GameWindowPosY + (g_GameWindowSizeY / 2);

	int facing = GetFacing(GameWindowCenterX, GameWindowCenterY, g_MouseX, g_MouseY, 1);
	
	bool mouseInWindow = true;

	if (g_MouseX < g_GameWindowPosX || g_MouseY < g_GameWindowPosY || g_MouseX > (g_GameWindowPosX + g_GameWindowSizeX) ||
		g_MouseY > (g_GameWindowPosY + g_GameWindowSizeY))
		mouseInWindow = false;

	if ((g_MovingFromMouse || (mouseInWindow && g_AutoMoving)) && !g_LastGumpRightMouseDown)
	{
		float mouse_range = std::hypotf((float)(GameWindowCenterX - g_MouseX), (float)(GameWindowCenterY - g_MouseY));
		//UO->DrawUnicodeFontFixed(1, std::to_wstring(mouse_range).c_str(), 773, 100, 100, 200);

		int dir = facing;

		if (!dir)
			dir = 8;

		bool run = ((mouse_range >= 150.0f) ? true : false);

		PathFinder->Walk(run, dir - 1);
	}
	
	bool gumpChecked = ((g_LastSelectedObject && g_LastObjectType != SOT_GAME_OBJECT && g_LastObjectType != SOT_STATIC_OBJECT && g_LastObjectType != SOT_LAND_OBJECT && g_LastObjectType != SOT_TEXT_OBJECT) || g_LastSelectedGump);

	if (!mouseInWindow || gumpChecked || (g_LeftMouseDown && g_LastGumpLeftMouseDown))
	{
		if (Target.IsTargeting())
			return g_CursorData[war][12]; //Targetting cursor

		return result;
	}

	if (Target.IsTargeting())
		return g_CursorData[war][12]; //Targetting cursor
	
	if (gumpChecked)
		return result;

	return g_CursorData[war][facing];
}
//---------------------------------------------------------------------------
bool TMouseManager::LoadCursorTextures()
{
	bool result = true;

	IFOR(i, 0, 2)
	{
		IFOR(j, 0, 16)
		{
			WORD ID = g_CursorData[i][j];

			TTextureObject *pth = UO->ExecuteStaticArt(ID);

			if (!i)
			{
				if (pth != NULL)
				{
					float OffsX = 0.0f;
					float OffsY = 0.0f;

					float DW = (float)pth->Width;
					float DH = (float)pth->Height;

					if (ID == 0x206A)
					{
						OffsX = -4.0f;
					}
					else if (ID == 0x206B)
					{
						OffsX = -DW;
					}
					else if (ID == 0x206C)
					{
						OffsX = -DW;
						OffsY = -(DH / 2.0f);
					}
					else if (ID == 0x206D)
					{
						OffsX = -DW;
						OffsY = -DH;
					}
					else if (ID == 0x206E)
					{
						OffsX = -(DW * 0.66f);
						OffsY = -DH;
					}
					else if (ID == 0x206F)
					{
						OffsY = ((-DH) + 4.0f);
					}
					else if (ID == 0x2070)
					{
						OffsY = ((-DH) + 4.0f);
					}
					else if (ID == 0x2075)
					{
						OffsY = -4.0f;
					}
					else if (ID == 0x2076)
					{
						OffsX = -12.0f;
						OffsY = -14.0f;
					}
					else if (ID == 0x2077)
					{
						OffsX = -(DW / 2.0f);
						OffsY = -(DH / 2.0f);
					}
					else if (ID == 0x2078)
					{
						OffsY = -(DH * 0.66f);
					}
					else if (ID == 0x2079)
					{
						OffsY = -(DH / 2.0f);
					}

					m_CursorOffset[0][j] = OffsX; //X
					m_CursorOffset[1][j] = OffsY; //Y
				}
				else
				{
					m_CursorOffset[0][j] = 0.0f; //X
					m_CursorOffset[1][j] = 0.0f; //Y
				}
			}
		}
	}
	return result;
}
//---------------------------------------------------------------------------
void TMouseManager::Draw(WORD id)
{
	TTextureObject *th = UO->ExecuteStaticArt(id);
	if (th != NULL)
	{
		if (id < 0x206A)
			id -= 0x2053;
		else
			id -= 0x206A;

		if (id < 16)
		{
			Tooltip.Draw(th->Width, th->Height);

			g_GL.Draw(th->Texture, (float)g_MouseX + m_CursorOffset[0][id], (float)g_MouseY + m_CursorOffset[1][id], (float)th->Width, (float)th->Height);
		}
	}
}
//---------------------------------------------------------------------------