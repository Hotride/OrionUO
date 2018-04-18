﻿/***********************************************************************************
**
** GameWorld.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GAMEWORLD_H
#define GAMEWORLD_H
//----------------------------------------------------------------------------------
typedef map<uint, CGameObject*> WORLD_MAP;
//----------------------------------------------------------------------------------
//!Класс игрового мира
class CGameWorld
{
public:
	uint ObjectToRemove = 0;

private:
	/*!
	Создать игрока
	@param [__in] serial Серийник игрока
	@return 
	*/
	void CreatePlayer(int serial);

	/*!
	Удалить игрока
	@return 
	*/
	void RemovePlayer();

public:
	CGameWorld(int serial);
	~CGameWorld();

	//!Предметы в памяти
	WORLD_MAP m_Map;

	//!Предметы в мире
	CGameObject *m_Items{ NULL };

	void ResetObjectHandlesState();

	/*!
	Обработка анимации всех персонажей
	@return 
	*/
	void ProcessAnimation();
	/*!
	Обработка звуков перемещения
	@return
	*/
	void ProcessSound(int ticks, CGameCharacter *gc);

	/*!
	Установить текущего чара с указанным серийником как основного
	@param [__in] serial Серийник нового игрока
	@return 
	*/
	void SetPlayer(int serial);

	/*!
	Создать (или взять, если уже существует) игровой предмет
	@param [__in] serial Серийник предмета
	@return Ссылка на предмет
	*/
	CGameItem *GetWorldItem(int serial);

	/*!
	Создать (или взять, если уже существует) игрового персонажа
	@param [__in] serial Серийник персонажа
	@return Ссылка на персонажа
	*/
	CGameCharacter *GetWorldCharacter(int serial);

	/*!
	Найти игровой объект в памяти
	@param [__in] serial Серийник объекта
	@return Ссылка на объект или NULL
	*/
	CGameObject *FindWorldObject(int serial);

	/*!
	Найти игровой предмет в памяти
	@param [__in] serial Серийник предмета
	@return Ссылка на предмет или NULL
	*/
	CGameItem *FindWorldItem(int serial);

	/*!
	Найти игрового персонажа в памяти
	@param [__in] serial Серийник персонажа
	@return Ссылка а персонажа или NULL
	*/
	CGameCharacter *FindWorldCharacter(int serial);

	void ReplaceObject(CGameObject *obj, int newSerial);

	/*!
	Удалить объект из памяти
	@param [__in] obj Ссылка на объект
	@return 
	*/
	void RemoveObject(CGameObject *obj);

	/*!
	Вынуть объект из контейнера
	@param [__in] obj Ссылка на объект
	@return 
	*/
	void RemoveFromContainer(CGameObject *obj);

	/*!
	Очистить указанный контейнер
	@param [__in] obj Ссылка на объект (контейнер)
	@return 
	*/
	void ClearContainer(CGameObject *obj);

	/*!
	Положить в контейнер
	@param [__in] obj Ссылка на объект
	@param [__in] containerSerial Серийник контейнера
	@return 
	*/
	void PutContainer(CGameObject *obj, int containerSerial)
	{
		CGameObject *cnt = FindWorldObject(containerSerial);
		if (cnt != NULL)
			PutContainer(obj, cnt);
	}

	/*!
	Положить в контейнер
	@param [__in] obj Ссылка на объект
	@param [__in] container Ссылка на контейнер
	@return 
	*/
	void PutContainer(CGameObject *obj, CGameObject *container);

	/*!
	Одеть предмет
	@param [__in] obj Ссылка на предмет
	@param [__in] containerSerial Серийник контейнера
	@param [__in] layer Слой, в который одеть предмет
	@return 
	*/
	void PutEquipment(CGameItem *obj, int containerSerial, int layer)
	{
		CGameObject *cnt = FindWorldObject(containerSerial);
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
	void PutEquipment(CGameItem *obj, CGameObject *container, int layer)
	{
		PutContainer(obj, container);
		obj->Layer = layer;
	}

	/*!
	Поднять объект вверх в очереди
	@param [__in] obj Ссылка на объект
	@return 
	*/
	void MoveToTop(CGameObject *obj);

	/*!
	Дамп предметов, хранящихся в памяти
	@param [__in] nCount Количество отступов
	@param [__in_opt] serial Серийник родителя
	@return 
	*/
	void Dump(uchar nCount = 0, uint serial = 0xFFFFFFFF);

	/*!
	Поиск объекта
	@param [__in] serialStart Начальный серийник для поиска
	@param [__in] scanDistance Дистанция поиска
	@param [__in] scanType Тип объектов поиска
	@param [__in] scanMode Режим поиска
	@return Ссылка на найденный объект или NULL
	*/
	CGameObject *SearchWorldObject(int serialStart, int scanDistance, SCAN_TYPE_OBJECT scanType, SCAN_MODE_OBJECT scanMode);

	void UpdateContainedItem(int serial, ushort graphic, uchar graphicIncrement, ushort count, int x, int y, int containerSerial, ushort color);

	void UpdateItemInContainer(CGameObject *obj, CGameObject *container, int x, int y);

	void UpdateGameObject(int serial, ushort graphic, uchar graphicIncrement, int count, int x, int y, char z, uchar direction, ushort color, uchar flags, int a11, UPDATE_GAME_OBJECT_TYPE updateType, ushort a13);

	void UpdatePlayer(int serial, ushort graphic, uchar graphicIncrement, ushort color, uchar flags, int x, int y, ushort serverID, uchar direction, char z);
 };
//---------------------------------------------------------------------------
//!Указатель на мир
extern CGameWorld *g_World;
//---------------------------------------------------------------------------
#endif