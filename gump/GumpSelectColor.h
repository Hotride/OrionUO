/****************************************************************************
**
** GumpSelectColor.h
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
#ifndef GumpSelectColorH
#define GumpSelectColorH
//---------------------------------------------------------------------------
class TGumpSelectColor : public TGump
{
protected:
	static const int ID_GSC_BUTTON_OKAY = 1;
	static const int ID_GSC_SLIDER = 2;
	static const int ID_GSC_COLORS = 10;

	SELECT_COLOR_GUMP_STATE m_State;
	WORD m_SliderPos;
	BYTE m_ColorRef;
	int m_SelectedIndex;

public:
	TGumpSelectColor(DWORD serial, short x, short y, SELECT_COLOR_GUMP_STATE state);
	virtual ~TGumpSelectColor();

	void PrepareTextures();

	void GenerateFrame(int posX, int posY);

	int Draw(bool &mode);

	void OnLeftMouseUp();

	virtual void OnSelectColor(WORD &color);
};
//---------------------------------------------------------------------------
#endif