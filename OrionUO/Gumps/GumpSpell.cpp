/***********************************************************************************
**
** GumpSpell.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GumpSpell.h"
#include "../SelectedObject.h"
#include "../OrionUO.h"
#include "../Managers/MouseManager.h"
//----------------------------------------------------------------------------------
CGumpSpell::CGumpSpell(uint serial, short x, short y, ushort graphic)
: CGump(GT_SPELL, serial, x, y)
{
	m_Graphic = graphic;
	m_Locker.Serial = ID_GS_LOCK_MOVING;

	Add(new CGUIGumppic(m_Graphic, 0, 0));
}
//----------------------------------------------------------------------------------
CGumpSpell::~CGumpSpell()
{
}
//----------------------------------------------------------------------------------
void CGumpSpell::OnButton(const uint &serial)
{
	if (serial == ID_GS_LOCK_MOVING)
		m_LockMoving = !m_LockMoving;
}
//----------------------------------------------------------------------------------
bool CGumpSpell::OnLeftMouseButtonDoubleClick()
{
	g_Orion.CastSpell(m_Serial);

	return true;
}
//----------------------------------------------------------------------------------
