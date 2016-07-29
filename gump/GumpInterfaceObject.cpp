/****************************************************************************
**
** GumpInterfaceObject.cpp
**
** Copyright (C) July 2016 Hotride
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
TGumpInterfaceObject::TGumpInterfaceObject(GUMP_OBJECT_TYPE type, WORD graphic, WORD graphicSelected, WORD graphicPressed, WORD color, short x, short y, DWORD serial)
: TRenderObject(serial, graphic, color, x, y), m_Type(type), m_GraphicSelected(graphicSelected),
m_GraphicPressed(graphicPressed), m_IsChecked(false)
{
}
//---------------------------------------------------------------------------
void TGumpInterfaceObject::PrepareTextures()
{
	if (m_Graphic)
		Orion->ExecuteGump(m_Graphic);

	if (m_GraphicSelected)
		Orion->ExecuteGump(m_GraphicSelected);

	if (m_GraphicPressed)
		Orion->ExecuteGump(m_GraphicPressed);
}
//---------------------------------------------------------------------------
DWORD TGumpInterfaceObject::Draw(const bool &useTransparentStencil)
{
	if (m_Graphic)
	{
		if (g_DrawMode)
		{
			WORD graphic = m_Graphic;

			if (m_IsChecked || g_GumpPressedElement == m_Serial)
				graphic = m_GraphicPressed;
			else if (g_GumpSelectElement == m_Serial)
				graphic = m_GraphicSelected;

			if (useTransparentStencil)
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				Orion->DrawGump(graphic, m_Color, m_X, m_Y);

				glDisable(GL_BLEND);

				glEnable(GL_STENCIL_TEST);

				Orion->DrawGump(graphic, m_Color, m_X, m_Y);

				glDisable(GL_STENCIL_TEST);
			}
			else
				Orion->DrawGump(graphic, m_Color, m_X, m_Y);
		}
		else
		{
			if (Orion->GumpPixelsInXY(m_Graphic, m_X, m_Y))
				return m_Serial;
		}
	}

	return 0;
}
//---------------------------------------------------------------------------
TGumpInterfacePage::TGumpInterfacePage(int page, DWORD serial)
: TGumpInterfaceObject(GOT_PAGE, 0, 0, 0, 0, 0, 0, serial), m_Page(page)
{
}
//---------------------------------------------------------------------------
TGumpInterfaceGroup::TGumpInterfaceGroup(int group, DWORD serial)
: TGumpInterfaceObject(GOT_GROUP, 0, 0, 0, 0, 0, 0, serial), m_Group(group)
{
}
//---------------------------------------------------------------------------
TGumpInterfaceEndGroup::TGumpInterfaceEndGroup(DWORD serial)
: TGumpInterfaceObject(GOT_ENDGROUP, 0, 0, 0, 0, 0, 0, serial)
{
}
//---------------------------------------------------------------------------
TGumpInterfacePolygonal::TGumpInterfacePolygonal(short x, short y, short width, short height, DWORD serial)
: TGumpInterfaceObject(GOT_RESIZEPIC, 0, 0, 0, 0, x, y, serial), m_Width(width), m_Height(height)
{
}
//---------------------------------------------------------------------------
TGumpInterfaceResizepic::TGumpInterfaceResizepic(WORD graphic, short x, short y, short width, short height, DWORD serial)
: TGumpInterfacePolygonal(x, y, width, height, serial)
{
	m_Type = GOT_RESIZEPIC;
	m_Graphic = graphic;
}
//---------------------------------------------------------------------------
void TGumpInterfaceResizepic::PrepareTextures()
{
	if (m_Graphic)
		Orion->ExecuteResizepic(m_Graphic);
}
//---------------------------------------------------------------------------
DWORD TGumpInterfaceResizepic::Draw(const bool &useTransparentStencil)
{
	if (g_DrawMode)
	{
		if (useTransparentStencil)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			Orion->DrawResizepicGump(m_Graphic, m_X, m_Y, m_Width, m_Height);

			glDisable(GL_BLEND);

			glEnable(GL_STENCIL_TEST);

			Orion->DrawResizepicGump(m_Graphic, m_X, m_Y, m_Width, m_Height);

			glDisable(GL_STENCIL_TEST);
		}
		else
			Orion->DrawResizepicGump(m_Graphic, m_X, m_Y, m_Width, m_Height);
	}
	else
	{
		if (m_CheckGump)
		{
			if (m_Graphic && Orion->ResizepicPixelsInXY(m_Graphic, m_X, m_Y, m_Width, m_Height))
				return m_Serial;
		}
		else if (Orion->PolygonePixelsInXY(m_X, m_Y, m_Width, m_Height))
			return m_Serial;
	}

	return 0;
}
//---------------------------------------------------------------------------
TGumpInterfaceChecktrans::TGumpInterfaceChecktrans(short x, short y, short width, short height, DWORD serial)
: TGumpInterfacePolygonal(x, y, width, height, serial)
{
	m_Type = GOT_CHECKTRANS;
}
//---------------------------------------------------------------------------
TGumpInterfaceChecktrans::~TGumpInterfaceChecktrans()
{
}
//---------------------------------------------------------------------------
DWORD TGumpInterfaceChecktrans::Draw(const bool &useTransparentStencil)
{
	if (g_DrawMode)
	{
		glColorMask(false, false, false, false);

		glStencilFunc(GL_ALWAYS, 1, 1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		g_GL.DrawPolygone(m_X, m_Y, m_Width, m_Height);

		glColorMask(true, true, true, true);

		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc(GL_NOTEQUAL, 1, 1);
	}

	return 0;
}
//---------------------------------------------------------------------------
TGumpInterfaceBoundingBox::TGumpInterfaceBoundingBox(bool cut, short x, short y, short width, short height, DWORD serial)
: TGumpInterfacePolygonal(x, y, width, height, serial), m_Cut(cut),
m_OffsetY(0)
{
	m_Type = GOT_CHECKTRANS;
}
//---------------------------------------------------------------------------
TGumpInterfaceBoundingBox::~TGumpInterfaceBoundingBox()
{
}
//---------------------------------------------------------------------------
void TGumpInterfaceBoundingBox::PrepareTextures()
{
	QFOR(item, m_Items, TGumpInterfaceObject*)
		item->PrepareTextures();
}
//---------------------------------------------------------------------------
DWORD TGumpInterfaceBoundingBox::Draw(const bool &useTransparentStencil)
{
	DWORD result = 0;

	if (m_Cut)
	{
		//Допилить отрезалку
		QFOR(item, m_Items, TGumpInterfaceObject*)
		{
			if (!result)
				result = item->Draw(useTransparentStencil);
		}
	}
	else
	{
		QFOR(item, m_Items, TGumpInterfaceObject*)
		{
			if (!result)
				result = item->Draw(useTransparentStencil);
		}
	}

	return result;
}
//---------------------------------------------------------------------------
TGumpInterfaceButton::TGumpInterfaceButton(WORD graphic, WORD graphicSelected, WORD graphicPressed, short x, short y, DWORD serial)
: TGumpInterfaceObject(GOT_BUTTON, graphic, graphicSelected, graphicPressed, 0, x, y, serial)
{
}
//---------------------------------------------------------------------------
TGumpInterfaceButtonTileArt::TGumpInterfaceButtonTileArt(WORD graphic, WORD graphicSelected, WORD graphicPressed, short x, short y, WORD tileGraphic, WORD tileColor, short tileX, short tileY, DWORD serial)
: TGumpInterfaceButton(graphic, graphicSelected, graphicPressed, x, y, serial),
m_TileGraphic(tileGraphic), m_TileColor(tileColor), m_TileX(tileX), m_TileY(tileY)
{
	m_Type = GOT_BUTTONTILEART;
}
//---------------------------------------------------------------------------
void TGumpInterfaceButtonTileArt::PrepareTextures()
{
	TGumpInterfaceObject::PrepareTextures();

	if (m_TileGraphic)
		Orion->ExecuteStaticArt(m_TileGraphic);
}
//---------------------------------------------------------------------------
DWORD TGumpInterfaceButtonTileArt::Draw(const bool &useTransparentStencil)
{
	DWORD result = TGumpInterfaceObject::Draw(useTransparentStencil);

	if (g_DrawMode)
	{
		if (useTransparentStencil)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			Orion->DrawStaticArtInContainer(m_TileGraphic, m_TileColor, m_X + m_TileX, m_Y + m_TileY);

			glDisable(GL_BLEND);

			glEnable(GL_STENCIL_TEST);

			Orion->DrawStaticArtInContainer(m_TileGraphic, m_TileColor, m_X + m_TileX, m_Y + m_TileY);

			glDisable(GL_STENCIL_TEST);
		}
		else
			Orion->DrawStaticArtInContainer(m_TileGraphic, m_TileColor, m_X + m_TileX, m_Y + m_TileY);
	}
	else if (!result && Orion->StaticPixelsInXYInContainer(m_TileGraphic, m_X + m_TileX, m_Y + m_TileY))
		result = m_Serial;

	return result;
}
//---------------------------------------------------------------------------
TGumpInterfaceCheckbox::TGumpInterfaceCheckbox(WORD graphic, WORD graphicChecked, WORD graphicDisabled, bool checked, short x, short y, DWORD serial)
: TGumpInterfaceObject(GOT_CHECKBOX, graphic, graphicChecked, graphicDisabled, 0, x, y, serial)
{
	m_IsChecked = checked;
}
//---------------------------------------------------------------------------
TGumpInterfaceRadio::TGumpInterfaceRadio(WORD graphic, WORD graphicChecked, WORD graphicDisabled, bool checked, short x, short y, DWORD serial)
: TGumpInterfaceObject(GOT_RADIO, graphic, graphicChecked, graphicDisabled, 0, x, y, serial)
{
	m_IsChecked = checked;
}
//---------------------------------------------------------------------------
TGumpInterfaceText::TGumpInterfaceText(WORD color, short x, short y, DWORD serial)
: TGumpInterfaceObject(GOT_TEXT, 0, 0, 0, color, x, y, serial)
{
}
//---------------------------------------------------------------------------
TGumpInterfaceText::~TGumpInterfaceText()
{
	m_Text.Clear();
}
//---------------------------------------------------------------------------
void TGumpInterfaceText::CreateTextTextureA(const BYTE &font, const string &str, int width, const TEXT_ALIGN_TYPE &align, const WORD &flags)
{
	FontManager->GenerateA(font, m_Text, str.c_str(), m_Color, width, align, flags);
}
//---------------------------------------------------------------------------
void TGumpInterfaceText::CreateTextTextureW(const BYTE &font, const wstring &str, const BYTE &cell, int width, const TEXT_ALIGN_TYPE &align, const WORD &flags)
{
	FontManager->GenerateW(font, m_Text, str.c_str(), m_Color, cell, width, align, flags);
}
//---------------------------------------------------------------------------
DWORD TGumpInterfaceText::Draw(const bool &useTransparentStencil)
{
	if (g_DrawMode)
	{
		if (useTransparentStencil)
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
	else if (Orion->PolygonePixelsInXY(m_X, m_Y, m_Text.Width, m_Text.Height))
		return m_Serial;

	return 0;
}
//---------------------------------------------------------------------------
TGumpInterfaceTextEntry::TGumpInterfaceTextEntry(WORD color, WORD colorSelected, WORD colorEntry, short x, short y, short maxWidth, bool unicode, BYTE font, TEXT_ALIGN_TYPE align, WORD textFlags, DWORD serial)
: TGumpInterfaceObject(GOT_TEXTENTRY, 0, 0, 0, color, x, y, serial), m_ColorSelected(colorSelected),
m_ColorEntry(colorEntry), m_Unicode(unicode), m_Font(font), m_Align(align),
m_TextFlags(textFlags)
{
	TextEntry = new TEntryText(0, 0, maxWidth);
}
//---------------------------------------------------------------------------
TGumpInterfaceTextEntry::~TGumpInterfaceTextEntry()
{
	if (TextEntry != NULL)
	{
		delete TextEntry;
		TextEntry = NULL;
	}
}
//---------------------------------------------------------------------------
DWORD TGumpInterfaceTextEntry::Draw(const bool &useTransparentStencil)
{
	if (g_DrawMode)
	{
		if (TextEntry != NULL)
		{
			WORD color = m_Color;

			if (TextEntry == EntryPointer)
				color = m_ColorEntry;
			else if (g_GumpSelectElement == m_Serial)
				color = m_ColorSelected;

			if (color && m_Unicode)
				color++;

			if (useTransparentStencil)
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				if (m_Unicode)
					TextEntry->DrawW(m_Font, color, m_X, m_Y, m_Align, m_TextFlags);
				else
					TextEntry->DrawA(m_Font, color, m_X, m_Y, m_Align, m_TextFlags);

				glDisable(GL_BLEND);

				glEnable(GL_STENCIL_TEST);

				if (m_Unicode)
					TextEntry->DrawW(m_Font, color, m_X, m_Y, m_Align, m_TextFlags);
				else
					TextEntry->DrawA(m_Font, color, m_X, m_Y, m_Align, m_TextFlags);

				glDisable(GL_STENCIL_TEST);
			}
			else
			{
				if (m_Unicode)
					TextEntry->DrawW(m_Font, color, m_X, m_Y, m_Align, m_TextFlags);
				else
					TextEntry->DrawA(m_Font, color, m_X, m_Y, m_Align, m_TextFlags);
			}
		}
	}
	else if (Orion->PolygonePixelsInXY(m_X, m_Y, TextEntry->m_Texture.Width, TextEntry->m_Texture.Height))
		return m_Serial;

	return 0;
}
//---------------------------------------------------------------------------
TGumpInterfaceTextEntryLimited::TGumpInterfaceTextEntryLimited(WORD color, WORD colorSelected, WORD colorEntry, short x, short y, short maxWidth, int maxLength, bool unicode, BYTE font, TEXT_ALIGN_TYPE align, WORD textFlags, DWORD serial)
: TGumpInterfaceTextEntry(color, colorSelected, colorEntry, x, y, maxWidth, unicode, font, align, textFlags, serial),
m_MaxLength(maxLength)
{
	m_Type = GOT_TEXTENTRYLIMITED;

	if (TextEntry != NULL)
		TextEntry->MaxLength = MaxLength;
}
//---------------------------------------------------------------------------
TGumpInterfaceTilepic::TGumpInterfaceTilepic(WORD graphic, WORD color, short x, short y, DWORD serial)
: TGumpInterfaceObject(GOT_TILEPIC, graphic, 0, 0, color, x, y, serial)
{
}
//---------------------------------------------------------------------------
void TGumpInterfaceTilepic::PrepareTextures()
{
	TGumpInterfaceObject::PrepareTextures();

	if (m_Graphic)
		Orion->ExecuteStaticArt(m_Graphic);
}
//---------------------------------------------------------------------------
DWORD TGumpInterfaceTilepic::Draw(const bool &useTransparentStencil)
{
	if (g_DrawMode)
	{
		if (useTransparentStencil)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			Orion->DrawStaticArtInContainer(m_Graphic, m_Color, m_X, m_Y);

			glDisable(GL_BLEND);

			glEnable(GL_STENCIL_TEST);

			Orion->DrawStaticArtInContainer(m_Graphic, m_Color, m_X, m_Y);

			glDisable(GL_STENCIL_TEST);
		}
		else
			Orion->DrawStaticArtInContainer(m_Graphic, m_Color, m_X, m_Y);
	}
	else if (Orion->StaticPixelsInXYInContainer(m_Graphic, m_X, m_Y))
		return m_Serial;

	return 0;
}
//---------------------------------------------------------------------------
TGumpInterfaceGumppic::TGumpInterfaceGumppic(WORD graphic, WORD color, short x, short y, DWORD serial)
: TGumpInterfaceObject(GOT_GUMPPIC, graphic, 0, 0, color, x, y, serial)
{
}
//---------------------------------------------------------------------------
TGumpInterfaceGumppicTiled::TGumpInterfaceGumppicTiled(WORD graphic, WORD color, short x, short y, short width, short height, DWORD serial)
: TGumpInterfacePolygonal(x, y, width, height, serial)
{
	m_Type = GOT_GUMPPICTILED;
	m_Graphic = graphic;
	m_Color = color;
}
//---------------------------------------------------------------------------
DWORD TGumpInterfaceGumppicTiled::Draw(const bool &useTransparentStencil)
{
	if (g_DrawMode)
	{
		if (useTransparentStencil)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			Orion->DrawGump(m_Graphic, m_Color, m_X, m_Y, m_Width, m_Height);

			glDisable(GL_BLEND);

			glEnable(GL_STENCIL_TEST);

			Orion->DrawGump(m_Graphic, m_Color, m_X, m_Y, m_Width, m_Height);

			glDisable(GL_STENCIL_TEST);
		}
		else
			Orion->DrawGump(m_Graphic, m_Color, m_X, m_Y, m_Width, m_Height);
	}
	else if (Orion->GumpPixelsInXY(m_Graphic, m_X, m_Y, m_Width, m_Height))
		return m_Serial;

	return 0;
}
//---------------------------------------------------------------------------
TGumpInterfaceTooltip::TGumpInterfaceTooltip(int clilocID)
: TGumpInterfaceObject(GOT_TOOLTIP, 0, 0, 0, 0, 0, 0, 0), m_ClilocID(clilocID)
{
}
//---------------------------------------------------------------------------
TGumpInterfaceHTMLGump::TGumpInterfaceHTMLGump(int textIndex, short x, short y, short width, short height, bool haveBackground, bool haveScrollbar, DWORD serial)
: TGumpInterfaceObject(GOT_HTMLGUMP, 0, 0, 0, 0, x, y, serial), m_Width(width), m_Height(height),
m_TextIndex(textIndex), m_HaveBackground(haveBackground),
m_HaveScrollbar(haveScrollbar), m_LineOffset(0), m_BarOffset(0),
m_HTMLText(L"")
{
}
//---------------------------------------------------------------------------
TGumpInterfaceHTMLGump::~TGumpInterfaceHTMLGump()
{
	m_Text.Clear();
}
//---------------------------------------------------------------------------
TGumpInterfaceXMFHTMLGump::TGumpInterfaceXMFHTMLGump(DWORD clilocID, WORD color, short x, short y, short width, short height, bool haveBackground, bool haveScrollbar, DWORD serial)
: TGumpInterfaceHTMLGump((int)clilocID, x, y, width, height, haveBackground, haveScrollbar, serial)
{
	m_Color = color;

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
		
		FontManager->GenerateW(0, m_Text, text.c_str(), color, 30, w);
		FontManager->SetUseHTML(false);
	}
}
//---------------------------------------------------------------------------
TGumpInterfaceXMFHTMLGump::~TGumpInterfaceXMFHTMLGump()
{
	m_Text.Clear();
}
//---------------------------------------------------------------------------