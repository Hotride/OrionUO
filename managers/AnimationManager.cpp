/****************************************************************************
**
** AnimationManager.cpp
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
#include "stdafx.h"

TAnimationManager *AnimationManager = NULL;
//----------------------------------------------------------------------------
int TAnimationManager::m_UsedLayers[8][USED_LAYER_COLUT] =
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
//----------------------------------------------------------------------------
TAnimationManager::TAnimationManager()
: m_UsedAnimList(NULL), m_Color(0), m_AnimGroup(0), m_Direction(0),
m_Grayed(false), m_ShadowCount(0), m_Sitting(0)
{
	memset(m_AddressIdx, 0, sizeof(m_AddressIdx));
	memset(m_AddressMul, 0, sizeof(m_AddressMul));
	memset(m_SizeIdx, 0, sizeof(m_SizeIdx));
	memset(m_DataIndex, 0, sizeof(m_DataIndex));
	memset(&m_ShadowList[0], 0, sizeof(m_ShadowList));
	memset(&m_CorpseReplaces[0], 0, sizeof(m_CorpseReplaces));

    LUMA_THRESHOLD = 20.0f;
    ALPHA_SCALE = 10.0f;
    ALPHA_BITS = 4;
    BIT_STEP = (int)(256 / (pow(2, ALPHA_BITS)));
}
//----------------------------------------------------------------------------
TAnimationManager::~TAnimationManager()
{
	ClearUnusedTextures(GetTickCount() + 100000);
}
//----------------------------------------------------------------------------
void TAnimationManager::AddShadow(GLuint texture, int drawX, int drawY, int zBuffer, int width, int height, bool mirror)
{
	if (m_ShadowCount < MAX_SHADOWS_COUNT && !g_GrayedPixels)
	{
		SHADOW_DATA &shadow = m_ShadowList[m_ShadowCount];
		m_ShadowCount++;

		shadow.Texture = texture;
		shadow.DrawX = drawX;
		shadow.DrawY = drawY;
		shadow.ZBuffer = zBuffer;
		shadow.Width = width;
		shadow.Height = height;
		shadow.Mirror = mirror;
	}
}
//----------------------------------------------------------------------------
void TAnimationManager::DrawShadows()
{
	if (m_ShadowCount)
	{
		WORD clr = 0x0386;
		ColorManager->SendColorsToShader(clr);
		glUniform1iARB(ShaderDrawMode, 1);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);

		IFOR(i, 0, m_ShadowCount)
		{
			SHADOW_DATA &shadow = m_ShadowList[i];
			g_ZBuffer = shadow.ZBuffer - 1;
			g_GL.DrawShadow(shadow.Texture, shadow.DrawX, shadow.DrawY, shadow.Width, shadow.Height, shadow.Mirror);
		}

		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
	}
}
//----------------------------------------------------------------------------
void TAnimationManager::Load(PDWORD verdata)
{
	IFOR(i, 0, MAX_ANIMATIONS_DATA_INDEX_COUNT)
	{
		WORD animID = 0;
		DWORD findID = 0;

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
				m_DataIndex[i].Address = (DWORD)aidx;
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
			PVERDATA_HEADER vh = (PVERDATA_HEADER)((DWORD)verdata + 4 + (j * sizeof(VERDATA_HEADER)));

			if (vh->FileID == 0x06) //Anim
			{
				DWORD graphic = vh->BlockID;

				//TPRINT("vh->ID = 0x%02X vh->BlockID = 0x%08X\n", vh->FileID, graphic);
				
				IFOR(i, 0, 0x0800)
				{
					WORD animID = i;
					DWORD findID = 0;

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
						m_DataIndex[i].Address = (DWORD)vh;
						m_DataIndex[i].Offset = 0xFFFFFFFF;
						m_DataIndex[i].Graphic = i; // animID;
						//TPRINT("\tLoad verdata anim ID = 0x%04X\n", animID);
					}
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
void TAnimationManager::InitIndexReplaces(PDWORD verdata)
{
	Load(verdata);

	TTextFileParser newBodyParser("", " \t,{}", "#;//", "");
	TTextFileParser bodyParser(FilePath("Body.def").c_str(), " \t", "#;//", "{}");
	TTextFileParser corpseParser(FilePath("Corpse.def").c_str(), " \t", "#;//", "{}");
	TTextFileParser bodyconvParser(FilePath("Bodyconv.def").c_str(), " \t", "#;//", "");

	while (!bodyconvParser.IsEOF())
	{
		std::vector<std::string> strings = bodyconvParser.ReadTokens();

		if (strings.size() >= 5)
		{
			int anim[4] = { -1, -1, -1, -1 };

			WORD index = atoi(strings[0].c_str());

			IFOR(i, 0, 4)
				anim[i] = atoi(strings[i + 1].c_str());

			if (index >= MAX_ANIMATIONS_DATA_INDEX_COUNT)
				continue;

			int startAnimID = -1;
			int animFile = 1;
			WORD convertedAnimID = 0;
			WORD realAnimID = 0;

			if (anim[0] != -1 && m_AddressIdx[2] != NULL && m_AddressMul[2] != NULL)
			{
				animFile = 2;
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
			else if (anim[1] != -1 && m_AddressIdx[3] != NULL && m_AddressMul[3] != NULL)
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
			else if (anim[2] != -1 && m_AddressIdx[4] != NULL && m_AddressMul[4] != NULL)
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
			else if (anim[3] != -1 && m_AddressIdx[5] != NULL && m_AddressMul[5] != NULL)
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

				if ((DWORD)startAnimID < m_SizeIdx[animFile])
				{
					PANIM_IDX_BLOCK aidx = (PANIM_IDX_BLOCK)(m_AddressIdx[animFile] + (startAnimID));

					if (aidx->Size && aidx->Position != 0xFFFFFFFF && aidx->Size != 0xFFFFFFFF)
					{
						m_DataIndex[index].Address = (DWORD)aidx;
						m_DataIndex[index].Offset = m_AddressMul[animFile];
						m_DataIndex[index].Graphic = realAnimID;// convertedAnimID;
					}
				}
			}
		}
	}

	while (!bodyParser.IsEOF())
	{
		std::vector<std::string> strings = bodyParser.ReadTokens();

		if (strings.size() >= 3)
		{
			WORD index = atoi(strings[0].c_str());

			if (index >= MAX_ANIMATIONS_DATA_INDEX_COUNT)
				continue;

			std::vector<std::string> newBody = newBodyParser.GetTokens(strings[1].c_str());

			int size = (int)newBody.size();

			IFOR(i, 0, size)
			{
				WORD checkIndex = atoi(newBody[i].c_str());

				if (checkIndex >= MAX_ANIMATIONS_DATA_INDEX_COUNT || !m_DataIndex[checkIndex].Offset)
					continue;

				memcpy(&m_DataIndex[index], &m_DataIndex[checkIndex], sizeof(TIndexAnimation));
				m_DataIndex[index].Group = NULL;
				m_DataIndex[index].Color = atoi(strings[2].c_str());

				break;
			}
		}
	}

	while (!corpseParser.IsEOF())
	{
		std::vector<std::string> strings = corpseParser.ReadTokens();

		if (strings.size() >= 3)
		{
			WORD index = atoi(strings[0].c_str());

			if (index >= MAX_ANIMATIONS_DATA_INDEX_COUNT)
				continue;

			std::vector<std::string> newBody = newBodyParser.GetTokens(strings[1].c_str());

			int size = (int)newBody.size();

			IFOR(i, 0, size)
			{
				WORD checkIndex = atoi(newBody[i].c_str());

				if (checkIndex >= MAX_ANIMATIONS_DATA_INDEX_COUNT || !m_DataIndex[checkIndex].Offset)
					continue;

				m_CorpseReplaces[index] = checkIndex;
				//m_DataIndex[index].Color = atoi(strings[2].c_str());

				break;
			}
		}
	}
}
//---------------------------------------------------------------------------
ANIMATION_GROUPS TAnimationManager::GetGroupIndex(WORD id)
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
//---------------------------------------------------------------------------
BYTE TAnimationManager::GetDieGroupIndex(WORD id, bool second)
{
	BYTE group = 0;

	if (id >= 200)
	{
		if (id >= 400)
			group = (BYTE)(second ? PAG_DIE_2 : PAG_DIE_1);
		else
			group = (BYTE)(second ? LAG_DIE_2 : LAG_DIE_1);
	}
	else
		group = (BYTE)(second ? HAG_DIE_2 : HAG_DIE_1);
	
	return group;
}
//----------------------------------------------------------------------------
void TAnimationManager::GetAnimDirection(BYTE &dir, bool &mirror)
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
//----------------------------------------------------------------------------
void TAnimationManager::GetSittingAnimDirection(BYTE &dir, bool &mirror, int &x, int &y)
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
//----------------------------------------------------------------------------
TTextureAnimation *TAnimationManager::GetAnimation(WORD id)
{
	if (id >= MAX_ANIMATIONS_DATA_INDEX_COUNT)
		return NULL;

	TTextureAnimation *anim = m_DataIndex[id].Group;

	if (anim == NULL)
	{
		anim = new TTextureAnimation();
		m_DataIndex[id].Group = anim;

		ADD_LINKED(m_UsedAnimList, m_DataIndex[id]);
	}

	return anim;
}
//----------------------------------------------------------------------------
void TAnimationManager::ClearUnusedTextures(DWORD ticks)
{
	TLinkedList *list = m_UsedAnimList;
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
	}
}
//----------------------------------------------------------------------------

/// <summary>Get luminance value for an RGB value specified</summary>
/// <param name="R">red component</param>
/// <param name="G">green component</param>
/// <param name="B">blue component</param>
/// <returns>luminance value 0.0-255.0</returns>
float TAnimationManager::getLuma(unsigned char &red, unsigned char &green, unsigned char &blue) 
{
	auto min_ = min(min(red, green), blue);
	auto max_ = max(max(red, green), blue);
	auto delta_ = ((float)(min_ + max_)) / 2.0f;

	return delta_;
}

//----------------------------------------------------------------------------
struct Coords
{
	short x;
	short y;
	Coords(short x, short y) { this->x = x; this->y = y; };
};

/// <summary>Sets the best guess alpha for adjucent pixels within
/// specified luma threshold</summary>
/// <param name="processed">список обработанных пикселей</param>
/// <param name="pixels">array of texture pixels in RGBA format</param>
/// <param name="width">texture width</param>
/// <param name="height">texture height</param>
/// <param name="x">x координата пикселя</param>
/// <param name="y">y координата пикселя</param>
/// <param name="alpha_scale">коэффицент маштабировования прозрачности</param>
/// <param name="luma_threshold">пороговое значение яркости, 
/// с начала которого пиксели будут считаться смежными</param>
void TAnimationManager::setAlphaAt(std::vector<bool> &processed, PDWORD pixels, short &width, short &height, int &x, int &y, float &alpha_scale, float &luma_threshold)
{
	std::deque<Coords> stack;
	stack.push_back(Coords(x, y));

	while (stack.size())
	{
		Coords &coords = stack.back();
		auto x = coords.x;
		auto y = coords.y;		

		stack.pop_back();

		if (x < 0 || y < 0 || x >= width || y >= height)
			continue;

		auto idx = width * y + x;		

		auto color = pixels[idx];

		BYTE alpha = (color >> 24) & 0xff;

		if (/*alpha != 255 ||*/ processed[idx]) {
			//processed[idx] = true;
			continue;
		}

		BYTE blue = (color >> 16) & 0xff;
		BYTE green = (color >> 8) & 0xff;
		BYTE red = color & 0xff;

		//яркость ч/б пикселя
		auto luma = getLuma(red, green, blue);

		if (luma > luma_threshold) {
			processed[idx] = true;
			continue;
		}

		//высчитываем новую прозрачность из учета яркости
		alpha = (((int)(alpha_scale * luma)) / BIT_STEP) * BIT_STEP;

		//выставляем новую альфу для спрайта (собираем пиксель)
		pixels[idx] = (alpha << 24) | (blue << 16) | (green << 8) | red;
		processed[idx] = true;

		//обрабатываем соседние пиксели
		stack.push_back(Coords(x + 1, y));
		stack.push_back(Coords(x - 1, y));
		stack.push_back(Coords(x, y + 1));
		stack.push_back(Coords(x, y - 1));		
	}
 
}                       

//----------------------------------------------------------------------------

/// <summary>Process each texture pixel with pre-calculated alpha value
/// from its luminance value and threshold</summary>
/// <param name="pixels">array of texture pixels in RGBA format</param>
/// <param name="width">texture width</param>
/// <param name="height">texture height</param>
void TAnimationManager::EstimateImageCornerAlpha(PDWORD pixels, short &width, short &height, float alpha_scale, float luma_threshold)
{
	auto pixels_count = width * height;

	std::vector<bool> processed(pixels_count);
	//processed.resize(pixels_count);
	auto test = processed.size();
	std::fill(processed.begin(), processed.end(), false);
	test = processed.size();
		
	IFOR(y, 0, height)
	{
		auto row_idx = width * y;

		IFOR(x, 0, width)        
		{
			auto idx = row_idx + x;

            DWORD color = pixels[idx];
            
			BYTE alpha = (color >> 24) & 0xff;
			BYTE blue = (color >> 16) & 0xff;
			BYTE green = (color >> 8) & 0xff;
			BYTE red = color & 0xff;

			if( (red!=0) || (blue!=0) || (green!=0) || processed[idx])
				continue;					

			setAlphaAt(processed, pixels, width, height, x, y, alpha_scale, luma_threshold);
        }        
	}
}
//----------------------------------------------------------------------------
bool TAnimationManager::ExecuteDirectionGroup(TTextureAnimationDirection *direction, WORD &id, int &offset)
{
	direction->Address = 0;

	if (id > MAX_ANIMATIONS_DATA_INDEX_COUNT || m_DataIndex[id].Address == NULL)
		return false;

	PBYTE dataStart = NULL;

	if (m_DataIndex[id].Offset == 0xFFFFFFFF) //in verdata
	{
		PVERDATA_HEADER vh = (PVERDATA_HEADER)(m_DataIndex[id].Address + (offset * sizeof(VERDATA_HEADER)));
		direction->Address = (DWORD)FileManager.VerdataMul.Address + vh->Position;
		
		dataStart = (PBYTE)direction->Address;
	}
	else //in original mulls
	{
		PANIM_IDX_BLOCK aidx = (PANIM_IDX_BLOCK)(m_DataIndex[id].Address + (offset * sizeof(ANIM_IDX_BLOCK)));
	
		if (aidx->Size && aidx->Position != 0xFFFFFFFF && aidx->Size != 0xFFFFFFFF)
			direction->Address = (DWORD)aidx;

		dataStart = (PBYTE)(m_DataIndex[id].Offset + aidx->Position);
	}

	if (!direction->Address)
		return false;

	PBYTE ptr = dataStart;

	dataStart += sizeof(WORD[256]); //Palette

	PWORD palette = (PWORD)ptr;
	ptr = dataStart;

	int frameCount = *((PDWORD)ptr);
	direction->FrameCount = frameCount;
	ptr += sizeof(DWORD);

	PDWORD FrameOffset = (PDWORD)ptr;
	ptr += (frameCount * sizeof(DWORD));

	WORD color = m_DataIndex[id].Color;

	IFOR(i, 0, frameCount)
	{
		TTextureAnimationFrame *frame = direction->GetFrame(i);

		if (frame->Texture != 0)
			continue;

		PWORD p = (PWORD)((DWORD)dataStart + FrameOffset[i]);

		short imageCenterX = *p;
		frame->CenterX = imageCenterX;
		p++;

		short imageCenterY = *p;
		frame->CenterY = imageCenterY;
		p++;

		short imageWidth = *p;
		frame->Width = imageWidth;
		p++;

		short imageHeight = *p;
		frame->Height = imageHeight;
		p++;

		int x = 0;
		int y = 0;

		int blocksize = imageWidth * imageHeight;

		PWORD pData = new WORD[blocksize];
		memset(&pData[0], 0, blocksize * 2);

		WORD prevLineNum = 0xFF;

		while (true)
		{
			WORD rowHeader = *p;
			p++;

			WORD rowOfs = *p;
			p++;

			if (rowHeader == 0x7FFF || rowOfs == 0x7FFF)
				break;

			WORD runLength = rowHeader & 0xFFF;
			WORD lineNum = rowHeader >> 12;

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

				PBYTE b = (PBYTE)p;

				IFOR(j, 0, runLength)
				{
					WORD val = palette[*b];

					if (color)
						val = ColorManager->GetColor16(val, color);

					WORD a = val ? 0x8000 : 0;
					int block = y * imageWidth + (x + j);
					pData[block] = a | val;

					b++;
				}

				p = (PWORD)b;

			}
		}

        //EstimateImageCornerAlpha( pData, imageWidth, imageHeight );

		g_GL.BindTexture16(frame->Texture, imageWidth, imageHeight, pData);

		delete pData;
	}

	return true;
}
//----------------------------------------------------------------------------
bool TAnimationManager::TestImagePixels(TTextureAnimationDirection *direction, BYTE &frame, WORD &id, int &checkX, int &checkY)
{
	PBYTE dataStart = NULL;

	if (m_DataIndex[id].Offset == 0xFFFFFFFF)
		dataStart = (PBYTE)direction->Address;
	else
	{
		PANIM_IDX_BLOCK aidx = (PANIM_IDX_BLOCK)direction->Address;
	
		dataStart = (PBYTE)(m_DataIndex[id].Offset + aidx->Position);
	}

	PBYTE ptr = dataStart;

	PWORD palette = (PWORD)ptr;

	dataStart += sizeof(WORD[256]); //Palette
	ptr = dataStart;

	int frameCount = *((PDWORD)ptr);
	ptr += sizeof(DWORD);

	PDWORD frameOffset = (PDWORD)ptr;
	ptr += (frameCount * sizeof(DWORD));

	PWORD data = (PWORD)((DWORD)dataStart + frameOffset[frame]);

	short imageCenterX = (short)*data++;
	short imageCenterY = (short)*data++;
	short imageWidth = (short)*data++;
	short imageHeight = (short)*data++;

	PWORD p = (PWORD)data;

	int x = 0;
	int y = 0;

	WORD prevLineNum = 0xFF;

	while (true)
	{
		WORD rowHeader = *p;
		p++;

		WORD rowOfs = *p;
		p++;

		if (rowHeader == 0x7FFF || rowOfs == 0x7FFF)
			break;

		WORD runLength = rowHeader & 0xFFF;
		WORD lineNum = rowHeader >> 12;

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

			PBYTE b = (PBYTE)p;

			IFOR(j, 0, runLength)
			{
				if ((x + j) == checkX && y == checkY)
					return (palette[*b] != 0);

				b++;
			}

			p = (PWORD) b;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
bool TAnimationManager::TestPixels(TGameObject *obj, int x, int y, bool &mirror, BYTE &frameIndex, WORD id)
{
	if (obj == NULL)
		return false;

	if (!id)
		id = obj->GetMountAnimation();
	
	TTextureAnimation *anim = GetAnimation(id);

	if (anim == NULL)
		return false;

	TTextureAnimationGroup *group = anim->GetGroup(m_AnimGroup);
	TTextureAnimationDirection *direction = group->GetDirection(m_Direction);

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

	TTextureAnimationFrame *frame = direction->GetFrame(frameIndex);

	if (frame != NULL)
	{
		y -= (frame->Height + frame->CenterY + 3);
		x -= frame->CenterX;

		x = g_MouseX - x;
		y = g_MouseY - y;

		if (mirror)
			x = frame->Width - x;

		if (x > 0 && x < frame->Width && y > 0 && y < frame->Height)
			return TestImagePixels(direction, frameIndex, id, x, y);
	}

	return false;
}
//----------------------------------------------------------------------------
TTextureAnimationFrame *TAnimationManager::GetFrame(TGameObject *obj, BYTE &frameIndex, WORD id)
{
	TTextureAnimationFrame *frame = NULL;
	if (obj != NULL)
	{
		if (!id)
			id = obj->GetMountAnimation();

		if (id < MAX_ANIMATIONS_DATA_INDEX_COUNT)
		{
			TTextureAnimation *anim = m_DataIndex[id].Group;

			if (anim != NULL)
			{
				TTextureAnimationGroup *group = anim->GetGroup(m_AnimGroup);
				TTextureAnimationDirection *direction = group->GetDirection(m_Direction);

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
//----------------------------------------------------------------------------
void TAnimationManager::Draw(TGameObject *obj, int x, int y, bool &mirror, BYTE &frameIndex, int id)
{
	if (obj == NULL)
		return;

	bool partialHue = false;
	bool isShadow = (id >= 0x10000);

	if (isShadow)
		id -= 0x10000;

	if (!id)
	{
		partialHue = obj->IsHuman();
		id = obj->GetMountAnimation();
	}

	if (id >= MAX_ANIMATIONS_DATA_INDEX_COUNT)
		return;

	TTextureAnimation *anim = m_DataIndex[id].Group;

	if (anim == NULL)
	{
		anim = new TTextureAnimation();
		m_DataIndex[id].Group = anim;
	}

	TTextureAnimationGroup *group = anim->GetGroup(m_AnimGroup);
	TTextureAnimationDirection *direction = group->GetDirection(m_Direction);

	if (direction->Address == 0)
	{
		int offset = (m_AnimGroup * 5) + m_Direction;

		WORD wID = id;
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

	TTextureAnimationFrame *frame = direction->GetFrame(frameIndex);
	
	if (frame->Texture == 0)
	{
		int offset = (m_AnimGroup * 5) + m_Direction;
		
		WORD wID = id;
		if (!ExecuteDirectionGroup(direction, wID, offset))
			return;
	}
	
	GLuint texture = frame->Texture;

	if (texture != 0)
	{
		if (id == 0x23D) //FWUO genie
			y += 40;
	
		y -= (frame->Height + frame->CenterY + 3);

		if (mirror)
			x -= (frame->Width - frame->CenterX);
		else
			x -= frame->CenterX;

		glEnable(GL_DEPTH_TEST);

		if (isShadow)
			AddShadow(texture, x, y, g_ZBuffer, frame->Width, frame->Height, mirror);
		else
		{
			WORD color = m_Color;

			if (color)
				partialHue = false;
			else
				color = obj->Color;

			int drawMode = (!g_GrayedPixels && color);
			bool spectralColor = (color > SPECTRAL_COLOR);
			
			if (spectralColor)
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

				ColorManager->SendColorsToShader(color);
			}

			glUniform1iARB(ShaderDrawMode, drawMode);

			if (m_Sitting)
				g_GL.DrawSitting(texture, x, y, frame->Width, frame->Height, mirror);
			else
				g_GL.Draw(texture, x, y, frame->Width, frame->Height, mirror);

			glDisable(GL_BLEND);
		}

		glDisable(GL_DEPTH_TEST);
	}
}
//----------------------------------------------------------------------------
void TAnimationManager::FixSittingDirection(BYTE &layerDirection, bool &mirror, int &x, int &y)
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

	if (mirror)
		y += data.MirrorOffsetY;
	else
		y += data.OffsetY;
}
//----------------------------------------------------------------------------
void TAnimationManager::DrawCharacter(TGameCharacter *obj, int x, int y, int z)
{
	WORD targetColor = 0;
	bool needHPLine = false;
	DWORD serial = obj->Serial;
	m_Grayed = g_GrayedPixels;

	if (obj->Hidden())
		m_Color = 0x038A;
	else if (g_StatusbarUnderMouse == serial)
		m_Color = ConfigManager.GetColorByNotoriety(obj->Notoriety);
	else
		m_Color = 0;

	bool isAttack = (serial == g_LastAttackObject);

	if (!obj->IsPlayer() && (isAttack || serial == g_LastTargetObject))
	{
		targetColor = ConfigManager.GetColorByNotoriety(obj->Notoriety);
		needHPLine = (serial != NewTargetSystem.Serial);

		if (isAttack)
			m_Color = targetColor;
	}
	
	m_Direction = 0;
	obj->UpdateAnimationInfo(m_Direction);

	bool mirror = false;
	BYTE layerDir = m_Direction;
	
	GetAnimDirection(m_Direction, mirror);

	BYTE animIndex = obj->AnimIndex;
	BYTE animGroup = obj->GetAnimationGroup();

	TGameItem *goi = obj->FindLayer(OL_MOUNT);
	
	int lightOffset = 20;
	int drawX = (int)(x + obj->OffsetX);
	int drawY = (int)(y + obj->OffsetY) - (z * 4) - obj->OffsetZ;
	
	if (goi != NULL) //Draw mount
	{
		m_Sitting = 0;
		lightOffset += 20;
		WORD mountID = goi->GetMountAnimation();

		m_AnimGroup = obj->GetAnimationGroup(mountID);
		
		Draw(goi, drawX, drawY, mirror, animIndex, mountID + 0x10000);
		Draw(goi, drawX, drawY, mirror, animIndex, mountID);

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
		}
	}
	
	m_AnimGroup = animGroup;
	
	if (!m_Sitting)
		Draw(obj, drawX, drawY, mirror, animIndex, 0x10000);

	Draw(obj, drawX, drawY, mirror, animIndex); //Draw character

	if (obj->IsHuman()) //Draw layred objects
	{
		if (!obj->Dead())
		{
			IFOR(l, 0, USED_LAYER_COLUT)
			{
				goi = obj->FindLayer(m_UsedLayers[layerDir][l]);

				if (goi != NULL)
				{
					if (goi->AnimID)
						Draw(goi, drawX, drawY, mirror, animIndex, goi->AnimID);

					if (goi->IsLightSource() && GameScreen->UseLight)
						GameScreen->AddLight(obj, goi, drawX, drawY - lightOffset);
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
	
	if (!ConfigManager.DisableNewTargetSystem && NewTargetSystem.Serial == obj->Serial)
	{
		WORD id = obj->GetMountAnimation();

		if (id < MAX_ANIMATIONS_DATA_INDEX_COUNT)
		{
			TTextureAnimation *anim = m_DataIndex[id].Group;

			if (anim != NULL)
			{
				TTextureAnimationGroup *group = anim->GetGroup(m_AnimGroup);
				TTextureAnimationDirection *direction = group->GetDirection(m_Direction);

				if (direction->Address != 0)
				{
					TTextureAnimationFrame *frame = direction->GetFrame(0);

					int frameWidth = 20;
					int frameHeight = 20;

					if (frame != NULL)
					{
						frameWidth = frame->Width;
						frameHeight = frame->Height;
					}

					if (frameWidth >= 80)
					{
						NewTargetSystem.GumpTop = 0x756D;
						NewTargetSystem.GumpBottom = 0x756A;
					}
					else if (frameWidth >= 40)
					{
						NewTargetSystem.GumpTop = 0x756E;
						NewTargetSystem.GumpBottom = 0x756B;
					}
					else
					{
						NewTargetSystem.GumpTop = 0x756F;
						NewTargetSystem.GumpBottom = 0x756C;
					}

					switch (obj->Notoriety)
					{
						case NT_INNOCENT:
						{
							NewTargetSystem.ColorGump = 0x7570;
							break;
						}
						case NT_FRIENDLY:
						{
							NewTargetSystem.ColorGump = 0x7571;
							break;
						}
						case NT_SOMEONE_GRAY:
						case NT_CRIMINAL:
						{
							NewTargetSystem.ColorGump = 0x7572;
							break;
						}
						case NT_ENEMY:
						{
							NewTargetSystem.ColorGump = 0x7573;
							break;
						}
						case NT_MURDERER:
						{
							NewTargetSystem.ColorGump = 0x7576;
							break;
						}
						case NT_INVULNERABLE:
						{
							NewTargetSystem.ColorGump = 0x7575;
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
					
					NewTargetSystem.Hits = per;
					NewTargetSystem.X = drawX;
					NewTargetSystem.TopY = drawY - frameHeight - 8;
					NewTargetSystem.BottomY = drawY + 7;
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
			AttackTargetGump.X = drawX - 20;
			AttackTargetGump.Y = drawY;
			AttackTargetGump.Color = targetColor;
			AttackTargetGump.Hits = per;
		}
		else
		{
			TargetGump.X = drawX - 20;
			TargetGump.Y = drawY;
			TargetGump.Color = targetColor;
			TargetGump.Hits = per;
		}
	}
}
//----------------------------------------------------------------------------
bool TAnimationManager::CharacterPixelsInXY(TGameCharacter *obj, int x, int y, int z)
{
	m_Sitting = obj->IsSitting();
	m_Direction = 0;
	obj->UpdateAnimationInfo(m_Direction);

	bool mirror = false;
	BYTE layerDir = m_Direction;
	
	GetAnimDirection(m_Direction, mirror);

	BYTE animIndex = obj->AnimIndex;
	BYTE animGroup = obj->GetAnimationGroup();
	
	TGameItem *goi = obj->FindLayer(OL_MOUNT);
	
	int drawX = (int)(x - obj->OffsetX);
	int drawY = (int)(y - obj->OffsetY) - (z * 4) - obj->OffsetZ;
	
	if (goi != NULL) //Check mount
	{
		WORD mountID = goi->GetMountAnimation();

		m_AnimGroup = obj->GetAnimationGroup(mountID);

		if (TestPixels(goi, drawX - 3, drawY, mirror, animIndex, mountID))
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
		IFOR(l, 0, USED_LAYER_COLUT && !result)
		{
			goi = obj->FindLayer(m_UsedLayers[layerDir][l]);

			if (goi != NULL && goi->GetAnimID())
				result = TestPixels(goi, drawX, drawY, mirror, animIndex, goi->GetAnimID());
		}
	}

	return result;
}
//----------------------------------------------------------------------------
void TAnimationManager::DrawCorpse(TGameItem *obj, int x, int y, int z)
{
	m_Sitting = 0;
	m_Direction = obj->Layer;

	if (m_Direction & 0x80)
		m_Direction &= 0x7F;

	bool mirror = false;

	GetAnimDirection(m_Direction, mirror);

	m_Grayed = g_GrayedPixels;

	if (obj->Hidden())
		m_Color = 0x038A;
	else
		m_Color = 0;

	BYTE animIndex = obj->AnimIndex;
	m_AnimGroup = GetDieGroupIndex(obj->GetMountAnimation(), obj->UsedLayer);
	
	y -= (z * 4);

	Draw(obj, x, y, mirror, animIndex); //Draw animation

	IFOR(l, 0, USED_LAYER_COLUT)
	{
		TGameItem *goi = obj->FindLayer(m_UsedLayers[m_Direction][l]);
		
		if (goi != NULL)
		{
			if (goi->AnimID)
				Draw(goi, x, y, mirror, animIndex, goi->AnimID);

			if (goi->IsLightSource() && GameScreen->UseLight)
				GameScreen->AddLight(obj, goi, x, y - 20);
		}
	}
}
//----------------------------------------------------------------------------
bool TAnimationManager::CorpsePixelsInXY(TGameItem *obj, int x, int y, int z)
{
	m_Sitting = 0;
	m_Direction = obj->Layer;
	bool mirror = false;

	if (m_Direction & 0x80)
		m_Direction &= 0x7F;

	GetAnimDirection(m_Direction, mirror);

	BYTE animIndex = obj->AnimIndex;
	m_AnimGroup = GetDieGroupIndex(obj->GetMountAnimation(), obj->UsedLayer);

	y -= (z * 4);

	bool result = TestPixels(obj, x, y, mirror, animIndex);

	IFOR(l, 0, USED_LAYER_COLUT && !result)
	{
		TGameItem *goi = obj->FindLayer(m_UsedLayers[m_Direction][l]);

		if (goi != NULL && goi->GetAnimID())
			result = TestPixels(goi, x, y, mirror, animIndex, goi->GetAnimID());
	}

	return result;
}
//----------------------------------------------------------------------------
bool TAnimationManager::AnimationExists(WORD &graphic, BYTE group)
{
	bool result = false;

	if (graphic < MAX_ANIMATIONS_DATA_INDEX_COUNT)
	{
		int offset = group * 5;
		PBYTE dataStart = NULL;

		if (m_DataIndex[graphic].Offset == 0xFFFFFFFF) //in verdata
		{
			PVERDATA_HEADER vh = (PVERDATA_HEADER)(m_DataIndex[graphic].Address + (offset * sizeof(VERDATA_HEADER)));
			dataStart = (PBYTE)((DWORD)FileManager.VerdataMul.Address + vh->Position);
		}
		else //in original mulls
		{
			PANIM_IDX_BLOCK aidx = (PANIM_IDX_BLOCK)(m_DataIndex[graphic].Address + (offset * sizeof(ANIM_IDX_BLOCK)));

			dataStart = (PBYTE)(m_DataIndex[graphic].Offset + aidx->Position);
		}

		result = (dataStart != NULL);
	}

	return result;
}
//----------------------------------------------------------------------------
void TAnimationManager::GetCorpseGraphic(WORD &graphic)
{
	if (graphic < MAX_ANIMATIONS_DATA_INDEX_COUNT && m_CorpseReplaces[graphic])
		graphic = m_CorpseReplaces[graphic];
}
//----------------------------------------------------------------------------
void TAnimationManager::GetBodyGraphic(WORD &graphic)
{
	if (graphic < MAX_ANIMATIONS_DATA_INDEX_COUNT)
		graphic = m_DataIndex[graphic].Graphic;
}
//----------------------------------------------------------------------------