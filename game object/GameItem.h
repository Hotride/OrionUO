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
#ifndef GameItemH
#define GameItemH
//--------------------------------------------------------------------------
class TObjectOnCursor;
//--------------------------------------------------------------------------
class TGameItem : public TGameObject
{
protected:
	BYTE m_Layer; //Слой
	WORD m_AnimID; //Индекс анимации
	WORD m_ImageID; //Индекс гампа для папердолла
	BYTE m_UsedLayer; //Используемый предметом слой (из tiledata.mul (verdata))
	bool m_Opened; //Открыт ли контейнер/спеллбук
	bool m_Dragged; //Выведен ли гамп разделения количества?

	//Функция для добавления объекта-мульти
	void AddMultiObject(TMultiObject *obj);
public:
	TGameItem(DWORD serial = 0);
	virtual ~TGameItem();

	//Указатель на элемент листа покупок
	TShopItem *ShopItem;

	//Вставка данных из объекта в руке (на курсоре)
	void Paste(TObjectOnCursor *obj);

	virtual int Draw(bool &mode, int &drawX, int &drawY, DWORD &ticks);
	
	SETGET(BYTE, Layer);
	SETGET(WORD, AnimID);
	SETGET(WORD, ImageID);
	SETGET(BYTE, UsedLayer);
	SETGET(bool, Opened);
	SETGET(bool, Dragged);

	void OnGraphicChange(int direction = 0);

	//Получить индекс картинки (для анимации)
	WORD GetMountAnimation();

	virtual bool IsInternal() { return ((m_TiledataPtr->Flags & 0x00010000) && !IsCorpse()); }
	bool IsHuman() { return (IsCorpse() && (m_Count >= 0x0190) && (m_Count <= 0x0193)); }

	//Загрузка мульти в текущий объект
	void LoadMulti();
	//Получение объекта мульти в заданных координатах
	TMulti *GetMultiAtXY(short x, short y);

	//Найти объект внутри (рекурсивно) по типу с учетом (и без) цвета
	TGameItem *FindItem(WORD graphic, WORD color = 0xFFFF);
	//Найти объект в контейнере на указанном слое
	TGameItem *FindLayer(int layer);
};
//---------------------------------------------------------------------------
#endif