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
class TPlayer: public TGameCharacter
{
private:
	short m_Str; //Сила
	short m_Int; //Интеллект
	short m_Dex; //Ловкость

	WORD m_MaxWeight; //Максимальный вес
	WORD m_Weight; //Текущее значение веса

	short m_Armor; //Значение брони

	DWORD m_Gold; //Количество денег на счету

	bool m_Warmode; //Режим боя

	TSkill *m_Skills; //Указатель на умения
public:
	TPlayer(DWORD serial);
	virtual ~TPlayer();
	
	SETGET(short, Str)
	SETGET(short, Int)
	SETGET(short, Dex)
	SETGET(WORD, MaxWeight)
	SETGET(WORD, Weight)
	SETGET(short, Armor)
	SETGET(DWORD, Gold)
	SETGET(bool, Warmode)

	void SetSkillBaseValue(int index, float val);
	void SetSkillValue(int index, float val);
	void SetSkillCap(int index, float val);
	void SetSkillStatus(int index, BYTE val);
	
	float GetSkillBaseValue(int index) const;
	float GetSkillValue(int index) const;
	float GetSkillCap(int index) const;
	BYTE GetSkillStatus(int index) const;

	//Для поиска бинтов в сумке (и подсумках)
	TGameItem *FindBandage();

	bool IsPlayer() {return true;}
};
//---------------------------------------------------------------------------
extern TPlayer *g_Player; //Ссылка на игрока
//---------------------------------------------------------------------------
#endif