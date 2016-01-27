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
class TEntryText
{
private:
	wstring m_Text;
	string m_CText;
	int m_MaxLength;
	int m_Position;
	bool m_Changed;
	int m_DrawOffset;
	int m_Width;
	int m_MaxWidth;
	WORD m_Color;
	POINT m_CaretPos;
	bool m_NumberOnly;

public:
	TEntryText(int maxLength = 0, int width = 0, int maxWidth = 0, bool numberOnly = false);
	virtual ~TEntryText();

	TTextTexture m_Texture;
	
	void Clear();

	bool Insert(wchar_t ch, TGump *gump = NULL);
	void Remove(bool left, TGump *gump = NULL);

	void AddPos(int val, TGump *gump = NULL);
	void SetPos(int val, TGump *gump = NULL);

	const wchar_t *Data() const {return m_Text.c_str();}
	const char *c_str();

	void SetText(const string &text);
	void SetText(const wstring &text);

	void OnClick(TGump *gump, BYTE font, bool unicode, int x, int y, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);
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

	string CheckMaxWidthA(BYTE font, string str);
	wstring CheckMaxWidthW(BYTE font, wstring str);

	void FixMaxWidthA(BYTE font);
	void FixMaxWidthW(BYTE font);

	void CreateTextureA(BYTE font, string str, WORD color, int width, TEXT_ALIGN_TYPE align, WORD flags);
	void CreateTextureW(BYTE font, wstring str, WORD color, int width, TEXT_ALIGN_TYPE align, WORD flags);

	virtual void DrawA(BYTE font, WORD color, int X, int Y, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);
	virtual void DrawW(BYTE font, WORD color, int X, int Y, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);
	void DrawMaskA(BYTE font, WORD color, int X, int Y, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);
	void DrawMaskW(BYTE font, WORD color, int X, int Y, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);
};
//---------------------------------------------------------------------------
extern TEntryText *EntryPointer;
//---------------------------------------------------------------------------
#endif