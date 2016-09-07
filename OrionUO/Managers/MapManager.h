/***********************************************************************************
**
** MapManager.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef MAPMANAGER_H
#define MAPMANAGER_H
//----------------------------------------------------------------------------------
//Использовать вектор блоков или очередь
#define USE_BLOCK_MAP 1
//----------------------------------------------------------------------------------
#include "../BaseQueue.h"
#include "../Game objects/MapBlock.h"
#include "../MulStruct.h"
//----------------------------------------------------------------------------------
//!Класс менеджера карт
class CMapManager : public CBaseQueue
{
private:
#if USE_BLOCK_MAP == 1
	//!Вектор ссылок на блоки карты
	CMapBlock **m_Blocks;
#endif

	//!Максимальный индекс блока для текущей карты
	uint m_MaxBlockIndex;

public:
	CMapManager();
	virtual ~CMapManager();

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
	virtual void LoadBlock(CMapBlock *block);

	/*!
	Получить блок для радара из муллов
	@param [__in] blockX Координата X блока
	@param [__in] blockY Координата Y блока
	@param [__out] mb Ссылка на блок
	@return 
	*/
	virtual void GetRadarMapBlock(const int &blockX, const int &blockY, MAP_BLOCK &mb);

	/*!
	Получить блок карты напрямую из мулов
	@param [__in] map Индекс карты
	@param [__in] blockX Координата X блока
	@param [__in] blockY Координата Y блока
	@param [__out] mb Ссылка на блок
	@return Код ошибки (0 - успешно)
	*/
	virtual int GetWorldMapBlock(const int &map, const int &blockX, const int &blockY, MAP_BLOCK &mb);

	/*!
	Получить значение Z координаты для указанной точки в мире
	@param [__in] x Координата X
	@param [__in] y Координата Y
	@param [__out] groundZ Значение Z коррдинаты земли
	@param [__out] staticZ Значение Z коррдинаты статики
	@return 
	*/
	void GetMapZ(const int &x, const int &y, int &groundZ, int &staticZ);

	/*!
	Инициализация
	@param [__in_opt] delayed По истечении времени на загрузку выходить из цикла
	@return 
	*/
	void Init(const bool &delayed = false);

	/*!
	Получить ссылку на блок
	@param [__in] index Индекс блока
	@return Ссылка на блок или NULL
	*/
	CMapBlock *GetBlock(const uint &index);

	/*!
	Добавить блок
	@param [__in] index Индекс блока
	@return Ссылка на блок или NULL
	*/
	CMapBlock *AddBlock(const uint &index);

	/*!
	Удалить блок
	@param [__in] index Индекс блока
	@return 
	*/
	void DeleteBlock(const uint &index);

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
	void AddRender(CRenderWorldObject *item);

};
//----------------------------------------------------------------------------------
//!Класс менеджера карт
class CUopMapManager : public CMapManager
{
public:
	CUopMapManager();
	virtual ~CUopMapManager();

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
	virtual void LoadBlock(CMapBlock *block);

	/*!
	Получить блок карты напрямую из мулов
	@param [__in] map Индекс карты
	@param [__in] blockX Координата X блока
	@param [__in] blockY Координата Y блока
	@param [__out] mb Ссылка на блок
	@return Код ошибки (0 - успешно)
	*/
	virtual int GetWorldMapBlock(const int &map, const int &blockX, const int &blockY, MAP_BLOCK &mb);

	/*!
	Получить блок для радара из муллов
	@param [__in] blockX Координата X блока
	@param [__in] blockY Координата Y блока
	@param [__out] mb Ссылка на блок
	@return
	*/
	virtual void GetRadarMapBlock(const int &blockX, const int &blockY, MAP_BLOCK &mb);
};
//----------------------------------------------------------------------------------
extern CMapManager *g_MapManager;
//----------------------------------------------------------------------------------
#endif
