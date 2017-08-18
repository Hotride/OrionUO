/***********************************************************************************
**
** GUIHTMLText.h
**
** Компонента текста с возможностью перехода по HTML-ссылкам для HTMLGump'а
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIHTMLTEXT_H
#define GUIHTMLTEXT_H
//----------------------------------------------------------------------------------
class CGUIHTMLText : public CBaseGUI
{
	//!Индекс текста
	SETGET(uint, TextID, 0);

	//!Начальный цвет текста
	SETGET(uint, HTMLStartColor, 0);

	//!Текст
	SETGET(wstring, Text, L"");

	//!Шрифт
	SETGET(uchar, Font, 0);

	//!Ориентация текста
	SETGET(TEXT_ALIGN_TYPE, Align, TS_LEFT);

	//!Флаги текста
	SETGET(ushort, TextFlags, 0);

	//!Ширина текста
	SETGET(int, Width, 0);

public:
	CGUIHTMLText(const uint &index, const uchar &font, const ushort &color, const int &x, const int &y, const int &width = 0, const TEXT_ALIGN_TYPE &align = TS_LEFT, const ushort &textFlags = 0, const uint &htmlStartColor = 0xFFFFFFFF);
	virtual ~CGUIHTMLText();

	//!Текстура текста
	CGLHTMLTextTexture m_Texture{ CGLHTMLTextTexture() };

	virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(m_Texture.Width, m_Texture.Height); }

	//!Создать текстуру текста
	void CreateTexture(const bool &backgroundCanBeColored);

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
