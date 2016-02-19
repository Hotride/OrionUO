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
//Класс эффекта погоды
class TWeatherEffect
{
private:
	//Координаты эффекта
	float m_X;
	float m_Y;

	//Скорость эффекта
	float m_SpeedX;
	float m_SpeedY;

	//Индекс эффекта
	int m_ID;

	float m_ScaleRatio;

	float m_SpeedAngle;

	float m_SpeedMagnitude;

public:
	//Ссылки на следующий и предыдущий эффекты
	TWeatherEffect *m_Next;
	TWeatherEffect *m_Prev;

	TWeatherEffect();

	~TWeatherEffect() {}

	int GetID() const {return m_ID;}
	float GetScaleRatio() const {return m_ScaleRatio;}
	
	SETGETEX(float, SpeedX);
	SETGETEX(float, SpeedY);
	SETGETEX(float, X);
	SETGETEX(float, Y);
	SETGETEX(float, SpeedAngle);
	SETGETEX(float, SpeedMagnitude);
};
//---------------------------------------------------------------------------
//Класс для работы с погодой
class TWeather
{
private:
	//Тип вогоды
	BYTE m_Type;

	//Максимальное количество эффектов на экране
	BYTE m_Count;

	//Текущее количество эффектов на экране
	BYTE m_CurrentCount;

	//Температура
	BYTE m_Temperature;

	//Направление ветра
	char m_Wind;

	//Таймер смены ветра
	DWORD m_WindTimer;

	//Время отрисовки
	DWORD m_Timer;

	//Время последней смены анимации погоды
	DWORD m_LastTick;

	float m_SimulationRatio; // скорость общей симуляции зависит от этого значения. Больше - медленней.

public:
	TWeather() : m_LastTick(0), m_SimulationRatio(37), m_Items(NULL) {}
	~TWeather() {}

	//Погодные эффекты
	TWeatherEffect *m_Items;
	
	SETGET(BYTE, Type);
	SETGETEX(BYTE, Count);
	SETGETEX(BYTE, CurrentCount);
	SETGETEX(BYTE, Temperature);
	
	SETGET(DWORD, Timer);

	//Выключить погодные эффекты
	void Reset();

	//Сгенерировать эффекты
	void Generate();

	//Отрисовать погодные эффекты
	void Draw(int &drawX, int &drawY);
};
//---------------------------------------------------------------------------
extern TWeather Weather;
//---------------------------------------------------------------------------
#endif