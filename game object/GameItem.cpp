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
m_Opened(false), m_Dragged(false), ShopItem(NULL)
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
	
	m_Serial = obj->Serial;
	m_Graphic = obj->Graphic;
	m_Color = obj->Color;
	m_X = obj->X;
	m_Y = obj->Y;
	m_Z = obj->Z;
	m_Count = obj->Count;
	m_Layer = obj->Layer;
	m_Flags = obj->Flags;
	m_NPC = obj->NPC;
	m_ImageID = obj->ImageID;
	m_Container = obj->Container;
	m_UsedLayer = obj->UsedLayer;
	m_Opened = false;
	m_AnimID = obj->AnimID;
	m_MapIndex = obj->MapIndex;
	m_Dragged = false;
	m_Clicked = false;

	SetName(obj->GetName());
	OnGraphicChange();
}
//---------------------------------------------------------------------------
void TGameItem::OnGraphicChange(int direction)
{
	if (m_Graphic < 0x4000)
	{
		if (IsCorpse())
		{
			if (World->FindWorldCorpseOwner(m_Serial))
				m_AnimIndex = 0;
			else
				m_AnimIndex = 99;

			if (direction & 0x80)
			{
				m_UsedLayer = 1;
				direction &= 0x7F;
			}
			else
				m_UsedLayer = 0;

			m_Layer = direction;

			m_RenderQueueIndex = 6;
		}
		else
		{
			m_TiledataPtr = &UO->m_StaticData[m_Graphic / 32].Tiles[m_Graphic % 32];
			STATIC_TILES &tile = *m_TiledataPtr;

			if (IsWearable() || m_Graphic == 0x0A28)
			{
				m_ImageID = tile.AnimID + 0xC350;
				m_AnimID = tile.AnimID;
				
				bool partialHue = IsPartialHue();

				UO->ExecuteGump(tile.AnimID + MALE_GUMP_OFFSET, partialHue);
				UO->ExecuteGump(tile.AnimID + FEMALE_GUMP_OFFSET, partialHue);

				m_UsedLayer = tile.Quality;
			}
			else if (m_Layer == OL_MOUNT)
			{
				m_AnimID = tile.AnimID;
				m_UsedLayer = tile.Quality;
			}

			if (IsBackground())
				m_RenderQueueIndex = 3 - (int)IsSurface();
			else if (IsSurface())
				m_RenderQueueIndex = 4;
			else
				m_RenderQueueIndex = 6;

			m_RenderQueueIndex++;

			UO->ExecuteStaticArt(m_Graphic);
		}
	}
	else if (m_Items == NULL)
		LoadMulti();
}
//---------------------------------------------------------------------------
int TGameItem::Draw(bool &mode, int &drawX, int &drawY, DWORD &ticks)
{
	if (m_Container == 0xFFFFFFFF && m_Graphic < 0x4000)
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
				AnimationManager->DrawCorpse(this, drawX, drawY, m_Z);
			else
			{
				bool doubleDraw = false;
				bool selMode = false;
				objGraphic = GetDrawGraphic(doubleDraw);

				if (g_LastObjectType == SOT_GAME_OBJECT && !Locked() && g_LastSelectedObject == m_Serial)
				{
					objColor = 0x0035;
					selMode = true;
				}

				if (IsSurface() || (IsBackground() && IsUnknown2()))
					;// glEnable(GL_DEPTH_TEST);

				if (doubleDraw)
				{
					drawX -= 2;
					UO->DrawStaticArt(objGraphic, objColor, drawX, drawY - 5, m_Z, selMode);
					UO->DrawStaticArt(objGraphic, objColor, drawX + 5, drawY, m_Z, selMode);
				}
				else
					UO->DrawStaticArtAnimated(objGraphic, objColor, drawX, drawY, m_Z, selMode);

				glDisable(GL_DEPTH_TEST);

				if (IsLightSource() && GameScreen->UseLight)
					GameScreen->AddLight(this, this, drawX, drawY - (m_Z * 4));
			}

			if (!ConfigManager.DisableNewTargetSystem && NewTargetSystem.Serial == m_Serial && !Locked())
			{
				POINT p = { 20, 20 };
				UO->GetArtDimension(objGraphic + 0x4000, p);

				if (p.x >= 80)
				{
					NewTargetSystem.GumpTop = 0x756D;
					NewTargetSystem.GumpBottom = 0x756A;
				}
				else if (p.x >= 40)
				{
					NewTargetSystem.GumpTop = 0x756E;
					NewTargetSystem.GumpBottom = 0x756B;
				}
				else
				{
					NewTargetSystem.GumpTop = 0x756F;
					NewTargetSystem.GumpBottom = 0x756C;
				}

				NewTargetSystem.ColorGump = 0x7570;

				NewTargetSystem.Hits = 0;
				NewTargetSystem.X = drawX;
				NewTargetSystem.TopY = drawY - (m_Z * 4) - p.y - 8;
				NewTargetSystem.BottomY = drawY - (m_Z * 4) + 7;
			}

			DrawEffects(drawX, drawY, ticks);
		}
		else
		{
			if (IsCorpse()) //Трупик
			{
				if (AnimationManager->CorpsePixelsInXY(this, drawX, drawY, m_Z))
				{
					g_LastObjectType = SOT_GAME_OBJECT;
					g_LastSelectedObject = m_Serial;
					g_SelectedObject = this;
				}
			}
			else
			{
				bool doubleDraw = false;
				WORD goGraphic = GetDrawGraphic(doubleDraw);

				if (doubleDraw)
				{
					drawX -= 2;

					if (UO->StaticPixelsInXY(goGraphic, drawX, drawY - 5, m_Z))
					{
						g_LastObjectType = SOT_GAME_OBJECT;
						g_LastSelectedObject = m_Serial;
						g_SelectedObject = this;
					}
					else if (UO->StaticPixelsInXY(goGraphic, drawX + 5, drawY, m_Z))
					{
						g_LastObjectType = SOT_GAME_OBJECT;
						g_LastSelectedObject = m_Serial;
						g_SelectedObject = this;
					}
				}
				else if (UO->StaticPixelsInXYAnimated(goGraphic, drawX, drawY, m_Z))
				{
					g_LastObjectType = SOT_GAME_OBJECT;
					g_LastSelectedObject = m_Serial;
					g_SelectedObject = this;
				}
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
	{
		graphic = (WORD)m_Count;
		AnimationManager->GetCorpseGraphic(graphic);
	}

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
		
		int minX = 0;
		int minY = 0;
		int maxX = 0;
		int maxY = 0;

		IFOR(j, 0, count)
		{
			PMULTI_BLOCK pmb = (PMULTI_BLOCK)(address + (j * sizeof(MULTI_BLOCK)));

			if (pmb->Flags)
			{
				TMultiObject *mo = new TMultiObject(pmb->ID, m_X + pmb->X, m_Y + pmb->Y, m_Z + (char)pmb->Z, pmb->Flags);
				MapManager->AddRender(mo);
				AddMultiObject(mo);
			}

			if (pmb->X < minX)
				minX = pmb->X;
			if (pmb->X > maxX)
				maxX = pmb->X;
			
			if (pmb->Y < minY)
				minY = pmb->Y;
			if (pmb->Y > maxY)
				maxY = pmb->Y;
		}

		TMulti *multi = (TMulti*)m_Items;

		if (multi != NULL)
		{
			multi->MinX = minX;
			multi->MinY = minY;

			multi->MaxX = maxX;
			multi->MaxY = maxY;
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