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
	m_NoMove = true;
	m_Blocked = true;
	TextEntry = new TEntryText(maxLength, 0, 0, (variant == 2));

	TGump *gumpEntry = GumpManager->GetTextEntryOwner();

	EntryPointer = TextEntry;
	
	if (gumpEntry != NULL)
		gumpEntry->FrameCreated = false;
	
	if (!g_GrayMenuCount)
	{
		Orion->InitScreen(GS_GAME_BLOCKED);
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
	Orion->ExecuteGump(0x0474);
}
//---------------------------------------------------------------------------
void TGumpTextEntryDialog::GenerateFrame()
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	CalculateGumpState();

	glNewList((GLuint)this, GL_COMPILE);
	
		Orion->DrawGump(0x0474, 0, 0, 0); //Gump
		
		FontManager->DrawA(2, m_Text.c_str(), 0x0386, 60, 50);

		FontManager->DrawA(2, m_Description.c_str(), 0x0386, 60, 108);

		WORD gumpID = 0x0475; //Text field
		if (EntryPointer == TextEntry)
			gumpID = 0x0477; //Text field (active)
		else if (g_GumpSelectElement == 3)
			gumpID = 0x0476; //Text field (lighted)

		Orion->DrawGump(gumpID, 0, 60, 130); //Text field

		if (gumpID == 0x0477) //Указатель консоли в гампе
			TextEntry->DrawA(1, 0x0386, 71, 137);

		gumpID = 0x047B; //Button Okay
		if (g_GumpPressedElement == ID_GTED_BUTTON_OKAY)
			gumpID = 0x047C; //Button Okay (pressed)
		else if (g_GumpSelectElement == ID_GTED_BUTTON_OKAY)
			gumpID = 0x047D; //Button Okay (lighted)

		Orion->DrawGump(gumpID, 0, 117, 190); //Button Okay

		if (!NoClose)
		{
			gumpID = 0x0478; //Button Cancel
			if (g_GumpPressedElement == ID_GTED_BUTTON_CANCEL)
				gumpID = 0x0479; //Button Cancel (pressed)
			else if (g_GumpSelectElement == ID_GTED_BUTTON_CANCEL)
				gumpID = 0x047A; //Button Cancel (lighted)

			Orion->DrawGump(gumpID, 0, 204, 190); //Button Cancel
		}

	glEndList();

	m_FrameRedraw = true;
	m_FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpTextEntryDialog::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	CalculateGumpState();

	if (mode)
	{
		if (!m_FrameCreated || g_GumpSelectElement)
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
		
		if (Orion->GumpPixelsInXY(0x0474, 0, 0))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}
		
		if (Orion->GumpPixelsInXY(0x0475, 60, 130))
			LSG = ID_GTED_TEXT_FIELD; //Text field
		else if (Orion->GumpPixelsInXY(0x047B, 117, 190))
			LSG = ID_GTED_BUTTON_OKAY; //Button okay
		else if (!NoClose && Orion->GumpPixelsInXY(0x0478, 204, 190))
			LSG = ID_GTED_BUTTON_CANCEL; //Button cancel

		g_MouseX = oldMouseX;
		g_MouseY = oldMouseY;

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
			else
				m_FrameCreated = false;
		}
	}
	else if (m_Variant == 1) //Что угодно, но с ограничением по длине
	{
		if (EntryPointer->Length() < m_MaxLength)
		{
			EntryPointer->Insert(wparam);
			m_FrameCreated = false;
		}
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
			m_FrameCreated = false;

			break;
		}
		case VK_END:
		{
			EntryPointer->SetPos(EntryPointer->Length());
			m_FrameCreated = false;

			break;
		}
		case VK_LEFT:
		{
			EntryPointer->AddPos(-1);
			m_FrameCreated = false;

			break;
		}
		case VK_RIGHT:
		{
			EntryPointer->AddPos(1);
			m_FrameCreated = false;

			break;
		}
		case VK_BACK:
		{
			EntryPointer->Remove(true);
			m_FrameCreated = false;

			break;
		}
		case VK_DELETE:
		{
			EntryPointer->Remove(false);
			m_FrameCreated = false;

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