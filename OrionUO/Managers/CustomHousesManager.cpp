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
CustomHousesManager::CustomHousesManager()
	: m_CustomHouses{0}
{
}
//----------------------------------------------------------------------------------
CustomHousesManager::~CustomHousesManager()
{
}
//----------------------------------------------------------------------------------
CustomHouse* CustomHousesManager::GetCustomHouse(uint serial)
{
	IFOR(i, 0, m_CustomHouses.size())
	{
		if (m_CustomHouses[i].Serial == serial)
			return &m_CustomHouses[i];
	}
	return NULL;
}
//----------------------------------------------------------------------------------
void CustomHousesManager::AddCustomHouse(CustomHouse &house)
{

}
//----------------------------------------------------------------------------------