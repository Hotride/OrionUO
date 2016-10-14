/***********************************************************************************
**
** GUIText.h
**
** Компонента для отображения текста
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUITEXT_H
#define GUITEXT_H
//----------------------------------------------------------------------------------
#include "BaseGUI.h"
#include "../GLEngine/GLTextTexture.h"
//----------------------------------------------------------------------------------
class CGUIText : public CBaseGUI
{
public:
	CGUIText(const ushort &color, const int &x, const int &y);
	virtual ~CGUIText();

	//!Текстура текста
	CGLTextTexture m_Texture;

	virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(m_Texture.Width, m_Texture.Height); }

	//!Создать обычную текстуру текста
	void CreateTextureA(const uchar &font, const string &str, int width = 0, const TEXT_ALIGN_TYPE &align = TS_LEFT, const ushort &flags = 0);

	//!Создать юникодовскую текстуру текста
	void CreateTextureW(const uchar &font, const wstring &str, const uchar &cell = 30, int width = 0, const TEXT_ALIGN_TYPE &align = TS_LEFT, const ushort &flags = 0);

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
