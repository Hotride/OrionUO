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
//Класс для работы с игровой консолью (чатик) снизу слева игрового окна
class CGameConsole : public CEntryText
{
private:
    //Тип консоли
    GAME_CONSOLE_TEXT_TYPE m_Type{ GCTT_NORMAL };

    //!Стек последнего введенного текста
    wstring m_ConsoleStack[MAX_CONSOLE_STACK_SIZE];

    //!Количество добавленного в стек текста
    int m_ConsoleStackCount{ 0 };

    //!Указатель на текущий элемент стека
    int m_ConsoleSelectedIndex{ 0 };

    bool m_PositionChanged{ false };

public:
    CGameConsole();
    virtual ~CGameConsole();

    //Отправить данные консоли
    void Send();

    static void Send(wstring text, ushort defaultColor = 0);

    //Тест данных на не стандартное сообщение
    static wstring
    IsSystemCommand(const wchar_t *text, size_t &len, int &member, GAME_CONSOLE_TEXT_TYPE &type);

    //Отрисовать текст консоли
    void
    DrawW(BYTE font, WORD color, int x, int y, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);

    void SaveConsoleMessage();

    void ChangeConsoleMessage(bool next);

    void ClearStack();
};
//----------------------------------------------------------------------------------
static const wstring g_ConsolePrefix[] = {
    L"",    //Normal
    L"! ",  //Yell
    L"; ",  //Whisper
    L": ",  //Emote
    L".",   //Command
    L"? ",  //Broadcast
    L"/ ",  //Party
    L"\\ ", //Guild
    L"| "   //Alliance
};
//----------------------------------------------------------------------------------
extern CGameConsole g_GameConsole;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
