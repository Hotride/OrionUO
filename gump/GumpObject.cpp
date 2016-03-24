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
void TGumpObject::Draw(bool &transparent, bool pressed)
{
	if (transparent)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		UO->DrawGump(m_Graphic, m_Color, m_X, m_Y);

		glDisable(GL_BLEND);

		glEnable(GL_STENCIL_TEST);

		UO->DrawGump(m_Graphic, m_Color, m_X, m_Y);

		glDisable(GL_STENCIL_TEST);
	}
	else
		UO->DrawGump(m_Graphic, m_Color, m_X, m_Y);
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
void TGumpResizepic::Draw(bool &transparent, bool pressed)
{
	if (transparent)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		UO->DrawResizepicGump(m_Graphic, m_X, m_Y, m_Width, m_Height);

		glDisable(GL_BLEND);

		glEnable(GL_STENCIL_TEST);

		UO->DrawResizepicGump(m_Graphic, m_X, m_Y, m_Width, m_Height);

		glDisable(GL_STENCIL_TEST);
	}
	else
		UO->DrawResizepicGump(m_Graphic, m_X, m_Y, m_Width, m_Height);
}
//---------------------------------------------------------------------------
TGumpChecktrans::TGumpChecktrans(short x, short y, short width, short height)
: TGumpObject(GOT_CHECKTRANS, 0, 0, x, y), m_Width(width), m_Height(height)
{
}
//---------------------------------------------------------------------------
void TGumpChecktrans::Draw(bool &transparent, bool pressed)
{
	glColorMask(false, false, false, false);

	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	g_GL.DrawPolygone(m_X, m_Y, m_Width, m_Height);

	glColorMask(true, true, true, true);

	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilFunc(GL_NOTEQUAL, 1, 1);
}
//---------------------------------------------------------------------------
TGumpChecktrans::~TGumpChecktrans()
{
}
//---------------------------------------------------------------------------
TGumpButton::TGumpButton(WORD graphic, WORD graphicLighted, WORD graphicPressed, int index, int toPage, bool action, short x, short y)
: TGumpObject(GOT_BUTTON, graphic, 0, x, y), m_GraphicLighted(graphicLighted),
m_GraphicPressed(graphicPressed), m_Index(index), m_ToPage(toPage), m_Action(action)
{
}
//---------------------------------------------------------------------------
void TGumpButton::Draw(bool &transparent, bool pressed)
{
	WORD graphic = pressed ? m_GraphicPressed : m_Graphic;

	if (transparent)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		UO->DrawGump(graphic, m_Color, m_X, m_Y);

		glDisable(GL_BLEND);

		glEnable(GL_STENCIL_TEST);

		UO->DrawGump(graphic, m_Color, m_X, m_Y);

		glDisable(GL_STENCIL_TEST);
	}
	else
		UO->DrawGump(graphic, m_Color, m_X, m_Y);
}
//---------------------------------------------------------------------------
TGumpButtonTileArt::TGumpButtonTileArt(WORD graphic, WORD graphicLighted, WORD graphicPressed, int index, int toPage, bool action, short x, short y, WORD tileGraphic, WORD tileColor, short tileX, short tileY)
: TGumpButton(graphic, graphicLighted, graphicPressed, index, toPage, action, x, y),
m_TileGraphic(tileGraphic), m_TileColor(tileColor), m_TileX(tileX), m_TileY(tileY)
{
	m_Type = GOT_BUTTONTILEART;
}
//---------------------------------------------------------------------------
void TGumpButtonTileArt::Draw(bool &transparent, bool pressed)
{
	TGumpButton::Draw(transparent, pressed);

	ColorizerShader->Use();

	if (transparent)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		UO->DrawStaticArtInContainer(m_TileGraphic, m_TileColor, m_X + m_TileX, m_Y + m_TileY);

		glDisable(GL_BLEND);

		glEnable(GL_STENCIL_TEST);

		UO->DrawStaticArtInContainer(m_TileGraphic, m_TileColor, m_X + m_TileX, m_Y + m_TileY);

		glDisable(GL_STENCIL_TEST);
	}
	else
		UO->DrawStaticArtInContainer(m_TileGraphic, m_TileColor, m_X + m_TileX, m_Y + m_TileY);

	UnuseShader();
}
//---------------------------------------------------------------------------
TGumpCheckbox::TGumpCheckbox(WORD graphic, WORD graphicChecked, WORD graphicDisabled, int index, bool action, short x, short y)
: TGumpObject(GOT_CHECKBOX, graphic, 0, x, y), m_GraphicChecked(graphicChecked),
m_GraphicDisabled(graphicDisabled), m_Index(index), m_Action(action)
{
}
//---------------------------------------------------------------------------
void TGumpCheckbox::Draw(bool &transparent, bool pressed)
{
	WORD graphic = pressed ? m_GraphicChecked : m_Graphic;

	if (transparent)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		UO->DrawGump(graphic, m_Color, m_X, m_Y);

		glDisable(GL_BLEND);

		glEnable(GL_STENCIL_TEST);

		UO->DrawGump(graphic, m_Color, m_X, m_Y);

		glDisable(GL_STENCIL_TEST);
	}
	else
		UO->DrawGump(graphic, m_Color, m_X, m_Y);
}
//---------------------------------------------------------------------------
TGumpRadio::TGumpRadio(WORD graphic, WORD graphicChecked, WORD graphicDisabled, int index, bool action, short x, short y)
: TGumpObject(GOT_RADIO, graphic, 0, x, y), m_GraphicChecked(graphicChecked),
m_GraphicDisabled(graphicDisabled), m_Index(index), m_Action(action)
{
}
//---------------------------------------------------------------------------
void TGumpRadio::Draw(bool &transparent, bool pressed)
{
	WORD graphic = pressed ? m_GraphicChecked : m_Graphic;

	if (transparent)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		UO->DrawGump(graphic, m_Color, m_X, m_Y);

		glDisable(GL_BLEND);

		glEnable(GL_STENCIL_TEST);

		UO->DrawGump(graphic, m_Color, m_X, m_Y);

		glDisable(GL_STENCIL_TEST);
	}
	else
		UO->DrawGump(graphic, m_Color, m_X, m_Y);
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
void TGumpText::Draw(bool &transparent, bool pressed)
{
	if (transparent)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_Text.Draw(m_X, m_Y);

		glDisable(GL_BLEND);

		glEnable(GL_STENCIL_TEST);

		m_Text.Draw(m_X, m_Y);

		glDisable(GL_STENCIL_TEST);
	}
	else
		m_Text.Draw(m_X, m_Y);
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
void TGumpTextEntry::Draw(bool &transparent, bool pressed)
{
	if (TextEntry != NULL)
	{
		WORD color = m_Color;
		if (color)
			color++;

		if (transparent)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			TextEntry->DrawW((BYTE)(ConnectionManager.ClientVersion > CV_OLD), color, m_X, m_Y, TS_LEFT, UOFONT_BLACK_BORDER);

			glDisable(GL_BLEND);

			glEnable(GL_STENCIL_TEST);

			TextEntry->DrawW((BYTE)(ConnectionManager.ClientVersion > CV_OLD), color, m_X, m_Y, TS_LEFT, UOFONT_BLACK_BORDER);

			glDisable(GL_STENCIL_TEST);
		}
		else
			TextEntry->DrawW((BYTE)(ConnectionManager.ClientVersion > CV_OLD), color, m_X, m_Y, TS_LEFT, UOFONT_BLACK_BORDER);
	}
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
TGumpTilepic::TGumpTilepic(WORD graphic, WORD color, short x, short y)
: TGumpObject(GOT_TILEPIC, graphic, color, x, y)
{
}
//---------------------------------------------------------------------------
void TGumpTilepic::Draw(bool &transparent, bool pressed)
{
	if (transparent)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		UO->DrawStaticArtInContainer(m_Graphic, m_Color, m_X, m_Y);

		glDisable(GL_BLEND);

		glEnable(GL_STENCIL_TEST);

		UO->DrawStaticArtInContainer(m_Graphic, m_Color, m_X, m_Y);

		glDisable(GL_STENCIL_TEST);
	}
	else
		UO->DrawStaticArtInContainer(m_Graphic, m_Color, m_X, m_Y);
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
void TGumpGumppicTiled::Draw(bool &transparent, bool pressed)
{
	if (transparent)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		UO->DrawGump(m_Graphic, m_Color, m_X, m_Y, m_Width, m_Height);

		glDisable(GL_BLEND);

		glEnable(GL_STENCIL_TEST);

		UO->DrawGump(m_Graphic, m_Color, m_X, m_Y, m_Width, m_Height);

		glDisable(GL_STENCIL_TEST);
	}
	else
		UO->DrawGump(m_Graphic, m_Color, m_X, m_Y, m_Width, m_Height);
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

		TCliloc *cliloc = ClilocManager->Cliloc(g_Language);
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