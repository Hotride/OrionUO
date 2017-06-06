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
	char X;
	char Y;
	char Z;
};
struct CustomHouseStruct
{
	uint Serial;
	uint Revision;
	vector<CustomHouseData> HouseData;
};
class CustomHousesManager : public WISP_DATASTREAM::CDataReader
{
	unordered_map<uint, CustomHouseStruct*> m_CustomHouses;
public:
	CustomHousesManager();
	~CustomHousesManager();
	CustomHouseStruct* GetCustomHouse(uint serial);
	void AddCustomHouse(CustomHouseStruct* house);
	bool TakeFromCache(CGameItem* houseFoundation, CustomHouseStruct* house);
};
//!Global reference to custom houses manager
extern CustomHousesManager g_CustomHousesManager;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------

