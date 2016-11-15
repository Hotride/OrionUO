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
#include "../BaseQueue.h"
#include "../Game objects/MapBlock.h"
#include "../MulStruct.h"
//----------------------------------------------------------------------------------
class CIndexMap
{
	SETGET(uint, MapAddress);
	SETGET(uint, StaticAddress);
	SETGET(uint, StaticCount);
	SETGET(bool, MapPatched);
	SETGET(bool, StaticPatched);

public:
	CIndexMap();
	virtual ~CIndexMap();
};
//----------------------------------------------------------------------------------
typedef vector<CIndexMap> MAP_INDEX_LIST;
//----------------------------------------------------------------------------------
//!Класс менеджера карт
class CMapManager : public CBaseQueue
{
	//!Максимальный индекс блока для текущей карты
	SETGET(uint, MaxBlockIndex);

private:
	//!Вектор ссылок на блоки карты
	CMapBlock **m_Blocks;

	MAP_INDEX_LIST m_BlockData[MAX_MAPS_COUNT];

public:
	CMapManager();
	virtual ~CMapManager();

	CIndexMap *GetIndex(const int &map, const int &blockX, const int &blockY);

	void CreateBlockTable(int map);

	void CreateBlocksTable();

	void ApplyMapPatches();

	/*!
	Получить индекс текущей карты
	@return 
	*/
	virtual int GetActualMap();

	void SetPatchedMapBlock(const uint &block, const uint &address);

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
	virtual void GetWorldMapBlock(const int &map, const int &blockX, const int &blockY, MAP_BLOCK &mb);

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
	virtual void GetWorldMapBlock(const int &map, const int &blockX, const int &blockY, MAP_BLOCK &mb);

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
