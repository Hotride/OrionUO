/****************************************************************************
**
** GumpTip.h
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
#ifndef GumpTipH
#define GumpTipH
//---------------------------------------------------------------------------
class TGumpTip : public TGump
{
private:
	static const int ID_GT_BUTTON_MINIMIZE = 1;
	static const int ID_GT_BUTTON_RESIZE = 2;
	static const int ID_GT_BUTTON_UP = 3;
	static const int ID_GT_BUTTON_DOWN = 4;
	static const int ID_GT_SCROLLER = 5;
	static const int ID_GT_PREV_GUMP = 6;
	//static const int ID_GT_REMOVE_GUMP = 7;
	static const int ID_GT_NEXT_GUMP = 8;

	int m_Height;
	int m_HeightBuffer;
	int m_CurrentLine;
	DWORD m_LastScrollChangeTime;
	bool m_Updates;

	void SetHeight(int val);

	void ListingList(bool direction, int divizor = 1);
public:
	TGumpTip(DWORD serial, short x, short y, bool updates);
	virtual ~TGumpTip();

	SETGET(bool, Updates);

	TTextTexture m_Text;

	void ChangeHeight();

	void PrepareTextures();

	int Draw(bool &mode);

	void GenerateFrame();

	void OnLeftMouseUp();
	bool OnLeftMouseDoubleClick();
	void OnMouseWheel(MOUSE_WHEEL_STATE &state);
};
//---------------------------------------------------------------------------
#endif