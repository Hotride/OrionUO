/***********************************************************************************
**
** CreateCharacterManager.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "CreateCharacterManager.h"
#include "../OrionUO.h"
//----------------------------------------------------------------------------------
CCreateCharacterManager g_CreateCharacterManager;
//----------------------------------------------------------------------------------
CC_HAIR_STYLE CCreateCharacterManager::m_MaleHairStyleTable[10] =
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
//----------------------------------------------------------------------------------
CC_HAIR_STYLE CCreateCharacterManager::m_FemaleHairStyleTable[11] =
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
//----------------------------------------------------------------------------------
CC_HAIR_STYLE CCreateCharacterManager::m_BeardStyleTable[8] =
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
//----------------------------------------------------------------------------------
CCreateCharacterManager::CCreateCharacterManager()
:m_Sex(false), m_HairStyle(1), m_BeardStyle(0), m_SkinTone(0x03EA),
m_ShirtColor(0x0084), m_PantsColor(0x035F), m_HairColor(0x044E),
m_BeardColor(0x044E), m_Rase(0)
{
}
//----------------------------------------------------------------------------------
CCreateCharacterManager::~CCreateCharacterManager()
{
}
//----------------------------------------------------------------------------------
/*!
Инициализация
@return 
*/
void CCreateCharacterManager::Init()
{
	IFOR(i, 1, 11)
		g_Orion.ExecuteGump(m_FemaleHairStyleTable[i].GumpID);
	IFOR(i, 1, 10)
		g_Orion.ExecuteGump(m_MaleHairStyleTable[i].GumpID);
	IFOR(i, 1, 8)
		g_Orion.ExecuteGump(m_BeardStyleTable[i].GumpID);
}
//----------------------------------------------------------------------------------
/*!
Очистка
@return 
*/
void CCreateCharacterManager::Clear()
{
	m_Sex = false;
	m_HairStyle = 1;
	m_BeardStyle = 0;

	m_SkinTone = 0x03EA;
	m_ShirtColor = 0x0084;
	m_PantsColor = 0x035F;
	m_HairColor = 0x044E;
	m_BeardColor = 0x044E;
}
//----------------------------------------------------------------------------------
/*!
Получить стиль волос
@param [__in] pos Индекс волос
@return Структура с данными о волосах
*/
CC_HAIR_STYLE CCreateCharacterManager::GetHair(uchar pos) const
{
	if (pos < 0  || pos >= (10 + (int)m_Sex))
		return m_MaleHairStyleTable[0];

	if (m_Sex)
		return m_FemaleHairStyleTable[pos];

	return m_MaleHairStyleTable[pos];
}
//----------------------------------------------------------------------------------
/*!
Получить стиль бороды
@param [__in] pos Индекс бороды
@return Структура с данными о бороде
*/
CC_HAIR_STYLE CCreateCharacterManager::GetBeard(uchar pos) const
{
	if (pos < 0  || pos >= 8)
		return m_BeardStyleTable[0];

	return m_BeardStyleTable[pos];
}
//----------------------------------------------------------------------------------
