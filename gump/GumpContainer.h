/****************************************************************************
**
** GumpContainer.h
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
#ifndef GumpContainerH
#define GumpContainerH
//---------------------------------------------------------------------------
class TGumpContainer : public TGump
{
private:
	DWORD m_CorpseEyesTicks;
	BYTE m_CorpseEyesOffset;
	static const DWORD ID_GC_LOCK_MOVING = 0xFFFFFFFE;
	static const DWORD ID_GC_MINIMIZE = 0xFFFFFFFF;
	bool m_IsGameBoard;
	
	TTextRenderer *TextRenderer;

public:
	TGumpContainer(DWORD serial, short x, short y);
	virtual ~TGumpContainer();
	
	TTextRenderer *GetTextRenderer() {return TextRenderer;}

	SETGET(bool, IsGameBoard);

	void PrepareTextures();

	int Draw(bool &mode);

	void GenerateFrame(int posX, int posY);

	void OnLeftMouseUp();
	bool OnLeftMouseDoubleClick();
};
//---------------------------------------------------------------------------
#endif