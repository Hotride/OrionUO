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
	Graphic = graphic;
}
//---------------------------------------------------------------------------
TGumpSpell::~TGumpSpell()
{
}
//---------------------------------------------------------------------------
void TGumpSpell::PrepareTextures()
{
	UO->ExecuteGump(Graphic);
}
//---------------------------------------------------------------------------
void TGumpSpell::GenerateFrame(int posX, int posY)
{
	if (!g_DrawMode)
	{
		FrameRedraw = false;
		FrameCreated = false;

		return;
	}
	
	glNewList((GLuint)this, GL_COMPILE);

		UO->DrawGump(Graphic, 0, posX, posY);
		
	glEndList();
	
	FrameCreated = true;
}
//---------------------------------------------------------------------------
int TGumpSpell::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	int posX = X;
	int posY = Y;

	if (CanBeMoved() && g_LeftMouseDown && g_LastGumpLeftMouseDown == index && !g_LastObjectLeftMouseDown)
	{
		posX += g_MouseX - g_DroppedLeftMouseX;
		posY += g_MouseY - g_DroppedLeftMouseY;

		if (mode)
			GenerateFrame(posX, posY);
	}

	if (mode)
	{
		if (!FrameCreated)
			GenerateFrame(posX, posY);

		glCallList((GLuint)index);

		if (g_ShowGumpLocker)
			g_GL.Draw(g_TextureGumpState[LockMoving], posX, posY, 10.0f, 14.0f);
	}
	else
	{
		if (UO->GumpPixelsInXY(Graphic, posX, posY))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}

		if (g_ShowGumpLocker && UO->PolygonePixelsInXY(posX, posY, 10, 14))
		{
			g_LastSelectedObject = ID_GS_LOCK_MOVING;
			g_LastSelectedGump = index;
		}
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
		UO->CastSpell(Serial);

		return true;
	}

	return false;
}
//---------------------------------------------------------------------------