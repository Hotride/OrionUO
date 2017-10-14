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
class CIndexMap
{
	SETGET(uint, OriginalMapAddress, 0);
	SETGET(uint, OriginalStaticAddress, 0);
	SETGET(uint, OriginalStaticCount, 0);

	SETGET(uint, MapAddress, 0);
	SETGET(uint, StaticAddress, 0);
	SETGET(uint, StaticCount, 0);

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
	SETGET(uint, MaxBlockIndex, 0);
	SETGET(int, PatchesCount, 0);

protected:
	//!Вектор ссылок на блоки карты
	CMapBlock **m_Blocks{ NULL };

	MAP_INDEX_LIST m_BlockData[MAX_MAPS_COUNT];

	bool m_BlockAccessList[0x1000];

	void ResetPatchesInBlockTable();

public:
	CMapManager();
	virtual ~CMapManager();

	int m_MapPatchCount[MAX_MAPS_COUNT];
	int m_StaticPatchCount[MAX_MAPS_COUNT];

	CIndexMap *GetIndex(const uint &map, const int &blockX, const int &blockY);

	void CreateBlockTable(int map);

	void CreateBlocksTable();

	void ApplyPatches(WISP_DATASTREAM::CDataReader &stream);

	void UpdatePatched();

	void ClearBlockAccess();

	char CalculateNearZ(char defaultZ, const int &x, const int &y, const int &z);

	/*!
	Получить индекс текущей карты
	@return 
	*/
	int GetActualMap();

	void SetPatchedMapBlock(const uint &block, const uint &address);

	/*!
	Загрузить блок
	@param [__inout] block Ссылка на блок для загрузки
	@return 
	*/
	void LoadBlock(CMapBlock *block);

	/*!
	Получить блок для радара из муллов
	@param [__in] blockX Координата X блока
	@param [__in] blockY Координата Y блока
	@param [__out] mb Ссылка на блок
	@return 
	*/
	void GetRadarMapBlock(const int &blockX, const int &blockY, RADAR_MAP_BLOCK &mb);

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

	void ClearUsedBlocks();

	/*!
	Добавить объект рендера
	@param [__in] item Ссылка на объект
	@return 
	*/
	void AddRender(CRenderWorldObject *item);
};
//----------------------------------------------------------------------------------
extern CMapManager g_MapManager;
//----------------------------------------------------------------------------------
#endif
