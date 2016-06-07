/****************************************************************************
**
** GameConsole.cpp
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

TGameConsole *GameConsole = NULL;
//---------------------------------------------------------------------------
TGameConsole::TGameConsole()
: TEntryText(64, 600, 600)
{
}
//---------------------------------------------------------------------------
TGameConsole::~TGameConsole()
{
	if (EntryPointer == this)
		EntryPointer = NULL;
}
//---------------------------------------------------------------------------
void TGameConsole::Send()
{
	int len = Length();

	if (len)
	{
		SPEECH_TYPE speechType = ST_NORMAL;
		int offset = 0;

		if (len > 1)
		{
			int member = -1;
			IsSystemCommand(Data(), len, member);

			if ((m_Type != GCTT_NORMAL && len > 2) || m_Type == GCTT_PARTY)
			{
				if (m_Type == GCTT_YELL)
				{
					speechType = ST_YELL;
					offset = 2;
				}
				else if (m_Type == GCTT_WHISPER)
				{
					speechType = ST_WHISPER;
					offset = 2;
				}
				else if (m_Type == GCTT_EMOTE)
				{
					speechType = ST_EMOTE;
					offset = 2;
				}
				else if (m_Type == GCTT_PARTY)
				{
					DWORD serial = 0;
					offset = 1;

					if (member != -1)
						serial = Party.Member[member].Serial;

					if (Party.Leader != 0)
					{
						TPacketPartyMessage packet(Data() + offset, len - offset, serial);
						packet.Send();
						packet.Free();
					}
					else
					{
						string str = "Note to self: " + ToString(Data() + offset);
						UO->CreateTextMessage(TT_SYSTEM, 0, 3, 0, str);
					}

					return;
				}
				else if (m_Type == GCTT_PARTY_ACCEPT)
				{
					if (Party.Inviter != 0 && Party.Leader == 0)
					{
						TPacketPartyAccept packet(Party.Inviter);
						packet.Send();

						Party.Leader = Party.Inviter;
						Party.Inviter = 0;
					}
					else
						UO->CreateTextMessage(TT_SYSTEM, 0, 3, 0, "No one has invited you to be in a party.");

					return;
				}
				else if (m_Type == GCTT_PARTY_DECLINE)
				{
					if (Party.Inviter != 0 && Party.Leader == 0)
					{
						TPacketPartyDecline packet(Party.Inviter);
						packet.Send();

						Party.Leader = 0;
						Party.Inviter = 0;
					}
					else
						UO->CreateTextMessage(TT_SYSTEM, 0, 3, 0, "No one has invited you to be in a party.");

					return;
				}
			}

		}

		TPacketUnicodeSpeechRequest packet(Data() + offset, speechType, 3, ConfigManager.SpeechColor, (PBYTE)g_Language.c_str());
		packet.Send();
		packet.Free();
	}
}
//---------------------------------------------------------------------------
wstring TGameConsole::IsSystemCommand(const wchar_t *text, int &len, int &member)
{
	m_Type = GCTT_NORMAL;
	wstring result = L"";

	if (*text == m_ConsolePrefix[GCTT_PARTY][0]) //Party
	{
		string lStr = ToString(text);
		const char *cText = lStr.c_str();
		
		char *ptr = (char*)cText + 1;

		while (ptr < cText + len && *ptr == ' ')
			ptr++;

		if (ptr < cText + len)
		{
			int i = 0;

			sscanf(ptr, "%i", &i);

			if (i > 0 && i < 11) //Party mebmer
			{
				char pmBuf[50] = {0};

				if (Party.Member[i - 1].Serial != 0)
					sprintf(pmBuf, "Tell [%s]:", Party.Member[i - 1].GetName(i).c_str());
				else
					sprintf(pmBuf, "Tell []:");

				result = ToWString(pmBuf);

				m_Type = GCTT_PARTY;
				member = i - 1;
			}
		}

		if (m_Type == GCTT_NORMAL && len >= 7)
		{
			int lLen = 7;

			if (len > 7)
				lLen++;

			char lBuf[10] = {0};

			memcpy(&lBuf[0], cText, lLen);

			_strlwr(lBuf);

			if (!memcmp(&lBuf[0], "/accept", 7)) //Party accept
				m_Type = GCTT_PARTY_ACCEPT;
			else if (!memcmp(&lBuf[0], "/decline", 8)) //Party decline
				m_Type = GCTT_PARTY_DECLINE;
		}

		if (m_Type == GCTT_NORMAL && !result.length())
		{
			result = L"Party:";
			m_Type = GCTT_PARTY;
		}
	}
	else if (!memcmp(&text[0], m_ConsolePrefix[GCTT_YELL].c_str(), 4)) //Yell
	{
		result = L"Yell:";
		m_Type = GCTT_YELL;
	}
	else if (!memcmp(&text[0], m_ConsolePrefix[GCTT_WHISPER].c_str(), 4)) //Whisper
	{
		result = L"Whisper:";
		m_Type = GCTT_WHISPER;
	}
	else if (!memcmp(&text[0], m_ConsolePrefix[GCTT_EMOTE].c_str(), 4)) //Emote
	{
		result = L"Emote:";
		m_Type = GCTT_EMOTE;
	}
	else if (g_Player->Graphic == 0x03DB && (*text == L'=' || *text == m_ConsolePrefix[GCTT_C][0])) //C
	{
		result = L"C:";
		m_Type = GCTT_C;
	}
	else if (!memcmp(&text[0], m_ConsolePrefix[GCTT_BROADCAST].c_str(), 4)) //Broadcast
	{
		result = L"Broadcast:";
		m_Type = GCTT_BROADCAST;
	}

	return result;
}
//---------------------------------------------------------------------------
void TGameConsole::DrawW(BYTE font, WORD color, int x, int y, TEXT_ALIGN_TYPE align, WORD flags)
{
	int posOffset = 0;
	wstring wtext = Data();

	if (Changed || Color != color)
		FixMaxWidthW(font);
	
	int len = Length();
	const wchar_t *text = Data();

	if (len >= 2)
	{
		int member = 0;
		wstring sysStr = IsSystemCommand(text, len, member);

		if (sysStr.length())
		{
			posOffset = FontManager->GetWidthW(font, sysStr.c_str(), sysStr.length());

			wchar_t trimPart[2] = L" ";
			*trimPart = *text;
			posOffset -= FontManager->GetWidthW(font, trimPart, 1);

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

	if (this == EntryPointer)
	{
		x += (FontManager->GetWidthW(font, wtext.c_str(), Pos()) + posOffset);
		FontManager->DrawW(font, L"_", color, x, y, 30, 0, align, UOFONT_BLACK_BORDER);
	}
}
//---------------------------------------------------------------------------