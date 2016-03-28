/****************************************************************************
**
** GumpTargetSystem.h
**
** Copyright (C) February 2016 Hotride
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
#ifndef GumpTargetSystemH
#define GumpTargetSystemH
//---------------------------------------------------------------------------
class TGumpTargetSystem : public TGump
{
private:
	static const int ID_GSB_BUFF_GUMP = 1;
	static const int ID_GSB_LOCK_MOVING = 2;

	string m_OldName;
public:
	TGumpTargetSystem(DWORD serial, short x, short y);
	virtual ~TGumpTargetSystem();

	SETGET(string, OldName);

	void PrepareTextures();

	int Draw(bool &mode);

	void GenerateFrame();
	
	void OnLeftMouseDown();
	void OnLeftMouseUp();
	bool OnLeftMouseDoubleClick();
};
//---------------------------------------------------------------------------
#endif