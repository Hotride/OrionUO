/****************************************************************************
**
** GumpBulletinBoardItem.h
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
#ifndef GumpBulletinBoardItemH
#define GumpBulletinBoardItemH
//---------------------------------------------------------------------------
class TGumpBulletinBoardItem : public TGump
{
protected:
	static const int ID_GBBI_BUTTON_RESIZE = 1;
	static const int ID_GBBI_SCROLLER_UP = 2;
	static const int ID_GBBI_SCROLLER_DOWN = 3;
	static const int ID_GBBI_SCROLLER = 4;
	static const int ID_GBBI_SUBJECT_TEXT_FIELD = 5;
	static const int ID_GBBI_POST = 7;
	static const int ID_GBBI_REPLY = 8;
	static const int ID_GBBI_REMOVE = 9;

	int m_Height;
	int m_HeightBuffer;
	int m_CurrentLine;
	DWORD m_LastScrollChangeTime;
	BYTE m_Variant;
	
	TTextTexture m_Text[3];

	TTextTexture m_Poster;
	TTextTexture m_DataTime;

	int GetHeight();

	void SetHeight(int val);

	void ListingList(bool direction, int divizor = 1);
	void CalculateGumpState();
public:
	TGumpBulletinBoardItem(DWORD serial, short x, short y, BYTE variant, DWORD id, string poster, string subject, string dataTime, string data);
	virtual ~TGumpBulletinBoardItem();
	
	static const int ID_GBBI_TEXT_FIELD = 6;

	TEntryText *TextEntrySubject;
	TEntryText *TextEntry;
	
	bool EntryPointerHere() {return (EntryPointer == TextEntrySubject || EntryPointer == TextEntry);}

	void ChangeHeight();

	void PrepareTextures();

	int Draw(bool &mode);

	void GenerateFrame(int posX, int posY);
	
	void OnLeftMouseDown();
	void OnLeftMouseUp();
	void OnMouseWheel(MOUSE_WHEEL_STATE &state);
	void OnCharPress(WPARAM &wparam, LPARAM &lparam);
	void OnKeyPress(WPARAM &wparam, LPARAM &lparam);
};
//---------------------------------------------------------------------------
#endif