/***********************************************************************************
**
** GUIGlobalColor.h
**
** Компонента для применения глобального цвета
**
** Copyright (C) December 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIGLOBALCOLOR_H
#define GUIGLOBALCOLOR_H
//----------------------------------------------------------------------------------
#include "BaseGUI.h"
//----------------------------------------------------------------------------------
class CGUIGlobalColor : public CBaseGUI
{
	//!R компонента цвета
	SETGET(GLfloat, R);

	//!G компонента цвета
	SETGET(GLfloat, G);

	//!B компонента цвета
	SETGET(GLfloat, B);

	//!A компонента цвета
	SETGET(GLfloat, A);

public:
	CGUIGlobalColor(const bool &enabled, const GLfloat &r, const GLfloat &g, const GLfloat &b, const GLfloat &a);
	virtual ~CGUIGlobalColor();

	virtual void Draw(const bool &checktrans = false);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
