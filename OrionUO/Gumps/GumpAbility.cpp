/***********************************************************************************
**
** GumpAbility.cpp
**
** Copyright (C) December 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GumpAbility.h"
#include "../SelectedObject.h"
#include "../OrionUO.h"
#include "../Managers/ClilocManager.h"
#include "../ToolTip.h"
#include "../Network/Packets.h"
#include "../Managers/GumpManager.h"
//----------------------------------------------------------------------------------
CGumpAbility::CGumpAbility(const uint &serial, const int &x, const int &y)
: CGump(GT_ABILITY, serial, x, y)
{
	int index = (m_Serial != 0 ? 1 : 0);
	ushort &ability = g_AbilityList[g_Ability[index]];

	m_Graphic = 0x5200 + (ability & 0x00FF);

	m_Locker.Serial = ID_GS_LOCK_MOVING;

	m_Colorizer = (CGUIGlobalColor*)Add(new CGUIGlobalColor((ability & 0x8000), 1.0f, 0.25f, 0.5f, 1.0f));

	m_Body = (CGUIGumppic*)Add(new CGUIGumppic(m_Graphic, 0, 0));
}
//----------------------------------------------------------------------------------
CGumpAbility::~CGumpAbility()
{
}
//----------------------------------------------------------------------------------
void CGumpAbility::InitToolTip()
{
	int index = (m_Serial != 0 ? 1 : 0);

	g_ToolTip.Set(g_ClilocManager.Cliloc(g_Language)->GetW(1028838 + (g_AbilityList[g_Ability[index]] & 0x00FF)), 80);
}
//----------------------------------------------------------------------------------
void CGumpAbility::UpdateContent()
{
	if (m_Colorizer != NULL && m_Body != NULL)
	{
		int index = (m_Serial != 0 ? 1 : 0);
		ushort &ability = g_AbilityList[g_Ability[index]];

		m_Colorizer->Enabled = (ability & 0x8000);

		m_Body->Graphic = 0x5200 + (ability & 0x00FF);
	}
}
//----------------------------------------------------------------------------------
void CGumpAbility::GUMP_BUTTON_EVENT_C
{
	if (serial == ID_GS_LOCK_MOVING)
		m_LockMoving = !m_LockMoving;
}
//----------------------------------------------------------------------------------
void CGumpAbility::OnAbilityUse(const int &index)
{
	ushort &ability = g_AbilityList[g_Ability[index]];

	CPacketUseCombatAbility(ability & 0xFF).Send();

	if (!(ability & 0x8000))
	{
		IFOR(i, 0, MAX_ABILITIES_COUNT)
		{
			ushort &tempAbility = g_AbilityList[i];
			tempAbility &= 0x00FF;
		}
	}

	ability ^= 0x8000;

	g_GumpManager.UpdateContent(0, 0, GT_ABILITY);
	g_GumpManager.UpdateContent(1, 0, GT_ABILITY);
}
//----------------------------------------------------------------------------------
bool CGumpAbility::OnLeftMouseButtonDoubleClick()
{
	OnAbilityUse(m_Serial != 0 ? 1 : 0);

	return true;
}
//----------------------------------------------------------------------------------
