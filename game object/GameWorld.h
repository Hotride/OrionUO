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
//!Класс игрового мира
class TGameWorld
{
private:
	/*!
	Создать игрока
	@param [__in] serial Серийник игрока
	@return 
	*/
	void CreatePlayer(__in DWORD serial);

	/*!
	Удалить игрока
	@return 
	*/
	void RemovePlayer();

public:
	TGameWorld(DWORD serial);
	~TGameWorld();

	//!Предметы в памяти
	WORLD_MAP m_Map;

	//!Предметы в мире
	TGameObject *m_Items;

	/*!
	Обработка анимации всех персонажей
	@return 
	*/
	void ProcessAnimation();

	/*!
	Установить текущего чара с указанным серийником как основного
	@param [__in] serial Серийник нового игрока
	@return 
	*/
	void SetPlayer(__in DWORD serial);

	/*!
	Создать (или взять, если уже существует) игровой предмет
	@param [__in] serial Серийник предмета
	@return Ссылка на предмет
	*/
	TGameItem *GetWorldItem(__in DWORD serial);

	/*!
	Создать (или взять, если уже существует) игрового персонажа
	@param [__in] serial Серийник персонажа
	@return Ссылка на персонажа
	*/
	TGameCharacter *GetWorldCharacter(__in DWORD serial);

	/*!
	Найти игровой объект в памяти
	@param [__in] serial Серийник объекта
	@return Ссылка на объект или NULL
	*/
	TGameObject *FindWorldObject(__in DWORD serial);

	/*!
	Найти игровой предмет в памяти
	@param [__in] serial Серийник предмета
	@return Ссылка на предмет или NULL
	*/
	TGameItem *FindWorldItem(__in DWORD serial);

	/*!
	Найти игрового персонажа в памяти
	@param [__in] serial Серийник персонажа
	@return Ссылка а персонажа или NULL
	*/
	TGameCharacter *FindWorldCharacter(__in DWORD serial);

	/*!
	Удалить объект из памяти
	@param [__in] obj Ссылка на объект
	@return 
	*/
	void RemoveObject(__in TGameObject *obj);

	/*!
	Вынуть объект из контейнера
	@param [__in] obj Ссылка на объект
	@return 
	*/
	void RemoveFromContainer(__in TGameObject *obj);

	/*!
	Очистить указанный контейнер
	@param [__in] obj Ссылка на объект (контейнер)
	@return 
	*/
	void ClearContainer(__in TGameObject *obj);

	/*!
	Положить в контейнер
	@param [__in] obj Ссылка на объект
	@param [__in] containerSerial Серийник контейнера
	@return 
	*/
	void PutContainer( __in TGameObject *obj, __in DWORD containerSerial)
	{
		TGameObject *cnt = FindWorldObject(containerSerial);
		if (cnt != NULL)
			PutContainer(obj, cnt);
	}

	/*!
	Положить в контейнер
	@param [__in] obj Ссылка на объект
	@param [__in] container Ссылка на контейнер
	@return 
	*/
	void PutContainer(__in TGameObject *obj, __in TGameObject *container);

	/*!
	Одеть предмет
	@param [__in] obj Ссылка на предмет
	@param [__in] containerSerial Серийник контейнера
	@param [__in] layer Слой, в который одеть предмет
	@return 
	*/
	void PutEquipment( __in TGameItem *obj, __in DWORD containerSerial, __in int layer)
	{
		TGameObject *cnt = FindWorldObject(containerSerial);
		if (cnt != NULL)
			PutEquipment(obj, cnt, layer);
	}

	/*!
	Одеть предмет
	@param [__in] obj Ссылка на объект
	@param [__in] container Ссылка на контейнер
	@param [__in] layer Слой, в который одеть предмет
	@return 
	*/
	void PutEquipment( __in TGameItem *obj, __in TGameObject *container, __in int layer)
	{
		PutContainer(obj, container);
		obj->Layer = layer;
	}

	/*!
	Поднять объект вверх в очереди
	@param [__in] obj Ссылка на объект
	@return 
	*/
	void MoveToTop(__in TGameObject *obj);

	/*!
	Дамп предметов, хранящихся в памяти
	@param [__in] nCount Количество отступов
	@param [__in_opt] serial Серийник родителя
	@return 
	*/
	void Dump(__in BYTE nCount = 0, __in_opt DWORD serial = 0xFFFFFFFF);

	/*!
	Поиск объекта
	@param [__in] serialStart Начальный серийник для поиска
	@param [__in] scanDistance Дистанция поиска
	@param [__in] scanType Тип объектов поиска
	@param [__in] scanMode Режим поиска
	@return Ссылка на найденный объект или NULL
	*/
	TGameObject *SearchWorldObject(__in DWORD serialStart, __in int scanDistance, __in SCAN_TYPE_OBJECT scanType, __in SCAN_MODE_OBJECT scanMode);
 };
//---------------------------------------------------------------------------
//!Указатель на мир
extern TGameWorld *World;
//---------------------------------------------------------------------------
#endif