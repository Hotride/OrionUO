/****************************************************************************
**
** ConnectionScreen.cpp
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

TConnectionScreen *ConnectionScreen = NULL;
//---------------------------------------------------------------------------
TConnectionScreen::TConnectionScreen()
: TBaseScreen(), m_ConnectionFailed(false), m_Connected(false), m_Completed(false),
m_ErrorCode(0)
{
	FontManager->GenerateA(2, m_Text[0], "Connecting...", 0x0386, 260, TS_CENTER);
	FontManager->GenerateA(2, m_Text[1], "Verifying Account...", 0x0386, 260, TS_CENTER);

	FontManager->GenerateA(1, m_Text[2], "Connection lost", 0x0386, 200, TS_CENTER);
	
	FontManager->GenerateA(2, m_Text[3], "Incorrect name/password.", 0x0386, 260, TS_CENTER);
	FontManager->GenerateA(2, m_Text[4], "Someone is already using this account.", 0x0386, 260, TS_CENTER);
	FontManager->GenerateA(2, m_Text[5], "Your account has been blocked.", 0x0386, 260, TS_CENTER);
	FontManager->GenerateA(2, m_Text[6], "Your account credentials are invalid.", 0x0386, 260, TS_CENTER);
	FontManager->GenerateA(2, m_Text[7], "Communication problem.", 0x0386, 260, TS_CENTER);
	FontManager->GenerateA(2, m_Text[8], "The IGR concurrency limit has been met.", 0x0386, 260, TS_CENTER);
	FontManager->GenerateA(2, m_Text[9], "The IGR time limit has been met.", 0x0386, 260, TS_CENTER);
	FontManager->GenerateA(2, m_Text[10], "General IGR authentication failure.", 0x0386, 260, TS_CENTER);
	FontManager->GenerateA(2, m_Text[11], "Couldn't connect to Ultima Online.  Please try again in a few moments.", 0x0386, 260, TS_CENTER);

	FontManager->GenerateA(2, m_Text[12], "Entering Britannia...", 0x0386, 260, TS_CENTER);
	FontManager->GenerateA(2, m_Text[13], "Your character name is too short.", 0x0386, 260, TS_CENTER);
	FontManager->GenerateA(2, m_Text[14], "No character to login with.", 0x0386, 260, TS_CENTER);
	
	FontManager->GenerateA(2, m_Text[15], "You must have three unique skills choosen!", 0x0386, 260, TS_CENTER);
	
	FontManager->GenerateA(2, m_Text[16], "That character password is invalid.", 0x0386, 260, TS_CENTER);
	FontManager->GenerateA(2, m_Text[17], "That character does not exist.", 0x0386, 260, TS_CENTER);
	FontManager->GenerateA(2, m_Text[18], "That character is being played right now.", 0x0386, 260, TS_CENTER);
	FontManager->GenerateA(2, m_Text[19], "That character is not old enough to delete. The character must be 7 days old before it can be deleted.", 0x0386, 260, TS_CENTER);
	FontManager->GenerateA(2, m_Text[20], "That character is currently queued for backup and cannot be deleted.", 0x0386, 260, TS_CENTER);
	FontManager->GenerateA(2, m_Text[21], "Couldn't carry out your request.", 0x0386, 260, TS_CENTER);
	
	FontManager->GenerateA(2, m_Text[22], "Incorrect password.", 0x0386, 260, TS_CENTER);
	FontManager->GenerateA(2, m_Text[23], "This character does not exist anymore.  You will have to recreate it.", 0x0386, 260, TS_CENTER);
	FontManager->GenerateA(2, m_Text[24], "This character already exists.\nPlaying...", 0x0386, 260, TS_CENTER);
	FontManager->GenerateA(2, m_Text[25], "The client could not attach to the game server. It must have been taken down, please wait a few minutes and try again.", 0x0386, 260, TS_CENTER);
	FontManager->GenerateA(2, m_Text[26], "The client could not attach to the game server. It must have been taken down, please wait a few minutes and try again.", 0x0386, 260, TS_CENTER);
	FontManager->GenerateA(2, m_Text[27], "Another character from this account is currently online in this world.  You must either log in as that character or wait for it to time out.", 0x0386, 260, TS_CENTER);
	FontManager->GenerateA(2, m_Text[28], "An error has occurred in the synchronization between the login servers and this world.  Please close your client and try again.", 0x0386, 260, TS_CENTER);
	FontManager->GenerateA(2, m_Text[29], "You have been idle for too long.  If you do not do anything in the next minute, you will be logged out.", 0x0386, 260, TS_CENTER);
	FontManager->GenerateA(2, m_Text[30], "", 0x0386, 260, TS_CENTER);
	FontManager->GenerateA(2, m_Text[31], "", 0x0386, 260, TS_CENTER);
}
//---------------------------------------------------------------------------
TConnectionScreen::~TConnectionScreen()
{
	IFOR(i, 0, 32)
		m_Text[i].Clear();
}
//---------------------------------------------------------------------------
/*!
Инициализация
@return 
*/
void TConnectionScreen::Init()
{
	m_ConnectionFailed = false;
	m_Connected = false;
	m_Completed= false;
	m_ErrorCode = 0;
	m_Type = CST_LOGIN;

	SmoothMonitor.UseSunrise();
	m_SmoothScreenAction = 0;

	ToolTip.SeqIndex = 0;

	//Prepare textures on Main Screen:
	Orion->ExecuteGump(0x0588); //Main Screen background
	Orion->ExecuteGump(0x157C); //Main Screen
	Orion->ExecuteGump(0x15A0); //Main Screen Notes
	Orion->ExecuteGump(0x1589); //X gump
	Orion->ExecuteResizepic(0xA28); //Connecting field
	Orion->ExecuteGumpPart(0x0481, 3); //Button v
}
//---------------------------------------------------------------------------
/*!
Обработка события после плавного затемнения экрана
@param [__in_opt] action Идентификатор действия
@return 
*/
void TConnectionScreen::ProcessSmoothAction( __in_opt BYTE action)
{
	if (action == 0xFF)
		action = m_SmoothScreenAction;

	if (action == ID_SMOOTH_CS_GO_SCREEN_MAIN)
		Orion->InitScreen(GS_MAIN);
	else if (action == ID_SMOOTH_CS_GO_SCREEN_CHARACTER)
		Orion->InitScreen(GS_CHARACTER);
	else if (action == ID_SMOOTH_CS_GO_SCREEN_PROFESSION)
		Orion->InitScreen(GS_PROFESSION_SELECT);
	else if (action == ID_SMOOTH_CS_SEND_DELETE)
	{
		TPacketDeleteCharacter packet(CharacterList.Selected);
		packet.Send();
	}
}
//---------------------------------------------------------------------------
/*!
Отрисовка/выбор объектов
@param [__in] mode true - отрисовка, false - выбор
@return При выборе объектов - идентификатор выбранного объекта
*/
int TConnectionScreen::Render( __in bool mode)
{
	DWORD ticks = g_Ticks;

	/*if (g_LastRenderTime > ticks)
	{
		if (mode || !g_SelectGumpObjects)
			return 0;
	}*/

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

		Orion->DrawGump(0x0588, 0, 0, 0, 640, 480); //Main Gump background
		Orion->DrawGump(0x157C, 0, 0, 0); //Main Gump
		Orion->DrawGump(0x15A0, 0, 0, 4); //Main Gump Notes
		Orion->DrawGump(0x1589, 0, 555, 4); //X gump
		
		if (m_Type != CST_CONLOST)
			Orion->DrawResizepicGump(0xA28, 142, 134, 356, 212); //Connecting field
		
		WORD mouseCursorID = 0x2073; //Main Gump mouse cursor

		if (m_Type == CST_CHARACTER_LIST)
		{
			if (m_ConnectionFailed)
			{
				m_Text[16 + m_ErrorCode].Draw(189, 178);
				
				WORD GumpID = 0x0481 + (int)(CanSelectedButton == ID_CS_OK);  //Button v / lighted
				if (CanPressedButton == ID_CS_OK)
					GumpID = 0x0483; //Button v pressed
				Orion->DrawGump(GumpID, 0, 306, 304);
			}
			else
			{
				char buf[80] = {0};
				sprintf(buf, "Permanently delete %s?", CharacterList.GetSelectedName().c_str());
				FontManager->DrawA(2, buf, 0x0386, 193, 184, 260, TS_CENTER);

				WORD GumpID = 0x0481 + (int)(CanSelectedButton == ID_CS_OK);  //Button v / lighted
				if (CanPressedButton == ID_CS_OK)
					GumpID = 0x0483; //Button v pressed
				Orion->DrawGump(GumpID, 0, 264, 304);
			
				GumpID = 0x047E + (int)(CanSelectedButton == ID_CS_CANCEL);  //Button x / lighted
				if (CanPressedButton == ID_CS_CANCEL)
					GumpID = 0x0480; //Button x pressed
				Orion->DrawGump(GumpID, 0, 348, 304);
			}
		}
		else if (m_Type == CST_GAME)
		{
			if (m_ConnectionFailed)
			{
				m_Text[12 + m_ErrorCode].Draw(189, 178);
				
				WORD GumpID = 0x0481 + (int)(CanSelectedButton == ID_CS_OK);  //Button v / lighted
				if (CanPressedButton == ID_CS_OK)
					GumpID = 0x0483; //Button v pressed
				Orion->DrawGump(GumpID, 0, 306, 304);
			}
			else
			{
				m_Text[12].Draw(189, 178);

				mouseCursorID = 0x2077; //Waiting mouse cursor
			}
		}
		else if (m_Type == CST_GAME_LOGIN)
		{
			m_Text[22 + m_ErrorCode].Draw(189, 178);
				
			WORD GumpID = 0x0481 + (int)(CanSelectedButton == ID_CS_OK);  //Button v / lighted
			if (CanPressedButton == ID_CS_OK)
				GumpID = 0x0483; //Button v pressed
			Orion->DrawGump(GumpID, 0, 306, 304);
		}
		else if (m_Type == CST_SELECT_PROFESSOIN)
		{
			WORD GumpID = 0x0481 + (int)(CanSelectedButton == ID_CS_OK);  //Button v / lighted
			if (CanPressedButton == ID_CS_OK)
				GumpID = 0x0483; //Button v pressed
			Orion->DrawGump(GumpID, 0, 306, 304);

			m_Text[15].Draw(189, 178);
		}
		else if (m_Type == CST_CONLOST)
		{
			Orion->DrawResizepicGump(0xA28, 210, 178, 203, 121); //Connecting field

			m_Text[2].Draw(215, 222);
			
			WORD GumpID = 0x0481 + (int)(CanSelectedButton == ID_CS_OK);  //Button v / lighted
			if (CanPressedButton == ID_CS_OK)
				GumpID = 0x0483; //Button v pressed
			Orion->DrawGump(GumpID, 0, 297, 257);

			mouseCursorID = 0x2077; //Waiting mouse cursor
		}
		else
		{
			if (m_ConnectionFailed)
			{
				m_Text[m_ErrorCode + 3].Draw(189, 178);

				WORD GumpID = 0x0481 + (int)(CanSelectedButton == ID_CS_OK);  //Button v / lighted
				if (CanPressedButton == ID_CS_OK)
					GumpID = 0x0483; //Button v pressed
				Orion->DrawGump(GumpID, 0, 306, 304);
			}
			else
			{
				m_Text[(int)m_Connected].Draw(189, 178);

				mouseCursorID = 0x2077; //Waiting mouse cursor
			}
		}

		DrawSmoothMonitorEffect();

		MouseManager.Draw(mouseCursorID);

		g_GL.EndDraw();
	}
	else
	{
		g_LastSelectedObject = 0;
		
		if (!m_ConnectionFailed && m_Type == CST_CHARACTER_LIST)
		{
			if (Orion->GumpPixelsInXY(0x0481, 264, 304))
				g_LastSelectedObject = ID_CS_OK; //Button v
			else if (Orion->GumpPixelsInXY(0x047E, 348, 304))
				g_LastSelectedObject = ID_CS_CANCEL; //Button x
		}
		else if (m_Type == CST_CONLOST)
		{
			if (Orion->GumpPixelsInXY(0x0481, 297, 257))
				g_LastSelectedObject = ID_CS_OK; //Button v
		}
		else if (Orion->GumpPixelsInXY(0x0481, 306, 304))
			g_LastSelectedObject = ID_CS_OK; //Button v
		
		return g_LastSelectedObject;
	}

	return 0;
}
//---------------------------------------------------------------------------
/*!
Нажатие левой кнопки мыши
@return 
*/
void TConnectionScreen::OnLeftMouseDown()
{
	Render(false);

	g_LeftMouseDown = true;
	g_LastObjectLeftMouseDown = g_LastSelectedObject;
}
//---------------------------------------------------------------------------
/*!
Отпускание левой кнопки мыши
@return 
*/
void TConnectionScreen::OnLeftMouseUp()
{
	Render(false);

	g_LeftMouseDown = false;

	if (g_LastSelectedObject == 0 || g_LastSelectedObject != g_LastObjectLeftMouseDown)
	{
		g_LastObjectLeftMouseDown = 0;
		return;
	}

	if (g_LastObjectLeftMouseDown == ID_CS_OK) //v button
	{
		if (m_Type == CST_CHARACTER_LIST)
		{
			if (!m_ConnectionFailed)
				CreateSmoothAction(ID_SMOOTH_CS_SEND_DELETE);
			else
				CreateSmoothAction(ID_SMOOTH_CS_GO_SCREEN_CHARACTER);
		}
		else if (m_Type == CST_SELECT_PROFESSOIN)
			CreateSmoothAction(ID_SMOOTH_CS_GO_SCREEN_PROFESSION);
		else if (m_Type == CST_GAME || m_Type == CST_GAME_LOGIN)
			CreateSmoothAction(ID_SMOOTH_CS_GO_SCREEN_CHARACTER);
		else if (m_Type == CST_CONLOST || m_ConnectionFailed)
			CreateSmoothAction(ID_SMOOTH_CS_GO_SCREEN_MAIN);
	}
	else if (g_LastObjectLeftMouseDown == ID_CS_CANCEL) //Button x
		CreateSmoothAction(ID_SMOOTH_CS_GO_SCREEN_CHARACTER);

	g_LastObjectLeftMouseDown = 0;
}
//---------------------------------------------------------------------------
/*!
Обработка нажатия клавиши
@param [__in] wparam не подписанный параметр
@param [__in] lparam не подписанный параметр
@return 
*/
void TConnectionScreen::OnKeyPress( __in WPARAM wparam, __in LPARAM lparam)
{
	switch (wparam)
	{
		case VK_RETURN:
		{
			if (m_Type == CST_CHARACTER_LIST)
				CreateSmoothAction(ID_SMOOTH_CS_SEND_DELETE);
			else if (m_Type == CST_SELECT_PROFESSOIN)
				CreateSmoothAction(ID_SMOOTH_CS_GO_SCREEN_PROFESSION);
			else if (m_Type == CST_GAME)
			{
				if (m_ErrorCode > 0)
					CreateSmoothAction(ID_SMOOTH_CS_GO_SCREEN_CHARACTER);
			}
			else if (m_Type == CST_CONLOST || m_ConnectionFailed)
				CreateSmoothAction(ID_SMOOTH_CS_GO_SCREEN_MAIN);

			break;
		}
		case VK_ESCAPE:
		{
			if (m_Type == CST_CHARACTER_LIST)
				CreateSmoothAction(ID_SMOOTH_CS_GO_SCREEN_CHARACTER);
			else if (m_Type == CST_SELECT_PROFESSOIN)
				CreateSmoothAction(ID_SMOOTH_CS_GO_SCREEN_PROFESSION);
			else if (m_Type == CST_GAME)
			{
				if (m_ErrorCode > 0)
					CreateSmoothAction(ID_SMOOTH_CS_GO_SCREEN_CHARACTER);
			}
			else if (m_Type == CST_CONLOST || m_ConnectionFailed)
				CreateSmoothAction(ID_SMOOTH_CS_GO_SCREEN_MAIN);

			break;
		}
		default:
			break;
	}
}
//---------------------------------------------------------------------------