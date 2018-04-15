// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** ScreenEffectManager.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"

CScreenEffectManager g_ScreenEffectManager;
//---------------------------------------------------------------------------
CScreenEffectManager::CScreenEffectManager()
{
}
//---------------------------------------------------------------------------
CScreenEffectManager::~CScreenEffectManager()
{
}
//---------------------------------------------------------------------------
int CScreenEffectManager::Process()
{
	WISPFUN_DEBUG("c154_f1");
	if (Mode == SEM_SUNRISE)
	{
		Alpha -= Step;

		if (Alpha <= 0.0f)
			Mode = SEM_NONE;
	}
	else if (Mode == SEM_SUNSET)
	{
		static bool mode = false;
		static uint timer = 0;
		Alpha += Step;

		if (Type == SET_TO_WHITE_THEN_BLACK)
		{
			if (Alpha > 1.0f)
			{
				Alpha = 1.0f;

				if (!mode && ColorR > 0.0f)
				{
					if (!timer)
						timer = g_Ticks + 1000;
					else if (timer < g_Ticks)
					{
						ColorR -= Step;

						if (ColorR <= 0.0f)
						{
							mode = true;
							ColorR = 0.0f;
							ColorG = 0.0f;
							ColorB = 0.0f;
							timer = g_Ticks + 500;
						}
						else
						{
							ColorG -= Step;
							ColorB -= Step;
						}
					}
				}
				else if (timer < g_Ticks)
					Mode = SEM_NONE;
			}
			else
			{
				mode = false;
				timer = 0;
			}
		}
		else if (Alpha >= 1.0f)
		{
			Mode = SEM_NONE;
			return 1;
		}
	}

	return 0;
}
//---------------------------------------------------------------------------
void CScreenEffectManager::Draw()
{
	WISPFUN_DEBUG("c154_f2");
	if (Mode != SEM_NONE)
	{
		glColor4f(ColorR, ColorG, ColorB, Alpha);

		if (Type == SET_TO_WHITE_THEN_BLACK && Alpha >= 1.0f)
			g_GL.DrawPolygone(0, 0, g_OrionWindow.GetSize().Width, g_OrionWindow.GetSize().Height);
		else
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			g_GL.DrawPolygone(0, 0, g_OrionWindow.GetSize().Width, g_OrionWindow.GetSize().Height);

			glDisable(GL_BLEND);
		}

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
}//---------------------------------------------------------------------------
bool CScreenEffectManager::Use(const SCREEN_EFFECT_MODE &mode, const SCREEN_EFFECT_TYPE &type, const bool &ignoreEnabled)
{
	WISPFUN_DEBUG("c154_f3");
	if (Enabled || ignoreEnabled)
	{
		Mode = mode;
		Type = type;

		if (mode == SEM_SUNSET)
			Alpha = 0.0f;
		else //if (mode == SEM_SUNRISE)
			Alpha = 1.0f;

		static const float colorTable[5] = { 0.0f, 1.0f, 1.0f, 1.0f, 0.0f };
		static const float speedTable[5] = { 0.02f, 0.02f, 0.08f, 0.02f, 0.15f };

		ColorR = ColorG = ColorB = colorTable[type];
		Step = speedTable[type];
	}
	else
		Mode = SEM_NONE;

	return Enabled;
}
//---------------------------------------------------------------------------
bool CScreenEffectManager::UseSunrise()
{
	WISPFUN_DEBUG("c154_f4");
	bool result = Use(SEM_SUNRISE);
	Step = 0.05f;
	return result;
}
//---------------------------------------------------------------------------
bool CScreenEffectManager::UseSunset()
{
	WISPFUN_DEBUG("c154_f5");
	bool result = Use(SEM_SUNSET);
	Step = 0.05f;
	return result;
}
//---------------------------------------------------------------------------