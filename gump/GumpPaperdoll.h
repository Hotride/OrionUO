/****************************************************************************
**
** Gump.h
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
#ifndef GumpPaperdollH
#define GumpPaperdollH
//---------------------------------------------------------------------------
class TGumpPaperdoll : public TGump
{
private:
	static const int ID_GP_BUTTON_HELP = 1;
	static const int ID_GP_BUTTON_OPTIONS = 2;
	static const int ID_GP_BUTTON_LOGOUT = 3;
	static const int ID_GP_BUTTON_JOURNAL_OR_QUESTS = 4;
	static const int ID_GP_BUTTON_SKILLS = 5;
	static const int ID_GP_BUTTON_CHAT_OR_GUILD = 6;
	static const int ID_GP_BUTTON_WARMODE = 7;
	static const int ID_GP_BUTTON_STATUS = 8;
	static const int ID_GP_BUTTON_MINIMIZE = 9;
	static const int ID_GP_PROFILE_SCROLL = 10;
	static const int ID_GP_PARTY_MANIFEST_SCROLL = 11;
	static const int ID_GP_LOCK_MOVING = 12;
	
	TTextRenderer *TextRenderer;
	
	static const int m_LayerCount = 22;
	static int UsedLayers[m_LayerCount];
public:
	TGumpPaperdoll(DWORD serial, short x, short y, bool minimized);
	virtual ~TGumpPaperdoll();
	
	TTextContainer *m_TextContainer;

	TTextRenderer *GetTextRenderer() {return TextRenderer;}

	static const int ID_GP_ITEMS = 20;

	void PrepareTextures();

	void GenerateFrame(int posX, int posY);

	int Draw(bool &mode);

	void OnLeftMouseUp();
	bool OnLeftMouseDoubleClick();
};
//---------------------------------------------------------------------------
#endif