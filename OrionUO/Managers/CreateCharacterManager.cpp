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
CC_HAIR_STYLE CCreateCharacterManager::m_HumanMaleHairStyleTable[HUMAN_MALE_HAIR_COUNT] =
{
	{ 0, 0, "NONE" },
	{ 0x0753, 0x203B, "Short" },
	{ 0x0754, 0x203C, "Long" },
	{ 0x0757, 0x203D, "Pony Tail" },
	{ 0x0755, 0x2044, "Mohawk" },
	{ 0x074F, 0x2045, "Pageboy" },
	{ 0x0752, 0x204A, "Topknot" },
	{ 0x0751, 0x2047, "Curly" },
	{ 0x0758, 0x2048, "Receding" },
	{ 0x074E, 0x2049, "2 Tails" }
};
//----------------------------------------------------------------------------------
CC_HAIR_STYLE CCreateCharacterManager::m_HumanFemaleHairStyleTable[HUMAN_FEMALE_HAIR_COUNT] =
{
	{ 0, 0, "NONE" },
	{ 0x0737, 0x203B, "Short" },
	{ 0x0732, 0x203C, "Long" },
	{ 0x0735, 0x203D, "Pony Tail" },
	{ 0x0733, 0x2044, "Mohawk" },
	{ 0x0734, 0x2045, "Pageboy" },
	{ 0x0730, 0x204A, "Topknot" },
	{ 0x072F, 0x2047, "Curly" },
	{ 0x0736, 0x2048, "Receding" },
	{ 0x072C, 0x2049, "2 Tails" },
	{ 0x0731, 0x2046, "Buns" }
};
//----------------------------------------------------------------------------------
CC_HAIR_STYLE CCreateCharacterManager::m_HumanBeardStyleTable[HUMAN_MALE_FACIAL_HAIR_COUNT] =
{
	{ 0, 0, "NONE" },
	{ 0x0759, 0x2040, "Goatee" },
	{ 0x075B, 0x203E, "Long beard" },
	{ 0x075D, 0x203F, "Short beard" },
	{ 0x075C, 0x2041, "Moustache" },
	{ 0x075E, 0x204B, "Short beard/moustache" },
	{ 0x075A, 0x204C, "Long beard/moustache" },
	{ 0x075F, 0x204D, "Vandyke" }
};
//----------------------------------------------------------------------------------
CC_HAIR_STYLE CCreateCharacterManager::m_ElfMaleHairStyleTable[ELF_MALE_HAIR_COUNT] =
{
	{ 0, 0, "NONE" },
	{ 0x06F8, 0x2FBF, "Mid Long" },
	{ 0x06F9, 0x2FC0, "Long Feather" },
	{ 0x06FA, 0x2FC1, "Short" },
	{ 0x06FB, 0x2FC2, "Mullet" },
	{ 0x06FD, 0x2FCD, "Long" },
	{ 0x06FE, 0x2FCE, "Topknot" },
	{ 0x06FF, 0x2FCF, "Long Braid" },
	{ 0x0701, 0x2FD1, "Spiked" }
};
//----------------------------------------------------------------------------------
CC_HAIR_STYLE CCreateCharacterManager::m_ElfFemaleHairStyleTable[ELF_FEMALE_HAIR_COUNT] =
{
	{ 0, 0, "NONE" },
	{ 0x06EF, 0x2FC0, "Long Feather" },
	{ 0x06F0, 0x2FC1, "Short" },
	{ 0x06F1, 0x2FC2, "Mullet" },
	{ 0x06F2, 0x2FCC, "Flower" },
	{ 0x06F4, 0x2FCE, "Topknot" },
	{ 0x06F5, 0x2FCF, "Long Braid" },
	{ 0x06F6, 0x2FD0, "Buns" },
	{ 0x06F7, 0x2FD1, "Spiked" }
};
//----------------------------------------------------------------------------------
CCreateCharacterManager::CCreateCharacterManager()
:m_Female(false), m_HairStyle(1), m_BeardStyle(0), m_SkinTone(0x03EA),
m_ShirtColor(0x0084), m_PantsColor(0x035F), m_HairColor(0x044E),
m_BeardColor(0x044E), m_Race(RT_HUMAN)
{
}
//----------------------------------------------------------------------------------
CCreateCharacterManager::~CCreateCharacterManager()
{
}
//----------------------------------------------------------------------------------
void CCreateCharacterManager::OnSexChanged(const bool &female)
{
	m_Female = female;

	if (m_HairStyle >= GetCurrentHairCount())
		m_HairStyle = 0;

	if (!m_Female && m_BeardStyle >= GetCurrentFacialHairCount())
		m_BeardStyle = 0;
}
//----------------------------------------------------------------------------------
void CCreateCharacterManager::OnChangeRace(const RACE_TYPE &newRace)
{
	m_Race = newRace;
	OnSexChanged(m_Female);

	if (newRace == RT_HUMAN)
	{
		/*
		m_SkinTone = 0x03EA;
		m_ShirtColor = 0x0084;
		m_PantsColor = 0x035F;
		m_HairColor = 0x044E;
		m_BeardColor = 0x044E;
		*/
	}
}
//----------------------------------------------------------------------------------
/*!
Инициализация
@return 
*/
void CCreateCharacterManager::Init()
{
	IFOR(i, 1, HUMAN_MALE_HAIR_COUNT)
		g_Orion.ExecuteGump(m_HumanMaleHairStyleTable[i].GumpID);

	IFOR(i, 1, HUMAN_FEMALE_HAIR_COUNT)
		g_Orion.ExecuteGump(m_HumanFemaleHairStyleTable[i].GumpID);

	IFOR(i, 1, HUMAN_MALE_FACIAL_HAIR_COUNT)
		g_Orion.ExecuteGump(m_HumanBeardStyleTable[i].GumpID);

	IFOR(i, 1, ELF_MALE_HAIR_COUNT)
		g_Orion.ExecuteGump(m_ElfMaleHairStyleTable[i].GumpID);

	IFOR(i, 1, ELF_FEMALE_HAIR_COUNT)
		g_Orion.ExecuteGump(m_ElfFemaleHairStyleTable[i].GumpID);
}
//----------------------------------------------------------------------------------
/*!
Очистка
@return 
*/
void CCreateCharacterManager::Clear()
{
	m_Female = false;
	m_HairStyle = 1;
	m_BeardStyle = 0;
	m_Race = RT_HUMAN;

	m_SkinTone = 0x03EA;
	m_ShirtColor = 0x0084;
	m_PantsColor = 0x035F;
	m_HairColor = 0x044E;
	m_BeardColor = 0x044E;
}
//----------------------------------------------------------------------------------
int CCreateCharacterManager::GetCurrentHairCount()
{
	static const int count[3][2] =
	{
		{ HUMAN_MALE_HAIR_COUNT, HUMAN_FEMALE_HAIR_COUNT },
		{ ELF_MALE_HAIR_COUNT, ELF_FEMALE_HAIR_COUNT },
		{ GARGOYLE_MALE_HAIR_COUNT, GARGOYLE_FEMALE_HAIR_COUNT }
	};

	return count[m_Race - 1][m_Female];
}
//----------------------------------------------------------------------------------
int CCreateCharacterManager::GetCurrentFacialHairCount()
{
	static const int count[3] = { HUMAN_MALE_FACIAL_HAIR_COUNT, 0, GARGOYLE_MALE_FACIAL_HAIR_COUNT };

	return count[m_Race - 1];
}
//----------------------------------------------------------------------------------
ushort CCreateCharacterManager::GetBodyGump()
{
	static const ushort gump[3][2] =
	{
		{ 0x0761, 0x0760 },
		{ 0x0766, 0x0765 },
		{ 0x076B, 0x076A }
	};

	return gump[m_Race - 1][m_Female];
}
//----------------------------------------------------------------------------------
ushort CCreateCharacterManager::GetShirtGump()
{
	static const ushort gump[3][2] =
	{
		{ 0x0739, 0x0714 },
		{ 0x0739, 0x0714 },
		{ 0x0778, 0x07A7 }
	};

	return gump[m_Race - 1][m_Female];
}
//----------------------------------------------------------------------------------
ushort CCreateCharacterManager::GetPantsGump()
{
	static const ushort gump[3][2] =
	{
		{ 0x0738, 0x0764 },
		{ 0x0738, 0x0764 },
		{ 0, 0 }
	};

	return gump[m_Race - 1][m_Female];
}
//----------------------------------------------------------------------------------
ushort CCreateCharacterManager::GetBootsGump()
{
	static const ushort gump[3][2] =
	{
		{ 0x0762, 0x0763 },
		{ 0x0762, 0x0763 },
		{ 0, 0 }
	};

	return gump[m_Race - 1][m_Female];
}
//----------------------------------------------------------------------------------
/*!
Получить стиль волос
@param [__in] pos Индекс волос
@return Структура с данными о волосах
*/
CC_HAIR_STYLE CCreateCharacterManager::GetHair(const uchar &pos) const
{
	if (m_Race == RT_HUMAN)
	{
		if (m_Female)
		{
			if (pos < HUMAN_FEMALE_HAIR_COUNT)
				return m_HumanFemaleHairStyleTable[pos];
		}
		else
		{
			if (pos < HUMAN_MALE_HAIR_COUNT)
				return m_HumanMaleHairStyleTable[pos];
		}
	}
	else if (m_Race == RT_ELF)
	{
		if (m_Female)
		{
			if (pos < ELF_FEMALE_HAIR_COUNT)
				return m_ElfFemaleHairStyleTable[pos];
		}
		else
		{
			if (pos < ELF_MALE_HAIR_COUNT)
				return m_ElfMaleHairStyleTable[pos];
		}
	}

	return m_HumanMaleHairStyleTable[0];
}
//----------------------------------------------------------------------------------
/*!
Получить стиль бороды
@param [__in] pos Индекс бороды
@return Структура с данными о бороде
*/
CC_HAIR_STYLE CCreateCharacterManager::GetBeard(const uchar &pos) const
{
	if (pos < HUMAN_MALE_FACIAL_HAIR_COUNT)
		return m_HumanBeardStyleTable[pos];

	return m_HumanBeardStyleTable[0];
}
//----------------------------------------------------------------------------------
