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
//---------------------------------------------------------------------------
TGumpQuestion::TGumpQuestion(DWORD serial, short x, short y, BYTE variant)
: TGump(GT_QUESTION, serial, x, y), m_Variant(variant)
{
	Blocked = true;
	g_GrayMenuCount++;
}
//---------------------------------------------------------------------------
TGumpQuestion::~TGumpQuestion()
{
}
//---------------------------------------------------------------------------
void TGumpQuestion::PrepareTextures()
{
	UO->ExecuteGump(0x0816);
	UO->ExecuteButton(0x0817);
	UO->ExecuteButton(0x081A);
}
//---------------------------------------------------------------------------
void TGumpQuestion::GenerateFrame(int posX, int posY)
{
	if (!g_DrawMode)
	{
		FrameRedraw = false;
		FrameCreated = false;

		return;
	}

	DWORD index = (DWORD)this;

	//Нажата ли кнопка в окне?
	bool IsPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);
	
	//Может ли быть подсвечен элемент?
	int CanSelectedButton = ((g_LastSelectedGump == index) ? g_LastSelectedObject : 0);

	//Может ли быть нажат элемент?
	int CanPressedButton = 0;
	if (IsPressed && g_LastObjectLeftMouseDown == g_LastSelectedObject)
		CanPressedButton = g_LastObjectLeftMouseDown;
	
	glNewList((GLuint)index, GL_COMPILE);
	
		UO->DrawGump(0x0816, 0, posX, posY);
		
		if (m_Variant == 1)
		{
			FontManager->DrawA(1, "Quit\nUltima Online?", 0x0386, posX + 33, posY + 30);
			//FontManager->DrawA(1, "Quit", 0x0386, posX + 33, posY + 30);
			//FontManager->DrawA(1, "Ultima Online?", 0x0386, posX + 33, posY + 45);
		}
		else if (m_Variant == 2)
		{
			FontManager->DrawA(1, "This may flag\nyou criminal!", 0x0386, posX + 33, posY + 30);
			//FontManager->DrawA(1, "This may flag", 0x0386, posX + 33, posY + 30);
			//FontManager->DrawA(1, "you criminal!", 0x0386, posX + 33, posY + 45);
		}

		WORD gumpID = 0x0817;
		if (CanPressedButton == ID_GQ_BUTTON_CANCEL)
			gumpID = 0x0818;
		else if (CanSelectedButton == ID_GQ_BUTTON_CANCEL)
			gumpID = 0x0819;
		UO->DrawGump(gumpID, 0, posX + 37, posY + 75); //Button cancel

		gumpID = 0x081A;
		if (CanPressedButton == ID_GQ_BUTTON_OKAY)
			gumpID = 0x081B;
		else if (CanSelectedButton == ID_GQ_BUTTON_OKAY)
			gumpID = 0x081C;
		UO->DrawGump(gumpID, 0, posX + 100, posY + 75); //Button okay

	glEndList();

	FrameRedraw = true;
	FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpQuestion::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	//Нажата ли кнопка в окне?
	bool IsPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);
	
	//Может ли быть подсвечен элемент?
	int CanSelectedButton = ((g_LastSelectedGump == index) ? g_LastSelectedObject : 0);

	//Может ли быть нажат элемент?
	int CanPressedButton = 0;
	if (IsPressed && g_LastObjectLeftMouseDown == g_LastSelectedObject)
		CanPressedButton = g_LastObjectLeftMouseDown;
	
	int posX = X;
	int posY = Y;

	if (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && !g_LastObjectLeftMouseDown)
	{
		posX += (g_MouseX - g_DroppedLeftMouseX);
		posY += (g_MouseY - g_DroppedLeftMouseY);

		if (mode)
			GenerateFrame(posX, posY);
	}
	else if (mode)
	{
		if (IsPressed || CanSelectedButton)
			GenerateFrame(posX, posY);
		else if (FrameRedraw)
		{
			GenerateFrame(posX, posY);
			FrameRedraw = false;
		}
	}

	if (mode)
	{
		if (!FrameCreated)
			GenerateFrame(posX, posY);

		glCallList((GLuint)index);
	}
	else
	{
		int LSG = 0;
		
		if (UO->GumpPixelsInXY(0x0816, posX, posY))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}

		posY += 75;

		if (UO->GumpPixelsInXY(0x0817, posX + 37, posY))
			LSG = ID_GQ_BUTTON_CANCEL; //Button cancel
		else if (UO->GumpPixelsInXY(0x081A, posX + 100, posY))
			LSG = ID_GQ_BUTTON_OKAY; //Button okay

		if (LSG != 0)
			g_LastSelectedObject = LSG; //Если что-то нашлось - выбираем

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
			UO->AttackReq(ID);
			GumpManager->RemoveGump(this);
		}
	}
}
//---------------------------------------------------------------------------