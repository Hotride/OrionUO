/***********************************************************************************
**
** TextData.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef TEXTDATA_H
#define TEXTDATA_H
//----------------------------------------------------------------------------------
//Класс для хранения информации о текстуре текста
class CTextData : public CRenderTextObject
{
	SETGET(bool, Unicode, false);
	SETGET(TEXT_TYPE, Type, TT_CLIENT);
	SETGET(uint, Serial, 0);
	SETGET(uchar, Font, 0);
	SETGET(uint, Timer, 0);
	SETGET(string, Text, "");
	SETGET(wstring, UnicodeText, L"");
	SETGET(uchar, Alpha, 0xFF);

public:
	CTextData();
	CTextData(CTextData *obj);
	virtual ~CTextData();

	virtual bool IsText() { return true; }

	bool CanBeDrawedInJournalGump();

	//Текстура текста
	CGLTextTexture m_Texture;

	//Генерация текстуры текста
	void GenerateTexture(const int &maxWidth, const ushort &flags = 0, const TEXT_ALIGN_TYPE &align = TS_LEFT, const uchar &cell = 30, int font = -1);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
