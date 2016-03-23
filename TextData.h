/****************************************************************************
**
** TextData.h
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
#ifndef TextDataH
#define TextDataH
//---------------------------------------------------------------------------
//Класс текста объекта мира
class TRenderTextObject
{
private:
	//Экранные координаты
	int m_DrawX;
	int m_DrawY;

	//Прозрачность текста
	bool m_Transparent;
public:
	TRenderTextObject();
	virtual ~TRenderTextObject();

	//Вытащить из очереди
	void Unlink();

	//Наверх очереди
	void ToTop();
	
	SETGET(int, DrawX);
	SETGET(int, DrawY);
	SETGET(bool, Transparent);

	//Текст ли это
	virtual bool IsText() {return false;}

	//Ссылки на следующий и предыдущий элементы
	TRenderTextObject *m_NextDraw;
	TRenderTextObject *m_PrevDraw;
};
//---------------------------------------------------------------------------
class TRenderWorldObject;
//---------------------------------------------------------------------------
//Класс для обработки вывода текста на экран
class TTextRenderer : public TRenderTextObject
{
private:
	//Список координат, занятых текстом
	std::deque<TTextImageBounds> m_TextRect;

public:
	TTextRenderer();
	virtual ~TTextRenderer();

	//Список текста
	TRenderTextObject *m_Items;

	//Добавить текст
	TRenderTextObject *Add(TRenderTextObject *obj);

	//Переместить текст наверх очереди
	void ToTop(TRenderTextObject *obj);

	//Добавить координаты текста
	void AddRect(TTextImageBounds &rect) {m_TextRect.push_back(rect);}

	//Очистить список координат текста
	void ClearRect() {m_TextRect.clear();}

	//Проверка, есть ли текст в координатах
	bool InRect(TTextImageBounds &rect, TRenderWorldObject *rwo);
};
//---------------------------------------------------------------------------
//Класс для хранения информации о текстуре текста
class TTextData : public TRenderTextObject
{
private:
	//Это юникод текст
	bool m_Unicode;

	//Тип текста
	TEXT_TYPE m_Type;

	//Серийник хозяина
	DWORD m_Serial;

	//Индекс шрифта
	WORD m_Font;

	//Цвет
	WORD m_Color;

	//Время жизни текста
	DWORD m_Timer;

	//ACSII текст
	string m_Text;

	//Unicode текст
	wstring m_UnicodeText;

public:
	TTextData();
	TTextData(TTextData *obj);
	virtual ~TTextData();

	virtual bool IsText() {return true;}

	//Ссылки на следующий и предыдущий элементы
	TTextData *m_Next;
	TTextData *m_Prev;
	
	SETGET(bool, Unicode);
	SETGET(TEXT_TYPE, Type);
	SETGET(DWORD, Serial);
	SETGET(WORD, Font);
	SETGET(WORD, Color);
	SETGET(DWORD, Timer);
	SETGET(string, Text);
	SETGET(wstring, UnicodeText);

	bool CanBeDrawedInJournalGump();

	//Текстура текста
	TTextTexture m_Texture;

	//Генерация текстуры текста
	void GenerateTexture(int MaxWidth, WORD flags = 0, TEXT_ALIGN_TYPE align = TS_LEFT, BYTE cell = 30, int font = -1);
};
//---------------------------------------------------------------------------
//Контейнер для текста
class TTextContainer
{
private:
	//Максимальный размер контейнера
	int m_MaxSize;

	//Текущий размер контейнера
	int m_Size;

public:
	TTextContainer(int max_size);
	~TTextContainer();

	TTextData *m_Head; //Указатель на ПОСЛЕДНИЙ добавленный элемент
	TTextData *m_Top; //Указатель на ПЕРВЫЙ добавленный элемент (из записанных)
	
	SETGET(int, MaxSize);
	SETGET(int, Size);

	//Добавить текст
	void Add(TTextData *obj);

	//Очистить контейнер
	void Clear();

	//Отрисовать системный чат
	void DrawSystemChat(int &x, int &y, int &height);
};
//---------------------------------------------------------------------------
//Класс для хранения и учета сообщений в журнале
class TJournal
{
private:
	//Максимальный размер
	int m_MaxSize;

	//Текущий размер
	int m_Size;

	//Высота сообщений журнала
	int m_Height;

	//Высота системных сообщений
	int m_SystemHeight;

	//Высота сообщений от объектов
	int m_ObjectsHeight;

	//Высота сообщений клиента
	int m_ClientHeight;

	//Индекс выбранного сообщения (на котором остановились в гампе)
	int m_SelectionIndex;

public:
	TJournal(int max_size);
	~TJournal();

	TTextData *m_Head; //Указатель на ПОСЛЕДНИЙ добавленный элемент
	TTextData *m_Top; //Указатель на ПЕРВЫЙ добавленный элемент (из записанных)
	
	SETGET(int, MaxSize);
	SETGET(int, Size);
	SETGET(int, Height);
	SETGET(int, SystemHeight);
	SETGET(int, ObjectsHeight);
	SETGET(int, ClientHeight);
	SETGETEX(int, SelectionIndex);

	//Добавить сообщение в журнал
	void Add(TTextData *obj);

	//Очистить журнал
	void Clear();
};
//---------------------------------------------------------------------------
extern TTextContainer *SystemChat;
extern TJournal *Journal;
extern TTextRenderer *WorldTextRenderer;
extern TRenderTextObject *g_SelectedTextObject;
//---------------------------------------------------------------------------
#endif