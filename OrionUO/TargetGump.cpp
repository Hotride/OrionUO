// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** TargetGump.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CTargetGump g_TargetGump;
CTargetGump g_AttackTargetGump;
CNewTargetSystem g_NewTargetSystem;
//----------------------------------------------------------------------------------
//-------------------------------------CTargetGump----------------------------------
//----------------------------------------------------------------------------------
CTargetGump::CTargetGump()
{
}
//----------------------------------------------------------------------------------
CTargetGump::~CTargetGump()
{
}
//----------------------------------------------------------------------------------
void CTargetGump::Draw()
{
	WISPFUN_DEBUG("c210_f1");
	if (m_Color != 0)
	{
		g_Orion.DrawGump(0x1068, m_Color, m_X, m_Y);

		if (m_Hits > 0)
			g_Orion.DrawGump(0x1069, m_HealthColor, m_X, m_Y, m_Hits, 0);
	}
}
//----------------------------------------------------------------------------------
//----------------------------------CNewTargetSystem--------------------------------
//----------------------------------------------------------------------------------
CNewTargetSystem::CNewTargetSystem()
{
}
//----------------------------------------------------------------------------------
CNewTargetSystem::~CNewTargetSystem()
{
}
//----------------------------------------------------------------------------------
void CNewTargetSystem::Draw()
{
	WISPFUN_DEBUG("c211_f1");
	if (!g_ConfigManager.DisableNewTargetSystem && m_ColorGump != 0)
	{
		CIndexObject &top = g_Orion.m_GumpDataIndex[m_GumpTop];

		int x = m_X - (top.Width / 2);

		g_Orion.DrawGump(m_GumpTop, 0, x, m_TopY - top.Height);
		g_Orion.DrawGump(m_ColorGump, 0, x, m_TopY - top.Height);
		g_Orion.DrawGump(m_GumpBottom, 0, x, m_BottomY);

		if (m_Hits > 0)
			g_Orion.DrawGump(0x1069, m_HealthColor, m_X - 16, m_BottomY + 15, m_Hits, 0);
	}
}
//----------------------------------------------------------------------------------
