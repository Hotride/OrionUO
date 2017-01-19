/***********************************************************************************
**
** WeatherEffect.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef WEATHEREFFECT_H
#define WEATHEREFFECT_H
//----------------------------------------------------------------------------------
#include "Globals.h"
//----------------------------------------------------------------------------------
//Класс эффекта погоды
class CWeatherEffect
{
	SETGET(float, SpeedX);
	SETGET(float, SpeedY);
	SETGET(float, X);
	SETGET(float, Y);
	SETGET(uint, ID);
	SETGET(float, ScaleRatio);
	SETGET(float, SpeedAngle);
	SETGET(float, SpeedMagnitude);

public:
	CWeatherEffect();
	virtual ~CWeatherEffect() {}
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
