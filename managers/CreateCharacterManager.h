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
//!Информация о волосеном покрове
struct CC_HAIR_STYLE
{
	//!Индекс гампа
	WORD GumpID;

	//!Индекс картинки из Art.mul
	WORD GraphicID;

	//!Название
	string Name;
};
//---------------------------------------------------------------------------
//!Класс менеджера создания персонажей
class TCreateCharacterManager
{
private:
	//!Пол персонажа. false - male; true - female
	bool m_Sex;

	//!Расса персонажа
	char m_Rase;

	//!Индекс стиля прически
	BYTE m_HairStyle;

	//!Индекс стиля бороды
	BYTE m_BeardStyle;

	//!Цвет тела
	WORD m_SkinTone;

	//!Цвет рубахи/платишка
	WORD m_ShirtColor;

	//!Цвет шортиков
	WORD m_PantsColor;

	//!Цвет волос
	WORD m_HairColor;

	//!Цвет бороды
	WORD m_BeardColor;

	//!Перечень стилей причесок и бороды
	static CC_HAIR_STYLE m_MaleHairStyleTable[10];
	static CC_HAIR_STYLE m_FemaleHairStyleTable[11];
	static CC_HAIR_STYLE m_BeardStyleTable[8];
public:
	TCreateCharacterManager();
	~TCreateCharacterManager();

	//!Введенное имя персонажа
	TEntryText *m_Name;

	SETGET(bool, Sex);
	SETGET(char, Rase);
	SETGET(BYTE, HairStyle);
	SETGET(BYTE, BeardStyle);
	SETGET(WORD, SkinTone);
	SETGET(WORD, ShirtColor);
	SETGET(WORD, PantsColor);
	SETGET(WORD, HairColor);
	SETGET(WORD, BeardColor);

	/*!
	Инициализация
	@return
	*/
	void Init();

	/*!
	Очистка
	@return
	*/
	void Clear();

	/*!
	Получить стиль волос
	@param [__in] pos Индекс волос
	@return Структура с данными о волосах
	*/
	CC_HAIR_STYLE GetHair(__in BYTE pos) const;

	/*!
	Получить стиль бороды
	@param [__in] pos Индекс бороды
	@return Структура с данными о бороде
	*/
	CC_HAIR_STYLE GetBeard(__in BYTE pos) const;

};
//---------------------------------------------------------------------------
//!Менеджер создания мерсонажа
extern TCreateCharacterManager CreateCharacterManager;
//---------------------------------------------------------------------------
#endif