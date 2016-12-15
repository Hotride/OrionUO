/***********************************************************************************
**
** GumpSkill.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GumpSkill.h"
#include "../Managers/MouseManager.h"
#include "../SelectedObject.h"
#include "../OrionUO.h"
#include "../Skills.h"
//----------------------------------------------------------------------------------
CGumpSkill::CGumpSkill(const uint &serial, const int &x, const int &y)
: CGump(GT_SKILL, serial, x, y)
{
	m_Locker.Serial = ID_GS_LOCK_MOVING;

	CGUIResizepic *resizepic = (CGUIResizepic*)Add(new CGUIResizepic(0, 0x24EA, 0, 0, 140, 20));
	resizepic->DrawOnly = true;

	if (m_Serial < g_Skills.size())
	{
		CGUIText *text = (CGUIText*)Add(new CGUIText(0x0481, 10, 5));
		text->CreateTextureW(1, ToWString(g_Skills[m_Serial].Name), 30, 120, TS_CENTER);

		resizepic->Height = 20 + text->m_Texture.Height;
	}
}
//----------------------------------------------------------------------------------
CGumpSkill::~CGumpSkill()
{
}
//----------------------------------------------------------------------------------
void CGumpSkill::GUMP_BUTTON_EVENT_C
{
	if (serial == ID_GS_LOCK_MOVING)
		m_LockMoving = !m_LockMoving;
}
//----------------------------------------------------------------------------------
void CGumpSkill::OnLeftMouseButtonUp()
{
	CGump::OnLeftMouseButtonUp();

	if (g_SelectedObject.Serial != ID_GS_LOCK_MOVING && m_Serial < g_Skills.size() && g_Skills[m_Serial].Button)
	{
		WISP_GEOMETRY::CPoint2Di offset = g_MouseManager.LeftDroppedOffset();

		if (!offset.X && !offset.Y)
			g_Orion.UseSkill(m_Serial);
	}
}
//----------------------------------------------------------------------------------
