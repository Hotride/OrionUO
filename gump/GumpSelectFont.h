/****************************************************************************
**
** GumpSelectFont.h
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
#ifndef GumpSelectFontH
#define GumpSelectFontH
//---------------------------------------------------------------------------
class TGumpSelectFont : public TGump
{
private:
	static const int ID_GSF_FONTS = 1;

	SELECT_FONT_GUMP_STATE m_State;

public:
	TGumpSelectFont(DWORD serial, short x, short y, SELECT_FONT_GUMP_STATE state);
	virtual ~TGumpSelectFont();
	
	void PrepareTextures();

	int Draw(bool &mode);

	void GenerateFrame(int posX, int posY);

	void OnLeftMouseUp();
};
//---------------------------------------------------------------------------
#endif