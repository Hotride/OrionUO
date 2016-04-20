/****************************************************************************
**
** GameObject.h
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
#ifndef GamePlayerH
#define GamePlayerH
//--------------------------------------------------------------------------
//!Класс игрока
class TPlayer: public TGameCharacter
{
private:
	//!Сила
	short m_Str;

	//!Интеллект
	short m_Int;

	//!Ловкость
	short m_Dex;

	BYTE m_LockStr;

	BYTE m_LockInt;

	BYTE m_LockDex;

	//!Максимальный вес
	WORD m_MaxWeight;

	//!Текущее значение веса
	WORD m_Weight;

	//!Значение брони
	short m_Armor;

	//!Количество денег на счету
	DWORD m_Gold;

	//!Максимальная сумма статов
	WORD m_StatsCap;
	
	//CHARACTER_RACE_TYPE m_Race;

	//!Количество подчиненных существ
	BYTE m_Followers;

	//!Максимальное количество подчиненных существ
	BYTE m_MaxFollowers;
	
	//!Сопротивление огню
	short m_FireResistance;

	//!Сопротивление холоду
	short m_ColdResistance;

	//!Сопротивление яду
	short m_PoisonResistance;

	//!Сопротивление енергии
	short m_EnergyResistance;
	
	//!Удача
	short m_Luck;

	//!Минимальный урон
	short m_MinDamage;

	//!Максимальный урон
	short m_MaxDamage;
	
	//!Очки для какой-то херни, вроде паладинские
	DWORD m_TithingPoints;

	//!Режим боя
	bool m_Warmode;

	//!Указатель на список умений
	TSkill *m_Skills;
public:
	TPlayer(DWORD serial);
	virtual ~TPlayer();
	
	SETGET(short, Str);
	SETGET(short, Int);
	SETGET(short, Dex);
	SETGET(BYTE, LockStr);
	SETGET(BYTE, LockInt);
	SETGET(BYTE, LockDex);
	SETGET(WORD, MaxWeight);
	SETGET(WORD, Weight);
	SETGET(short, Armor);
	SETGET(DWORD, Gold);
	SETGET(bool, Warmode);
	SETGET(WORD, StatsCap);
	SETGET(BYTE, Followers);
	SETGET(BYTE, MaxFollowers);
	SETGET(short, FireResistance);
	SETGET(short, ColdResistance);
	SETGET(short, PoisonResistance);
	SETGET(short, EnergyResistance);
	SETGET(short, Luck);
	SETGET(short, MinDamage);
	SETGET(short, MaxDamage);
	SETGET(DWORD, TithingPoints);
	//SETGET(CHARACTER_RACE_TYPE, Race);

	/*!
	Установить имя
	@param [__in] val Новое имя
	@return 
	*/
	void SetName(__in string val);

	/*!
	Установить базовое значение навыка
	@param [__in] index Индекс навыка
	@param [__in] val Новое значение
	@return 
	*/
	void SetSkillBaseValue(__in int index, __in float val);

	/*!
	Установить значение навыка
	@param [__in] index Индекс навыка
	@param [__in] val Новое значение
	@return 
	*/
	void SetSkillValue(__in int index, __in float val);

	/*!
	Установить максимальное значение навыка
	@param [__in] index Индекс навыка
	@param [__in] val Новое значение
	@return 
	*/
	void SetSkillCap(__in int index, __in float val);

	/*!
	Установить статус навыка
	@param [__in] index Индекс навыка
	@param [__in] val Новое состояние
	@return 
	*/
	void SetSkillStatus(__in int index, __in BYTE val);

	/*!
	Получить базовое значение навыка
	@param [__in] index Индекс навыка
	@return Текущее значение
	*/
	float GetSkillBaseValue(__in int index) const;

	/*!
	Получить значение навыка
	@param [__in] index Индекс навыка
	@return Текущее значение
	*/
	float GetSkillValue(__in int index) const;

	/*!
	Получить максимальное значение навыка
	@param [__in] index Индекс навыка
	@return Текущее значение
	*/
	float GetSkillCap(__in int index) const;

	/*!
	Получить статус навыка
	@param [__in] index Индекс навыка
	@return Текущее состояние
	*/
	BYTE GetSkillStatus(__in int index) const;

	/*!
	Поиск бинтов в сумке (и подсумках)
	@return Ссылка на бинт или NULL
	*/
	TGameItem *FindBandage();

	/*!
	Проверка на игрока (персонаж - игрок человека за компьютером)
	@return Игрок или нет
	*/
	bool IsPlayer() {return true;}
};
//---------------------------------------------------------------------------
//!Ссылка на игрока
extern TPlayer *g_Player;
//---------------------------------------------------------------------------
#endif