/***********************************************************************************
**
** GumpScreenConnection.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GumpScreenConnection.h"
#include "../Screen stages/ConnectionScreen.h"
#include "../CharacterList.h"
//----------------------------------------------------------------------------------
CGumpScreenConnection::CGumpScreenConnection()
: CGump(GT_NONE, 0, 0, 0)
{
	m_NoMove = true;
	m_NoClose = true;
}
//----------------------------------------------------------------------------------
CGumpScreenConnection::~CGumpScreenConnection()
{
}
//----------------------------------------------------------------------------------
void CGumpScreenConnection::CreateText(const int &x, const int &y, string str, const uchar &font)
{
	if (g_ConnectionScreen.Message.length())
		str = g_ConnectionScreen.Message;

	CGUIText *obj = new CGUIText(0x0386, x, y);
	obj->CreateTextureA(font, str, 260, TS_CENTER);
	Add(obj);
}
//----------------------------------------------------------------------------------
void CGumpScreenConnection::UpdateContent()
{
	Clear();

	Add(new CGUIGumppicTiled(0x0588, 0, 0, 640, 480));
	Add(new CGUIGumppic(0x157C, 0, 0));
	Add(new CGUIGumppic(0x15A0, 0, 4));
	Add(new CGUIGumppic(0x1589, 555, 4));

	if (g_ConnectionScreen.Type != CST_CONLOST)
		Add(new CGUIResizepic(0, 0x0A28, 142, 134, 356, 212));

	g_ConnectionScreen.CursorGraphic = 0x2073; //Main Gump mouse cursor

	if (g_ConnectionScreen.Type == CST_CHARACTER_LIST)
	{
		if (g_ConnectionScreen.ConnectionFailed)
		{
			const char *text[6] =
			{
				"That character password is invalid.",
				"That character does not exist.",
				"That character is being played right now.",
				"That character is not old enough to delete. The character must be 7 days old before it can be deleted.",
				"That character is currently queued for backup and cannot be deleted.",
				"Couldn't carry out your request."
			};

			CreateText(189, 178, text[g_ConnectionScreen.ErrorCode], 2);

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
	else if (g_ConnectionScreen.Type == CST_GAME)
	{
		if (g_ConnectionScreen.ConnectionFailed)
		{
			const char *text[3] =
			{
				"Entering Britannia...",
				"Your character name is too short.",
				"No character to login with."
			};

			CreateText(189, 178, text[g_ConnectionScreen.ErrorCode], 2);

			Add(new CGUIButton(ID_CS_OK, 0x0481, 0x0482, 0x0483, 306, 304));
		}
		else
		{
			CreateText(189, 178, "Entering Britannia...", 2);

			g_ConnectionScreen.CursorGraphic = 0x2077; //Waiting mouse cursor
		}
	}
	else if (g_ConnectionScreen.Type == CST_GAME_LOGIN)
	{
		const char *text[10] =
		{
			"Incorrect password.",
			"This character does not exist anymore.  You will have to recreate it.",
			"This character already exists.\nPlaying...",
			"The client could not attach to the game server. It must have been taken down, please wait a few minutes and try again.",
			"The client could not attach to the game server. It must have been taken down, please wait a few minutes and try again.",
			"Another character from this account is currently online in this world.  You must either log in as that character or wait for it to time out.",
			"An error has occurred in the synchronization between the login servers and this world.  Please close your client and try again.",
			"You have been idle for too long.  If you do not do anything in the next minute, you will be logged out.",
			"",
			""
		};

		CreateText(189, 178, text[g_ConnectionScreen.ErrorCode], 2);

		Add(new CGUIButton(ID_CS_OK, 0x0481, 0x0482, 0x0483, 306, 304));
	}
	else if (g_ConnectionScreen.Type == CST_SELECT_PROFESSOIN)
	{
		CreateText(189, 178, "You must have three unique skills choosen!", 2);

		Add(new CGUIButton(ID_CS_OK, 0x0481, 0x0482, 0x0483, 306, 304));
	}
	else if (g_ConnectionScreen.Type == CST_CONLOST)
	{
		Add(new CGUIResizepic(0, 0x0A28, 210, 178, 203, 121));

		CreateText(215, 222, "Connection lost", 1);

		Add(new CGUIButton(ID_CS_OK, 0x0481, 0x0482, 0x0483, 297, 257));

		g_ConnectionScreen.CursorGraphic = 0x2077; //Waiting mouse cursor
	}
	else
	{
		if (g_ConnectionScreen.ConnectionFailed)
		{
			const char *text[9] =
			{
				"Incorrect name/password.",
				"Someone is already using this account.",
				"Your account has been blocked.",
				"Your account credentials are invalid.",
				"Communication problem.",
				"The IGR concurrency limit has been met.",
				"The IGR time limit has been met.",
				"General IGR authentication failure.",
				"Couldn't connect to Ultima Online.  Please try again in a few moments."
			};

			CreateText(189, 178, text[g_ConnectionScreen.ErrorCode], 2);

			Add(new CGUIButton(ID_CS_OK, 0x0481, 0x0482, 0x0483, 306, 304));
		}
		else
		{
			const char *text[2] =
			{
				"Connecting...",
				"Verifying Account..."
			};

			CreateText(189, 178, text[g_ConnectionScreen.Connected], 2);

			g_ConnectionScreen.CursorGraphic = 0x2077; //Waiting mouse cursor
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpScreenConnection::GUMP_BUTTON_EVENT_C
{
	if (serial == ID_CS_OK) //v button
	{
		if (g_ConnectionScreen.Type == CST_CHARACTER_LIST)
		{
			if (!g_ConnectionScreen.ConnectionFailed)
				g_ConnectionScreen.CreateSmoothAction(CConnectionScreen::ID_SMOOTH_CS_SEND_DELETE);
			else
				g_ConnectionScreen.CreateSmoothAction(CConnectionScreen::ID_SMOOTH_CS_GO_SCREEN_CHARACTER);
		}
		else if (g_ConnectionScreen.Type == CST_SELECT_PROFESSOIN)
			g_ConnectionScreen.CreateSmoothAction(CConnectionScreen::ID_SMOOTH_CS_GO_SCREEN_PROFESSION);
		else if (g_ConnectionScreen.Type == CST_GAME || g_ConnectionScreen.Type == CST_GAME_LOGIN)
			g_ConnectionScreen.CreateSmoothAction(CConnectionScreen::ID_SMOOTH_CS_GO_SCREEN_CHARACTER);
		else if (g_ConnectionScreen.Type == CST_CONLOST || g_ConnectionScreen.ConnectionFailed)
			g_ConnectionScreen.CreateSmoothAction(CConnectionScreen::ID_SMOOTH_CS_GO_SCREEN_MAIN);
	}
	else if (serial == ID_CS_CANCEL) //Button x
		g_ConnectionScreen.CreateSmoothAction(CConnectionScreen::ID_SMOOTH_CS_GO_SCREEN_CHARACTER);
}
//----------------------------------------------------------------------------------
