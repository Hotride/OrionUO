/***********************************************************************************
**
** GumpConsoleType.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GumpConsoleType.h"
#include "../TextEngine/GameConsole.h"
#include "../SelectedObject.h"
#include "../ToolTip.h"
#include "../Screen stages/GameScreen.h"
#include "../Managers/ConfigManager.h"
//----------------------------------------------------------------------------------
CGumpConsoleType *g_GumpConsoleType = NULL;
//----------------------------------------------------------------------------------
CGumpConsoleType::CGumpConsoleType(bool minimized, bool showFullText)
: CGump(GT_CONSOLE_TYPE, 0, 0, 0), m_SelectedType(0), m_ShowFullText(showFullText)
{
	m_Minimized = minimized;
	m_NoMove = true;
	g_GumpConsoleType = this;
}
//----------------------------------------------------------------------------------
CGumpConsoleType::~CGumpConsoleType()
{
	g_GumpConsoleType = NULL;
}
//----------------------------------------------------------------------------------
bool CGumpConsoleType::CanBeDisplayed()
{
	return g_ConfigManager.ShowDefaultConsoleEntryMode;
}
//----------------------------------------------------------------------------------
void CGumpConsoleType::OnChanShowFullText(const bool &val)
{
	m_WantUpdateContent = true;
}
//----------------------------------------------------------------------------------
bool CGumpConsoleType::ConsoleIsEmpty()
{
	bool result = (g_GameConsole.Length() == 0);

	switch (m_SelectedType)
	{
		case GCTT_YELL:
		case GCTT_WHISPER:
		case GCTT_EMOTE:
		case GCTT_C:
		case GCTT_BROADCAST:
		case GCTT_PARTY:
		{	   
			result = (g_ConsolePrefix[m_SelectedType] == g_GameConsole.Data());
			break;
		}
		default:
			break;
	}

	return result;
}
//----------------------------------------------------------------------------------
void CGumpConsoleType::DeleteConsolePrefix()
{
	static const wstring space = L" ";

	switch (m_SelectedType)
	{
		case GCTT_YELL:
		case GCTT_WHISPER:
		case GCTT_EMOTE:
		case GCTT_C:
		case GCTT_BROADCAST:
		case GCTT_PARTY:
		{
			wstring str(g_GameConsole.Data());

			if (str.find(g_ConsolePrefix[m_SelectedType]) == 0)
			{
				str.erase(str.begin(), str.begin() + g_ConsolePrefix[m_SelectedType].length());
				g_GameConsole.SetText(str);
			}

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
void CGumpConsoleType::SetConsolePrefix()
{
	switch (m_SelectedType)
	{
		case GCTT_YELL:
		case GCTT_WHISPER:
		case GCTT_EMOTE:
		case GCTT_C:
		case GCTT_BROADCAST:
		case GCTT_PARTY:
		{
			wstring str(g_GameConsole.Data());
			str = g_ConsolePrefix[m_SelectedType] + str;
			g_GameConsole.SetText(str);

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
void CGumpConsoleType::InitToolTip()
{
	uint selected = g_SelectedObject.Serial;

	switch (selected)
	{
		case ID_GCT_NORMAL:
		{
			g_ToolTip.Set(L"Normal entry mode\nno prefix", g_SelectedObject.Object());
			break;
		}
		case ID_GCT_YELL:
		{
			g_ToolTip.Set(L"Yell entry mode\nprefix is '! '", g_SelectedObject.Object());
			break;
		}
		case ID_GCT_WHISPER:
		{
			g_ToolTip.Set(L"Whisper entry mode\nprefix is '; '", g_SelectedObject.Object());
			break;
		}
		case ID_GCT_EMOTE:
		{
			g_ToolTip.Set(L"Emote entry mode\nprefix is ': '", g_SelectedObject.Object());
			break;
		}
		case ID_GCT_COMMAND:
		{
			g_ToolTip.Set(L"Command entry mode\nprefix is '. '", g_SelectedObject.Object());
			break;
		}
		case ID_GCT_BROADCAST:
		{
			g_ToolTip.Set(L"Broadcast entry mode\nprefix is '? '", g_SelectedObject.Object());
			break;
		}
		case ID_GCT_PARTY:
		{
			g_ToolTip.Set(L"Party entry mode\nprefix is '/ '", g_SelectedObject.Object());
			break;
		}
		case ID_GCT_MINIMIZE:
		{
			g_ToolTip.Set(L"Show/hide configuration", g_SelectedObject.Object());
			break;
		}
		case ID_GCT_SHOW_FULL_TEXT:
		{
			g_ToolTip.Set(L"Show full mode names", g_SelectedObject.Object());
			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
void CGumpConsoleType::UpdateContent()
{
	Clear();

	CGUIText *obj = (CGUIText*)Add(new CGUIText(0, 14, 0));
	obj->CreateTextureA(3, "Default entry text mode:");

	if (m_Minimized)
		Add(new CGUIButton(ID_GCT_MINIMIZE, 0x0985, 0x0986, 0x0986, 0, 6));
	else
	{
		Add(new CGUIButton(ID_GCT_MINIMIZE, 0x0983, 0x0984, 0x0984, 0, 6));

		int offsetX = 0;
		int offsetY = obj->m_Texture.Height;

		const char *text[] =
		{
			" Normal ",
			" Yell ",
			" Whisper ",
			" Emote ",
			" Command ",
			" Broadcast ",
			" Party "
		};

		int text0Height = 0;

		IFOR(i, 0, 7)
		{
			CGUITextEntry *entry = new CGUITextEntry(i + 1, 0, 0, 0, offsetX, offsetY, 0, false, 3);
			entry->CheckOnSerial = true;
			entry->ReadOnly = true;
			entry->SetGlobalColor(true, 0xFFFFFFFF, 0xFF267FFF, 0xFF00FF00);
			entry->Focused = (i == m_SelectedType);

			if (m_ShowFullText || !i)
			{
				entry->m_Entry.SetText(text[i]);
				entry->m_Entry.CreateTextureA(3, text[i], 0, 0, TS_LEFT, 0);
			}
			else
			{
				string str = " ";
				str += ToString(g_ConsolePrefix[i].c_str());

				if (str.length() < 3)
					str += " ";

				entry->m_Entry.SetText(str);
				entry->m_Entry.CreateTextureA(3, str, 0, 0, TS_LEFT, 0);
			}

			if (!i)
				text0Height = entry->m_Entry.m_Texture.Height;

			Add(new CGUIColoredPolygone(i + 1, 0, offsetX + 1, offsetY, entry->m_Entry.m_Texture.Width - 2, entry->m_Entry.m_Texture.Height, 0x3F000000));
			Add(entry);

			offsetX += entry->m_Entry.m_Texture.Width;
		}

		offsetY += text0Height + 3;

		CGUICheckbox *checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GCT_SHOW_FULL_TEXT, 0x00D2, 0x00D3, 0x00D2, 0, offsetY + 2));
		checkbox->Checked = m_ShowFullText;

		obj = (CGUIText*)Add(new CGUIText(0, 24, offsetY));
		obj->CreateTextureA(3, "Show full mode name");
	}
}
//----------------------------------------------------------------------------------
void CGumpConsoleType::CalculateGumpState()
{
	CGump::CalculateGumpState();

	g_GumpMovingOffset.X = 0;
	g_GumpMovingOffset.Y = 0;

	g_GumpTranslate.X = (float)(g_RenderBounds.GameWindowPosX + 2);
	g_GumpTranslate.Y = (float)(g_RenderBounds.GameWindowPosY + g_RenderBounds.GameWindowHeight + 2);
}
//----------------------------------------------------------------------------------
void CGumpConsoleType::GUMP_BUTTON_EVENT_C
{
	if (serial == ID_GCT_MINIMIZE)
	{
		m_Minimized = !m_Minimized;
		m_WantUpdateContent = true;
	}
	else if (serial == ID_GCT_SHOW_FULL_TEXT)
	{
		m_ShowFullText = !m_ShowFullText;
		m_WantUpdateContent = true;
	}
}
//----------------------------------------------------------------------------------
void CGumpConsoleType::GUMP_CHECKBOX_EVENT_C
{
	if (serial == ID_GCT_SHOW_FULL_TEXT && m_ShowFullText != state)
	{
		m_ShowFullText = state;
		m_WantUpdateContent = true;
	}
}
//----------------------------------------------------------------------------------
void CGumpConsoleType::GUMP_TEXT_ENTRY_EVENT_C
{
	DeleteConsolePrefix();

	m_SelectedType = serial - 1;

	QFOR(item, m_Items, CBaseGUI*)
	{
		if (item->Type == GOT_TEXTENTRY)
		{
			CGUITextEntry *entry = (CGUITextEntry*)item;
			entry->Focused = (item->Serial == serial);
		}
	}

	SetConsolePrefix();

	m_WantRedraw = true;
}
//----------------------------------------------------------------------------------
