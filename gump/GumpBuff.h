/****************************************************************************
**
** GumpBuff.h
**
** Copyright (C) June 2016 Hotride
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
#ifndef GumpBuffH
#define GumpBuffH
//---------------------------------------------------------------------------
class TGumpBuff : public TGump
{
private:
	static const int BUFF_ITEM_STEP_OFFSET_X = 3;
	static const int BUFF_ITEM_STEP_OFFSET_Y = 3;
	static const int MUNIMUM_ICON_ALPHA = 80;
	static const int ALPHA_CHANGE_KOEFF = 600;
	static const int USE_ALPHA_BLENDING_WHEN_TIMER_LESS = 10000;

	static const int ID_GB_NEXT_WINDOW_DIRECTION = 1;
	static const int ID_GB_LOCK_MOVING = 2;
	static const int ID_GB_BUFF_ITEM = 10;

	void GetGumpStatus(POINT &ball, POINT &items, bool &useX, bool &decX, bool &decY, POINT &startGump, POINT &endGump);

protected:
	void CalculateGumpState();

public:
	TGumpBuff(DWORD serial, short x, short y);
	virtual ~TGumpBuff();

	void UpdateBuffIcons();

	void OnToolTip();

	void PrepareTextures();

	int Draw(bool &mode);

	void GenerateFrame();

	void OnLeftMouseUp();
};
//---------------------------------------------------------------------------
#endif