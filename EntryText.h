/****************************************************************************
**
** EntryText.h
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
#ifndef EntryTextH
#define EntryTextH
//---------------------------------------------------------------------------
//Класс для работы с полем для ввода текста
class TEntryText
{
private:
	//Unicode строка
	wstring m_Text;

	//ASCII строка
	string m_CText;

	//Максимальная длина строки
	int m_MaxLength;

	//Позиция каретки в строке
	int m_Position;

	//Флаг изменения строки
	bool m_Changed;

	//Смещение рисования текста
	int m_DrawOffset;

	//Ширина текста
	int m_Width;

	//Максимальная ширина текста
	int m_MaxWidth;

	//Цвет текста
	WORD m_Color;

	//Координаты каретки
	POINT m_CaretPos;

	//Флаг для ввода только цифр
	bool m_NumberOnly;

public:
	TEntryText(int maxLength = 0, int width = 0, int maxWidth = 0, bool numberOnly = false);
	virtual ~TEntryText();

	//Текстура текста
	TTextTexture m_Texture;

	//Очистить данные
	void Clear();

	//Вставить символ
	bool Insert(wchar_t ch, TGump *gump = NULL);

	//Удалить символ
	void Remove(bool left, TGump *gump = NULL);

	//Сместить позицию каретки в тексте
	void AddPos(int val, TGump *gump = NULL);

	//Установить позицию каретки в тексте
	void SetPos(int val, TGump *gump = NULL);

	//Unicode данные строки
	const wchar_t *Data() const {return m_Text.c_str();}

	//ASCII данные стоки
	const char *c_str();

	//Установить ASCII текст
	void SetText(const string &text);

	//Установить Unicode текст
	void SetText(const wstring &text);

	//Событие нажатия мышкой на текст
	void OnClick(TGump *gump, BYTE font, bool unicode, int x, int y, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);

	//Нажатие клавиши в активном текстовом поле
	void OnKey(TGump *gump, WPARAM wParam);

	SETGET(int, MaxLength);
	SETGET(int, Width);
	SETGET(int, MaxWidth);
	SETGET(bool, Changed);
	SETGET(WORD, Color);
	SETGET(int, DrawOffset);
	SETGET(POINT, CaretPos);

	int Pos() const {return m_Position;}

	int Length() const {return m_Text.length();}

	//Вычислить ASCII строку по ширине
	string CheckMaxWidthA(BYTE font, string str);

	//Вычислить Unicode строку по ширине
	wstring CheckMaxWidthW(BYTE font, wstring str);

	//Отформатировать ASCII текст по ширине
	void FixMaxWidthA(BYTE font);

	//Отформатировать Unicode текст по ширине
	void FixMaxWidthW(BYTE font);

	//Создать текстуру ASCII текста
	void CreateTextureA(BYTE font, string str, WORD color, int width, TEXT_ALIGN_TYPE align, WORD flags);

	//Создать текстуру Unicode текста
	void CreateTextureW(BYTE font, wstring str, WORD color, int width, TEXT_ALIGN_TYPE align, WORD flags);

	//Отрисовать ASCII текст
	virtual void DrawA(BYTE font, WORD color, int X, int Y, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);

	//Отрисовать Unicode текст
	virtual void DrawW(BYTE font, WORD color, int X, int Y, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);

	//Отрисовать ASCII текст замаскированный звездочками
	void DrawMaskA(BYTE font, WORD color, int X, int Y, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);

	//Отрисовать Unicode текст замаскированный звездочками
	void DrawMaskW(BYTE font, WORD color, int X, int Y, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);
};
//---------------------------------------------------------------------------
extern TEntryText *EntryPointer;
//---------------------------------------------------------------------------
#endif