/***********************************************************************************
**
** GUIComboboxText.h
**
** Компонента для отображения текста комбобокса
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUICOMBOBOXTEXT_H
#define GUICOMBOBOXTEXT_H
//----------------------------------------------------------------------------------
class CGUIComboboxText : public CGUIText
{
public:
	CGUIComboboxText(const ushort &color, const uchar &font, const string &str, const int &width = 0, const TEXT_ALIGN_TYPE &align = TS_LEFT, const ushort &flags = 0);
	CGUIComboboxText(const ushort &color, const uchar &font, const wstring &str, const int &width = 0, const TEXT_ALIGN_TYPE &align = TS_LEFT, const ushort &flags = 0);
	virtual ~CGUIComboboxText();

	virtual void OnMouseEnter();
	virtual void OnMouseExit();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
