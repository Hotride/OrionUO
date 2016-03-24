/****************************************************************************
**
** GumpQuestion.h
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
#ifndef GumpQuestionH
#define GumpQuestionH
//---------------------------------------------------------------------------
class TGumpQuestion : public TGump
{
private:
	static const int ID_GQ_BUTTON_CANCEL = 1;
	static const int ID_GQ_BUTTON_OKAY = 2;

	static TTextTexture m_Text[2];

	BYTE m_Variant;
public:
	TGumpQuestion(DWORD serial, short x, short y, BYTE variant);
	virtual ~TGumpQuestion();

	static void InitTextTextures();
	static void ReleaseTextTextures();

	SETGET(BYTE, Variant);
	
	void PrepareTextures();

	int Draw(bool &mode);

	void GenerateFrame(int posX, int posY);

	void OnLeftMouseUp();
};
//---------------------------------------------------------------------------
#endif