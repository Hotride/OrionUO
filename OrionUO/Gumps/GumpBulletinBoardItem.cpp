/***********************************************************************************
**
** GumpBulletinBoardItem.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpBulletinBoardItem::CGumpBulletinBoardItem(uint serial, short x, short y, uchar variant, uint id, string poster, string subject, string dataTime, string data)
: CGumpBaseScroll(GT_BULLETIN_BOARD_ITEM, serial, 0x0820, 250, x, y, false, 70),
m_Variant(variant)
{
	WISPFUN_DEBUG("c90_f1");
	m_ID = id;
	m_MinHeight = 200;

	CGUIText *text = (CGUIText*)Add(new CGUIText(0, 30, 40));
	text->CreateTextureA(6, "Author:");

	CGUIText *text2 = (CGUIText*)Add(new CGUIText(0x0386, 30 + text->m_Texture.Width, 46));
	text2->CreateTextureA(9, poster);

	text = (CGUIText*)Add(new CGUIText(0, 30, 56));
	text->CreateTextureA(6, "Time:");

	text2 = (CGUIText*)Add(new CGUIText(0x0386, 30 + text->m_Texture.Width, 62));
	text2->CreateTextureA(9, dataTime);

	text = (CGUIText*)Add(new CGUIText(0, 30, 72));
	text->CreateTextureA(6, "Subject:");

	ushort subjectColor = 0x0386;

	if (!m_Variant)
		subjectColor = 0x0008;

	Add(new CGUIHitBox(ID_GBBI_SUBJECT_TEXT_FIELD, 30 + text->m_Texture.Width, 78, 160, 16));

	m_EntrySubject = (CGUITextEntry*)Add(new CGUITextEntry(ID_GBBI_SUBJECT_TEXT_FIELD, subjectColor, subjectColor, subjectColor, 30 + text->m_Texture.Width, 78, 150, false, 9));
	m_EntrySubject->m_Entry.SetText(subject);

	if (!m_Variant)
	{
		CGump *gumpEntry = g_GumpManager.GetTextEntryOwner();

		if (gumpEntry != NULL)
			gumpEntry->WantRedraw = true;

		g_EntryPointer = &m_EntrySubject->m_Entry;
	}

	Add(new CGUIGumppicTiled(0x0835, 30, 100, 204, 0));

	m_Entry = (CGUITextEntry*)m_HTMLGump->Add(new CGUITextEntry(ID_GBBI_TEXT_FIELD, 0x0386, 0x0386, 0x0386, 3, 3, 220, false, 9));
	m_Entry->m_Entry.MaxWidth = 0;
	m_Entry->m_Entry.SetText(data);
	m_Entry->m_Entry.CreateTextureA(9, m_Entry->m_Entry.c_str(), 0x0386, 220, TS_LEFT, 0);
	m_HitBox = (CGUIHitBox*)m_HTMLGump->Add(new CGUIHitBox(ID_GBBI_TEXT_FIELD, 3, 3, 220, m_Entry->m_Entry.m_Texture.Height));

	if (m_HitBox->Height < 14)
		m_HitBox->Height = 14;

	m_HTMLGump->CalculateDataSize();

	m_ButtonPost = NULL;
	m_ButtonRemove = NULL;
	m_ButtonReply = NULL;

	switch (m_Variant)
	{
		case 0:
		{
			Add(new CGUIGumppic(0x0883, 97, 12)); //NEW MESSAGE
			m_ButtonPost = (CGUIButton*)Add(new CGUIButton(ID_GBBI_POST, 0x0886, 0x0886, 0x0886, 37, m_Height - 22)); //Post
			m_ButtonPost->CheckPolygone = true;

			break;
		}
		case 2:
			m_ButtonRemove = (CGUIButton*)Add(new CGUIButton(ID_GBBI_REMOVE, 0x0885, 0x0885, 0x0885, 235, m_Height - 22)); //Remove
			m_ButtonRemove->CheckPolygone = true;
		case 1:
		{
			m_ButtonReply = (CGUIButton*)Add(new CGUIButton(ID_GBBI_REPLY, 0x0884, 0x0884, 0x0884, 37, m_Height - 22)); //Reply
			m_ButtonReply->CheckPolygone = true;

			m_EntrySubject->ReadOnly = true;
			m_Entry->ReadOnly = true;

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
CGumpBulletinBoardItem::~CGumpBulletinBoardItem()
{
}
//----------------------------------------------------------------------------------
void CGumpBulletinBoardItem::UpdateHeight()
{
	WISPFUN_DEBUG("c90_f2");
	CGumpBaseScroll::UpdateHeight();

	if (m_ButtonPost != NULL)
		m_ButtonPost->Y = m_Height - 22; //Post

	if (m_ButtonRemove != NULL)
		m_ButtonRemove->Y = m_Height - 22; //Remove

	if (m_ButtonRemove != NULL)
		m_ButtonReply->Y = m_Height - 22; //Reply
}
//----------------------------------------------------------------------------------
void CGumpBulletinBoardItem::RecalculateHeight()
{
	WISPFUN_DEBUG("c90_f3");
	if (g_EntryPointer == &m_Entry->m_Entry)
	{
		m_Entry->m_Entry.CreateTextureA(9, m_Entry->m_Entry.c_str(), 0x0386, 220, TS_LEFT, 0);
		m_HitBox->Height = m_Entry->m_Entry.m_Texture.Height;

		if (m_HitBox->Height < 14)
			m_HitBox->Height = 14;

		m_HTMLGump->CalculateDataSize();
	}
}
//----------------------------------------------------------------------------------
void CGumpBulletinBoardItem::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c90_f4");
	if (m_EntrySubject != NULL)
	{
		if (serial == ID_GBBI_POST)
		{
			CPacketBulletinBoardPostMessage(m_ID, 0, m_EntrySubject->m_Entry.c_str(), m_Entry->m_Entry.c_str()).Send();

			m_RemoveMark = true;
		}
		else if (serial == ID_GBBI_REPLY)
		{
			string subj("RE: ");
			subj += m_EntrySubject->m_Entry.c_str();

			CGumpBulletinBoardItem *gump = new CGumpBulletinBoardItem(0, 0, 0, 0, m_ID, g_Player->Name, subj, "Date/Time", "");

			g_GumpManager.AddGump(gump);
		}
		else if (serial == ID_GBBI_REMOVE)
		{
			CPacketBulletinBoardRemoveMessage(m_ID, m_Serial).Send();

			//GumpManager->CloseGump(Serial, ID, GT_BULLETIN_BOARD_ITEM);
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpBulletinBoardItem::OnCharPress(const WPARAM &wParam, const LPARAM &lParam)
{
	WISPFUN_DEBUG("c90_f5");
	g_EntryPointer->Insert(wParam);

	RecalculateHeight();
	m_WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGumpBulletinBoardItem::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
	WISPFUN_DEBUG("c90_f6");
	if (wParam == VK_RETURN && m_Entry != NULL && g_EntryPointer == &m_Entry->m_Entry)
	{
		g_EntryPointer->Insert(L'\n');

		RecalculateHeight();
		m_WantRedraw = true;
	}
	else
	{
		g_EntryPointer->OnKey(this, wParam);

		if (m_WantRedraw)
			RecalculateHeight();
	}
}
//----------------------------------------------------------------------------------
