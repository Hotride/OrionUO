/****************************************************************************
**
** GumpInterfaceObject.h
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
#ifndef GumpInterfaceObjectH
#define GumpInterfaceObjectH
//---------------------------------------------------------------------------
class TGumpInterfaceObject : public TRenderObject
{
protected:
	GUMP_OBJECT_TYPE m_Type;
	WORD m_GraphicSelected;
	WORD m_GraphicPressed;
	bool m_IsChecked;
public:
	TGumpInterfaceObject(GUMP_OBJECT_TYPE type, WORD graphic, WORD graphicSelected, WORD graphicPressed, WORD color, short x, short y, DWORD serial);
	virtual ~TGumpInterfaceObject() {}

	SETGET(GUMP_OBJECT_TYPE, Type);
	SETGET(WORD, GraphicSelected);
	SETGET(WORD, GraphicPressed);
	SETGET(bool, IsChecked);

	virtual void PrepareTextures();

	virtual DWORD Draw(const bool &useTransparentStencil = false);
};
//---------------------------------------------------------------------------
class TGumpInterfacePage : public TGumpInterfaceObject
{
protected:
	int m_Page;
public:
	TGumpInterfacePage(int page, DWORD serial);
	virtual ~TGumpInterfacePage() {}

	SETGET(int, Page);
};
//---------------------------------------------------------------------------
class TGumpInterfaceGroup : public TGumpInterfaceObject
{
protected:
	int m_Group;
public:
	TGumpInterfaceGroup(int group, DWORD serial);
	virtual ~TGumpInterfaceGroup() {}

	SETGET(int, Group);
};
//---------------------------------------------------------------------------
class TGumpInterfaceEndGroup : public TGumpInterfaceObject
{
public:
	TGumpInterfaceEndGroup(DWORD serial);
	virtual ~TGumpInterfaceEndGroup() {}
};
//---------------------------------------------------------------------------
class TGumpInterfacePolygonal : public TGumpInterfaceObject
{
protected:
	short m_Width;
	short m_Height;
	bool m_CheckGump;

public:
	TGumpInterfacePolygonal(short x, short y, short width, short height, DWORD serial);
	virtual ~TGumpInterfacePolygonal() {}

	SETGET(short, Width);
	SETGET(short, Height);
};
//---------------------------------------------------------------------------
class TGumpInterfaceResizepic : public TGumpInterfacePolygonal
{
public:
	TGumpInterfaceResizepic(WORD graphic, short x, short y, short width, short height, DWORD serial);
	virtual ~TGumpInterfaceResizepic() {}

	virtual void PrepareTextures();

	virtual DWORD Draw(const bool &useTransparentStencil = false);
};
//---------------------------------------------------------------------------
class TGumpInterfaceChecktrans : public TGumpInterfacePolygonal
{
public:
	TGumpInterfaceChecktrans(short x, short y, short width, short height, DWORD serial);
	virtual ~TGumpInterfaceChecktrans();

	virtual DWORD Draw(const bool &useTransparentStencil = false);
};
//---------------------------------------------------------------------------
class TGumpInterfaceBoundingBox : public TGumpInterfacePolygonal
{
protected:
	bool m_Cut;
	int m_OffsetY;
public:
	TGumpInterfaceBoundingBox(bool cut, short x, short y, short width, short height, DWORD serial);
	virtual ~TGumpInterfaceBoundingBox();

	SETGET(bool, Cut);
	SETGET(int, OffsetY);

	virtual void PrepareTextures();

	virtual DWORD Draw(const bool &useTransparentStencil = false);
};
//---------------------------------------------------------------------------
class TGumpInterfaceButton : public TGumpInterfaceObject
{
public:
	TGumpInterfaceButton(WORD graphic, WORD graphicSelected, WORD graphicPressed, short x, short y, DWORD serial);
	virtual ~TGumpInterfaceButton() {}
};
//---------------------------------------------------------------------------
class TGumpInterfaceButtonTileArt : public TGumpInterfaceButton
{
protected:
	WORD m_TileGraphic;
	WORD m_TileColor;
	short m_TileX;
	short m_TileY;
public:
	TGumpInterfaceButtonTileArt(WORD graphic, WORD graphicSelected, WORD graphicPressed, short x, short y, WORD tileGraphic, WORD tileColor, short tileX, short tileY, DWORD serial);
	virtual ~TGumpInterfaceButtonTileArt() {}

	SETGET(WORD, TileGraphic);
	SETGET(WORD, TileColor);
	SETGET(short, TileX);
	SETGET(short, TileY);

	virtual void PrepareTextures();

	virtual DWORD Draw(const bool &useTransparentStencil = false);
};
//---------------------------------------------------------------------------
class TGumpInterfaceCheckbox : public TGumpInterfaceObject
{
public:
	TGumpInterfaceCheckbox(WORD graphic, WORD graphicChecked, WORD graphicDisabled, bool checked, short x, short y, DWORD serial);
	virtual ~TGumpInterfaceCheckbox() {}
};
//---------------------------------------------------------------------------
class TGumpInterfaceRadio : public TGumpInterfaceObject
{
public:
	TGumpInterfaceRadio(WORD graphic, WORD graphicChecked, WORD graphicDisabled, bool checked, short x, short y, DWORD serial);
	virtual ~TGumpInterfaceRadio() {}
};
//---------------------------------------------------------------------------
class TGumpInterfaceText : public TGumpInterfaceObject
{
public:
	TGumpInterfaceText(WORD color, short x, short y, DWORD serial);
	virtual ~TGumpInterfaceText();

	TTextTexture m_Text;

	void CreateTextTextureA(const BYTE &font, const string &str, int width = 0, const TEXT_ALIGN_TYPE &align = TS_LEFT, const WORD &flags = 0);
	void CreateTextTextureW(const BYTE &font, const wstring &str, const BYTE &cell = 30, int width = 0, const TEXT_ALIGN_TYPE &align = TS_LEFT, const WORD &flags = 0);

	virtual DWORD Draw(const bool &useTransparentStencil = false);
};
//---------------------------------------------------------------------------
class TGumpInterfaceTextEntry : public TGumpInterfaceObject
{
protected:
	WORD m_ColorSelected;
	WORD m_ColorEntry;
	bool m_Unicode;
	BYTE m_Font;
	TEXT_ALIGN_TYPE m_Align;
	WORD m_TextFlags;

public:
	TGumpInterfaceTextEntry(WORD color, WORD colorSelected, WORD colorEntry, short x, short y, short maxWidth, bool unicode, BYTE font, TEXT_ALIGN_TYPE align, WORD textFlags, DWORD serial);
	virtual ~TGumpInterfaceTextEntry();

	TEntryText *TextEntry;

	SETGET(bool, Unicode);
	SETGET(BYTE, Font);
	SETGET(TEXT_ALIGN_TYPE, Align);
	SETGET(WORD, TextFlags);

	virtual DWORD Draw(const bool &useTransparentStencil = false);
};
//---------------------------------------------------------------------------
class TGumpInterfaceTextEntryLimited : public TGumpInterfaceTextEntry
{
protected:
	int m_MaxLength;
public:
	TGumpInterfaceTextEntryLimited(WORD color, WORD colorSelected, WORD colorEntry, short x, short y, short maxWidth, int maxLength, bool unicode, BYTE font, TEXT_ALIGN_TYPE align, WORD textFlags, DWORD serial);
	virtual ~TGumpInterfaceTextEntryLimited() {}

	SETGET(int, MaxLength);
};
//---------------------------------------------------------------------------
class TGumpInterfaceTilepic : public TGumpInterfaceObject
{
public:
	TGumpInterfaceTilepic(WORD graphic, WORD color, short x, short y, DWORD serial);
	virtual ~TGumpInterfaceTilepic() {}

	virtual void PrepareTextures();

	virtual DWORD Draw(const bool &useTransparentStencil = false);
};
//---------------------------------------------------------------------------
class TGumpInterfaceGumppic : public TGumpInterfaceObject
{
public:
	TGumpInterfaceGumppic(WORD graphic, WORD color, short x, short y, DWORD serial);
	virtual ~TGumpInterfaceGumppic() {}
};
//---------------------------------------------------------------------------
class TGumpInterfaceGumppicTiled : public TGumpInterfacePolygonal
{
public:
	TGumpInterfaceGumppicTiled(WORD graphic, WORD color, short x, short y, short width, short height, DWORD serial);
	virtual ~TGumpInterfaceGumppicTiled() {}

	virtual DWORD Draw(const bool &useTransparentStencil = false);
};
//---------------------------------------------------------------------------
class TGumpInterfaceTooltip : public TGumpInterfaceObject
{
protected:
	int m_ClilocID;
public:
	TGumpInterfaceTooltip(int clilocID);
	virtual ~TGumpInterfaceTooltip() {}

	SETGET(int, ClilocID);
};
//---------------------------------------------------------------------------
class TGumpInterfaceHTMLGump : public TGumpInterfaceObject
{
protected:
	short m_Width;
	short m_Height;
	int m_TextIndex;
	bool m_HaveBackground;
	bool m_HaveScrollbar;
	int m_LineOffset;
	int m_BarOffset;
	wstring m_HTMLText;
public:
	TGumpInterfaceHTMLGump(int textIndex, short x, short y, short width, short height, bool haveBackground, bool haveScrollbar, DWORD serial);
	virtual ~TGumpInterfaceHTMLGump();
	
	THTMLTextTexture m_Text;

	SETGET(short, Width);
	SETGET(short, Height);
	SETGET(int, TextIndex);
	SETGET(bool, HaveBackground);
	SETGET(bool, HaveScrollbar);
	SETGETEX(int, LineOffset);
	SETGETEX(int, BarOffset);
	SETGET(wstring, HTMLText);
};
//---------------------------------------------------------------------------
class TGumpInterfaceXMFHTMLGump : public TGumpInterfaceHTMLGump
{
public:
	TGumpInterfaceXMFHTMLGump(DWORD clilocID, WORD color, short x, short y, short width, short height, bool haveBackground, bool haveScrollbar, DWORD serial);
	virtual ~TGumpInterfaceXMFHTMLGump();
};
//---------------------------------------------------------------------------
#endif