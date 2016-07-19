/****************************************************************************
**
** Weather.cpp
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
#include "stdafx.h"

TWeather Weather;
/// <summary>
/// колебания по синусоиде с частотой freq и амплитудой -range до +range
/// </summary>
/// <param name="freq">Частота колебаний в Гц (в сек).</param>
/// <param name="range">Диапазон колебаний (от -range до +range).</param>
/// <param name="current_tick">Текущий тик, мс.</param>
/// <returns>Значение от -range до +range для данного тика</returns>
float SinOscillate(float freq, int range, DWORD current_tick)
{
	//float anglef = int((current_tick / (1000.0f / 360.0f)) * freq) % 360;

	float anglef = (float)(int((current_tick / 2.7777f) * freq) % 360);
	return sinf(deg2radf(anglef)) * range;
}
//---------------------------------------------------------------------------
TWeatherEffect::TWeatherEffect()
:m_Next(NULL), m_Prev(NULL), m_X(0), m_Y(0), m_SpeedX(0), m_SpeedY(0)
{
	m_ID = RandomIntMinMax(0, 2000);

	m_ScaleRatio = float(m_ID % 20) / 13.0f;
}
//---------------------------------------------------------------------------
void TWeather::Reset()
{
	m_Type = 0;
	m_Count = 0;
	m_CurrentCount = 0;
	m_Temperature = 0;

	m_Wind = 0;
	m_WindTimer = 0;

	m_Timer = 0;
	
	TWeatherEffect *effect = m_Items;

	while (effect != NULL)
	{
		TWeatherEffect *next = effect->m_Next;
		delete effect;
		effect = next;
	}

	m_Items = NULL;
}
//---------------------------------------------------------------------------
void TWeather::Generate()
{
	m_LastTick = g_Ticks;

	if (m_Type == 0xFF || m_Type == 0xFE)
		return;

	int drawX = g_GameWindowPosX;
	int drawY = g_GameWindowPosY;

	if (m_Count > 70)
		m_Count = 70;

	m_WindTimer = 0;
	
	while (m_CurrentCount < m_Count)
	{
		TWeatherEffect *we = new TWeatherEffect();

		we->X = (float)(drawX + RandomInt(g_GameWindowWidth));
		we->Y = (float)(drawY + RandomInt(g_GameWindowHeight));

		we->m_Prev = NULL;
		we->m_Next = m_Items;

		if (m_Items != NULL)
			m_Items->m_Prev = we;

		m_Items = we;

		m_CurrentCount++;
	}
}
//---------------------------------------------------------------------------
void TWeather::Draw(int &drawX, int &drawY)
{    
	DWORD currentTick = g_Ticks;
    
	bool removeEffects = false;

	if (m_Timer < currentTick)
	{
		//if (m_CurrentCount) Reset();
		if (!m_CurrentCount)
			return;

		removeEffects = true;
	}
	else if (m_Type == 0xFF || m_Type == 0xFE)
		return;

	DWORD passed = currentTick - m_LastTick;

	if (passed > 7000) // если времени слишком много прошло со старой симуляции
	{
		m_LastTick = currentTick;
		passed = 25;
	}

	bool windChanged = false;

	if (m_WindTimer < currentTick)
	{
		if (!m_WindTimer)
			windChanged = true; //Для установки стартовых значений снежинок

		m_WindTimer = currentTick + ( RandomIntMinMax(7, 13) * 1000 );

		char lastWind = m_Wind;

		m_Wind = RandomInt(4);

		if (RandomInt(2))
			m_Wind *= (-1);
		
		if (m_Wind < 0 && lastWind > 0)
			m_Wind = 0;
		else if (m_Wind > 0 && lastWind < 0)
			m_Wind = 0;

		if (lastWind != m_Wind)
			windChanged = true;
	}
	
	TWeatherEffect *effect = m_Items;

	switch (m_Type)
	{
		case WT_RAIN:
		case WT_FIERCE_STORM:
		{
			glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

			break;
		}
		case WT_SNOW:
		case WT_STORM:
		{
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			break;
		}
		default:
			break;
	}

	while (effect != NULL)
	{
		if ((effect->X < drawX || effect->X > (drawX + g_GameWindowWidth)) || (effect->Y < drawY || effect->Y > (drawY + g_GameWindowHeight)))
		{
			if (removeEffects)
			{
				TWeatherEffect *enext = effect->m_Next;
				
				if (effect->m_Prev == NULL)
				{
					m_Items = enext;
					if (m_Items != NULL) m_Items->m_Prev = NULL;
					delete effect;
				}
				else
				{
					effect->m_Prev->m_Next = enext;
					if (enext != NULL) enext->m_Prev = effect->m_Prev;
					delete effect;
				}

				if (m_CurrentCount > 0) m_CurrentCount--;
				else m_CurrentCount = 0;

				effect = enext;
				continue;
			}
			else
			{
				effect->X = (float)(drawX + RandomInt(g_GameWindowWidth));
				effect->Y = (float)(drawY + RandomInt(g_GameWindowHeight));
			}
		}        

		switch (m_Type)
		{
			case WT_RAIN:
			{
				float scaleRatio = effect->GetScaleRatio();
				effect->SpeedX = -4.5f - scaleRatio;
				effect->SpeedY = 5.0f + scaleRatio;
				break;
			}
			case WT_FIERCE_STORM:
			{
				effect->SpeedX = m_Wind;
				effect->SpeedY = 6.0f;
				break;
			}
			case WT_SNOW:
			case WT_STORM:
			{
				if (m_Type == WT_SNOW)
				{
					effect->SpeedX = m_Wind;
					effect->SpeedY = 1.0f;
				}
				else
				{
					effect->SpeedX = (m_Wind * 1.5f);
					effect->SpeedY = 1.5f;
				}

				if (windChanged)
				{
					// вычисление угла скорости в градусах
					effect->SetSpeedAngle(rad2degf(std::atan2f(effect->SpeedX, effect->SpeedY)));
					// числинное значение скорости
					effect->SetSpeedMagnitude(sqrtf(powf(effect->SpeedX, 2) + powf(effect->SpeedY, 2)));
				}

				float speed_angle = effect->GetSpeedAngle();
				float speed_magnitude = effect->GetSpeedMagnitude();

				// коэффицент скейлирования (используеться для рандомизации скорости снега)
				speed_magnitude += effect->GetScaleRatio();

				// тут движение УГЛА силы по синусоиде, ID() снежинки добавляется для смещения фазы
				// хотя там можно заюзать любое постоянное число, например, порядковый номер снежинки
				speed_angle += SinOscillate(0.4f, 20, currentTick + effect->GetID());

				// обратная проекция на оси X, Y из угла и (скалярного) значения
				effect->SpeedX = speed_magnitude * sinf(deg2radf(speed_angle));
				effect->SpeedY = speed_magnitude * cosf(deg2radf(speed_angle));

				break;
			}
			default:
				break;
		}

		float speedOffset = (float)passed / m_SimulationRatio;

		switch (m_Type)
		{
			case WT_RAIN:
			case WT_FIERCE_STORM:
			{
				int oldX = (int)effect->X;
				int oldY = (int)effect->Y;

				float ofsx = (effect->SpeedX * speedOffset);
				float ofsy = (effect->SpeedY * speedOffset);

				effect->X += ofsx;
				effect->Y += ofsy;

				const float maxOffsetXY = 5.0f;

				if (ofsx >= maxOffsetXY)
					oldX = (int)(effect->X - maxOffsetXY);
				else if (ofsx <= -maxOffsetXY)
					oldX = (int)(effect->X + maxOffsetXY);

				if (ofsy >= maxOffsetXY)
					oldY = (int)(effect->Y - maxOffsetXY);
				else if (ofsy <= -maxOffsetXY)
					oldY = (int)(effect->Y + maxOffsetXY);

				g_GL.DrawLine(drawX + oldX, drawY + oldY, drawX + (int)effect->X, drawY + (int)effect->Y);

				break;
			}
			case WT_SNOW:
			case WT_STORM:
			{
				effect->X += (effect->SpeedX * speedOffset);
				effect->Y += (effect->SpeedY * speedOffset);

				g_GL.DrawPolygone(drawX + (int)effect->X, drawY + (int)effect->Y, 2, 2);
				break;
			}
			default:
				break;
		}

		effect = effect->m_Next;
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	m_LastTick = currentTick;
}
//---------------------------------------------------------------------------