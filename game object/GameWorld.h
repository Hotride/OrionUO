/****************************************************************************
**
** GameWorld.h
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
#ifndef GameWorldH
#define GameWorldH
//---------------------------------------------------------------------------
typedef std::map<DWORD, TGameObject*> WORLD_MAP;
//---------------------------------------------------------------------------
class TGameWorld
{
private:
	//Создать игрока
	void CreatePlayer(DWORD serial);
	//Удалить игрока
	void RemovePlayer();
public:
	TGameWorld(DWORD serial);
	~TGameWorld();

	//Предметы в памяти
	WORLD_MAP m_Map;

	//Предметы в мире
	TGameObject *m_Items;

	//Обработка анимации всех персонажей
	void ProcessAnimation();

	//Установить текущего чара с указанным серийником как основного
	void SetPlayer(DWORD serial);

	//Создать (или взять, если уже существует) игровой предмет
	TGameItem *GetWorldItem(DWORD serial);
	//Создать (или взять, если уже существует) игрового персонажа
	TGameCharacter *GetWorldCharacter(DWORD serial);
	//Найти игровой объект в памяти (NULL если нет)
	TGameObject *FindWorldObject(DWORD serial);
	//Найти игровой предмет в памяти (NULL если нет)
	TGameItem *FindWorldItem(DWORD serial);
	//Найти игрового персонажа в памяти (NULL если нет)
	TGameCharacter *FindWorldCharacter(DWORD serial);
	//Найти игрового персонажа (владельца трупа) в памяти (NULL если нет)
	TGameCharacter *FindWorldCorpseOwner(DWORD serial);

	//Удалить объект из памяти
	void RemoveObject(TGameObject *obj);
	//Вынуть объект из контейнера
	void RemoveFromContainer(TGameObject *obj);
	//Очистить указанный контейнер
	void ClearContainer(TGameObject *obj);
	//Положить в контейнер
	void PutContainer(TGameObject *obj, DWORD container_serial)
	{
		TGameObject *cnt = FindWorldObject(container_serial);
		if (cnt != NULL)
			PutContainer(obj, cnt);
	}
	//Положить в контейнер
	void PutContainer(TGameObject *obj, TGameObject *container);
	//Одеть предмет
	void PutEquipment(TGameItem *obj, DWORD container_serial, int layer)
	{
		TGameObject *cnt = FindWorldObject(container_serial);
		if (cnt != NULL)
			PutEquipment(obj, cnt, layer);
	}
	//Одеть предмет
	void PutEquipment(TGameItem *obj, TGameObject *container, int layer)
	{
		PutContainer(obj, container);
		obj->Layer = layer;
	}

	//Поднять предмет вверх в очереди
	void MoveToTop(TGameObject *obj);

	//Дамп предметов, хранящихся в памяти
	void Dump(BYTE nCount = 0, DWORD serial = 0xFFFFFFFF);

	//Поиск объекта
	TGameObject *SearchWorldObject(DWORD serialStart, int scanDistance, SCAN_TYPE_OBJECT scanType, SCAN_MODE_OBJECT scanMode);
};
//---------------------------------------------------------------------------
extern TGameWorld *World; //Указатель на мир
//---------------------------------------------------------------------------
#endif