/***********************************************************************************
**
** CustomHousesManager.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef CUSTOMHOUSESMANAGER_H
#define CUSTOMHOUSESMANAGER_H
//----------------------------------------------------------------------------------
#include "../Wisp/WispDataStream.h"
#include "../zconf.h"
#include "../Game objects/GameItem.h"
//----------------------------------------------------------------------------------
struct CustomHouseData
{
	ushort Graphic;
	short X;
	short Y;
	uchar Z;
};
struct CustomHouse
{
	uint Serial;
	uint Revision;
	vector<CustomHouseData> HouseData;
};
class CustomHousesManager : public WISP_DATASTREAM::CDataReader
{
	unordered_map<uint, CustomHouse*> m_CustomHouses;
public:
	CustomHousesManager();
	~CustomHousesManager();
	CustomHouse* GetCustomHouse(uint serial);
	void AddCustomHouse(CustomHouse* house);
	bool TakeFromCache(CGameItem* houseFoundation, CustomHouse* house);
};
//!Global reference to custom houses manager
extern CustomHousesManager g_CustomHousesManager;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------

