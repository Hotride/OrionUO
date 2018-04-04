/***********************************************************************************
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
	SETGET(uint, ObjectToRemove, 0);

private:
	/*!
	Создать игрока
	@param [__in] serial Серийник игрока
	@return 
	*/
	void CreatePlayer(const uint &serial);

	/*!
	Удалить игрока
	@return 
	*/
	void RemovePlayer();

public:
	CGameWorld(const uint &serial);
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
	void ProcessSound(const uint &ticks, CGameCharacter *gc);

	/*!
	Установить текущего чара с указанным серийником как основного
	@param [__in] serial Серийник нового игрока
	@return 
	*/
	void SetPlayer(const uint &serial);

	/*!
	Создать (или взять, если уже существует) игровой предмет
	@param [__in] serial Серийник предмета
	@return Ссылка на предмет
	*/
	CGameItem *GetWorldItem(const uint &serial);

	/*!
	Создать (или взять, если уже существует) игрового персонажа
	@param [__in] serial Серийник персонажа
	@return Ссылка на персонажа
	*/
	CGameCharacter *GetWorldCharacter(const uint &serial);

	/*!
	Найти игровой объект в памяти
	@param [__in] serial Серийник объекта
	@return Ссылка на объект или NULL
	*/
	CGameObject *FindWorldObject(const uint &serial);

	/*!
	Найти игровой предмет в памяти
	@param [__in] serial Серийник предмета
	@return Ссылка на предмет или NULL
	*/
	CGameItem *FindWorldItem(const uint &serial);

	/*!
	Найти игрового персонажа в памяти
	@param [__in] serial Серийник персонажа
	@return Ссылка а персонажа или NULL
	*/
	CGameCharacter *FindWorldCharacter(const uint &serial);

	void ReplaceObject(CGameObject *obj, const uint &newSerial);

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
	void PutContainer(CGameObject *obj, const uint &containerSerial)
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
	void PutEquipment(CGameItem *obj, const uint &containerSerial, const int &layer)
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
	void PutEquipment(CGameItem *obj, CGameObject *container, const int &layer)
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
	CGameObject *SearchWorldObject(const uint &serialStart, const int &scanDistance, const SCAN_TYPE_OBJECT &scanType, const SCAN_MODE_OBJECT &scanMode);

	void UpdateContainedItem(const uint &serial, const ushort &graphic, const uchar &graphicIncrement, ushort count, const int &x, const int &y, const uint &containerSerial, const ushort &color);

	void UpdateItemInContainer(CGameObject *obj, CGameObject *container, const int &x, const int &y);

	void UpdateGameObject(const uint &serial, ushort graphic, const uchar &graphicIncrement, int count, const int &x, const int &y, const char &z, const uchar &direction, const ushort &color, const uchar &flags, const int &a11, const UPDATE_GAME_OBJECT_TYPE &updateType, const ushort &a13);

	void UpdatePlayer(const uint &serial, const ushort &graphic, const uchar &graphicIncrement, const ushort &color, const uchar &flags, const int &x, const int &y, const ushort &serverID, const uchar &direction, const char &z);
 };
//---------------------------------------------------------------------------
//!Указатель на мир
extern CGameWorld *g_World;
//---------------------------------------------------------------------------
#endif