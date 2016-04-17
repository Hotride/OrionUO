/****************************************************************************
**
** GumpPopupMenu.h
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
#ifndef GumpPopupMenuH
#define GumpPopupMenuH
//---------------------------------------------------------------------------
class TGumpPopupMenu : public TGump
{
protected:
	int m_Width;
	int m_Height;

	void CalculateGumpState();

public:
	TGumpPopupMenu(DWORD serial, short x, short y);
	virtual ~TGumpPopupMenu();

	SETGET(int, Width);
	SETGET(int, Height);

	void PrepareTextures();

	int Draw(bool &mode);

	void GenerateFrame();

	void OnLeftMouseUp();
};
//---------------------------------------------------------------------------
extern TGumpPopupMenu *g_PopupMenu;
//---------------------------------------------------------------------------
#endif