/****************************************************************************
**
** GumpJournal.h
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
#ifndef GumpJournalH
#define GumpJournalH
//---------------------------------------------------------------------------
class TGumpJournal : public TGump
{
private:
	static const int ID_GJ_BUTTON_MINIMIZE = 1;
	static const int ID_GJ_BUTTON_RESIZE = 2;
	static const int ID_GJ_SHOW_SYSTEM = 3;
	static const int ID_GJ_SHOW_OBJECTS = 4;
	static const int ID_GJ_SHOW_CLIENTS = 5;
	static const int ID_GJ_BUTTON_LOCK = 6;
	static const int ID_GJ_BUTTON_UP = 7;
	static const int ID_GJ_BUTTON_DOWN = 8;
	static const int ID_GJ_SCROLLER = 9;
	static const int ID_GJ_LOCK_MOVING = 10;

	DWORD m_LastScrollChangeTime;

	static TTextTexture m_Text[3];

	int m_Height;
	int m_HeightBuffer;

public:
	TGumpJournal(DWORD serial, short x, short y, bool minimized);
	virtual ~TGumpJournal();

	static void InitTextTextures();
	static void ReleaseTextTextures();

	void PrepareTextures();

	void ChangeHeight();

	void SetHeight(int val);
	
	short GetHeight() const {return m_Height;}

	bool GetResizing() const {return (m_HeightBuffer != 0);}
	
	int Draw(bool &mode);

	void GenerateFrame(int posX, int posY);

	void OnLeftMouseUp();
	bool OnLeftMouseDoubleClick();
	void OnMouseWheel(MOUSE_WHEEL_STATE &state);
};
//---------------------------------------------------------------------------
#endif