/***********************************************************************************
**
** GumpScreenCharacterList.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GumpScreenCharacterList.h"
#include "../Screen stages/CharacterListScreen.h"
#include "../OrionUO.h"
#include "../CharacterList.h"
#include "../Managers/PacketManager.h"
#include "../Managers/ConfigManager.h"
//----------------------------------------------------------------------------------
CGumpScreenCharacterList::CGumpScreenCharacterList()
: CGump(GT_NONE, 0, 0, 0)
{
	m_NoMove = true;
	m_NoClose = true;
}
//----------------------------------------------------------------------------------
CGumpScreenCharacterList::~CGumpScreenCharacterList()
{
}
//----------------------------------------------------------------------------------
void CGumpScreenCharacterList::UpdateContent()
{
	Clear();

	int count = g_CharacterList.Count;
	bool testField = (g_PacketManager.ClientVersion >= CV_305D);
	int posInList = 0;
	int yOffset = 150;
	int yBonus = 0;
	int listTitleY = 106;

	if (g_PacketManager.ClientVersion >= CV_6040)
	{
		listTitleY = 96;
		yOffset = 125;
		yBonus = 45;
	}

	Add(new CGUIGumppicTiled(0x0E14, 0, 0, 640, 480));
	Add(new CGUIGumppic(0x157C, 0, 0));
	Add(new CGUIGumppic(0x15A0, 0, 4));
	Add(new CGUIButton(ID_CS_QUIT, 0x1589, 0x158A, 0x158B, 555, 4));
	Add(new CGUIResizepic(0, 0x0A28, 160, 70, 408, 343 + yBonus));

	CGUIText *obj = new CGUIText(0x0386, 267, listTitleY);
	obj->CreateTextureA(2, "Character Selection");
	Add(obj);

	IFOR(i, 0, count)
	{
		int len = g_CharacterList.GetName(i).length();

		if (!testField || len != 0 || !i)
		{
			Add(new CGUIResizepic(ID_CS_CHARACTERS + i, 0x0BB8, 224, yOffset + (posInList * 40), 280, 30));

			if (len)
			{
				CGUITextEntry *entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_CS_CHARACTERS + i, 0x034F, 0x03E3, 0x0021, 229, yOffset + (posInList * 40), 270, false, 5, TS_CENTER, 0, 32));
				entry->m_Entry.Width = 270;
				entry->m_Entry.SetText(g_CharacterList.GetName(i));
				entry->CheckOnSerial = true;
				entry->ReadOnly = true;
				entry->Focused = (g_CharacterList.Selected == i);
			}

			posInList++;
		}
	}

	if (!g_CharacterList.OnePerson)
		Add(new CGUIButton(ID_CS_NEW, 0x159D, 0x159E, 0x159F, 224, 350 + yBonus));

	Add(new CGUIButton(ID_CS_DELETE, 0x159A, 0x159B, 0x159C, 442, 350 + yBonus));
	Add(new CGUIButton(ID_CS_ARROW_PREV, 0x15A1, 0x15A2, 0x15A3, 586, 445));
	Add(new CGUIButton(ID_CS_ARROW_NEXT, 0x15A4, 0x15A5, 0x15A6, 610, 445));
}
//----------------------------------------------------------------------------------
void CGumpScreenCharacterList::InitToolTip()
{
	if (!g_ConfigManager.UseToolTips)
		return;

	uint id = g_SelectedObject.Serial;

	switch (id)
	{
		case ID_CS_QUIT:
		{
			g_ToolTip.Set(L"Quit Ultima Online", 80);
			break;
		}
		case ID_CS_ARROW_NEXT:
		{
			g_ToolTip.Set(L"Play UO with selected character");
			break;
		}
		case ID_CS_ARROW_PREV:
		{
			g_ToolTip.Set(L"Back to shard selection screen");
			break;
		}
		case ID_CS_NEW:
		{
			g_ToolTip.Set(L"Create new character", 150);
			break;
		}
		case ID_CS_DELETE:
		{
			g_ToolTip.Set(L"Delete this character", 150);
			break;
		}
		default:
			break;
	}

	if (id >= ID_CS_CHARACTERS)
	{
		bool testField = (g_PacketManager.ClientVersion >= CV_305D);

		IFOR(i, 0, g_CharacterList.Count)
		{
			if (!testField || !i || g_CharacterList.GetName(i).length() != 0)
			{
				if (id == (ID_CS_CHARACTERS + i))
				{
					g_ToolTip.Set(L"Click here to select this character", 150);

					break;
				}
			}
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpScreenCharacterList::GUMP_BUTTON_EVENT_C
{
	if (serial == ID_CS_QUIT) //x button
		g_CharacterListScreen.CreateSmoothAction(CCharacterListScreen::ID_SMOOTH_CLS_QUIT);
	else if (serial == ID_CS_ARROW_PREV) //< button
		g_CharacterListScreen.CreateSmoothAction(CCharacterListScreen::ID_SMOOTH_CLS_CONNECT);
	else if (serial == ID_CS_ARROW_NEXT) //> button
		g_CharacterListScreen.CreateSmoothAction(CCharacterListScreen::ID_SMOOTH_CLS_SELECT_CHARACTER);
	else if (serial == ID_CS_NEW) //New button
		g_CharacterListScreen.CreateSmoothAction(CCharacterListScreen::ID_SMOOTH_CLS_GO_SCREEN_PROFESSION_SELECT);
	else if (serial == ID_CS_DELETE) //Delete button
		g_CharacterListScreen.CreateSmoothAction(CCharacterListScreen::ID_SMOOTH_CLS_GO_SCREEN_DELETE);
}
//----------------------------------------------------------------------------------
void CGumpScreenCharacterList::GUMP_TEXT_ENTRY_EVENT_C
{
	IFOR(i, 0, g_CharacterList.Count)
	{
		if (serial == (ID_CS_CHARACTERS + i))
		{
			g_CharacterList.Selected = i;

			QFOR(item, m_Items, CBaseGUI*)
			{
				if (item->Type == GOT_TEXTENTRY)
					((CGUITextEntry*)item)->Focused = item->Serial == serial;
			}

			break;
		}
	}
}
//----------------------------------------------------------------------------------
/*!
Двойной клик левой кнопкой мыши
@return true при успешной обработке
*/
bool CGumpScreenCharacterList::OnLeftMouseButtonDoubleClick()
{
	IFOR(i, 0, g_CharacterList.Count)
	{
		if (g_SelectedObject.Serial == (ID_CS_CHARACTERS + i))
		{
			if (!g_CharacterList.GetName(i).length())
				g_CharacterListScreen.CreateSmoothAction(CCharacterListScreen::ID_SMOOTH_CLS_GO_SCREEN_PROFESSION_SELECT);
			else
				g_CharacterListScreen.CreateSmoothAction(CCharacterListScreen::ID_SMOOTH_CLS_SELECT_CHARACTER);

			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------------
