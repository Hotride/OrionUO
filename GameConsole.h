/****************************************************************************
**
** GameConsole.h
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
#ifndef GameConsoleH
#define GameConsoleH
//---------------------------------------------------------------------------
class TGameConsole : public TEntryText
{
private:
	GAME_CONSOLE_TEXT_TYPE m_Type;

	wstring IsSystemCommand(const wchar_t *text, int &len, int &member);

public:
	TGameConsole();
	virtual ~TGameConsole();

	void Send();

	void DrawW(BYTE font, WORD color, int x, int y, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);
};
//---------------------------------------------------------------------------
extern TGameConsole *GameConsole;
//---------------------------------------------------------------------------
#endif