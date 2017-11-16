// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GumpPartyManifest.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpPartyManifest::CGumpPartyManifest(uint serial, short x, short y, bool canLoot)
: CGump(GT_PARTY_MANIFEST, serial, x, y), m_CanLoot(canLoot)
{
}
//----------------------------------------------------------------------------------
CGumpPartyManifest::~CGumpPartyManifest()
{
}
//----------------------------------------------------------------------------------
void CGumpPartyManifest::UpdateContent()
{
	WISPFUN_DEBUG("c106_f1");
	Clear();

	Add(new CGUIResizepic(0, 0x0A28, 0, 0, 450, 480));

	CGUIText *text = (CGUIText*)Add(new CGUIText(0x0386, 40, 30));
	text->CreateTextureA(1, "Tell");

	text = (CGUIText*)Add(new CGUIText(0x0386, 80, 30));
	text->CreateTextureA(1, "Kick");

	text = (CGUIText*)Add(new CGUIText(0x0386, 153, 20));
	text->CreateTextureA(2, "Party Manifest");

	bool isLeader = (g_Party.Leader == 0 || g_Party.Leader == g_PlayerSerial);
	bool isMember = (g_Party.Leader != 0 && g_Party.Leader != g_PlayerSerial);

	int yPtr = 48;
	ushort gumpID = 0;

	IFOR(i, 0, 10)
	{
		Add(new CGUIButton(ID_GPM_BUTTON_TELL_MEMBER + (int)i, 0x0FAB, 0x0FAC, 0x0FAD, 40, yPtr + 2));

		if (isLeader)
			Add(new CGUIButton(ID_GPM_BUTTON_KICK_MEMBER + (int)i, 0x0FB1, 0x0FB2, 0x0FB3, 80, yPtr + 2));

		Add(new CGUIGumppic(0x0475, 130, yPtr));

		text = (CGUIText*)Add(new CGUIText(0x0386, 140, yPtr + 1));
		text->CreateTextureA(2, g_Party.Member[i].GetName((int)i + 1), 250, TS_CENTER);

		yPtr += 25;
	}

	Add(new CGUIButton(ID_GPM_BUTTON_SEND_MESSAGE, 0x0FAB, 0x0FAC, 0x0FAD, 70, 307));

	text = (CGUIText*)Add(new CGUIText(0x0386, 110, 307));
	text->CreateTextureA(2, "Send the party a message");

	if (m_CanLoot)
	{
		Add(new CGUIButton(ID_GPM_BUTTON_LOOT_TYPE, 0x0FA2, 0x0FA2, 0x0FA2, 70, 334));

		text = (CGUIText*)Add(new CGUIText(0x0386, 110, 334));
		text->CreateTextureA(2, "Party can loot me");
	}
	else
	{
		Add(new CGUIButton(ID_GPM_BUTTON_LOOT_TYPE, 0x0FA9, 0x0FA9, 0x0FA9, 70, 334));

		text = (CGUIText*)Add(new CGUIText(0x0386, 110, 334));
		text->CreateTextureA(2, "Party CANNOT loot me");
	}

	Add(new CGUIButton(ID_GPM_BUTTON_LEAVE, 0x0FAE, 0x0FAF, 0x0FB0, 70, 360));

	if (isMember)
	{
		text = (CGUIText*)Add(new CGUIText(0x0386, 110, 360));
		text->CreateTextureA(2, "Leave the party");
	}
	else
	{
		text = (CGUIText*)Add(new CGUIText(0x0386, 110, 360));
		text->CreateTextureA(2, "Disband the party");
	}
	
	if (isLeader)
	{
		Add(new CGUIButton(ID_GPM_BUTTON_ADD, 0x0FA8, 0x0FA9, 0x0FAA, 70, 385));

		text = (CGUIText*)Add(new CGUIText(0x0386, 110, 385));
		text->CreateTextureA(2, "Add New Member");
	}

	Add(new CGUIButton(ID_GPM_BUTTON_OKAY, 0x00F9, 0x00F7, 0x00F8, 130, 430));
	Add(new CGUIButton(ID_GPM_BUTTON_CANCEL, 0x00F3, 0x00F2, 0x00F1, 236, 430));
}
//----------------------------------------------------------------------------------
void CGumpPartyManifest::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c106_f2");
	if (serial == ID_GPM_BUTTON_OKAY)
	{
		if (g_Party.Leader != 0 && g_Party.CanLoot != m_CanLoot)
		{
			g_Party.CanLoot = m_CanLoot;

			CPacketPartyChangeLootTypeRequest((uchar)m_CanLoot).Send();
		}

		m_RemoveMark = true;
	}
	else if (serial == ID_GPM_BUTTON_CANCEL)
		m_RemoveMark = true;
	else if (serial == ID_GPM_BUTTON_SEND_MESSAGE)
	{
		if (g_Party.Leader == 0)
			g_Orion.CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "You are not in a party.");
		else
			g_GameConsole.SetText(L"/");
	}
	else if (serial == ID_GPM_BUTTON_LOOT_TYPE)
	{
		m_CanLoot = !m_CanLoot;
		m_WantUpdateContent = true;
	}
	else if (serial == ID_GPM_BUTTON_LEAVE)
	{
		if (g_Party.Leader == 0)
			g_Orion.CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "You are not in a party.");
		else
		{
//???????????????
			IFOR(i, 0, 10)
			{
				if (g_Party.Member[i].Serial != 0)
					CPacketPartyRemoveRequest(g_Party.Member[i].Serial).Send();
			}
//???????????????
		}
	}
	else if (serial == ID_GPM_BUTTON_ADD)
	{
		if (g_Party.Leader == 0 || g_Party.Leader == g_PlayerSerial)
			CPacketPartyInviteRequest().Send();
	}
	else if (serial >= ID_GPM_BUTTON_TELL_MEMBER && serial < ID_GPM_BUTTON_KICK_MEMBER)
	{
		int memberIndex = serial - ID_GPM_BUTTON_TELL_MEMBER;
		
		if (g_Party.Member[memberIndex].Serial == 0)
			g_Orion.CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "There is no one in that party slot.");
		else
		{
			char buf[10] = {0};
			sprintf_s(buf, "/%i ", memberIndex + 1);
			g_GameConsole.SetText(buf);
		}
	}
	else if (serial >= ID_GPM_BUTTON_KICK_MEMBER)
	{
		int memberIndex = serial - ID_GPM_BUTTON_KICK_MEMBER;
		
		if (g_Party.Member[memberIndex].Serial == 0)
			g_Orion.CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "There is no one in that party slot.");
		else
			CPacketPartyRemoveRequest(g_Party.Member[memberIndex].Serial).Send();
	}
}
//----------------------------------------------------------------------------------
