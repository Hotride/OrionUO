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
	static const int ID_GB_NEXT_WINDOW_DIRECTION = 1;

	bool UnderMouse();

public:
	TGumpBuff(DWORD serial, short x, short y);
	virtual ~TGumpBuff();

	void PrepareTextures();

	int Draw(bool &mode);

	void GenerateFrame();

	void OnLeftMouseUp();
};
//---------------------------------------------------------------------------
#endif