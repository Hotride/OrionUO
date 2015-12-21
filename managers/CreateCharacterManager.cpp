/****************************************************************************
**
** CharacterCreate.cpp
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

TCreateCharacterManager CreateCharacterManager;
//---------------------------------------------------------------------------
CC_HAIR_STYLE TCreateCharacterManager::m_MaleHairStyleTable[10] = 
{
	{0, 0, "NONE"},
	{0x0753, 0x203B, "Short"},
	{0x0754, 0x203C, "Long"},
	{0x0757, 0x203D, "Pony Tail"},
	{0x0755, 0x2044, "Mohawk"},
	{0x074F, 0x2045, "Pageboy"},
	{0x0752, 0, "Topknot"},
	{0x0751, 0, "Curly"},
	{0x0758, 0x2048, "Receding"},
	{0x074E, 0x2049, "2 Tails"}
};
//---------------------------------------------------------------------------
CC_HAIR_STYLE TCreateCharacterManager::m_FemaleHairStyleTable[11] = 
{
	{0, 0, "NONE"},
	{0x0737, 0x203B, "Short"},
	{0x0732, 0x203C, "Long"},
	{0x0735, 0x203D, "Pony Tail"},
	{0x0733, 0, "Mohawk"},
	{0x0734, 0, "Pageboy"},
	{0x0730, 0, "Topknot"},
	{0x072F, 0, "Curly"},
	{0x0736, 0x2048, "Receding"},
	{0x072C, 0x2049, "2 Tails"},
	{0x0731, 0x2046, "Buns"}
};
//---------------------------------------------------------------------------
CC_HAIR_STYLE TCreateCharacterManager::m_BeardStyleTable[8] = 
{
	{0, 0, "NONE"},
	{0x0759, 0x2040, "Goatee"},
	{0x075B, 0x203E, "Long beard"},
	{0x075D, 0x203F, "Short beard"},
	{0x075C, 0x2041, "Moustache"},
	{0x075E, 0x204B, "Short beard/moustache"},
	{0x075A, 0x204C, "Long beard/moustache"},
	{0x075F, 0x204D, "Vandyke"}
};
//---------------------------------------------------------------------------
TCreateCharacterManager::TCreateCharacterManager()
:m_Sex(false), m_HairStyle(1), m_BeardStyle(0), m_SkinTone(0x03EA),
m_ShirtColor(0x0084), m_PantsColor(0x035F), m_HairColor(0x044E),
m_BeardColor(0x044E), m_SelectedFace(0), m_SelectedColor(0), m_Name(NULL)
{
}
//---------------------------------------------------------------------------
TCreateCharacterManager::~TCreateCharacterManager()
{
	if (m_Name != NULL)
		delete m_Name;

	m_Name = NULL;
}
//---------------------------------------------------------------------------
void TCreateCharacterManager::Init()
{
	m_Name = new TEntryText();

	IFOR(i, 1, 11)
		UO->ExecuteGump(m_FemaleHairStyleTable[i].GumpID, 0);
	IFOR(i, 1, 10)
		UO->ExecuteGump(m_MaleHairStyleTable[i].GumpID, 0);
	IFOR(i, 1, 8)
		UO->ExecuteGump(m_BeardStyleTable[i].GumpID, 0);

}
//---------------------------------------------------------------------------
void TCreateCharacterManager::Clear()
{
	m_Sex = false;
	m_HairStyle = 1;
	m_BeardStyle = 0;
	
	m_SelectedFace = m_SelectedColor = 0;

	m_SkinTone = 0x03EA;
	m_ShirtColor = 0x0084;
	m_PantsColor = 0x035F;
	m_HairColor = 0x044E;
	m_BeardColor = 0x044E;
	m_Name->SetText(L"");
}
//---------------------------------------------------------------------------
CC_HAIR_STYLE TCreateCharacterManager::GetHair(BYTE pos) const
{
	if (pos < 0  || pos >= (10 + (int)m_Sex))
		return m_MaleHairStyleTable[0];

	if (m_Sex)
		return m_FemaleHairStyleTable[pos];

	return m_MaleHairStyleTable[pos];
}
//---------------------------------------------------------------------------
CC_HAIR_STYLE TCreateCharacterManager::GetBeard(BYTE pos) const
{
	if (pos < 0  || pos >= 8)
		return m_BeardStyleTable[0];

	return m_BeardStyleTable[pos];
}
//---------------------------------------------------------------------------