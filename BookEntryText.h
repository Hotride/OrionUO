/****************************************************************************
**
** BookEntryText.h
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
#ifndef BookEntryTextH
#define BookEntryTextH
//---------------------------------------------------------------------------
class TBookEntryText : public TEntryText
{
private:
public:
	TBookEntryText();
	virtual ~TBookEntryText();

	void DrawA(BYTE font, WORD color, int X, int Y, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);
};
//---------------------------------------------------------------------------
#endif