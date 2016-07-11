/****************************************************************************
**
** GumpBook.h
**
** Copyright (C) October 2015 Hotride
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
#ifndef GumpBookH
#define GumpBookH
//---------------------------------------------------------------------------
class TGumpBook : public TGump
{
private:
	static const int ID_GB_TEXT_AREA_AUTHOR = 1;
	static const int ID_GB_TEXT_AREA_TITLE = 2;
	static const int ID_GB_TEXT_AREA_PAGE_LEFT = 3;
	static const int ID_GB_TEXT_AREA_PAGE_RIGHT = 4;
	static const int ID_GB_BUTTON_PREV = 5;
	static const int ID_GB_BUTTON_NEXT = 6;

	bool m_Writable;
	short m_PageCount;
	short m_Page;
	bool m_Unicode;

	TTextTexture *m_PageIndexText;

	static TTextTexture m_TextTitle;
	static TTextTexture m_TextBy;

public:
	TGumpBook(DWORD serial, short x, short y, short pageCount, bool writable, bool unicode);
	virtual ~TGumpBook();

	static void InitTextTextures();
	static void ReleaseTextTextures();

	SETGET(bool, Writable);
	SETGET(short, PageCount);
	SETGET(bool, Unicode);
	SETGET(short, Page);

	TEntryText *TextEntryAuthor;
	TEntryText *TextEntryTitle;
	TBookEntryText *TextEntry;

	void PrepareTextures();

	int Draw(bool &mode);

	void GenerateFrame();

	void OnLeftMouseUp();
	bool OnLeftMouseDoubleClick();

	void OnCharPress(WPARAM &wparam, LPARAM &lparam);
	void OnKeyPress(WPARAM &wparam, LPARAM &lparam);

	bool EntryPointerHere();
};
//---------------------------------------------------------------------------
#endif