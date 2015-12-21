/****************************************************************************
**
** TextEntryDialogGump.cpp
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
TGumpTextEntryDialog::TGumpTextEntryDialog(DWORD serial, short x, short y, BYTE variant, int maxLength, string text, string description)
: TGump(GT_TEXT_ENTRY_DIALOG, serial, x, y), m_Text(text), m_Description(description),
m_Variant(variant), m_MaxLength(maxLength), m_ParentID(0), m_ButtonID(0)
{
	NoMove = true;
	Blocked = true;
	TextEntry = new TEntryText(maxLength, 0, 0, (variant == 2));

	TGump *gumpEntry = GumpManager->GetTextEntryOwner();

	EntryPointer = TextEntry;
	
	if (gumpEntry != NULL)
		gumpEntry->UpdateFrame();
	
	if (!g_GrayMenuCount)
	{
		UO->InitScreen(GS_GAME_BLOCKED);
		GameBlockedScreen->Code = 2;
	}

	g_GrayMenuCount++;
}
//---------------------------------------------------------------------------
TGumpTextEntryDialog::~TGumpTextEntryDialog()
{
	if (TextEntry != NULL)
	{
		delete TextEntry;
		TextEntry = NULL;
	}
}
//---------------------------------------------------------------------------
void TGumpTextEntryDialog::PrepareTextures()
{
	UO->ExecuteGump(0x0474);
}
//---------------------------------------------------------------------------
void TGumpTextEntryDialog::GenerateFrame(int posX, int posY)
{
	if (!g_DrawMode)
	{
		FrameRedraw = false;
		FrameCreated = false;

		return;
	}
	
	glNewList((GLuint)this, GL_COMPILE);
	
		UO->DrawGump(0x0474, 0, posX, posY); //Gump
		
		FontManager->DrawA(2, m_Text.c_str(), 0x0386, posX + 60, posY + 50);

		FontManager->DrawA(2, m_Description.c_str(), 0x0386, posX + 60, posY + 108);
		
	glEndList();
	
	FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpTextEntryDialog::Draw(bool &mode)
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

	if (mode)
	{
		if (!FrameCreated)
			GenerateFrame(posX, posY);

		glCallList((GLuint)index);

		WORD gumpID = 0x0475; //Text field
		if (EntryPointer == TextEntry)
			gumpID = 0x0477; //Text field (active)
		else if (CanSelectedButton == 3)
			gumpID = 0x0476; //Text field (lighted)

		UO->DrawGump(gumpID, 0, posX + 60, posY + 130); //Text field

		if (gumpID == 0x0477) //Указатель консоли в гампе
			TextEntry->DrawA(1, 0x0386, posX + 71, posY + 137);
		
		posY += 190;

		gumpID = 0x047B; //Button Okay
		if (CanPressedButton == ID_GTED_BUTTON_OKAY)
			gumpID = 0x047C; //Button Okay (pressed)
		else if (CanSelectedButton == ID_GTED_BUTTON_OKAY)
			gumpID = 0x047D; //Button Okay (lighted)

		UO->DrawGump(gumpID, 0, posX + 117, posY); //Button Okay
		
		if (!NoClose)
		{
			gumpID = 0x0478; //Button Cancel
			if (CanPressedButton == ID_GTED_BUTTON_CANCEL)
				gumpID = 0x0479; //Button Cancel (pressed)
			else if (CanSelectedButton == ID_GTED_BUTTON_CANCEL)
				gumpID = 0x047A; //Button Cancel (lighted)

			UO->DrawGump(gumpID, 0, posX + 204, posY); //Button Cancel
		}
	}
	else
	{
		int LSG = 0;
		
		if (UO->GumpPixelsInXY(0x0474, posX, posY))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}
		
		if (UO->GumpPixelsInXY(0x0475, posX + 60, posY + 130))
			LSG = ID_GTED_TEXT_FIELD; //Text field
		else if (UO->GumpPixelsInXY(0x047B, posX + 117, posY + 190))
			LSG = ID_GTED_BUTTON_OKAY; //Button okay
		else if (!NoClose && UO->GumpPixelsInXY(0x0478, posX + 204, posY + 190))
			LSG = ID_GTED_BUTTON_CANCEL; //Button cancel

		if (LSG != 0)
			g_LastSelectedObject = LSG; //Если что-то нашлось - выбираем

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpTextEntryDialog::OnLeftMouseUp()
{
	if (g_LastSelectedObject == ID_GTED_BUTTON_OKAY) //Button okay
		SendTextEntryDialogResponse(true);
	else if (g_LastSelectedObject == ID_GTED_BUTTON_CANCEL) //Button cancel
		SendTextEntryDialogResponse(false);
}
//----------------------------------------------------------------------------
void TGumpTextEntryDialog::OnCharPress(WPARAM &wparam, LPARAM &lparam)
{
	if (wparam == VK_RETURN || wparam == VK_BACK || wparam == VK_ESCAPE || EntryPointer == NULL)
		return; //Ignore no print keys

	wchar_t wstr[2] = {0};
	wstr[0] = wparam;

	wstring str(wstr);
	if (!str.length())
		return;

	if (m_Variant == 2) //Только числа
	{
		if (wparam >= '0' && wparam <= '9')
		{
			EntryPointer->Insert(wparam);

			int val = atoi(EntryPointer->c_str());
			if (val > m_MaxLength)
				EntryPointer->Remove(true);
		}
	}
	else if (m_Variant == 1) //Что угодно, но с ограничением по длине
	{
		if (EntryPointer->Length() < m_MaxLength)
			EntryPointer->Insert(wparam);
	}
}
//----------------------------------------------------------------------------
void TGumpTextEntryDialog::OnKeyPress(WPARAM &wparam, LPARAM &lparam)
{
	switch (wparam)
	{
		case VK_RETURN:
		{
			SendTextEntryDialogResponse(true);

			break;
		}
		case VK_HOME:
		{
			EntryPointer->SetPos(0);

			break;
		}
		case VK_END:
		{
			EntryPointer->SetPos(EntryPointer->Length());

			break;
		}
		case VK_LEFT:
		{
			EntryPointer->AddPos(-1);

			break;
		}
		case VK_RIGHT:
		{
			EntryPointer->AddPos(1);

			break;
		}
		case VK_BACK:
		{
			EntryPointer->Remove(true);

			break;
		}
		case VK_DELETE:
		{
			EntryPointer->Remove(false);

			break;
		}
		case VK_ESCAPE:
		{
			SendTextEntryDialogResponse(false);

			break;
		}
		default:

			break;
	}
}
//---------------------------------------------------------------------------