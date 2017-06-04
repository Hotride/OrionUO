/***********************************************************************************
**
** CustomHousesManager.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
#include "CustomHousesManager.h"
//----------------------------------------------------------------------------------
CustomHousesManager g_CustomHousesManager;
//----------------------------------------------------------------------------------
CustomHousesManager::CustomHousesManager()
{
}
//----------------------------------------------------------------------------------
CustomHousesManager::~CustomHousesManager()
{
}
//----------------------------------------------------------------------------------
CustomHouseStruct* CustomHousesManager::GetCustomHouse(uint serial)
{
	if (m_CustomHouses.find(serial) != m_CustomHouses.end())
	{
		return m_CustomHouses.at(serial);
	}
	return NULL;
}
//----------------------------------------------------------------------------------
void CustomHousesManager::AddCustomHouse(CustomHouseStruct* house)
{
	m_CustomHouses[house->Serial] = house;
}
//----------------------------------------------------------------------------------
bool CustomHousesManager::TakeFromCache(CGameItem* houseFoundation, CustomHouseStruct* house)
{
	if (house != NULL && houseFoundation != NULL)
	{
		int houses = house->HouseData.size();
		if (houses == 0) return false;
		IFOR(i, 0, houses)
		{
			CustomHouseData data = house->HouseData[i];
			houseFoundation->AddMulti(data.Graphic, data.X, data.Y, data.Z);
		}
		return true;
	}
	return false;
}