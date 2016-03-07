/****************************************************************************
**
** SmoothMonitor.h
**
** Copyright (C) February 2016 Hotride
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

TSmoothMonitor SmoothMonitor;
//---------------------------------------------------------------------------
TSmoothMonitor::TSmoothMonitor()
: m_Type(SMT_NONE), m_Enabled(false), m_Color(0.0f), m_Step(0.03f), m_Scale(15)
{
}
//---------------------------------------------------------------------------
TSmoothMonitor::~TSmoothMonitor()
{
}
//---------------------------------------------------------------------------
int TSmoothMonitor::Process()
{
	if (m_Type == SMT_SUNRISE)
	{
		m_Color += m_Step;

		if (m_Color >= 1.0f)
		{
			m_Color = 1.0f;
			m_Type = SMT_NONE;
		}
	}
	else if (m_Type == SMT_SUNSET)
	{
		m_Color -= m_Step;

		if (m_Color <= 0.0f)
		{
			m_Color = 1.0f;
			m_Type = SMT_NONE;
			glColor3f(m_Color, m_Color, m_Color);

			return 1;
		}
	}
	else
		m_Color = 1.0f;

	//glColor3f(m_Color, m_Color, m_Color);

	return 0;
}
//---------------------------------------------------------------------------
void TSmoothMonitor::Draw()
{
	if (m_Color != 1.0f)
	{
		glColor4f(m_Color, m_Color, m_Color, 1.0f);

		glEnable(GL_BLEND);
		glBlendFunc(GL_ZERO, GL_SRC_COLOR);

		g_GL.DrawPolygone(0.0f, 0.0f, (GLfloat)g_ClientWidth, (GLfloat)g_ClientHeight);

		glDisable(GL_BLEND);
	}
}
//---------------------------------------------------------------------------
bool TSmoothMonitor::UseSunrise()
{
	if (m_Enabled)
	{
		m_Type = SMT_SUNRISE;
		m_Color = 0.0f;
		m_Step = (GLfloat)m_Scale * 0.01f;
	}
	else
		m_Type = SMT_NONE;

	return m_Enabled;
}
//---------------------------------------------------------------------------
bool TSmoothMonitor::UseSunset()
{
	if (m_Enabled)
	{
		m_Type = SMT_SUNSET;
		m_Color = 1.0f;
	}
	else
		m_Type = SMT_NONE;

	return m_Enabled;
}
//---------------------------------------------------------------------------