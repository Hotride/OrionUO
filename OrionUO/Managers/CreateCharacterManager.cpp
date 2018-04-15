// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** CreateCharacterManager.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CCreateCharacterManager g_CreateCharacterManager;
//----------------------------------------------------------------------------------
const CC_HAIR_STYLE CCreateCharacterManager::m_HumanMaleHairStyleTable[HUMAN_MALE_HAIR_COUNT] =
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
const CC_HAIR_STYLE CCreateCharacterManager::m_HumanFemaleHairStyleTable[HUMAN_FEMALE_HAIR_COUNT] =
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
const CC_HAIR_STYLE CCreateCharacterManager::m_HumanBeardStyleTable[HUMAN_MALE_FACIAL_HAIR_COUNT] =
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
const CC_HAIR_STYLE CCreateCharacterManager::m_ElfMaleHairStyleTable[ELF_MALE_HAIR_COUNT] =
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
const CC_HAIR_STYLE CCreateCharacterManager::m_ElfFemaleHairStyleTable[ELF_FEMALE_HAIR_COUNT] =
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
const CC_HAIR_STYLE CCreateCharacterManager::m_GargoyleMaleHairStyleTable[GARGOYLE_MALE_HAIR_COUNT] =
{
	{ 0, 0, "NONE" },
	{ 0x076C, 0x4258, "Horn Style 1" },
	{ 0x076D, 0x4259, "Horn Style 2" },
	{ 0x0773, 0x425A, "Horn Style 3" },
	{ 0x076E, 0x425B, "Horn Style 4" },
	{ 0x0774, 0x425C, "Horn Style 5" },
	{ 0x0775, 0x425D, "Horn Style 6" },
	{ 0x0776, 0x425E, "Horn Style 7" },
	{ 0x0777, 0x425F, "Horn Style 8" },
};
//----------------------------------------------------------------------------------
const CC_HAIR_STYLE CCreateCharacterManager::m_GargoyleFemaleHairStyleTable[GARGOYLE_FEMALE_HAIR_COUNT] =
{
	{ 0, 0, "NONE" },
	{ 0x07A0, 0x4261, "Horn Style 1" },
	{ 0x07A1, 0x4262, "Horn Style 2" },
	{ 0x079E, 0x4273, "Horn Style 3" },
	{ 0x07A2, 0x4274, "Horn Style 4" },
	{ 0x077B, 0x4275, "Horn Style 5" },
	{ 0x077C, 0x42AA, "Horn Style 6" },
	{ 0x077D, 0x42AB, "Horn Style 7" },
	{ 0x077E, 0x42B1, "Horn Style 8" },
};
//----------------------------------------------------------------------------------
const CC_HAIR_STYLE CCreateCharacterManager::m_GargoyleBeardStyleTable[GARGOYLE_MALE_FACIAL_HAIR_COUNT] =
{
	{ 0, 0, "NONE" },
	{ 0x076F, 0x42AD, "Horn Style 1" },
	{ 0x0770, 0x42AE, "Horn Style 2" },
	{ 0x0771, 0x42AF, "Horn Style 3" },
	{ 0x0772, 0x42B0, "Horn Style 4" },
};
//----------------------------------------------------------------------------------
const ushort CCreateCharacterManager::m_HumanSkinTone[HUMAN_SKIN_TONE_COUNT] =
{
	0x03E9, 0x03F1, 0x03F9, 0x0401, 0x0409, 0x0411, 0x0419, 0x0421, 0x03EA, 0x03F2,
	0x03FA, 0x0402, 0x040A, 0x0412, 0x041A, 0x0421, 0x03EB, 0x03F3, 0x03FB, 0x0403,
	0x040B, 0x0413, 0x041B, 0x0421, 0x03EC, 0x03F4, 0x03FC, 0x0404, 0x040C, 0x0414,
	0x041C, 0x0421, 0x03ED, 0x03F5, 0x03FD, 0x0405, 0x040D, 0x0415, 0x041D, 0x0421,
	0x03EE, 0x03F6, 0x03FE, 0x0406, 0x040E, 0x0416, 0x041E, 0x0421, 0x03EF, 0x03F7,
	0x03FF, 0x0407, 0x040F, 0x0417, 0x041F, 0x0421, 0x03F0, 0x03F8, 0x0400, 0x0408,
	0x0410, 0x0418, 0x0420, 0x0421
};
//----------------------------------------------------------------------------------
const ushort CCreateCharacterManager::m_ElfSkinTone[ELF_SKIN_TONE_COUNT] =
{
	0x04DD, 0x076B, 0x0834, 0x042F, 0x024C, 0x024D, 0x024E, 0x00BE, 0x04A6, 0x0360,
	0x0374, 0x0366, 0x03E7, 0x03DD, 0x0352, 0x0902, 0x076C, 0x0383, 0x0578, 0x03E8,
	0x0373, 0x0388, 0x0384, 0x0375, 0x053E, 0x0380, 0x0381, 0x0382, 0x076A, 0x03E4,
	0x051C, 0x03E5
};
//----------------------------------------------------------------------------------
const ushort CCreateCharacterManager::m_GargoyleSkinTone[GARGOYLE_SKIN_TONE_COUNT] =
{
	0x06DA, 0x06DB, 0x06DC, 0x06DD, 0x06DE, 0x06DF, 0x06E0, 0x06E1, 0x06E2, 0x06E3,
	0x06E4, 0x06E5, 0x06E6, 0x06E7, 0x06E8, 0x06E9, 0x06EA, 0x06EB, 0x06EC, 0x06ED,
	0x06EE, 0x06EF, 0x06F0, 0x06F1, 0x06F2, 0x06DA, 0x06DB, 0x06DC
};
//----------------------------------------------------------------------------------
const ushort CCreateCharacterManager::m_HumanHairColor[HUMAN_HAIR_COLOR_COUNT] =
{
	0x044D, 0x0455, 0x045D, 0x0465, 0x046D, 0x0475, 0x044E, 0x0456, 0x045E, 0x0466,
	0x046E, 0x0476, 0x044F, 0x0457, 0x045F, 0x0467, 0x046F, 0x0477, 0x0450, 0x0458,
	0x0460, 0x0468, 0x0470, 0x0478, 0x0451, 0x0459, 0x0461, 0x0469, 0x0471, 0x0479,
	0x0452, 0x045A, 0x0462, 0x046A, 0x0472, 0x047A, 0x0453, 0x045B, 0x0463, 0x046B,
	0x0473, 0x047B, 0x0454, 0x045C, 0x0464, 0x046C, 0x0474, 0x047C
};
//----------------------------------------------------------------------------------
const ushort CCreateCharacterManager::m_ElfHairColor[ELF_HAIR_COLOR_COUNT] =
{
	0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0100, 0x06B7, 0x0206, 0x0210,
	0x026B, 0x02C2, 0x02C8, 0x01E3, 0x0238, 0x0368, 0x059C, 0x0852, 0x008D, 0x008E,
	0x008F, 0x0090, 0x0091, 0x0158, 0x0159, 0x015A, 0x015B, 0x015C, 0x015D, 0x01BC,
	0x0724, 0x0057, 0x0127, 0x012E, 0x01F2, 0x0250, 0x031C, 0x031D, 0x031E, 0x031F,
	0x0320, 0x0321, 0x0322, 0x0323, 0x0324, 0x0325, 0x0385, 0x0386, 0x0387, 0x0388,
	0x0389, 0x0385, 0x0386, 0x0387
};
//----------------------------------------------------------------------------------
const ushort CCreateCharacterManager::m_GargoyleHairColor[GARGOYLE_HAIR_COLOR_COUNT] =
{
	0x0708, 0x070A, 0x070C, 0x070E, 0x0710, 0x0762, 0x0764, 0x0767, 0x076A, 0x06F2,
	0x06F0, 0x06EE, 0x06E3, 0x06E1, 0x06DF, 0x0708, 0x070A, 0x070C
};
//----------------------------------------------------------------------------------
CCreateCharacterManager::CCreateCharacterManager()
{
}
//----------------------------------------------------------------------------------
CCreateCharacterManager::~CCreateCharacterManager()
{
}
//----------------------------------------------------------------------------------
void CCreateCharacterManager::SetFemale(const bool &female)
{
	WISPFUN_DEBUG("c140_f1");
	m_Female = female;

	if (HairStyle >= GetCurrentHairCount())
		HairStyle = 0;

	if (!m_Female && BeardStyle >= GetCurrentFacialHairCount())
		BeardStyle = 0;
}
//----------------------------------------------------------------------------------
void CCreateCharacterManager::SetRace(const RACE_TYPE &newRace)
{
	WISPFUN_DEBUG("c140_f2");
	m_Race = newRace;
	SetFemale(m_Female);

	SkinTone = (*GetSkinTonePtr()) + 1;
	pushort ptr = GetHairColorPtr();
	HairColor = (*ptr) + 1;
	BeardColor = (*ptr) + 1;
}
//----------------------------------------------------------------------------------
/*!
Инициализация
@return 
*/
void CCreateCharacterManager::Init()
{
	WISPFUN_DEBUG("c140_f3");
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
	WISPFUN_DEBUG("c140_f4");
	m_Female = false;
	HairStyle = 1;
	BeardStyle = 0;
	m_Race = RT_HUMAN;

	SkinTone = 0x03EA;
	ShirtColor = 0x0084;
	PantsColor = 0x035F;
	HairColor = 0x044E;
	BeardColor = 0x044E;
}
//----------------------------------------------------------------------------------
int CCreateCharacterManager::GetCurrentHairCount()
{
	WISPFUN_DEBUG("c140_f5");
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
	WISPFUN_DEBUG("c140_f6");
	static const int count[3] = { HUMAN_MALE_FACIAL_HAIR_COUNT, 0, GARGOYLE_MALE_FACIAL_HAIR_COUNT };

	return count[m_Race - 1];
}
//----------------------------------------------------------------------------------
ushort CCreateCharacterManager::GetBodyGump()
{
	WISPFUN_DEBUG("c140_f7");
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
	WISPFUN_DEBUG("c140_f8");
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
	WISPFUN_DEBUG("c140_f9");
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
	WISPFUN_DEBUG("c140_f10");
	static const ushort gump[3][2] =
	{
		{ 0x0762, 0x0763 },
		{ 0x0762, 0x0763 },
		{ 0, 0 }
	};

	return gump[m_Race - 1][m_Female];
}
//----------------------------------------------------------------------------------
pushort CCreateCharacterManager::GetSkinTonePtr()
{
	WISPFUN_DEBUG("c140_f11");
	static const pushort ptr[3] =
	{
		(pushort)&m_HumanSkinTone[0],
		(pushort)&m_ElfSkinTone[0],
		(pushort)&m_GargoyleSkinTone[0]
	};

	return ptr[m_Race - 1];
}
//----------------------------------------------------------------------------------
pushort CCreateCharacterManager::GetHairColorPtr()
{
	WISPFUN_DEBUG("c140_f12");
	static const pushort ptr[3] =
	{
		(pushort)&m_HumanHairColor[0],
		(pushort)&m_ElfHairColor[0],
		(pushort)&m_GargoyleHairColor[0]
	};

	return ptr[m_Race - 1];
}
//----------------------------------------------------------------------------------
/*!
Получить стиль волос
@param [__in] pos Индекс волос
@return Структура с данными о волосах
*/
CC_HAIR_STYLE CCreateCharacterManager::GetHair(const uchar &pos) const
{
	WISPFUN_DEBUG("c140_f13");
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
	else if (m_Race == RT_GARGOYLE)
	{
		if (m_Female)
		{
			if (pos < GARGOYLE_FEMALE_HAIR_COUNT)
				return m_GargoyleFemaleHairStyleTable[pos];
		}
		else
		{
			if (pos < GARGOYLE_MALE_HAIR_COUNT)
				return m_GargoyleMaleHairStyleTable[pos];
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
	WISPFUN_DEBUG("c140_f14");
	if (m_Race == RT_HUMAN)
	{
		if (pos < HUMAN_MALE_FACIAL_HAIR_COUNT)
			return m_HumanBeardStyleTable[pos];
	}
	else if (m_Race == RT_GARGOYLE)
	{
		if (pos < GARGOYLE_MALE_FACIAL_HAIR_COUNT)
			return m_GargoyleBeardStyleTable[pos];
	}

	return m_HumanBeardStyleTable[0];
}
//----------------------------------------------------------------------------------
