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
class CGUIGlobalColor : public CBaseGUI
{
	//!R компонента цвета
	SETGET(GLfloat, R, 0.0f);

	//!G компонента цвета
	SETGET(GLfloat, G, 0.0f);

	//!B компонента цвета
	SETGET(GLfloat, B, 0.0f);

	//!A компонента цвета
	SETGET(GLfloat, A, 0.0f);

public:
	CGUIGlobalColor(const bool &enabled, const GLfloat &r, const GLfloat &g, const GLfloat &b, const GLfloat &a);
	virtual ~CGUIGlobalColor();

	virtual void Draw(const bool &checktrans = false);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
