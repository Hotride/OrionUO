/****************************************************************************
**
** GumpDye.h
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
#ifndef GumpDyeH
#define GumpDyeH
//---------------------------------------------------------------------------
class TGumpDye : public TGumpSelectColor
{
private:
public:
	TGumpDye(DWORD serial, short x, short y, WORD graphic);
	virtual ~TGumpDye();

	void OnSelectColor(WORD &color);
};
//---------------------------------------------------------------------------
#endif