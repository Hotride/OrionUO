// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** Weather.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CWeather g_Weather;
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
//----------------------------------------------------------------------------------
CWeather::CWeather()
{
}
//---------------------------------------------------------------------------
void CWeather::Reset()
{
	WISPFUN_DEBUG("c214_f1");
	Type = 0;
	Count = 0;
	CurrentCount = 0;
	Temperature = 0;

	Wind = 0;
	WindTimer = 0;

	Timer = 0;
	
	m_Effects.clear();
}
//---------------------------------------------------------------------------
void CWeather::Generate()
{
	WISPFUN_DEBUG("c214_f2");
	LastTick = g_Ticks;

	if (Type == 0xFF || Type == 0xFE)
		return;

	int drawX = g_ConfigManager.GameWindowX;
	int drawY = g_ConfigManager.GameWindowY;

	if (Count > 70)
		Count = 70;

	WindTimer = 0;
	
	while (CurrentCount < Count)
	{
		CWeatherEffect effect;

		effect.X = (float)(drawX + RandomInt(g_ConfigManager.GameWindowWidth));
		effect.Y = (float)(drawY + RandomInt(g_ConfigManager.GameWindowHeight));

		m_Effects.push_back(effect);

		CurrentCount++;
	}
}
//---------------------------------------------------------------------------
void CWeather::Draw(const int &x, const int &y)
{    
	WISPFUN_DEBUG("c214_f3");
	bool removeEffects = false;

	if (Timer < g_Ticks)
	{
		//if (CurrentCount) Reset();
		if (!CurrentCount)
			return;

		removeEffects = true;
	}
	else if (Type == 0xFF || Type == 0xFE)
		return;

	uint passed = g_Ticks - LastTick;

	if (passed > 7000) // если времени слишком много прошло со старой симуляции
	{
		LastTick = g_Ticks;
		passed = 25;
	}

	bool windChanged = false;

	if (WindTimer < g_Ticks)
	{
		if (!WindTimer)
			windChanged = true; //Для установки стартовых значений снежинок

		WindTimer = g_Ticks + (RandomIntMinMax(7, 13) * 1000);

		char lastWind = Wind;

		Wind = RandomInt(4);

		if (RandomInt(2))
			Wind *= (-1);
		
		if (Wind < 0 && lastWind > 0)
			Wind = 0;
		else if (Wind > 0 && lastWind < 0)
			Wind = 0;

		if (lastWind != Wind)
			windChanged = true;
	}
	
	switch (Type)
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

	for (deque<CWeatherEffect>::iterator effect = m_Effects.begin(); effect != m_Effects.end();)
	{
		if ((effect->X < x || effect->X > (x + g_ConfigManager.GameWindowWidth)) || (effect->Y < y || effect->Y > (y + g_ConfigManager.GameWindowHeight)))
		{
			if (removeEffects)
			{
				effect = m_Effects.erase(effect);

				if (CurrentCount > 0)
					CurrentCount--;
				else
					CurrentCount = 0;

				continue;
			}
			else
			{
				effect->X = (float)(x + RandomInt(g_ConfigManager.GameWindowWidth));
				effect->Y = (float)(y + RandomInt(g_ConfigManager.GameWindowHeight));
			}
		}        

		switch (Type)
		{
			case WT_RAIN:
			{
				float scaleRatio = effect->ScaleRatio;
				effect->SpeedX = -4.5f - scaleRatio;
				effect->SpeedY = 5.0f + scaleRatio;
				break;
			}
			case WT_FIERCE_STORM:
			{
				effect->SpeedX = Wind;
				effect->SpeedY = 6.0f;
				break;
			}
			case WT_SNOW:
			case WT_STORM:
			{
				if (Type == WT_SNOW)
				{
					effect->SpeedX = Wind;
					effect->SpeedY = 1.0f;
				}
				else
				{
					effect->SpeedX = (Wind * 1.5f);
					effect->SpeedY = 1.5f;
				}

				if (windChanged)
				{
					// вычисление угла скорости в градусах
					effect->SpeedAngle = rad2degf(std::atan2f(effect->SpeedX, effect->SpeedY));
					// числинное значение скорости
					effect->SpeedMagnitude = sqrtf(powf(effect->SpeedX, 2) + powf(effect->SpeedY, 2));
				}

				float speed_angle = effect->SpeedAngle;
				float speed_magnitude = effect->SpeedMagnitude;

				// коэффицент скейлирования (используеться для рандомизации скорости снега)
				speed_magnitude += effect->ScaleRatio;

				// тут движение УГЛА силы по синусоиде, ID() снежинки добавляется для смещения фазы
				// хотя там можно заюзать любое постоянное число, например, порядковый номер снежинки
				speed_angle += SinOscillate(0.4f, 20, g_Ticks + effect->ID);

				// обратная проекция на оси X, Y из угла и (скалярного) значения
				effect->SpeedX = speed_magnitude * sinf(deg2radf(speed_angle));
				effect->SpeedY = speed_magnitude * cosf(deg2radf(speed_angle));

				break;
			}
			default:
				break;
		}

		float speedOffset = passed / SimulationRatio;

		switch (Type)
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

				g_GL.DrawLine(x + oldX, y + oldY, x + (int)effect->X, y + (int)effect->Y);

				break;
			}
			case WT_SNOW:
			case WT_STORM:
			{
				effect->X += (effect->SpeedX * speedOffset);
				effect->Y += (effect->SpeedY * speedOffset);

				g_GL.DrawPolygone(x + (int)effect->X, y + (int)effect->Y, 2, 2);

				break;
			}
			default:
				break;
		}

		++effect;
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	LastTick = g_Ticks;
}
//---------------------------------------------------------------------------