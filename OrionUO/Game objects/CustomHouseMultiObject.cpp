/***********************************************************************************
**
** CustomHouseMultiObject.h
**
** Copyright (C) September 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CCustomHouseMultiObject::CCustomHouseMultiObject(const ushort &graphic, const ushort &color, const short &x, const short &y, const char &z, const uint &flags)
: CMultiObject(graphic, x, y, z, flags)
{
	WISPFUN_DEBUG("");
	m_Color = color;
}
//----------------------------------------------------------------------------------
CCustomHouseMultiObject::~CCustomHouseMultiObject()
{
}
//----------------------------------------------------------------------------------
void CCustomHouseMultiObject::Draw(const int &x, const int &y)
{
	WISPFUN_DEBUG("");
#if UO_DEBUG_INFO!=0
	g_RenderedObjectsCountInGameWindow++;
#endif

	if (m_State & CHMOF_TRANSPARENT)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f, 1.0f, 1.0f, 0.75f);

		g_Orion.DrawStaticArt(m_Graphic, m_Color, x, y);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glDisable(GL_BLEND);
	}
	else
	{
		m_RenderGraphic = m_Graphic;

		if (g_DeveloperMode == DM_DEBUGGING && g_SelectedObject.Object == this)
			m_RenderColor = SELECT_MULTI_COLOR;
		else
			m_RenderColor = m_Color;

		CRenderStaticObject::Draw(x, y);
	}
}
//----------------------------------------------------------------------------------
