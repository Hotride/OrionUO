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
#include "RenderTextObject.h"
#include "../GLEngine/GLTextTexture.h"
//----------------------------------------------------------------------------------
//Класс для хранения информации о текстуре текста
class CTextData : public CRenderTextObject
{
	SETGET(bool, Unicode);
	SETGET(TEXT_TYPE, Type);
	SETGET(uint, Serial);
	SETGET(uchar, Font);
	SETGET(uint, Timer);
	SETGET(string, Text);
	SETGET(wstring, UnicodeText);
	SETGET(int, DrawX);
	SETGET(int, DrawY);
	SETGET(uchar, Alpha);

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
