/****************************************************************************
**
** DragGump.h
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
#ifndef GumpDragH
#define GumpDragH
//---------------------------------------------------------------------------
class TGumpDrag : public TGump
{
private:
	static const int ID_GD_SCROLL = 1;
	static const int ID_GD_OKAY = 2;
	static const int ID_GD_TEXT_FIELD = 3;

	int m_ScrollPos;
	bool m_StartText;
public:
	TGumpDrag(DWORD serial, short x, short y);
	virtual ~TGumpDrag();

	void GenerateFrame();

	void PrepareTextures();

	int Draw(bool &mode);

	TEntryText *TextEntry;

	void OnLeftMouseUp();
	void OnCharPress(WPARAM &wparam, LPARAM &lparam);
	void OnKeyPress(WPARAM &wparam, LPARAM &lparam);

	bool EntryPointerHere() {return (EntryPointer == TextEntry);}

	void OnOkayPressed();
};
//---------------------------------------------------------------------------
#endif