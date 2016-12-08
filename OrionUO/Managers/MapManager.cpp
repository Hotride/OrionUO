/***********************************************************************************
**
** MapManager.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "MapManager.h"
#include "FileManager.h"
#include "../Constants.h"
#include "../Game objects/StaticObject.h"
#include "../Game objects/GamePlayer.h"
#include "GumpManager.h"
#include "../Gumps/GumpMinimap.h"
//----------------------------------------------------------------------------------
CMapManager *g_MapManager = NULL;
//----------------------------------------------------------------------------------
CIndexMap::CIndexMap()
: m_OriginalMapAddress(0), m_OriginalStaticAddress(0), m_OriginalStaticCount(0),
m_MapAddress(0), m_StaticAddress(0), m_StaticCount(0)
{
}
//----------------------------------------------------------------------------------
CIndexMap::~CIndexMap()
{
}
//----------------------------------------------------------------------------------
CMapManager::CMapManager()
: CBaseQueue(), m_MaxBlockIndex(0), m_Blocks(NULL)
{
}
//----------------------------------------------------------------------------------
CMapManager::~CMapManager()
{
	if (m_Blocks != NULL)
	{
		ClearUsedBlocks();

		delete[] m_Blocks;
		m_Blocks = NULL;
	}

	m_MaxBlockIndex = 0;
}
//----------------------------------------------------------------------------------
void CMapManager::CreateBlocksTable()
{
	IFOR(map, 0, MAX_MAPS_COUNT)
		CreateBlockTable(map);
}
//----------------------------------------------------------------------------------
void CMapManager::CreateBlockTable(int map)
{
	MAP_INDEX_LIST &list = m_BlockData[map];
	WISP_GEOMETRY::CSize &size = g_MapBlockSize[map];

	int maxBlockCount = size.Width * size.Height;

	//Return and error notification?
	if (maxBlockCount < 1)
		return;

	list.resize(maxBlockCount);

	uint mapAddress = (uint)g_FileManager.m_MapMul[map].Start;
	uint endMapAddress = mapAddress + g_FileManager.m_MapMul[map].Size;

	uint staticIdxAddress = (uint)g_FileManager.m_StaticIdx[map].Start;
	uint endStaticIdxAddress = staticIdxAddress + g_FileManager.m_StaticIdx[map].Size;

	uint staticAddress = (uint)g_FileManager.m_StaticMul[map].Start;
	uint endStaticAddress = staticAddress + g_FileManager.m_StaticMul[map].Size;

	if (!mapAddress || !staticIdxAddress || !staticAddress)
		return;

	IFOR(block, 0, maxBlockCount)
	{
		CIndexMap &index = list[block];

		uint realMapAddress = 0;
		uint realStaticAddress = 0;
		int realStaticCount = 0;

		if (mapAddress != 0)
		{
			uint address = mapAddress + (block * sizeof(MAP_BLOCK));

			if (address < endMapAddress)
				realMapAddress = address;
		}

		if (staticIdxAddress != 0 && staticAddress != 0)
		{
			PSTAIDX_BLOCK sidx = (PSTAIDX_BLOCK)(staticIdxAddress + (block * sizeof(STAIDX_BLOCK)));

			if (sidx->Size > 0 && sidx->Position != 0xFFFFFFFF && (uint)sidx < endStaticIdxAddress)
			{
				uint address = staticAddress + sidx->Position;

				if (address < endStaticAddress)
				{
					realStaticAddress = address;
					realStaticCount = sidx->Size / sizeof(STATICS_BLOCK);

					if (realStaticCount > 0)
					{
						if (realStaticCount > 1024)
							realStaticCount = 1024;
					}
				}
			}
		}

		if (!realStaticCount)
			realStaticCount = 0;

		index.OriginalMapAddress = realMapAddress;
		index.OriginalStaticAddress = realStaticAddress;
		index.OriginalStaticCount = realStaticCount;

		index.MapAddress = realMapAddress;
		index.StaticAddress = realStaticAddress;
		index.StaticCount = realStaticCount;
	}
}
//----------------------------------------------------------------------------------
void CMapManager::SetPatchedMapBlock(const uint &block, const uint &address)
{
	MAP_INDEX_LIST &list = m_BlockData[0];
	WISP_GEOMETRY::CSize &size = g_MapBlockSize[0];

	int maxBlockCount = size.Width * size.Height;

	if (maxBlockCount < 1)
		return;

	list[block].OriginalMapAddress = address;
	list[block].MapAddress = address;
}
//----------------------------------------------------------------------------------
void CMapManager::ResetPatchesInBlockTable()
{
	IFOR(map, 0, MAX_MAPS_COUNT)
	{
		MAP_INDEX_LIST &list = m_BlockData[map];
		WISP_GEOMETRY::CSize &size = g_MapBlockSize[map];

		int maxBlockCount = size.Width * size.Height;

		//Return and error notification?
		if (maxBlockCount < 1)
			return;

		if (g_FileManager.m_MapMul[map].Start == NULL || g_FileManager.m_StaticIdx[map].Start == NULL || g_FileManager.m_StaticMul[map].Start == NULL)
			return;

		IFOR(block, 0, maxBlockCount)
		{
			CIndexMap &index = list[block];

			index.MapAddress = index.OriginalMapAddress;
			index.StaticAddress = index.OriginalStaticAddress;
			index.StaticCount = index.OriginalStaticCount;
		}
	}
}
//----------------------------------------------------------------------------------
void CMapManager::ApplyPatches(WISP_DATASTREAM::CDataReader &stream)
{
	ResetPatchesInBlockTable();

	int count = stream.ReadUInt32BE();

	if (count < 0)
		count = 0;

	if (count > MAX_MAPS_COUNT)
		count = MAX_MAPS_COUNT;

	IFOR(i, 0, count)
	{
		int mapPatchesCount = stream.ReadUInt32BE();
		int staticsPatchesCount = stream.ReadUInt32BE();

		MAP_INDEX_LIST &list = m_BlockData[i];
		WISP_GEOMETRY::CSize &size = g_MapBlockSize[i];

		uint maxBlockCount = size.Height * size.Width;

		if (mapPatchesCount)
		{
			WISP_FILE::CMappedFile &difl = g_FileManager.m_MapDifl[i];
			WISP_FILE::CMappedFile &dif = g_FileManager.m_MapDif[i];

			mapPatchesCount = min(mapPatchesCount, difl.Size / 4);

			difl.ResetPtr();
			dif.ResetPtr();

			IFOR(j, 0, mapPatchesCount)
			{
				uint blockIndex = difl.ReadUInt32LE();

				if (blockIndex < maxBlockCount)
					list[blockIndex].MapAddress = (uint)dif.Ptr;

				dif.Move(sizeof(MAP_BLOCK));
			}
		}

		if (staticsPatchesCount)
		{
			WISP_FILE::CMappedFile &difl = g_FileManager.m_StaDifl[i];
			WISP_FILE::CMappedFile &difi = g_FileManager.m_StaDifi[i];
			uint startAddress = (uint)g_FileManager.m_StaDif[i].Start;

			staticsPatchesCount = min(staticsPatchesCount, difl.Size / 4);

			difl.ResetPtr();
			difi.ResetPtr();

			IFOR(j, 0, staticsPatchesCount)
			{
				uint blockIndex = difl.ReadUInt32LE();

				PSTAIDX_BLOCK sidx = (PSTAIDX_BLOCK)difi.Ptr;

				difi.Move(sizeof(STAIDX_BLOCK));

				if (blockIndex < maxBlockCount)
				{
					uint realStaticAddress = 0;
					int realStaticCount = 0;

					if (sidx->Size > 0 && sidx->Position != 0xFFFFFFFF)
					{
						realStaticAddress = startAddress + sidx->Position;
						realStaticCount = sidx->Size / sizeof(STATICS_BLOCK);

						if (realStaticCount > 0)
						{
							if (realStaticCount > 1024)
								realStaticCount = 1024;
						}
					}

					list[blockIndex].StaticAddress = realStaticAddress;
					list[blockIndex].StaticCount = realStaticCount;
				}
			}
		}
	}

	UpdatePatched();
}
//----------------------------------------------------------------------------------
void CMapManager::UpdatePatched()
{
	if (g_Player == NULL)
		return;

	deque<CRenderWorldObject*> objectsList;

	if (m_Blocks != NULL)
	{
		QFOR(block, m_Items, CMapBlock*)
		{
			IFOR(x, 0, 8)
			{
				IFOR(y, 0, 8)
				{
					for (CRenderWorldObject *item = block->GetRender(x, y); item != NULL; item = item->m_NextXY)
					{
						if (!item->IsLandObject() && !item->IsStaticObject())
							objectsList.push_back(item);
					}
				}
			}
		}
	}

	Init(false);

	for (CRenderWorldObject *item : objectsList)
		AddRender(item);

	CGumpMinimap *gump = (CGumpMinimap*)g_GumpManager.UpdateGump(g_PlayerSerial, 0, GT_MINIMAP);

	if (gump != NULL)
		gump->LastX = 0;
}
//----------------------------------------------------------------------------------
CIndexMap *CMapManager::GetIndex(const int &map, const int &blockX, const int &blockY)
{
	int block = (blockX * g_MapBlockSize[map].Height) + blockY;
	MAP_INDEX_LIST &list = m_BlockData[map];

	if (block >= (int)list.size())
		return NULL;

	return &list[block];
}
//----------------------------------------------------------------------------------
/*!
Получить блок карты напрямую из мулов
@param [__in] map Индекс карты
@param [__in] blockX Координата X блока
@param [__in] blockY Координата Y блока
@param [__out] mb Ссылка на блок
@return Код ошибки (0 - успешно)
*/
void CMapManager::GetWorldMapBlock(const int &map, const int &blockX, const int &blockY, MAP_BLOCK &mb)
{
	CIndexMap *indexMap = GetIndex(map, blockX, blockY);

	if (indexMap == NULL || indexMap->MapAddress == 0)
		return;

	PMAP_BLOCK pmb = (PMAP_BLOCK)indexMap->MapAddress;

	IFOR(x, 0, 8)
	{
		IFOR(y, 0, 8)
		{
			int pos = (y * 8) + x;
			mb.Cells[pos].TileID = pmb->Cells[pos].TileID;
			mb.Cells[pos].Z = pmb->Cells[pos].Z;
		}
	}

	PSTATICS_BLOCK sb = (PSTATICS_BLOCK)indexMap->StaticAddress;

	if (sb != NULL)
	{
		int count = indexMap->StaticCount;

		IFOR(c, 0, count)
		{
			if (sb->Color && sb->Color != 0xFFFF)
			{
				int pos = (sb->Y * 8) + sb->X;
				//if (pos > 64) continue;

				if (mb.Cells[pos].Z <= sb->Z)
				{
					mb.Cells[pos].TileID = sb->Color + 0x4000;
					mb.Cells[pos].Z = sb->Z;
				}
			}

			sb++;
		}
	}
}
//----------------------------------------------------------------------------------
/*!
Получить блок для радара из муллов
@param [__in] blockX Координата X блока
@param [__in] blockY Координата Y блока
@param [__out] mb Ссылка на блок
@return 
*/
void CMapManager::GetRadarMapBlock(const int &blockX, const int &blockY, MAP_BLOCK &mb)
{
	CIndexMap *indexMap = GetIndex(GetActualMap(), blockX, blockY);

	if (indexMap == NULL || indexMap->MapAddress == 0)
		return;

	PMAP_BLOCK pmb = (PMAP_BLOCK)indexMap->MapAddress;
	
	IFOR(x, 0, 8)
	{
		IFOR(y, 0, 8)
		{
			int pos = (y * 8) + x;
			mb.Cells[pos].TileID = pmb->Cells[pos].TileID;
			mb.Cells[pos].Z = pmb->Cells[pos].Z;
		}
	}

	PSTATICS_BLOCK sb = (PSTATICS_BLOCK)indexMap->StaticAddress;

	if (sb != NULL)
	{
		int count = indexMap->StaticCount;

		IFOR(c, 0, count)
		{
			if (sb->Color && sb->Color != 0xFFFF)
			{
				int pos = (sb->Y * 8) + sb->X;
				//if (pos > 64) continue;

				if (mb.Cells[pos].Z <= sb->Z)
				{
					mb.Cells[pos].TileID = sb->Color + 0x4000;
					mb.Cells[pos].Z = sb->Z;
				}
			}

			sb++;
		}
	}
}
//----------------------------------------------------------------------------------
/*!
Получить значение Z координаты для указанной точки в мире
@param [__in] x Координата X
@param [__in] y Координата Y
@param [__out] groundZ Значение Z коррдинаты земли
@param [__out] staticZ Значение Z коррдинаты статики
@return 
*/
void CMapManager::GetMapZ(const int &x, const int &y, int &groundZ, int &staticZ)
{
	int blockX = x / 8;
	int blockY = y / 8;
	uint index = (blockX * g_MapBlockSize[g_CurrentMap].Height) + blockY;

	if (index < m_MaxBlockIndex)
	{
		CMapBlock *block = GetBlock(index);

		if (block == NULL)
		{
			block = AddBlock(index);
			block->X = blockX;
			block->Y = blockY;
			LoadBlock(block);
		}

		CMapObject *item = block->Block[x % 8][y % 8];

		while (item != NULL)
		{
			if (item->IsLandObject())
				groundZ = item->Z;
			else if (staticZ < item->Z)
				staticZ = item->Z;

			item = (CMapObject*)item->m_Next;
		}
	}
}
//----------------------------------------------------------------------------------
/*!
Удалить неиспользуемые блоки
@return 
*/
void CMapManager::ClearUnusedBlocks()
{
	CMapBlock *block = (CMapBlock*)m_Items;
	uint ticks = g_Ticks - CLEAR_TEXTURES_DELAY;

	while (block != NULL)
	{
		CMapBlock *next = (CMapBlock*)block->m_Next;

		if (block->LastAccessTime < ticks && block->HasNoExternalData())
		{
			uint index = block->Index;
			Delete(block);

			m_Blocks[index] = NULL;
		}

		block = next;
	}
}
//----------------------------------------------------------------------------------
void CMapManager::ClearUsedBlocks()
{
	CMapBlock *block = (CMapBlock*)m_Items;

	while (block != NULL)
	{
		CMapBlock *next = (CMapBlock*)block->m_Next;

		uint index = block->Index;
		Delete(block);

		m_Blocks[index] = NULL;

		block = next;
	}
}
//----------------------------------------------------------------------------------
/*!
Инициализация
@param [__in_opt] delayed По истечении времени на загрузку выходить из цикла
@return 
*/
void CMapManager::Init(const bool &delayed)
{
	if (g_Player == NULL)
		return;

	int map = GetActualMap();

	if (!delayed)
	{
		if (m_Blocks != NULL)
		{
			ClearUsedBlocks();

			delete[] m_Blocks;
			m_Blocks = NULL;
		}

		m_MaxBlockIndex = g_MapBlockSize[map].Width * g_MapBlockSize[map].Height;
		m_Blocks = new CMapBlock*[m_MaxBlockIndex];
		memset(&m_Blocks[0], 0, sizeof(CMapBlock*) * m_MaxBlockIndex);
	}
	
	const int XY_Offset = 30; //70;

	int minBlockX = (g_Player->X - XY_Offset) / 8 - 1;
	int minBlockY = (g_Player->Y - XY_Offset) / 8 - 1;
	int maxBlockX = ((g_Player->X + XY_Offset) / 8) + 1;
	int maxBlockY = ((g_Player->Y + XY_Offset) / 8) + 1;

	uint ticks = g_Ticks;
	uint maxDelay = g_FrameDelay[1] / 2;

	for (int i = minBlockX; i <= maxBlockX; i++)
	{
		if (i < 0 || i >= g_MapBlockSize[map].Width)
			continue;

		for (int j = minBlockY; j <= maxBlockY; j++)
		{
			if (j < 0 || j >= g_MapBlockSize[map].Height)
				continue;

			uint index = (i * g_MapBlockSize[map].Height) + j;

			if (index < m_MaxBlockIndex)
			{
				CMapBlock *block = GetBlock(index);

				if (block == NULL)
				{
					if (delayed && g_Ticks - ticks >= maxDelay)
						return;

					block = AddBlock(index);
					block->X = i;
					block->Y = j;
					LoadBlock(block);
				}
			}
		}
	}
}
//----------------------------------------------------------------------------------
/*!
Загрузить блок
@param [__inout] block Ссылка на блок для загрузки
@return 
*/
void CMapManager::LoadBlock(CMapBlock *block)
{
	int map = GetActualMap();

	CIndexMap *indexMap = GetIndex(GetActualMap(), block->X, block->Y);

	if (indexMap == NULL || indexMap->MapAddress == 0)
		return;

	PMAP_BLOCK pmb = (PMAP_BLOCK)indexMap->MapAddress;

	int bx = block->X * 8;
	int by = block->Y * 8;

	IFOR(x, 0, 8)
	{
		IFOR(y, 0, 8)
		{
			int pos = y * 8 + x;
			CMapObject *obj = new CLandObject(pos, pmb->Cells[pos].TileID & 0x3FFF, 0, bx + x, by + y, pmb->Cells[pos].Z);
			block->AddObject(obj, x, y);
		}
	}

	PSTATICS_BLOCK sb = (PSTATICS_BLOCK)indexMap->StaticAddress;

	if (sb != NULL)
	{
		int count = indexMap->StaticCount;

		for (int c = 0; c < count; c++, sb++)
		{
			if (sb->Color && sb->Color != 0xFFFF)
			{
				int x = sb->X;
				int y = sb->Y;

				int pos = (y * 8) + x;

				if (pos >= 64)
					continue;

				CRenderStaticObject *obj = new CStaticObject(pos, sb->Color, sb->Hue, bx + x, by + y, sb->Z);

				string lowerName = ToLowerA(obj->GetStaticData()->Name);

				if (sb->Color == 0x21A3 || lowerName == "nodraw" || lowerName == "no draw")
					delete obj;
				else
					block->AddObject(obj, x, y);
			}
		}
	}

	block->CreateLandTextureRect();
}
//----------------------------------------------------------------------------------
/*!
Получить индекс текущей карты
@return
*/
int CMapManager::GetActualMap()
{
	if (g_CurrentMap == 1 && (!g_FileManager.m_MapMul[1].Start || !g_FileManager.m_StaticIdx[1].Start || !g_FileManager.m_StaticMul[1].Start))
		return 0;

	return g_CurrentMap;
}
//----------------------------------------------------------------------------------
/*!
Добавить объект рендера
@param [__in] item Ссылка на объект
@return 
*/
void CMapManager::AddRender(CRenderWorldObject *item)
{
	int itemX = item->X;
	int itemY = item->Y;

	int x = itemX / 8;
	int y = itemY / 8;
	
	uint index = (x * g_MapBlockSize[g_CurrentMap].Height) + y;
	
	if (index < m_MaxBlockIndex)
	{
		CMapBlock *block = GetBlock(index);

		if (block == NULL)
		{
			block = AddBlock(index);
			block->X = x;
			block->Y = y;
			LoadBlock(block);
		}

		x = itemX % 8;
		y = itemY % 8;

		block->AddRender(item, x, y);
	}
}
//----------------------------------------------------------------------------------
/*!
Получить ссылку на блок
@param [__in] index Индекс блока
@return Ссылка на блок или NULL
*/
CMapBlock *CMapManager::GetBlock(const uint &index)
{
	CMapBlock *block = NULL;

	if (index < m_MaxBlockIndex)
	{
		block = m_Blocks[index];

		if (block != NULL)
			block->LastAccessTime = g_Ticks;
	}

	return block;
}
//----------------------------------------------------------------------------------
/*!
Добавить блок
@param [__in] index Индекс блока
@return Ссылка на блок или NULL
*/
CMapBlock *CMapManager::AddBlock(const uint &index)
{
	CMapBlock *block = (CMapBlock*)Add(new CMapBlock(index));

	m_Blocks[index] = block;

	return block;
}
//----------------------------------------------------------------------------------
/*!
Удалить блок
@param [__in] index Индекс блока
@return 
*/
void CMapManager::DeleteBlock(const uint &index)
{
	CMapBlock *block = (CMapBlock*)m_Items;

	while (block != NULL)
	{
		if (block->Index == index)
		{
			Delete(block);
			m_Blocks[index] = NULL;

			break;
		}

		block = (CMapBlock*)block->m_Next;
	}
}
//----------------------------------------------------------------------------------
CUopMapManager::CUopMapManager()
: CMapManager()
{
}
//----------------------------------------------------------------------------------
CUopMapManager::~CUopMapManager()
{
}
//----------------------------------------------------------------------------------
/*!
Получить индекс текущей карты
@return
*/
int CUopMapManager::GetActualMap()
{
	//if (g_CurrentMap == 1 && (!FileManager.MapMul[1].Address || !FileManager.StaticIdx[1].Address || !FileManager.StaticMul[1].Address))
	//	return 0;

	return g_CurrentMap;
}
//----------------------------------------------------------------------------------
/*!
Загрузить блок
@param [__inout] block Ссылка на блок для загрузки
@return
*/
void CUopMapManager::LoadBlock(CMapBlock *block)
{
}
//----------------------------------------------------------------------------------
/*!
Получить блок карты напрямую из мулов
@param [__in] map Индекс карты
@param [__in] blockX Координата X блока
@param [__in] blockY Координата Y блока
@param [__out] mb Ссылка на блок
@return Код ошибки (0 - успешно)
*/
void CUopMapManager::GetWorldMapBlock(const int &map, const int &blockX, const int &blockY, MAP_BLOCK &mb)
{
}
//----------------------------------------------------------------------------------
/*!
Получить блок для радара из муллов
@param [__in] blockX Координата X блока
@param [__in] blockY Координата Y блока
@param [__out] mb Ссылка на блок
@return
*/
void CUopMapManager::GetRadarMapBlock(const int &blockX, const int &blockY, MAP_BLOCK &mb)
{
}
//----------------------------------------------------------------------------------
