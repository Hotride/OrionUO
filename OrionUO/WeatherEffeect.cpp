// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** WeatherEffect.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CWeatherEffect::CWeatherEffect()
{
	m_ID = RandomInt(2000);

	m_ScaleRatio = (float)(m_ID % 20) / 13.0f;
}
//----------------------------------------------------------------------------------
