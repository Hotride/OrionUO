/****************************************************************************
**
** GumpMenu.h
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
#ifndef GumpMenuH
#define GumpMenuH
//---------------------------------------------------------------------------
class TGumpMenu : public TGump
{
private:
	static const int ID_GM_LEFT = 1;
	static const int ID_GM_RIGHT = 2;
	static const int ID_GM_ITEMS = 10;

	string m_Text;

	int m_Count;
	int m_CurrentIndex;
	int m_CurrentItemOffset;
	int m_CurrentOffset;

	void MoveItems();

public:
	TGumpMenu(DWORD serial, short x, short y, string text);
	virtual ~TGumpMenu();

	SETGET(int, Count);
	
	void PrepareTextures();

	int Draw(bool &mode);

	void GenerateFrame(int posX, int posY);

	void OnLeftMouseUp();
	bool OnLeftMouseDoubleClick();
};
//---------------------------------------------------------------------------
#endif