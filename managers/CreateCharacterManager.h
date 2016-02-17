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
//Информация о волосеном покрове
struct CC_HAIR_STYLE
{
	WORD GumpID;
	WORD GraphicID;
	string Name;
};
//---------------------------------------------------------------------------
//Менеджер создания персонажей
class TCreateCharacterManager
{
private:
	//Пол персонажа. false - male; true - female
	bool m_Sex;

	//Индекс стиля прически
	BYTE m_HairStyle;

	//Индекс стиля бороды
	BYTE m_BeardStyle;

	//Выбранный объект
	BYTE m_SelectedFace;

	//Выбранный цвет
	BYTE m_SelectedColor;

	//Цвет тела
	WORD m_SkinTone;

	//Цвет рубахи/платишка
	WORD m_ShirtColor;

	//Цвет шортиков
	WORD m_PantsColor;

	//Цвет волос
	WORD m_HairColor;

	//Цвет бороды
	WORD m_BeardColor;

	//Перечень стилей причесок и бороды
	static CC_HAIR_STYLE m_MaleHairStyleTable[10];
	static CC_HAIR_STYLE m_FemaleHairStyleTable[11];
	static CC_HAIR_STYLE m_BeardStyleTable[8];
public:
	TCreateCharacterManager();
	~TCreateCharacterManager();

	//Инициализация
	void Init();

	//Очистка
	void Clear();

	SETGET(bool, Sex);
	SETGET(BYTE, HairStyle);
	SETGET(BYTE, BeardStyle);
	SETGET(WORD, SkinTone);
	SETGET(WORD, ShirtColor);
	SETGET(WORD, PantsColor);
	SETGET(WORD, HairColor);
	SETGET(WORD, BeardColor);
	SETGET(BYTE, SelectedFace);
	SETGET(BYTE, SelectedColor);

	//Введенное имя персонажа
	TEntryText *m_Name;

	//Получить стиль волос/бороды по индексу
	CC_HAIR_STYLE GetHair(BYTE pos) const;
	CC_HAIR_STYLE GetBeard(BYTE pos) const;
};
//---------------------------------------------------------------------------
extern TCreateCharacterManager CreateCharacterManager;
//---------------------------------------------------------------------------
#endif