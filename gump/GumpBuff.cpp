/****************************************************************************
**
** GumpBuff.cpp
**
** Copyright (C) June 2016 Hotride
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
//----------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------
TGumpBuff::TGumpBuff(DWORD serial, short x, short y)
: TGump(GT_BUFF, serial, x, y)
{
	m_Graphic = 0x7580;
}
//----------------------------------------------------------------------------
TGumpBuff::~TGumpBuff()
{
}
//----------------------------------------------------------------------------
void TGumpBuff::PrepareTextures()
{
	UO->ExecuteGump(m_Graphic);
}
//---------------------------------------------------------------------------
void TGumpBuff::GenerateFrame()
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	CalculateGumpState();

	glNewList((GLuint)this, GL_COMPILE);

		UO->DrawGump(m_Graphic, 0, 0, 0); //Body

	glEndList();

	m_FrameCreated = true;
	m_FrameRedraw = true;
}
//----------------------------------------------------------------------------
bool TGumpBuff::UnderMouse()
{
	return UO->GumpPixelsInXY(m_Graphic, 0, 0);
}
//----------------------------------------------------------------------------
int TGumpBuff::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	CalculateGumpState();

	if (mode) //Отрисовка
	{
		if (!m_FrameCreated || g_GumpPressedElement || g_GumpMovingOffsetX || g_GumpMovingOffsetY)
			GenerateFrame();
		else if (m_FrameRedraw)
		{
			GenerateFrame();
			m_FrameRedraw = false;
		}

		glTranslatef(g_GumpTranslateX, g_GumpTranslateY, 0.0f);

		glCallList((GLuint)this);

		glTranslatef(-g_GumpTranslateX, -g_GumpTranslateY, 0.0f);
	}
	else //Выбор объектов
	{
		int oldMouseX = g_MouseX;
		int oldMouseY = g_MouseY;
		g_MouseX -= (int)g_GumpTranslateX;
		g_MouseY -= (int)g_GumpTranslateY;

		int LSG = 0;

		if (UnderMouse())
		{
			g_LastSelectedGump = (DWORD)this;
			LSG = 0;
		}

		g_MouseX = oldMouseX;
		g_MouseY = oldMouseY;

		if (LSG != 0)
			g_LastSelectedObject = LSG; //Если что-то нашлось - выбираем

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpBuff::OnLeftMouseUp()
{
	//Проверим валидность переменных
	if (g_LastObjectLeftMouseDown != g_LastSelectedObject || !g_LastObjectLeftMouseDown || Minimized || !g_LastSelectedGump)
		return;

	if (g_LastObjectLeftMouseDown == ID_GB_NEXT_WINDOW_DIRECTION)
	{
		m_Graphic++;

		if (m_Graphic > 0x7582)
			m_Graphic = 0x757F;
	}
}
//----------------------------------------------------------------------------