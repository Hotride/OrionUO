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
//----------------------------------------------------------------------------------
CMapManager *g_MapManager = NULL;
//----------------------------------------------------------------------------------
CMapManager::CMapManager()
: CBaseQueue(), m_MaxBlockIndex(0)
#if USE_BLOCK_MAP == 1
, m_Blocks(NULL)
#endif
{
}
//----------------------------------------------------------------------------------
CMapManager::~CMapManager()
{
#if USE_BLOCK_MAP == 1
	if (m_Blocks != NULL)
	{
		delete[] m_Blocks;
		m_Blocks = NULL;
	}
#endif

	m_MaxBlockIndex = 0;
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
int CMapManager::GetWorldMapBlock(const int &map, const int &blockX, const int &blockY, MAP_BLOCK &mb)
{
	if (!g_FileManager.m_MapMul[map].Start || !g_FileManager.m_StaticIdx[map].Start || !g_FileManager.m_StaticMul[map].Start)
		return 1;
	else if (blockX < 0 || blockX >= g_MapBlockSize[map].Width)
		return 2;
	else if ( blockY < 0 || blockY >= g_MapBlockSize[map].Height)
		return 3;

	int block = (blockX * g_MapBlockSize[map].Height) + blockY;
	PMAP_BLOCK pmb = (PMAP_BLOCK)((uint)g_FileManager.m_MapMul[map].Size + (block * sizeof(MAP_BLOCK)));
	
	if ((uint)pmb  > (uint)g_FileManager.m_MapMul[map].Start + g_FileManager.m_MapMul[map].Size)
		return 4;

	IFOR(x, 0, 8)
	{
		IFOR(y, 0, 8)
		{
			int pos = (y * 8) + x;
			mb.Cells[pos].TileID = pmb->Cells[pos].TileID;
			mb.Cells[pos].Z = pmb->Cells[pos].Z;
		}
	}

	PSTAIDX_BLOCK sidx = (PSTAIDX_BLOCK)((uint)g_FileManager.m_StaticIdx[map].Start + (block * sizeof(STAIDX_BLOCK)));
	if (sidx->Size > 0 && sidx->Position != 0xFFFFFFFF && (uint)sidx < (uint)g_FileManager.m_StaticIdx[map].Start + g_FileManager.m_StaticIdx[map].Size)
	{
		PSTATICS_BLOCK sb = (PSTATICS_BLOCK)((uint)g_FileManager.m_StaticMul[map].Start + sidx->Position);

		int scnt = sidx->Size / sizeof(STATICS_BLOCK);

		if (scnt > 0)
		{
			if (scnt > 1024)
				scnt = 1024;

			IFOR(c, 0, scnt)
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

	return 0;
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
	int map = GetActualMap();
	
	if (!g_FileManager.m_MapMul[map].Start || !g_FileManager.m_StaticIdx[map].Start || !g_FileManager.m_StaticMul[map].Start)
		return;

	if (blockX < 0 || blockY < 0 || blockX >= g_MapBlockSize[map].Width || blockY >= g_MapBlockSize[map].Height)
		return;

	int block = (blockX * g_MapBlockSize[map].Height) + blockY;
	PMAP_BLOCK pmb = (PMAP_BLOCK)((uint)g_FileManager.m_MapMul[map].Start + (block * sizeof(MAP_BLOCK)));
	
	if ((uint)pmb  > (uint)g_FileManager.m_MapMul[map].Start + g_FileManager.m_MapMul[map].Size)
		return;

	IFOR(x, 0, 8)
	{
		IFOR(y, 0, 8)
		{
			int pos = (y * 8) + x;
			mb.Cells[pos].TileID = pmb->Cells[pos].TileID;
			mb.Cells[pos].Z = pmb->Cells[pos].Z;
		}
	}

	PSTAIDX_BLOCK sidx = (PSTAIDX_BLOCK)((uint)g_FileManager.m_StaticIdx[map].Start + (block * sizeof(STAIDX_BLOCK)));
	if (sidx->Size > 0 && sidx->Position != 0xFFFFFFFF && (uint)sidx < (uint)g_FileManager.m_StaticIdx[map].Start + g_FileManager.m_StaticIdx[map].Size)
	{
		PSTATICS_BLOCK sb = (PSTATICS_BLOCK)((uint)g_FileManager.m_StaticMul[map].Start + sidx->Position);

		int scnt = sidx->Size / sizeof(STATICS_BLOCK);
		if (scnt < 1)
			return;

		if (scnt > 1024)
			scnt = 1024;

		IFOR(c, 0, scnt)
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
	int index = (blockX * g_MapBlockSize[g_CurrentMap].Height) + blockY;

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
			DWORD index = block->Index;
			Delete(block);

#if USE_BLOCK_MAP == 1
			m_Blocks[index] = NULL;
#endif
		}

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

#if USE_BLOCK_MAP == 1
	if (!delayed)
	{
		if (m_Blocks != NULL)
		{
			delete[] m_Blocks;
			m_Blocks = NULL;
		}

		m_MaxBlockIndex = g_MapBlockSize[map].Width * g_MapBlockSize[map].Height;
		m_Blocks = new CMapBlock*[m_MaxBlockIndex];
		memset(&m_Blocks[0], 0, sizeof(CMapBlock*) * m_MaxBlockIndex);
	}
#endif
	
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

			int index = (i * g_MapBlockSize[map].Height) + j;
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
//----------------------------------------------------------------------------------
/*!
Загрузить блок
@param [__inout] block Ссылка на блок для загрузки
@return 
*/
void CMapManager::LoadBlock(CMapBlock *block)
{
	int map = GetActualMap();
	
	if (!g_FileManager.m_MapMul[map].Start || !g_FileManager.m_StaticIdx[map].Start || !g_FileManager.m_StaticMul[map].Start)
		return;

	int index = block->Index;
	PMAP_BLOCK pmb = (PMAP_BLOCK)((uint)g_FileManager.m_MapMul[map].Start + (index * sizeof(MAP_BLOCK)));
	
	int bx = block->X * 8;
	int by = block->Y * 8;

	IFOR(x, 0, 8)
	{
		IFOR(y, 0, 8)
		{
			int pos = y * 8 + x;
			CMapObject *obj = new CLandObject(pos, pmb->Cells[pos].TileID, 0, bx + x, by + y, pmb->Cells[pos].Z);
			block->AddObject(obj, x, y);
		}
	}

	PSTAIDX_BLOCK sidx = (PSTAIDX_BLOCK)((uint)g_FileManager.m_StaticIdx[map].Start + (index * sizeof(STAIDX_BLOCK)));
	if (sidx->Size > 0 && sidx->Position != 0xFFFFFFFF && (uint)sidx < (uint)g_FileManager.m_StaticIdx[map].Start + g_FileManager.m_StaticIdx[map].Size)
	{
		PSTATICS_BLOCK sb = (PSTATICS_BLOCK)((uint)g_FileManager.m_StaticMul[map].Start + sidx->Position);

		int scnt = sidx->Size / sizeof(STATICS_BLOCK);

		if (scnt > 1024)
			scnt = 1024;

		for (int c = 0; c < scnt; c++, sb++)
		{
			if (sb->Color && sb->Color != 0xFFFF)
			{
				int x = sb->X;
				int y = sb->Y;

				int pos = (y * 8) + x;
				if (pos >= 64)
					continue;

				CRenderStaticObject *obj = new CStaticObject(pos, sb->Color, sb->Hue, bx + x, by + y, sb->Z);

				if (ToLowerA(obj->GetStaticData()->Name) == "nodraw")
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
	
	int index = (x * g_MapBlockSize[g_CurrentMap].Height) + y;
	
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
//----------------------------------------------------------------------------------
/*!
Получить ссылку на блок
@param [__in] index Индекс блока
@return Ссылка на блок или NULL
*/
CMapBlock *CMapManager::GetBlock(const uint &index)
{
#if USE_BLOCK_MAP == 0
	TMapBlock *block = (TMapBlock*)m_Items;

	while (block != NULL)
	{
		if (block->Index == index)
		{
			block->LastAccessTime = g_Ticks;

			break;
		}

		block = (TMapBlock*)block->m_Next;
	}
#else
	CMapBlock *block = NULL;

	if (index < m_MaxBlockIndex)
		block = m_Blocks[index];

	if (block != NULL)
		block->LastAccessTime = g_Ticks;
#endif

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

#if USE_BLOCK_MAP == 1
	m_Blocks[index] = block;
#endif

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
#if USE_BLOCK_MAP == 1
			m_Blocks[index] = NULL;
#endif

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
int CUopMapManager::GetWorldMapBlock(const int &map, const int &blockX, const int &blockY, MAP_BLOCK &mb)
{
	return 0;
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
