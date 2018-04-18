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
	CGUIComboboxText(ushort color, uchar font, const string &str, int width = 0, TEXT_ALIGN_TYPE align = TS_LEFT, ushort flags = 0);
	CGUIComboboxText(ushort color, uchar font, const wstring &str, int width = 0, TEXT_ALIGN_TYPE align = TS_LEFT, ushort flags = 0);
	virtual ~CGUIComboboxText();

	virtual void OnMouseEnter();
	virtual void OnMouseExit();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
