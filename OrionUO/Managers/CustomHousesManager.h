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

//----------------------------------------------------------------------------------
struct CustomHouse
{
	uint Serial;
	uint Revision;
	uchar *CompressedData;
	uLongf DecompressedDataSize;
};
class CustomHousesManager : public WISP_DATASTREAM::CDataReader
{
	unordered_map<uint, CustomHouse> m_CustomHouses;
public:
	CustomHousesManager();
	~CustomHousesManager();
	CustomHouse* GetCustomHouse(uint serial);
	void AddCustomHouse(CustomHouse &house);
};
//!Global reference to custom houses manager
extern CustomHousesManager g_CustomHousesManager;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------

