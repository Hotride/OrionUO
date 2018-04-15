// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GumpSkill.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpSkill::CGumpSkill(int serial, int x, int y)
: CGump(GT_SKILL, serial, x, y)
{
	WISPFUN_DEBUG("c124_f1");
	m_Locker.Serial = ID_GS_LOCK_MOVING;

	CGUIResizepic *resizepic = (CGUIResizepic*)Add(new CGUIResizepic(0, 0x24EA, 0, 0, 140, 20));
	resizepic->DrawOnly = true;

	if (Serial < (uint)g_SkillsManager.Count)
	{
		CGUIText *text = (CGUIText*)Add(new CGUIText(0x0481, 10, 5));

		CSkill *skill = g_SkillsManager.Get(Serial);

		if (skill != NULL)
			text->CreateTextureW(1, ToWString(skill->Name), 30, 120, TS_CENTER);

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
	WISPFUN_DEBUG("c124_f2");
	if (serial == ID_GS_LOCK_MOVING)
		LockMoving = !LockMoving;
}
//----------------------------------------------------------------------------------
void CGumpSkill::OnLeftMouseButtonUp()
{
	WISPFUN_DEBUG("c124_f3");
	CGump::OnLeftMouseButtonUp();

	if (g_SelectedObject.Serial != ID_GS_LOCK_MOVING && Serial < (uint)g_SkillsManager.Count)
	{
		CSkill *skill = g_SkillsManager.Get(Serial);

		if (skill != NULL && skill->Button)
		{
			WISP_GEOMETRY::CPoint2Di offset = g_MouseManager.LeftDroppedOffset();

			if (!offset.X && !offset.Y)
				g_Orion.UseSkill(Serial);
		}
	}
}
//----------------------------------------------------------------------------------
