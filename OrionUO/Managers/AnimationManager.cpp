/***********************************************************************************
**
** AnimationManager.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "AnimationManager.h"
#include "../MulStruct.h"
#include "../Wisp/WispTextFileParser.h"
#include "FileManager.h"
#include "ColorManager.h"
#include "../GLEngine/GLEngine.h"
#include "../Game objects/GameWorld.h"
#include "../Game objects/GamePlayer.h"
#include "MouseManager.h"
#include "../OrionUO.h"
#include "../Target.h"
#include "../TargetGump.h"
#include "ConfigManager.h"
#include "../SelectedObject.h"
#include "../Screen stages/GameScreen.h"
#include "PacketManager.h"
//----------------------------------------------------------------------------------
CAnimationManager g_AnimationManager;
//----------------------------------------------------------------------------------
#pragma region layers table
const int CAnimationManager::m_UsedLayers[8][USED_LAYER_COUNT] =
{
	{ //dir 0
		OL_SHIRT,
		OL_PANTS,
		OL_SHOES,
		OL_LEGS,
		OL_TORSO,
		OL_RING,
		OL_TALISMAN,
		OL_BRACELET,
		OL_15,
		OL_ARMS,
		OL_GLOVES,
		OL_SKIRT,
		OL_TUNIC,
		OL_ROBE,
		OL_WAIST,
		OL_NECKLACE,
		OL_HAIR,
		OL_BEARD,
		OL_EARRINGS,
		OL_HELMET,
		OL_1_HAND,
		OL_2_HAND,
		OL_CLOAK,
	},
	{ //dir 1
		OL_SHIRT,
		OL_PANTS,
		OL_SHOES,
		OL_LEGS,
		OL_TORSO,
		OL_RING,
		OL_TALISMAN,
		OL_BRACELET,
		OL_15,
		OL_ARMS,
		OL_GLOVES,
		OL_SKIRT,
		OL_TUNIC,
		OL_ROBE,
		OL_WAIST,
		OL_NECKLACE,
		OL_HAIR,
		OL_BEARD,
		OL_EARRINGS,
		OL_HELMET,
		OL_1_HAND,
		OL_2_HAND,
		OL_CLOAK,
	},
	{ //dir 2
		OL_SHIRT,
		OL_PANTS,
		OL_SHOES,
		OL_LEGS,
		OL_TORSO,
		OL_RING,
		OL_TALISMAN,
		OL_BRACELET,
		OL_15,
		OL_ARMS,
		OL_GLOVES,
		OL_SKIRT,
		OL_TUNIC,
		OL_ROBE,
		OL_WAIST,
		OL_NECKLACE,
		OL_HAIR,
		OL_BEARD,
		OL_EARRINGS,
		OL_HELMET,
		OL_1_HAND,
		OL_CLOAK,
		OL_2_HAND,
	},
	{ //dir 3
		OL_CLOAK,
		OL_SHIRT,
		OL_PANTS,
		OL_SHOES,
		OL_LEGS,
		OL_TORSO,
		OL_RING,
		OL_TALISMAN,
		OL_BRACELET,
		OL_15,
		OL_ARMS,
		OL_GLOVES,
		OL_SKIRT,
		OL_TUNIC,
		OL_ROBE,
		OL_WAIST,
		OL_NECKLACE,
		OL_HAIR,
		OL_BEARD,
		OL_EARRINGS,
		OL_HELMET,
		OL_1_HAND,
		OL_2_HAND,
	},
	{ //dir 4
		OL_SHIRT,
		OL_PANTS,
		OL_SHOES,
		OL_LEGS,
		OL_TORSO,
		OL_RING,
		OL_TALISMAN,
		OL_BRACELET,
		OL_15,
		OL_ARMS,
		OL_GLOVES,
		OL_SKIRT,
		OL_TUNIC,
		OL_ROBE,
		OL_WAIST,
		OL_NECKLACE,
		OL_HAIR,
		OL_BEARD,
		OL_EARRINGS,
		OL_HELMET,
		OL_1_HAND,
		OL_CLOAK,
		OL_2_HAND,
	},
	{ //dir 5
		OL_SHIRT,
		OL_PANTS,
		OL_SHOES,
		OL_LEGS,
		OL_TORSO,
		OL_RING,
		OL_TALISMAN,
		OL_BRACELET,
		OL_15,
		OL_ARMS,
		OL_GLOVES,
		OL_SKIRT,
		OL_TUNIC,
		OL_ROBE,
		OL_WAIST,
		OL_NECKLACE,
		OL_HAIR,
		OL_BEARD,
		OL_EARRINGS,
		OL_HELMET,
		OL_1_HAND,
		OL_2_HAND,
		OL_CLOAK,
	},
	{ //dir 6
		OL_SHIRT,
		OL_PANTS,
		OL_SHOES,
		OL_LEGS,
		OL_TORSO,
		OL_RING,
		OL_TALISMAN,
		OL_BRACELET,
		OL_15,
		OL_ARMS,
		OL_GLOVES,
		OL_SKIRT,
		OL_TUNIC,
		OL_ROBE,
		OL_WAIST,
		OL_NECKLACE,
		OL_HAIR,
		OL_BEARD,
		OL_EARRINGS,
		OL_HELMET,
		OL_1_HAND,
		OL_2_HAND,
		OL_CLOAK,
	},
	{ //dir 7
		OL_SHIRT,
		OL_PANTS,
		OL_SHOES,
		OL_LEGS,
		OL_TORSO,
		OL_RING,
		OL_TALISMAN,
		OL_BRACELET,
		OL_15,
		OL_ARMS,
		OL_GLOVES,
		OL_SKIRT,
		OL_TUNIC,
		OL_ROBE,
		OL_WAIST,
		OL_NECKLACE,
		OL_HAIR,
		OL_BEARD,
		OL_EARRINGS,
		OL_HELMET,
		OL_1_HAND,
		OL_2_HAND,
		OL_CLOAK,
	},
};
#pragma endregion
//----------------------------------------------------------------------------------
CAnimationManager::CAnimationManager()
: WISP_DATASTREAM::CDataReader(), m_UsedAnimList(NULL), m_Color(0), m_AnimGroup(0),
m_Direction(0), m_Sitting(0), m_Transform(false), m_UseBlending(false)
{
	memset(m_AddressIdx, 0, sizeof(m_AddressIdx));
	memset(m_AddressMul, 0, sizeof(m_AddressMul));
	memset(m_SizeIdx, 0, sizeof(m_SizeIdx));
}
//----------------------------------------------------------------------------------
CAnimationManager::~CAnimationManager()
{
	ClearUnusedTextures(g_Ticks + 100000);
}
//----------------------------------------------------------------------------------
/*!
Загрузка данных
@param [__in] verdata Ссылка на адрес в памяти файла патчей (verdata.mul)
@return 
*/
void CAnimationManager::Load(puint verdata)
{
	uint maxAddress = m_AddressIdx[0] + m_SizeIdx[0];

	IFOR(i, 0, MAX_ANIMATIONS_DATA_INDEX_COUNT)
	{
		CIndexAnimation &index = m_DataIndex[i];

		ANIMATION_GROUPS_TYPE groupType = AGT_UNKNOWN;
		uint findID = 0;

		if (i >= 200)
		{
			if (i >= 400) //People
			{
				groupType = AGT_HUMAN;
				findID = (((i - 400) * 175) + 35000) * sizeof(ANIM_IDX_BLOCK);
			}
			else //Low
			{
				groupType = AGT_ANIMAL;
				findID = (((i - 200) * 65) + 22000) * sizeof(ANIM_IDX_BLOCK);
			}
		}
		else //Hight
		{
			groupType = AGT_MONSTER;
			findID = (i * 110) * sizeof(ANIM_IDX_BLOCK);
		}

		if (findID >= m_SizeIdx[0])
			break;

		index.Graphic = i;

		if (index.Type != AGT_UNKNOWN)
			groupType = index.Type;

		int count = 0;

		switch (groupType)
		{
			case AGT_MONSTER:
			case AGT_SEA_MONSTER:
			{
				count = 22;
				break;
			}
			case AGT_HUMAN:
			case AGT_EQUIPMENT:
			{
				count = 35;
				break;
			}
			case AGT_ANIMAL:
			default:
			{
				count = 13;
				break;
			}
		}

		index.Type = groupType;

		uint address = m_AddressIdx[0] + findID;

		IFOR(j, 0, count)
		{
			CTextureAnimationGroup &group = index.m_Groups[j];
			int offset = j * 5;

			IFOR(d, 0, 5)
			{
				CTextureAnimationDirection &direction = group.m_Direction[d];

				PANIM_IDX_BLOCK aidx = (PANIM_IDX_BLOCK)(address + ((offset + d) * sizeof(ANIM_IDX_BLOCK)));

				if ((uint)aidx >= maxAddress)
					break;

				if (aidx->Size && aidx->Position != 0xFFFFFFFF && aidx->Size != 0xFFFFFFFF)
				{
					direction.Address = m_AddressMul[0] + aidx->Position;
					direction.Size = aidx->Size;
				}
			}
		}
	}

	if (verdata != NULL)
	{
		int dataCount = *verdata;

		IFOR(j, 0, dataCount)
		{
			PVERDATA_HEADER vh = (PVERDATA_HEADER)((uint)verdata + 4 + (j * sizeof(VERDATA_HEADER)));

			if (vh->FileID == 0x06) //Anim
			{
				ANIMATION_GROUPS_TYPE groupType = AGT_HUMAN;
				uint graphic = vh->BlockID;
				ushort id = 0xFFFF;
				uint group = 0;
				uint dir = 0;
				uint offset = 0;
				int count = 0;

				//LOG("vh->ID = 0x%02X vh->BlockID = 0x%08X\n", vh->FileID, graphic);

				if (graphic < 35000)
				{
					if (graphic < 22000) //monsters
					{
						count = 22;
						groupType = AGT_MONSTER;
						id = graphic / 110;
						offset = graphic - (id * 110);
					}
					else //animals
					{
						count = 13;
						groupType = AGT_ANIMAL;
						id = (graphic - 22000) / 65;
						offset = graphic - ((id * 65) + 22000);
						id += 200;
					}
				}
				else //humans
				{
					groupType = AGT_HUMAN;
					count = 35;
					id = (graphic - 35000) / 175;
					offset = graphic - ((id * 175) + 35000);
					id += 400;
				}

				group = offset / 5;
				dir = offset % 5;

				if (id >= MAX_ANIMATIONS_DATA_INDEX_COUNT)
				{
					LOG("Invalid animation patch 0x%04X (0x%08X)\n", id, graphic);
					continue;
				}
				else if (group >= (uint)count)
				{
					LOG("Invalid group index: %i in animation patch 0x%04X (0x%08X)\n", group, id, graphic);
					continue;
				}

				CIndexAnimation &index = m_DataIndex[id];

				CTextureAnimationDirection &direction = index.m_Groups[group].m_Direction[dir];

				direction.Address = (uint)g_FileManager.m_VerdataMul.Start + vh->Position;
				direction.Size = vh->Size;

				index.Graphic = id;
				index.Type = groupType;
			}
		}
	}
}
//----------------------------------------------------------------------------------
/*!
Загрузка файла корректора индексов картинок анимаций
@param [__in] verdata Ссылка на адрес в памяти файла патчей (verdata.mul)
@return 
*/
void CAnimationManager::InitIndexReplaces(puint verdata)
{
	if (g_PacketManager.ClientVersion >= CV_500A)
	{
		static const string typeNames[5] = { "animal", "monster", "sea_monster", "human", "equipment" };

		WISP_FILE::CTextFileParser mobtypesParser(g_App.FilePath("mobtypes.txt").c_str(), " \t", "#;//", "");

		while (!mobtypesParser.IsEOF())
		{
			STRING_LIST strings = mobtypesParser.ReadTokens();

			if (strings.size() >= 3)
			{
				ushort index = atoi(strings[0].c_str());

				if (index >= MAX_ANIMATIONS_DATA_INDEX_COUNT)
					continue;

				string testType = ToLowerA(strings[1]);

				IFOR(i, 0, 5)
				{
					if (testType == typeNames[i])
						m_DataIndex[index].Type = (ANIMATION_GROUPS_TYPE)i;
				}
			}
		}
	}

	Load(verdata);

	if (g_PacketManager.ClientVersion < CV_305D) //CV_204C
		return;

	WISP_FILE::CTextFileParser newBodyParser("", " \t,{}", "#;//", "");
	WISP_FILE::CTextFileParser bodyParser(g_App.FilePath("Body.def").c_str(), " \t", "#;//", "{}");
	WISP_FILE::CTextFileParser bodyconvParser(g_App.FilePath("Bodyconv.def").c_str(), " \t", "#;//", "");
	WISP_FILE::CTextFileParser corpseParser(g_App.FilePath("Corpse.def").c_str(), " \t", "#;//", "{}");

	WISP_FILE::CTextFileParser animParser[4]
	{
		WISP_FILE::CTextFileParser(g_App.FilePath("Anim1.def").c_str(), " \t", "#;//", "{}"),
		WISP_FILE::CTextFileParser(g_App.FilePath("Anim2.def").c_str(), " \t", "#;//", "{}"),
		WISP_FILE::CTextFileParser(g_App.FilePath("Anim3.def").c_str(), " \t", "#;//", "{}"),
		WISP_FILE::CTextFileParser(g_App.FilePath("Anim4.def").c_str(), " \t", "#;//", "{}")
	};

	while (!bodyconvParser.IsEOF())
	{
		STRING_LIST strings = bodyconvParser.ReadTokens();

		if (strings.size() >= 2)
		{
			ushort index = atoi(strings[0].c_str());

			if (index >= MAX_ANIMATIONS_DATA_INDEX_COUNT)
				continue;

			int anim[4] =
			{
				atoi(strings[1].c_str()),
				-1,
				-1,
				-1
			};

			if (strings.size() >= 3)
			{
				anim[1] = atoi(strings[2].c_str());

				if (strings.size() >= 4)
				{
					anim[2] = atoi(strings[3].c_str());

					if (strings.size() >= 5)
						anim[3] = atoi(strings[4].c_str());
				}
			}

			int startAnimID = -1;
			int animFile = 1;
			ushort realAnimID = 0;
			char mountedHeightOffset = 0;

			if (anim[0] != -1 && m_AddressIdx[2] != 0 && m_AddressMul[2] != 0)
			{
				animFile = 2;

				realAnimID = anim[0];

				if (realAnimID == 68)
					realAnimID = 122;

				if (realAnimID >= 200) //Low
					startAnimID = ((realAnimID - 200) * 65) + 22000;
				else //Hight
					startAnimID = realAnimID * 110;
			}
			else if (anim[1] != -1 && m_AddressIdx[3] != 0 && m_AddressMul[3] != 0)
			{
				animFile = 3;
				realAnimID = anim[1];

				if (realAnimID >= 200)
				{
					if (realAnimID >= 400) //People
						startAnimID = ((realAnimID - 400) * 175) + 35000;
					else //Low
						startAnimID = ((realAnimID - 200) * 110) + 22000;
				}
				else //Hight
					startAnimID = (realAnimID * 65) + 9000;
			}
			else if (anim[2] != -1 && m_AddressIdx[4] != 0 && m_AddressMul[4] != 0)
			{
				animFile = 4;
				realAnimID = anim[2];

				if (realAnimID >= 200)
				{
					if (realAnimID >= 400) //People
						startAnimID = ((realAnimID - 400) * 175) + 35000;
					else //Low
						startAnimID = ((realAnimID - 200) * 65) + 22000;
				}
				else //Hight
					startAnimID = realAnimID * 110;
			}
			else if (anim[3] != -1 && m_AddressIdx[5] != 0 && m_AddressMul[5] != 0)
			{
				animFile = 5;
				realAnimID = anim[3];
				mountedHeightOffset = -9;

				if (realAnimID == 34)
					startAnimID = ((realAnimID - 200) * 65) + 22000;
				else if (realAnimID >= 200)
				{
					if (realAnimID >= 400) //People
						startAnimID = ((realAnimID - 400) * 175) + 35000;
					else //Low
						startAnimID = ((realAnimID - 200) * 65) + 22000;
				}
				else //Hight
					startAnimID = realAnimID * 110;
			}

			if (animFile != 1 && startAnimID != -1)
			{
				startAnimID = startAnimID * sizeof(ANIM_IDX_BLOCK);

				if ((uint)startAnimID < m_SizeIdx[animFile])
				{
					CIndexAnimation &dataIndex = m_DataIndex[index];
					dataIndex.MountedHeightOffset = mountedHeightOffset;

					if (g_PacketManager.ClientVersion < CV_500A)
					{
						if (realAnimID >= 200)
						{
							if (realAnimID >= 400) //People
								dataIndex.Type = AGT_HUMAN;
							else //Low
								dataIndex.Type = AGT_ANIMAL;
						}
						else
							dataIndex.Type = AGT_MONSTER;
					}

					int count = 0;

					switch (dataIndex.Type)
					{
						case AGT_MONSTER:
						case AGT_SEA_MONSTER:
						{
							count = 22;
							break;
						}
						case AGT_HUMAN:
						case AGT_EQUIPMENT:
						{
							count = 35;
							break;
						}
						case AGT_ANIMAL:
						default:
						{
							count = 13;
							break;
						}
					}

					uint address = m_AddressIdx[animFile] + startAnimID;
					uint maxAddress = m_AddressIdx[animFile] + m_SizeIdx[animFile];

					IFOR(j, 0, count)
					{
						CTextureAnimationGroup &group = dataIndex.m_Groups[j];
						int offset = j * 5;

						IFOR(d, 0, 5)
						{
							CTextureAnimationDirection &direction = group.m_Direction[d];

							PANIM_IDX_BLOCK aidx = (PANIM_IDX_BLOCK)(address + ((offset + d) * sizeof(ANIM_IDX_BLOCK)));

							if ((uint)aidx >= maxAddress)
								break;

							if (aidx->Size && aidx->Position != 0xFFFFFFFF && aidx->Size != 0xFFFFFFFF)
							{
								direction.Address = m_AddressMul[animFile] + aidx->Position;
								direction.Size = aidx->Size;
							}
						}
					}
				}
			}
		}
	}

	/*IFOR(i, 0, 4)
	{
		STRING_LIST strings = animParser[i].ReadTokens();

		if (strings.size() >= 3)
		{
			ushort index = atoi(strings[0].c_str());

			if (index >= MAX_ANIMATIONS_DATA_INDEX_COUNT || m_DataIndex[index].Offset)
				continue;

			STRING_LIST newBody = newBodyParser.GetTokens(strings[1].c_str());

			IFOR(j, 0, (int)newBody.size())
			{
				ushort checkIndex = atoi(newBody[j].c_str());

				if (checkIndex >= MAX_ANIMATIONS_DATA_INDEX_COUNT || !m_DataIndex[checkIndex].Offset)
					continue;

				//memcpy(&m_DataIndex[index], &m_DataIndex[checkIndex], sizeof(CIndexAnimation));

				if (g_PacketManager.ClientVersion < CV_500A)
				{
					if (checkIndex >= 200)
					{
						if (checkIndex >= 400) //People
							m_DataIndex[index].Type = AGT_HUMAN;
						else //Low
							m_DataIndex[index].Type = AGT_ANIMAL;
					}
					else
						m_DataIndex[index].Type = AGT_MONSTER;
				}

				m_DataIndex[index].Graphic = checkIndex;
				m_DataIndex[index].Color = atoi(strings[2].c_str());

				break;
			}
		}
	}*/

	while (!bodyParser.IsEOF())
	{
		STRING_LIST strings = bodyParser.ReadTokens();

		if (strings.size() >= 3)
		{
			ushort index = atoi(strings[0].c_str());

			STRING_LIST newBody = newBodyParser.GetTokens(strings[1].c_str());

			if (index >= MAX_ANIMATIONS_DATA_INDEX_COUNT || !newBody.size())
				continue;

			ushort checkIndex = atoi(newBody[0].c_str());

			if (checkIndex >= MAX_ANIMATIONS_DATA_INDEX_COUNT)
				continue;

			CIndexAnimation &dataIndex = m_DataIndex[index];
			CIndexAnimation &checkDataIndex = m_DataIndex[checkIndex];
			
			int count = 0;
			int ignoreGroups[2] = { -1, -1 };

			switch (checkDataIndex.Type)
			{
				case AGT_MONSTER:
				case AGT_SEA_MONSTER:
				{
					count = 22;
					ignoreGroups[0] = HAG_DIE_1;
					ignoreGroups[1] = HAG_DIE_2;

					break;
				}
				case AGT_HUMAN:
				case AGT_EQUIPMENT:
				{
					count = 35;
					ignoreGroups[0] = PAG_DIE_1;
					ignoreGroups[1] = PAG_DIE_2;

					break;
				}
				case AGT_ANIMAL:
				{
					count = 13;
					ignoreGroups[0] = LAG_DIE_1;
					ignoreGroups[1] = LAG_DIE_2;

					break;
				}
				default:
					break;
			}

			IFOR(j, 0, count)
			{
				if (j == ignoreGroups[0] || j == ignoreGroups[1])
					continue;

				CTextureAnimationGroup &group = dataIndex.m_Groups[j];
				CTextureAnimationGroup &newGroup = checkDataIndex.m_Groups[j];

				IFOR(d, 0, 5)
				{
					CTextureAnimationDirection &direction = group.m_Direction[d];
					CTextureAnimationDirection &newDirection = newGroup.m_Direction[d];

					direction.Address = newDirection.Address;
					direction.Size = newDirection.Size;
				}
			}

			dataIndex.Type = checkDataIndex.Type;
			dataIndex.Graphic = checkIndex;
			dataIndex.Color = atoi(strings[2].c_str());
		}
	}

	while (!corpseParser.IsEOF())
	{
		STRING_LIST strings = corpseParser.ReadTokens();
		
		if (strings.size() >= 3)
		{
			ushort index = atoi(strings[0].c_str());

			STRING_LIST newBody = newBodyParser.GetTokens(strings[1].c_str());

			if (index >= MAX_ANIMATIONS_DATA_INDEX_COUNT || !newBody.size())
				continue;

			ushort checkIndex = atoi(newBody[0].c_str());

			if (checkIndex >= MAX_ANIMATIONS_DATA_INDEX_COUNT)
				continue;

			CIndexAnimation &dataIndex = m_DataIndex[index];
			CIndexAnimation &checkDataIndex = m_DataIndex[checkIndex];
			
			int ignoreGroups[2] = { -1, -1 };

			switch (checkDataIndex.Type)
			{
				case AGT_MONSTER:
				case AGT_SEA_MONSTER:
				{
					ignoreGroups[0] = HAG_DIE_1;
					ignoreGroups[1] = HAG_DIE_2;

					break;
				}
				case AGT_HUMAN:
				case AGT_EQUIPMENT:
				{
					ignoreGroups[0] = PAG_DIE_1;
					ignoreGroups[1] = PAG_DIE_2;

					break;
				}
				case AGT_ANIMAL:
				{
					ignoreGroups[0] = LAG_DIE_1;
					ignoreGroups[1] = LAG_DIE_2;

					break;
				}
				default:
					break;
			}

			if (ignoreGroups[0] == -1)
				continue;

			IFOR(j, 0, 2)
			{
				CTextureAnimationGroup &group = dataIndex.m_Groups[ignoreGroups[j]];
				CTextureAnimationGroup &newGroup = checkDataIndex.m_Groups[ignoreGroups[j]];

				IFOR(d, 0, 5)
				{
					CTextureAnimationDirection &direction = group.m_Direction[d];
					CTextureAnimationDirection &newDirection = newGroup.m_Direction[d];

					direction.Address = newDirection.Address;
					direction.Size = newDirection.Size;
				}
			}

			dataIndex.Type = checkDataIndex.Type;
			dataIndex.Graphic = checkIndex;
			dataIndex.Color = atoi(strings[2].c_str());
		}
	}
}
//----------------------------------------------------------------------------------
/*!
Получить индекс группы по индексу картинки
@param [__in] id Индекс картинки
@return Группа анимаций
*/
ANIMATION_GROUPS CAnimationManager::GetGroupIndex(const ushort &id)
{
	switch (m_DataIndex[id].Type)
	{
		case AGT_ANIMAL:
			return AG_LOW;
		case AGT_MONSTER:
		case AGT_SEA_MONSTER:
			return AG_HIGHT;
		case AGT_HUMAN:
		case AGT_EQUIPMENT:
			return AG_PEOPLE;
	}

	return AG_HIGHT;
}
//----------------------------------------------------------------------------------
/*!
Получить индекс группы смерти анимации
@param [__in] id BИндекс картинки
@param [__in] second Группа смерти номер 2
@return Индекс группы анимации
*/
uchar CAnimationManager::GetDieGroupIndex(ushort id, const bool &second)
{
	switch (m_DataIndex[id].Type)
	{
		case AGT_ANIMAL:
			return (uchar)(second ? LAG_DIE_2 : LAG_DIE_1);
		case AGT_MONSTER:
		case AGT_SEA_MONSTER:
			return (uchar)(second ? HAG_DIE_2 : HAG_DIE_1);
		case AGT_HUMAN:
		case AGT_EQUIPMENT:
			return (uchar)(second ? PAG_DIE_2 : PAG_DIE_1);
	}

	return 0;
}
//----------------------------------------------------------------------------------
/*!
Коррекция направления и режима зеркального отображения
@param [__inout] dir Направление
@param [__inout] mirror Зеркальное отображение
@return 
*/
void CAnimationManager::GetAnimDirection(uchar &dir, bool &mirror)
{
	switch (dir)
	{
		case 2:
		case 4:
		{
			mirror = (dir == 2);
			dir = 1;

			break;
		}
		case 1:
		case 5:
		{
			mirror = (dir == 1);
			dir = 2;

			break;
		}
		case 0:
		case 6:
		{
			mirror = (dir == 0);
			dir = 3;

			break;
		}
		case 3:
		{
			dir = 0;

			break;
		}
		case 7:
		{
			dir = 4;

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
/*!
Коррекция направления и режима зеркального отображения для сидячего персонажа
@param [__inout] dir Направление
@param [__inout] mirror Зеркальное отображение
@param [__inout] x Координата X
@param [__inout] y Координата Y
@return 
*/
void CAnimationManager::GetSittingAnimDirection(uchar &dir, bool &mirror, int &x, int &y)
{
	switch (dir)
	{
		case 0:
		{
			mirror = true;
			dir = 3;

			break;
		}
		case 2:
		{
			mirror = true;
			dir = 1;

			break;
		}
		case 4:
		{
			mirror = false;
			dir = 1;

			break;
		}
		case 6:
		{
			mirror = false;
			dir = 3;

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
/*!
Очистка неиспользуемых текстур
@param [__in] ticks Таймер удаления
@return 
*/
void CAnimationManager::ClearUnusedTextures(uint ticks)
{
	ticks -= CLEAR_ANIMATION_TEXTURES_DELAY;
	int count = 0;

	for (deque<CTextureAnimationDirection*>::iterator it = m_UsedAnimList.begin(); it != m_UsedAnimList.end();)
	{
		CTextureAnimationDirection *obj = *it;

		if (obj->LastAccessTime < ticks)
		{
			obj->Clear();
			obj->FrameCount = 0;
			obj->LastAccessTime = 0;
			count++;

			it = m_UsedAnimList.erase(it);
		}
		else
			it++;
	}

	LOG("CAnimationManager::ClearUnusedTextures::removed %i\n", count);
}
//----------------------------------------------------------------------------------
/*!
Загрузка картинок для указанного направления персонажа
@param [__in] direction Ссылка на направление анимации
@return true в случае успешной загрузки
*/
bool CAnimationManager::LoadDirectionGroup(CTextureAnimationDirection &direction)
{
	if (direction.Address == 0)
		return false;

	SetData((puchar)direction.Address, direction.Size);

	pushort palette = (pushort)m_Start;
	Move(sizeof(ushort[256])); //Palette
	puchar dataStart = m_Ptr;

	int frameCount = ReadUInt32LE();
	direction.FrameCount = frameCount;

	puint frameOffset = (puint)m_Ptr;

	//ushort color = m_DataIndex[graphic].Color;

	IFOR(i, 0, frameCount)
	{
		CTextureAnimationFrame *frame = direction.GetFrame(i);

		if (frame->m_Texture.Texture != 0)
			continue;

		m_Ptr = dataStart + frameOffset[i];

		uint imageCenterX = ReadInt16LE();
		frame->CenterX = imageCenterX;

		uint imageCenterY = ReadInt16LE();
		frame->CenterY = imageCenterY;

		uint imageWidth = ReadInt16LE();

		uint imageHeight = ReadInt16LE();

		if (!imageWidth || !imageHeight)
			continue;

		USHORT_LIST data(imageWidth * imageHeight, 0);

		uint header = ReadUInt32LE();

		while (header != 0x7FFF7FFF && !IsEOF())
		{
			ushort runLength = (header & 0x0FFF);

			int x = (header >> 22) & 0x03FF;

			if (x & 0x0200)
				x |= 0xFFFFFE00;

			int y = (header >> 12) & 0x03FF;

			if (y & 0x0200)
				y |= 0xFFFFFE00;

			x += imageCenterX;
			y += imageCenterY + imageHeight;

			int block = (y * imageWidth) + x;

			IFOR(k, 0, runLength)
			{
				ushort val = palette[ReadUInt8()];

				if (val)
					data[block] = 0x8000 | val;
				else
					data[block] = 0;

				block++;
			}

			header = ReadUInt32LE();
		}

		g_GL_BindTexture16(frame->m_Texture, imageWidth, imageHeight, &data[0]);
	}

	m_UsedAnimList.push_back(&direction);

	return true;
}
//----------------------------------------------------------------------------------
bool CAnimationManager::TestImagePixels(CTextureAnimationDirection &direction, const uchar &frame, const int &checkX, const int &checkY)
{
	SetData((puchar)direction.Address, direction.Size);

	pushort palette = (pushort)m_Start;
	Move(sizeof(ushort[256])); //Palette
	puchar dataStart = m_Ptr;

	int frameCount = ReadUInt32LE();
	puint frameOffset = (puint)m_Ptr;

	m_Ptr = dataStart + frameOffset[frame];

	uint imageCenterX = ReadInt16LE();
	uint imageCenterY = ReadInt16LE();
	uint imageWidth = ReadInt16LE();
	uint imageHeight = ReadInt16LE();

	uint header = ReadUInt32LE();

	while (header != 0x7FFF7FFF && !IsEOF())
	{
		ushort runLength = (header & 0x0FFF);

		int x = (header >> 22) & 0x03FF;

		if (x & 0x0200)
			x |= 0xFFFFFE00;

		int y = (header >> 12) & 0x03FF;

		if (y & 0x0200)
			y |= 0xFFFFFE00;

		x += imageCenterX;
		y += imageCenterY + imageHeight;

		if (y != checkY)
			Move(runLength);
		else
		{
			if (checkX >= x && checkX <= x + runLength)
			{
				Move(checkX);

				return (palette[ReadUInt8()] != 0);
			}

			Move(runLength);
		}

		header = ReadUInt32LE();
	}

	return false;
}
//----------------------------------------------------------------------------------
bool CAnimationManager::TestPixels(CGameObject *obj, int x, int y, const bool &mirror, uchar &frameIndex, ushort id)
{
	if (obj == NULL)
		return false;

	if (!id)
		id = obj->GetMountAnimation();

	if (id >= MAX_ANIMATIONS_DATA_INDEX_COUNT)
		return NULL;

	CTextureAnimationDirection &direction = m_DataIndex[id].m_Groups[m_AnimGroup].m_Direction[m_Direction];

	if (direction.FrameCount == 0 && !LoadDirectionGroup(direction))
		return false;

	int fc = direction.FrameCount;

	if (fc > 0 && frameIndex >= fc)
	{
		if (obj->IsCorpse())
			frameIndex = fc - 1;
		else
			frameIndex = 0;
	}

	CTextureAnimationFrame *frame = direction.GetFrame(frameIndex);

	if (frame != NULL)
	{
		CGLTexture &texture = frame->m_Texture;
		y -= texture.Height + frame->CenterY;
		
		if (mirror)
			x -= texture.Width - frame->CenterX;
		else
			x -= frame->CenterX;

		x = g_MouseManager.Position.X - x;
		y = g_MouseManager.Position.Y - y;

		if (mirror)
			x = texture.Width - x;

		if (x >= 0 && x < texture.Width && y >= 0 && y < texture.Height)
			return TestImagePixels(direction, frameIndex, x, y);
	}

	return false;
}
//----------------------------------------------------------------------------------
/*!
Получение ссылки на указанный фрэйм
@param [__in] obj Игровой объект
@param [__inout] frameIndex Индекс кадра
@param [__in_opt] id Индекс картинки
@return Ссылка на кадр анимации
*/
CTextureAnimationFrame *CAnimationManager::GetFrame(CGameObject *obj, uchar frameIndex, ushort graphic)
{
	CTextureAnimationFrame *frame = NULL;

	if (obj != NULL)
	{
		if (!graphic)
			graphic = obj->GetMountAnimation();

		if (graphic < MAX_ANIMATIONS_DATA_INDEX_COUNT)
		{
			CTextureAnimationDirection &direction = m_DataIndex[graphic].m_Groups[m_AnimGroup].m_Direction[m_Direction];

			if (direction.Address != 0)
			{
				int fc = direction.FrameCount;

				if (fc > 0 && frameIndex >= fc)
				{
					if (obj->IsCorpse())
						frameIndex = fc - 1;
					else
						frameIndex = 0;
				}

				frame = direction.GetFrame(frameIndex);
			}
		}
	}

	return frame;
}
//----------------------------------------------------------------------------------
void CAnimationManager::Draw(CGameObject *obj, int x, int y, const bool &mirror, uchar &frameIndex, int id)
{
	//if (obj == NULL)
	//	return;

	bool isShadow = (id >= 0x10000);

	if (isShadow)
		id -= 0x10000;

	if (!id)
		id = obj->GetMountAnimation();

	if (id >= MAX_ANIMATIONS_DATA_INDEX_COUNT)
		return;

	CTextureAnimationDirection &direction = m_DataIndex[id].m_Groups[m_AnimGroup].m_Direction[m_Direction];

	if (direction.FrameCount == 0 && !LoadDirectionGroup(direction))
		return;

	int fc = direction.FrameCount;

	if (fc > 0 && frameIndex >= fc)
	{
		if (obj->IsCorpse())
			frameIndex = fc - 1;
		else
			frameIndex = 0;
	}

	CTextureAnimationFrame *frame = direction.GetFrame(frameIndex);
	
	if (frame != NULL && frame->m_Texture.Texture != 0)
	{
		if (mirror)
			x -= frame->m_Texture.Width - frame->CenterX;
		else
			x -= frame->CenterX;

		y -= frame->m_Texture.Height + frame->CenterY;

		if (isShadow)
		{
			glUniform1iARB(g_ShaderDrawMode, 12);

			glEnable(GL_BLEND);
			glBlendFunc(GL_DST_COLOR, GL_ZERO);

			g_GL_DrawShadow(frame->m_Texture, x, y, mirror);

			if (m_UseBlending)
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			else
				glDisable(GL_BLEND);
		}
		else
		{
			int drawMode = 0;
			bool spectralColor = false;

			if (!g_GrayedPixels)
			{
				ushort color = m_Color;
				bool partialHue = false;

				if (!color)
				{
					color = obj->Color;
					partialHue = obj->IsPartialHue();

					if (color & 0x8000)
					{
						partialHue = true;
						color &= 0x7FFF;
					}

					if (!color)
					{
						color = m_DataIndex[id].Color;
						partialHue = false;
					}
				}

				if (color & SPECTRAL_COLOR)
				{
					spectralColor = true;
					glEnable(GL_BLEND);

					if (color == SPECTRAL_COLOR_SPECIAL)
					{
						glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
						drawMode = 11;
					}
					else
					{
						glBlendFunc(GL_ZERO, GL_SRC_COLOR);
						drawMode = 10;
					}
				}
				else if (color)
				{
					if (partialHue)
						drawMode = 2;
					else
						drawMode = 1;

					g_ColorManager.SendColorsToShader(color);
				}
			}

			glUniform1iARB(g_ShaderDrawMode, drawMode);

			if (m_Transform)
			{
				if (obj->IsHuman())
				{
					short frameHeight = frame->m_Texture.Height;
					m_CharacterFrameStartY = y;
					m_CharacterFrameHeight = frame->m_Texture.Height;
					m_StartCharacterWaistY = (int)(frameHeight * UPPER_BODY_RATIO) + m_CharacterFrameStartY;
					m_StartCharacterKneesY = (int)(frameHeight * MID_BODY_RATIO) + m_CharacterFrameStartY;
					m_StartCharacterFeetY = (int)(frameHeight * LOWER_BODY_RATIO) + m_CharacterFrameStartY;
				}

				float h3mod = UPPER_BODY_RATIO;
				float h6mod = MID_BODY_RATIO;
				float h9mod = LOWER_BODY_RATIO;

				if (!obj->NPC)
				{
					float itemsEndY = (float)(y + frame->m_Texture.Height);

					//Определяем соотношение верхней части текстуры, до перелома.
					if (y >= m_StartCharacterWaistY)
						h3mod = 0;
					else if (itemsEndY <= m_StartCharacterWaistY)
						h3mod = 1.0f;
					else
					{
						float upperBodyDiff = (float)(m_StartCharacterWaistY - y);
						h3mod = upperBodyDiff / frame->m_Texture.Height;
						if (h3mod < 0)
							h3mod = 0;
					}
					
					//Определяем соотношение средней части, где идет деформация с растягиванием по Х.
					if (m_StartCharacterWaistY >= itemsEndY || y >= m_StartCharacterKneesY)
						h6mod = 0;
					else if (m_StartCharacterWaistY <= y && itemsEndY <= m_StartCharacterKneesY)
						h6mod = 1.0f;
					else
					{
						float midBodyDiff = 0.0f;
						if (y >= m_StartCharacterWaistY)
							midBodyDiff = (float)(m_StartCharacterKneesY - y);
						else if (itemsEndY <= m_StartCharacterKneesY)
							midBodyDiff = (float)(itemsEndY - m_StartCharacterWaistY);
						else
							midBodyDiff = (float)(m_StartCharacterKneesY - m_StartCharacterWaistY);

						h6mod = h3mod + midBodyDiff / frame->m_Texture.Height;
						if (h6mod < 0)
							h6mod = 0;
					}
						
					//Определяем соотношение нижней части, она смещена на 8 Х.
					if (itemsEndY <= m_StartCharacterKneesY)
						h9mod = 0;
					else if (y >= m_StartCharacterKneesY)
						h9mod = 1.0f;
					else
					{
						float lowerBodyDiff = itemsEndY - m_StartCharacterKneesY;
						h9mod = h6mod + lowerBodyDiff / frame->m_Texture.Height;
						if (h9mod < 0)
							h9mod = 0;
					}
				}

				g_GL_DrawSitting(frame->m_Texture, x, y, mirror, h3mod, h6mod, h9mod);
			}			
			else
				g_GL_DrawMirrored(frame->m_Texture, x, y, mirror);

			if (spectralColor)
			{
				if (m_UseBlending)
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				else
					glDisable(GL_BLEND);
			}
		}
	}
}
//----------------------------------------------------------------------------------
void CAnimationManager::FixSittingDirection(uchar &layerDirection, bool &mirror, int &x, int &y)
{
	const SITTING_INFO_DATA &data = SITTING_INFO[m_Sitting - 1];

	switch (m_Direction)
	{
		case 7:
		case 0:
		{
			if (data.Direction1 == -1)
			{
				if (m_Direction == 7)
					m_Direction = data.Direction4;
				else
					m_Direction = data.Direction2;
			}
			else
				m_Direction = data.Direction1;

			break;
		}
		case 1:
		case 2:
		{
			if (data.Direction2 == -1)
			{
				if (m_Direction == 1)
					m_Direction = data.Direction1;
				else
					m_Direction = data.Direction3;
			}
			else
				m_Direction = data.Direction2;

			break;
		}
		case 3:
		case 4:
		{
			if (data.Direction3 == -1)
			{
				if (m_Direction == 3)
					m_Direction = data.Direction2;
				else
					m_Direction = data.Direction4;
			}
			else
				m_Direction = data.Direction3;

			break;
		}
		case 5:
		case 6:
		{
			if (data.Direction4 == -1)
			{
				if (m_Direction == 5)
					m_Direction = data.Direction3;
				else
					m_Direction = data.Direction1;
			}
			else
				m_Direction = data.Direction4;

			break;
		}
		default:
			break;
	}

	layerDirection = m_Direction;
	GetSittingAnimDirection(m_Direction, mirror, x, y);

	int offsX = g_Orion.m_StaticData[data.Graphic / 32].Tiles[data.Graphic % 32].SittingOffset;

	if (offsX > 10 || offsX == 0)
		offsX = SITTING_OFFSET_X;

	if (mirror)
	{		
		
		if (m_Direction == 3)
		{
			y += 23 + data.MirrorOffsetY;
			x += offsX - 4;
		}	
		else
		{
			y += data.OffsetY + 9;
		}			
	}
	else
	{				 
		if (m_Direction == 3)
		{
			y += 23 + data.MirrorOffsetY;
			x -= 3;
		}
		else
		{
			y += 9 + data.OffsetY;
			x -= offsX + 1;
		}
	}
}
//----------------------------------------------------------------------------------
/*!
Отрисовать персонажа
@param [__in] obj Ссылка на персонажа
@param [__in] x Экранная координата X
@param [__in] y Экранная координата Y
@param [__in] z Координата Z
@return 
*/
void CAnimationManager::DrawCharacter(CGameCharacter *obj, int x, int y, int z)
{
	m_Transform = false;

	int drawX = (int)(x + obj->OffsetX);
	int drawY = (int)(y + obj->OffsetY) - (z * 4) - (int)obj->OffsetZ - 3;

	ushort targetColor = 0;
	bool needHPLine = false;
	uint serial = obj->Serial;
	bool drawShadow = !obj->Dead();
	m_UseBlending = false;

	if (g_DrawAura)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		uint auraColor = g_ColorManager.GetPolygoneColor(16, g_ConfigManager.GetColorByNotoriety(obj->Notoriety));
		glColor4ub(GetRValue(auraColor), GetGValue(auraColor), GetBValue(auraColor), 0xFF);

		glUniform1iARB(g_ShaderDrawMode, 0);
		g_AuraTexture.Draw(drawX - g_AuraTexture.Width / 2, drawY - g_AuraTexture.Height / 2);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glDisable(GL_BLEND);
	}

	if (obj->Hidden())
	{
		drawShadow = false;
		m_Color = 0x038E;

		if (obj->IsPlayer() || !g_ConfigManager.UseHiddenModeOnlyForSelf)
		{
			switch (g_ConfigManager.HiddenCharactersRenderMode)
			{
				case HCRM_ALPHA_BLENDING:
				{
					m_UseBlending = true;

					glColor4ub(0xFF, 0xFF, 0xFF, g_ConfigManager.HiddenAlpha);
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

					m_Color = 0x038C;

					break;
				}
				case HCRM_SPECTRAL_COLOR:
				{
					m_Color = 0x4001;
					break;
				}
				case HCRM_SPECIAL_SPECTRAL_COLOR:
				{
					m_Color = 0x4666;
					break;
				}
				default:
					break;
			}
		}
	}
	else if (g_StatusbarUnderMouse == serial)
		m_Color = g_ConfigManager.GetColorByNotoriety(obj->Notoriety);
	else
	{
		m_Color = 0;

		if (g_ConfigManager.ApplyStateColorOnCharacters)
		{
			if (obj->Poisoned())
				m_Color = 0x0044;
			else if (obj->Notoriety != NT_INVULNERABLE && obj->YellowHits())
				m_Color = 0x0030;
		}
	}

	puchar drawTextureColor = obj->m_DrawTextureColor;

	if (!m_UseBlending && drawTextureColor[3] != 0xFF)
	{
		m_UseBlending = true;
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4ub(drawTextureColor[0], drawTextureColor[1], drawTextureColor[2], drawTextureColor[3]);
	}

	bool isAttack = (serial == g_LastAttackObject);
	bool underMouseTarget = (g_SelectedObject.Object() == obj && g_Target.IsTargeting());

	if (!obj->IsPlayer() && (isAttack || underMouseTarget || serial == g_LastTargetObject))
	{
		targetColor = g_ConfigManager.GetColorByNotoriety(obj->Notoriety);

		if (isAttack || serial == g_LastTargetObject)
			needHPLine = (serial != g_NewTargetSystem.Serial);

		if (isAttack || underMouseTarget)
			m_Color = targetColor;
	}
	
	m_Direction = 0;
	obj->UpdateAnimationInfo(m_Direction);

	bool mirror = false;
	uchar layerDir = m_Direction;
	
	GetAnimDirection(m_Direction, mirror);

	uchar animIndex = obj->AnimIndex;
	uchar animGroup = obj->GetAnimationGroup();
	m_AnimGroup = animGroup;

	CGameItem *goi = obj->FindLayer(OL_MOUNT);
	
	int lightOffset = 20;
	
	if (obj->IsHuman() && goi != NULL) //Draw mount
	{
		m_Sitting = 0;
		lightOffset += 20;

		ushort mountID = goi->GetMountAnimation();
		int mountedHeightOffset = 0;

		if (mountID < MAX_ANIMATIONS_DATA_INDEX_COUNT)
			mountedHeightOffset = m_DataIndex[mountID].MountedHeightOffset;

		if (drawShadow)
		{
			Draw(obj, drawX, drawY + 10 + mountedHeightOffset, mirror, animIndex, 0x10000);
			m_AnimGroup = obj->GetAnimationGroup(mountID);

			Draw(goi, drawX, drawY, mirror, animIndex, mountID + 0x10000);
		}
		else
			m_AnimGroup = obj->GetAnimationGroup(mountID);

		Draw(goi, drawX, drawY, mirror, animIndex, mountID);
		drawY += mountedHeightOffset;
	}
	else
	{
		m_Sitting = obj->IsSitting();

		if (m_Sitting)
		{
			animGroup = PAG_STAND;
			animIndex = 0;

			obj->UpdateAnimationInfo(m_Direction);
			
			FixSittingDirection(layerDir, mirror, drawX, drawY);

			if (m_Direction == 3)
				animGroup = 25;
			else
				m_Transform = true;
		}
		else if (drawShadow)
			Draw(obj, drawX, drawY, mirror, animIndex, 0x10000);
	}
	
	m_AnimGroup = animGroup;

	Draw(obj, drawX, drawY, mirror, animIndex); //Draw character

	if (obj->IsHuman()) //Draw layered objects
	{
		DrawEquippedLayers(false, obj, drawX, drawY, mirror, layerDir, animIndex, lightOffset);

		const SITTING_INFO_DATA &sittingData = SITTING_INFO[m_Sitting - 1];

		if (m_Sitting && m_Direction == 3 && sittingData.DrawBack && obj->FindLayer(OL_CLOAK) == NULL)
		{
			for (CRenderWorldObject *ro = obj->m_PrevXY; ro != NULL; ro = (CRenderWorldObject*)ro->m_PrevXY)
			{
				if ((ro->Graphic & 0x3FFF) == sittingData.Graphic)
				{
					//оффсеты для ножниц
					int xOffset = mirror ? -20 : 0;
					int yOffset = -70;

					g_GL.PushScissor(drawX + xOffset, drawY + yOffset, 20, 40);
					g_Orion.DrawStaticArt(sittingData.Graphic, ro->Color, x, y, ro->Z);
					g_GL.PopScissor();

					break;
				}
			}
		}
	}

	if (m_UseBlending)
	{
		m_UseBlending = false;
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glDisable(GL_BLEND);
	}

	if (!g_ConfigManager.DisableNewTargetSystem && g_NewTargetSystem.Serial == obj->Serial)
	{
		ushort id = obj->GetMountAnimation();

		if (id < MAX_ANIMATIONS_DATA_INDEX_COUNT)
		{
			CTextureAnimationDirection &direction = m_DataIndex[id].m_Groups[m_AnimGroup].m_Direction[m_Direction];

			if (direction.Address != 0)
			{
				CTextureAnimationFrame *frame = direction.GetFrame(0);

				int frameWidth = 20;
				int frameHeight = 20;

				if (frame != NULL)
				{
					frameWidth = frame->m_Texture.Width;
					frameHeight = frame->m_Texture.Height;
				}

				if (frameWidth >= 80)
				{
					g_NewTargetSystem.GumpTop = 0x756D;
					g_NewTargetSystem.GumpBottom = 0x756A;
				}
				else if (frameWidth >= 40)
				{
					g_NewTargetSystem.GumpTop = 0x756E;
					g_NewTargetSystem.GumpBottom = 0x756B;
				}
				else
				{
					g_NewTargetSystem.GumpTop = 0x756F;
					g_NewTargetSystem.GumpBottom = 0x756C;
				}

				switch (obj->Notoriety)
				{
					case NT_INNOCENT:
					{
						g_NewTargetSystem.ColorGump = 0x7570;
						break;
					}
					case NT_FRIENDLY:
					{
						g_NewTargetSystem.ColorGump = 0x7571;
						break;
					}
					case NT_SOMEONE_GRAY:
					case NT_CRIMINAL:
					{
						g_NewTargetSystem.ColorGump = 0x7572;
						break;
					}
					case NT_ENEMY:
					{
						g_NewTargetSystem.ColorGump = 0x7573;
						break;
					}
					case NT_MURDERER:
					{
						g_NewTargetSystem.ColorGump = 0x7576;
						break;
					}
					case NT_INVULNERABLE:
					{
						g_NewTargetSystem.ColorGump = 0x7575;
						break;
					}
					default:
						break;
				}

				int per = obj->MaxHits;

				if (per > 0)
				{
					per = (obj->Hits * 100) / per;

					if (per > 100)
						per = 100;

					if (per < 1)
						per = 0;
					else
						per = (34 * per) / 100;

				}

				g_NewTargetSystem.Hits = per;
				g_NewTargetSystem.X = drawX;
				g_NewTargetSystem.TopY = drawY - frameHeight - 8;
				g_NewTargetSystem.BottomY = drawY + 7;
			}
		}
	}

	if (needHPLine)
	{
		int per = obj->MaxHits;

		if (per > 0)
		{
			per = (obj->Hits * 100) / per;

			if (per > 100)
				per = 100;
			
			if (per < 1)
				per = 0;
			else
				per = (34 * per) / 100;

		}

		if (isAttack)
		{
			g_AttackTargetGump.X = drawX - 20;
			g_AttackTargetGump.Y = drawY;
			g_AttackTargetGump.Color = targetColor;
			g_AttackTargetGump.Hits = per;
		}
		else
		{
			g_TargetGump.X = drawX - 20;
			g_TargetGump.Y = drawY;
			g_TargetGump.Color = targetColor;
			g_TargetGump.Hits = per;
		}
	}
}
//----------------------------------------------------------------------------------
/*!
Проверить наличие пикселя персонажа в указанных координатах
@param [__in] obj Ссылка на персонажа
@param [__in] x Экранная координата X
@param [__in] y Экранная координата Y
@param [__in] z Координата Z
@return true в случае, если хоть 1 пиксель находится под мышкой
*/
bool CAnimationManager::CharacterPixelsInXY(CGameCharacter *obj, int x, int y, int z)
{
	y -= 3;
	m_Sitting = obj->IsSitting();
	m_Direction = 0;
	obj->UpdateAnimationInfo(m_Direction);

	bool mirror = false;
	uchar layerDir = m_Direction;
	
	GetAnimDirection(m_Direction, mirror);

	uchar animIndex = obj->AnimIndex;
	uchar animGroup = obj->GetAnimationGroup();
	
	CGameItem *goi = obj->FindLayer(OL_MOUNT);
	
	int drawX = (int)(x - obj->OffsetX);
	int drawY = (int)(y - obj->OffsetY) - (z * 4) - obj->OffsetZ;
	
	if (obj->IsHuman() && goi != NULL) //Check mount
	{
		ushort mountID = goi->GetMountAnimation();

		m_AnimGroup = obj->GetAnimationGroup(mountID);

		if (TestPixels(goi, drawX, drawY, mirror, animIndex, mountID))
			return true;

		if (mountID < MAX_ANIMATIONS_DATA_INDEX_COUNT)
			drawY += m_DataIndex[mountID].MountedHeightOffset;
	}

	m_AnimGroup = animGroup;

	return TestPixels(obj, drawX, drawY, mirror, animIndex) || DrawEquippedLayers(true, obj, drawX, drawY, mirror, layerDir, animIndex, 0);
}
//----------------------------------------------------------------------------------
/*!
Отрисовать труп
@param [__in] obj Ссылка на труп
@param [__in] x Экранная координата X
@param [__in] y Экранная координата Y
@param [__in] z Координата Z
@return 
*/
void CAnimationManager::DrawCorpse(CGameItem *obj, const int &x, const int &y)
{
	m_Sitting = 0;
	m_Direction = obj->Layer & 0x7F;
	bool mirror = false;

	GetAnimDirection(m_Direction, mirror);

	if (obj->Hidden())
		m_Color = 0x038E;
	else
		m_Color = 0;

	uchar animIndex = obj->AnimIndex;
	m_AnimGroup = GetDieGroupIndex(obj->GetMountAnimation(), obj->UsedLayer);
	
	Draw(obj, x, y, mirror, animIndex); //Draw animation

	DrawEquippedLayers(false, obj, x, y, mirror, m_Direction, animIndex, 0);
}
//----------------------------------------------------------------------------------
/*!
Проверить наличие пикселя трупа в указанных координатах
@param [__in] obj Ссылка на труп
@param [__in] x Экранная координата X
@param [__in] y Экранная координата Y
@param [__in] z Координата Z
@return 
*/
bool CAnimationManager::CorpsePixelsInXY(CGameItem *obj, const int &x, const int &y)
{
	m_Sitting = 0;
	m_Direction = obj->Layer & 0x7F;
	bool mirror = false;

	GetAnimDirection(m_Direction, mirror);

	uchar animIndex = obj->AnimIndex;
	m_AnimGroup = GetDieGroupIndex(obj->GetMountAnimation(), obj->UsedLayer);

	return TestPixels(obj, x, y, mirror, animIndex) || DrawEquippedLayers(true, obj, x, y, mirror, m_Direction, animIndex, 0);
}
//----------------------------------------------------------------------------------
/*!
Существует ли анимация в файле
@param [__in] graphic Индекс картинки
@param [__in] group Группа анимации
@return true в случае успеха
*/
bool CAnimationManager::AnimationExists(const ushort &graphic, uchar group)
{
	bool result = false;

	if (graphic < MAX_ANIMATIONS_DATA_INDEX_COUNT && group < ANIMATION_GROUPS_COUNT)
		result = (m_DataIndex[graphic].m_Groups[group].m_Direction[0].Address != 0);

	return result;
}
//----------------------------------------------------------------------------------
/*!
Получить индекс анимации
@param [__inout] graphic Индекс каритнки
@return 
*/
void CAnimationManager::GetBodyGraphic(ushort &graphic)
{
	//if (graphic < MAX_ANIMATIONS_DATA_INDEX_COUNT)
	//	graphic = m_DataIndex[graphic].Graphic;
}
//----------------------------------------------------------------------------------
ANIMATION_DIMENSIONS CAnimationManager::GetAnimationDimensions(CGameObject *obj, uchar frameIndex, const uchar &defaultDirection, const uchar &defaultGroup)
{
	ANIMATION_DIMENSIONS result = { 0 };

	uchar dir = defaultDirection & 0x7F;
	uchar animGroup = defaultGroup;
	ushort id = obj->GetMountAnimation();
	bool mirror = false;

	if (obj->NPC)
	{
		CGameCharacter *gc = obj->GameCharacterPtr();
		gc->UpdateAnimationInfo(dir);
		animGroup = gc->GetAnimationGroup();
		GetAnimDirection(dir, mirror);
	}
	else if (obj->IsCorpse())
	{
		dir = ((CGameItem*)obj)->Layer;
		animGroup = GetDieGroupIndex(id, ((CGameItem*)obj)->UsedLayer);
		GetAnimDirection(dir, mirror);
	}
	else if (((CGameItem*)obj)->Layer != OL_MOUNT) //TGameItem
		id = ((CGameItem*)obj)->AnimID;

	if (frameIndex == 0xFF)
		frameIndex = (uchar)obj->AnimIndex;

	bool found = false;

	if (id < MAX_ANIMATIONS_DATA_INDEX_COUNT)
	{
		if (dir < 5)
		{
			CTextureAnimationDirection &direction = m_DataIndex[id].m_Groups[animGroup].m_Direction[dir];

			if (direction.Address != NULL)
			{
				int fc = direction.FrameCount;

				if (fc > 0 && frameIndex >= fc)
				{
					if (obj->IsCorpse())
						frameIndex = fc - 1;
					else
						frameIndex = 0;
				}

				CTextureAnimationFrame *frame = direction.FindFrame(frameIndex);

				if (frame != NULL)
				{
					result.Width = frame->m_Texture.Width;
					result.Height = frame->m_Texture.Height;
					result.CenterX = frame->CenterX;
					result.CenterY = frame->CenterY;

					found = true;
				}
			}
		}

		if (!found)
		{
			CTextureAnimationDirection &direction = m_DataIndex[id].m_Groups[animGroup].m_Direction[0];

			puchar ptr = (puchar)direction.Address;

			if (ptr != NULL)
			{
				SetData(ptr, direction.Size);
				Move(sizeof(ushort[256]));  //Palette
				puchar dataStart = m_Ptr;

				int frameCount = ReadUInt32LE();

				if (frameCount > 0 && frameIndex >= frameCount)
				{
					if (obj->IsCorpse())
						frameIndex = frameCount - 1;
					else
						frameIndex = 0;
				}

				if (frameIndex < frameCount)
				{
					puint frameOffset = (puint)m_Ptr;
					//Move(frameOffset[frameIndex]);
					m_Ptr = dataStart + frameOffset[frameIndex];

					result.CenterX = ReadInt16LE();
					result.CenterY = ReadInt16LE();
					result.Width = ReadInt16LE();
					result.Height = ReadInt16LE();
				}
			}
		}
		else if (g_FileManager.UseUOP)
			TryReadUOPAnimDimins(result);
	}

	return result;
}
//----------------------------------------------------------------------------------
void CAnimationManager::TryReadUOPAnimDimins(ANIMATION_DIMENSIONS &dimins)
{
	//We'll either read, decompress and then read mmaped uop here, which might be slow and we might get virtual memory issues later on.
	//Or we have to implement a method to read, decompress and save temp uop files somewhere in InitIndexReplaces() and then use a file stream to read those files during the runtime in this method.
	//Not sure what to chose yet. 2nd option has complications because of the temp files, but is more resource friendly. 1st option is a memory hit and actually might be slower.
}
//----------------------------------------------------------------------------------
void CAnimationManager::CalculateFrameInformation(FRAME_OUTPUT_INFO &info, CGameObject *obj, const bool &mirror, const uchar &animIndex)
{
	ANIMATION_DIMENSIONS dim = GetAnimationDimensions(obj, animIndex, m_Direction, m_AnimGroup);

	int y = -(dim.Height + dim.CenterY + 3);
	int x = -dim.CenterX;

	if (mirror)
		x = -(dim.Width - dim.CenterX);

	if (x < info.StartX)
		info.StartX = x;

	if (y < info.StartY)
		info.StartY = y;

	if (info.EndX < x + dim.Width)
		info.EndX = x + dim.Width;

	if (info.EndY < y + dim.Height)
		info.EndY = y + dim.Height;
}
//----------------------------------------------------------------------------------
DRAW_FRAME_INFORMATION CAnimationManager::CollectFrameInformation(CGameObject *gameObject, const bool &checkLayers)
{
	m_Sitting = false;
	m_Direction = 0;

	DRAW_FRAME_INFORMATION dfInfo = { 0 };

	vector<CGameItem*> &list = gameObject->m_DrawLayeredObjects;

	if (checkLayers)
	{
		list.clear();

		memset(&m_CharacterLayerGraphic[0], 0, sizeof(m_CharacterLayerGraphic));
		memset(&m_CharacterLayerAnimID[0], 0, sizeof(m_CharacterLayerAnimID));

		QFOR(item, gameObject->m_Items, CGameItem*)
		{
			if (item->Layer < OL_MOUNT)
			{
				m_CharacterLayerGraphic[item->Layer] = item->Graphic;
				m_CharacterLayerAnimID[item->Layer] = item->AnimID;
			}
		}
	}

	if (gameObject->NPC)
	{
		CGameCharacter *obj = (CGameCharacter*)gameObject;
		obj->UpdateAnimationInfo(m_Direction);

		bool mirror = false;
		uchar layerDir = m_Direction;

		GetAnimDirection(m_Direction, mirror);

		uchar animIndex = obj->AnimIndex;
		uchar animGroup = obj->GetAnimationGroup();

		FRAME_OUTPUT_INFO info = { 0 };

		CGameItem *goi = obj->FindLayer(OL_MOUNT);

		if (goi != NULL) //Check mount
		{
			ushort mountID = goi->GetMountAnimation();

			m_AnimGroup = obj->GetAnimationGroup(mountID);

			CalculateFrameInformation(info, goi, mirror, animIndex);

			switch (animGroup)
			{
				case PAG_FIDGET_1:
				case PAG_FIDGET_2:
				case PAG_FIDGET_3:
				{
					animGroup = PAG_ONMOUNT_STAND;
					animIndex = 0;
					break;
				}
				default:
					break;
			}
		}

		m_AnimGroup = animGroup;

		CalculateFrameInformation(info, obj, mirror, animIndex);

		if (obj->IsHuman() && checkLayers) //Check layred objects
		{
			IFOR(l, 0, USED_LAYER_COUNT)
			{
				goi = obj->FindLayer(m_UsedLayers[layerDir][l]);

				if (goi == NULL || !goi->AnimID)
					continue;

				if (!IsCovered(goi->Layer, obj))
				{
					list.push_back(goi);
					CalculateFrameInformation(info, goi, mirror, animIndex);
				}
			}
		}

		dfInfo.OffsetX = abs(info.StartX);
		dfInfo.OffsetY = abs(info.StartY);
		dfInfo.Width = dfInfo.OffsetX + info.EndX;
		dfInfo.Height = dfInfo.OffsetY + info.EndY;
	}
	else if (gameObject->IsCorpse())
	{
		CGameItem *obj = (CGameItem*)gameObject;

		m_Direction = obj->Layer & 0x7F;
		bool mirror = false;

		GetAnimDirection(m_Direction, mirror);

		uchar animIndex = obj->AnimIndex;
		m_AnimGroup = GetDieGroupIndex(obj->GetMountAnimation(), obj->UsedLayer);

		FRAME_OUTPUT_INFO info = { 0 };

		CalculateFrameInformation(info, obj, mirror, animIndex);

		if (checkLayers)
		{
			IFOR(l, 0, USED_LAYER_COUNT)
			{
				CGameItem *goi = obj->FindLayer(m_UsedLayers[m_Direction][l]);

				if (goi != NULL && goi->AnimID)
				{
					if (!IsCovered(goi->Layer, obj))
					{
						list.push_back(goi);
						CalculateFrameInformation(info, goi, mirror, animIndex);
					}
				}
			}
		}

		dfInfo.OffsetX = abs(info.StartX);
		dfInfo.OffsetY = abs(info.StartY);
		dfInfo.Width = dfInfo.OffsetX + info.EndX;
		dfInfo.Height = dfInfo.OffsetY + info.EndY;
	}

	return dfInfo;
}
//----------------------------------------------------------------------------------
bool CAnimationManager::DrawEquippedLayers(const bool &selection, CGameObject *obj, const int &drawX, const int &drawY, const bool &mirror, const uchar &layerDir, uchar animIndex, const int &lightOffset)
{
	bool result = false;

	vector<CGameItem*> &list = obj->m_DrawLayeredObjects;

	if (selection)
	{
		for (vector<CGameItem*>::iterator i = list.begin(); i != list.end() && !result; i++)
			result = TestPixels(*i, drawX, drawY, mirror, animIndex, (*i)->AnimID);
	}
	else
	{
		for (vector<CGameItem*>::iterator i = list.begin(); i != list.end(); i++)
		{
			CGameItem *item = *i;

			Draw(item, drawX, drawY, mirror, animIndex, item->AnimID);

			if (item->IsLightSource() && g_GameScreen.UseLight)
				g_GameScreen.AddLight(obj, item, drawX, drawY - lightOffset);
		}
	}

	return result;
}
//----------------------------------------------------------------------------------
bool CAnimationManager::IsCovered(const int &layer, CGameObject *owner)
{
	bool result = false;

	switch (layer)
	{
		case OL_SHOES:
		{
			if (m_CharacterLayerGraphic[OL_LEGS] != 0 || m_CharacterLayerGraphic[OL_PANTS] == 0x1411)
				result = true;
			else if (m_CharacterLayerAnimID[OL_PANTS] == 0x0513 || m_CharacterLayerAnimID[OL_PANTS] == 0x0514 || m_CharacterLayerAnimID[OL_ROBE] == 0x0504)
				result = true;

			break;
		}
		case OL_PANTS:
		{
			const ushort &robe = m_CharacterLayerAnimID[OL_ROBE];
			const ushort &pants = m_CharacterLayerAnimID[OL_PANTS];

			if (m_CharacterLayerGraphic[OL_LEGS] != 0 || robe == 0x0504)
				result = true;
			if (pants == 0x01EB || pants == 0x03E5 || pants == 0x03EB)
			{
				const ushort &skirt = m_CharacterLayerAnimID[OL_SKIRT];

				if (skirt != 0x01C7 && skirt != 0x01E4)
					result = true;
				else if (robe != 0x0229 && (robe <= 0x04E7 || robe <= 0x04EB))
					result = true;
			}

			break;
		}
		case OL_TUNIC:
		{
			const ushort &robe = m_CharacterLayerGraphic[OL_ROBE];
			
			if (robe != 0)
				result = true;
			else if (m_CharacterLayerGraphic[OL_TUNIC] == 0x0238)
				result = (robe != 0x9985 && robe != 0x9986);

			break;
		}
		case OL_TORSO:
		{
			const ushort &robe = m_CharacterLayerGraphic[OL_ROBE];

			if (robe != 0 && robe != 0x9985 && robe != 0x9986)
				result = true;
			else
			{
				const ushort &tunic = m_CharacterLayerGraphic[OL_TUNIC];
				const ushort &torso = m_CharacterLayerGraphic[OL_TORSO];

				if (tunic != 0 && tunic != 0x1541 && tunic != 0x1542)
					result = true;
				else if (torso == 0x782A || torso == 0x782B)
					result = true;
			}

			break;
		}
		case OL_ARMS:
		{
			const ushort &robe = m_CharacterLayerGraphic[OL_ROBE];
			result = (robe != 0 && robe != 0x9985 && robe != 0x9986);

			break;
		}
		case OL_HAIR:
		case OL_HELMET:
		{
			const ushort &robe = m_CharacterLayerGraphic[OL_ROBE];

			if (robe > 0x3173)
			{
				if ((robe == 0x4B9D || robe == 0x7816) /*&& wat?*/)
					result = true;
			}
			else
			{
				if (robe <= 0x2687)
				{
					if (robe < 0x2683)
					{
						if (robe >= 0x204E && robe <= 0x204F && (layer == OL_HAIR || !g_ConfigManager.DrawHelmetsOnShroud))
							result = true;
					}
					else
						result = true;
				}
				else if (robe == 0x2FB9 || robe == 0x3173)
					result = true;
			}

			break;
		}
		case OL_SKIRT:
		{
			const ushort &skirt = m_CharacterLayerAnimID[OL_SKIRT];

			if (skirt == 0x01C7 || skirt == 0x01E4)
			{
				//const ushort &pants = m_CharacterLayerAnimID[OL_PANTS];

				//result = (!pants || pants == 0x0200);
			}

			break;
		}
		default:
			break;
	}

	return result;
}
//----------------------------------------------------------------------------------
void CAnimationManager::AddUopAnimData(unsigned long long hash, UOPAnimationData animData)
{
	uopFrameDataRefMap[hash] = animData;
}
//----------------------------------------------------------------------------------
