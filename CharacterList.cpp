/****************************************************************************
**
** CharacterList.cpp
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

TCharacterList CharacterList;
//---------------------------------------------------------------------------
TCharacterList::TCharacterList()
:m_Count(5), m_Selected(0), m_OnePerson(false), m_Have6Slot(false),
m_Have7Slot(false), m_ClientFlag(0)
{
	//!Чистим при создании
	Clear();
}
//---------------------------------------------------------------------------
/*!
Очистка списка
@return 
*/
void TCharacterList::Clear()
{
	IFOR(i, 0, 7)
		m_Name[i] = "";

	m_Selected = 0;
	m_OnePerson = false;
	m_Have6Slot = false;
	m_Have7Slot = false;
}
//---------------------------------------------------------------------------
/*!
Установить имя персонажа в указанном слоте
@param [__in] pos Позиция в списке
@param [__in] name Новое имя
@return 
*/
void TCharacterList::SetName( __in int pos, __in string name)
{
	//!При корректной позиции записываем имя
	if (pos >= 0 && pos < m_Count)
		m_Name[pos] = name;
}
//---------------------------------------------------------------------------
/*!
Получить имя персонажа в указанном слоте
@param [__in] pos Позиция в списке
@return Имя персонажа
*/
string TCharacterList::GetName( __in int pos) const
{
	string result = "";

	//!При корректной позиции возвращаем имя
	if (pos >= 0 && pos < m_Count)
		result = m_Name[pos];

	return result;
}
//---------------------------------------------------------------------------
/*!
Получить имя выбранного персонажа
@return Имя персонажа
*/
string TCharacterList::GetSelectedName() const
{
	string result = "";

	//!При корректной позиции возвращаем имя
	if (m_Selected >= 0 && m_Selected < m_Count)
		result = m_Name[m_Selected];

	return result;
}
//---------------------------------------------------------------------------