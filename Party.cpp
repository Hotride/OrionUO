/****************************************************************************
**
** Party.cpp
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
//----------------------------------------------------------------------------
#include "stdafx.h"

TParty Party;
//----------------------------------------------------------------------------
//---------------------------------TPartyObject-------------------------------
//----------------------------------------------------------------------------
TPartyObject::TPartyObject()
: m_Serial(0)
{
}
//----------------------------------------------------------------------------
//Get ordinary class property customized
#define GET_PARTY_BODY(type, name) \
type TPartyObject::Get##name() \
{ \
	type result = (type)0; \
	if (m_Serial) \
	{ \
		TGameCharacter *gc = World->FindWorldCharacter(m_Serial); \
		if (gc != NULL) \
			result = (type)gc-> ##name; \
	} \
	return result; \
}
//----------------------------------------------------------------------------
GET_PARTY_BODY(short, Hits)
GET_PARTY_BODY(short, MaxHits)
GET_PARTY_BODY(short, Mana)
GET_PARTY_BODY(short, MaxMana)
GET_PARTY_BODY(short, Stam)
GET_PARTY_BODY(short, MaxStam)
GET_PARTY_BODY(NOTORIETY_TYPE, Notoriety)
GET_PARTY_BODY(bool, CanChangeName)
//----------------------------------------------------------------------------
string TPartyObject::GetName(int index)
{
	if (m_Serial)
	{
		TGameCharacter *gc = World->FindWorldCharacter(m_Serial);

		if (gc != NULL)
			return gc->GetName();
	}

	char buf[10] = {0};
	sprintf(buf, "[%i]", index);

	return string(buf);
}
//----------------------------------------------------------------------------
//------------------------------------TParty----------------------------------
//----------------------------------------------------------------------------
TParty::TParty()
: m_Leader(0), m_Inviter(0), m_CanLoot(false)
{
}
//----------------------------------------------------------------------------
TParty::~TParty()
{
}
//----------------------------------------------------------------------------
bool TParty::Contains(const DWORD &serial)
{
	bool result = false;

	if (m_Leader != 0)
	{
		IFOR(i, 0, 10)
		{
			if (Member[i].Serial == serial)
			{
				result = true;

				break;
			}
		}
	}

	return result;
}
//----------------------------------------------------------------------------
void TParty::Clear()
{
	IFOR(i, 0, 10)
		Member[i].Serial = 0;;
}
//----------------------------------------------------------------------------
void TParty::ParsePacketData(PBYTE buf, const int &size)
{
	int pSize = size - 5;

	switch (*buf)
	{
		case 1: //Add member
		case 2: //Remove member
		{
			Clear();

			if (*buf == 1)
			{
				if (m_Leader == 0)
				{
					m_Leader = g_PlayerSerial;
					m_Inviter = 0;
				}
			}
			else if (buf[1] <= 1)
			{
				m_Leader = 0;
				m_Inviter = 0;

				break;
			}

			PBYTE ptr = buf + 2;
			
			int x = g_MouseX - 76;
			int y = g_MouseY - 30;
			
			g_MouseX = 76;
			g_MouseY = 30;

			TGumpStatusbar *prevGump = NULL;

			IFOR(i, 0, buf[1])
			{
				DWORD serial = unpack32(ptr);
				Member[i].Serial = serial;

				TGumpStatusbar *gump = (TGumpStatusbar*)GumpManager->GetGump(serial, 0, GT_STATUSBAR);
				if (gump == NULL)
				{
					UO->OpenStatus(serial);
					
					gump = (TGumpStatusbar*)GumpManager->GetGump(serial, 0, GT_STATUSBAR);

					if (serial == g_PlayerSerial)
						gump->Minimized = false;

					if (prevGump != NULL)
						prevGump->AddStatusbar(gump);

					prevGump = gump;

					g_MouseY += 59;
				}
				else
				{
					UO->StatusReq(serial);
					gump->FrameCreated = false;
				}

				ptr += 4;
			}
			
			g_MouseX = x;
			g_MouseY = y;
			
			GumpManager->UpdateGump(0, 0, GT_PARTY_MANIFEST);

			break;
		}
		case 3: //Private party message
		case 4: //Party message
		{
			DWORD serial = unpack32(buf + 1);

			PBYTE ptr = buf + 5;
			PBYTE end = buf + pSize;

			BYTE newstr[256] = {0};
			PBYTE newptr = newstr;

			while (ptr < end)
			{
				*newptr = ptr[1];
				newptr[1] = *ptr;

				ptr += 2;
			
				if (*((wchar_t*)newptr) == 0)
					break;

				newptr += 2;
			}

			IFOR(i, 0, 10)
			{
				if (Member[i].Serial == serial)
				{
					string str = "[" + Member[i].GetName(i) + "]: " + ToString((wchar_t*)newstr);

					UO->CreateTextMessage(TT_SYSTEM, serial, 3, ConfigManager.PartyMessageColor, str);

					break;
				}
			}

			break;
		}
		case 7: //Party invition
		{
			Party.Inviter = unpack32(buf + 1);

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------