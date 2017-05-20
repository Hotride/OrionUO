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
CustomHouse* CustomHousesManager::GetCustomHouse(uint serial)
{
	if (m_CustomHouses.find(serial) != m_CustomHouses.end())
	{
		return &m_CustomHouses.at(serial);
	}
	return NULL;
}
//----------------------------------------------------------------------------------
void CustomHousesManager::AddCustomHouse(CustomHouse &house)
{
	m_CustomHouses[house.Serial] = house;
}
//----------------------------------------------------------------------------------