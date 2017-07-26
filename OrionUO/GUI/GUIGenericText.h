/***********************************************************************************
**
** GUIGenericText.h
**
** Компонента текста для гампов от сервера
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIGENERICTEXT_H
#define GUIGENERICTEXT_H
//----------------------------------------------------------------------------------
#include "GUIText.h"
//----------------------------------------------------------------------------------
class CGUIGenericText : public CGUIText
{
	//!Индекс текста
	SETGET(uint, TextID, 0);

	//!Максимальная ширина (для CroppedText)
	SETGET(int, MaxWidth, 0);

public:
	CGUIGenericText(const uint &index, const ushort &color, const int &x, const int &y, const int &maxWidth = 0);
	virtual ~CGUIGenericText();

	void CreateTexture(const wstring &str);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
