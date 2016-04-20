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
m_Weight(0), m_Armor(0), m_Gold(0), m_Warmode(false), m_StatsCap(0), m_Followers(0),
m_MaxFollowers(5), m_FireResistance(0), m_ColdResistance(0), m_PoisonResistance(0),
m_EnergyResistance(0), m_Luck(0), m_MinDamage(0), m_MaxDamage(0), m_TithingPoints(0),
m_LockStr(0), m_LockInt(0), m_LockDex(0)
//, m_Race(CRT_HUMAN)
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
/*!
Установить имя
@param [__in] val Новое имя
@return 
*/
void TPlayer::SetName( __in string val)
{
	m_Name = val;

	if (g_GameState >= GS_GAME)
	{
		char buf[256] = { 0 };
		sprintf(buf, "Ultima Online - %s (%s)", val.c_str(), ServerList.GetServerName().c_str());

		SetWindowTextA(g_hWnd, buf);
	}

	if (PluginManager != NULL)
		PluginManager->WindowProc(g_hWnd, UOMSG_SET_PLAYER_NAME, (WPARAM)val.c_str(), 0);
}
//---------------------------------------------------------------------------
/*!
Установить базовое значение навыка
@param [__in] index Индекс навыка
@param [__in] val Новое значение
@return 
*/
void TPlayer::SetSkillBaseValue( __in int index, __in float val)
{
	if (index < 0 || index >= g_SkillsCount)
		return;

	m_Skills[index].SetBaseValue(val);
}
//---------------------------------------------------------------------------
/*!
Установить значение навыка
@param [__in] index Индекс навыка
@param [__in] val Новое значение
@return 
*/
void TPlayer::SetSkillValue( __in int index, __in float val)
{
	if (index < 0 || index >= g_SkillsCount)
		return;

	m_Skills[index].SetValue(val);
}
//---------------------------------------------------------------------------
/*!
Установить максимальное значение навыка
@param [__in] index Индекс навыка
@param [__in] val Новое значение
@return 
*/
void TPlayer::SetSkillCap( __in int index, __in float val)
{
	if (index < 0 || index >= g_SkillsCount)
		return;

	m_Skills[index].SetCap(val);
}
//---------------------------------------------------------------------------
/*!
Установить статус навыка
@param [__in] index Индекс навыка
@param [__in] val Новое состояние
@return 
*/
void TPlayer::SetSkillStatus( __in int index, __in BYTE val)
{
	if (index < 0 || index >= g_SkillsCount)
		return;

	m_Skills[index].SetStatus(val);
}
//---------------------------------------------------------------------------
/*!
Получить базовое значение навыка
@param [__in] index Индекс навыка
@return Текущее значение
*/
float TPlayer::GetSkillBaseValue( __in int index) const
{
	if (index < 0 || index >= g_SkillsCount)
		return 0.0f;

	return m_Skills[index].GetBaseValue();
}
//---------------------------------------------------------------------------
/*!
Получить значение навыка
@param [__in] index Индекс навыка
@return Текущее значение
*/
float TPlayer::GetSkillValue( __in int index) const
{
	if (index < 0 || index >= g_SkillsCount)
		return 0.0f;

	return m_Skills[index].GetValue();
}
//---------------------------------------------------------------------------
/*!
Получить максимальное значение навыка
@param [__in] index Индекс навыка
@return Текущее значение
*/
float TPlayer::GetSkillCap( __in int index) const
{
	if (index < 0 || index >= g_SkillsCount)
		return 0.0f;

	return m_Skills[index].GetCap();
}
//---------------------------------------------------------------------------
/*!
Получить статус навыка
@param [__in] index Индекс навыка
@return Текущее состояние
*/
BYTE TPlayer::GetSkillStatus( __in int index) const
{
	if (index < 0 || index >= g_SkillsCount)
		return 0;

	return m_Skills[index].GetStatus();
}
//---------------------------------------------------------------------------
/*!
Поиск бинтов в сумке (и подсумках)
@return Ссылка на бинт или NULL
*/
TGameItem *TPlayer::FindBandage()
{
	TGameItem *item = FindLayer(OL_BACKPACK);

	if (item != NULL)
		item = item->FindItem(0x0E21);

	return item;
}
//---------------------------------------------------------------------------