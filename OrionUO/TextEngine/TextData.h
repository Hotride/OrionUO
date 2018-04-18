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
public:
	bool Unicode = false;
	TEXT_TYPE Type = TT_CLIENT;
	uchar Font = 0;
	uint Timer = 0;
	uint MoveTimer = 0;
	string Text = "";
	wstring UnicodeText = L"";
	uchar Alpha = 0xFF;
	CRenderWorldObject* Owner = NULL;
	CTextData();
	CTextData(CTextData *obj);
	virtual ~CTextData();

	virtual bool IsText() { return true; }

	bool CanBeDrawedInJournalGump();

	//Текстура текста
	CGLTextTexture m_Texture;

	//Генерация текстуры текста
	void GenerateTexture(int maxWidth, ushort flags = 0, TEXT_ALIGN_TYPE align = TS_LEFT, uchar cell = 30, int font = -1);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
