/****************************************************************************
**
** GumpStatusbar.h
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
#ifndef GumpStatusbarH
#define GumpStatusbarH
//---------------------------------------------------------------------------
class TGumpStatusbar : public TGump
{
private:
	static const int ID_GSB_MINIMIZE = 1;
	static const int ID_GSB_TEXT_FIELD = 2;
	static const int ID_GSB_BUTTON_HEAL_1 = 3;
	static const int ID_GSB_BUTTON_HEAL_2 = 4;
	static const int ID_GSB_BUTTON_REMOVE_FROM_GROUP = 5;
	static const int ID_GSB_BUFF_GUMP = 6;
	static const int ID_GSB_LOCK_MOVING = 7;

public:
	TGumpStatusbar(DWORD serial, short x, short y, bool minimized);
	virtual ~TGumpStatusbar();

	TEntryText *TextEntry;
	
	TGumpStatusbar *m_GroupNext;
	TGumpStatusbar *m_GroupPrev;

	TGumpStatusbar *GetTopStatusbar();
	TGumpStatusbar *GetNearStatusbar(int &x, int &y);
	bool GetStatusbarGroupOffset(int &x, int &y);
	void UpdateGroup(int x, int y);
	void AddStatusbar(TGumpStatusbar *bar);
	void RemoveFromGroup();

	bool InGroup() {return (m_GroupNext != NULL || m_GroupPrev != NULL);}

	void PrepareTextures();

	int Draw(bool &mode);

	void GenerateFrame();
	
	void OnLeftMouseDown();
	void OnLeftMouseUp();
	bool OnLeftMouseDoubleClick();
	void OnCharPress(WPARAM &wparam, LPARAM &lparam);
	void OnKeyPress(WPARAM &wparam, LPARAM &lparam);

	bool EntryPointerHere() {return (EntryPointer == TextEntry);}
};
//---------------------------------------------------------------------------
#endif