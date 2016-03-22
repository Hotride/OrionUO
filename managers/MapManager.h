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
class TMapManager : public TBaseQueue
{
private:
#if USE_BLOCK_MAP == 1
	TMapBlock **m_Blocks;
#endif

	//Максимальный индекс блока для текущей карты
	DWORD m_MaxBlockIndex;
public:
	TMapManager();
	virtual ~TMapManager();

	//Получить индекс текущей карты
	int GetActualMap();

	//Загрузить блок
	void LoadBlock(TMapBlock *block);

	//Получить блок для радара из муллов
	void GetRadarMapBlock(int blockX, int blockY, MAP_BLOCK &mb);

	//Получить блок карты напрямую из мулов
	int GetWorldMapBlock(int &map, int &blockX, int &blockY, MAP_BLOCK &mb);

	//Получить значение Z координаты для указанной точки в мире
	void GetMapZ(int x, int y, int &groundZ, int &staticZ);

	//Инициализация
	void Init(bool delayed = false);

	//Снять флаг создания рендер листа
	void UnselectRangedCreateRenderList();

	//Получить ссылку на блок
	TMapBlock *GetBlock(DWORD index);

	//Добавить блок
	TMapBlock *AddBlock(DWORD index);

	TLandObject *GetLand(int &x, int &y);

	void UnselectCreateRenderList(int x, int y);

	//Удалить блок
	void DeleteBlock(DWORD index);

	//Удалить неиспользуемые блоки
	void ClearUnusedBlocks();

	//Добавить объект рендера
	void AddRender(TRenderWorldObject *item);
};
//--------------------------------------------------------------------------
extern TMapManager *MapManager;
//--------------------------------------------------------------------------
#endif