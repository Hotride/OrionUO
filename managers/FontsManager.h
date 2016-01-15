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
//---------------------------------------------------------------------------
#ifndef FontsManagerH
#define FontsManagerH
//--------------------------------------------------------------------------
const int g_UnicodeSpaceWidth = 8;
const float g_ItalicFontKoeff = 3.3f;
const int g_MaxHTMLTextHeight = 18;
//--------------------------------------------------------------------------
struct WEB_LINK
{
	bool Visited;
	string WebLink;
};
//--------------------------------------------------------------------------
typedef std::map<WORD, WEB_LINK> WEBLINK_MAP;
//--------------------------------------------------------------------------
struct HTML_char
{
	wchar_t Char;
	BYTE Font;
	TEXT_ALIGN_TYPE Align;
	WORD Flags;
	DWORD Color;
	WORD LinkID;
};
//--------------------------------------------------------------------------
class TFontsManager
{
private:
	FONT_DATA *m_Font;
	int m_FontCount;

	WEBLINK_MAP m_WebLink;

	static BYTE m_FontIndex[256];
	
	DWORD m_UnicodeFontAddress[20];
	DWORD m_UnicodeFontSize[20];

	bool m_SavePixels;

	bool m_UseHTML;
	DWORD m_HTMLColor;
	
	WORD GetWebLinkID(wchar_t *link, DWORD &color);

	HTML_char *GetHTMLData(BYTE font, const wchar_t *str, int &len, TEXT_ALIGN_TYPE align, WORD flags);
	PMULTILINES_FONT_INFO GetInfoHTML(BYTE font, const wchar_t *str, int len, TEXT_ALIGN_TYPE align, WORD flags, int width);
	
	bool GenerateABase(BYTE &font, TTextTexture &th, const char *str, WORD &color, int &width, TEXT_ALIGN_TYPE &align, WORD &flags);

	bool GenerateWBase(BYTE &font, TTextTexture &th, const wchar_t *str, WORD &color, BYTE &cell, int &width, TEXT_ALIGN_TYPE &align, WORD &flags);

public:
	TFontsManager();
	~TFontsManager();
	
	SETGET(bool, SavePixels)

	void SetUseHTML(bool val, DWORD htmlStartColor = 0xFFFFFFFF) {m_UseHTML = val; m_HTMLColor = htmlStartColor;}
	bool GetUseHTML() const {return m_UseHTML;}
	__declspec(property(get = GetUseHTML)) bool UseHTML;
	
	bool LoadFonts();

	void GoToWebLink(WORD link);
	
	//ASCII
	bool IsPrintASCII(BYTE index) {return (m_FontIndex[index] != 0xFF);}
	int GetFontOffsetY(BYTE Number, BYTE Index);
	POINT GetCaretPosA(BYTE font, const char *str, int pos, int width, TEXT_ALIGN_TYPE align, WORD flags);
	int CalculateCaretPosA(BYTE font, const char *str, int x, int y, int width, TEXT_ALIGN_TYPE align, WORD flags);
	int GetWidthA(BYTE font, const char *str, int len = 0);
	int GetHeightA(BYTE font, const char *str, int width = 0, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);
	int GetHeightA(PMULTILINES_FONT_INFO info);
	string GetTextByWidthA(BYTE font, const char *str, int len, int width, bool IsCropped);
	PMULTILINES_FONT_INFO GetInfoA(BYTE font, const char *str, int len, TEXT_ALIGN_TYPE align, WORD flags, int width);
	bool GenerateA(BYTE font, TTextTexture &th, const char *str, WORD color = 0, int width = 0, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);
	void DrawA(BYTE font, const char *str, WORD color, int X, int Y, int width = 0, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);



	//Unicode
	POINT GetCaretPosW(BYTE font, const wchar_t *str, int pos, int width, TEXT_ALIGN_TYPE align, WORD flags);
	int CalculateCaretPosW(BYTE font, const wchar_t *str, int x, int y, int width, TEXT_ALIGN_TYPE align, WORD flags);
	int GetWidthW(BYTE font, const wchar_t *str, int len = 0);
	int GetHeightW(BYTE font, const wchar_t *str, int width = 0, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);
	int GetHeightW(PMULTILINES_FONT_INFO info);
	wstring GetTextByWidthW(BYTE font, const wchar_t *str, int len, int width, bool IsCropped);
	PMULTILINES_FONT_INFO GetInfoW(BYTE font, const wchar_t *str, int len, TEXT_ALIGN_TYPE align, WORD flags, int width);
	bool GenerateW(BYTE font, TTextTexture &th, const wchar_t *str, WORD color = 0, BYTE cell = 30, int width = 0, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);
	void DrawW(BYTE font, const wchar_t *str, WORD color, int X, int Y, BYTE cell = 30, int width = 0, TEXT_ALIGN_TYPE align = TS_LEFT, WORD flags = 0);
};

//--------------------------------------------------------------------------
extern TFontsManager *FontManager;
//--------------------------------------------------------------------------
#endif