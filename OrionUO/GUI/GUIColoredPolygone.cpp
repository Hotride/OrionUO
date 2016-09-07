/***********************************************************************************
**
** GUIColoredPolygone.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GUIColoredPolygone.h"
#include "../SelectedObject.h"
//----------------------------------------------------------------------------------
CGUIColoredPolygone::CGUIColoredPolygone(const uint &serial, const ushort &color, const int &x, const int &y, const int &width, const int &height, const uint &polygoneColor)
: CGUIPolygonal(GOT_COLOREDPOLYGONE, x, y, width, height), m_DrawDot(false),
m_Focused(false)
{
	m_Serial = serial;
	UpdateColor(color, polygoneColor);
}
//----------------------------------------------------------------------------------
CGUIColoredPolygone::~CGUIColoredPolygone()
{
}
//----------------------------------------------------------------------------------
void CGUIColoredPolygone::UpdateColor(const ushort &color, const uint &polygoneColor)
{
	m_Color = color;

	m_ColorR = GetRValue(polygoneColor);
	m_ColorG = GetGValue(polygoneColor);
	m_ColorB = GetBValue(polygoneColor);
	m_ColorA = polygoneColor >> 24;

	if (!m_ColorA)
		m_ColorA = 0xFF;
}
//----------------------------------------------------------------------------------
void CGUIColoredPolygone::Draw(const bool &checktrans)
{
	glColor4ub(m_ColorR, m_ColorG, m_ColorB, m_ColorA);

	if (m_ColorA < 0xFF)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		g_GL.DrawPolygone(m_X, m_Y, m_Width, m_Height);

		glDisable(GL_BLEND);
	}
	else
		g_GL.DrawPolygone(m_X, m_Y, m_Width, m_Height);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	if (m_Focused || (m_DrawDot && g_GumpSelectedElement == this))
		g_GL.DrawPolygone(m_X + (m_Width / 2) - 1, m_Y + (m_Height / 2) - 1, 2, 2);
}
//----------------------------------------------------------------------------------
void CGUIColoredPolygone::OnMouseEnter()
{
	if (m_DrawDot && g_SelectedObject.Gump() != NULL)
		g_SelectedObject.Gump()->WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGUIColoredPolygone::OnMouseExit()
{
	if (m_DrawDot && g_LastSelectedObject.Gump() != NULL)
		g_LastSelectedObject.Gump()->WantRedraw = true;
}
//----------------------------------------------------------------------------------
