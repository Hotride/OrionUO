/***********************************************************************************
**
** GumpJournal.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpJournal::CGumpJournal(short x, short y, bool minimized, int height)
: CGumpBaseScroll(GT_JOURNAL, 0, 0x0820, height, x, y, true, 0, true)
{
	WISPFUN_DEBUG("c98_f1");
	m_Locker.Serial = ID_GJ_LOCK_MOVING;

	if (minimized)
	{
		m_Page = 1;
		m_Minimized = true;
		m_MinimizedX = x;
		m_MinimizedY = y;
	}
	else
		m_Page = 2;

	Add(new CGUIPage(1));
	Add(new CGUIGumppic(0x0830, 0, 0));

	Add(new CGUIPage(2));
	Add(new CGUIGumppic(0x082A, 111, 32)); //Journal text gump
	Add(new CGUIGumppic(0x082B, 30, 58)); //Top line
	m_BottomLine = (CGUIGumppic*)Add(new CGUIGumppic(0x082B, 31, m_Height - 26)); //Bottom line

	m_TextLocker = (CGUIButton*)Add(new CGUIButton(ID_GJ_BUTTON_LOCK, 0x082C, 0x082C, 0x082C, 242, m_Height - 27));
	m_TextLocker->CheckPolygone = true;
	m_TextLocker->Visible = false;

	m_CheckboxShowSystem = (CGUICheckbox*)Add(new CGUICheckbox(ID_GJ_SHOW_SYSTEM, 0x00D2, 0x00D3, 0x00D2, 40, m_Height - 5));
	m_CheckboxShowSystem->Checked = g_JournalShowSystem;
	m_CheckboxShowObjects = (CGUICheckbox*)Add(new CGUICheckbox(ID_GJ_SHOW_OBJECTS, 0x00D2, 0x00D3, 0x00D2, 126, m_Height - 5));
	m_CheckboxShowObjects->Checked = g_JournalShowObjects;
	m_CheckboxShowClient = (CGUICheckbox*)Add(new CGUICheckbox(ID_GJ_SHOW_CLIENT, 0x00D2, 0x00D3, 0x00D2, 210, m_Height - 5));
	m_CheckboxShowClient->Checked = g_JournalShowClient;

	m_TextShowSystem = (CGUIText*)Add(new CGUIText(0x0386, 63, m_Height - 2));
	m_TextShowSystem->CreateTextureA(1, "System");
	m_TextShowObjects = (CGUIText*)Add(new CGUIText(0x0386, 149, m_Height - 2));
	m_TextShowObjects->CreateTextureA(1, "Objects");
	m_TextShowClient = (CGUIText*)Add(new CGUIText(0x0386, 233, m_Height - 2));
	m_TextShowClient->CreateTextureA(1, "Client");

	QFOR(item, g_Journal.m_Items, CTextData*)
		AddText(item);
}
//----------------------------------------------------------------------------------
CGumpJournal::~CGumpJournal()
{
}
//---------------------------------------------------------------------------
void CGumpJournal::InitToolTip()
{
	WISPFUN_DEBUG("c98_f2");
	uint id = g_SelectedObject.Serial;

	if (!m_Minimized)
	{
		switch (id)
		{
			case ID_GBS_BUTTON_MINIMIZE:
			{
				g_ToolTip.Set(L"Minimize the journal gump");
				break;
			}
			case ID_GBS_BUTTON_RESIZE:
			{
				g_ToolTip.Set(L"Start resizing for journal gump");
				break;
			}
			case ID_GJ_SHOW_SYSTEM:
			{
				g_ToolTip.Set(L"Show/hide system messages");
				break;
			}
			case ID_GJ_SHOW_OBJECTS:
			{
				g_ToolTip.Set(L"Show/hide objects messages");
				break;
			}
			case ID_GJ_SHOW_CLIENT:
			{
				g_ToolTip.Set(L"Show/hide client messages");
				break;
			}
			case ID_GJ_BUTTON_LOCK:
			{
				g_ToolTip.Set(L"Reset scroller position");
				break;
			}
			case ID_GJ_LOCK_MOVING:
			{
				g_ToolTip.Set(L"Lock moving/closing the journal gump");
				break;
			}
			default:
				break;
		}
	}
	else
		g_ToolTip.Set(L"Double click to maximize journal gump");
}
//----------------------------------------------------------------------------------
void CGumpJournal::UpdateHeight()
{
	WISPFUN_DEBUG("c98_f3");
	CGumpBaseScroll::UpdateHeight();

	m_BottomLine->Y = m_Height - 26; //Bottom line

	m_TextLocker->Y = m_Height - 27;

	m_CheckboxShowSystem->Y = m_Height - 5;
	m_CheckboxShowObjects->Y = m_Height - 5;
	m_CheckboxShowClient->Y = m_Height - 5;

	m_TextShowSystem->Y = m_Height - 2;
	m_TextShowObjects->Y = m_Height - 2;
	m_TextShowClient->Y = m_Height - 2;

	if (!m_TextLocker->Visible)
		m_HTMLGump->m_Slider->Value = m_HTMLGump->m_Slider->MaxValue;
}
//----------------------------------------------------------------------------------
int CGumpJournal::RecalculateHeight()
{
	WISPFUN_DEBUG("c98_f4");
	int height = 0;

	QFOR(item, m_HTMLGump->m_Items, CBaseGUI*)
	{
		if (item->Type == GOT_TEXT)
		{
			item->Y = height;

			bool visible = true;

			if (item->Graphic == TT_SYSTEM)
				visible = g_JournalShowSystem;
			else if (item->Graphic == TT_OBJECT)
				visible = g_JournalShowObjects;
			else if (item->Graphic == TT_CLIENT)
				visible = g_JournalShowClient;

			item->Visible = visible;

			if (visible)
				height += ((CGUIText*)item)->m_Texture.Height;
		}
	}

	return height;
}
//----------------------------------------------------------------------------------
void CGumpJournal::AddText(CTextData *obj)
{
	WISPFUN_DEBUG("c98_f5");
	CGUIText *text = (CGUIText*)m_HTMLGump->Add(new CGUIText(obj->Color, 4, RecalculateHeight()));
	text->MoveOnDrag = true;
	text->Serial = (uint)obj;
	text->Graphic = (ushort)obj->Type;
	
	ushort flags = UOFONT_INDENTION;

	if (obj->Type == TT_OBJECT || obj->Unicode)
		flags |= UOFONT_BLACK_BORDER;

	if (obj->Unicode)
		text->CreateTextureW(obj->Font, obj->UnicodeText, 30, 214, TS_LEFT, flags);
	else
		text->CreateTextureA(obj->Font, obj->Text, 214, TS_LEFT, flags);

	m_HTMLGump->CalculateDataSize();

	if (!m_TextLocker->Visible)
	{
		m_HTMLGump->m_Slider->Value = m_HTMLGump->m_Slider->MaxValue;
		m_HTMLGump->m_Slider->CalculateOffset();
	}
}
//----------------------------------------------------------------------------------
void CGumpJournal::DeleteText(CTextData *obj)
{
	WISPFUN_DEBUG("c98_f6");
	QFOR(item, m_HTMLGump->m_Items, CBaseGUI*)
	{
		if (item->Type == GOT_TEXT && item->Serial == (uint)obj)
		{
			m_HTMLGump->Delete(item);
			m_HTMLGump->CalculateDataSize();
			RecalculateHeight();

			break;
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpJournal::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c98_f7");
	if (serial == ID_GBS_BUTTON_MINIMIZE)
	{
		m_Minimized = true;
		m_Page = 1;
	}
	else if (serial == ID_GJ_LOCK_MOVING)
		m_LockMoving = !m_LockMoving;
	else if (serial == ID_GJ_BUTTON_LOCK)
	{
		m_HTMLGump->m_Slider->Value = m_HTMLGump->m_Slider->MaxValue;
		m_HTMLGump->m_Slider->CalculateOffset();
		m_TextLocker->Visible = false;
	}
}
//----------------------------------------------------------------------------------
void CGumpJournal::GUMP_CHECKBOX_EVENT_C
{
	WISPFUN_DEBUG("c98_f8");
	if (serial == ID_GJ_SHOW_SYSTEM)
	{
		g_JournalShowSystem = !g_JournalShowSystem;
		RecalculateHeight();
		m_HTMLGump->CalculateDataSize();

		if (!m_TextLocker->Visible)
		{
			m_HTMLGump->m_Slider->Value = m_HTMLGump->m_Slider->MaxValue;
			m_HTMLGump->m_Slider->CalculateOffset();
		}
	}
	else if (serial == ID_GJ_SHOW_OBJECTS)
	{
		g_JournalShowObjects = !g_JournalShowObjects;
		RecalculateHeight();
		m_HTMLGump->CalculateDataSize();

		if (!m_TextLocker->Visible)
		{
			m_HTMLGump->m_Slider->Value = m_HTMLGump->m_Slider->MaxValue;
			m_HTMLGump->m_Slider->CalculateOffset();
		}
	}
	else if (serial == ID_GJ_SHOW_CLIENT)
	{
		g_JournalShowClient = !g_JournalShowClient;
		RecalculateHeight();
		m_HTMLGump->CalculateDataSize();

		if (!m_TextLocker->Visible)
		{
			m_HTMLGump->m_Slider->Value = m_HTMLGump->m_Slider->MaxValue;
			m_HTMLGump->m_Slider->CalculateOffset();
		}
	}
}
//----------------------------------------------------------------------------------
bool CGumpJournal::OnLeftMouseButtonDoubleClick()
{
	WISPFUN_DEBUG("c98_f9");
	if (m_Minimized)
	{
		m_Minimized = false;
		m_WantRedraw = true;
		m_Page = 2;

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------------
