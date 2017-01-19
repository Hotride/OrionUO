/***********************************************************************************
**
** GumpRacialAbility.cpp
**
** Copyright (C) December 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GumpRacialAbility.h"
#include "../SelectedObject.h"
#include "../OrionUO.h"
#include "../Managers/ClilocManager.h"
#include "../ToolTip.h"
#include "../Network/Packets.h"
#include "../Game objects/GamePlayer.h"
//----------------------------------------------------------------------------------
CGumpRacialAbility::CGumpRacialAbility(const uint &serial, const int &x, const int &y)
: CGump(GT_RACIAL_ABILITY, serial, x, y)
{
	m_Locker.Serial = ID_GS_LOCK_MOVING;

	Add(new CGUIGumppic(serial, 0, 0));
}
//----------------------------------------------------------------------------------
CGumpRacialAbility::~CGumpRacialAbility()
{
}
//----------------------------------------------------------------------------------
void CGumpRacialAbility::InitToolTip()
{
	g_ToolTip.Set(g_ClilocManager.Cliloc(g_Language)->GetW(1112198 + (m_Serial - 0x5DD0)), 200);
}
//----------------------------------------------------------------------------------
void CGumpRacialAbility::GUMP_BUTTON_EVENT_C
{
	if (serial == ID_GS_LOCK_MOVING)
		m_LockMoving = !m_LockMoving;
}
//----------------------------------------------------------------------------------
void CGumpRacialAbility::OnAbilityUse(const ushort &index)
{
	switch (index)
	{
		case 0x5DDA:
		{
			if (g_Player->Race == RT_GARGOYLE)
				CPacketToggleGargoyleFlying().Send();

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
bool CGumpRacialAbility::OnLeftMouseButtonDoubleClick()
{
	OnAbilityUse((ushort)m_Serial);

	return true;
}
//----------------------------------------------------------------------------------
