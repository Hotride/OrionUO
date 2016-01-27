/****************************************************************************
**
** GumpSpellbook.h
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
#ifndef GumpSpellbookH
#define GumpSpellbookH
//---------------------------------------------------------------------------
class TGumpSpellbook : public TGump
{
private:
	static const int ID_GSB_BUTTON_PREV = 1;
	static const int ID_GSB_BUTTON_NEXT = 2;
	static const int ID_GSB_BUTTON_CIRCLE_1_2 = 3;
	static const int ID_GSB_BUTTON_CIRCLE_3_4 = 4;
	static const int ID_GSB_BUTTON_CIRCLE_5_6 = 5;
	static const int ID_GSB_BUTTON_CIRCLE_7_8 = 6;
	static const int ID_GSB_BUTTON_MINIMIZE = 7;
	static const int ID_GSB_LOCK_MOVING = 8;
	static const int ID_GSB_SPELL_ICON_LEFT = 10;
	static const int ID_GSB_SPELL_ICON_RIGHT = 100;

	static TTextTexture m_TextIndex;
	static TTextTexture m_TextReagents;
	static TTextTexture m_TextCircle[8];
	static TTextTexture m_TextReagent[8];
	static TTextTexture m_TextSpell[64];

	static WORD m_ReagentsIndex[8];

	int GetReagentIndex(WORD &id);

	int m_Page;

public:
	TGumpSpellbook(DWORD serial, short x, short y);
	virtual ~TGumpSpellbook();

	static void InitTextTextures();
	static void ReleaseTextTextures();

	SETGET(int, Page);

	void PrepareTextures();

	int Draw(bool &mode);
	
	void GenerateFrame(int posX, int posY);

	void OnLeftMouseUp();
	bool OnLeftMouseDoubleClick();
};
//---------------------------------------------------------------------------
#endif