/***********************************************************************************
**
** GUIExternalTexture.h
**
** Компонента для отображения внешней текстуры
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIEXTERNALTEXTURE_H
#define GUIEXTERNALTEXTURE_H
//----------------------------------------------------------------------------------
class CGUIExternalTexture : public CBaseGUI
{
	//!Удалять ли текстуру при удалении объекта
	SETGET(bool, DeleteTextureOnDestroy, false);

	SETGET(short, DrawWidth, 0);
	SETGET(short, DrawHeight, 0);

public:
	CGUIExternalTexture(CGLTexture *texture, const bool &deleteTextureOnDestroy, const int &x, const int &y, const int &drawWidth = 0, const int &drawHeight = 0);
	virtual ~CGUIExternalTexture();

	//!Ссылка на текстуру
	CGLTexture *m_Texture;

	virtual WISP_GEOMETRY::CSize GetSize();

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
