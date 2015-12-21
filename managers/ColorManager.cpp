/****************************************************************************
**
** ColorManager.cpp
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
#include "stdafx.h"

TColorManager *ColorManager;
//---------------------------------------------------------------------------
TColorManager::TColorManager()
{
	DWORD addr = (DWORD)FileManager.HuesMul.Address;
	DWORD size = FileManager.HuesMul.Size;

	if (addr > 0 && size > 0 && addr != 0xFFFFFFFF && size != 0xFFFFFFFF)
	{
		int EntryCount = size / sizeof(HUES_GROUP);

		m_HuesCount = EntryCount * 8;
		m_HuesRange = new HUES_GROUP[EntryCount];

		memcpy(m_HuesRange, (PVOID)addr, EntryCount * sizeof(HUES_GROUP));
	}
	else
	{
		m_HuesRange = NULL;
		m_HuesCount = 0;
	}

	addr = (DWORD)FileManager.RadarcolMul.Address;
	if (addr > 0 && addr != 0xFFFFFFFF)
		memcpy(m_Radarcol, (PVOID)addr, 0x10000);
	else
		memset(m_Radarcol, 0, 0x10000);

	addr = (DWORD)FileManager.PaletteMul.Address;
	if (addr > 0 && addr != 0xFFFFFFFF)
		memcpy(m_Palette, (PVOID)addr, 256 * sizeof(PALETTE_BLOCK));
	else
		memset(m_Palette, 0, 256 * sizeof(PALETTE_BLOCK));
}
//---------------------------------------------------------------------------
TColorManager::~TColorManager()
{
	if (m_HuesRange != NULL)
		delete m_HuesRange;
	m_HuesRange = NULL;

	m_HuesCount = 0;
}
//---------------------------------------------------------------------------
void TColorManager::SetHuesBlock(int Index, PVERDATA_HUES_GROUP group)
{
	if (Index < 0 || Index >= m_HuesCount)
		return;

	m_HuesRange[Index].Header = group->Header;
	IFOR(i, 0, 8)
		memcpy(&m_HuesRange[Index].Entries[i].ColorTable[0], &group->Entries[i].ColorTable[0], sizeof(WORD[32]));
}
//---------------------------------------------------------------------------
DWORD TColorManager::Color16To32(WORD &C)
{
	return
	(
		(((C >> 10) & 0x1F) * 0xFF / 0x1F) |
		((((C >> 5) & 0x1F) * 0xFF / 0x1F) << 8) |
		(((C & 0x1F) * 0xFF / 0x1F) << 16)
	);
}
//---------------------------------------------------------------------------
WORD TColorManager::Color32To16(DWORD &C)
{
	return
	(
		((C & 0xFF) * 32) / 256) |
		(((((C >> 16) & 0xff) * 32) / 256) << 10) |
		(((((C >> 8) & 0xff) * 32) / 256) << 5
	);
}
//---------------------------------------------------------------------------
WORD TColorManager::ConvertToGray(WORD &C)
{
	return ((C & 0x1F) * 299 + ((C >> 5) & 0x1F) * 587 + ((C >> 10) & 0x1F) * 114) / 1000;
}
//---------------------------------------------------------------------------
DWORD TColorManager::GetRadarColor(WORD &C)
{
	return Color16To32(m_Radarcol[C]);
}
//---------------------------------------------------------------------------
DWORD TColorManager::GetPolygoneColor(WORD C, WORD Color)
{
	if (Color != 0 && Color < m_HuesCount)
	{
		Color -= 1;
		int g = Color / 8;
		int e = Color % 8;

		return Color16To32(m_HuesRange[g].Entries[e].ColorTable[C]);
	}

	return 0xFF010101; //Black
}
//---------------------------------------------------------------------------
DWORD TColorManager::GetUnicodeFontColor(WORD &C, WORD Color)
{
	if (Color != 0 && Color < m_HuesCount)
	{
		Color -= 1;
		int g = Color / 8;
		int e = Color % 8;

		return Color16To32(m_HuesRange[g].Entries[e].ColorTable[8]);
	}

	return Color16To32(C);
}
//---------------------------------------------------------------------------
DWORD TColorManager::GetColor(WORD &C, WORD Color)
{
	if (Color != 0 && Color < m_HuesCount)
	{
		Color -= 1;
		int g = Color / 8;
		int e = Color % 8;

		return Color16To32(m_HuesRange[g].Entries[e].ColorTable[(C >> 10) & 0x1F]);
	}

	return Color16To32(C);
}
//---------------------------------------------------------------------------
DWORD TColorManager::GetPartialHueColor(WORD &C, WORD Color)
{
	if (Color != 0 && Color < m_HuesCount)
	{
		Color -= 1;
		int g = Color / 8;
		int e = Color % 8;

		DWORD Cl = Color16To32(C);

		if (GetRValue(Cl) == GetGValue(Cl) && GetBValue(Cl) == GetGValue(Cl))
			return Color16To32(m_HuesRange[g].Entries[e].ColorTable[(C >> 10) & 0x1F]);

		return Cl;
	}

	return Color16To32(C);
}
//---------------------------------------------------------------------------