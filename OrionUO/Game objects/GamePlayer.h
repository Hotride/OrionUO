/***********************************************************************************
**
** GamePlayer.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GAMEPLAYER_H
#define GAMEPLAYER_H
//----------------------------------------------------------------------------------
#include "GameCharacter.h"
#include "../Skills.h"
//----------------------------------------------------------------------------------
//!Класс игрока
class CPlayer: public CGameCharacter
{
	SETGET(short, Str);
	SETGET(short, Int);
	SETGET(short, Dex);
	SETGET(uchar, LockStr);
	SETGET(uchar, LockInt);
	SETGET(uchar, LockDex);
	SETGET(ushort, MaxWeight);
	SETGET(ushort, Weight);
	SETGET(short, Armor);
	SETGET(uint, Gold);
	SETGET(bool, Warmode);
	SETGET(WORD, StatsCap);
	SETGET(uchar, Followers);
	SETGET(uchar, MaxFollowers);
	SETGET(short, FireResistance);
	SETGET(short, ColdResistance);
	SETGET(short, PoisonResistance);
	SETGET(short, EnergyResistance);
	SETGET(short, Luck);
	SETGET(short, MinDamage);
	SETGET(short, MaxDamage);
	SETGET(uint, TithingPoints);
	//SETGET(CHARACTER_RACE_TYPE, Race);

	//!Указатель на список умений
	CSkill *m_Skills;

public:
	CPlayer(const uint &serial);
	virtual ~CPlayer();

	/*!
	Установить базовое значение навыка
	@param [__in] index Индекс навыка
	@param [__in] val Новое значение
	@return 
	*/
	void SetSkillBaseValue(const int &index, const float &val);

	/*!
	Установить значение навыка
	@param [__in] index Индекс навыка
	@param [__in] val Новое значение
	@return 
	*/
	void SetSkillValue(const int &index, const float &val);

	/*!
	Установить максимальное значение навыка
	@param [__in] index Индекс навыка
	@param [__in] val Новое значение
	@return 
	*/
	void SetSkillCap(const int &index, const float &val);

	/*!
	Установить статус навыка
	@param [__in] index Индекс навыка
	@param [__in] val Новое состояние
	@return 
	*/
	void SetSkillStatus(const int &index, const uchar &val);

	/*!
	Получить базовое значение навыка
	@param [__in] index Индекс навыка
	@return Текущее значение
	*/
	float GetSkillBaseValue(const int &index) const;

	/*!
	Получить значение навыка
	@param [__in] index Индекс навыка
	@return Текущее значение
	*/
	float GetSkillValue(const int &index) const;

	/*!
	Получить максимальное значение навыка
	@param [__in] index Индекс навыка
	@return Текущее значение
	*/
	float GetSkillCap(const int &index) const;

	/*!
	Получить статус навыка
	@param [__in] index Индекс навыка
	@return Текущее состояние
	*/
	uchar GetSkillStatus(const int &index) const;

	/*!
	Поиск бинтов в сумке (и подсумках)
	@return Ссылка на бинт или NULL
	*/
	class CGameItem *FindBandage();

	/*!
	Проверка на игрока (персонаж - игрок человека за компьютером)
	@return Игрок или нет
	*/
	bool IsPlayer() {return true;}

	void UpdateAbilities();
};
//----------------------------------------------------------------------------------
//!Ссылка на игрока
extern CPlayer *g_Player;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
