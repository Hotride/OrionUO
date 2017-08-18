/***********************************************************************************
**
** GUILine.h
**
** Компонента для отображения линии
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUILINE_H
#define GUILINE_H
//----------------------------------------------------------------------------------
class CGUILine : public CBaseGUI
{
	//!Целевая координата по оси X
	SETGET(int, TargetX, 0);

	//!Целевая координата по оси Y
	SETGET(int, TargetY, 0);

	//!R-компонента цвета
	SETGET(uchar, ColorR, 0);

	//!G-компонента цвета
	SETGET(uchar, ColorG, 0);

	//!B-компонента цвета
	SETGET(uchar, ColorB, 0);

	//!Значение альфа-канала
	SETGET(uchar, ColorA, 0);

public:
	CGUILine(const int &x, const int &y, const int &targetX, const int &targetY, const uint &polygoneColor);
	virtual ~CGUILine();

	virtual void Draw(const bool &checktrans = false);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
