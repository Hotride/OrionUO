/****************************************************************************
**
** GumpConsoleType.h
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
#ifndef GumpConsoleTypeH
#define GumpConsoleTypeH
//---------------------------------------------------------------------------
class TGumpConsoleType : public TGump
{
protected:
	int m_SelectedType;
	bool m_ShowFullText;

	static const int ID_GCT_NORMAL = 1;
	static const int ID_GCT_YELL = 2;
	static const int ID_GCT_WHISPER = 3;
	static const int ID_GCT_EMOTE = 4;
	static const int ID_GCT_COMMAND = 5;
	static const int ID_GCT_BROADCAST = 6;
	static const int ID_GCT_PARTY = 7;
	static const int ID_GCT_MINIMIZE = 8;
	static const int ID_GCT_SHOW_FULL_TEXT = 9;

	static TTextTexture m_TextTitle;
	static TTextTexture m_TextCheckbox;
	static TTextTexture m_Text[2][7];

	void CalculateGumpState();

public:
	TGumpConsoleType(bool minimized, bool showFullText);
	virtual ~TGumpConsoleType();

	static void InitTextTextures();
	static void ReleaseTextTextures();

	SETGET(bool, ShowFullText);

	bool ConsoleIsEmpty();

	void DeleteConsolePrefix();
	void SetConsolePrefix();

	void PrepareTextures();

	int Draw(bool &mode);

	void OnToolTip();

	void GenerateFrame();

	void OnLeftMouseUp();
};
//---------------------------------------------------------------------------
extern TGumpConsoleType *g_GumpConsoleType;
//---------------------------------------------------------------------------
#endif