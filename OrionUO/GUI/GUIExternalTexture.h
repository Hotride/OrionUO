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
	bool DeleteTextureOnDestroy = false;

	short DrawWidth = 0;
	short DrawHeight = 0;

public:
	CGUIExternalTexture(CGLTexture *texture, bool deleteTextureOnDestroy, int x, int y, int drawWidth = 0, int drawHeight = 0);
	virtual ~CGUIExternalTexture();

	//!Ссылка на текстуру
	CGLTexture *m_Texture;

	virtual WISP_GEOMETRY::CSize GetSize();

	virtual void SetShaderMode();
	virtual void Draw(bool checktrans = false);
	virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
