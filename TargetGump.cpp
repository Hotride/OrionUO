/****************************************************************************
**
** TargetGump.cpp
**
** Copyright (C) February 2016 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//---------------------------------------------------------------------------
#include "stdafx.h"

TTargetGump TargetGump;
TTargetGump AttackTargetGump;
TNewTargetSystem NewTargetSystem;
//--------------------------------------------------------------------------
//-------------------------------TTargetGump--------------------------------
//--------------------------------------------------------------------------
TTargetGump::TTargetGump()
: m_X(0), m_Y(0), m_Hits(0), m_Color(0)
{
}
//--------------------------------------------------------------------------
TTargetGump::~TTargetGump()
{
}
//--------------------------------------------------------------------------
void TTargetGump::Draw()
{
	if (m_Color != 0)
	{
		UO->DrawGump(0x1068, m_Color, m_X, m_Y);

		if (m_Hits > 0)
			UO->DrawGump(0x1069, 0x005A, m_X, m_Y, m_Hits, 0);
	}
}
//--------------------------------------------------------------------------
//-------------------------------TNewTargetSystem---------------------------
//--------------------------------------------------------------------------
TNewTargetSystem::TNewTargetSystem()
: m_Serial(0), m_X(0), m_TopY(0), m_BottomY(0), m_ColorGump(0), m_GumpTop(0),
m_GumpBottom(0), m_GumpX(20), m_GumpY(20), m_Hits(0)
{
}
//--------------------------------------------------------------------------
TNewTargetSystem::~TNewTargetSystem()
{
}
//--------------------------------------------------------------------------
void TNewTargetSystem::Draw()
{
	if (!ConfigManager.DisableNewTargetSystem && m_ColorGump != 0)
	{
		TIndexObject *top = UO->GetGumpPointer(m_GumpTop);

		int x = m_X - (top->Width / 2);

		UO->DrawGump(m_GumpTop, 0, x, m_TopY - top->Height);
		UO->DrawGump(m_ColorGump, 0, x, m_TopY - top->Height);
		UO->DrawGump(m_GumpBottom, 0, x, m_BottomY);

		if (m_Hits > 0)
			UO->DrawGump(0x1069, 0x005A, m_X - 16, m_BottomY + 15, m_Hits, 0);
	}
}
//---------------------------------------------------------------------------