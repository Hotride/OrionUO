/****************************************************************************
**
** GumpPartyManifest.cpp
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
TTextTexture TGumpPartyManifest::m_Text[9];
//---------------------------------------------------------------------------
TGumpPartyManifest::TGumpPartyManifest(DWORD serial, short x, short y, bool canLoot)
: TGump(GT_PARTY_MANIFEST, serial, x, y), m_CanLoot(canLoot)
{
}
//---------------------------------------------------------------------------
TGumpPartyManifest::~TGumpPartyManifest()
{
}
//---------------------------------------------------------------------------
void TGumpPartyManifest::InitTextTextures()
{
	FontManager->GenerateA(1, m_Text[0], "Tell", 0x0386);
	FontManager->GenerateA(1, m_Text[1], "Kick", 0x0386);
	FontManager->GenerateA(2, m_Text[2], "Party Manifest", 0x0386);
	FontManager->GenerateA(2, m_Text[3], "Send the party a message", 0x0386);
	FontManager->GenerateA(2, m_Text[4], "Party can loot me", 0x0386);
	FontManager->GenerateA(2, m_Text[5], "Party CANNOT loot me", 0x0386);
	FontManager->GenerateA(2, m_Text[6], "Disband the party", 0x0386);
	FontManager->GenerateA(2, m_Text[7], "Leave the party", 0x0386);
	FontManager->GenerateA(2, m_Text[8], "Add New Member", 0x0386);
}
//---------------------------------------------------------------------------
void TGumpPartyManifest::ReleaseTextTextures()
{
	IFOR(i, 0, 9)
		m_Text[i].Clear();
}
//---------------------------------------------------------------------------
void TGumpPartyManifest::PrepareTextures()
{
	UO->ExecuteResizepic(0x0A28);
	UO->ExecuteButton(0x0FAB);
	UO->ExecuteButton(0x0FB1);
	UO->ExecuteGump(0x0475);
	UO->ExecuteGump(0x0FA2);
	UO->ExecuteGump(0x0FA9);
	UO->ExecuteButton(0x00F1);
	UO->ExecuteButton(0x00F7);
	UO->ExecuteButton(0x0FAE);
	UO->ExecuteButton(0x0FA8);
}
//---------------------------------------------------------------------------
void TGumpPartyManifest::GenerateFrame()
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	CalculateGumpState();

	glNewList((GLuint)this, GL_COMPILE);

		UO->DrawResizepicGump(0x0A28, 0, 0, 450, 480); //Body
		
		m_Text[0].Draw(40, 30); //Tell
		m_Text[1].Draw(80, 30); //Kick
		m_Text[2].Draw(153, 20); //Party Manifest
		
		bool isLeader = (Party.Leader == 0 || Party.Leader == g_PlayerSerial);
		bool isMember = (Party.Leader != 0 && Party.Leader != g_PlayerSerial);

		int yPtr = 48;
		WORD gumpID = 0;

		IFOR(i, 0, 10)
		{
			gumpID = 0x0FAB;
			if (g_GumpPressedElement - ID_GPM_BUTTON_TELL_MEMBER == i)
				gumpID += 2;
			else if (g_GumpSelectElement - ID_GPM_BUTTON_TELL_MEMBER == i)
				gumpID++;

			UO->DrawGump(gumpID, 0, 40, yPtr + 2);

			if (isLeader)
			{
				gumpID = 0x0FB1;
				if (g_GumpPressedElement - ID_GPM_BUTTON_KICK_MEMBER == i)
					gumpID += 2;
				else if (g_GumpSelectElement - ID_GPM_BUTTON_KICK_MEMBER == i)
					gumpID++;

				UO->DrawGump(gumpID, 0, 80, yPtr + 2);
			}
			
			UO->DrawGump(0x0475, 0, 130, yPtr);

			string name = Party.Member[i].GetName(i + 1);
			FontManager->DrawA(2, name.c_str(), 0x0386, 140, yPtr + 1, 250, TS_CENTER);

			yPtr += 25;
		}
		
		gumpID = 0x0FAB + (int)(g_GumpSelectElement == ID_GPM_BUTTON_SEND_MESSAGE);
		if (g_GumpPressedElement == ID_GPM_BUTTON_SEND_MESSAGE)
			gumpID = 0x0FAD;

		UO->DrawGump(gumpID, 0, 70, 307);
		m_Text[3].Draw(110, 307); //Send the party a message
		
		gumpID = m_CanLoot ? 0x0FA2 : 0x0FA9;
		UO->DrawGump(gumpID, 0, 70, 334);
		m_Text[4 + (int)m_CanLoot].Draw(110, 334); //Party can/CANNOT loot me
		
		gumpID = 0x0FAE + (int)(g_GumpSelectElement == ID_GPM_BUTTON_LEAVE);
		if (g_GumpPressedElement == ID_GPM_BUTTON_LEAVE)
			gumpID = 0x0FB0;

		UO->DrawGump(gumpID, 0, 70, 360);
		m_Text[6 + (int)isMember].Draw(110, 360); //Disband/Leave the party
		
		if (isLeader)
		{
			gumpID = 0x0FA8 + (int)(g_GumpSelectElement == ID_GPM_BUTTON_ADD);
			if (g_GumpPressedElement == ID_GPM_BUTTON_ADD)
				gumpID = 0x0FAA;

			UO->DrawGump(gumpID, 0, 70, 385);
			m_Text[8].Draw(110, 385); //Add New Member
		}
		
		if (g_GumpPressedElement == ID_GPM_BUTTON_OKAY)
			gumpID = 0x00F8; //Okay button (down)
		else if (g_GumpSelectElement == ID_GPM_BUTTON_OKAY)
			gumpID = 0x00F7; //Okay button selected
		else
			gumpID = 0x00F9; //Okay button
		UO->DrawGump(gumpID, 0, 130, 430);
		
		if (g_GumpPressedElement == ID_GPM_BUTTON_CANCEL)
			gumpID = 0x00F1; //Cancel button (down)
		else if (g_GumpSelectElement == ID_GPM_BUTTON_CANCEL)
			gumpID = 0x00F2; //Cancel button selected
		else
			gumpID = 0x00F3; //Cancel button
		UO->DrawGump(gumpID, 0, 236, 430);

	glEndList();
	
	m_FrameRedraw = true;
	m_FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpPartyManifest::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	CalculateGumpState();

	if (mode)
	{
		if (!m_FrameCreated || g_GumpSelectElement) // || g_GumpPressed
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

		//Если выбран основной гамп - меняем глобальный указатель на выбранный гамп на него
		if (UO->ResizepicPixelsInXY(0x0A28, 0, 0, 450, 480))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;

			bool isLeader = (Party.Leader == 0 || Party.Leader == g_PlayerSerial);
			
			int yPtr = 50;
			WORD gumpID = 0;

			IFOR(i, 0, 10)
			{
				if (UO->GumpPixelsInXY(0x0FAB, 40, yPtr))
					LSG = ID_GPM_BUTTON_TELL_MEMBER + i; //Tell

				if (isLeader)
				{
					if (UO->GumpPixelsInXY(0x0FB1, 80, yPtr))
						LSG = ID_GPM_BUTTON_KICK_MEMBER + i; //Kick
				}

				yPtr += 25;
			}

			gumpID = m_CanLoot ? 0x0FA2 : 0x0FA9;

			if (UO->GumpPixelsInXY(0x0FAB, 70, 307))
				LSG = ID_GPM_BUTTON_SEND_MESSAGE; //Send the party a message
			else if (UO->GumpPixelsInXY(gumpID, 70, 334))
				LSG = ID_GPM_BUTTON_LOOT_TYPE; //Party can/CANNOT loot me
			else if (UO->GumpPixelsInXY(0x0FAE, 70, 360))
				LSG = ID_GPM_BUTTON_LEAVE; //Disband/Leave the party
			else if (isLeader && UO->GumpPixelsInXY(0x0FA8, 70, 385))
				LSG = ID_GPM_BUTTON_ADD; //Add New Member
			else if (UO->GumpPixelsInXY(0x00F9, 130, 430))
				LSG = ID_GPM_BUTTON_OKAY; //Okay button
			else if (UO->GumpPixelsInXY(0x00F3, 236, 430))
				LSG = ID_GPM_BUTTON_CANCEL; //Cancel button

			if (LSG != 0)
				g_LastSelectedObject = LSG; //Если что-то нашлось - выбираем
		}

		g_MouseX = oldMouseX;
		g_MouseY = oldMouseY;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpPartyManifest::OnLeftMouseUp()
{
	if (g_LastObjectLeftMouseDown != g_LastSelectedObject)
		return;
	
	if (g_LastObjectLeftMouseDown == ID_GPM_BUTTON_OKAY)
	{
		if (Party.Leader != 0 && Party.CanLoot != m_CanLoot)
		{
			Party.CanLoot = m_CanLoot;

			TPacketPartyChangeLootTypeRequest packet((BYTE)m_CanLoot);
			packet.Send();
		}

		GumpManager->RemoveGump(this);
	}
	else if (g_LastObjectLeftMouseDown == ID_GPM_BUTTON_CANCEL)
		GumpManager->RemoveGump(this);
	else if (g_LastObjectLeftMouseDown == ID_GPM_BUTTON_SEND_MESSAGE)
	{
		if (Party.Leader == 0)
			UO->CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "You are not in a party.");
		else
			GameConsole->SetText(L"/");
	}
	else if (g_LastObjectLeftMouseDown == ID_GPM_BUTTON_LOOT_TYPE)
		m_CanLoot = !m_CanLoot;
	else if (g_LastObjectLeftMouseDown == ID_GPM_BUTTON_LEAVE)
	{
		if (Party.Leader == 0)
			UO->CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "You are not in a party.");
		else
		{
//???????????????
			IFOR(i, 0, 10)
			{
				if (Party.Member[i].Serial != 0)
				{
					TPacketPartyRemoveRequest packet(Party.Member[i].Serial);
					packet.Send();
				}
			}
//???????????????
		}
	}
	else if (g_LastObjectLeftMouseDown == ID_GPM_BUTTON_ADD)
	{
		if (Party.Leader == 0 || Party.Leader == g_PlayerSerial)
		{
			TPacketPartyInviteRequest packet;
			packet.Send();
		}
	}
	else if (g_LastObjectLeftMouseDown >= ID_GPM_BUTTON_TELL_MEMBER && g_LastObjectLeftMouseDown < ID_GPM_BUTTON_KICK_MEMBER)
	{
		int memberIndex = g_LastObjectLeftMouseDown - ID_GPM_BUTTON_TELL_MEMBER;
		
		if (Party.Member[memberIndex].Serial == 0)
			UO->CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "There is no one in that party slot.");
		else
		{
			char buf[10] = {0};
			sprintf(buf, "/%i ", memberIndex + 1);
			GameConsole->SetText(buf);
		}
	}
	else if (g_LastObjectLeftMouseDown >= ID_GPM_BUTTON_KICK_MEMBER)
	{
		int memberIndex = g_LastObjectLeftMouseDown - ID_GPM_BUTTON_KICK_MEMBER;
		
		if (Party.Member[memberIndex].Serial == 0)
			UO->CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "There is no one in that party slot.");
		else
		{
			TPacketPartyRemoveRequest packet(Party.Member[memberIndex].Serial);
			packet.Send();
		}
	}
}
//---------------------------------------------------------------------------