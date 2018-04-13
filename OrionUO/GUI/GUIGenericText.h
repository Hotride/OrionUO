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
class CGUIGenericText : public CGUIText
{
public:
	//!Индекс текста
	uint TextID = 0;

	//!Максимальная ширина (для CroppedText)
	int MaxWidth = 0;

	CGUIGenericText(const uint &index, const ushort &color, const int &x, const int &y, const int &maxWidth = 0);
	virtual ~CGUIGenericText();

	void CreateTexture(const wstring &str);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
