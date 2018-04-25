// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GameConsole.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGameConsole g_GameConsole;
//----------------------------------------------------------------------------------
CGameConsole::CGameConsole()
: CEntryText(128, 1000, 1000)
{
}
//----------------------------------------------------------------------------------
CGameConsole::~CGameConsole()
{
	WISPFUN_DEBUG("c170_f1");
	if (g_EntryPointer == this)
		g_EntryPointer = NULL;
}
//----------------------------------------------------------------------------------
void CGameConsole::Send()
{
	WISPFUN_DEBUG("c170_f2");
	Send(Text);
}
//----------------------------------------------------------------------------------
void CGameConsole::Send(wstring text, ushort defaultColor)
{
	WISPFUN_DEBUG("c170_f2.1");
	size_t len = text.length();

	if (len)
	{
		SPEECH_TYPE speechType = ST_NORMAL;
		ushort sendColor = g_ConfigManager.SpeechColor;
		int offset = 0;

		if (len > 1)
		{
			int member = -1;
			GAME_CONSOLE_TEXT_TYPE type = GCTT_NORMAL;
			IsSystemCommand(text.c_str(), len, member, type);

			if ((type != GCTT_NORMAL && len > 2) || type == GCTT_PARTY)
			{
				if (type == GCTT_YELL)
				{
					speechType = ST_YELL;
					offset = 2;
				}
				else if (type == GCTT_WHISPER)
				{
					speechType = ST_WHISPER;
					offset = 2;
				}
				else if (type == GCTT_EMOTE)
				{
					text = text.replace(0, 2, L": *").append(L"*");
					speechType = ST_EMOTE;
					sendColor = g_ConfigManager.EmoteColor;
					offset = 2;
				}
				else if (type == GCTT_GUILD)
				{
					speechType = ST_GUILD_CHAT;
					sendColor = g_ConfigManager.GuildMessageColor;
					offset = 2;
				}
				else if (type == GCTT_ALLIANCE)
				{
					sendColor = g_ConfigManager.AllianceMessageColor;
					speechType = ST_ALLIANCE_CHAT;
					offset = 2;
				}
				else if (type == GCTT_PARTY)
				{
					uint serial = 0;
					offset = 1;
					sendColor = g_ConfigManager.PartyMessageColor;

					if (member != -1)
						serial = g_Party.Member[member].Serial;

					if (g_Party.Leader != 0)
						CPacketPartyMessage(text.c_str() + offset, len - offset, serial).Send();
					else
					{
						string str = "Note to self: " + ToString(text.c_str() + offset);
						g_Orion.CreateTextMessage(TT_SYSTEM, 0, 3, 0, str);
					}

					return;
				}
				else if (type == GCTT_PARTY_ACCEPT)
				{
					if (g_Party.Inviter != 0 && g_Party.Leader == 0)
					{
						CPacketPartyAccept(g_Party.Inviter).Send();

						g_Party.Leader = g_Party.Inviter;
						g_Party.Inviter = 0;
					}
					else
						g_Orion.CreateTextMessage(TT_SYSTEM, 0, 3, 0, "No one has invited you to be in a party.");

					return;
				}
				else if (type == GCTT_PARTY_DECLINE)
				{
					if (g_Party.Inviter != 0 && g_Party.Leader == 0)
					{
						CPacketPartyDecline(g_Party.Inviter).Send();

						g_Party.Leader = 0;
						g_Party.Inviter = 0;
					}
					else
						g_Orion.CreateTextMessage(TT_SYSTEM, 0, 3, 0, "No one has invited you to be in a party.");

					return;
				}
				else if (type == GCTT_PARTY_ADD)
				{
					if (g_Party.Leader == 0 || g_Party.Leader == g_PlayerSerial)
						CPacketPartyInviteRequest().Send();
					else
						g_Orion.CreateTextMessage(TT_SYSTEM, 0, 3, 0, "You are not party leader.");

					return;
				}
				else if (type == GCTT_PARTY_LEAVE)
				{
					if (g_Party.Leader != 0)
					{
						IFOR(i, 0, 10)
						{
							if (g_Party.Member[i].Serial != 0)
								CPacketPartyRemoveRequest(g_Party.Member[i].Serial).Send();
						}
					}
					else
						g_Orion.CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "You are not in a party.");

					return;
				}
			}

		}

		if (defaultColor)
			sendColor = defaultColor;

		CPacketUnicodeSpeechRequest(text.c_str() + offset, speechType, 3, sendColor, (puchar)g_Language.c_str()).Send();
	}
}
//----------------------------------------------------------------------------------
wstring CGameConsole::IsSystemCommand(const wchar_t *text, size_t &len, int &member, GAME_CONSOLE_TEXT_TYPE &type)
{
	WISPFUN_DEBUG("c170_f3");
	type = GCTT_NORMAL;
	wstring result = L"";

	if (*text == g_ConsolePrefix[GCTT_PARTY][0]) //Party
	{
		string lStr = ToString(text);
		const char *cText = lStr.c_str();
		
		char *ptr = (char*)cText + 1;

		while (ptr < cText + len && *ptr == ' ')
			ptr++;

		if (ptr < cText + len)
		{
			int i = 0;

			sscanf_s(ptr, "%i", &i);

			if (i > 0 && i < 11) //Party mebmer
			{
				char pmBuf[50] = {0};

				if (g_Party.Member[i - 1].Serial != 0)
					sprintf_s(pmBuf, "Tell [%s]:", g_Party.Member[i - 1].GetName(i).c_str());
				else
					sprintf_s(pmBuf, "Tell []:");

				result = ToWString(pmBuf);

				type = GCTT_PARTY;
				member = i - 1;
			}
		}

		if (type == GCTT_NORMAL && len >= 4 && len <= 8)
		{
			size_t lLen = 4;

			if (len > 4)
				lLen = len;

			char lBuf[10] = {0};

			memcpy(&lBuf[0], cText, lLen);

			_strlwr(lBuf);

			if (!memcmp(&lBuf[0], "/add", 4)) //Party add
				type = GCTT_PARTY_ADD;
			else if ((!memcmp(&lBuf[0], "/quit", 5)) || (!memcmp(&lBuf[0], "/leave", 6))) //Party leave & quit
				type = GCTT_PARTY_LEAVE;
			else if (!memcmp(&lBuf[0], "/accept", 7)) //Party accept
				type = GCTT_PARTY_ACCEPT;
			else if (!memcmp(&lBuf[0], "/decline", 8)) //Party decline
				type = GCTT_PARTY_DECLINE;
		}
		
		if ((type == GCTT_PARTY_ADD || type == GCTT_PARTY_LEAVE || type == GCTT_PARTY_ACCEPT || type == GCTT_PARTY_DECLINE) && !result.length())
			result = L"Party:";
		
		if (type == GCTT_NORMAL && !result.length())
		{
			result = L"Party:";
			type = GCTT_PARTY;
		}
	}
	else if (!memcmp(&text[0], g_ConsolePrefix[GCTT_YELL].c_str(), 4)) //Yell
	{
		result = L"Yell:";
		type = GCTT_YELL;
	}
	else if (!memcmp(&text[0], g_ConsolePrefix[GCTT_WHISPER].c_str(), 4)) //Whisper
	{
		result = L"Whisper:";
		type = GCTT_WHISPER;
	}
	else if (!memcmp(&text[0], g_ConsolePrefix[GCTT_EMOTE].c_str(), 4)) //Emote
	{
		result = L"Emote:";
		type = GCTT_EMOTE;
	}
	else if (g_Player->Graphic == 0x03DB && (*text == L'=' || *text == g_ConsolePrefix[GCTT_C][0])) //C
	{
		result = L"C:";
		type = GCTT_C;
	}
	else if (!memcmp(&text[0], g_ConsolePrefix[GCTT_BROADCAST].c_str(), 4)) //Broadcast
	{
		result = L"Broadcast:";
		type = GCTT_BROADCAST;
	}
	else if (!memcmp(&text[0], g_ConsolePrefix[GCTT_GUILD].c_str(), 4)) //Guild
	{
		result = L"Guild:";
		type = GCTT_GUILD;
	}
	else if (!memcmp(&text[0], g_ConsolePrefix[GCTT_ALLIANCE].c_str(), 4)) //Alliance
	{
		result = L"Alliance:";
		type = GCTT_ALLIANCE;
	}

	return result;
}
//----------------------------------------------------------------------------------
void CGameConsole::DrawW(BYTE font, WORD color, int x, int y, TEXT_ALIGN_TYPE align, WORD flags)
{
	WISPFUN_DEBUG("c170_f4");
	int posOffset = 0;
	wstring wtext = Data();

	if (Changed || Color != color)
		FixMaxWidthW(font);
	
	size_t len = Length();
	const wchar_t *text = Data();

	if (len >= 2)
	{
		int member = 0;
		wstring sysStr = IsSystemCommand(text, len, member, m_Type);

		if (sysStr.length())
		{
			posOffset = g_FontManager.GetWidthW(font, sysStr);

			wchar_t trimPart[2] = L" ";
			*trimPart = *text;
			posOffset -= g_FontManager.GetWidthW(font, trimPart);

			sysStr += (text + 1);

			wtext = sysStr;
		}
	}

	if (Changed || Color != color)
	{
		CheckMaxWidthW(font, wtext);

		CreateTextureW(font, wtext, color, Width, align, flags);

		Changed = false;
		Color = color;
	}

	m_Texture.Draw(x, y);

	if (this == g_EntryPointer)
	{
		if (m_Position)
		{
			wtext.resize(m_Position);
			x += (g_FontManager.GetWidthW(font, wtext) + posOffset);
		}

		g_FontManager.DrawW(font, L"_", color, x, y, 30, 0, align, UOFONT_BLACK_BORDER);
	}
}
//----------------------------------------------------------------------------------
void CGameConsole::SaveConsoleMessage()
{
	WISPFUN_DEBUG("c170_f5");
	if (m_ConsoleStack[m_ConsoleSelectedIndex % MAX_CONSOLE_STACK_SIZE] != Text)
	{
		m_ConsoleStack[m_ConsoleStackCount % MAX_CONSOLE_STACK_SIZE] = Text;
		m_ConsoleStackCount++;

		if (m_ConsoleStackCount > 1100)
			m_ConsoleStackCount -= 1000;
	}

	m_ConsoleSelectedIndex = (m_ConsoleStackCount - 1) % MAX_CONSOLE_STACK_SIZE;
	m_PositionChanged = false;
}
//----------------------------------------------------------------------------------
void CGameConsole::ChangeConsoleMessage(bool next)
{
	WISPFUN_DEBUG("c170_f6");
	if (m_ConsoleStackCount)
	{
		if (m_PositionChanged)
		{
			if (next)
			{
				m_ConsoleSelectedIndex = (m_ConsoleSelectedIndex + 1) % MAX_CONSOLE_STACK_SIZE;

				if (m_ConsoleSelectedIndex >= m_ConsoleStackCount)
					m_ConsoleSelectedIndex = 0;
			}
			else
			{
				m_ConsoleSelectedIndex--;

				if (m_ConsoleSelectedIndex < 0)
					m_ConsoleSelectedIndex = (m_ConsoleStackCount - 1) % MAX_CONSOLE_STACK_SIZE;
			}
		}
		else if (next)
			m_ConsoleSelectedIndex = 0;

		SetText(m_ConsoleStack[m_ConsoleSelectedIndex]);
		SetPos((int)m_ConsoleStack[m_ConsoleSelectedIndex].length());
		m_PositionChanged = true;
	}
}
//----------------------------------------------------------------------------------
void CGameConsole::ClearStack()
{
	WISPFUN_DEBUG("c170_f7");
	m_ConsoleStack[0] = L"";
	m_ConsoleStackCount = 0;
	m_ConsoleSelectedIndex = 0;
	m_PositionChanged = false;
}
//----------------------------------------------------------------------------------
