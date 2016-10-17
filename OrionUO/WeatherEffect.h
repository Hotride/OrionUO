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
	SETGETEX(float, SpeedX);
	SETGETEX(float, SpeedY);
	SETGETEX(float, X);
	SETGETEX(float, Y);
	SETGETEX(uint, ID);
	SETGETEX(float, ScaleRatio);
	SETGETEX(float, SpeedAngle);
	SETGETEX(float, SpeedMagnitude);

public:
	CWeatherEffect();
	virtual ~CWeatherEffect() {}
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
