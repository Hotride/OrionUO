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
//----------------------------------------------------------------------------------
CAnimationManager g_AnimationManager;
//----------------------------------------------------------------------------------
int CAnimationManager::m_UsedLayers[8][USED_LAYER_COUNT] =
{
	{ //dir 0
		OL_SHIRT,
		OL_PANTS,
		OL_SHOES,
		OL_LEGS,
		OL_TORSO,
		OL_BRACELET,
		OL_ARMS,
		OL_GLOVES,
		OL_SKIRT,
		OL_TUNIC,
		OL_NECKLACE,
		OL_HAIR,
		OL_ROBE,
		OL_WAIST,
		OL_BEARD,
		OL_HELMET,
		OL_1_HAND,
		OL_2_HAND,
		OL_CLOAK
	},
	{ //dir 1
		OL_SHIRT,
		OL_PANTS,
		OL_SHOES,
		OL_LEGS,
		OL_TORSO,
		OL_BRACELET,
		OL_ARMS,
		OL_GLOVES,
		OL_SKIRT,
		OL_TUNIC,
		OL_NECKLACE,
		OL_HAIR,
		OL_ROBE,
		OL_WAIST,
		OL_BEARD,
		OL_HELMET,
		OL_1_HAND,
		OL_CLOAK,
		OL_2_HAND
	},
	{ //dir 2
		OL_SHIRT,
		OL_PANTS,
		OL_SHOES,
		OL_LEGS,
		OL_TORSO,
		OL_BRACELET,
		OL_ARMS,
		OL_GLOVES,
		OL_SKIRT,
		OL_TUNIC,
		OL_NECKLACE,
		OL_HAIR,
		OL_ROBE,
		OL_WAIST,
		OL_BEARD,
		OL_HELMET,
		OL_1_HAND,
		OL_CLOAK,
		OL_2_HAND
	},
	{ //dir 3
		OL_CLOAK,
		OL_SHIRT,
		OL_PANTS,
		OL_SHOES,
		OL_LEGS,
		OL_TORSO,
		OL_BRACELET,
		OL_ARMS,
		OL_GLOVES,
		OL_SKIRT,
		OL_TUNIC,
		OL_NECKLACE,
		OL_HAIR,
		OL_ROBE,
		OL_WAIST,
		OL_BEARD,
		OL_HELMET,
		OL_1_HAND,
		OL_2_HAND
	},
	{ //dir 4
		OL_SHIRT,
		OL_PANTS,
		OL_SHOES,
		OL_LEGS,
		OL_TORSO,
		OL_BRACELET,
		OL_ARMS,
		OL_GLOVES,
		OL_SKIRT,
		OL_TUNIC,
		OL_NECKLACE,
		OL_HAIR,
		OL_ROBE,
		OL_WAIST,
		OL_BEARD,
		OL_HELMET,
		OL_1_HAND,
		OL_CLOAK,
		OL_2_HAND
	},
	{ //dir 5
		OL_SHIRT,
		OL_PANTS,
		OL_SHOES,
		OL_LEGS,
		OL_TORSO,
		OL_BRACELET,
		OL_ARMS,
		OL_GLOVES,
		OL_SKIRT,
		OL_TUNIC,
		OL_NECKLACE,
		OL_HAIR,
		OL_ROBE,
		OL_WAIST,
		OL_BEARD,
		OL_HELMET,
		OL_1_HAND,
		OL_CLOAK,
		OL_2_HAND
	},
	{ //dir 6
		OL_SHIRT,
		OL_PANTS,
		OL_SHOES,
		OL_LEGS,
		OL_TORSO,
		OL_BRACELET,
		OL_ARMS,
		OL_GLOVES,
		OL_SKIRT,
		OL_TUNIC,
		OL_NECKLACE,
		OL_HAIR,
		OL_ROBE,
		OL_WAIST,
		OL_BEARD,
		OL_HELMET,
		OL_1_HAND,
		OL_2_HAND,
		OL_CLOAK
	},
	{ //dir 7
		OL_SHIRT,
		OL_PANTS,
		OL_SHOES,
		OL_LEGS,
		OL_TORSO,
		OL_BRACELET,
		OL_ARMS,
		OL_GLOVES,
		OL_SKIRT,
		OL_TUNIC,
		OL_NECKLACE,
		OL_HAIR,
		OL_ROBE,
		OL_WAIST,
		OL_BEARD,
		OL_HELMET,
		OL_1_HAND,
		OL_2_HAND,
		OL_CLOAK
	},
};
//----------------------------------------------------------------------------------
CAnimationManager::CAnimationManager()
: m_UsedAnimList(NULL), m_Color(0), m_AnimGroup(0), m_Direction(0), m_Sitting(0),
m_Transform(false)
{
	memset(m_AddressIdx, 0, sizeof(m_AddressIdx));
	memset(m_AddressMul, 0, sizeof(m_AddressMul));
	memset(m_SizeIdx, 0, sizeof(m_SizeIdx));
	memset(m_DataIndex, 0, sizeof(m_DataIndex));
	memset(&m_CorpseReplaces[0], 0, sizeof(m_CorpseReplaces));
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
	IFOR(i, 0, MAX_ANIMATIONS_DATA_INDEX_COUNT)
	{
		m_DataIndex[i].Address = 0;
		m_DataIndex[i].Offset = 0;
		m_DataIndex[i].Graphic = 0;

		ushort animID = 0;
		uint findID = 0;

		if (i >= 200)
		{
			if (i >= 400) //People
			{
				animID = i - 400;
				findID = ((animID * 175) + 35000) * sizeof(ANIM_IDX_BLOCK);
			}
			else //Low
			{
				animID = i - 200;
				findID = ((animID * 65) + 22000) * sizeof(ANIM_IDX_BLOCK);
			}
		}
		else //Hight
		{
			animID = i;
			findID = (animID * 110) * sizeof(ANIM_IDX_BLOCK);
		}
		
		if (findID < m_SizeIdx[0])
		{
			PANIM_IDX_BLOCK aidx = (PANIM_IDX_BLOCK)(m_AddressIdx[0] + findID);
			
			if (aidx->Size && aidx->Position != 0xFFFFFFFF && aidx->Size != 0xFFFFFFFF)
			{
				m_DataIndex[i].Address = (uint)aidx;
				m_DataIndex[i].Offset = m_AddressMul[0];
				m_DataIndex[i].Graphic = i;// animID;
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
				uint graphic = vh->BlockID;

				//TPRINT("vh->ID = 0x%02X vh->BlockID = 0x%08X\n", vh->FileID, graphic);
				
				IFOR(i, 0, 0x0800)
				{
					ushort animID = i;
					uint findID = 0;

					if (i >= 200)
					{
						if (i >= 400) //People
						{
							animID -= 400;
							findID = (animID * 175) + 35000;
						}
						else //Low
						{
							animID -= 200;
							findID = (animID * 65) + 22000;
						}
					}
					else //Hight
						findID = animID * 110;

					if (graphic == findID)
					{
						m_DataIndex[i].Address = (uint)vh;
						m_DataIndex[i].Offset = 0xFFFFFFFF;
						m_DataIndex[i].Graphic = i; // animID;
						//TPRINT("\tLoad verdata anim ID = 0x%04X\n", animID);
					}
				}
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
	Load(verdata);

	WISP_FILE::CTextFileParser newBodyParser("", " \t,{}", "#;//", "");
	WISP_FILE::CTextFileParser bodyParser(g_App.FilePath("Body.def").c_str(), " \t", "#;//", "{}");
	WISP_FILE::CTextFileParser corpseParser(g_App.FilePath("Corpse.def").c_str(), " \t", "#;//", "{}");
	WISP_FILE::CTextFileParser bodyconvParser(g_App.FilePath("Bodyconv.def").c_str(), " \t", "#;//", "");

	while (!bodyParser.IsEOF())
	{
		STRING_LIST strings = bodyParser.ReadTokens();

		if (strings.size() >= 3)
		{
			ushort index = atoi(strings[0].c_str());

			if (index >= MAX_ANIMATIONS_DATA_INDEX_COUNT)
				continue;

			STRING_LIST newBody = newBodyParser.GetTokens(strings[1].c_str());

			int size = (int)newBody.size();

			IFOR(i, 0, size)
			{
				ushort checkIndex = atoi(newBody[i].c_str());

				if (checkIndex >= MAX_ANIMATIONS_DATA_INDEX_COUNT || !m_DataIndex[checkIndex].Offset)
					continue;

				memcpy(&m_DataIndex[index], &m_DataIndex[checkIndex], sizeof(CIndexAnimation));
				m_DataIndex[index].Group = NULL;
				m_DataIndex[index].Color = atoi(strings[2].c_str());

				break;
			}
		}
	}

	while (!bodyconvParser.IsEOF())
	{
		STRING_LIST strings = bodyconvParser.ReadTokens();

		if (strings.size() >= 5)
		{
			int anim[4] = { -1, -1, -1, -1 };

			ushort index = atoi(strings[0].c_str());

			IFOR(i, 0, 4)
				anim[i] = atoi(strings[i + 1].c_str());

			if (index >= MAX_ANIMATIONS_DATA_INDEX_COUNT)
				continue;

			int startAnimID = -1;
			int animFile = 1;
			ushort convertedAnimID = 0;
			ushort realAnimID = 0;

			if (anim[0] != -1 && m_AddressIdx[2] != 0 && m_AddressMul[2] != 0)
			{
				animFile = 2;

				if (anim[0] == 68)
					anim[0] = 122;

				realAnimID = anim[0];

				if (anim[0] >= 200) //Low
				{
					convertedAnimID = anim[0] - 200;
					startAnimID = (convertedAnimID * 65) + 22000;
				}
				else //Hight
				{
					startAnimID = anim[0] * 110;
					convertedAnimID = anim[0];
				}
			}
			else if (anim[1] != -1 && m_AddressIdx[3] != 0 && m_AddressMul[3] != 0)
			{
				animFile = 3;
				realAnimID = anim[1];

				if (anim[1] >= 200)
				{
					if (anim[1] >= 400) //People
					{
						convertedAnimID = anim[1] - 400;
						startAnimID = (convertedAnimID * 175) + 35000;
					}
					else //Low
					{
						convertedAnimID = anim[1] - 200;
						startAnimID = (convertedAnimID * 110) + 22000;
					}
				}
				else //Hight
				{
					convertedAnimID = anim[1];
					startAnimID = (anim[1] * 65) + 9000;
				}
			}
			else if (anim[2] != -1 && m_AddressIdx[4] != 0 && m_AddressMul[4] != 0)
			{
				animFile = 4;
				realAnimID = anim[2];

				if (anim[2] >= 200)
				{
					if (anim[2] >= 400) //People
					{
						convertedAnimID = anim[2] - 400;
						startAnimID = (convertedAnimID * 175) + 35000;
					}
					else //Low
					{
						convertedAnimID = anim[2] - 200;
						startAnimID = (convertedAnimID * 65) + 22000;
					}
				}
				else //Hight
				{
					convertedAnimID = anim[2];
					startAnimID = anim[2] * 110;
				}
			}
			else if (anim[3] != -1 && m_AddressIdx[5] != 0 && m_AddressMul[5] != 0)
			{
				animFile = 5;
				realAnimID = anim[3];

				if (anim[3] >= 200)
				{
					if (anim[3] >= 400) //People
					{
						convertedAnimID = anim[3] - 400;
						startAnimID = (convertedAnimID * 175) + 35000;
					}
					else //Low
					{
						convertedAnimID = anim[3] - 200;
						startAnimID = (convertedAnimID * 65) + 22000;
					}
				}
				else //Hight
				{
					convertedAnimID = anim[3];
					startAnimID = anim[3] * 110;
				}
			}

			if (animFile != 1 && startAnimID != -1)
			{
				startAnimID = startAnimID * sizeof(ANIM_IDX_BLOCK);

				if ((uint)startAnimID < m_SizeIdx[animFile])
				{
					PANIM_IDX_BLOCK aidx = (PANIM_IDX_BLOCK)(m_AddressIdx[animFile] + startAnimID);

					if (aidx->Size && aidx->Position != 0xFFFFFFFF && aidx->Size != 0xFFFFFFFF)
					{
						m_DataIndex[index].Address = (uint)aidx;
						m_DataIndex[index].Offset = m_AddressMul[animFile];
						m_DataIndex[index].Graphic = realAnimID;// convertedAnimID;
					}
				}
			}
		}
	}

	while (!corpseParser.IsEOF() && false)
	{
		STRING_LIST strings = corpseParser.ReadTokens();

		if (strings.size() >= 3)
		{
			ushort index = atoi(strings[0].c_str());

			if (index >= MAX_ANIMATIONS_DATA_INDEX_COUNT)
				continue;

			STRING_LIST newBody = newBodyParser.GetTokens(strings[1].c_str());

			int size = (int)newBody.size();

			IFOR(i, 0, size)
			{
				ushort checkIndex = atoi(newBody[i].c_str());

				if (checkIndex >= MAX_ANIMATIONS_DATA_INDEX_COUNT || !m_DataIndex[checkIndex].Offset)
					continue;

				m_CorpseReplaces[index] = checkIndex;
				//m_DataIndex[index].Color = atoi(strings[2].c_str());

				break;
			}
		}
	}
}
//----------------------------------------------------------------------------------
/*!
Получить индекс группы по индексу картинки
@param [__in] id Индекс картинки
@return Группа анимаций
*/
ANIMATION_GROUPS CAnimationManager::GetGroupIndex(ushort id)
{
	ANIMATION_GROUPS group = AG_HIGHT;

	if (id >= 200)
	{
		if (id >= 400)
			group = AG_PEOPLE;
		else
			group = AG_LOW;
	}
	
	return group;
}
//----------------------------------------------------------------------------------
/*!
Получить индекс группы смерти анимации
@param [__in] id Byltrc rfhnbyrb
@param [__in] second Группа смерти номер 2
@return Индекс группы анимации
*/
uchar CAnimationManager::GetDieGroupIndex(ushort id, bool second)
{
	uchar group = 0;

	if (id >= 200)
	{
		if (id >= 400)
			group = (uchar)(second ? PAG_DIE_2 : PAG_DIE_1);
		else
			group = (uchar)(second ? LAG_DIE_2 : LAG_DIE_1);
	}
	else
		group = (uchar)(second ? HAG_DIE_2 : HAG_DIE_1);
	
	return group;
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

			/*if (mirror)
			{
				drawX += (SITTING_OFFSET_X / 2) - 1;

				drawY += 5 + sittingData.MirrorOffsetY;
			}
			else
			{
				drawX -= SITTING_OFFSET_X;

				drawY += 12 + sittingData.OffsetY;
			}*/
}
//----------------------------------------------------------------------------------
/*!
Получить ссылку на данные анимации
@param [__in] id Индекс картинки
@return Ссылка на анимацию
*/
CTextureAnimation *CAnimationManager::GetAnimation(ushort id)
{
	if (id >= MAX_ANIMATIONS_DATA_INDEX_COUNT)
		return NULL;

	CTextureAnimation *anim = m_DataIndex[id].Group;

	if (anim == NULL)
	{
		anim = new CTextureAnimation();
		m_DataIndex[id].Group = anim;

		m_UsedAnimList.push_back(&m_DataIndex[id]);
	}

	return anim;
}
//----------------------------------------------------------------------------------
/*!
Очистка неиспользуемых текстур
@param [__in] ticks Таймер удаления
@return 
*/
void CAnimationManager::ClearUnusedTextures(uint ticks)
{
	/*TLinkedList *list = m_UsedAnimList;
	TLinkedList *prev = list;

	ticks -= CLEAR_TEXTURES_DELAY;

	while (list != NULL)
	{
		TIndexAnimation *obj = (TIndexAnimation*)list->Data;
		TTextureAnimation *anim = obj->Group;

		if (anim != NULL)
		{
			TTextureAnimationGroup *group = (TTextureAnimationGroup*)anim->m_Items;

			while (group != NULL)
			{
				TTextureAnimationGroup *nextGroup = (TTextureAnimationGroup*)group->m_Next;

				TTextureAnimationDirection *direction = (TTextureAnimationDirection*)group->m_Items;

				while (direction != NULL)
				{
					TTextureAnimationDirection *nextDirection = (TTextureAnimationDirection*)direction->m_Next;

					if (direction->LastAccessTime < ticks)
						group->Delete(direction);

					direction = nextDirection;
				}

				if (group->m_Items == NULL)
					anim->Delete(group);

				group = nextGroup;
			}

			TLinkedList *next = list->Next;

			if (anim->m_Items == NULL)
			{
				obj->Group = NULL;

				if (list == m_UsedAnimList)
				{
					m_UsedAnimList = next;
					prev = next;
				}
				else
					prev->Next = next;

				list->Next = NULL;
				delete list;
			}

			list = next;
		}
		else
		{
			prev = list;
			list = list->Next;
		}
	}*/
}
//----------------------------------------------------------------------------------
/*!
Загрузка картинок для указанного направления персонажа
@param [__in] direction Ссылка на направление анимации
@param [__in] id Индекс картинки
@param [__in] offset Смещение относительно начала анимаций
@return true в случае успешной загрузки
*/
bool CAnimationManager::ExecuteDirectionGroup(CTextureAnimationDirection *direction, ushort &id, int &offset)
{
	direction->Address = 0;

	if (id > MAX_ANIMATIONS_DATA_INDEX_COUNT || m_DataIndex[id].Address == NULL)
		return false;

	puchar dataStart = NULL;

	if (m_DataIndex[id].Offset == 0xFFFFFFFF) //in verdata
	{
		PVERDATA_HEADER vh = (PVERDATA_HEADER)(m_DataIndex[id].Address + (offset * sizeof(VERDATA_HEADER)));

		if (vh->Position != 0xFFFFFFFF)
			direction->Address = (uint)g_FileManager.m_VerdataMul.Start + vh->Position;
		
		dataStart = (puchar)direction->Address;
	}
	else //in original mulls
	{
		PANIM_IDX_BLOCK aidx = (PANIM_IDX_BLOCK)(m_DataIndex[id].Address + (offset * sizeof(ANIM_IDX_BLOCK)));
	
		if (aidx->Size && aidx->Position != 0xFFFFFFFF && aidx->Size != 0xFFFFFFFF)
			direction->Address = (uint)aidx;

		dataStart = (puchar)(m_DataIndex[id].Offset + aidx->Position);
	}

	if (!direction->Address)
		return false;

	puchar ptr = dataStart;

	dataStart += sizeof(ushort[256]); //Palette

	pushort palette = (pushort)ptr;
	ptr = dataStart;

	int frameCount = *((puint)ptr);
	direction->FrameCount = frameCount;
	ptr += sizeof(uint);

	puint frameOffset = (puint)ptr;
	ptr += (frameCount * sizeof(uint));

	//ushort color = m_DataIndex[id].Color;

	IFOR(i, 0, frameCount)
	{
		CTextureAnimationFrame *frame = direction->GetFrame(i);

		if (frame->Texture != 0)
			continue;

		pushort p = (pushort)((uint)dataStart + frameOffset[i]);

		//LOG("Load anim: 0x%04X[%i]: 0x%08X: ", id, i, p);

		short imageCenterX = (short)*p;
		frame->CenterX = imageCenterX;
		p++;

		short imageCenterY = (short)*p;
		frame->CenterY = imageCenterY;
		p++;

		short imageWidth = (short)*p;
		frame->Width = imageWidth;
		p++;

		short imageHeight = (short)*p;
		frame->Height = imageHeight;
		p++;

		//LOG("%i %i, %i %i\n", imageCenterX, imageCenterY, imageWidth, imageHeight);

		int x = 0;
		int y = 0;

		int blocksize = imageWidth * imageHeight;

		pushort pData = new ushort[blocksize];
		memset(&pData[0], 0, blocksize * 2);

		ushort prevLineNum = 0xFF;

		while (true)
		{
			ushort rowHeader = *p;
			p++;

			ushort rowOfs = *p;
			p++;

			if (rowHeader == 0x7FFF || rowOfs == 0x7FFF)
				break;

			ushort runLength = rowHeader & 0xFFF;
			ushort lineNum = rowHeader >> 12;

			x = (rowOfs >> 6) & 0x3FF;

			if (rowOfs & 0x8000)
				x = imageCenterX + x - 0x400;
			else
				x = imageCenterX + x;

			if (prevLineNum != 0xFF && lineNum != prevLineNum)
				y++;

			prevLineNum = lineNum;

			if (y >= 0)
			{
				if (y >= imageHeight)
					break;

				puchar b = (puchar)p;

				IFOR(j, 0, runLength)
				{
					ushort val = palette[*b];

					//if (color)
					//	val = g_ColorManager.GetColor16(val, color);

					ushort a = val ? 0x8000 : 0;
					int block = y * imageWidth + (x + j);
					pData[block] = a | val;

					b++;
				}

				p = (pushort)b;

			}
		}

		g_GL.BindTexture16(frame->Texture, imageWidth, imageHeight, pData);

		delete pData;
	}

	return true;
}
//----------------------------------------------------------------------------------
bool CAnimationManager::TestImagePixels(CTextureAnimationDirection *direction, uchar &frame, ushort &id, int &checkX, int &checkY)
{
	puchar dataStart = NULL;

	if (m_DataIndex[id].Offset == 0xFFFFFFFF)
		dataStart = (puchar)direction->Address;
	else
	{
		PANIM_IDX_BLOCK aidx = (PANIM_IDX_BLOCK)direction->Address;
	
		dataStart = (puchar)(m_DataIndex[id].Offset + aidx->Position);
	}

	puchar ptr = dataStart;

	pushort palette = (pushort)ptr;

	dataStart += sizeof(ushort[256]); //Palette
	ptr = dataStart;

	int frameCount = *((puint)ptr);
	ptr += sizeof(uint);

	puint frameOffset = (puint)ptr;
	ptr += (frameCount * sizeof(uint));

	pushort data = (pushort)((uint)dataStart + frameOffset[frame]);

	short imageCenterX = (short)*data++;
	short imageCenterY = (short)*data++;
	short imageWidth = (short)*data++;
	short imageHeight = (short)*data++;

	pushort p = (pushort)data;

	int x = 0;
	int y = 0;

	ushort prevLineNum = 0xFF;

	while (true)
	{
		ushort rowHeader = *p;
		p++;

		ushort rowOfs = *p;
		p++;

		if (rowHeader == 0x7FFF || rowOfs == 0x7FFF)
			break;

		ushort runLength = rowHeader & 0xFFF;
		ushort lineNum = rowHeader >> 12;

		x = (rowOfs >> 6) & 0x3FF;

		x += imageCenterX;

		if (rowOfs & 0x8000)
			x -= 0x0400;

		if (prevLineNum != 0xFF && lineNum != prevLineNum)
			y++;

		prevLineNum = lineNum;

		if (y >= 0)
		{
			if (y >= imageHeight)
				break;

			puchar b = (puchar)p;

			IFOR(j, 0, runLength)
			{
				if ((x + j) == checkX && y == checkY)
					return (palette[*b] != 0);

				b++;
			}

			p = (pushort)b;
		}
	}

	return false;
}
//----------------------------------------------------------------------------------
bool CAnimationManager::TestPixels(CGameObject *obj, int x, int y, bool &mirror, uchar &frameIndex, ushort id)
{
	if (obj == NULL)
		return false;

	if (!id)
		id = obj->GetMountAnimation();
	
	CTextureAnimation *anim = GetAnimation(id);

	if (anim == NULL)
		return false;

	CTextureAnimationGroup *group = anim->GetGroup(m_AnimGroup);
	CTextureAnimationDirection *direction = group->GetDirection(m_Direction);

	if (direction->Address == 0)
	{
		int offset = (m_AnimGroup * 5) + m_Direction;

		if (!ExecuteDirectionGroup(direction, id, offset))
			return false;
	}

	int fc = direction->FrameCount;

	if (fc > 0 && frameIndex >= fc)
	{
		if (obj->IsCorpse())
			frameIndex = fc - 1;
		else
			frameIndex = 0;
	}

	CTextureAnimationFrame *frame = direction->GetFrame(frameIndex);

	if (frame != NULL)
	{
		y -= frame->Height + frame->CenterY;
		
		if (mirror)
			x -= frame->Width - frame->CenterX;
		else
			x -= frame->CenterX;

		x = g_MouseManager.Position.X - x;
		y = g_MouseManager.Position.Y - y;

		if (mirror)
			x = frame->Width - x;

		if (x >= 0 && x < frame->Width && y >= 0 && y < frame->Height)
			return TestImagePixels(direction, frameIndex, id, x, y);
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
CTextureAnimationFrame *CAnimationManager::GetFrame(CGameObject *obj, uchar &frameIndex, ushort id)
{
	CTextureAnimationFrame *frame = NULL;

	if (obj != NULL)
	{
		if (!id)
			id = obj->GetMountAnimation();

		if (id < MAX_ANIMATIONS_DATA_INDEX_COUNT)
		{
			CTextureAnimation *anim = m_DataIndex[id].Group;

			if (anim != NULL)
			{
				CTextureAnimationGroup *group = anim->GetGroup(m_AnimGroup);
				CTextureAnimationDirection *direction = group->GetDirection(m_Direction);

				if (direction->Address != 0)
				{
					int fc = direction->FrameCount;

					if (fc > 0 && frameIndex >= fc)
					{
						if (obj->IsCorpse())
							frameIndex = fc - 1;
						else
							frameIndex = 0;
					}

					frame = direction->GetFrame(frameIndex);
				}
			}
		}
	}

	return frame;
}
//----------------------------------------------------------------------------------
void CAnimationManager::Draw(CGameObject *obj, int x, int y, bool &mirror, uchar &frameIndex, int id)
{
	if (obj == NULL)
		return;

	bool isShadow = (id >= 0x10000);

	if (isShadow)
		id -= 0x10000;

	if (!id)
		id = obj->GetMountAnimation();

	if (id >= MAX_ANIMATIONS_DATA_INDEX_COUNT)
		return;

	ushort objColor = obj->Color;
	bool partialHue = obj->IsPartialHue();

	if (objColor & 0x8000)
	{
		partialHue = true;
		objColor &= 0x7FFF;
	}

	if (!objColor)
	{
		objColor = m_DataIndex[id].Color;
		partialHue = true;
	}

	CTextureAnimation *anim = m_DataIndex[id].Group;

	if (anim == NULL)
	{
		anim = new CTextureAnimation();
		m_DataIndex[id].Group = anim;

		m_UsedAnimList.push_back(&m_DataIndex[id]);
	}
	
	CTextureAnimationGroup *group = anim->GetGroup(m_AnimGroup);
	CTextureAnimationDirection *direction = group->GetDirection(m_Direction);

	if (direction->Address == 0)
	{
		int offset = (m_AnimGroup * 5) + m_Direction;

		ushort wID = id;
		if (!ExecuteDirectionGroup(direction, wID, offset))
			return;
	}

	int fc = direction->FrameCount;

	if (fc > 0 && frameIndex >= fc)
	{
		if (obj->IsCorpse())
			frameIndex = fc - 1;
		else
			frameIndex = 0;
	}

	CTextureAnimationFrame *frame = direction->GetFrame(frameIndex);
	
	if (frame->Texture == 0)
	{
		int offset = (m_AnimGroup * 5) + m_Direction;
		
		ushort wID = id;
		if (!ExecuteDirectionGroup(direction, wID, offset))
			return;
	}
	
	if (frame->Texture != 0)
	{
		if (mirror)
			x -= frame->Width - frame->CenterX;
		else
			x -= frame->CenterX;

		y -= frame->Height + frame->CenterY;

		/*if (!isShadow)
		{
			if (!obj->NPC && ((CGameItem*)obj)->Layer == OL_MOUNT)
				LOG("mount frame: 0x%04X %i %i ; %i %i\n", id, frame->CenterX, frame->CenterY, frame->Width, frame->Height);
			else
				LOG("NPC frame: 0x%04X %i %i ; %i %i\n", id, frame->CenterX, frame->CenterY, frame->Width, frame->Height);
		}*/

		if (obj->IsHuman())
		{
			short frameHeight = frame->Height;
			m_CharacterFrameStartY = y;
			m_CharacterFrameHeight = frame->Height;
			m_StartCharacterWaistY = (int)(frameHeight * UPPER_BODY_RATIO) + m_CharacterFrameStartY;
			m_StartCharacterKneesY = (int)(frameHeight * MID_BODY_RATIO) + m_CharacterFrameStartY;
			m_StartCharacterFeetY = (int)(frameHeight * LOWER_BODY_RATIO) + m_CharacterFrameStartY;
		}

		if (isShadow)
		{
			glUniform1iARB(g_ShaderDrawMode, 12);

			glEnable(GL_BLEND);
			glBlendFunc(GL_DST_COLOR, GL_ZERO);

			int fWidth = frame->Width;
			int fHeight = frame->Height;

			if (obj->NPC)
			{
				CGameItem *mount = ((CGameCharacter*)obj)->FindLayer(OL_MOUNT);

				if (mount != NULL)
				{
					WORD mountID = mount->GetMountAnimation();

					ANIMATION_DIMENSIONS dim = GetAnimationDimensions(mount, frameIndex, m_Direction, ((CGameCharacter*)obj)->GetAnimationGroup(mountID));

					/*y += (frame->Height + frame->CenterY);

					if (mirror)
						x += (frame->Width - frame->CenterX);
					else
						x += frame->CenterX;*/

					//y += (frame->Height + frame->CenterY + 3);
					//y -= (dim.Height + dim.CenterY + 3);
					//y += 10;
					/*y -= (dim.Height + dim.CenterY + 3);

					if (mirror)
						x -= (dim.Width - dim.CenterX);
					else
						x -= dim.CenterX;*/

					//y += (frame->CenterY + dim.CenterY);
					//y -= frame->Height - frame->CenterY;

					if (mirror)
						x += frame->Width - frame->CenterX;
					else
						x += frame->CenterX;
					
					/*if (mirror)
						x -= (dim.CenterX - frame->CenterX);
					else
						x -=  (frame->CenterX - dim.CenterX);*/

					//TPRINT("Character: %i %i, %i %i\n", frame->Width, frame->Height, frame->CenterX, frame->CenterY);
					//TPRINT("Mount: %i %i, %i %i\n", dim.Width, dim.Height, dim.CenterX, dim.CenterY);
				}
			}

			g_GL.DrawShadow(frame->Texture, x, y, (float)fWidth, fHeight / 2.0f, mirror);

			glDisable(GL_BLEND);
		}
		else
		{
			ushort color = m_Color;

			if (color)
				partialHue = false;
			else
				color = objColor;

			int drawMode = (!g_GrayedPixels && color);
			bool spectralColor = (color & SPECTRAL_COLOR);
			
			if (!g_GrayedPixels && spectralColor)
			{
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
			else if (drawMode)
			{
				if (partialHue)
					drawMode = 2;

				g_ColorManager.SendColorsToShader(color);
			}

			glUniform1iARB(g_ShaderDrawMode, drawMode);

			if (m_Transform)
			{
				float h3mod = UPPER_BODY_RATIO;
				float h6mod = MID_BODY_RATIO;
				float h9mod = LOWER_BODY_RATIO;

				if (!obj->NPC)
				{
					float itemsEndY = (float)(y + frame->Height);

					//Определяем соотношение верхней части текстуры, до перелома.
					if (y >= m_StartCharacterWaistY)
						h3mod = 0;
					else if (itemsEndY <= m_StartCharacterWaistY)
						h3mod = 1.0f;
					else
					{
						float upperBodyDiff = (float)(m_StartCharacterWaistY - y);
						h3mod = upperBodyDiff / frame->Height;
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

						h6mod = h3mod + midBodyDiff / frame->Height;
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
						h9mod = h6mod + lowerBodyDiff / frame->Height;
						if (h9mod < 0)
							h9mod = 0;
					}
				}
				g_GL.DrawSitting(frame->Texture, x, y, frame->Width, frame->Height, mirror, h3mod, h6mod, h9mod);
			}			
			else
				g_GL.Draw(frame->Texture, x, y, frame->Width, frame->Height, mirror);

			if (spectralColor)
				glDisable(GL_BLEND);
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

	y -= 3;
	int originalX = x;
	int originalY = y;

	ushort targetColor = 0;
	bool needHPLine = false;
	uint serial = obj->Serial;

	if (obj->Hidden())
		m_Color = 0x038D;
	else if (g_StatusbarUnderMouse == serial)
		m_Color = g_ConfigManager.GetColorByNotoriety(obj->Notoriety);
	else
		m_Color = 0;

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
	int drawX = (int)(x + obj->OffsetX);
	int drawY = (int)(y + obj->OffsetY) - (z * 4) - (int)obj->OffsetZ;
	
	/*DRAW_FRAME_INFORMATION &dfInfo = obj->m_FrameInfo;
	CurrentShader->Pause();
	g_GL.DrawPolygone(drawX - dfInfo.OffsetX, drawY - dfInfo.OffsetY, dfInfo.Width, dfInfo.Height);
	CurrentShader->Resume();*/

	if (goi != NULL) //Draw mount
	{
		m_Sitting = 0;
		lightOffset += 20;

		ushort mountID = goi->GetMountAnimation();

		Draw(obj, drawX, drawY + 10, mirror, animIndex, 0x10000);
		m_AnimGroup = obj->GetAnimationGroup(mountID);

		int mountDrawY = drawY;
		int mountedDrawY = drawY;

		/*if (goi->GetStaticData()->Quality == 25)
		{
			if (goi->GetStaticData()->Weight == 0xFF)
			{
				mountDrawY -= 40;
				mountedDrawY -= 20;
			}
			else
			{
				mountedDrawY += 20;
				mountDrawY += 40;
			}
		}*/

		Draw(goi, drawX, mountDrawY, mirror, animIndex, mountID + 0x10000);
		Draw(goi, drawX, mountDrawY, mirror, animIndex, mountID);

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
		else if (!obj->Dead())
			Draw(obj, drawX, drawY, mirror, animIndex, 0x10000);
	}
	
	m_AnimGroup = animGroup;

	Draw(obj, drawX, drawY, mirror, animIndex); //Draw character

	if (obj->IsHuman()) //Draw layred objects
	{
		if (!obj->Dead())
		{
			DrawEquippedLayers(obj, drawX, drawY, mirror, layerDir, animIndex, lightOffset);
			SITTING_INFO_DATA sittingData = SITTING_INFO[m_Sitting - 1];
			if (m_Sitting && m_Direction == 3 && sittingData.DrawBack && obj->FindLayer(OL_CLOAK) == NULL)
			{
				ushort graphic = sittingData.Graphic;
				for (CRenderWorldObject *ro = obj->m_PrevXY; ro != NULL; ro = (CRenderWorldObject*)ro->m_PrevXY)
				{
					ushort roGraphic = ro->Graphic;
					if (roGraphic >= 0x4000)
						roGraphic -= 0x4000;
					if (roGraphic == graphic)
					{
						//оффсеты для ножниц
					    int xOffset = mirror ? -20 : 0;
					    int yOffset = -70;

						g_GL.PushScissor(drawX + xOffset, drawY + yOffset, 20, 40);
						g_Orion.DrawStaticArt(graphic, ro->Color, originalX, originalY, ro->Z);
						g_GL.PopScissor();
						break;
					}
				}
			}
		}
		else
		{
			goi = obj->FindLayer(OL_ROBE);

			if (goi != NULL && goi->AnimID)
				Draw(goi, drawX, drawY, mirror, animIndex, goi->AnimID);
		}
	}
	
	if (!g_ConfigManager.DisableNewTargetSystem && g_NewTargetSystem.Serial == obj->Serial)
	{
		ushort id = obj->GetMountAnimation();

		if (id < MAX_ANIMATIONS_DATA_INDEX_COUNT)
		{
			CTextureAnimation *anim = m_DataIndex[id].Group;

			if (anim != NULL)
			{
				CTextureAnimationGroup *group = anim->GetGroup(m_AnimGroup);
				CTextureAnimationDirection *direction = group->GetDirection(m_Direction);

				if (direction->Address != 0)
				{
					CTextureAnimationFrame *frame = direction->GetFrame(0);

					int frameWidth = 20;
					int frameHeight = 20;

					if (frame != NULL)
					{
						frameWidth = frame->Width;
						frameHeight = frame->Height;
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
	
	if (goi != NULL) //Check mount
	{
		ushort mountID = goi->GetMountAnimation();

		m_AnimGroup = obj->GetAnimationGroup(mountID);

		int mountDrawY = drawY;

		if (goi->GetStaticData()->Quality == 25)
		{
			if (goi->GetStaticData()->Weight == 0xFF)
				mountDrawY -= 40;
			else
				mountDrawY += 40;
		}

		if (TestPixels(goi, drawX, mountDrawY, mirror, animIndex, mountID))
			return true;

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

	bool result = TestPixels(obj, drawX, drawY, mirror, animIndex); //Check character

	if (obj->IsHuman()) //Check layred objects
	{
		if (!obj->Dead())
		{
			IFOR(l, 0, USED_LAYER_COUNT && !result)
			{
				goi = obj->FindLayer(m_UsedLayers[layerDir][l]);

				if (goi != NULL && goi->AnimID)
					result = TestPixels(goi, drawX, drawY, mirror, animIndex, goi->AnimID);
			}
		}
		else if (!result)
		{
			goi = obj->FindLayer(OL_ROBE);

			if (goi != NULL && goi->AnimID)
				result = TestPixels(goi, drawX, drawY, mirror, animIndex, goi->AnimID);
		}
	}

	return result;
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
void CAnimationManager::DrawCorpse(CGameItem *obj, int x, int y, int z)
{
	y -= 3;
	m_Sitting = 0;
	m_Direction = obj->Layer;

	if (m_Direction & 0x80)
		m_Direction &= 0x7F;

	bool mirror = false;

	GetAnimDirection(m_Direction, mirror);

	if (obj->Hidden())
		m_Color = 0x038D;
	else
		m_Color = 0;

	uchar animIndex = obj->AnimIndex;
	m_AnimGroup = GetDieGroupIndex(obj->GetMountAnimation(), obj->UsedLayer);
	
	y -= (z * 4);

	Draw(obj, x, y, mirror, animIndex); //Draw animation

	IFOR(l, 0, USED_LAYER_COUNT)
	{
		CGameItem *goi = obj->FindLayer(m_UsedLayers[m_Direction][l]);
		
		if (goi != NULL)
		{
			if (goi->AnimID)
				Draw(goi, x, y, mirror, animIndex, goi->AnimID);

			if (goi->IsLightSource() && g_GameScreen.UseLight)
				g_GameScreen.AddLight(obj, goi, x, y - 20);
		}
	}
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
bool CAnimationManager::CorpsePixelsInXY(CGameItem *obj, int x, int y, int z)
{
	m_Sitting = 0;
	m_Direction = obj->Layer;
	bool mirror = false;

	if (m_Direction & 0x80)
		m_Direction &= 0x7F;

	GetAnimDirection(m_Direction, mirror);

	uchar animIndex = obj->AnimIndex;
	m_AnimGroup = GetDieGroupIndex(obj->GetMountAnimation(), obj->UsedLayer);

	y -= (z * 4);

	bool result = TestPixels(obj, x, y, mirror, animIndex);

	IFOR(l, 0, USED_LAYER_COUNT && !result)
	{
		CGameItem *goi = obj->FindLayer(m_UsedLayers[m_Direction][l]);

		if (goi != NULL && goi->AnimID)
			result = TestPixels(goi, x, y, mirror, animIndex, goi->AnimID);
	}

	return result;
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

	if (graphic < MAX_ANIMATIONS_DATA_INDEX_COUNT && m_DataIndex[graphic].Address != NULL)
	{
		int offset = group * 5;
		puchar dataStart = NULL;

		if (m_DataIndex[graphic].Offset == 0xFFFFFFFF) //in verdata
		{
			PVERDATA_HEADER vh = (PVERDATA_HEADER)(m_DataIndex[graphic].Address + (offset * sizeof(VERDATA_HEADER)));

			if (vh->Position != 0 && vh->Position != 0xFFFFFFFF)
				dataStart = (puchar)((uint)g_FileManager.m_VerdataMul.Start + vh->Position);
		}
		else //in original mulls
		{
			PANIM_IDX_BLOCK aidx = (PANIM_IDX_BLOCK)(m_DataIndex[graphic].Address + (offset * sizeof(ANIM_IDX_BLOCK)));

			if (aidx->Position != 0 && aidx->Position != 0xFFFFFFFF)
				dataStart = (puchar)(m_DataIndex[graphic].Offset + aidx->Position);
		}

		if (dataStart != NULL)
		{
			dataStart += sizeof(ushort[256]); //Palette

			int frameCount = *((puint)dataStart);

			result = (frameCount > 0);
		}
	}

	return result;
}
//----------------------------------------------------------------------------------
/*!
Получить индекс картинки трупа
@param [__inout] graphic Индекс картинки
@return 
*/
void CAnimationManager::GetCorpseGraphic(ushort &graphic)
{
	//if (graphic < MAX_ANIMATIONS_DATA_INDEX_COUNT && m_CorpseReplaces[graphic])
	//	graphic = m_CorpseReplaces[graphic];
}
//----------------------------------------------------------------------------------
/*!
Получить индекс анимации
@param [__inout] graphic Индекс каритнки
@return 
*/
void CAnimationManager::GetBodyGraphic(ushort &graphic)
{
	if (graphic < MAX_ANIMATIONS_DATA_INDEX_COUNT)
		graphic = m_DataIndex[graphic].Graphic;
}
//----------------------------------------------------------------------------------
ANIMATION_DIMENSIONS CAnimationManager::GetAnimationDimensions(CGameObject *obj, uchar frameIndex, const uchar &defaultDirection, const uchar &defaultGroup)
{
	ANIMATION_DIMENSIONS result = { 0 };

	uchar dir = defaultDirection;
	uchar animGroup = defaultGroup;
	ushort id = obj->GetMountAnimation();
	bool mirror = false;

	if (dir & 0x80)
		dir &= 0x7F;

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
		id = ((CGameItem*)obj)->GetAnimID();

	if (frameIndex == 0xFF)
		frameIndex = (uchar)obj->AnimIndex;

	CTextureAnimation *anim = GetAnimation(id);

	bool found = false;

	if (anim != NULL)
	{
		CTextureAnimationGroup *group = anim->GetGroup(animGroup);

		if (group != NULL)
		{
			CTextureAnimationDirection *direction = group->GetDirection(dir);

			if (direction != NULL && direction->Address != NULL)
			{
				int fc = direction->FrameCount;

				if (fc > 0 && frameIndex >= fc)
				{
					if (obj->IsCorpse())
						frameIndex = fc - 1;
					else
						frameIndex = 0;
				}

				CTextureAnimationFrame *frame = direction->FindFrame(frameIndex);

				if (frame != NULL)
				{
					result.Width = frame->Width;
					result.Height = frame->Height;
					result.CenterX = frame->CenterX;
					result.CenterY = frame->CenterY;

					found = true;
				}
			}
		}
	}
	
	if (!found)
	{
		ushort graphic = id; // obj->GetMountAnimation();

		if (graphic < MAX_ANIMATIONS_DATA_INDEX_COUNT && m_DataIndex[graphic].Address != 0)
		{
			int offset = animGroup * 5;
			puchar dataStart = NULL;

			if (m_DataIndex[graphic].Offset == 0xFFFFFFFF) //in verdata
			{
				PVERDATA_HEADER vh = (PVERDATA_HEADER)(m_DataIndex[graphic].Address + (offset * sizeof(VERDATA_HEADER)));

				if (vh->Position != 0 && vh->Position != 0xFFFFFFFF)
					dataStart = (puchar)((uint)g_FileManager.m_VerdataMul.Start + vh->Position);
			}
			else //in original mulls
			{
				PANIM_IDX_BLOCK aidx = (PANIM_IDX_BLOCK)(m_DataIndex[graphic].Address + (offset * sizeof(ANIM_IDX_BLOCK)));

				if (aidx->Position != 0 && aidx->Position != 0xFFFFFFFF)
					dataStart = (puchar)(m_DataIndex[graphic].Offset + aidx->Position);
			}

			if (dataStart != NULL)
			{
				dataStart += sizeof(ushort[256]); //Palette

				int frameCount = *((puint)dataStart);

				//if (frameIndex >= frameCount)
				//	frameIndex = 0;

				if (frameCount > 0 && frameIndex >= frameCount)
				{
					if (obj->IsCorpse())
						frameIndex = frameCount - 1;
					else
						frameIndex = 0;
				}

				if (frameIndex < frameCount)
				{
					puint frameOffset = (puint)(dataStart + sizeof(uint));

					pushort p = (pushort)(dataStart + frameOffset[frameIndex]);

					result.CenterX = (short)*p++;
					result.CenterY = (short)*p++;
					result.Width = (short)*p++;
					result.Height = (short)*p++;
				}
			}
		}
	}

	return result;
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
			if (!obj->Dead())
			{
				IFOR(l, 0, USED_LAYER_COUNT)
				{
					goi = obj->FindLayer(m_UsedLayers[layerDir][l]);

					if (goi != NULL && goi->GetAnimID())
						CalculateFrameInformation(info, goi, mirror, animIndex);
				}
			}
			else
			{
				goi = obj->FindLayer(OL_ROBE);

				if (goi != NULL && goi->AnimID)
					CalculateFrameInformation(info, goi, mirror, animIndex);
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

		m_Direction = obj->Layer;
		bool mirror = false;

		if (m_Direction & 0x80)
			m_Direction &= 0x7F;

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

				if (goi != NULL && goi->GetAnimID())
					CalculateFrameInformation(info, goi, mirror, animIndex);
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
void CAnimationManager::DrawEquippedLayers(CGameCharacter* obj, int drawX, int drawY, bool mirror, uchar layerDir, uchar animIndex, int lightOffset)
{
	CGameItem *robe = obj->FindLayer(OL_ROBE);
	CGameItem *goi = {0};
	IFOR(l, 0, USED_LAYER_COUNT)
	{
		goi = obj->FindLayer(m_UsedLayers[layerDir][l]);

		if (goi == NULL || (robe != NULL && IsUnderRobe(goi->GetLayer())))
			continue;
		if (goi->AnimID)
			Draw(goi, drawX, drawY, mirror, animIndex, goi->AnimID);

		if (goi->IsLightSource() && g_GameScreen.UseLight)
			g_GameScreen.AddLight(obj, goi, drawX, drawY - lightOffset);
	}
}
//----------------------------------------------------------------------------------
bool CAnimationManager::IsUnderRobe(int layer)
{
	switch (layer)
	{
	case OL_PANTS:
		return true;
	case OL_SHIRT:
		return true;
	case OL_TORSO:
		return true;
	case OL_BRACELET:
		return true;
	case OL_TUNIC:
		return true;
	case OL_ARMS:
		return true;
	case OL_SKIRT:
		return true;
	case OL_LEGS:
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------------
