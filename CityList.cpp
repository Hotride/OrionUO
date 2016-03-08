/****************************************************************************
**
** CityList.cpp
**
** Copyright (C) October 2015 Hotride
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

TCityList *CityList = NULL;
//---------------------------------------------------------------------------
//-----------------------------------TCityItem-------------------------------
//---------------------------------------------------------------------------
TCityItem::TCityItem()
: TBaseQueueItem(), m_LocationIndex(0), m_Name(""), m_Area(""), m_Selected(false),
m_City(NULL)
{
}
//---------------------------------------------------------------------------
TCityItem::~TCityItem()
{
}
//---------------------------------------------------------------------------
void TCityItem::InitCity()
{
	//Линкуем город
	m_City = CityManager->GetCity(m_Name);
}
//---------------------------------------------------------------------------
//---------------------------------TCityItemNew------------------------------
//---------------------------------------------------------------------------
TCityItemNew::TCityItemNew()
: TCityItem(), m_X(0), m_Y(0), m_Z(0), m_MapIndex(0) //, m_Cliloc(0)
{
}
//---------------------------------------------------------------------------
TCityItemNew::~TCityItemNew()
{
	//Очистим текстуру
	m_Texture.Clear();
}
//---------------------------------------------------------------------------
void TCityItemNew::CreateTextTexture(DWORD clilocID)
{
	//Очистим текстуру
	m_Texture.Clear();

	//Используем обработку HTML-тэгов при создании текстуры текста
	FontManager->SetUseHTML(true);

	//Получаем строку клилока с описанием города
	wstring wtext = ClilocManager->Cliloc(g_Language)->GetW(clilocID);

	//Создаем текстуру текста
	FontManager->GenerateW(1, m_Texture, wtext.c_str(), 0, 30, 150);

	//Выключаем обработку HTML-тэгов
	FontManager->SetUseHTML(false);
}
//---------------------------------------------------------------------------
//-----------------------------------TCityList-------------------------------
//---------------------------------------------------------------------------
TCityList::TCityList()
: TBaseQueue()
{
}
//---------------------------------------------------------------------------
TCityList::~TCityList()
{
}
//---------------------------------------------------------------------------
TCityItem *TCityList::GetCity(int index)
{
	//Указатель на список городов
	TCityItem *city = (TCityItem*)m_Items;

	//Пройдемся по списку
	while (city != NULL)
	{
		//Если это город с нужным индексом локации - сваливаем
		if (city->LocationIndex == index)
			break;

		//Или переходим к следующему городу
		city = (TCityItem*)city->m_Next;
	}

	//Что нашлось (или не нашлось)
	return city;
}
//---------------------------------------------------------------------------