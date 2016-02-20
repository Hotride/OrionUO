/****************************************************************************
**
** SmoothMonitor.h
**
** Copyright (C) February 2016 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//---------------------------------------------------------------------------
#ifndef SmoothMonitorH
#define SmoothMonitorH
//---------------------------------------------------------------------------
//Класс для работы с плавным переключением экрана
class TSmoothMonitor
{
protected:
	//Тип перехода
	SMOOTH_MONITOR_TYPE m_Type;

	//Использовать плавное переключение экрана
	bool m_Enabled;

	//Цвет переключения
	GLfloat m_Color;

	//Шаг цвета переключения
	GLfloat m_Step;

	//Масштабирование переключения
	int m_Scale;

public:
	TSmoothMonitor();
	virtual ~TSmoothMonitor();

	SETGET(SMOOTH_MONITOR_TYPE, Type);
	SETGET(bool, Enabled);
	SETGET(GLfloat, Color);
	SETGET(GLfloat, Step);
	SETGET(int, Scale);

	int Process();
	void Draw();
	bool UseSunrise();
	bool UseSunset();
};
//---------------------------------------------------------------------------
extern TSmoothMonitor SmoothMonitor;
//---------------------------------------------------------------------------
#endif