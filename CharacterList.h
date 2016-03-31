/****************************************************************************
**
** CharacterList.h
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
#ifndef CharacterListH
#define CharacterListH
//--------------------------------------------------------------------------
//!Класс списка персонажей для экрана выбора персонажей
class TCharacterList
{
private:
	//!Текущее количество персонажей (из пакета)
	int m_Count;

	//!Указатель на индекс выбранного персонажа
	int m_Selected;

	//!Доступен всего 1 персонаж с данного аккаунта
	bool m_OnePerson;

	//!Данный аккаунт имеет 6 слот
	bool m_Have6Slot;

	//!Данный аккаунт имеет 7 слот
	bool m_Have7Slot;

	//!Флаги для отправки на сервер при создании персонажа
	WORD m_ClientFlag;

	//!Память под 7 персонажей
	string m_Name[7];

public:
	TCharacterList();
	~TCharacterList() {}

	/*!
	Очистка списка
	@return 
	*/
	void Clear();

	SETGET(int, Count);
	SETGET(int, Selected);
	SETGET(bool, OnePerson);
	SETGET(bool, Have6Slot);
	SETGET(bool, Have7Slot);
	SETGET(WORD, ClientFlag);

	/*!
	Установить имя персонажа в указанном слоте
	@param [__in] pos Позиция в списке
	@param [__in] name Новое имя
	@return 
	*/
	void SetName(__in int pos, __in string name);

	/*!
	Получить имя персонажа в указанном слоте
	@param [__in] pos Позиция в списке
	@return Имя персонажа
	*/
	string GetName(__in int pos) const;

	/*!
	Получить имя выбранного персонажа
	@return Имя персонажа
	*/
	string GetSelectedName() const;
};
//---------------------------------------------------------------------------
//!Указатель на список персонажей
extern TCharacterList CharacterList;
//---------------------------------------------------------------------------
#endif