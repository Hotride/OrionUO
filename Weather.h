/****************************************************************************
**
** Weather.h
**
** Copyright (C) September 2015 Hotride
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
#ifndef WeatherH
#define WeatherH
//---------------------------------------------------------------------------
class TWeatherEffect
{
private:
	float m_X;
	float m_Y;

	float m_SpeedX;
	float m_SpeedY;

	int m_ID;
	float m_ScaleRatio;

	float m_SpeedAngle;
	float m_SpeedMagnitude;

public:
	TWeatherEffect *m_Next;
	TWeatherEffect *m_Prev;

	TWeatherEffect();

	~TWeatherEffect() {}

	int GetID() const {return m_ID;}
	float GetScaleRatio() const {return m_ScaleRatio;}
	
	SETGETEX(float, SpeedX)
	SETGETEX(float, SpeedY)
	SETGETEX(float, X)
	SETGETEX(float, Y)
	SETGETEX(float, SpeedAngle)
	SETGETEX(float, SpeedMagnitude)
};
//---------------------------------------------------------------------------
class TWeather
{
private:
	BYTE m_Type;
	BYTE m_Count;
	BYTE m_CurrentCount;
	BYTE m_Temperature;

	char m_Wind;
	DWORD m_WindTimer;

	DWORD m_Timer;
	DWORD m_LastTick; // время в мс предыдущего тика обработки анимации погоды

	float m_SimulationRatio; // скорость общей симуляции зависит от этого значения. Больше - медленней.

public:
	TWeather() : m_LastTick(0), m_SimulationRatio(37), m_Items(NULL) {}
	~TWeather() {}

	TWeatherEffect *m_Items;
	
	SETGET(BYTE, Type)
	SETGETEX(BYTE, Count)
	SETGETEX(BYTE, CurrentCount)
	SETGETEX(BYTE, Temperature)
	
	SETGET(DWORD, Timer)
	
	void Reset();

	void Generate();
	
	void Draw(int &drawX, int &drawY);
};
//---------------------------------------------------------------------------
extern TWeather Weather;
//---------------------------------------------------------------------------
#endif