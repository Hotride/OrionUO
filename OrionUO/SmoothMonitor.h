/***********************************************************************************
**
** SmoothMonitor.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef SMOOTHMONITOR_H
#define SMOOTHMONITOR_H
//----------------------------------------------------------------------------------
#include "Globals.h"
#include "EnumList.h"
//----------------------------------------------------------------------------------
//Класс для работы с плавным переключением экрана
class CSmoothMonitor
{
	SETGET(SMOOTH_MONITOR_TYPE, Type);
	SETGET(bool, Enabled);
	SETGET(GLfloat, Color);
	SETGET(GLfloat, Step);
	SETGET(int, Scale);

public:
	CSmoothMonitor();
	virtual ~CSmoothMonitor();

	int Process();
	void Draw();
	bool UseSunrise();
	bool UseSunset();
};
//----------------------------------------------------------------------------------
extern CSmoothMonitor g_SmoothMonitor;
//----------------------------------------------------------------------------------
#endif
