/****************************************************************************
**
** GameObject.cpp
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
//---------------------------------------------------------------------------
TGameItem::TGameItem(DWORD serial)
:TGameObject(serial), m_Layer(0), m_AnimID(0), m_ImageID(0), m_UsedLayer(0),
m_Opened(false), m_Dragged(false), m_ObjectFlags(0), ShopItem(NULL)
{
}
//---------------------------------------------------------------------------
TGameItem::~TGameItem()
{
	if (m_Graphic >= 0x4000 && m_Items != NULL)
	{
		TMulti *multi = (TMulti*)m_Items;
		m_Items = NULL;
		delete multi;
	}

	if (ShopItem != NULL)
	{
		delete ShopItem;
		ShopItem = NULL;
	}
	
	if (m_Opened)
	{
		GumpManager->CloseGump(m_Serial, 0, GT_CONTAINER);
		GumpManager->CloseGump(m_Serial, 0, GT_SPELLBOOK);
		GumpManager->CloseGump(m_Serial, 0, GT_MAP);

		TGump *gump = GumpManager->GetGump(m_Serial, 0, GT_BULLETIN_BOARD);
		if (gump != NULL)
			GumpManager->RemoveGump(gump);

		m_Opened = false;
	}

	if (m_Dragged)
	{
		GumpManager->CloseGump(m_Serial, 0, GT_DRAG);

		m_Dragged = false;
	}
}
//---------------------------------------------------------------------------
void TGameItem::Paste(TObjectOnCursor *obj)
{
	if (obj == NULL)
		return;

	m_Items = NULL;
	m_Next = NULL;
	m_Prev = NULL;
	
	m_Serial = obj->GetSerial();
	m_Graphic = obj->GetGraphic();
	m_Color = obj->GetColor();
	m_X = obj->GetX();
	m_Y = obj->GetY();
	m_Z = obj->GetZ();
	m_Count = obj->GetCount();
	m_Layer = obj->GetLayer();
	m_Flags = obj->GetFlags();
	m_NPC = obj->GetNPC();
	m_ImageID = obj->GetImageID();
	m_Container = obj->GetContainer();
	m_UsedLayer = obj->GetUsedLayer();
	m_Opened = false;
	m_ObjectFlags = obj->GetObjectFlags();
	m_AnimID = obj->GetAnimID();
	m_MapIndex = obj->GetMapIndex();
	m_Dragged = false;
	m_Clicked = false;

	SetName(obj->GetName());
}
//---------------------------------------------------------------------------
int TGameItem::Draw(bool &mode, RENDER_LIST_DATA &data, DWORD &ticks)
{
	if (mode)
	{
#if UO_DEBUG_INFO!=0
		g_RenderedObjectsCountInGameWindow++;
#endif

		WORD objGraphic = m_Graphic;
		WORD objColor = m_Color;

		if (Hidden())
			objColor = 0x038A;

		if (IsCorpse()) //Трупик
			AnimationManager->DrawCorpse(this, data.DrawX, data.DrawY, m_Z);
		else
		{
			bool doubleDraw = false;
			bool selMode = false;

			if (g_LastObjectType == SOT_GAME_OBJECT && !Locked() && g_LastSelectedObject == m_Serial)
			{
				objColor = 0x0035;
				selMode = true;
			}

			if (m_Count > 1)
			{
				if (objGraphic == 0x0EED)
				{
					if (m_Count > 5)
						objGraphic = 0x0EEF;
					else
						objGraphic = 0x0EEE;
				}
				else if (IsStackable())
					doubleDraw = true;
			}

			if (doubleDraw)
			{
				int drawX = data.DrawX - 2;
				UO->DrawStaticArt(objGraphic, objColor, drawX, data.DrawY - 5, m_Z, selMode);
				UO->DrawStaticArt(objGraphic, objColor, drawX + 5, data.DrawY, m_Z, selMode);
			}
			else
				UO->DrawStaticArtAnimated(objGraphic, objColor, data.DrawX, data.DrawY, m_Z, selMode);

			if (IsLightSource())
			{
				STATIC_TILES &tile = UO->m_StaticData[objGraphic / 32].Tiles[objGraphic % 32];

				LIGHT_DATA light = { tile.Quality, tile.Hue, X, Y, m_Z, data.DrawX, data.DrawY - (m_Z * 4) };

				if (ConfigManager.ColoredLighting)
					light.Color = UO->GetLightColor(objGraphic);

				GameScreen->AddLight(light);
			}
		}

		int drawX = data.DrawX;
		int drawY = data.DrawY;

		DrawEffects(drawX, drawY, ticks);
	}
	else
	{
		if (IsCorpse()) //Трупик
		{
			if (AnimationManager->CorpsePixelsInXY(this, data.DrawX, data.DrawY, m_Z))
			{
				g_LastObjectType = SOT_GAME_OBJECT;
				g_LastSelectedObject = m_Serial;
				g_SelectedObject = this;
			}
		}
		else
		{
			WORD goGraphic = m_Graphic;

			bool doubleDraw = false;

			if (m_Count > 1)
			{
				if (goGraphic == 0x0EED)
				{
					if (m_Count > 5)
						goGraphic = 0x0EEF;
					else
						goGraphic = 0x0EEE;
				}
				else if (IsStackable())
					doubleDraw = true;
			}

			if (doubleDraw)
			{
				int drawX = data.DrawX - 2;

				if (UO->StaticPixelsInXY(goGraphic, data.DrawX, data.DrawY - 5, m_Z))
				{
					g_LastObjectType = SOT_GAME_OBJECT;
					g_LastSelectedObject = m_Serial;
					g_SelectedObject = this;
				}
				else if (UO->StaticPixelsInXY(goGraphic, data.DrawX + 5, data.DrawY, m_Z))
				{
					g_LastObjectType = SOT_GAME_OBJECT;
					g_LastSelectedObject = m_Serial;
					g_SelectedObject = this;
				}
			}
			else if (UO->StaticPixelsInXYAnimated(goGraphic, data.DrawX, data.DrawY, m_Z))
			{
				g_LastObjectType = SOT_GAME_OBJECT;
				g_LastSelectedObject = m_Serial;
				g_SelectedObject = this;
			}
		}
	}

	return 0;
}
//---------------------------------------------------------------------------
WORD TGameItem::GetMountAnimation()
{
	WORD graphic = m_Graphic;

	if (m_Layer == OL_MOUNT)
	{
		switch (graphic)
		{
			//FWUO
			case 0x2F01:
			{
				graphic = 0xD5; // Polar bear
				break;
			}
			case 0x2F03:
			{
				graphic = 0xF6; // Ridgeback
				break;
			}
			case 0x2F04:
			{
				graphic = 0xF3; // Skeletal mount
				break;
			}
			case 0x2F05:
			{
				graphic = 0x23D; // Genie
				break;
			}
			case 0x2F10:
			{
				graphic = 0xEB; // Kyryn
				break;
			}
			case 0x3001:
			{
				graphic = 0xF4; // Beetle
				break;
			}



			case 16034:
			{
				graphic = 204; //dark brown/horse
				break;
			}
			case 16031:
			{
				graphic = 200; //lightbrown/horse2
				break;
			}
			case 16032:
			{
				graphic = 226; //light grey/horse3
				break;
			}
			case 16033:
			{
				graphic = 228; //greybrown/horse4
				break;
			}
			case 16038:
			{
				graphic = 220; //Llama
				break;
			}
			case 16035:
			{
				graphic = 210; //desert ostard
				break;
			}
			case 16186:
			{
				graphic = 213; //snow bear
				break;
			}
			case 16036:
			{
				graphic = 218; //frenzied ostard (=zostrich)
				break;
			}
			case 16037:
			{
				graphic = 219; //forest ostard
				break;
			}
			case 16039:
			{
				graphic = 177; // Nightmare / Vortex
				break;
			}
			case 16040:
			{
				graphic = 117; // Silver Steed
				break;
			}
			case 16041:
			{
				graphic = 114; // Nightmare
				break;
			}
			case 16042:
			{
				graphic = 115; // Ethereal Horse
				break;
			}
			case 16043:
			{
				graphic = 170; // Ethereal Llama
				break;
			}
			case 16044:
			{
				graphic = 171; // Ethereal Ostard
				break;
			}
			case 16023:
			{
				graphic = 195; // Ethereal Giant Beetle
				break;
			}
			case 16024:
			{
				graphic = 194; // Ethereal Swamp Dragon
				break;
			}
			case 16026:
			{
				graphic = 193; // Ethereal Ridgeback
				break;
			}
			case 16028:
			{
				graphic = 191; // Ethereal Kirin
				break;
			}
			case 16029:
			{
				graphic = 192; // Ethereal Unicorn
				break;
			}
			case 16047:
			{
				graphic = 120; // War Horse (Blood Red)
				break;
			}
			case 16048:
			{
				graphic = 121; // War Horse (Light Green)
				break;
			}
			case 16049:
			{
				graphic = 119; // War Horse (Light Blue)
				break;
			}
			case 16051:
			{
				graphic = 144; // Sea Horse (Medium Blue)
				break;
			}
			case 16053:
			{
				graphic = 116; // Nightmare
				break;
			}
			case 16054:
			{
				graphic = 178; // Nightmare 4
				break;
			}
			case 16045:
			{
				graphic = 132; // Kirin
				break;
			}
			case 16050:
			{
				graphic = 118; // War Horse (Purple)
				break;
			}
			case 16052:
			{
				graphic = 122; // Unicorn
				break;
			}
			case 16055:
			{
				graphic = 179; // Dark Steed
				break;
			}
			case 16056:
			{
				graphic = 187; // Ridgeback
				break;
			}
			case 16058:
			{
				graphic = 188; // Ridgeback, Savage
				break;
			}
			case 16059:
			{
				graphic = 793; // Skeletal Mount
				break;
			}
			case 16060:
			{
				graphic = 791; // Beetle
				break;
			}
			case 16061:
			{
				graphic = 794; // SwampDragon
				break;
			}
			case 16062:
			{
				graphic = 799; // Armored Swamp Dragon
				break;
			}
			default:
				break;
		}
	}
	else if (graphic == 0x2006) //Corpse
		graphic = (WORD)m_Count;

	return graphic;
}
//---------------------------------------------------------------------------
void TGameItem::LoadMulti()
{
	TIndexMulti *index = UO->GetMultiPointer(m_Graphic - 0x4000);
	
	if (index != NULL && index->Address != NULL)
	{
		DWORD address = index->Address;

		int count = index->Count;
		
		int MinX = 0;
		int MinY = 0;
		int MaxX = 0;
		int MaxY = 0;

		int x = X;
		int y = Y;
		char z = Z;

		IFOR(j, 0, count)
		{
			PMULTI_BLOCK pmb = (PMULTI_BLOCK)(address + (j * sizeof(MULTI_BLOCK)));

			if (pmb->Flags)
			{
				TMultiObject *mo = new TMultiObject(pmb->ID + 0x4000, x + pmb->X, y + pmb->Y, z + (char)pmb->Z, pmb->Flags);
				MapManager->AddRender(mo);
				AddMultiObject(mo);
			}

			if (pmb->X < MinX)
				MinX = pmb->X;
			if (pmb->X > MaxX)
				MaxX = pmb->X;
			
			if (pmb->Y < MinY)
				MinY = pmb->Y;
			if (pmb->Y > MaxY)
				MaxY = pmb->Y;
		}

		TMulti *multi = (TMulti*)m_Items;

		if (multi != NULL)
		{
			multi->MinX = MinX;
			multi->MinY = MinY;

			multi->MaxX = MaxX;
			multi->MaxY = MaxY;
		}
	}
}
//---------------------------------------------------------------------------
void TGameItem::AddMultiObject(TMultiObject *obj)
{
	if (m_Items == NULL)
	{
		m_Items = new TMulti(obj->X, obj->Y);
		m_Items->m_Next = NULL;
		m_Items->m_Items = obj;
		obj->m_Next = NULL;
		obj->m_Prev = NULL;
	}
	else
	{
		TMulti *multi = GetMultiAtXY(obj->X, obj->Y);

		if (multi != NULL)
		{
			TMultiObject *multiobj = (TMultiObject*)multi->m_Items;

			while (multiobj != NULL)
			{
				if (obj->Z < multiobj->Z)
				{
					if (multiobj->m_Prev == NULL)
					{
						obj->m_Prev = NULL;
						obj->m_Next = multiobj;
						multiobj->m_Prev = obj;
						multi->m_Items = obj;
					}
					else
					{
						obj->m_Next = multiobj->m_Next;
						multiobj->m_Next = obj;
						obj->m_Prev = multiobj;
					}

					return;
				}

				if (multiobj->m_Next == NULL)
				{
					multiobj->m_Next = obj;
					obj->m_Prev = multiobj;
					obj->m_Next = NULL;

					return;
				}

				multiobj = (TMultiObject*)multiobj->m_Next;
			}

			//Если пришли сюда - что-то пошло не так
		}
		else
		{
			TMulti *newmulti = new TMulti(obj->X, obj->Y);
			newmulti->m_Next = NULL;
			newmulti->m_Items = obj;
			obj->m_Next = NULL;
			obj->m_Prev = NULL;

			multi = (TMulti*)m_Items;

			while (multi != NULL)
			{
				if (multi->m_Next == NULL)
				{
					multi->m_Next = newmulti;
					break;
				}

				multi = (TMulti*)multi->m_Next;
			}
		}
	}
}
//---------------------------------------------------------------------------
TMulti *TGameItem::GetMultiAtXY(short x, short y)
{
	TMulti *multi = (TMulti*)m_Items;

	while (multi != NULL)
	{
		if (multi->X == x && multi->Y == y)
			return multi;

		multi = (TMulti*)multi->m_Next;
	}

	return multi;
}
//---------------------------------------------------------------------------
TGameItem *TGameItem::FindItem(WORD graphic, WORD color)
{
	TGameItem *obj = (TGameItem*)m_Items;
	TGameItem *item = NULL;

	if (color == 0xFFFF) //Поиск по минимальному цвету
	{
		WORD minColor = 0xFFFF;

		while (obj != NULL)
		{
			if (obj->Graphic == graphic)
			{
				if (obj->Color < minColor)
				{
					item = obj;
					minColor = obj->Color;
				}
			}

			if (!obj->Empty())
			{
				TGameItem *found = obj->FindItem(graphic, color);
				
				if (found != NULL && found->Color < minColor)
				{
					item = found;
					minColor = found->Color;
				}
			}

			obj = (TGameItem*)obj->m_Next;
		}
	}
	else //стандартный поиск
	{
		while (obj != NULL)
		{
			if (obj->Graphic == graphic && obj->Color == color)
				item = obj;

			if (!obj->Empty())
			{
				TGameItem *found = obj->FindItem(graphic, color);
				
				if (found != NULL)
					item = found;
			}

			obj = (TGameItem*)obj->m_Next;
		}
	}

	return item;
}
//---------------------------------------------------------------------------
TGameItem *TGameItem::FindLayer(int layer)
{
	TGameItem *item = NULL;

	for (TGameObject *obj = (TGameObject*)m_Items; obj != NULL; obj = (TGameObject*)obj->m_Next)
	{
		if (((TGameItem*)obj)->Layer == layer)
		{
			item = (TGameItem*)obj;
			break;
		}
	}

	return item;
}
//---------------------------------------------------------------------------