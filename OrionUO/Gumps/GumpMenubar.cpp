/***********************************************************************************
**
** GumpMenubar.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpMenubar::CGumpMenubar(short x, short y)
: CGump(GT_MENUBAR, 0, x, y)
{
	WISPFUN_DEBUG("c101_f1");
	m_Page = 2;

	Add(new CGUIPage(1));
	Add(new CGUIResizepic(0, 0x13BE, 0, 0, 30, 27));
	Add(new CGUIButton(ID_GMB_MINIMIZE, 0x15A1, 0x15A1, 0x15A1, 5, 3));

	Add(new CGUIPage(2));
	CGLTexture *th1 = g_Orion.ExecuteGump(0x098B);
	CGLTexture *th2 = g_Orion.ExecuteGump(0x098D);

	int smallWidth = 50;
	if (th1 != NULL)
		smallWidth = th1->Width;

	int largeWidth = 100;
	if (th2 != NULL)
		largeWidth = th2->Width;

	static const int textPosTable[8][5] =
	{
		{ 0x098B, 30, 32, smallWidth, ID_GMB_MAP },
		{ 0x098D, 93, 96, largeWidth, ID_GMB_PAPERDOLL },
		{ 0x098D, 201, 204, largeWidth, ID_GMB_INVENTORY },
		{ 0x098D, 309, 312, largeWidth, ID_GMB_JOURNAL },
		{ 0x098B, 417, 422, smallWidth, ID_GMB_CHAT },
		{ 0x098B, 480, 482, smallWidth, ID_GMB_HELP },
		{ 0x098D, 543, 546, largeWidth, ID_GMB_WORLD_MAP },
		{ 0x098B, 651, 654, smallWidth, ID_GMB_INFO }
	};

	static const wstring text[8] =
	{
		g_ClilocManager.Cliloc(g_Language)->GetW(3000430, false, "Map"),
		g_ClilocManager.Cliloc(g_Language)->GetW(3002133, false, "Paperdoll"),
		g_ClilocManager.Cliloc(g_Language)->GetW(3000431, false, "Inventory"),
		g_ClilocManager.Cliloc(g_Language)->GetW(3002135, false, "Journal"),
		L"Chat",
		g_ClilocManager.Cliloc(g_Language)->GetW(3000134, false, "Help"),
		L"World Map",
		L"< ? >"
	};

	Add(new CGUIResizepic(0, 0x13BE, 0, 0, 718, 27));
	Add(new CGUIButton(ID_GMB_MINIMIZE, 0x15A4, 0x15A4, 0x15A4, 5, 3));

	IFOR(i, 0, 8)
	{
		Add(new CGUIButton(textPosTable[i][4], textPosTable[i][0], textPosTable[i][0], textPosTable[i][0], textPosTable[i][1], 1));

		CGUITextEntry *entry = (CGUITextEntry*)Add(new CGUITextEntry(textPosTable[i][4], 0, 0x0036, 0x0036, textPosTable[i][2], 2, textPosTable[i][3], true, 1, TS_CENTER));
		entry->m_Entry.SetText(text[i]);
		entry->CheckOnSerial = true;
		entry->ReadOnly = true;
		entry->FocusedOffsetY = 2;
	}
}
//----------------------------------------------------------------------------------
CGumpMenubar::~CGumpMenubar()
{
}
//---------------------------------------------------------------------------
void CGumpMenubar::OnChangeOpened(const bool &val)
{
	WISPFUN_DEBUG("c101_f2");
	if (val)
		m_Page = 2;
	else
		m_Page = 1;

	m_WantRedraw = true;
}
//---------------------------------------------------------------------------
void CGumpMenubar::InitToolTip()
{
	WISPFUN_DEBUG("c101_f3");
	uint id = g_SelectedObject.Serial;

	if (!m_Minimized)
	{
		switch (id)
		{
			case ID_GMB_MINIMIZE:
			{
				g_ToolTip.Set(L"Minimize the menubar gump");
				break;
			}
			case ID_GMB_MAP:
			{
				g_ToolTip.Set(L"Open the minimap gump");
				break;
			}
			case ID_GMB_PAPERDOLL:
			{
				g_ToolTip.Set(L"Open the paperdoll gump");
				break;
			}
			case ID_GMB_INVENTORY:
			{
				g_ToolTip.Set(L"Open backpack");
				break;
			}
			case ID_GMB_JOURNAL:
			{
				g_ToolTip.Set(L"Open the journal gump");
				break;
			}
			case ID_GMB_CHAT:
			{
				g_ToolTip.Set(L"Open the chat gump");
				break;
			}
			case ID_GMB_HELP:
			{
				g_ToolTip.Set(L"Open server's help menu gump");
				break;
			}
			case ID_GMB_WORLD_MAP:
			{
				g_ToolTip.Set(L"Open Orion's world map gump");
				break;
			}
			case ID_GMB_INFO:
			{
				g_ToolTip.Set(L"This option is not work now");
				break;
			}
			default:
				break;
		}
	}
	else
		g_ToolTip.Set(L"Maximize the menubar gump");
}
//----------------------------------------------------------------------------------
void CGumpMenubar::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c101_f4");
	switch (serial)
	{
		case ID_GMB_MINIMIZE:
		{
			m_Opened = !m_Opened;

			m_Page = 1 + (int)m_Opened;

			break;
		}
		case ID_GMB_MAP:
		{
			g_Orion.OpenMinimap();

			break;
		}
		case ID_GMB_PAPERDOLL:
		{
			g_Orion.PaperdollReq(g_PlayerSerial);

			break;
		}
		case ID_GMB_INVENTORY:
		{
			g_Orion.OpenBackpack();

			break;
		}
		case ID_GMB_JOURNAL:
		{
			g_Orion.OpenJournal();

			break;
		}
		case ID_GMB_CHAT:
		{
			g_Orion.OpenChat();

			break;
		}
		case ID_GMB_HELP:
		{
			CPacketHelpRequest().Send();

			break;
		}
		case ID_GMB_WORLD_MAP:
		{
			g_Orion.OpenWorldMap();

			break;
		}
		case ID_GMB_INFO:
		{
			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
void CGumpMenubar::GUMP_TEXT_ENTRY_EVENT_C
{
	WISPFUN_DEBUG("c101_f5");
	QFOR(item, m_Items, CBaseGUI*)
	{
		if (item->Type == GOT_TEXTENTRY)
		{
			CGUITextEntry *entry = (CGUITextEntry*)item;
			entry->Focused = (entry->Serial == serial);
		}
	}

	OnButton(serial);
}
//----------------------------------------------------------------------------------
void CGumpMenubar::OnLeftMouseButtonUp()
{
	WISPFUN_DEBUG("c101_f6");
	CGump::OnLeftMouseButtonUp();

	QFOR(item, m_Items, CBaseGUI*)
	{
		if (item->Type == GOT_TEXTENTRY)
		{
			CGUITextEntry *entry = (CGUITextEntry*)item;
			entry->Focused = false;
		}
	}

	m_WantRedraw = true;
}
//----------------------------------------------------------------------------------
