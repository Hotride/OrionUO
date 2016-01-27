/****************************************************************************
**
** GumpObject.h
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
#ifndef GumpObjectH
#define GumpObjectH
//---------------------------------------------------------------------------
class TGumpObject : public TRenderObject
{
private:
	GUMP_OBJECT_TYPE m_Type;
public:
	TGumpObject(GUMP_OBJECT_TYPE type, WORD graphic, WORD color, short x, short y);
	virtual ~TGumpObject() {}

	SETGET(GUMP_OBJECT_TYPE, Type);
};
//---------------------------------------------------------------------------
class TGumpPage : public TGumpObject
{
private:
	int m_Page;
public:
	TGumpPage(int page);
	virtual ~TGumpPage() {}

	SETGET(int, Page);
};
//---------------------------------------------------------------------------
class TGumpGroup : public TGumpObject
{
private:
	int m_Group;
public:
	TGumpGroup(int group);
	virtual ~TGumpGroup() {}

	SETGET(int, Group);
};
//---------------------------------------------------------------------------
class TGumpEndGroup : public TGumpObject
{
private:
public:
	TGumpEndGroup();
	virtual ~TGumpEndGroup() {}
};
//---------------------------------------------------------------------------
class TGumpMasterGump : public TGumpObject
{
private:
	int m_Index;
public:
	TGumpMasterGump(int index);
	virtual ~TGumpMasterGump() {}

	SETGET(int, Index);
};
//---------------------------------------------------------------------------
class TGumpResizepic : public TGumpObject
{
private:
	short m_Width;
	short m_Height;
public:
	TGumpResizepic(WORD graphic, short x, short y, short width, short height);
	virtual ~TGumpResizepic() {}

	SETGET(short, Width);
	SETGET(short, Height);
};
//---------------------------------------------------------------------------
class TGumpChecktrans : public TGumpObject
{
private:
	short m_Width;
	short m_Height;
public:
	TGumpChecktrans(short x, short y, short width, short height);
	virtual ~TGumpChecktrans();

	SETGET(short, Width);
	SETGET(short, Height);
};
//---------------------------------------------------------------------------
class TGumpButton : public TGumpObject
{
private:
	WORD m_GraphicLighted;
	WORD m_GraphicPressed;
	int m_Index;
	int m_ToPage;
	bool m_Action;
public:
	TGumpButton(WORD graphic, WORD graphicLighted, WORD graphicPressed, int index, int toPage, bool action, short x, short y);
	virtual ~TGumpButton() {}

	SETGET(WORD, GraphicLighted);
	SETGET(WORD, GraphicPressed);
	SETGET(int, Index);
	SETGET(int, ToPage);
	SETGET(bool, Action);
};
//---------------------------------------------------------------------------
class TGumpButtonTileArt : public TGumpButton
{
private:
	WORD m_TileGraphic;
	WORD m_TileColor;
	short m_TileX;
	short m_TileY;
public:
	TGumpButtonTileArt(WORD graphic, WORD graphicLighted, WORD graphicPressed, int index, int toPage, bool action, short x, short y, WORD tileGraphic, WORD tileColor, short tileX, short tileY);
	virtual ~TGumpButtonTileArt() {}

	SETGET(WORD, TileGraphic);
	SETGET(WORD, TileColor);
	SETGET(short, TileX);
	SETGET(short, TileY);
};
//---------------------------------------------------------------------------
class TGumpCheckbox : public TGumpObject
{
private:
	WORD m_GraphicChecked;
	WORD m_GraphicDisabled;
	int m_Index;
	bool m_Action;
public:
	TGumpCheckbox(WORD graphic, WORD graphicChecked, WORD graphicDisabled, int index, bool action, short x, short y);
	virtual ~TGumpCheckbox() {}

	SETGET(WORD, GraphicChecked);
	SETGET(WORD, GraphicDisabled);
	SETGET(int, Index);
	SETGET(bool, Action);
};
//---------------------------------------------------------------------------
class TGumpRadio : public TGumpObject
{
private:
	WORD m_GraphicChecked;
	WORD m_GraphicDisabled;
	int m_Index;
	bool m_Action;
public:
	TGumpRadio(WORD graphic, WORD graphicChecked, WORD graphicDisabled, int index, bool action, short x, short y);
	virtual ~TGumpRadio() {}

	SETGET(WORD, GraphicChecked);
	SETGET(WORD, GraphicDisabled);
	SETGET(int, Index);
	SETGET(bool, Action);
};
//---------------------------------------------------------------------------
class TGumpText : public TGumpObject
{
private:
	int m_TextIndex;
public:
	TGumpText(int textIndex, WORD color, short x, short y);
	virtual ~TGumpText();

	TTextTexture m_Text;

	SETGET(int, TextIndex);
};
//---------------------------------------------------------------------------
class TGumpCroppedText : public TGumpText
{
private:
	short m_Width;
	short m_Height;
public:
	TGumpCroppedText(int textIndex, WORD color, short x, short y, short width, short height);
	virtual ~TGumpCroppedText() {}

	SETGET(short, Width);
	SETGET(short, Height);
};
//---------------------------------------------------------------------------
class TGumpTextEntry : public TGumpText
{
private:
	short m_Width;
	short m_Height;
	int m_Index;
public:
	TGumpTextEntry(int textIndex, WORD color, short x, short y, short width, short height, int index);
	virtual ~TGumpTextEntry();

	TEntryText *TextEntry;

	SETGET(short, Width);
	SETGET(short, Height);
	SETGET(int, Index);
};
//---------------------------------------------------------------------------
class TGumpTextEntryLimited : public TGumpTextEntry
{
private:
	int m_MaxLength;
public:
	TGumpTextEntryLimited(int textIndex, WORD color, short x, short y, short width, short height, int index, int maxLength);
	virtual ~TGumpTextEntryLimited() {}

	SETGET(int, MaxLength);
};
//---------------------------------------------------------------------------
class TGumpTilepic : public TGumpObject
{
private:
public:
	TGumpTilepic(WORD graphic, short x, short y);
	virtual ~TGumpTilepic() {}
};
//---------------------------------------------------------------------------
class TGumpTilepicHue : public TGumpObject
{
private:
public:
	TGumpTilepicHue(WORD graphic, WORD color, short x, short y);
	virtual ~TGumpTilepicHue() {}
};
//---------------------------------------------------------------------------
class TGumpGumppic : public TGumpObject
{
private:
public:
	TGumpGumppic(WORD graphic, WORD color, short x, short y);
	virtual ~TGumpGumppic() {}
};
//---------------------------------------------------------------------------
class TGumpGumppicTiled : public TGumpObject
{
private:
	short m_Width;
	short m_Height;
public:
	TGumpGumppicTiled(WORD graphic, short x, short y, short width, short height);
	virtual ~TGumpGumppicTiled() {}

	SETGET(short, Width);
	SETGET(short, Height);
};
//---------------------------------------------------------------------------
class TGumpTooltip : public TGumpObject
{
private:
	int m_ClilocID;
public:
	TGumpTooltip(int clilocID);
	virtual ~TGumpTooltip() {}

	SETGET(int, ClilocID);
};
//---------------------------------------------------------------------------
class TGumpHTMLGump : public TGumpObject
{
private:
	short m_Width;
	short m_Height;
	int m_TextIndex;
	bool m_HaveBackground;
	bool m_HaveScrollbar;
	int m_LineOffset;
	int m_BarOffset;
	wstring m_HTMLText;
public:
	TGumpHTMLGump(int textIndex, short x, short y, short width, short height, bool haveBackground, bool haveScrollbar);
	virtual ~TGumpHTMLGump();
	
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
class TGumpXFMHTMLGump : public TGumpHTMLGump
{
private:
public:
	TGumpXFMHTMLGump(DWORD clilocID, WORD color, short x, short y, short width, short height, bool haveBackground, bool haveScrollbar);
	virtual ~TGumpXFMHTMLGump();
};
//---------------------------------------------------------------------------
class TGumpMenuObject : public TGumpObject
{
private:
	bool m_Pressed;
	string m_Text;
public:
	TGumpMenuObject(WORD graphic, WORD color, string text);

	SETGET(bool, Pressed);
	SETGET(string, Text);
};
//---------------------------------------------------------------------------
class TGumpSellObject : public TGumpObject
{
private:
	short m_Count;
	DWORD m_Price;
	string m_Name;
	bool m_Selected;

	short m_SelectedCount;
public:
	TGumpSellObject();
	virtual ~TGumpSellObject() {}

	SETGET(short, Count);
	SETGET(DWORD, Price);
	SETGET(string, Name);
	SETGET(bool, Selected);
	SETGETEX(short, SelectedCount);
};
//---------------------------------------------------------------------------
class TGumpBulletinBoardObject : public TGumpObject
{
private:
public:
	TGumpBulletinBoardObject(DWORD serial, string text);
	virtual ~TGumpBulletinBoardObject();

	TTextTexture Texture;
};
//---------------------------------------------------------------------------
#endif