/****************************************************************************
**
** CreateCharacterManager.h
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
#ifndef CharacterCreateH
#define CharacterCreateH
//---------------------------------------------------------------------------
struct CC_HAIR_STYLE
{
	WORD GumpID;
	WORD GraphicID;
	string Name;
};
//---------------------------------------------------------------------------
class TCreateCharacterManager
{
private:
	bool m_Sex; //false - male; true - female
	
	BYTE m_SelectedFace;
	BYTE m_SelectedColor;
	
	BYTE m_HairStyle;
	BYTE m_BeardStyle;
	
	WORD m_SkinTone;
	WORD m_ShirtColor;
	WORD m_PantsColor;
	WORD m_HairColor;
	WORD m_BeardColor;

	static CC_HAIR_STYLE m_MaleHairStyleTable[10];
	static CC_HAIR_STYLE m_FemaleHairStyleTable[11];
	static CC_HAIR_STYLE m_BeardStyleTable[8];
public:
	TCreateCharacterManager();
	~TCreateCharacterManager();

	void Init();
	void Clear();

	SETGET(bool, Sex)
	SETGET(BYTE, HairStyle)
	SETGET(BYTE, BeardStyle)
	SETGET(WORD, SkinTone)
	SETGET(WORD, ShirtColor)
	SETGET(WORD, PantsColor)
	SETGET(WORD, HairColor)
	SETGET(WORD, BeardColor)
	SETGET(BYTE, SelectedFace)
	SETGET(BYTE, SelectedColor)

	TEntryText *m_Name;
	
	CC_HAIR_STYLE GetHair(BYTE pos) const;
	CC_HAIR_STYLE GetBeard(BYTE pos) const;
};

extern TCreateCharacterManager CreateCharacterManager;
//---------------------------------------------------------------------------
#endif