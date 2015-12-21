/****************************************************************************
**
** BookEntryText.cpp
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
#include "stdafx.h"
//---------------------------------------------------------------------------
TBookEntryText::TBookEntryText()
: TEntryText()
{
}
//---------------------------------------------------------------------------
TBookEntryText::~TBookEntryText()
{
}
//---------------------------------------------------------------------------
void TBookEntryText::DrawA(BYTE font, WORD color, int x, int y, TEXT_ALIGN_TYPE align, WORD flags)
{
	if (Changed || Color != color)
	{
		FixMaxWidthA(font);

		CreateTextureA(font, c_str(), color, Width, align, flags);

		Changed = false;
		Color = color;
	}

	m_Texture.Draw(x + DrawOffset, y);

	if (this == EntryPointer)
	{
		int offsY = 0;
		const int offsetTable[] = {1, 2, 1, 1, 1, 2, 1, 1, 2, 2};

		if (font < 10)
			offsY = offsetTable[font];

		FontManager->DrawA(font, "_", color, x + DrawOffset + CaretPos.x, y + offsY + CaretPos.y);
	}
}
//---------------------------------------------------------------------------