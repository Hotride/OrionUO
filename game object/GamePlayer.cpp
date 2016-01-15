/****************************************************************************
**
** GameObject.cpp
**
** Copyright (C) September 2015 Hotride
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

TPlayer *g_Player = NULL;
//---------------------------------------------------------------------------
TPlayer::TPlayer(DWORD serial)
: TGameCharacter(serial), m_Str(0), m_Int(0), m_Dex(0), m_MaxWeight(0),
m_Weight(0), m_Armor(0), m_Gold(0), m_Warmode(false)
{
	m_Skills = new TSkill[g_SkillsCount];
}
//---------------------------------------------------------------------------
TPlayer::~TPlayer()
{
	if (m_Skills != NULL)
	{
		delete []m_Skills;
		m_Skills = NULL;
	}
}
//---------------------------------------------------------------------------
void TPlayer::SetName(string val)
{
	m_Name = val;

	if (g_GameState >= GS_GAME)
	{
		char buf[256] = { 0 };
		sprintf(buf, "Ultima Online - %s (%s)", val.c_str(), ServerList.GetServerName().c_str());

		SetWindowTextA(g_hWnd, buf);
	}
}
//---------------------------------------------------------------------------
void TPlayer::SetSkillBaseValue(int index, float val)
{
	if (index < 0 || index >= g_SkillsCount)
		return;

	m_Skills[index].SetBaseValue(val);
}
//---------------------------------------------------------------------------
void TPlayer::SetSkillValue(int index, float val)
{
	if (index < 0 || index >= g_SkillsCount)
		return;

	m_Skills[index].SetValue(val);
}
//---------------------------------------------------------------------------
void TPlayer::SetSkillCap(int index, float val)
{
	if (index < 0 || index >= g_SkillsCount)
		return;

	m_Skills[index].SetCap(val);
}
//---------------------------------------------------------------------------
void TPlayer::SetSkillStatus(int index, BYTE val)
{
	if (index < 0 || index >= g_SkillsCount)
		return;

	m_Skills[index].SetStatus(val);
}
//---------------------------------------------------------------------------
float TPlayer::GetSkillBaseValue(int index) const
{
	if (index < 0 || index >= g_SkillsCount)
		return 0.0f;

	return m_Skills[index].GetBaseValue();
}
//---------------------------------------------------------------------------
float TPlayer::GetSkillValue(int index) const
{
	if (index < 0 || index >= g_SkillsCount)
		return 0.0f;

	return m_Skills[index].GetValue();
}
//---------------------------------------------------------------------------
float TPlayer::GetSkillCap(int index) const
{
	if (index < 0 || index >= g_SkillsCount)
		return 0.0f;

	return m_Skills[index].GetCap();
}
//---------------------------------------------------------------------------
BYTE TPlayer::GetSkillStatus(int index) const
{
	if (index < 0 || index >= g_SkillsCount)
		return 0;

	return m_Skills[index].GetStatus();
}
//---------------------------------------------------------------------------
TGameItem *TPlayer::FindBandage()
{
	TGameItem *item = FindLayer(OL_BACKPACK);

	if (item != NULL)
		item = item->FindItem(0x0E21);

	return item;
}
//---------------------------------------------------------------------------