/***********************************************************************************
**
** GameConsole.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GAMECONSOLE_H
#define GAMECONSOLE_H
//----------------------------------------------------------------------------------
#include "EntryText.h"
#include "../Constants.h"
//----------------------------------------------------------------------------------
//Класс для работы с игровой консолью (чатик) снизу слева игрового окна
class CGameConsole : public CEntryText
{
private:
	//Тип консоли
	GAME_CONSOLE_TEXT_TYPE m_Type;

	//Тест данных на не стандартное сообщение
	wstring IsSystemCommand(const wchar_t *text, int &len, int &member);

	//!Стек последнего введенного текста
	wstring m_ConsoleStack[MAX_CONSOLE_STACK_SIZE];

	//!Количество добавленного в стек текста
	int m_ConsoleStackCount;

	//!Указатель на текущий элемент стека
	int m_ConsoleSelectedIndex;

	bool m_PositionChanged;

public:
	CGameConsole();
	virtual ~CGameConsole();

	//Отправить данные консоли
	void Send();

	//Отрисовать текст консоли
	void DrawW(BYTE font, WORD color, int x, int y, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);

	void SaveConsoleMessage();

	void ChangeConsoleMessage(const bool &next);

	void ClearStack();
};
//----------------------------------------------------------------------------------
static const wstring g_ConsolePrefix[7] =
{
	L"", //Normal
	L"! ", //Yell
	L"; ", //Whisper
	L": ", //Emote
	L".", //Command
	L"? ", //Broadcast
	L"/ ", //Party
};
//----------------------------------------------------------------------------------
extern CGameConsole g_GameConsole;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
