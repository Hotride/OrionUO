// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** CharacterList.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"

CCharacterList g_CharacterList;
//----------------------------------------------------------------------------------
CCharacterList::CCharacterList()
{
	WISPFUN_DEBUG("c182_f1");
	//!Чистим при создании
	Clear();
}
//----------------------------------------------------------------------------------
/*!
Очистка списка
@return 
*/
void CCharacterList::Clear()
{
	WISPFUN_DEBUG("c182_f2");
	IFOR(i, 0, 7)
		m_Name[i] = "";

	Selected = 0;
	OnePerson = false;
	Have6Slot = false;
	Have7Slot = false;
}
//----------------------------------------------------------------------------------
/*!
Установить имя персонажа в указанном слоте
@param [__in] pos Позиция в списке
@param [__in] name Новое имя
@return 
*/
void CCharacterList::SetName(intptr_t pos, const string &name)
{
	WISPFUN_DEBUG("c182_f3");
	//!При корректной позиции записываем имя
	if (pos >= 0 && pos < Count)
		m_Name[pos] = name;
}
//----------------------------------------------------------------------------------
/*!
Получить имя персонажа в указанном слоте
@param [__in] pos Позиция в списке
@return Имя персонажа
*/
string CCharacterList::GetName(intptr_t pos) const
{
	WISPFUN_DEBUG("c182_f4");
	string result = "";

	//!При корректной позиции возвращаем имя
	if (pos >= 0 && pos < Count)
		result = m_Name[pos];

	return result;
}
//----------------------------------------------------------------------------------
/*!
Получить имя выбранного персонажа
@return Имя персонажа
*/
string CCharacterList::GetSelectedName() const
{
	WISPFUN_DEBUG("c182_f5");
	string result = "";

	//!При корректной позиции возвращаем имя
	if (Selected >= 0 && Selected < Count)
		result = m_Name[Selected];

	return result;
}
//----------------------------------------------------------------------------------
