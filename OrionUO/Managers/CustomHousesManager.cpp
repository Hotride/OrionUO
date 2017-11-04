// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** CustomHousesManager.cpp
**
** Copyright (C) September 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CustomHousesManager g_CustomHousesManager;
//----------------------------------------------------------------------------------
void CCustomHouse::Paste(CGameItem *foundation)
{
	WISPFUN_DEBUG("");
	if (foundation == NULL)
		return;

	foundation->ClearCustomHouseMultis(0);
	int z = foundation->Z;

	for (const CBuildObject &item : m_Items)
		foundation->AddMulti(item.Graphic, 0, item.X, item.Y, item.Z + z, true);

	if (g_CustomHouseGump != NULL && g_CustomHouseGump->Serial == m_Serial)
	{
		g_CustomHouseGump->WantUpdateContent = true;
		g_CustomHouseGump->GenerateFloorPlace();
	}
}
//----------------------------------------------------------------------------------
//--------------------------------CustomHousesManager-------------------------------
//----------------------------------------------------------------------------------
CustomHousesManager::~CustomHousesManager()
{
	WISPFUN_DEBUG("");
	Clear();
}
//----------------------------------------------------------------------------------
void CustomHousesManager::Clear()
{
	WISPFUN_DEBUG("");
	for (unordered_map<uint, CCustomHouse*>::iterator i = m_Items.begin(); i != m_Items.end(); i++)
	{
		CCustomHouse *house = i->second;
		delete house;
		i->second = NULL;
	}
}
//----------------------------------------------------------------------------------
CCustomHouse *CustomHousesManager::Get(const uint &serial)
{
	WISPFUN_DEBUG("");
	for (unordered_map<uint, CCustomHouse*>::iterator i = m_Items.begin(); i != m_Items.end(); i++)
	{
		CCustomHouse *house = i->second;

		if (house != NULL && house->Serial == serial)
			return i->second;
	}

	return NULL;
}
//----------------------------------------------------------------------------------
void CustomHousesManager::Add(CCustomHouse* house)
{
	WISPFUN_DEBUG("");
	if (house != NULL)
		m_Items[house->Serial] = house;
}
//----------------------------------------------------------------------------------
void CustomHousesManager::Load(const string &path)
{
	WISPFUN_DEBUG("");
	Clear();

	WISP_FILE::CMappedFile file;

	if (file.Load(path) && file.Size)
	{
		uchar version = file.ReadUInt8();

		int count = file.ReadInt32LE();

		IFOR(i, 0, count)
		{
			uint serial = file.ReadUInt32LE();

			if (!serial)
				break;

			uint revision = file.ReadUInt32LE();
			int itemsCount = file.ReadInt32LE();

			CCustomHouse *house = Get(serial);

			LOG("Load house from cache file: 0x%08X 0x%08X\n", serial, revision);

			if (house == NULL)
			{
				house = new CCustomHouse(serial, revision);
				g_CustomHousesManager.Add(house);
			}
			else
				house->Revision = revision;

			IFOR(j, 0, itemsCount)
			{
				ushort graphic = file.ReadUInt16LE();
				char x = file.ReadInt8();
				char y = file.ReadInt8();
				char z = file.ReadInt8();

				house->m_Items.push_back(CBuildObject(graphic, x, y, z));
			}
		}

		file.Unload();
	}
}
//----------------------------------------------------------------------------------
void CustomHousesManager::Save(const string &path)
{
	WISPFUN_DEBUG("");
	WISP_FILE::CBinaryFileWritter writter;

	writter.Open(path);

	writter.WriteInt8(1); //version
	writter.WriteBuffer();

	int count = 0;

	for (unordered_map<uint, CCustomHouse*>::iterator i = m_Items.begin(); i != m_Items.end(); i++)
	{
		CCustomHouse *house = i->second;

		if (house != NULL && house->m_Items.size())
			count++;
	}

	writter.WriteInt32LE(count);
	writter.WriteBuffer();

	for (unordered_map<uint, CCustomHouse*>::iterator i = m_Items.begin(); i != m_Items.end(); i++)
	{
		CCustomHouse *house = i->second;

		if (house == NULL || !house->m_Items.size())
			continue;

		writter.WriteUInt32LE(house->Serial);
		writter.WriteUInt32LE(house->Revision);
		writter.WriteInt32LE(house->m_Items.size());
		writter.WriteBuffer();

		for (const CBuildObject &item : house->m_Items)
		{
			writter.WriteUInt16LE(item.Graphic);
			writter.WriteInt8(item.X);
			writter.WriteInt8(item.Y);
			writter.WriteInt8(item.Z);
			writter.WriteBuffer();
		}
	}

	writter.WriteUInt32LE(0); //EOF
	writter.WriteBuffer();

	writter.Close();
}
//----------------------------------------------------------------------------------

