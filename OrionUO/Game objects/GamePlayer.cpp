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
#include "ObjectOnCursor.h"
#include "../Gumps/GumpCombatBook.h"
#include "../Managers/GumpManager.h"
#include "../OrionUO.h"
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
void CPlayer::UpdateAbilities()
{
	ushort equippedGraphic = 0;
	uint ignoreSerial = 0;

	if (g_ObjectInHand != NULL)
		ignoreSerial = g_ObjectInHand->Serial;

	CGameItem *layerObject = g_Player->FindLayer(OL_1_HAND);

	if (layerObject != NULL)
	{
		if (layerObject->Serial != ignoreSerial)
			equippedGraphic = layerObject->Graphic;
	}
	else
	{
		layerObject = g_Player->FindLayer(OL_2_HAND);

		if (layerObject != NULL && layerObject->Serial != ignoreSerial)
			equippedGraphic = layerObject->Graphic;
	}

	int found[2] = { -1, -1 };

	if (equippedGraphic)
	{
		ushort graphics[2] = { equippedGraphic, 0 };
		ushort imageID = layerObject->GetStaticData()->AnimID;
		int count = 1;

		ushort testGraphic = equippedGraphic - 1;

		if (g_Orion.m_StaticData[testGraphic / 32].Tiles[testGraphic % 32].AnimID == imageID)
		{
			graphics[1] = testGraphic;
			count = 2;
		}
		else
		{
			testGraphic = equippedGraphic + 1;

			if (g_Orion.m_StaticData[testGraphic / 32].Tiles[testGraphic % 32].AnimID == imageID)
			{
				graphics[1] = testGraphic;
				count = 2;
			}
		}

		IFOR(k, 0, count)
		{
			testGraphic = graphics[k];
			int foundIndex = 0;

			IFOR(i, 0, 2)
			{
				IFOR(j, 0, MAX_ABILITIES_COUNT)
				{
					if (i && found[0] == j)
						continue;

					USHORT_LIST list = CGumpCombatBook::GetItemsList(j);

					int size = (int)list.size();

					IFOR(item, 0, size)
					{
						if (list[item] == testGraphic)
						{
							if (i && item < foundIndex)
							{
								found[1] = found[0];
								found[0] = j;

								break;
							}

							foundIndex = item;
							found[i] = j;

							break;
						}
					}

					if (found[i] != -1)
						break;
				}
			}

			if (found[0] != -1 && found[1] != -1)
				break;
		}
	}

	if (found[0] != -1 && found[1] != -1)
	{
		g_Ability[0] = found[0];
		g_Ability[1] = found[1];
	}
	else
	{
		g_Ability[0] = 4;
		g_Ability[1] = 10;
	}

	g_GumpManager.UpdateContent(0, 0, GT_ABILITY);
	g_GumpManager.UpdateContent(1, 0, GT_ABILITY);

	g_GumpManager.UpdateContent(0, 0, GT_COMBAT_BOOK);
}
//---------------------------------------------------------------------------