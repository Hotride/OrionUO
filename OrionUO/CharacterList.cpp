/***********************************************************************************
**
** CharacterList.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "CharacterList.h"

CCharacterList g_CharacterList;
//----------------------------------------------------------------------------------
CCharacterList::CCharacterList()
:m_Count(5), m_Selected(0), m_OnePerson(false), m_Have6Slot(false),
m_Have7Slot(false), m_ClientFlag(0)
{
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
	IFOR(i, 0, 7)
		m_Name[i] = "";

	m_Selected = 0;
	m_OnePerson = false;
	m_Have6Slot = false;
	m_Have7Slot = false;
}
//----------------------------------------------------------------------------------
/*!
Установить имя персонажа в указанном слоте
@param [__in] pos Позиция в списке
@param [__in] name Новое имя
@return 
*/
void CCharacterList::SetName(__in int pos, __in string name)
{
	//!При корректной позиции записываем имя
	if (pos >= 0 && pos < m_Count)
		m_Name[pos] = name;
}
//----------------------------------------------------------------------------------
/*!
Получить имя персонажа в указанном слоте
@param [__in] pos Позиция в списке
@return Имя персонажа
*/
string CCharacterList::GetName(__in int pos) const
{
	string result = "";

	//!При корректной позиции возвращаем имя
	if (pos >= 0 && pos < m_Count)
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
	string result = "";

	//!При корректной позиции возвращаем имя
	if (m_Selected >= 0 && m_Selected < m_Count)
		result = m_Name[m_Selected];

	return result;
}
//----------------------------------------------------------------------------------
