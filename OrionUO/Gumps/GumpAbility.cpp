// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GumpAbility.cpp
**
** Copyright (C) December 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpAbility::CGumpAbility(const uint &serial, const int &x, const int &y)
: CGump(GT_ABILITY, serial, x, y)
{
	WISPFUN_DEBUG("c85_f1");
	int index = (Serial != 0 ? 1 : 0);
	uchar &ability = g_Ability[index];

	Graphic = 0x5200 + (ability & 0x7F) - 1;

	m_Locker.Serial = ID_GS_LOCK_MOVING;

	m_Colorizer = (CGUIGlobalColor*)Add(new CGUIGlobalColor((ability & 0x80), 1.0f, 0.25f, 0.5f, 1.0f));

	m_Body = (CGUIGumppic*)Add(new CGUIGumppic(Graphic, 0, 0));
}
//----------------------------------------------------------------------------------
CGumpAbility::~CGumpAbility()
{
}
//----------------------------------------------------------------------------------
void CGumpAbility::InitToolTip()
{
	WISPFUN_DEBUG("c85_f2");
	int index = (Serial != 0 ? 1 : 0);

	g_ToolTip.Set(g_ClilocManager.Cliloc(g_Language)->GetW(1028838 + (g_Ability[index] & 0x7F) - 1, true), 80);
}
//----------------------------------------------------------------------------------
void CGumpAbility::UpdateContent()
{
	WISPFUN_DEBUG("c85_f3");
	if (m_Colorizer != NULL && m_Body != NULL)
	{
		int index = (Serial != 0 ? 1 : 0);
		uchar &ability = g_Ability[index];

		m_Colorizer->Enabled = (ability & 0x80);

		m_Body->Graphic = 0x5200 + (ability & 0x7F) - 1;
	}
}
//----------------------------------------------------------------------------------
void CGumpAbility::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c85_f4");
	if (serial == ID_GS_LOCK_MOVING)
		LockMoving = !LockMoving;
}
//----------------------------------------------------------------------------------
void CGumpAbility::OnAbilityUse(const int &index)
{
	WISPFUN_DEBUG("c85_f5");
	uchar &ability = g_Ability[index];

	if (!(ability & 0x80))
	{
		IFOR(i, 0, 2)
			g_Ability[i] &= 0x7F;

		CPacketUseCombatAbility(ability).Send();
	}
	else
		CPacketUseCombatAbility(0).Send();

	ability ^= 0x80;

	g_GumpManager.UpdateContent(0, 0, GT_ABILITY);
	g_GumpManager.UpdateContent(1, 0, GT_ABILITY);
}
//----------------------------------------------------------------------------------
bool CGumpAbility::OnLeftMouseButtonDoubleClick()
{
	WISPFUN_DEBUG("c85_f6");
	OnAbilityUse(Serial != 0 ? 1 : 0);

	return true;
}
//----------------------------------------------------------------------------------
