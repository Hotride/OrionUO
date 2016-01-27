/****************************************************************************
**
** GumpMenubar.h
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
#ifndef GumpMenubarH
#define GumpMenubarH
//---------------------------------------------------------------------------
class TGumpMenubar : public TGump
{
private:
	static const int ID_GMB_MINIMIZE = 1;
	static const int ID_GMB_MAP = 2;
	static const int ID_GMB_PAPERDOLL = 3;
	static const int ID_GMB_INVENTORY = 4;
	static const int ID_GMB_JOURNAL = 5;
	static const int ID_GMB_CHAT = 6;
	static const int ID_GMB_HELP = 7;
	static const int ID_GMB_WORLD_MAP = 8;
	static const int ID_GMB_INFO = 9;

	static TTextTexture m_Texture[8][2];
	bool m_Opened;
public:
	TGumpMenubar(DWORD serial, short x, short y);
	virtual ~TGumpMenubar();
	
	static void InitTextTextures();
	static void ReleaseTextTextures();

	bool CanBeDisplayed() {return false;}

	SETGET(bool, Opened);

	void PrepareTextures();

	int Draw(bool &mode);

	void GenerateFrame(int posX, int posY);

	void OnLeftMouseUp();
};
//---------------------------------------------------------------------------
#endif