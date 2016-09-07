/***********************************************************************************
**
** SmoothMonitor.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "SmoothMonitor.h"
#include "GLEngine/GLEngine.h"
#include "OrionWindow.h"

CSmoothMonitor g_SmoothMonitor;
//---------------------------------------------------------------------------
CSmoothMonitor::CSmoothMonitor()
: m_Type(SMT_NONE), m_Enabled(false), m_Color(0.0f), m_Step(0.03f), m_Scale(15)
{
}
//---------------------------------------------------------------------------
CSmoothMonitor::~CSmoothMonitor()
{
}
//---------------------------------------------------------------------------
int CSmoothMonitor::Process()
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
void CSmoothMonitor::Draw()
{
	if (m_Color != 1.0f)
	{
		glColor4f(m_Color, m_Color, m_Color, 1.0f);

		glEnable(GL_BLEND);
		glBlendFunc(GL_ZERO, GL_SRC_COLOR);

		g_GL.DrawPolygone(0, 0, g_OrionWindow.Size.Width, g_OrionWindow.Size.Height);

		glDisable(GL_BLEND);
	}
}
//---------------------------------------------------------------------------
bool CSmoothMonitor::UseSunrise()
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
bool CSmoothMonitor::UseSunset()
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