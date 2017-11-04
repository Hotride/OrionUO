// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** MultiObject.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CMultiObject::CMultiObject(const ushort &graphic, const short &x, const short &y, const char &z, const uint &flags)
: CRenderStaticObject(ROT_MULTI_OBJECT, 0, graphic, 0, x, y, z), m_OnTarget(flags == 2)
{
	WISPFUN_DEBUG("c25_f1");
	m_OriginalGraphic = graphic;
	UpdateGraphicBySeason();

#if UO_DEBUG_INFO!=0
	g_MultiObjectsCount++;
#endif //UO_DEBUG_INFO!=0
}
//----------------------------------------------------------------------------------
CMultiObject::~CMultiObject()
{
#if UO_DEBUG_INFO!=0
	g_MultiObjectsCount--;
#endif //UO_DEBUG_INFO!=0
}
//----------------------------------------------------------------------------------
void CMultiObject::UpdateGraphicBySeason()
{
	WISPFUN_DEBUG("c25_f2");
	//ushort graphic = m_Graphic;

	m_Graphic = g_Orion.GetSeasonGraphic(m_OriginalGraphic);

	//if (m_Graphic != graphic)
	{
		m_Vegetation = g_Orion.IsVegetation(m_Graphic);
	}
}
//----------------------------------------------------------------------------------
void CMultiObject::Draw(const int &x, const int &y)
{
	WISPFUN_DEBUG("c25_f3");

	ushort color = m_Color;

	if (m_State)
	{
		if (m_State & CHMOF_IGNORE_IN_RENDER)
			return;

		if (m_State & CHMOF_INCORRECT_PLACE)
			color = 0x002B;

		if (m_State & CHMOF_TRANSPARENT)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(1.0f, 1.0f, 1.0f, 0.75f);

			g_Orion.DrawStaticArt(m_Graphic, color, x, y);

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glDisable(GL_BLEND);

#if UO_DEBUG_INFO!=0
			g_RenderedObjectsCountInGameWindow++;
#endif

			return;
		}
	}

#if UO_DEBUG_INFO!=0
	g_RenderedObjectsCountInGameWindow++;
#endif

	if (m_OnTarget) //Мульти на таргете
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);

		g_Orion.DrawStaticArt(m_Graphic, color, x, y);

		glDisable(GL_BLEND);
	}
	else
	{
		m_RenderGraphic = m_Graphic;

		if (g_DeveloperMode == DM_DEBUGGING && g_SelectedObject.Object == this)
			m_RenderColor = SELECT_MULTI_COLOR;
		else
			m_RenderColor = color;

		CRenderStaticObject::Draw(x, y);
	}
}
//----------------------------------------------------------------------------------
void CMultiObject::Select(const int &x, const int &y)
{
	WISPFUN_DEBUG("c25_f4");
	if (!m_OnTarget)
	{
		if (m_State)
		{
			if (m_State & CHMOF_IGNORE_IN_RENDER)
				return;
			else if ((m_State & CHMOF_TRANSPARENT) && !(m_State & CHMOF_GENERIC_INTERNAL))
			{
				if (g_CustomHouseGump == NULL || !g_CustomHouseGump->SelectedGraphic)
					return;
			}
		}

		m_RenderGraphic = m_Graphic;

		CRenderStaticObject::Select(x, y);
	}
}
//----------------------------------------------------------------------------------
