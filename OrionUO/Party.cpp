// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** Party.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CParty g_Party;
//----------------------------------------------------------------------------------
CParty::CParty()
{
}
//----------------------------------------------------------------------------------
CParty::~CParty()
{
}
//----------------------------------------------------------------------------------
bool CParty::Contains(int serial)
{
	WISPFUN_DEBUG("c196_f1");
	bool result = false;

	if (Leader != 0)
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
//----------------------------------------------------------------------------------
void CParty::Clear()
{
	WISPFUN_DEBUG("c196_f2");
	IFOR(i, 0, 10)
	{
		Member[i].Serial = 0;
		Member[i].Character = NULL;
	}
		
}
//----------------------------------------------------------------------------------
void CParty::ParsePacketData(WISP_DATASTREAM::CDataReader &reader)
{
	WISPFUN_DEBUG("c196_f3");
	uchar code = reader.ReadUInt8();

	switch (code)
	{
		case 1: //Add member
		{
		}
		case 2: //Remove member
		{
			uchar count = reader.ReadUInt8();

			if (count <= 1)
			{
				Leader = 0;
				Inviter = 0;
				IFOR(i, 0, 10)
				{
					CPartyObject &member = Member[i];
					if (member.Character == NULL) break;
					CGumpStatusbar *gump = (CGumpStatusbar*)g_GumpManager.UpdateContent(member.Character->Serial, 0, GT_STATUSBAR);
					if (gump != NULL) gump->WantRedraw = true;
				}
				Clear();
				g_GumpManager.UpdateContent(0, 0, GT_PARTY_MANIFEST);
				break;
			}
			Clear();
			WISP_GEOMETRY::CPoint2Di oldPos = g_MouseManager.Position;
			WISP_GEOMETRY::CPoint2Di mousePos(76, 30);
			g_MouseManager.Position = mousePos;

			CGumpStatusbar *prevGump = NULL;

			IFOR(i, 0, count)
			{
				uint serial = reader.ReadUInt32BE();
				Member[i].Serial = serial;
				Member[i].Character = g_World->FindWorldCharacter(serial);
				if (i == 0)
					g_Party.Leader = serial;

				CGumpStatusbar *gump = (CGumpStatusbar*)g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR);

				if (gump == NULL)
				{
					g_Orion.OpenStatus(serial);
					
					gump = (CGumpStatusbar*)g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR);

					if (serial == g_PlayerSerial)
					{
						gump->Minimized = false;
					}						
						
					if (prevGump != NULL)
						prevGump->AddStatusbar(gump);

					prevGump = gump;

					mousePos.Y += 59;
					g_MouseManager.Position = mousePos;
				}
				else
				{
					CPacketStatusRequest(serial).Send();
					gump->WantRedraw = true;
				}			
			}
			
			g_MouseManager.Position = oldPos;
			
			g_GumpManager.UpdateContent(0, 0, GT_PARTY_MANIFEST);

			break;
		}
		case 3: //Private party message
		case 4: //Party message
		{
			uint serial = reader.ReadUInt32BE();
			wstring name = reader.ReadWString(0, true);

			IFOR(i, 0, 10)
			{
				if (Member[i].Serial == serial)
				{
					string str = "[" + Member[i].GetName((int)i) + "]: " + ToString(name);

					g_Orion.CreateTextMessage(TT_SYSTEM, serial, 3, g_ConfigManager.PartyMessageColor, str);

					break;
				}
			}

			break;
		}
		case 7: //Party invition
		{
			g_Party.Inviter = reader.ReadUInt32BE();

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
