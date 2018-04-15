// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GumpGeneric.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpGeneric::CGumpGeneric(uint serial, short x, short y, uint id)
: CGump(GT_GENERIC, serial, x, y)
{
	Page = 1;
	ID = id;
}
//----------------------------------------------------------------------------------
CGumpGeneric::~CGumpGeneric()
{
}
//----------------------------------------------------------------------------------
void CGumpGeneric::InitToolTip()
{
	WISPFUN_DEBUG("c128_f3");

	if (g_SelectedObject.Object != NULL && g_SelectedObject.Object->IsGUI())
	{
		CBaseGUI *obj = (CBaseGUI*)g_SelectedObject.Object;

		if (obj->Type == GOT_VIRTURE_GUMP)
		{
			int offset = 0;

			switch (g_SelectedObject.Serial)
			{
				case 0x69:
					offset = 2;
					break;
				case 0x6A:
					offset = 7;
					break;
				case 0x6B:
					offset = 5;
					break;
				case 0x6C:
					offset = 0;
					break;
				case 0x6D:
					offset = 6;
					break;
				case 0x6E:
					offset = 1;
					break;
				case 0x6F:
					offset = 3;
					break;
				case 0x70:
					offset = 4;
					break;
				default:
					break;
			}

			g_ToolTip.Set(1051000 + offset, "Some virture gump item", 100);
		}
		else if (obj->ClilocID)
			g_ToolTip.Set(obj->ClilocID, "");
	}
}
//----------------------------------------------------------------------------------
void CGumpGeneric::AddText(const int &index, const wstring &text, CBaseGUI *start, const bool &backbroundCanBeColored)
{
	WISPFUN_DEBUG("c96_f1");
	if (start == NULL)
		start = (CBaseGUI*)m_Items;

	QFOR(item, start, CBaseGUI*)
	{
		switch (item->Type)
		{
			case GOT_TEXT:
			{
				CGUIGenericText *gt = (CGUIGenericText*)item;

				if (gt->TextID == index)
					gt->CreateTexture(text);

				break;
			}
			case GOT_TEXTENTRY:
			{
				CGUIGenericTextEntry *gte = (CGUIGenericTextEntry*)item;

				if (gte->TextID == index)
					gte->m_Entry.SetText(text);

				break;
			}
			case GOT_HTMLTEXT:
			{
				CGUIHTMLText *gt = (CGUIHTMLText*)item;

				if (gt->TextID == index)
				{
					gt->Text = text;
					gt->CreateTexture(backbroundCanBeColored);
				}

				break;
			}
			case GOT_HTMLGUMP:
			{
				CGUIHTMLGump *ghtml = (CGUIHTMLGump*)item;

				if (ghtml->Serial == index + 1)
				{
					AddText(index, text, (CBaseGUI*)item->m_Items, !ghtml->HaveBackground);
					ghtml->CalculateDataSize();
				}

				break;
			}
			default:
				break;
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpGeneric::SendGumpResponse(int index)
{
	WISPFUN_DEBUG("c96_f2");
	//Ответ на гамп
	CPacketGumpResponse(this, index).Send();

	//Удаляем использованный гамп
	RemoveMark = true;
}
//----------------------------------------------------------------------------------
void CGumpGeneric::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c96_f3");
	SendGumpResponse(serial);
}
//----------------------------------------------------------------------------------
void CGumpGeneric::GUMP_DIRECT_HTML_LINK_EVENT_C
{
	WISPFUN_DEBUG("c96_f4");
	g_FontManager.GoToWebLink(link);
}
//----------------------------------------------------------------------------------
bool CGumpGeneric::OnLeftMouseButtonDoubleClick()
{
	WISPFUN_DEBUG("c96_f5");
	if (g_GeneratedMouseDown)
		return false;

	if (g_PressedObject.LeftObject != NULL && g_PressedObject.LeftObject->IsGUI() && ((CBaseGUI*)g_PressedObject.LeftObject)->Type == GOT_VIRTURE_GUMP)
	{
		//Ответ на гамп
		CPacketVirtureGumpResponse(this, g_PressedObject.LeftSerial).Send();

		//Удаляем использованный гамп
		RemoveMark = true;

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------------
void CGumpGeneric::OnCharPress(const WPARAM &wParam, const LPARAM &lParam)
{
	WISPFUN_DEBUG("c96_f6");
	g_EntryPointer->Insert((wchar_t)wParam);
	WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGumpGeneric::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
	WISPFUN_DEBUG("c96_f7");
	if (wParam == VK_RETURN)
	{
		if (g_ConfigManager.GetConsoleNeedEnter())
			g_EntryPointer = NULL;
		else
			g_EntryPointer = &g_GameConsole;

		WantRedraw = true;
	}
	else
		g_EntryPointer->OnKey(this, wParam);
}
//----------------------------------------------------------------------------------
