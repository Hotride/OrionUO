/****************************************************************************
**
** GumpSecureTrading.h
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
#ifndef GumpSecureTradingH
#define GumpSecureTradingH
//---------------------------------------------------------------------------
class TGumpSecureTrading : public TGump
{
private:
	static const int ID_GST_CHECKBOX = 1;
	
	TTextRenderer *TextRenderer;
	
	DWORD m_ID2;
	bool m_StateMy;
	bool m_StateOpponent;
	string m_Text;
public:
	TGumpSecureTrading(DWORD serial, short x, short y, DWORD id, DWORD id2);
	virtual ~TGumpSecureTrading();
	
	TTextRenderer *GetTextRenderer() {return TextRenderer;}

	SETGET(DWORD, ID2);
	SETGET(bool, StateMy);
	SETGET(bool, StateOpponent);
	SETGET(string, Text);

	void PrepareTextures();

	int Draw(bool &mode);

	void GenerateFrame(int posX, int posY);

	void OnLeftMouseUp();
};
//---------------------------------------------------------------------------
#endif