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
	if (m_Mode == SEM_SUNRISE)
	{
		m_Alpha -= m_Step;

		if (m_Alpha <= 0.0f)
			m_Mode = SEM_NONE;
	}
	else if (m_Mode == SEM_SUNSET)
	{
		static bool mode = false;
		static uint timer = 0;
		m_Alpha += m_Step;

		if (m_Type == SET_TO_WHITE_THEN_BLACK)
		{
			if (m_Alpha > 1.0f)
			{
				m_Alpha = 1.0f;

				if (!mode && m_ColorR > 0.0f)
				{
					if (!timer)
						timer = g_Ticks + 1000;
					else if (timer < g_Ticks)
					{
						m_ColorR -= m_Step;

						if (m_ColorR <= 0.0f)
						{
							mode = true;
							m_ColorR = 0.0f;
							m_ColorG = 0.0f;
							m_ColorB = 0.0f;
							timer = g_Ticks + 500;
						}
						else
						{
							m_ColorG -= m_Step;
							m_ColorB -= m_Step;
						}
					}
				}
				else if (timer < g_Ticks)
					m_Mode = SEM_NONE;
			}
			else
			{
				mode = false;
				timer = 0;
			}
		}
		else if (m_Alpha >= 1.0f)
		{
			m_Mode = SEM_NONE;
			return 1;
		}
	}

	return 0;
}
//---------------------------------------------------------------------------
void CScreenEffectManager::Draw()
{
	WISPFUN_DEBUG("c154_f2");
	if (m_Mode != SEM_NONE)
	{
		glColor4f(m_ColorR, m_ColorG, m_ColorB, m_Alpha);

		if (m_Type == SET_TO_WHITE_THEN_BLACK && m_Alpha >= 1.0f)
			g_GL.DrawPolygone(0, 0, g_OrionWindow.Size.Width, g_OrionWindow.Size.Height);
		else
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			g_GL.DrawPolygone(0, 0, g_OrionWindow.Size.Width, g_OrionWindow.Size.Height);

			glDisable(GL_BLEND);
		}

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
}//---------------------------------------------------------------------------
bool CScreenEffectManager::Use(const SCREEN_EFFECT_MODE &mode, const SCREEN_EFFECT_TYPE &type, const bool &ignoreEnabled)
{
	WISPFUN_DEBUG("c154_f3");
	if (m_Enabled || ignoreEnabled)
	{
		m_Mode = mode;
		m_Type = type;

		if (mode == SEM_SUNSET)
			m_Alpha = 0.0f;
		else //if (mode == SEM_SUNRISE)
			m_Alpha = 1.0f;

		static const float colorTable[5] = { 0.0f, 1.0f, 1.0f, 1.0f, 0.0f };
		static const float speedTable[5] = { 0.02f, 0.02f, 0.08f, 0.02f, 0.15f };

		m_ColorR = m_ColorG = m_ColorB = colorTable[type];
		m_Step = speedTable[type];
	}
	else
		m_Mode = SEM_NONE;

	return m_Enabled;
}
//---------------------------------------------------------------------------
bool CScreenEffectManager::UseSunrise()
{
	WISPFUN_DEBUG("c154_f4");
	bool result = Use(SEM_SUNRISE);
	m_Step = 0.05f;
	return result;
}
//---------------------------------------------------------------------------
bool CScreenEffectManager::UseSunset()
{
	WISPFUN_DEBUG("c154_f5");
	bool result = Use(SEM_SUNSET);
	m_Step = 0.05f;
	return result;
}
//---------------------------------------------------------------------------