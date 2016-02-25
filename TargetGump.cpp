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
			UO->DrawGump(0x1069, m_Color, m_X, m_Y, m_Hits, 0);
	}
}
//--------------------------------------------------------------------------
//-------------------------------TNewTargetSystem---------------------------
//--------------------------------------------------------------------------
TNewTargetSystem::TNewTargetSystem()
: m_Serial(0), m_TopX(0), m_TopY(0), m_BottomX(0), m_BottomY(0), m_Color(0),
m_GumpX(20), m_GumpY(20)
{
}
//--------------------------------------------------------------------------
TNewTargetSystem::~TNewTargetSystem()
{
}
//--------------------------------------------------------------------------
void TNewTargetSystem::Draw()
{
	if (m_Serial != 0 && m_TopY != m_BottomY)
	{
		UO->DrawGump(0x1068, m_Color, m_TopX, m_TopY);
		UO->DrawGump(0x1068, m_Color, m_BottomX, m_BottomY);
	}
}
//---------------------------------------------------------------------------