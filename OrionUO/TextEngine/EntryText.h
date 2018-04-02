/***********************************************************************************
**
** EntryText.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef ENTRYTEXT_H
#define ENTRYTEXT_H
//----------------------------------------------------------------------------------
class CGump;
//----------------------------------------------------------------------------------
//!Класс для работы с полем для ввода текста
class CEntryText
{
	SETGET(int, MaxLength, 0);
	SETGET(int, Width, 0);
	SETGET(int, MaxWidth, 0);
	SETGET(bool, Changed, true);
	SETGET(ushort, Color, 0);
	SETGET(int, DrawOffset, 0);
	SETGET(WISP_GEOMETRY::CPoint2Di, CaretPos, WISP_GEOMETRY::CPoint2Di());
	SETGET(bool, NumberOnly, false);

protected:
	//!Позиция каретки в строке
	int m_Position{ 0 };

	//!ASCII строка
	string m_CText = string{ "" };

	//!Unicode строка
	wstring m_Text = wstring{ L"" };


public:
	CEntryText(int maxLength = 0, int width = 0, int maxWidth = 0, bool numberOnly = false);
	virtual ~CEntryText();

	//!Текстура текста
	CGLTextTexture m_Texture;

	//!Текстура каретки
	CGLTextTexture m_CaretTexture;

	/*!
	Очистить данные
	@return 
	*/
	void Clear();

	void Paste();

	//Получить количество строк
	int GetLinesCountA(uchar font, TEXT_ALIGN_TYPE align = TS_LEFT, ushort flags = 0, int width = 0);

	//Получить количество строк
	int GetLinesCountW(uchar font, TEXT_ALIGN_TYPE align = TS_LEFT, ushort flags = 0, int width = 0);

	//Вставить символ
	bool Insert(wchar_t ch, CGump *gump = NULL);

	//Удалить символ
	void Remove(bool left, CGump *gump = NULL);

	//Delete a sequence of symbols
	void RemoveSequence(int startPos, int length);

	//Получить самый последний символ
	WPARAM GetLastChar();

	//Сместить позицию каретки в тексте
	void AddPos(int val, CGump *gump = NULL);

	//Установить позицию каретки в тексте
	void SetPos(int val, CGump *gump = NULL);

	//Unicode данные строки
	const wchar_t *Data() const {return m_Text.c_str();}

	//ASCII данные стоки
	const char *c_str();

	//Установить ASCII текст
	void SetText(const string &text);

	//Установить Unicode текст
	void SetText(const wstring &text);

	string GetTextA() const;

	wstring GetTextW() const;

	//Событие нажатия мышкой на текст
	void OnClick(CGump *gump, uchar font, bool unicode, int x, int y, TEXT_ALIGN_TYPE align = TS_LEFT, ushort flags = 0);

	//Нажатие клавиши в активном текстовом поле
	void OnKey(CGump *gump, WPARAM wParam);

	int Pos() const {return m_Position;}

	size_t Length() const { return m_Text.length(); }

	//Вычислить ASCII строку по ширине
	string CheckMaxWidthA(uchar font, string str);

	//Вычислить Unicode строку по ширине
	wstring CheckMaxWidthW(uchar font, wstring str);

	//Отформатировать ASCII текст по ширине
	void FixMaxWidthA(uchar font);

	//Отформатировать Unicode текст по ширине
	void FixMaxWidthW(uchar font);

	//Создать текстуру ASCII текста
	void CreateTextureA(uchar font, string str, ushort color, int width, TEXT_ALIGN_TYPE align, ushort flags);

	//Создать текстуру Unicode текста
	void CreateTextureW(uchar font, wstring str, ushort color, int width, TEXT_ALIGN_TYPE align, ushort flags);

	//Подготовить ASCII текст
	virtual void PrepareToDrawA(uchar font, ushort color, TEXT_ALIGN_TYPE align = TS_LEFT, ushort flags = 0);

	//Подготовить Unicode текст
	virtual void PrepareToDrawW(uchar font, ushort color, TEXT_ALIGN_TYPE align = TS_LEFT, ushort flags = 0);

	//Отрисовать ASCII текст
	virtual void DrawA(uchar font, ushort color, int x, int y, TEXT_ALIGN_TYPE align = TS_LEFT, ushort flags = 0);

	//Отрисовать Unicode текст
	virtual void DrawW(uchar font, ushort color, int x, int y, TEXT_ALIGN_TYPE align = TS_LEFT, ushort flags = 0);

	//Отрисовать ASCII текст замаскированный звездочками
	void DrawMaskA(uchar font, ushort color, int X, int Y, TEXT_ALIGN_TYPE align = TS_LEFT, ushort flags = 0);

	//Отрисовать Unicode текст замаскированный звездочками
	void DrawMaskW(uchar font, ushort color, int X, int Y, TEXT_ALIGN_TYPE align = TS_LEFT, ushort flags = 0);
};
//----------------------------------------------------------------------------------
extern CEntryText *g_EntryPointer;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
