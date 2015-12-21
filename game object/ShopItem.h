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
class TShopItem
{
private:
	short m_Count; //Количество, которое выделено в списке покупок
	string m_Name; //Имя предмета для списка покупок
	DWORD m_Price; //Цена предмета
	bool m_Selected; //Выделен в байлисте

public:
	TShopItem();
	~TShopItem();

	SETGETEX(short, Count)
	SETGET(string, Name)
	SETGET(DWORD, Price)
	SETGET(bool, Selected)

};
//---------------------------------------------------------------------------
#endif