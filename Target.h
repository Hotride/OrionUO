/****************************************************************************
**
** Target.h
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
#ifndef TargetH
#define TargetH
//--------------------------------------------------------------------------
//Класс для работы с таргетом
class TTarget
{
private:
	//Тип объекта прицела
	BYTE m_Type;

	//Тип прицела
	BYTE m_CursorType;

	//Серийник объекта, к которому привязан прицел
	DWORD m_CursorID;

	//Флаг состояния прицела
	bool m_Targeting;

	//Текущие и последний данные прицела
	BYTE m_Data[19];
	BYTE m_LastData[19];

	//Мульти на таргете
	WORD m_MultiGraphic;

	//Список объектов мульти на таргете
	TMulti *m_Multi;

	//Добавить мульти-объект
	void AddMultiObject(TMultiObject *obj);
public:
	TTarget();
	~TTarget() {}
	
	SETGET(BYTE, Type);
	SETGET(BYTE, CursorType);
	SETGET(WORD, MultiGraphic);

	//Установить данные прицела
	void SetData(PBYTE buf, int size);

	//Установить данные мульти-таргета
	void SetMultiData(PBYTE buf, int size);

	bool IsTargeting() const {return m_Targeting;}

	//Очистить таргет
	void Reset();

	//Послать таргет на объект
	void SendTargetObject(DWORD Serial);

	//Послать таргет на тайл
	void SendTargetTile(WORD tileID, WORD X, WORD Y, char Z);

	//Послать отмену таргета
	void SendCancelTarget();

	//Послать таргет на последнюю цель
	void SendLastTarget();

	//Послать таргет
	void SendTarget();

	//Загрузить мульти-объект
	void LoadMulti(int x, int y, char z);

	//Выгрузить мульти-объект
	void UnloadMulti();

	//Получить объект мульти в координатах
	TMulti *GetMultiAtXY(short x, short y);
};
//--------------------------------------------------------------------------
extern TTarget Target;
//--------------------------------------------------------------------------
#endif