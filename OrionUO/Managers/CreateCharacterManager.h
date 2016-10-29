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
	SETGETE(bool, Female, OnSexChanged);
	SETGETE(RACE_TYPE, Race, OnChangeRace);
	SETGET(uchar, HairStyle);
	SETGET(uchar, BeardStyle);
	SETGET(ushort, SkinTone);
	SETGET(ushort, ShirtColor);
	SETGET(ushort, PantsColor);
	SETGET(ushort, HairColor);
	SETGET(ushort, BeardColor);

public:
	static const int HUMAN_MALE_HAIR_COUNT = 10;
	static const int HUMAN_FEMALE_HAIR_COUNT = 11;
	static const int HUMAN_MALE_FACIAL_HAIR_COUNT = 8;

	static const int ELF_MALE_HAIR_COUNT = 9;
	static const int ELF_FEMALE_HAIR_COUNT = 9;

	static const int GARGOYLE_MALE_HAIR_COUNT = 0;
	static const int GARGOYLE_FEMALE_HAIR_COUNT = 0;
	static const int GARGOYLE_MALE_FACIAL_HAIR_COUNT = 0;

private:
	//!Перечень стилей причесок и бороды
	static CC_HAIR_STYLE m_HumanMaleHairStyleTable[HUMAN_MALE_HAIR_COUNT];
	static CC_HAIR_STYLE m_HumanFemaleHairStyleTable[HUMAN_FEMALE_HAIR_COUNT];
	static CC_HAIR_STYLE m_HumanBeardStyleTable[HUMAN_MALE_FACIAL_HAIR_COUNT];

	static CC_HAIR_STYLE m_ElfMaleHairStyleTable[ELF_MALE_HAIR_COUNT];
	static CC_HAIR_STYLE m_ElfFemaleHairStyleTable[ELF_FEMALE_HAIR_COUNT];

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

	int GetCurrentHairCount();

	int GetCurrentFacialHairCount();

	ushort GetBodyGump();

	ushort GetShirtGump();

	ushort GetPantsGump();

	ushort GetBootsGump();

	/*!
	Получить стиль волос
	@param [__in] pos Индекс волос
	@return Структура с данными о волосах
	*/
	CC_HAIR_STYLE GetHair(const uchar &pos) const;

	/*!
	Получить стиль бороды
	@param [__in] pos Индекс бороды
	@return Структура с данными о бороде
	*/
	CC_HAIR_STYLE GetBeard(const uchar &pos) const;

};
//----------------------------------------------------------------------------------
//!Менеджер создания мерсонажа
extern CCreateCharacterManager g_CreateCharacterManager;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
