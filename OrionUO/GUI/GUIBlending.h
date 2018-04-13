/***********************************************************************************
**
** GUIBlending.h
**
** Компонента для применения блендинга
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIBLENDING_H
#define GUIBLENDING_H
//----------------------------------------------------------------------------------
class CGUIBlending : public CBaseGUI
{
	//!Функция для исходных пикселей
	GLenum SFactor = GL_SRC_ALPHA;

	//!Функция для входных пикселей
	GLenum DFactor = GL_ONE_MINUS_SRC_ALPHA;

public:
	CGUIBlending(const bool &enabled, const GLenum &sFactor, const GLenum &dFactor);
	virtual ~CGUIBlending();

	virtual void Draw(const bool &checktrans = false);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
