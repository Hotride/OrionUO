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
//!Класс игрока
class CPlayer: public CGameCharacter
{
	SETGET(short, Str, 0);
	SETGET(short, Int, 0);
	SETGET(short, Dex, 0);
	SETGET(uchar, LockStr, 0);
	SETGET(uchar, LockInt, 0);
	SETGET(uchar, LockDex, 0);
	SETGET(ushort, MaxWeight, 0);
	SETGET(ushort, Weight, 0);
	SETGET(short, Armor, 0);
	SETGET(uint, Gold, 0);
	SETGET(bool, Warmode, 0);
	SETGET(ushort, StatsCap, 0);
	SETGET(uchar, Followers, 0);
	SETGET(uchar, MaxFollowers, 5);
	SETGET(short, FireResistance, 0);
	SETGET(short, ColdResistance, 0);
	SETGET(short, PoisonResistance, 0);
	SETGET(short, EnergyResistance, 0);
	SETGET(short, MaxPhysicalResistance, 0);
	SETGET(short, MaxFireResistance, 0);
	SETGET(short, MaxColdResistance, 0);
	SETGET(short, MaxPoisonResistance, 0);
	SETGET(short, MaxEnergyResistance, 0);
	SETGET(short, Luck, 0);
	SETGET(short, MinDamage, 0);
	SETGET(short, MaxDamage, 0);
	SETGET(uint, TithingPoints, 0);
	SETGET(short, DefenceChance, 0);
	SETGET(short, MaxDefenceChance, 0);
	SETGET(short, AttackChance, 0);
	SETGET(short, WeaponSpeed, 0);
	SETGET(short, WeaponDamage, 0);
	SETGET(short, LowerRegCost, 0);
	SETGET(short, SpellDamage, 0);
	SETGET(short, CastRecovery, 0);
	SETGET(short, CastSpeed, 0);
	SETGET(short, LowerManaCost, 0);
	//SETGET(CHARACTER_RACE_TYPE, Race);

	SETGET(short, OldX, 0);
	SETGET(short, OldY, 0);
	SETGET(char, OldZ, 0);

	//!Указатель на список умений
	CSkill *m_Skills{ NULL };

public:
	CPlayer(const uint &serial);
	virtual ~CPlayer();

	//Указатель на Fast Walk Stack
	CFastWalkStack m_FastWalkStack;

	void CloseBank();

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

	void UpdateRemoveRange();
};
//----------------------------------------------------------------------------------
//!Ссылка на игрока
extern CPlayer *g_Player;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
