/***********************************************************************************
**
** GumpDrag.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpDrag::CGumpDrag(uint serial, short x, short y)
: CGump(GT_DRAG, serial, x, y)
{
}
//----------------------------------------------------------------------------------
CGumpDrag::~CGumpDrag()
{
}
//----------------------------------------------------------------------------------
void CGumpDrag::UpdateContent()
{
	WISPFUN_DEBUG("c94_f1");
	Clear();

	Add(new CGUIGumppic(0x085C, 0, 0));

	int count = 0;
	CGameItem *selobj = g_World->FindWorldItem(m_Serial);

	if (selobj != NULL)
		count = selobj->Count;

	m_Slider = (CGUISlider*)Add(new CGUISlider(ID_GD_SCROLL, 0x0845, 0x0846, 0x0846, 0, 0, false, 29, 16, 105, 0, count, count));

	CGump *gumpEntry = g_GumpManager.GetTextEntryOwner();

	if (gumpEntry != NULL)
		gumpEntry->WantRedraw = true;

	Add(new CGUIHitBox(ID_GD_TEXT_FIELD, 28, 40, 60, 16));

	m_Entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_GD_TEXT_FIELD, 0x0386, 0x0386, 0x0386, 29, 42, 0, false, 1, TS_LEFT, 0, count + 1));
	m_Entry->CheckOnSerial = true;
	g_EntryPointer = &m_Entry->m_Entry;
	g_EntryPointer->NumberOnly = true;
	g_EntryPointer->SetText(std::to_wstring(count));

	Add(new CGUIButton(ID_GD_OKAY, 0x081A, 0x081C, 0x081B, 102, 37));
}
//----------------------------------------------------------------------------------
void CGumpDrag::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c94_f2");
	if (serial == ID_GD_OKAY) //Button Okay
		OnOkayPressed();
}
//----------------------------------------------------------------------------------
void CGumpDrag::GUMP_SLIDER_CLICK_EVENT_C
{
	WISPFUN_DEBUG("c94_f3");
	OnSliderMove(serial);
}
//----------------------------------------------------------------------------------
void CGumpDrag::GUMP_SLIDER_MOVE_EVENT_C
{
	WISPFUN_DEBUG("c94_f4");
	if (m_StartText)
		m_StartText = false;

	if (m_Entry!= NULL)
		m_Entry->m_Entry.SetText(std::to_wstring(m_Slider->Value));
}
//----------------------------------------------------------------------------------
void CGumpDrag::OnCharPress(const WPARAM &wParam, const LPARAM &lParam)
{
	WISPFUN_DEBUG("c94_f5");
	if (wParam >= '0' && wParam <= '9')
	{
		if (m_StartText)
		{
			g_EntryPointer->Clear();
			m_StartText = false;
		}

		g_EntryPointer->Insert(wParam);

		int val = 0;
		
		if (g_EntryPointer->Length())
			val = atoi(g_EntryPointer->c_str());

		m_Slider->Value = val;
		m_Slider->CalculateOffset();

		m_WantRedraw = true;
	}
}
//----------------------------------------------------------------------------------
void CGumpDrag::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
	WISPFUN_DEBUG("c94_f6");
	CGameItem *item = g_World->FindWorldItem(Serial);

	if (item != NULL)
	{
		switch (wParam)
		{
			case VK_RETURN:
			{
				OnOkayPressed();

				if (g_ConfigManager.ConsoleNeedEnter)
					g_EntryPointer = NULL;
				else
					g_EntryPointer = &g_GameConsole;

				break;
			}
			case VK_HOME:
			case VK_END:
			case VK_LEFT:
			case VK_RIGHT:
			{
				g_EntryPointer->OnKey(this, wParam);

				if (m_StartText)
					m_StartText = false;

				m_WantRedraw = true;

				break;
			}
			case VK_DELETE:
			case VK_BACK:
			{
				g_EntryPointer->OnKey(this, wParam);

				if (m_StartText)
					m_StartText = false;

				int val = 0;

				if (g_EntryPointer->Length())
					val = atoi(g_EntryPointer->c_str());

				m_Slider->Value = val;
				m_Slider->CalculateOffset();

				m_WantRedraw = true;

				break;
			}
			default:
				break;
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpDrag::OnOkayPressed()
{
	WISPFUN_DEBUG("c94_f7");
	if (!g_ObjectInHand.Enabled)
	{
		m_RemoveMark = true;

		if (m_Slider->Value)
		{
			CGameItem *obj = g_World->FindWorldItem(m_Serial);

			if (obj != NULL)
				g_Orion.PickupItem(obj, m_Slider->Value);
		}
	}
}
//----------------------------------------------------------------------------------
