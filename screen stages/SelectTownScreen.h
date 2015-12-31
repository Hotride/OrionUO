/****************************************************************************
**
** CreateCharacterSelectionTownGump.h
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
#ifndef SelectTownScreenH
#define SelectTownScreenH
//---------------------------------------------------------------------------
class TSelectTownScreen : public TBaseScreen
{
private:
	int m_PixelOffset;
	DWORD m_LastScrollChangeTime;

	static const BYTE ID_SMOOTH_STS_QUIT = 1;
	static const BYTE ID_SMOOTH_STS_GO_SCREEN_CHARACTER = 2;
	static const BYTE ID_SMOOTH_STS_GO_SCREEN_GAME_CONNECT = 3;

	void ListingList(bool direction, int divizor = 1);
	
	static const int ID_STS_QUIT = 1;
	static const int ID_STS_ARROW_PREV = 2;
	static const int ID_STS_ARROW_NEXT = 3;
	static const int ID_STS_SCROLLBAR_UP = 4;
	static const int ID_STS_SCROLLBAR_DOWN = 5;
	static const int ID_STS_SCROLLBAR = 6;
	static const int ID_STS_SCROLLBAR_BACKGROUND = 7;
	static const int ID_STS_TOWN = 10;
	
	int GetScrollBoxHeight();
	void DrawTownInfo(int x, int y);

	static POINT m_TownButtonText[9];

public:
	TSelectTownScreen();
	~TSelectTownScreen();
	
	TCityItem *m_City;

	void Init();

	void ProcessSmoothAction(BYTE action = 0xFF);

	void InitTooltip();

	int Render(bool mode);
	
	void OnLeftMouseDown();
	void OnLeftMouseUp();
	void OnMouseWheel(MOUSE_WHEEL_STATE state);
};
//---------------------------------------------------------------------------
extern TSelectTownScreen *SelectTownScreen;
//---------------------------------------------------------------------------
#endif