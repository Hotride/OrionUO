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
//Класс для работы с игровой консолью (чатик) снизу слева игрового окна
class TGameConsole : public TEntryText
{
private:
	//Тип консоли
	GAME_CONSOLE_TEXT_TYPE m_Type;

	//Тест данных на не стандартное сообщение
	wstring IsSystemCommand(const wchar_t *text, int &len, int &member);

public:
	TGameConsole();
	virtual ~TGameConsole();

	//Отправить данные консоли
	void Send();

	//Отрисовать текст консоли
	void DrawW(BYTE font, WORD color, int x, int y, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);
};
//---------------------------------------------------------------------------
static const wstring m_ConsolePrefix[7] =
{
	L"", //Normal
	L"! ", //Yell
	L"; ", //Whisper
	L": ", //Emote
	L".", //Command
	L"? ", //Broadcast
	L"/ ", //Party
};
//---------------------------------------------------------------------------
extern TGameConsole *GameConsole;
//---------------------------------------------------------------------------
#endif