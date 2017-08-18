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
//Класс эффекта погоды
class CWeatherEffect
{
	SETGET(float, SpeedX, 0.0f);
	SETGET(float, SpeedY, 0.0f);
	SETGET(float, X, 0.0f);
	SETGET(float, Y, 0.0f);
	SETGET(uint, ID, 0);
	SETGET(float, ScaleRatio, 0.0f);
	SETGET(float, SpeedAngle, 0.0f);
	SETGET(float, SpeedMagnitude, 0.0f);

public:
	CWeatherEffect();
	virtual ~CWeatherEffect() {}
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
