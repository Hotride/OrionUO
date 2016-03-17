/****************************************************************************
**
** MinimapGump.h
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
#ifndef GumpMinimapH
#define GumpMinimapH
//---------------------------------------------------------------------------
class TGumpMinimap : public TGump
{
private:
	static const int ID_GMM_LOCK_MOVING = 1;

	BYTE m_Count;

	GLuint m_Texture;
	WORD m_LastX;
	WORD m_LastY;

	void GenerateMap();

public:
	TGumpMinimap(DWORD serial, short x, short y, bool minimized);
	virtual ~TGumpMinimap();

	SETGET(WORD, LastX);
	
	void PrepareTextures();

	void GenerateFrame(int posX, int posY);

	int Draw(bool &mode);

	void OnLeftMouseUp();
};
//---------------------------------------------------------------------------
#endif