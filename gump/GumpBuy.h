/****************************************************************************
**
** GumpBuy.h
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
#ifndef GumpBuyH
#define GumpBuyH
//---------------------------------------------------------------------------
class TGumpBuy : public TGump
{
private:
	static const int ID_GB_BUTTON_UP_1 = 1;
	static const int ID_GB_BUTTON_DOWN_1 = 2;
	static const int ID_GB_SCROLL_1 = 3;
	static const int ID_GB_BUTTON_UP_2 = 4;
	static const int ID_GB_BUTTON_DOWN_2 = 5;
	static const int ID_GB_SCROLL_2 = 6;
	static const int ID_GB_BUTTON_ACCEPT = 7;
	static const int ID_GB_BUTTON_CLEAR = 8;
	static const int ID_GB_ITEM_LIST = 1000;
	static const int ID_GB_BUTTON_INC = 2000;
	static const int ID_GB_BUTTON_DEC = 3000;
	static const int ID_GB_BUTTON_END = 4000;

	DWORD m_LastChangedLineTime;
	DWORD m_LastChangeCountTime;
	DWORD m_StartChangeCountTime;
	int m_SelectedLine1;
	int m_SelectedLine2;
	int m_SelectedListCount;
	int m_ContinueCounter;
	int m_Count;

	static int m_Layers[2];
	
	bool ChangeItemCount(bool increase, int idx, int count = 1);
	void CalculateBuyListCount();
	void CalculateSelectedListCount();
	void ListingList(bool direction, bool gumpNumber, int divizor = 1);

public:
	TGumpBuy(DWORD serial, short x, short y);
	virtual ~TGumpBuy();

	void PrepareTextures();

	int Draw(bool &mode);

	void GenerateFrame(int posX, int posY);
	
	void OnLeftMouseDown();
	void OnLeftMouseUp();
	bool OnLeftMouseDoubleClick();
	void OnMouseWheel(MOUSE_WHEEL_STATE &state);
};
//---------------------------------------------------------------------------
#endif