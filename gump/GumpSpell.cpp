/****************************************************************************
**
** GumpSpell.cpp
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
//---------------------------------------------------------------------------
TGumpSpell::TGumpSpell(DWORD serial, short x, short y, WORD graphic)
: TGump(GT_SPELL, serial, x, y)
{
	m_Graphic = graphic;
}
//---------------------------------------------------------------------------
TGumpSpell::~TGumpSpell()
{
}
//---------------------------------------------------------------------------
void TGumpSpell::PrepareTextures()
{
	UO->ExecuteGump(m_Graphic);
}
//---------------------------------------------------------------------------
int TGumpSpell::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	int x = m_X;
	int y = m_Y;

	if (CanBeMoved() && g_LeftMouseDown && g_LastGumpLeftMouseDown == index && !g_LastObjectLeftMouseDown)
	{
		x += g_MouseX - g_DroppedLeftMouseX;
		y += g_MouseY - g_DroppedLeftMouseY;
	}

	if (mode)
	{
		UO->DrawGump(m_Graphic, 0, x, y);

		DrawLocker(x, y);
	}
	else
	{
		DWORD LSG = 0;

		if (UO->GumpPixelsInXY(m_Graphic, x, y))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
			LSG = index;
		}

		if (g_ShowGumpLocker && UO->PolygonePixelsInXY(x, y, 10, 14))
		{
			g_LastSelectedObject = ID_GS_LOCK_MOVING;
			g_LastSelectedGump = index;
			LSG = index;
		}

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpSpell::OnLeftMouseUp()
{
	if (g_LastObjectLeftMouseDown == g_LastSelectedObject)
	{
		if (g_LastObjectLeftMouseDown == ID_GS_LOCK_MOVING)
		{
			LockMoving = !LockMoving;
			g_CancelDoubleClick = true;
		}
	}
}
//---------------------------------------------------------------------------
bool TGumpSpell::OnLeftMouseDoubleClick()
{
	if (!g_LastObjectLeftMouseDown)
	{
		UO->CastSpell(m_Serial);

		return true;
	}

	return false;
}
//---------------------------------------------------------------------------