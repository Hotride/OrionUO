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
//----------------------------------------------------------------------------------
struct CustomHouse
{
	uint Serial;
	uint Revision;
	UCHAR_LIST CompressedData;
	ushort CompressedDataSize;
	ushort DecompressedDataSize;
};
class CustomHousesManager : public WISP_DATASTREAM::CDataReader
{
public:
	CustomHousesManager();
	~CustomHousesManager();
	CustomHouse GetCustomHouse(uint serial);
	void AddCustomHouse(CustomHouse &house);
};
//!Global reference to custom houses manager
extern CustomHousesManager g_CustomHousesManager;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------

