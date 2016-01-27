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
class TCharacterList
{
private:
	int m_Count; //Текущее количество персонажей (из пакета)
	int m_Selected; //Указатель на индекс выбранного персонажа

	bool m_OnePerson; //Доступен всего 1 персонаж с данного аккаунта
	bool m_Have6Slot; //Данный аккаунт имеет 6 слот
	bool m_Have7Slot; //Данный аккаунт имеет 7 слот
	WORD m_ClientFlag; //Флаги для отправки на сервер при создании персонажа

	string m_Name[7]; //Память под 7 персонажей

public:
	TCharacterList();
	~TCharacterList() {}

	//Очистка списка
	void Clear();

	SETGET(int, Count);
	SETGET(int, Selected);
	SETGET(bool, OnePerson);
	SETGET(bool, Have6Slot);
	SETGET(bool, Have7Slot);
	SETGET(WORD, ClientFlag);

	void SetName(int pos, string name);
	
	string GetName(int pos) const;
	string GetSelectedName() const;
};
//---------------------------------------------------------------------------
extern TCharacterList CharacterList; //Указатель на список персонажей
//---------------------------------------------------------------------------
#endif