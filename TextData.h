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
class TRenderTextObject
{
private:
	int m_DrawX;
	int m_DrawY;
	bool m_Transparent;
public:
	TRenderTextObject();
	virtual ~TRenderTextObject();

	void Unlink();
	void ToTop();
	
	SETGET(int, DrawX);
	SETGET(int, DrawY);
	SETGET(bool, Transparent);

	virtual bool IsText() {return false;}
	
	TRenderTextObject *m_NextDraw;
	TRenderTextObject *m_PrevDraw;
};
//---------------------------------------------------------------------------
class TRenderWorldObject;
//---------------------------------------------------------------------------
class TTextRenderer : public TRenderTextObject
{
private:
	std::deque<TEXT_IMAGE_BOUNDS> m_TextRect;

public:
	TTextRenderer();
	virtual ~TTextRenderer();
	
	TRenderTextObject *m_Items;
	
	TRenderTextObject *Add(TRenderTextObject *obj);
	void ToTop(TRenderTextObject *obj);

	void AddRect(TEXT_IMAGE_BOUNDS &rect) {m_TextRect.push_back(rect);}
	void ClearRect() {m_TextRect.clear();}
	bool InRect(TEXT_IMAGE_BOUNDS &rect, TRenderWorldObject *rwo);
};
//---------------------------------------------------------------------------
class TTextData : public TRenderTextObject
{
private:
	bool m_Unicode;
	TEXT_TYPE m_Type;
	DWORD m_Serial;
	WORD m_Font;
	WORD m_Color;
	DWORD m_Timer;

	string m_Text;
	wstring m_UnicodeText;

public:
	TTextData();
	TTextData(TTextData *obj);
	virtual ~TTextData();

	virtual bool IsText() {return true;}

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
	
	TTextTexture m_Texture;

	void GenerateTexture(int MaxWidth, WORD flags = 0, TEXT_ALIGN_TYPE align = TS_LEFT, BYTE cell = 30, int font = -1);
};
//---------------------------------------------------------------------------
class TTextContainer
{
private:
	int m_MaxSize;
	int m_Size;

public:
	TTextContainer(int max_size);
	~TTextContainer();

	TTextData *m_Head; //Указатель на ПОСЛЕДНИЙ добавленный элемент
	TTextData *m_Top; //Указатель на ПЕРВЫЙ добавленный элемент (из записанных)
	
	SETGET(int, MaxSize);
	SETGET(int, Size);

	void Add(TTextData *obj);

	void Clear();
};
//---------------------------------------------------------------------------
class TJournal
{
private:
	int m_MaxSize;
	int m_Size;
	int m_Height;
	int m_SystemHeight;
	int m_ObjectsHeight;
	int m_ClientHeight;
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

	void Add(TTextData *obj);

	void Clear();
};
//---------------------------------------------------------------------------
extern TTextContainer *SystemChat;
extern TJournal *Journal;
extern TTextRenderer *WorldTextRenderer;
extern TRenderTextObject *g_SelectedTextObject;
//---------------------------------------------------------------------------
#endif