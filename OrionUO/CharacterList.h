/***********************************************************************************
**
** CharacterList.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef CHARACTERLIST_H
#define CHARACTERLIST_H
//----------------------------------------------------------------------------------
#include "Globals.h"
//----------------------------------------------------------------------------------
//!Класс списка персонажей для экрана выбора персонажей
class CCharacterList
{
	SETGET(int, Count);
	SETGET(int, Selected);
	SETGET(bool, OnePerson);
	SETGET(bool, Have6Slot);
	SETGET(bool, Have7Slot);
	SETGET(ushort, ClientFlag);

private:
	//!Память под 7 персонажей
	string m_Name[7];

public:
	CCharacterList();
	virtual ~CCharacterList() {}

	/*!
	Очистка списка
	@return 
	*/
	void Clear();


	/*!
	Установить имя персонажа в указанном слоте
	@param [__in] pos Позиция в списке
	@param [__in] name Новое имя
	@return 
	*/
	void SetName(int pos, string name);

	/*!
	Получить имя персонажа в указанном слоте
	@param [__in] pos Позиция в списке
	@return Имя персонажа
	*/
	string GetName(int pos) const;

	/*!
	Получить имя выбранного персонажа
	@return Имя персонажа
	*/
	string GetSelectedName() const;
};
//----------------------------------------------------------------------------------
//!Указатель на список персонажей
extern CCharacterList g_CharacterList;
//----------------------------------------------------------------------------------
#endif
