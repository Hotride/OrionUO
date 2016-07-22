/****************************************************************************
**
** MapManager.h
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
#ifndef MapManagerH
#define MapManagerH

//Использовать вектор блоков или очередь
#define USE_BLOCK_MAP 1
//--------------------------------------------------------------------------
//!Класс менеджера карт
class TMapManager : public TBaseQueue
{
private:
#if USE_BLOCK_MAP == 1
	//!Вектор ссылок на блоки карты
	TMapBlock **m_Blocks;
#endif

	//!Максимальный индекс блока для текущей карты
	DWORD m_MaxBlockIndex;

public:
	TMapManager();
	virtual ~TMapManager();

	/*!
	Получить индекс текущей карты
	@return 
	*/
	virtual int GetActualMap();

	/*!
	Загрузить блок
	@param [__inout] block Ссылка на блок для загрузки
	@return 
	*/
	virtual void LoadBlock(__inout TMapBlock *block);

	/*!
	Получить блок для радара из муллов
	@param [__in] blockX Координата X блока
	@param [__in] blockY Координата Y блока
	@param [__out] mb Ссылка на блок
	@return 
	*/
	virtual void GetRadarMapBlock(__in int blockX, __in int blockY, __out MAP_BLOCK &mb);

	/*!
	Получить блок карты напрямую из мулов
	@param [__in] map Индекс карты
	@param [__in] blockX Координата X блока
	@param [__in] blockY Координата Y блока
	@param [__out] mb Ссылка на блок
	@return Код ошибки (0 - успешно)
	*/
	virtual int GetWorldMapBlock(__in int &map, __in int &blockX, __in int &blockY, __out MAP_BLOCK &mb);

	/*!
	Получить значение Z координаты для указанной точки в мире
	@param [__in] x Координата X
	@param [__in] y Координата Y
	@param [__out] groundZ Значение Z коррдинаты земли
	@param [__out] staticZ Значение Z коррдинаты статики
	@return 
	*/
	void GetMapZ(__in int x, __in int y, __out int &groundZ, __out int &staticZ);

	/*!
	Инициализация
	@param [__in_opt] delayed По истечении времени на загрузку выходить из цикла
	@return 
	*/
	void Init(__in_opt bool delayed = false);

	/*!
	Получить ссылку на блок
	@param [__in] index Индекс блока
	@return Ссылка на блок или NULL
	*/
	TMapBlock *GetBlock(__in DWORD index);

	/*!
	Добавить блок
	@param [__in] index Индекс блока
	@return Ссылка на блок или NULL
	*/
	TMapBlock *AddBlock(__in DWORD index);

	/*!
	Удалить блок
	@param [__in] index Индекс блока
	@return 
	*/
	void DeleteBlock(__in DWORD index);

	/*!
	Удалить неиспользуемые блоки
	@return 
	*/
	void ClearUnusedBlocks();

	/*!
	Добавить объект рендера
	@param [__in] item Ссылка на объект
	@return 
	*/
	void AddRender(__in TRenderWorldObject *item);

};
//--------------------------------------------------------------------------
//!Класс менеджера карт
class TUopMapManager : public TMapManager
{
public:
	TUopMapManager();
	virtual ~TUopMapManager();

	/*!
	Получить индекс текущей карты
	@return
	*/
	virtual int GetActualMap();

	/*!
	Загрузить блок
	@param [__inout] block Ссылка на блок для загрузки
	@return
	*/
	virtual void LoadBlock(__inout TMapBlock *block);

	/*!
	Получить блок карты напрямую из мулов
	@param [__in] map Индекс карты
	@param [__in] blockX Координата X блока
	@param [__in] blockY Координата Y блока
	@param [__out] mb Ссылка на блок
	@return Код ошибки (0 - успешно)
	*/
	virtual int GetWorldMapBlock(__in int &map, __in int &blockX, __in int &blockY, __out MAP_BLOCK &mb);

	/*!
	Получить блок для радара из муллов
	@param [__in] blockX Координата X блока
	@param [__in] blockY Координата Y блока
	@param [__out] mb Ссылка на блок
	@return
	*/
	virtual void GetRadarMapBlock(__in int blockX, __in int blockY, __out MAP_BLOCK &mb);
};
//--------------------------------------------------------------------------
extern TMapManager *MapManager;
//--------------------------------------------------------------------------
#endif