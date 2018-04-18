﻿// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GumpRacialAbilitiesBook.cpp
**
** Copyright (C) December 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpRacialAbilitiesBook::CGumpRacialAbilitiesBook(int x, int y)
: CGump(GT_RACIAL_ABILITIES_BOOK, 0, x, y)
{
	Draw2Page = true;
}
//----------------------------------------------------------------------------------
CGumpRacialAbilitiesBook::~CGumpRacialAbilitiesBook()
{
}
//----------------------------------------------------------------------------------
void CGumpRacialAbilitiesBook::InitToolTip()
{
	WISPFUN_DEBUG("c110_f1");
	if (Minimized)
	{
		g_ToolTip.Set(L"Double click to maximize book gump");
		return;
	}

	uint serial = g_SelectedObject.Serial;

	if (Page >= DictionaryPagesCount && serial >= ID_GRAB_ICON)
		g_ToolTip.Set(g_ClilocManager.Cliloc(g_Language)->GetW(TooltipOffset + (serial - ID_GRAB_ICON), true), 150);
}
//----------------------------------------------------------------------------
void CGumpRacialAbilitiesBook::PrepareContent()
{
	WISPFUN_DEBUG("c110_f2");
	int abilityOnPage = 0;
	ushort iconStartGraphic = 0;

	GetSummaryBookInfo(abilityOnPage, iconStartGraphic);

	if (g_PressedObject.LeftGump == this && Page >= DictionaryPagesCount && g_PressedObject.LeftSerial >= ID_GRAB_ICON && !((CBaseGUI*)g_PressedObject.LeftObject)->MoveOnDrag)
	{
		WISP_GEOMETRY::CPoint2Di offset = g_MouseManager.LeftDroppedOffset();

		if ((abs(offset.X) >= DRAG_PIXEL_RANGE || abs(offset.Y) >= DRAG_PIXEL_RANGE) || (g_MouseManager.LastLeftButtonClickTimer + g_MouseManager.DoubleClickDelay < g_Ticks))
		{
			g_GumpManager.AddGump(new CGumpRacialAbility(iconStartGraphic + g_PressedObject.LeftSerial - ID_GRAB_ICON, g_MouseManager.Position.X - 20, g_MouseManager.Position.Y - 20));

			g_OrionWindow.EmulateOnLeftMouseButtonDown();
		}
	}
}
//----------------------------------------------------------------------------
void CGumpRacialAbilitiesBook::GetSummaryBookInfo(int &abilityOnPage, ushort &iconStartGraphic)
{
	WISPFUN_DEBUG("c110_f3");
	DictionaryPagesCount = 2;
	abilityOnPage = 3;
	
	switch (g_Player->Race)
	{
		case RT_HUMAN:
		{
			AbilityCount = 4;
			iconStartGraphic = 0x5DD0;
			TooltipOffset = 1112198;
			break;
		}
		case RT_ELF:
		{
			AbilityCount = 6;
			iconStartGraphic = 0x5DD4;
			TooltipOffset = 1112202;
			break;
		}
		case RT_GARGOYLE:
		{
			AbilityCount = 5;
			iconStartGraphic = 0x5DDA;
			TooltipOffset = 1112208;
			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------
string CGumpRacialAbilitiesBook::GetAbilityName(int offset, bool &passive)
{
	WISPFUN_DEBUG("c110_f4");
	string result = "";
	passive = true;

	static const string humanNames[] = { "Strong Back", "Tough", "Workhorse", "Jack of All Trades" };
	static const string elfNames[] = { "Night Sight", "Infused with Magic", "Knowledge of Nature", "Difficult to Track", "Perception", "Wisdom" };
	static const string gargoyleNames[] = { "Flying", "Berserk", "Master Artisan", "Deadly Aim", "Mystic Insight" };
	
	switch (g_Player->Race)
	{
		case RT_HUMAN:
		{
			result = humanNames[offset];

			break;
		}
		case RT_ELF:
		{
			result = elfNames[offset];

			break;
		}
		case RT_GARGOYLE:
		{
			if (!offset)
				passive = false;

			result = gargoyleNames[offset];

			break;
		}
		default:
			break;
	}

	return result;
}
//----------------------------------------------------------------------------
void CGumpRacialAbilitiesBook::UpdateContent()
{
	WISPFUN_DEBUG("c110_f5");
	m_PrevPage = NULL;
	m_NextPage = NULL;

	Clear();

	Add(new CGUIPage(-1));

	if (Minimized)
	{
		Add(new CGUIGumppic(0x2B27, 0, 0));

		return;
	}

	Add(new CGUIGumppic(0x2B29, 0, 0));

	Add(new CGUIHitBox(ID_GRAB_BUTTON_MINIMIZE, 6, 100, 16, 16, true));

	int abilityOnPage = 0;
	ushort iconStartGraphic = 0;

	GetSummaryBookInfo(abilityOnPage, iconStartGraphic);

	PagesCount = DictionaryPagesCount + AbilityCount;

	int offs = 0;

	IFOR(page, 0, DictionaryPagesCount)
	{
		Add(new CGUIPage((int)page));

		int indexX = 106;
		int dataX = 62;
		int y = 0;

		if (page % 2)
		{
			indexX = 269;
			dataX = 225;
		}

		CGUIText *text = (CGUIText*)Add(new CGUIText(0x0288, indexX, 10));
		text->CreateTextureA(6, "INDEX");

		IFOR(i, 0, abilityOnPage)
		{
			if (offs >= AbilityCount)
				break;

			CGUIHitBox *box = (CGUIHitBox*)Add(new CGUIHitBox(ID_GRAB_DICTIONARY_ICON + offs, dataX, 52 + y, 100, 16, true));
			box->MoveOnDrag = true;

			CGUITextEntry *entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_GRAB_DICTIONARY_ICON + offs, 0x0288, 0, 0, dataX, 52 + y, 0, false, 9));
			bool passive = true;
			entry->m_Entry.SetText(GetAbilityName(offs, passive));
			entry->CheckOnSerial = true;
			entry->ReadOnly = true;

			y += 15;

			offs++;
		}
	}

	int page = DictionaryPagesCount;

	IFOR(i, 0, AbilityCount)
	{
		int iconX = 62;
		int iconTextX = 112;

		if (page % 2)
		{
			iconX = 225;
			iconTextX = 275;
		}

		Add(new CGUIPage(page));
		page++;

		bool passive = true;
		string spellName = GetAbilityName((int)i, passive);

		CGUIText *text = (CGUIText*)Add(new CGUIText(0x0288, iconTextX, 34));
		text->CreateTextureA(6, spellName, 100);

		if (passive)
		{
			text = (CGUIText*)Add(new CGUIText(0x0288, iconTextX, 64));
			text->CreateTextureA(6, "(Passive)");
		}

		CGUIGumppic *icon = (CGUIGumppic*)Add(new CGUIGumppic(iconStartGraphic + (int)i, iconX, 40));
		icon->Serial = ID_GRAB_ICON + (int)i;
		icon->CheckPolygone = true;
		icon->MoveOnDrag = passive;

		Add(new CGUIGumppicTiled(0x0835, iconX, 88, 120, 0));
	}


	Add(new CGUIPage(-1));

	m_PrevPage = (CGUIButton*)Add(new CGUIButton(ID_GRAB_BUTTON_PREV, 0x08BB, 0x08BB, 0x08BB, 50, 8));
	m_PrevPage->Visible = (Page != 0);
	m_NextPage = (CGUIButton*)Add(new CGUIButton(ID_GRAB_BUTTON_NEXT, 0x08BC, 0x08BC, 0x08BC, 321, 8));
	m_NextPage->Visible = (Page + 2 < PagesCount);
}
//----------------------------------------------------------------------------
void CGumpRacialAbilitiesBook::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c110_f6");
	int newPage = -1;

	if (serial == ID_GRAB_BUTTON_PREV)
	{
		if (Page > 0)
		{
			newPage = Page - 2;

			if (newPage < 0)
				newPage = 0;
		}
	}
	else if (serial == ID_GRAB_BUTTON_NEXT)
	{
		if (Page < PagesCount)
		{
			newPage = Page + 2;

			if (newPage >= PagesCount)
				newPage = PagesCount - 1;
		}
	}
	else if (serial == ID_GRAB_BUTTON_MINIMIZE)
	{
		Minimized = true;
		WantUpdateContent = true;
	}
	else if (serial == ID_GRAB_LOCK_MOVING)
		LockMoving = !LockMoving;
	else if (serial >= ID_GRAB_DICTIONARY_ICON)
	{
		if (Page < DictionaryPagesCount) //List of spells
			newPage = DictionaryPagesCount + (serial - ID_GRAB_DICTIONARY_ICON);
	}

	if (newPage > -1 && !g_ClickObject.Enabled)
	{
		if (newPage % 2)
			newPage--;

		g_ClickObject.Init(g_PressedObject.LeftObject, this);
		g_ClickObject.Timer = g_Ticks + g_MouseManager.DoubleClickDelay;
		g_ClickObject.Page = newPage;
	}
}
//----------------------------------------------------------------------------
bool CGumpRacialAbilitiesBook::OnLeftMouseButtonDoubleClick()
{
	WISPFUN_DEBUG("c110_f7");
	bool result = false;

	if (Minimized)
	{
		Minimized = false;
		WantUpdateContent = true;

		result = true;
	}
	else
	{
		if (g_PressedObject.LeftSerial == ID_GRAB_BUTTON_PREV)
		{
			ChangePage(0);

			WantRedraw = true;

			result = true;
		}
		else if (g_PressedObject.LeftSerial == ID_GRAB_BUTTON_NEXT)
		{
			int newPage = PagesCount - 1;

			if (newPage % 2)
				newPage--;

			ChangePage(newPage);

			WantRedraw = true;

			result = true;
		}
		else if (g_PressedObject.LeftSerial >= ID_GRAB_ICON)
		{
			CGumpRacialAbility::OnAbilityUse(g_PressedObject.LeftObject->Graphic);

			result = true;
		}
	}

	return result;
}
//----------------------------------------------------------------------------------
void CGumpRacialAbilitiesBook::DelayedClick(CRenderObject *obj)
{
	WISPFUN_DEBUG("c110_f8");
	if (obj != NULL)
	{
		ChangePage(g_ClickObject.Page);
		WantRedraw = true;
	}
}
//----------------------------------------------------------------------------------
void CGumpRacialAbilitiesBook::ChangePage(int newPage)
{
	WISPFUN_DEBUG("c110_f9");
	Page = newPage;

	m_PrevPage->Visible = (Page != 0);
	m_NextPage->Visible = (Page + 2 < PagesCount);

	g_Orion.PlaySoundEffect(0x0055);
}
//----------------------------------------------------------------------------------
