/****************************************************************************
**
** FontsManager.h
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
//--------------------------------------------------------------------------
#ifndef FontsManagerH
#define FontsManagerH
//--------------------------------------------------------------------------
//Структура с данными о ссылке
struct WEB_LINK
{
	//Была посещена
	bool Visited;

	//Ссылка
	string WebLink;
};
//--------------------------------------------------------------------------
//Структура с данными о HTML-символе
struct HTML_char
{
	//Индекс символа
	wchar_t Char;

	//Индекс шрифта
	BYTE Font;

	//Выравнивание
	TEXT_ALIGN_TYPE Align;

	//Набор флагов
	WORD Flags;

	//Цвет символа
	DWORD Color;

	//Индекс ссылки
	WORD LinkID;
};
//--------------------------------------------------------------------------
typedef std::map<WORD, WEB_LINK> WEBLINK_MAP;
//--------------------------------------------------------------------------
//Менеджер шрифтов
class TFontsManager
{
private:
	//Список ASCII шрифтов
	FONT_DATA *m_Font;

	//Количество ASCII ширфтов
	int m_FontCount;

	//Список ссылок
	WEBLINK_MAP m_WebLink;

	//Таблица ассоциации ASCII шрифтов
	static BYTE m_FontIndex[256];

	//Данные о юникод шрифтах: адрес, размер
	DWORD m_UnicodeFontAddress[20];
	DWORD m_UnicodeFontSize[20];

	//Сохранение пикселей текстуры
	bool m_SavePixels;

	//Использование HTML расширений
	bool m_UseHTML;

	//Цвет HTML
	DWORD m_HTMLColor;

	//Получение индекса ссылки
	WORD GetWebLinkID(wchar_t *link, DWORD &color);

	//Получение HTML данных
	HTML_char *GetHTMLData(BYTE font, const wchar_t *str, int &len, TEXT_ALIGN_TYPE align, WORD flags);

	//Получение данных многострочного текста
	PMULTILINES_FONT_INFO GetInfoHTML(BYTE font, const wchar_t *str, int len, TEXT_ALIGN_TYPE align, WORD flags, int width);

	//Создание ASCII текстуры
	bool GenerateABase(BYTE &font, TTextTexture &th, const char *str, WORD &color, int &width, TEXT_ALIGN_TYPE &align, WORD &flags);

	//Создание Unicode текстуры
	bool GenerateWBase(BYTE &font, TTextTexture &th, const wchar_t *str, WORD &color, BYTE &cell, int &width, TEXT_ALIGN_TYPE &align, WORD &flags);

public:
	TFontsManager();
	~TFontsManager();
	
	SETGET(bool, SavePixels);

	void SetUseHTML(bool val, DWORD htmlStartColor = 0xFFFFFFFF) {m_UseHTML = val; m_HTMLColor = htmlStartColor;}
	bool GetUseHTML() const {return m_UseHTML;}
	__declspec(property(get = GetUseHTML)) bool UseHTML;

	//Загрузка ASCII шрифтов
	bool LoadFonts();

	//Переход по ссылке по индексу
	void GoToWebLink(WORD link);
	

	//-----------------------------
	//---Работа с ASCII шрифтами---
	//-----------------------------

	//Проверка, печатаемый ли символ
	bool IsPrintASCII(BYTE index) {return (m_FontIndex[index] != 0xFF);}

	//Получить смещение символа ширифта
	int GetFontOffsetY(BYTE Number, BYTE Index);

	//Получить позицию каретки в тексте
	POINT GetCaretPosA(BYTE font, const char *str, int pos, int width, TEXT_ALIGN_TYPE align, WORD flags);

	//Вычислить положение каретки
	int CalculateCaretPosA(BYTE font, const char *str, int x, int y, int width, TEXT_ALIGN_TYPE align, WORD flags);

	//Получить ширину текста
	int GetWidthA(BYTE font, const char *str, int len = 0);

	//Получить высоту текста
	int GetHeightA(BYTE font, const char *str, int width = 0, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);

	//Получить высоту текста по списку строк
	int GetHeightA(PMULTILINES_FONT_INFO info);

	//Получить текст указанной ширины
	string GetTextByWidthA(BYTE font, const char *str, int len, int width, bool IsCropped);

	//Получить информацию о тексте (в мультистрочном виде)
	PMULTILINES_FONT_INFO GetInfoA(BYTE font, const char *str, int len, TEXT_ALIGN_TYPE align, WORD flags, int width);

	//Сгенерировать текстуру текста
	bool GenerateA(BYTE font, TTextTexture &th, const char *str, WORD color = 0, int width = 0, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);

	//Отрисовать текст
	void DrawA(BYTE font, const char *str, WORD color, int X, int Y, int width = 0, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);



	//-------------------------------
	//---Работа с Unicode шрифтами---
	//-------------------------------

	//Получить позицию каретки в тексте
	POINT GetCaretPosW(BYTE font, const wchar_t *str, int pos, int width, TEXT_ALIGN_TYPE align, WORD flags);

	//Вычислить положение каретки
	int CalculateCaretPosW(BYTE font, const wchar_t *str, int x, int y, int width, TEXT_ALIGN_TYPE align, WORD flags);

	//Получить ширину текста
	int GetWidthW(BYTE font, const wchar_t *str, int len = 0);

	//Получить высоту текста
	int GetHeightW(BYTE font, const wchar_t *str, int width = 0, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);

	//Получить высоту текста по списку строк
	int GetHeightW(PMULTILINES_FONT_INFO info);

	//Получить текст указанной ширины
	wstring GetTextByWidthW(BYTE font, const wchar_t *str, int len, int width, bool IsCropped);

	//Получить информацию о тексте (в мультистрочном виде)
	PMULTILINES_FONT_INFO GetInfoW(BYTE font, const wchar_t *str, int len, TEXT_ALIGN_TYPE align, WORD flags, int width);

	//Сгенерировать текстуру текста
	bool GenerateW(BYTE font, TTextTexture &th, const wchar_t *str, WORD color = 0, BYTE cell = 30, int width = 0, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);

	//Отрисовать текст
	void DrawW(BYTE font, const wchar_t *str, WORD color, int X, int Y, BYTE cell = 30, int width = 0, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);
};
//--------------------------------------------------------------------------
extern TFontsManager *FontManager;
//--------------------------------------------------------------------------
#endif