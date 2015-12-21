/****************************************************************************
**
** ServerGump.h
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
#ifndef ServerScreenH
#define ServerScreenH
//---------------------------------------------------------------------------
class TServerScreen : public TBaseScreen
{
private:
	TTextTexture m_Text[2];

	static const int ID_SS_QUIT = 1;
	static const int ID_SS_ARROW_PREV = 2;
	static const int ID_SS_ARROW_NEXT = 3;
	static const int ID_SS_TIME_ZONE = 4;
	static const int ID_SS_FULL = 5;
	static const int ID_SS_CONNECTION = 6;
	static const int ID_SS_EARTH = 7;
	static const int ID_SS_SCROLLBAR_UP = 8;
	static const int ID_SS_SCROLLBAR_DOWN = 9;
	static const int ID_SS_SCROLLBAR = 10;
	static const int ID_SS_SCROLLBAR_BACKGROUND = 11;
	static const int ID_SS_SERVER_LIST = 20;

	int GetServerListHeight();
	void ListingList(bool direction, int divizor = 1);

	DWORD m_LastScrollChangeTime;
	int m_PixelOffset;

public:
	TServerScreen();
	virtual ~TServerScreen();

	void Init();

	void InitTooltip();

	int Render(bool mode);
	
	void OnLeftMouseDown();
	void OnLeftMouseUp();
	void OnMouseWheel(MOUSE_WHEEL_STATE state);
	void OnKeyPress(WPARAM wparam, LPARAM lparam);
};

extern TServerScreen *ServerScreen;
//---------------------------------------------------------------------------
#endif