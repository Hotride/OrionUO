/****************************************************************************
**
** CharacterListGump.cpp
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

TCharacterListScreen *CharacterListScreen = NULL;
//---------------------------------------------------------------------------
TCharacterListScreen::TCharacterListScreen()
: TBaseScreen()
{
	FontManager->GenerateA(2, m_Text, "Character Selection", 0x0386);
}
//---------------------------------------------------------------------------
TCharacterListScreen::~TCharacterListScreen()
{
	m_Text.Clear();
}
//---------------------------------------------------------------------------
void TCharacterListScreen::Init()
{
	char buf[128] = {0};
	sprintf(buf, "Ultima Online - %s (%s)", MainScreen->m_Account->c_str(), ServerList.GetServerName().c_str());

	SetWindowTextA(g_hWnd, buf);

	CharacterList.Selected = 0;

	g_UpdateRange = 18;

	SmoothMonitor.UseSunrise();
	m_SmoothScreenAction = 0;

	PopupHelp.SeqIndex = 0;

	//Prepare textures on Main Screen:
	UO->ExecuteGump(0x0588); //Main Screen background
	UO->ExecuteGump(0x157C); //Main Screen
	UO->ExecuteGump(0x15A0); //Main Screen Notes
	UO->ExecuteResizepic(0x13BE); //ActPwd Container
	UO->ExecuteGump(0x058A); //Main Screen Castle?
	UO->ExecuteGumpPart(0x1589, 3); //X gump
	UO->ExecuteGumpPart(0x15A4, 3); //> gump
	UO->ExecuteResizepic(0x0BB8); //Account/Password text field
	UO->ExecuteGumpPart(0x00D2, 2); //Checkbox on / off
}
//---------------------------------------------------------------------------
void TCharacterListScreen::ProcessSmoothAction(BYTE action)
{
	if (action == 0xFF)
		action = m_SmoothScreenAction;

	if (action == ID_SMOOTH_CLS_QUIT)
		PostMessage(g_hWnd, WM_CLOSE, 0, 0);
	else if (action == ID_SMOOTH_CLS_CONNECT)
		UO->Connect();
	else if (action == ID_SMOOTH_CLS_SELECT_CHARACTER)
	{
		UO->CharacterSelection(CharacterList.Selected);

		if (!CharacterList.GetName(CharacterList.Selected).length())
			ConnectionScreen->ErrorCode = 2;
	}
	else if (action == ID_SMOOTH_CLS_GO_SCREEN_PROFESSION_SELECT)
		UO->InitScreen(GS_PROFESSION_SELECT);
	else if (action == ID_SMOOTH_CLS_GO_SCREEN_DELETE)
	{
		if (CharacterList.GetSelectedName().length())
		{
			UO->InitScreen(GS_DELETE);
			ConnectionScreen->Type = CST_CHARACTER_LIST;
		}
	}
}
//---------------------------------------------------------------------------
void TCharacterListScreen::InitPopupHelp()
{
	if (!ConfigManager.UseToolTips)
		return;
}
//---------------------------------------------------------------------------
int TCharacterListScreen::Render(bool mode)
{
	DWORD ticks = GetTickCount();

	if (g_LastRenderTime > ticks)
	{
		if (mode || !g_SelectGumpObjects)
			return 0;
	}

	int count = CharacterList.Count;
	bool testField = (ConnectionManager.ClientVersion >= CV_305D);
	int posInList = 0;
	int yOffset = 150;
	int yBonus = 0;
	int listTitleY = 106;

	if (ConnectionManager.ClientVersion >= CV_6040)
	{
		listTitleY = 96;
		yOffset = 125;
		yBonus = 45;
	}

	if (mode)
	{
		int CanSelectedButton = g_LastSelectedObject;

		int CanPressedButton = 0;
		if (g_LeftMouseDown && g_LastObjectLeftMouseDown == g_LastSelectedObject)
			CanPressedButton = g_LastObjectLeftMouseDown;
	
		g_LastRenderTime = ticks + (g_FrameDelay[(int)(GetForegroundWindow() == g_hWnd)]);

		g_GL.BeginDraw();

		if (DrawSmoothMonitor())
			return 0;

		glColor3f(1.0f, 1.0f, 1.0f);

		UO->DrawGump(0x0588, 0, 0, 0, 640, 480); //Main Gump background
		UO->DrawGump(0x157C, 0, 0, 0); //Main Gump
		UO->DrawGump(0x15A0, 0, 0, 4); //Main Gump Notes

		WORD gumpID = 0x1589 + (int)(CanSelectedButton == ID_CS_QUIT); //X gump /lighted
		if (g_LastObjectLeftMouseDown == ID_CS_QUIT)
			gumpID = 0x158B; //X gump
		UO->DrawGump(gumpID, 0, 555, 4); //X gump
		
		UO->DrawResizepicGump(0xA28, 160, 70, 408, 343 + yBonus); //Character list field

		m_Text.Draw(267, listTitleY);

		IFOR(i, 0, count)
		{
			int len = CharacterList.GetName(i).length();

			if (!testField || len != 0 || !i)
			{
				UO->DrawResizepicGump(0xBB8, 224, yOffset + (posInList * 40), 280, 30); //Character name field
				
				if (len)
				{
					WORD FColor = 0x034F;
					if (CharacterList.Selected == i)
						FColor = 0x0021;
					else if (g_LastSelectedObject == (ID_CS_CHARACTERS + i))
						FColor = 0x03E3;

					FontManager->DrawA(5, CharacterList.GetName(i).c_str(), FColor, 229, yOffset + (posInList * 40), 270, TS_CENTER); //Character name
				}

				posInList++;
			}
		}
		
		gumpID = 0x159D + (int)(CanSelectedButton == ID_CS_NEW); //New button
		if (g_LastObjectLeftMouseDown == ID_CS_NEW)
			gumpID = 0x159F; //New button pressed
		UO->DrawGump(gumpID, 0, 224, 350 + yBonus);
		
		gumpID = 0x159A + (int)(CanSelectedButton == ID_CS_DELETE); //Delete button
		if (g_LastObjectLeftMouseDown == ID_CS_DELETE)
			gumpID = 0x159C; //Delete button pressed
		UO->DrawGump(gumpID, 0, 442, 350 + yBonus);

		gumpID = 0x15A1 + (int)(CanSelectedButton == ID_CS_ARROW_PREV); //< gump
		if (g_LastObjectLeftMouseDown == ID_CS_ARROW_PREV)
			gumpID = 0x15A3; //< gump pressed
		UO->DrawGump(gumpID, 0, 586, 445);

		gumpID = 0x15A4 + (int)(CanSelectedButton == ID_CS_ARROW_NEXT); //> gump
		if (g_LastObjectLeftMouseDown == ID_CS_ARROW_NEXT)
			gumpID = 0x15A6; //> gump pressed
		UO->DrawGump(gumpID, 0, 610, 445);

		InitPopupHelp();

		DrawSmoothMonitorEffect();

		MouseManager.Draw(0x2073); //Main Gump mouse cursor

		g_GL.EndDraw();
	}
	else
	{
		g_LastSelectedObject = 0;

		if (UO->GumpPixelsInXY(0x1589, 555, 4))
			g_LastSelectedObject = ID_CS_QUIT; //X gump
		else if (UO->GumpPixelsInXY(0x15A1, 586, 445))
			g_LastSelectedObject = ID_CS_ARROW_PREV; //< gump
		else if (UO->GumpPixelsInXY(0x15A4, 610, 445))
			g_LastSelectedObject = ID_CS_ARROW_NEXT; //> gump
		else if (UO->GumpPixelsInXY(0x159D, 224, 350 + yBonus))
			g_LastSelectedObject = ID_CS_NEW; //New button
		else if (UO->GumpPixelsInXY(0x159A, 442, 350 + yBonus))
			g_LastSelectedObject = ID_CS_DELETE; //Delete button
		else
		{
			IFOR(i, 0, count)
			{
				int len = CharacterList.GetName(i).length();

				if (!testField || len != 0 || !i)
				{
					if (UO->ResizepicPixelsInXY(0xBB8, 224, yOffset + (posInList * 40), 280, 30))
					{
						g_LastSelectedObject = ID_CS_CHARACTERS + i; //Character name field

						break;
					}
				
					posInList++;
				}
			}
		}
		
		return g_LastSelectedObject;
	}

	return 0;
}
//---------------------------------------------------------------------------
void TCharacterListScreen::OnLeftMouseDown()
{
	Render(false);
	g_LeftMouseDown = true;
	g_LastObjectLeftMouseDown = g_LastSelectedObject;

	int count = CharacterList.Count;

	IFOR(i, 0, count)
	{
		if (g_LastSelectedObject == (ID_CS_CHARACTERS + i))
		{
			CharacterList.Selected = i;

			break;
		}
	}
}
//---------------------------------------------------------------------------
void TCharacterListScreen::OnLeftMouseUp()
{
	Render(false);
	g_LeftMouseDown = false;

	if (g_LastSelectedObject == 0 || g_LastSelectedObject != g_LastObjectLeftMouseDown)
	{
		g_LastObjectLeftMouseDown = 0;
		return;
	}

	if (g_LastObjectLeftMouseDown == ID_CS_QUIT) //x button
		CreateSmoothAction(ID_SMOOTH_CLS_QUIT);
	else if (g_LastObjectLeftMouseDown == ID_CS_ARROW_PREV) //< button
		CreateSmoothAction(ID_SMOOTH_CLS_CONNECT);
	else if (g_LastObjectLeftMouseDown == ID_CS_ARROW_NEXT) //> button
		CreateSmoothAction(ID_SMOOTH_CLS_SELECT_CHARACTER);
	else if (g_LastObjectLeftMouseDown == ID_CS_NEW) //New button
		CreateSmoothAction(ID_SMOOTH_CLS_GO_SCREEN_PROFESSION_SELECT);
	else if (g_LastObjectLeftMouseDown == ID_CS_DELETE) //Delete button
	{
		CreateSmoothAction(ID_SMOOTH_CLS_GO_SCREEN_DELETE);

		g_LastObjectLeftMouseDown = 0;
	}
	else
		g_LastObjectLeftMouseDown = 0;
}
//---------------------------------------------------------------------------
bool TCharacterListScreen::OnLeftMouseDoubleClick()
{
	Render(false);

	if (g_LastSelectedObject == 0)
		return false;

	int count = CharacterList.Count;

	IFOR(i, 0, count)
	{
		if (g_LastSelectedObject == (ID_CS_CHARACTERS + i))
		{
			if (!CharacterList.GetName(i).length())
				CreateSmoothAction(ID_SMOOTH_CLS_GO_SCREEN_PROFESSION_SELECT);
			else
				CreateSmoothAction(ID_SMOOTH_CLS_SELECT_CHARACTER);

			return true;
		}
	}

	return false;
}
//---------------------------------------------------------------------------