// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** UseItemsList.cpp
**
** Copyright (C) September 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CUseItemActions g_UseItemActions;
//----------------------------------------------------------------------------------
void CUseItemActions::Add(int serial)
{
	WISPFUN_DEBUG("c186_f1");
	for (deque<uint>::iterator i = m_List.begin(); i != m_List.end(); ++i)
	{
		if (*i == serial)
			return;
	}

	m_List.push_back(serial);
}
//----------------------------------------------------------------------------------
void CUseItemActions::Process()
{
	WISPFUN_DEBUG("c186_f2");
	if (Timer <= g_Ticks)
	{
		Timer = g_Ticks + 1000;

		if (m_List.empty())
			return;

		uint serial = m_List.front();
		m_List.pop_front();

		if (g_World->FindWorldObject(serial) != NULL)
		{
			if (serial < 0x40000000) //NPC
				g_Orion.PaperdollReq(serial);
			else //item
				g_Orion.DoubleClick(serial);
		}
	}
}
//----------------------------------------------------------------------------------
