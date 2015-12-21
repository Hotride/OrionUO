/****************************************************************************
**
** GumpNotify.h
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
#ifndef GumpNotifyH
#define GumpNotifyH
//---------------------------------------------------------------------------
class TGumpNotify : public TGump
{
private:
	static const int ID_GN_BUTTON_OK = 1;

	BYTE m_Variant;
	short m_Width;
	short m_Height;
	string m_Text;

	void Process();
public:
	TGumpNotify(DWORD serial, short x, short y, BYTE variant, short width, short height, string text);
	virtual ~TGumpNotify();

	SETGET(BYTE, Variant)
	SETGET(short, Width)
	SETGET(short, Height)
	SETGET(string, Text)

	void PrepareTextures();

	int Draw(bool &mode);

	void GenerateFrame(int posX, int posY);

	void OnLeftMouseUp();
	void OnKeyPress(WPARAM &wparam, LPARAM &lparam);
};
//---------------------------------------------------------------------------
#endif