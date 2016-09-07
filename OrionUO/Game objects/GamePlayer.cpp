/***********************************************************************************
**
** GamePlayer.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GamePlayer.h"
#include "GameItem.h"
//----------------------------------------------------------------------------------
CPlayer *g_Player = NULL;
//----------------------------------------------------------------------------------
CPlayer::CPlayer(const uint &serial)
: CGameCharacter(serial), m_Str(0), m_Int(0), m_Dex(0), m_MaxWeight(0),
m_Weight(0), m_Armor(0), m_Gold(0), m_Warmode(false), m_StatsCap(0), m_Followers(0),
m_MaxFollowers(5), m_FireResistance(0), m_ColdResistance(0), m_PoisonResistance(0),
m_EnergyResistance(0), m_Luck(0), m_MinDamage(0), m_MaxDamage(0), m_TithingPoints(0),
m_LockStr(0), m_LockInt(0), m_LockDex(0)
//, m_Race(CRT_HUMAN)
{
	m_Skills = new CSkill[g_SkillsCount];
}
//---------------------------------------------------------------------------
CPlayer::~CPlayer()
{
	if (m_Skills != NULL)
	{
		delete []m_Skills;
		m_Skills = NULL;
	}
}
//---------------------------------------------------------------------------
/*!
Установить базовое значение навыка
@param [__in] index Индекс навыка
@param [__in] val Новое значение
@return 
*/
void CPlayer::SetSkillBaseValue(const int &index, const float &val)
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
void CPlayer::SetSkillValue(const int &index, const float &val)
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
void CPlayer::SetSkillCap(const int &index, const float &val)
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
void CPlayer::SetSkillStatus(const int &index, const uchar &val)
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
float CPlayer::GetSkillBaseValue(const int &index) const
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
float CPlayer::GetSkillValue(const int &index) const
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
float CPlayer::GetSkillCap(const int &index) const
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
uchar CPlayer::GetSkillStatus(const int &index) const
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
CGameItem *CPlayer::FindBandage()
{
	CGameItem *item = FindLayer(OL_BACKPACK);

	if (item != NULL)
		item = item->FindItem(0x0E21);

	return item;
}
//---------------------------------------------------------------------------