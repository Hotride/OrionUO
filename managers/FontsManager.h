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
//!Структура с данными о ссылке
struct WEB_LINK
{
	//!Была посещена
	bool Visited;

	//!Ссылка
	string WebLink;
};
//--------------------------------------------------------------------------
//!Структура с данными о HTML-символе
struct HTML_char
{
	//!Индекс символа
	wchar_t Char;

	//!Индекс шрифта
	BYTE Font;

	//!Выравнивание
	TEXT_ALIGN_TYPE Align;

	//!Набор флагов
	WORD Flags;

	//!Цвет символа
	DWORD Color;

	//!Индекс ссылки
	WORD LinkID;
};
//--------------------------------------------------------------------------
typedef std::map<WORD, WEB_LINK> WEBLINK_MAP;
//--------------------------------------------------------------------------
//!Класс менеджера шрифтов
class TFontsManager
{
private:
	//!Список ASCII шрифтов
	FONT_DATA *m_Font;

	//!Количество ASCII ширфтов
	int m_FontCount;

	//!Список ссылок
	WEBLINK_MAP m_WebLink;

	//!Таблица ассоциации ASCII шрифтов
	static const BYTE m_FontIndex[256];

	//!Данные о юникод шрифтах: адрес, размер
	DWORD m_UnicodeFontAddress[20];
	DWORD m_UnicodeFontSize[20];

	//!Сохранение пикселей текстуры
	bool m_SavePixels;

	//!Использование HTML расширений
	bool m_UseHTML;

	//!Цвет HTML
	DWORD m_HTMLColor;

	/*!
	Получить индекс ссылки
	@param [__in] link Ссылка
	@param [__out] color Цвет ссылки
	@return Индекс ссылки
	*/
	WORD GetWebLinkID(__in wchar_t *link, __out DWORD &color);

	/*!
	Получение HTML данных
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] len Длина текста
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@return Массив HTML символов
	*/
	HTML_char *GetHTMLData(__in BYTE font, __in const wchar_t *str, __in int &len, __in TEXT_ALIGN_TYPE align, __in WORD flags);

	/*!
	Получение данных многострочного текста HTML
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] len Длина текста
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@param [__in] width Ширина текста
	@return Ссылка на данные
	*/
	PMULTILINES_FONT_INFO GetInfoHTML(__in BYTE font, __in const wchar_t *str, __in int len, __in TEXT_ALIGN_TYPE align, __in WORD flags, __in int width);

	/*!
	Создание ASCII текстуры
	@param [__in] font Шрифт
	@param [__out] th Данные текстуры
	@param [__in] str Текст
	@param [__in] color Цвет
	@param [__in] width Ширина текстуры
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@return true при успешной генерации
	*/
	bool GenerateABase(__in BYTE &font, __out TTextTexture &th, __in const char *str, __in WORD &color, __in int &width, __in TEXT_ALIGN_TYPE &align, __in WORD &flags);

	/*!
	Создание Unicode текстуры
	@param [__in] font Шрифт
	@param [__out] th Данные текстуры
	@param [__in] str Текст
	@param [__in] color Цвет
	@param [__in] cell Номер ячейки в палитре цветов
	@param [__in] width Ширина текстуры
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@return true при успешной генерации
	*/
	bool GenerateWBase(__in BYTE &font, __out TTextTexture &th, __in const wchar_t *str, __in WORD &color, __in BYTE &cell, __in int &width, __in TEXT_ALIGN_TYPE &align, __in WORD &flags);

public:
	TFontsManager();
	~TFontsManager();
	
	SETGET(bool, SavePixels);

	/*!
	Установить использование HTML-тэгов
	@param [__in] val Состояние использования
	@param [__in_opt] htmlStartColor Начальный цвет
	@return
	*/
	void SetUseHTML(__in bool val, __in_opt DWORD htmlStartColor = 0xFFFFFFFF) { m_UseHTML = val; m_HTMLColor = htmlStartColor;}
	bool GetUseHTML() const { return m_UseHTML; }
	__declspec(property(get = GetUseHTML)) bool UseHTML;

	/*!
	Загрузка ASCII шрифтов
	@return true при успешной загрузке
	*/
	bool LoadFonts();

	/*!
	Переход по ссылке по индексу
	@param [__in] link Индекс ссылки
	@return 
	*/
	void GoToWebLink(__in WORD link);



	//-----------------------------
	//---Работа с ASCII шрифтами---
	//-----------------------------

	/*!
	Проверка, печатаемый ли символ
	@param [__in] index Индекс символа
	@return true если это печатаемый символ
	*/
	bool IsPrintASCII(__in BYTE index) __in{ return (m_FontIndex[index] != 0xFF); }

	/*!
	Получить смещение символа ширифта
	@param [__in] font Шрифт
	@param [__in] index Индекс символа
	@return Смещение в пикселях
	*/
	int GetFontOffsetY(__in BYTE font, __in BYTE index);

	/*!
	Получить позицию каретки в тексте
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] pos Текущая позиция в тексте
	@param [__in] width Ширина текстуры
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@return Координаты каретки
	*/
	POINT GetCaretPosA(__in BYTE font, __in const char *str, __in int pos, __in int width, __in TEXT_ALIGN_TYPE align, __in WORD flags);

	/*!
	Вычислить положение каретки
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] x Координата каретки по оси X
	@param [__in] y Координата каретки по оси Y
	@param [__in] width Ширина текстуры
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@return Позиция каретки в строке
	*/
	int CalculateCaretPosA(__in BYTE font, __in const char *str, __in int x, __in int y, __in int width, __in TEXT_ALIGN_TYPE align, __in WORD flags);

	/*!
	Получить ширину текста
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in_opt] len Длина текста
	@return Ширина текста в пикселях
	*/
	int GetWidthA(__in BYTE font, __in const char *str, __in_opt int len = 0);

	/*!
	Получить ширину текста (с учетом параметров отрисовки)
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] len Длина текста
	@param [__in] maxWidth Максимальная ширина текстуры
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@return Ширина текста в пикселях
	*/
	int GetWidthExA(__in BYTE font, __in const char *str, __in int len, __in int maxWidth, __in TEXT_ALIGN_TYPE align, __in WORD flags);

	/*!
	Получить высоту текста
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in_opt] width Ширина текстуры
	@param [__in_opt] align Расположение текста
	@param [__in_opt] flags Эффекты текста
	@return Высота текста в пикселях
	*/
	int GetHeightA(__in BYTE font, __in const char *str, __in_opt int width = 0, __in_opt TEXT_ALIGN_TYPE align = TS_LEFT, __in_opt WORD flags = 0);

	/*!
	Получить высоту текста по списку строк
	@param [__in] info Ссылка на мультистрочный текст
	@return Высота текста в пикселях
	*/
	int GetHeightA(__in PMULTILINES_FONT_INFO info);

	/*!
	Получить текст указанной ширины
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] len Длина текста
	@param [__in] width Ширина текстуры
	@param [__in] IsCropped Ограниченный текст, вышедшая за доступные пределы часть обрезается и заменяется на многоточие
	@return Результирующий текст
	*/
	string GetTextByWidthA(__in BYTE font, __in const char *str, __in int len, __in int width, __in bool IsCropped);

	/*!
	Получить информацию о тексте (в мультистрочном виде)
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] len Длина текста
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@param [__in] width Ширина текстуры
	@return Ссылка на мультистрочный текст или NULL
	*/
	PMULTILINES_FONT_INFO GetInfoA(__in BYTE font, __in const char *str, __in int len, __in TEXT_ALIGN_TYPE align, __in WORD flags, __in int width);

	/*!
	Сгенерировать пиксели текстуры текста
	@param [__in] font Шрифт
	@param [__inout] th Данные о текстуре текста
	@param [__in] str Текст
	@param [__in] color Цвет текста
	@param [__in] width Ширина текстуры
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@return Ссылка на массив пикселей
	*/
	PDWORD GeneratePixelsA(__in BYTE &font, __inout TTextTexture &th, __in const char *str, __in WORD &color, __in int &width, __in TEXT_ALIGN_TYPE &align, __in WORD &flags);

	/*!
	Сгенерировать текстуру текста
	@param [__in] font Шрифт
	@param [__inout] th Данные о текстуре текста
	@param [__in] str Текст
	@param [__in_opt] color Цвет
	@param [__in_opt] width Ширина текстуры
	@param [__in_opt] align Расположение текста
	@param [__in_opt] flags Эффекты текста
	@return true при успешной генерации
	*/
	bool GenerateA(__in BYTE font, __inout TTextTexture &th, __in const char *str, __in_opt WORD color = 0, __in_opt int width = 0, __in_opt TEXT_ALIGN_TYPE align = TS_LEFT, __in_opt WORD flags = 0);

	/*!
	Отрисовать текст
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] color Цвет
	@param [__in] x Экранная координата X
	@param [__in] y Экранная координата Y
	@param [__in_opt] width Ширина текстуры
	@param [__in_opt] align Расположение текста
	@param [__in_opt] flags Эффекты текста
	@return 
	*/
	void DrawA(__in BYTE font, __in const char *str, __in WORD color, __in int x, __in int y, __in_opt int width = 0, __in_opt TEXT_ALIGN_TYPE align = TS_LEFT, __in_opt WORD flags = 0);



	//-------------------------------
	//---Работа с Unicode шрифтами---
	//-------------------------------

	/*!
	Получить позицию каретки в тексте
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] pos Текущая позиция в тексте
	@param [__in] width Ширина текстуры
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@return Координаты каретки
	*/
	POINT GetCaretPosW(__in BYTE font, __in const wchar_t *str, __in int pos, __in int width, __in TEXT_ALIGN_TYPE align, __in WORD flags);

	/*!
	Вычислить положение каретки
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] x Координата каретки по оси X
	@param [__in] y Координата каретки по оси Y
	@param [__in] width Ширина текстуры
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@return Позиция каретки в строке
	*/
	int CalculateCaretPosW(__in BYTE font, __in const wchar_t *str, __in int x, __in int y, __in int width, __in TEXT_ALIGN_TYPE align, __in WORD flags);

	/*!
	Получить ширину текста
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in_opt] len Длина текста
	@return Ширина текста в пикселях
	*/
	int GetWidthW(__in BYTE font, __in const wchar_t *str, __in_opt int len = 0);

	/*!
	Получить ширину текста (с учетом параметров отрисовки)
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] len Длина текста
	@param [__in] maxWidth Максимальная ширина текстуры
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@return Ширина текста в пикселях
	*/
	int GetWidthExW(__in BYTE font, __in const wchar_t *str, __in int len, __in int maxWidth, __in TEXT_ALIGN_TYPE align, __in WORD flags);

	/*!
	Получить высоту текста
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in_opt] width Ширина текстуры
	@param [__in_opt] align Расположение текста
	@param [__in_opt] flags Эффекты текста
	@return Высота текста в пикселях
	*/
	int GetHeightW(__in BYTE font, __in const wchar_t *str, __in_opt int width = 0, __in_opt TEXT_ALIGN_TYPE align = TS_LEFT, __in_opt WORD flags = 0);

	/*!
	Получить высоту текста по списку строк
	@param [__in] info Ссылка на мультистрочный текст
	@return Высота текста в пикселях
	*/
	int GetHeightW(__in PMULTILINES_FONT_INFO info);

	/*!
	Получить текст указанной ширины
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] len Длина текста
	@param [__in] width Ширина текстуры
	@param [__in] IsCropped Ограниченный текст, вышедшая за доступные пределы часть обрезается и заменяется на многоточие
	@return Результирующий текст
	*/
	wstring GetTextByWidthW(__in BYTE font, __in const wchar_t *str, __in int len, __in int width, __in bool IsCropped);

	/*!
	Получить информацию о тексте (в мультистрочном виде)
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] len Длина текста
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@param [__in] width Ширина текстуры
	@return Ссылка на мультистрочный текст или NULL
	*/
	PMULTILINES_FONT_INFO GetInfoW(__in BYTE font, __in const wchar_t *str, __in int len, __in TEXT_ALIGN_TYPE align, __in WORD flags, __in int width);

	/*!
	Сгенерировать пиксели текстуры текста
	@param [__in] font Шрифт
	@param [__inout] th Данные о текстуре текста
	@param [__in] str Текст
	@param [__in] color Цвет текста
	@param [__in] cell Ячейка в палитре цветов
	@param [__in] width Ширина текстуры
	@param [__in] align Расположение текста
	@param [__in] flags Эффекты текста
	@return Ссылка на массив пикселей
	*/
	PDWORD GeneratePixelsW(__in BYTE &font, __inout TTextTexture &th, __in const wchar_t *str, __in WORD &color, __in BYTE &cell, __in int &width, __in TEXT_ALIGN_TYPE &align, __in WORD &flags);

	/*!
	Сгенерировать текстуру текста
	@param [__in] font Шрифт
	@param [__inout] th Данные о текстуре текста
	@param [__in] str Текст
	@param [__in_opt] color Цвет
	@param [__in_opt] cell Ячейка в палитре цветов
	@param [__in_opt] width Ширина текстуры
	@param [__in_opt] align Расположение текста
	@param [__in_opt] flags Эффекты текста
	@return true при успешной генерации
	*/
	bool GenerateW(__in BYTE font, __inout TTextTexture &th, __in const wchar_t *str, __in_opt WORD color = 0, __in_opt BYTE cell = 30, __in_opt int width = 0, __in_opt TEXT_ALIGN_TYPE align = TS_LEFT, __in_opt WORD flags = 0);

	/*!
	Отрисовать текст
	@param [__in] font Шрифт
	@param [__in] str Текст
	@param [__in] color Цвет
	@param [__in] x Экранная координата X
	@param [__in] y Экранная координата Y
	@param [__in_opt] cell Ячейка в палитре цветов
	@param [__in_opt] width Ширина текстуры
	@param [__in_opt] align Расположение текста
	@param [__in_opt] flags Эффекты текста
	@return
	*/
	void DrawW(__in BYTE font, __in const wchar_t *str, __in WORD color, __in int x, __in int y, __in_opt BYTE cell = 30, __in_opt int width = 0, __in_opt TEXT_ALIGN_TYPE align = TS_LEFT, __in_opt WORD flags = 0);
};
//--------------------------------------------------------------------------
//!Ссылка на менеджер шрифтов
extern TFontsManager *FontManager;
//--------------------------------------------------------------------------
#endif