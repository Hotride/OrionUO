/****************************************************************************
**
** GumpMap.h
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
#ifndef GumpMapH
#define GumpMapH
//---------------------------------------------------------------------------
class TGumpMap : public TGump
{
private:
	static const int ID_GM_PLOT_COURSE = 1;
	static const int ID_GM_STOP_PLOTTING = 2;
	static const int ID_GM_CLEAR_COURSE = 3;
	static const int ID_GM_PIN_LIST = 10;
	static const int ID_GM_PIN_LIST_INSERT = 1000;

	int m_PlotState;

	int m_StartX;
	int m_StartY;
	int m_EndX;
	int m_EndY;
	int m_Width;
	int m_Height;

	DWORD m_PinTimer;
	TGumpObject *PinOnCursor;

	int LineUnderMouse(__inout int &x1, __inout int &y1, __in int x2, __in int y2);
public:
	TGumpMap(__in DWORD serial, __in short x, __in short y);
	virtual ~TGumpMap();

	SETGET(int, PlotState);
	SETGET(int, StartX);
	SETGET(int, StartY);
	SETGET(int, EndX);
	SETGET(int, EndY);
	SETGET(int, Width);
	SETGET(int, Height);

	GLuint Texture;

	void PrepareTextures();

	int Draw(__in bool &mode);

	void GenerateFrame();
	
	void OnLeftMouseDown();
	void OnLeftMouseUp();
};
//---------------------------------------------------------------------------
#endif