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
#include "BaseGUI.h"
#include "../GLEngine/GLHTMLTextTexture.h"
//----------------------------------------------------------------------------------
class CGUIHTMLText : public CBaseGUI
{
	//!Индекс текста
	SETGET(uint, TextID);

	//!Начальный цвет текста
	SETGET(uint, HTMLStartColor);

	//!Текст
	SETGET(wstring, Text);

	//!Шрифт
	SETGET(uchar, Font);

	//!Ориентация текста
	SETGET(TEXT_ALIGN_TYPE, Align);

	//!Флаги текста
	SETGET(ushort, TextFlags);

	//!Ширина текста
	SETGET(int, Width);

public:
	CGUIHTMLText(const uint &index, const uchar &font, const ushort &color, const int &x, const int &y, const int &width = 0, const TEXT_ALIGN_TYPE &align = TS_LEFT, const ushort &textFlags = 0, const uint &htmlStartColor = 0xFFFFFFFF);
	virtual ~CGUIHTMLText();

	//!Текстура текста
	CGLHTMLTextTexture m_Texture;

	virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(m_Texture.Width, m_Texture.Height); }

	//!Создать текстуру текста
	void CreateTexture(const bool &backgroundCanBeColored);

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
