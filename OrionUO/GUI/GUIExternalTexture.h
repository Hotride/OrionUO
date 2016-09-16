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
#include "BaseGUI.h"
//----------------------------------------------------------------------------------
class CGUIExternalTexture : public CBaseGUI
{
	//!Удалять ли текстуру при удалении объекта
	SETGET(bool, DeleteTextureOnDestroy);

public:
	CGUIExternalTexture(CGLTexture *texture, const bool &deleteTextureOnDestroy, const int &x, const int &y);
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
