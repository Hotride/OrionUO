/****************************************************************************
**
** GumpObject.cpp
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
TGumpObject::TGumpObject(GUMP_OBJECT_TYPE type, WORD graphic, WORD color, short x, short y)
: TRenderObject(0, graphic, color, x, y), m_Type(type)
{
}
//---------------------------------------------------------------------------
TGumpPage::TGumpPage(int page)
: TGumpObject(GOT_PAGE, 0, 0, 0, 0), m_Page(page)
{
}
//---------------------------------------------------------------------------
TGumpGroup::TGumpGroup(int group)
: TGumpObject(GOT_GROUP, 0, 0, 0, 0), m_Group(group)
{
}
//---------------------------------------------------------------------------
TGumpEndGroup::TGumpEndGroup()
: TGumpObject(GOT_ENDGROUP, 0, 0, 0, 0)
{
}
//---------------------------------------------------------------------------
TGumpMasterGump::TGumpMasterGump(int index)
: TGumpObject(GOT_MASTERGUMP, 0, 0, 0, 0), m_Index(index)
{
}
//---------------------------------------------------------------------------
TGumpResizepic::TGumpResizepic(WORD graphic, short x, short y, short width, short height)
:TGumpObject(GOT_RESIZEPIC, graphic, 0, x, y), m_Width(width), m_Height(height)
{
}
//---------------------------------------------------------------------------
TGumpChecktrans::TGumpChecktrans(short x, short y, short width, short height)
: TGumpObject(GOT_CHECKTRANS, 0, 0, x, y), m_Width(width), m_Height(height)
{
	/*PDWORD pixels = new DWORD[width * height];

	IFOR(i, 0, width)
	{
		IFOR(j, 0, height)
		{
			DWORD val = 0;

			if ((i % 2) && (j % 2))
				val = 0xFFFFFFFF;

			pixels[j * width + i] = val;
		}
	}

	g_GL.BindTexture(Texture, width, height, pixels);

	delete pixels;*/
}
//---------------------------------------------------------------------------
TGumpChecktrans::~TGumpChecktrans()
{
	if (Texture != 0)
	{
		glDeleteTextures(1, &Texture);
		Texture = 0;
	}
}
//---------------------------------------------------------------------------
TGumpButton::TGumpButton(WORD graphic, WORD graphicLighted, WORD graphicPressed, int index, int toPage, bool action, short x, short y)
: TGumpObject(GOT_BUTTON, graphic, 0, x, y), m_GraphicLighted(graphicLighted),
m_GraphicPressed(graphicPressed), m_Index(index), m_ToPage(toPage), m_Action(action)
{
}
//---------------------------------------------------------------------------
TGumpButtonTileArt::TGumpButtonTileArt(WORD graphic, WORD graphicLighted, WORD graphicPressed, int index, int toPage, bool action, short x, short y, WORD tileGraphic, WORD tileColor, short tileX, short tileY)
: TGumpButton(graphic, graphicLighted, graphicPressed, index, toPage, action, x, y),
m_TileGraphic(tileGraphic), m_TileColor(tileColor), m_TileX(tileX), m_TileY(tileY)
{
	Type = GOT_BUTTONTILEART;
}
//---------------------------------------------------------------------------
TGumpCheckbox::TGumpCheckbox(WORD graphic, WORD graphicChecked, WORD graphicDisabled, int index, bool action, short x, short y)
: TGumpObject(GOT_CHECKBOX, graphic, 0, x, y), m_GraphicChecked(graphicChecked),
m_GraphicDisabled(graphicDisabled), m_Index(index), m_Action(action)
{
}
//---------------------------------------------------------------------------
TGumpRadio::TGumpRadio(WORD graphic, WORD graphicChecked, WORD graphicDisabled, int index, bool action, short x, short y)
: TGumpObject(GOT_RADIO, graphic, 0, x, y), m_GraphicChecked(graphicChecked),
m_GraphicDisabled(graphicDisabled), m_Index(index), m_Action(action)
{
}
//---------------------------------------------------------------------------
TGumpText::TGumpText(int textIndex, WORD color, short x, short y)
: TGumpObject(GOT_TEXT, 0, color, x, y), m_TextIndex(textIndex)
{
}
//---------------------------------------------------------------------------
TGumpText::~TGumpText()
{
	m_Text.Clear();
}
//---------------------------------------------------------------------------
TGumpCroppedText::TGumpCroppedText(int textIndex, WORD color, short x, short y, short width, short height)
: TGumpText(textIndex, color, x, y), m_Width(width), m_Height(height)
{
	Type = GOT_CROPPEDTEXT;
}
//---------------------------------------------------------------------------
TGumpTextEntry::TGumpTextEntry(int textIndex, WORD color, short x, short y, short width, short height, int index)
: TGumpText(textIndex, color, x, y), m_Width(width), m_Height(height), m_Index(index)
{
	Type = GOT_TEXTENTRY;
	TextEntry = new TEntryText();
}
//---------------------------------------------------------------------------
TGumpTextEntry::~TGumpTextEntry()
{
	if (TextEntry != NULL)
	{
		delete TextEntry;
		TextEntry = NULL;
	}
}
//---------------------------------------------------------------------------
TGumpTextEntryLimited::TGumpTextEntryLimited(int textIndex, WORD color, short x, short y, short width, short height, int index, int maxLength)
: TGumpTextEntry(textIndex, color, x, y, width, height, index), m_MaxLength(maxLength)
{
	Type = GOT_TEXTENTRYLIMITED;
}
//---------------------------------------------------------------------------
TGumpTilepic::TGumpTilepic(WORD graphic, short x, short y)
: TGumpObject(GOT_TILEPIC, graphic, 0, x, y)
{
}
//---------------------------------------------------------------------------
TGumpTilepicHue::TGumpTilepicHue(WORD graphic, WORD color, short x, short y)
: TGumpObject(GOT_TILEPICHUE, graphic, color, x, y)
{
}
//---------------------------------------------------------------------------
TGumpGumppic::TGumpGumppic(WORD graphic, WORD color, short x, short y)
: TGumpObject(GOT_GUMPPIC, graphic, color, x, y)
{
}
//---------------------------------------------------------------------------
TGumpGumppicTiled::TGumpGumppicTiled(WORD graphic, short x, short y, short width, short height)
: TGumpObject(GOT_GUMPPICTILED, graphic, 0, x, y), m_Width(width), m_Height(height)
{
}
//---------------------------------------------------------------------------
TGumpTooltip::TGumpTooltip(int clilocID)
: TGumpObject(GOT_TOOLTIP, 0, 0, 0, 0), m_ClilocID(clilocID)
{
}
//---------------------------------------------------------------------------
TGumpHTMLGump::TGumpHTMLGump(int textIndex, short x, short y, short width, short height, bool haveBackground, bool haveScrollbar)
: TGumpObject(GOT_HTMLGUMP, 0, 0, x, y), m_Width(width), m_Height(height),
m_TextIndex(textIndex), m_HaveBackground(haveBackground),
m_HaveScrollbar(haveScrollbar), m_LineOffset(0), m_BarOffset(0),
m_HTMLText(L"")
{
}
//---------------------------------------------------------------------------
TGumpHTMLGump::~TGumpHTMLGump()
{
	m_Text.Clear();
}
//---------------------------------------------------------------------------
TGumpXFMHTMLGump::TGumpXFMHTMLGump(DWORD clilocID, WORD color, short x, short y, short width, short height, bool haveBackground, bool haveScrollbar)
: TGumpHTMLGump((int)clilocID, x, y, width, height, haveBackground, haveScrollbar)
{
	Color = color;

	int w = width;
	if (haveScrollbar)
		w -= 16;

	if (w > 10)
	{
		if (!haveBackground)
		{
			//color = 0xFFFF;
			if (!haveScrollbar)
				FontManager->SetUseHTML(true, 0xFF010101);
			else
				FontManager->SetUseHTML(true);
		}
		else
		{
			w -= 9;
			FontManager->SetUseHTML(true, 0xFF010101);
		}

		TCliloc *cliloc = ClilocManager->Cliloc(g_Language.c_str());
		wstring text = cliloc->GetW(clilocID);
		
		FontManager->GenerateW(1, m_Text, text.c_str(), color, 30, w);
		FontManager->SetUseHTML(false);
	}
}
//---------------------------------------------------------------------------
TGumpXFMHTMLGump::~TGumpXFMHTMLGump()
{
	m_Text.Clear();
}
//---------------------------------------------------------------------------
TGumpMenuObject::TGumpMenuObject(WORD graphic, WORD color, string text)
: TGumpObject(GOT_BUTTON, graphic, color, 0, 0), m_Pressed(false), m_Text(text)
{
}
//---------------------------------------------------------------------------
TGumpSellObject::TGumpSellObject()
: TGumpObject(GOT_BUTTON, 0, 0, 0, 0), m_Count(0), m_Price(0), m_Name(""),
m_Selected(false), m_SelectedCount(0)
{
}
//---------------------------------------------------------------------------
TGumpBulletinBoardObject::TGumpBulletinBoardObject(DWORD serial, string text)
: TGumpObject(GOT_BUTTON, 0, 0, 0, 0)
{
	Serial = serial;

	FontManager->GenerateA(9, Texture, text.c_str(), 0x0386);
}
//---------------------------------------------------------------------------
TGumpBulletinBoardObject::~TGumpBulletinBoardObject()
{
	Texture.Clear();
}
//---------------------------------------------------------------------------