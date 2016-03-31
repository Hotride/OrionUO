/****************************************************************************
**
** ShopItem.h
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
#ifndef ShopItemH
#define ShopItemH
//--------------------------------------------------------------------------
//!Класс объекта магазина
class TShopItem
{
private:
	//!Количество, которое выделено в списке покупок
	int m_Count;

	//!Имя предмета для списка покупок
	string m_Name;

	//!Цена предмета
	DWORD m_Price;

	//!Выделен в байлисте
	bool m_Selected;

public:
	TShopItem();
	~TShopItem();

	SETGETEX(int, Count);
	SETGET(string, Name);
	SETGET(DWORD, Price);
	SETGET(bool, Selected);
};
//---------------------------------------------------------------------------
#endif