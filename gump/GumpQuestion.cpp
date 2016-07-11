/****************************************************************************
**
** GumpQuestion.cpp
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

TTextTexture TGumpQuestion::m_Text[2];
//---------------------------------------------------------------------------
TGumpQuestion::TGumpQuestion(DWORD serial, short x, short y, BYTE variant)
: TGump(GT_QUESTION, serial, x, y), m_Variant(variant)
{
	m_Blocked = true;
	g_GrayMenuCount++;
}
//---------------------------------------------------------------------------
TGumpQuestion::~TGumpQuestion()
{
}
//----------------------------------------------------------------------------
void TGumpQuestion::InitTextTextures()
{
	FontManager->GenerateA(1, m_Text[0], "Quit\nUltima Online?", 0x0386);
	FontManager->GenerateA(1, m_Text[1], "This may flag\nyou criminal!", 0x0386);
}
//----------------------------------------------------------------------------
void TGumpQuestion::ReleaseTextTextures()
{
	IFOR(i, 0, 2)
		m_Text[i].Clear();
}
//---------------------------------------------------------------------------
void TGumpQuestion::PrepareTextures()
{
	UO->ExecuteGump(0x0816);
	UO->ExecuteButton(0x0817);
	UO->ExecuteButton(0x081A);
}
//---------------------------------------------------------------------------
void TGumpQuestion::GenerateFrame()
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	CalculateGumpState();

	glNewList((GLuint)this, GL_COMPILE);
	
		UO->DrawGump(0x0816, 0, 0, 0);
		
		if (m_Variant == 1)
			m_Text[0].Draw(33, 30);
		else if (m_Variant == 2)
			m_Text[1].Draw(33, 30);

		WORD gumpID = 0x0817;
		if (g_GumpPressedElement == ID_GQ_BUTTON_CANCEL)
			gumpID = 0x0818;
		else if (g_GumpSelectElement == ID_GQ_BUTTON_CANCEL)
			gumpID = 0x0819;
		UO->DrawGump(gumpID, 0, 37, 75); //Button cancel

		gumpID = 0x081A;
		if (g_GumpPressedElement == ID_GQ_BUTTON_OKAY)
			gumpID = 0x081B;
		else if (g_GumpSelectElement == ID_GQ_BUTTON_OKAY)
			gumpID = 0x081C;
		UO->DrawGump(gumpID, 0, 100, 75); //Button okay

	glEndList();

	m_FrameRedraw = true;
	m_FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpQuestion::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	CalculateGumpState();

	if (mode)
	{
		if (!m_FrameCreated || g_GumpSelectElement) // || g_GumpPressed
			GenerateFrame();
		else if (m_FrameRedraw)
		{
			GenerateFrame();
			m_FrameRedraw = false;
		}

		glTranslatef(g_GumpTranslateX, g_GumpTranslateY, 0.0f);

		glCallList((GLuint)index);

		glTranslatef(-g_GumpTranslateX, -g_GumpTranslateY, 0.0f);
	}
	else
	{
		int oldMouseX = g_MouseX;
		int oldMouseY = g_MouseY;
		g_MouseX -= (int)g_GumpTranslateX;
		g_MouseY -= (int)g_GumpTranslateY;

		int LSG = 0;
		
		if (UO->GumpPixelsInXY(0x0816, 0, 0))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}

		if (UO->GumpPixelsInXY(0x0817, 37, 75))
			LSG = ID_GQ_BUTTON_CANCEL; //Button cancel
		else if (UO->GumpPixelsInXY(0x081A, 100, 75))
			LSG = ID_GQ_BUTTON_OKAY; //Button okay

		g_MouseX = oldMouseX;
		g_MouseY = oldMouseY;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpQuestion::OnLeftMouseUp()
{
	if (g_LastSelectedObject == ID_GQ_BUTTON_CANCEL) //Button cancel
		GumpManager->RemoveGump(this);
	else if (g_LastSelectedObject == ID_GQ_BUTTON_OKAY) //Button okay
	{
		if (m_Variant == 1)
			GameScreen->CreateSmoothAction(TGameScreen::ID_SMOOTH_GS_LOGOUT);
		else if (m_Variant == 2)
		{
			UO->AttackReq(m_ID);
			GumpManager->RemoveGump(this);
		}
	}
}
//---------------------------------------------------------------------------