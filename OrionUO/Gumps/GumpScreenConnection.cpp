// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GumpScreenConnection.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpScreenConnection::CGumpScreenConnection()
: CGump(GT_NONE, 0, 0, 0)
{
	NoMove = true;
	NoClose = true;
}
//----------------------------------------------------------------------------------
CGumpScreenConnection::~CGumpScreenConnection()
{
}
//----------------------------------------------------------------------------------
void CGumpScreenConnection::CreateText(int x, int y, string str, uchar font)
{
	WISPFUN_DEBUG("c113_f1");
	if (g_ConnectionScreen.GetText().length())
		str = g_ConnectionScreen.GetText();

	CGUIText *obj = new CGUIText(0x0386, x, y);
	obj->CreateTextureA(font, str, 260, TS_CENTER);
	Add(obj);
}
//----------------------------------------------------------------------------------
void CGumpScreenConnection::UpdateContent()
{
	WISPFUN_DEBUG("c113_f2");
	Clear();

	Add(new CGUIGumppicTiled(0x0E14, 0, 0, 640, 480));
	Add(new CGUIGumppic(0x157C, 0, 0));
	Add(new CGUIGumppic(0x15A0, 0, 4));
	Add(new CGUIGumppic(0x1589, 555, 4));

	if (g_ConnectionScreen.GetType() != CST_CONLOST)
		Add(new CGUIResizepic(0, 0x0A28, 142, 134, 356, 212));

	g_ConnectionScreen.CursorGraphic = 0x2073; //Main Gump mouse cursor

	if (g_ConnectionScreen.GetType() == CST_CHARACTER_LIST)
	{
		if (g_ConnectionScreen.GetConnectionFailed())
		{
			const string text[6] =
			{
				g_ClilocManager.Cliloc(g_Language)->GetA(3000018, false, "That character password is invalid."),
				g_ClilocManager.Cliloc(g_Language)->GetA(3000019, false, "That character does not exist."),
				g_ClilocManager.Cliloc(g_Language)->GetA(3000020, false, "That character is being played right now."),
				g_ClilocManager.Cliloc(g_Language)->GetA(3000021, false, "That character is not old enough to delete. The character must be 7 days old before it can be deleted."),
				g_ClilocManager.Cliloc(g_Language)->GetA(3000022, false, "That character is currently queued for backup and cannot be deleted."),
				g_ClilocManager.Cliloc(g_Language)->GetA(3000023, false, "Couldn't carry out your request.")
			};
			int index = g_ConnectionScreen.GetErrorCode();
			if (index >= 6)
				index = 5;
			CreateText(189, 178, text[index], 2);

			Add(new CGUIButton(ID_CS_OK, 0x0481, 0x0482, 0x0483, 306, 304));
		}
		else
		{
			char buf[80] = { 0 };
			sprintf_s(buf, "Permanently delete %s?", g_CharacterList.GetSelectedName().c_str());

			CreateText(193, 184, buf, 2);

			Add(new CGUIButton(ID_CS_OK, 0x0481, 0x0482, 0x0483, 264, 304));
			Add(new CGUIButton(ID_CS_CANCEL, 0x047E, 0x047F, 0x0480, 348, 304));
		}
	}
	else if (g_ConnectionScreen.GetType() == CST_GAME)
	{
		if (g_ConnectionScreen.GetConnectionFailed())
		{
			const string text[3] =
			{
				g_ClilocManager.Cliloc(g_Language)->GetA(3000033, false, "Either the Account Name or Password you provided were incorrect. If this is a new account your account may not be active yet. Please try again shortly."),
				g_ClilocManager.Cliloc(g_Language)->GetA(3000612, false, "Your character name is too short."),
				"No character to login with."
			};
			int index = g_ConnectionScreen.GetErrorCode();
			if (index >= 3)
				index = 2;
			CreateText(189, 178, text[index], 2);

			Add(new CGUIButton(ID_CS_OK, 0x0481, 0x0482, 0x0483, 306, 304));
		}
		else
		{
			CreateText(189, 178, g_ClilocManager.Cliloc(g_Language)->GetA(3000001, false, "Entering Britannia..."), 2);

			g_ConnectionScreen.CursorGraphic = 0x2077; //Waiting mouse cursor
		}
	}
	else if (g_ConnectionScreen.GetType() == CST_GAME_LOGIN)
	{
		const string text[10] =
		{
			g_ClilocManager.Cliloc(g_Language)->GetA(3000007, false, "Incorrect password."),
			g_ClilocManager.Cliloc(g_Language)->GetA(3000009, false, "This character does not exist anymore.  You will have to recreate it."),
			g_ClilocManager.Cliloc(g_Language)->GetA(3000006, false, "This character already exists.\nPlaying..."),
			g_ClilocManager.Cliloc(g_Language)->GetA(3000016, false, "The client could not attach to the game server. It must have been taken down, please wait a few minutes and try again."),
			g_ClilocManager.Cliloc(g_Language)->GetA(3000017, false, "The client could not attach to the game server. It must have been taken down, please wait a few minutes and try again."),
			g_ClilocManager.Cliloc(g_Language)->GetA(3000012, false, "Another character from this account is currently online in this world.  You must either log in as that character or wait for it to time out."),
			g_ClilocManager.Cliloc(g_Language)->GetA(3000013, false, "An error has occurred in the synchronization between the login servers and this world.  Please close your client and try again."),
			g_ClilocManager.Cliloc(g_Language)->GetA(3000005, false, "You have been idle for too long.  If you do not do anything in the next minute, you will be logged out."),
			"",
			""
		};
		int index = g_ConnectionScreen.GetErrorCode();
		if (index >= 10)
			index = 9;
		CreateText(189, 178, text[index], 2);

		Add(new CGUIButton(ID_CS_OK, 0x0481, 0x0482, 0x0483, 306, 304));
	}
	else if (g_ConnectionScreen.GetType() == CST_SELECT_PROFESSOIN)
	{
		CreateText(189, 178, "You must have three unique skills choosen!", 2);

		Add(new CGUIButton(ID_CS_OK, 0x0481, 0x0482, 0x0483, 306, 304));
	}
	else if (g_ConnectionScreen.GetType() == CST_CONLOST)
	{
		Add(new CGUIResizepic(0, 0x0A28, 210, 178, 203, 121));

		CreateText(215, 222, "Connection lost", 1);

		Add(new CGUIButton(ID_CS_OK, 0x0481, 0x0482, 0x0483, 297, 257));

		g_ConnectionScreen.CursorGraphic = 0x2077; //Waiting mouse cursor
	}
	else
	{
		if (g_ConnectionScreen.GetConnectionFailed())
		{
			const string text[9] =
			{
				g_ClilocManager.Cliloc(g_Language)->GetA(3000007, false, "Incorrect name/password."),
				g_ClilocManager.Cliloc(g_Language)->GetA(3000034, false, "Someone is already using this account."),
				g_ClilocManager.Cliloc(g_Language)->GetA(3000035, false, "Your account has been blocked."),
				g_ClilocManager.Cliloc(g_Language)->GetA(3000036, false, "Your account credentials are invalid."),
				"Communication problem.",
				"The IGR concurrency limit has been met.",
				"The IGR time limit has been met.",
				"General IGR authentication failure.",
				g_ClilocManager.Cliloc(g_Language)->GetA(3000037, false, "Couldn't connect to Ultima Online.  Please try again in a few moments.")
			};
			int index = g_ConnectionScreen.GetErrorCode();
			if (index >= 9)
				index = 8;
			CreateText(189, 178, text[index], 2);

			Add(new CGUIButton(ID_CS_OK, 0x0481, 0x0482, 0x0483, 306, 304));
		}
		else
		{
			const string text[2] =
			{
				g_ClilocManager.Cliloc(g_Language)->GetA(3000002, false, "Connecting..."),
				g_ClilocManager.Cliloc(g_Language)->GetA(3000003, false, "Verifying Account...")
			};

			CreateText(189, 178, text[g_ConnectionScreen.GetConnected()], 2);

			g_ConnectionScreen.CursorGraphic = 0x2077; //Waiting mouse cursor
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpScreenConnection::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c113_f3");
	if (serial == ID_CS_OK) //v button
	{
		if (g_ConnectionScreen.GetType() == CST_CHARACTER_LIST)
		{
			if (!g_ConnectionScreen.GetConnectionFailed())
				g_ConnectionScreen.CreateSmoothAction(CConnectionScreen::ID_SMOOTH_CS_SEND_DELETE);
			else
				g_ConnectionScreen.CreateSmoothAction(CConnectionScreen::ID_SMOOTH_CS_GO_SCREEN_CHARACTER);
		}
		else if (g_ConnectionScreen.GetType() == CST_SELECT_PROFESSOIN)
			g_ConnectionScreen.CreateSmoothAction(CConnectionScreen::ID_SMOOTH_CS_GO_SCREEN_PROFESSION);
		else if (g_ConnectionScreen.GetType() == CST_GAME || g_ConnectionScreen.GetType() == CST_GAME_LOGIN)
			g_ConnectionScreen.CreateSmoothAction(CConnectionScreen::ID_SMOOTH_CS_GO_SCREEN_CHARACTER);
		else if (g_ConnectionScreen.GetType() == CST_CONLOST || g_ConnectionScreen.GetConnectionFailed())
			g_ConnectionScreen.CreateSmoothAction(CConnectionScreen::ID_SMOOTH_CS_GO_SCREEN_MAIN);
	}
	else if (serial == ID_CS_CANCEL) //Button x
		g_ConnectionScreen.CreateSmoothAction(CConnectionScreen::ID_SMOOTH_CS_GO_SCREEN_CHARACTER);
}
//----------------------------------------------------------------------------------
