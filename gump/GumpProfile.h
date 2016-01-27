/****************************************************************************
**
** GumpProfile.h
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
#ifndef GumpProfileH
#define GumpProfileH
//---------------------------------------------------------------------------
class TGumpProfile : public TGump
{
private:
	static const int ID_GP_BUTTON_MINIMIZE = 1;
	static const int ID_GP_BUTTON_RESIZE = 2;
	static const int ID_GP_SCROLLER = 3;
	static const int ID_GP_APPLY = 5;

	int m_Height;
	int m_HeightBuffer;
	int m_CurrentLine;
	DWORD m_LastScrollChangeTime;
	bool m_Changed;
	
	TTextTexture m_TopText;
	TTextTexture m_BottomText;

	int GetHeight();

	void SetHeight(int val);

	void ListingList(bool direction, int divizor = 1);
public:
	TGumpProfile(DWORD serial, short x, short y, wstring topText, wstring bottomText, wstring dataText);
	virtual ~TGumpProfile();
	
	static const int ID_GP_TEXT_FIELD = 4;

	SETGET(bool, Changed);

	TEntryText *TextEntry;
	
	bool EntryPointerHere() {return (EntryPointer == TextEntry);}

	void ChangeHeight();

	void PrepareTextures();

	int Draw(bool &mode);

	void GenerateFrame(int posX, int posY);
	
	void OnLeftMouseDown();
	void OnLeftMouseUp();
	bool OnLeftMouseDoubleClick();
	void OnMouseWheel(MOUSE_WHEEL_STATE &state);
	void OnCharPress(WPARAM &wparam, LPARAM &lparam);
	void OnKeyPress(WPARAM &wparam, LPARAM &lparam);
};
//---------------------------------------------------------------------------
#endif