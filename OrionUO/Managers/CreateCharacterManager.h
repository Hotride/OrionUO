/***********************************************************************************
**
** CreateCharacterManager.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef CREATECHARACTERMANAGER_H
#define CREATECHARACTERMANAGER_H
//----------------------------------------------------------------------------------
#include "../Globals.h"
#include "../TextEngine/EntryText.h"
//----------------------------------------------------------------------------------
//!Информация о волосеном покрове
struct CC_HAIR_STYLE
{
	//!Индекс гампа
	ushort GumpID;

	//!Индекс картинки из Art.mul
	ushort GraphicID;

	//!Название
	string Name;
};
//----------------------------------------------------------------------------------
//!Класс менеджера создания персонажей
class CCreateCharacterManager
{
	SETGET(bool, Female);
	SETGET(char, Race);
	SETGET(uchar, HairStyle);
	SETGET(uchar, BeardStyle);
	SETGET(ushort, SkinTone);
	SETGET(ushort, ShirtColor);
	SETGET(ushort, PantsColor);
	SETGET(ushort, HairColor);
	SETGET(ushort, BeardColor);

private:
	//!Перечень стилей причесок и бороды
	static CC_HAIR_STYLE m_MaleHairStyleTable[10];
	static CC_HAIR_STYLE m_FemaleHairStyleTable[11];
	static CC_HAIR_STYLE m_BeardStyleTable[8];

public:
	CCreateCharacterManager();
	~CCreateCharacterManager();


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
	CC_HAIR_STYLE GetHair(uchar pos) const;

	/*!
	Получить стиль бороды
	@param [__in] pos Индекс бороды
	@return Структура с данными о бороде
	*/
	CC_HAIR_STYLE GetBeard(uchar pos) const;

};
//----------------------------------------------------------------------------------
//!Менеджер создания мерсонажа
extern CCreateCharacterManager g_CreateCharacterManager;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
