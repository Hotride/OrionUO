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
/*!
Вставка данных из объекта в руке (на курсоре)
@param [__in] obj Ссылка на объект на курсоре
@return 
*/
void TGameItem::Paste( __in TObjectOnCursor *obj)
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
/*!
Событие изменения картинки объекта
@param [__in_opt] direction Направление предмета (для трупов)
@return 
*/
void TGameItem::OnGraphicChange( __in_opt int direction)
{
	if (m_Graphic < 0x4000)
	{
		if (IsCorpse())
		{
			m_AnimIndex = 99;

			for (CORPSE_LIST_MAP::iterator i = g_CorpseSerialList.begin(); i != g_CorpseSerialList.end(); i++)
			{
				if (i->first == m_Serial)
				{
					g_CorpseSerialList.erase(i);
					m_AnimIndex = 0;
					break;
				}
			}

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
			m_TiledataPtr = &Orion->m_StaticData[m_Graphic / 32].Tiles[m_Graphic % 32];
			STATIC_TILES &tile = *m_TiledataPtr;

			if (ToLowerA(tile.Name) == "nodraw")
				m_Graphic = 1;

			if (IsWearable() || m_Graphic == 0x0A28)
			{
				m_ImageID = tile.AnimID + 0xC350;
				m_AnimID = tile.AnimID;
				
				bool partialHue = IsPartialHue();

				Orion->ExecuteGump(tile.AnimID + MALE_GUMP_OFFSET, partialHue);
				Orion->ExecuteGump(tile.AnimID + FEMALE_GUMP_OFFSET, partialHue);

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
			else if (IsAnimated())
				m_RenderQueueIndex = 6;
			else
				m_RenderQueueIndex = 5;

			m_RenderQueueIndex++;

			Orion->ExecuteStaticArt(m_Graphic);
		}
	}
	else if (m_Items == NULL)
		LoadMulti();
}
//---------------------------------------------------------------------------
/*!
Отрисовать предмет
@param [__in] mode Режим рисования. true - рисование, false - выбор объектов
@param [__in] drawX Экранная координата X объекта
@param [__in] drawY Экранная координата Y объекта
@param [__in] ticks Таймер рендера
@return При выборе объектов возвращает выбранный элемент
*/
int TGameItem::Draw(__in bool &mode, __in int &drawX, __in int &drawY, __in DWORD &ticks)
{
	if (m_Container == 0xFFFFFFFF && m_Graphic < 0x4000 && m_Graphic != 1)
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

#if UO_DEPTH_TEST == 1
				//if (IsSurface() || (IsBackground() && IsUnknown2()))
					// glEnable(GL_DEPTH_TEST);
#endif

				if (doubleDraw)
				{
					Orion->DrawStaticArt(objGraphic, objColor, drawX - 2, drawY - 5, m_Z, selMode);
					Orion->DrawStaticArt(objGraphic, objColor, drawX + 3, drawY, m_Z, selMode);
				}
				else
					Orion->DrawStaticArtAnimated(objGraphic, objColor, drawX, drawY, m_Z, selMode);

#if UO_DEPTH_TEST == 1
				//glDisable(GL_DEPTH_TEST);
#endif

				if (IsLightSource() && GameScreen->UseLight)
					GameScreen->AddLight(this, this, drawX, drawY - (m_Z * 4));
			}

			if (!ConfigManager.DisableNewTargetSystem && NewTargetSystem.Serial == m_Serial && !Locked())
			{
				POINT p = { 20, 20 };
				Orion->GetArtDimension(objGraphic + 0x4000, p);

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
					if (Orion->StaticPixelsInXY(goGraphic, drawX - 2, drawY - 5, m_Z))
					{
						g_LastObjectType = SOT_GAME_OBJECT;
						g_LastSelectedObject = m_Serial;
						g_SelectedObject = this;
					}
					else if (Orion->StaticPixelsInXY(goGraphic, drawX + 3, drawY, m_Z))
					{
						g_LastObjectType = SOT_GAME_OBJECT;
						g_LastSelectedObject = m_Serial;
						g_SelectedObject = this;
					}
				}
				else if (Orion->StaticPixelsInXYAnimated(goGraphic, drawX, drawY, m_Z))
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
/*!
Получить индекс картинки (для анимации)
@return Индекс картинки
*/
WORD TGameItem::GetMountAnimation()
{
	WORD graphic = m_Graphic;

	if (m_Layer == OL_MOUNT)
	{
		switch (graphic)
		{
			case 0x3E90: // 16016 Reptalon
			{
				graphic = 0x0114;
				break;
			}
			case 0x3E91: // 16017
			{
				graphic = 0x0115;
				break;
			}
			case 0x3E92: // 16018
			{
				graphic = 0x011C;
				break;
			}
			case 0x3E94: // 16020
			{
				graphic = 0x00F3;
				break;
			}
			case 0x3E95: // 16021
			{
				graphic = 0x00A9;
				break;
			}
			case 0x3E97: // 16023 Ethereal Giant Beetle
			{
				graphic = 0x00C3;
				break;
			}
			case 0x3E98: // 16024 Ethereal Swamp Dragon
			{
				graphic = 0x00C2;
				break;
			}
			case 0x3E9A: // 16026 Ethereal Ridgeback
			{
				graphic = 0x00C1;
				break;
			}
			case 0x3E9B: // 16027
			case 0x3E9D: // 16029 Ethereal Unicorn
			{
				graphic = 0x00C0;
				break;
			}
			case 0x3E9C: // 16028 Ethereal Kirin
			{
				graphic = 0x00BF;
				break;
			}
			case 0x3E9E: // 16030
			{
				graphic = 0x00BE;
				break;
			}
			case 0x3EA0: // 16032 light grey/horse3
			{
				graphic = 0x00E2;
				break;
			}
			case 0x3EA1: // 16033 greybrown/horse4
			{
				graphic = 0x00E4;
				break;
			}
			case 0x3EA2: // 16034 dark brown/horse
			{
				graphic = 0x00CC;
				break;
			}
			case 0x3EA3: // 16035 desert ostard
			{
				graphic = 0x00D2;
				break;
			}
			case 0x3EA4: // 16036 frenzied ostard (=zostrich)
			{
				graphic = 0x00DA;
				break;
			}
			case 0x3EA5: // 16037 forest ostard
			{
				graphic = 0x00DB;
				break;
			}
			case 0x3EA6: // 16038 Llama
			{
				graphic = 0x00DC;
				break;
			}
			case 0x3EA7: // 16039 Nightmare / Vortex
			{
				graphic = 0x0074;
				break;
			}
			case 0x3EA8: // 16040 Silver Steed
			{
				graphic = 0x0075;
				break;
			}
			case 0x3EA9: // 16041 Nightmare
			{
				graphic = 0x0072;
				break;
			}
			case 0x3EAA: // 16042 Ethereal Horse
			{
				graphic = 0x0073;
				break;
			}
			case 0x3EAB: // 16043 Ethereal Llama
			{
				graphic = 0x00AA;
				break;
			}
			case 0x3EAC: // 16044 Ethereal Ostard
			{
				graphic = 0x00AB;
				break;
			}
			case 0x3EAD: // 16045 Kirin
			{
				graphic = 0x0084;
				break;
			}
			case 0x3EAF: // 16047 War Horse (Blood Red)
			{
				graphic = 0x0078;
				break;
			}
			case 0x3EB0: // 16048 War Horse (Light Green)
			{
				graphic = 0x0079;
				break;
			}
			case 0x3EB1: // 16049 War Horse (Light Blue)
			{
				graphic = 0x0077;
				break;
			}
			case 0x3EB2: // 16050 War Horse (Purple)
			{
				graphic = 0x0076;
				break;
			}
			case 0x3EB3: // 16051 Sea Horse (Medium Blue)
			{
				graphic = 0x0090;
				break;
			}
			case 0x3EB4: // 16052 Unicorn
			{
				graphic = 0x007A;
				break;
			}
			case 0x3EB5: // 16053 Nightmare
			{
				graphic = 0x00B1;
				break;
			}
			case 0x3EB6: // 16054 Nightmare 4
			{
				graphic = 0x00B2;
				break;
			}
			case 0x3EB7: // 16055 Dark Steed
			{
				graphic = 0x00B3;
				break;
			}
			case 0x3EB8: // 16056 Ridgeback
			{
				graphic = 0x00BC;
				break;
			}
			case 0x3EBA: // 16058 Ridgeback, Savage
			{
				graphic = 0x00BB;
				break;
			}
			case 0x3EBB: // 16059 Skeletal Mount
			{
				graphic = 0x0319;
				break;
			}
			case 0x3EBC: // 16060 Beetle
			{
				graphic = 0x0317;
				break;
			}
			case 0x3EBD: // 16061 SwampDragon
			{
				graphic = 0x031A;
				break;
			}
			case 0x3EBE: // 16062 Armored Swamp Dragon
			{
				graphic = 0x031F;
				break;
			}
			case 0x3EC3: //16067 Beetle
			{
				graphic = 0x02D4;
				break;
			}
			case 0x3EC5: // 16069
			case 0x3F3A: // 16186 snow bear ???
			{
				graphic = 0x00D5;
				break;
			}
			case 0x3EC6: // 16070
			{
				graphic = 0x01B0;
				break;
			}
			case 0x3EC7: // 16071
			{
				graphic = 0x04E6;
				break;
			}
			case 0x3EC8: // 16072
			{
				graphic = 0x04E7;
				break;
			}
			case 0x3EC9: // 16073
			{
				graphic = 0x042D;
				break;
			}

			//start FWUO ???
			case 0x2F01: // 12033 Polar bear
			{
				graphic = 0x00D5;
				break;
			}
			case 0x2F03: // 12035 Ridgeback
			{
				graphic = 0x00F6;
				break;
			}
			case 0x2F04: // 12036 Skeletal mount
			{
				graphic = 0x00F3;
				break;
			}
			case 0x2F05: // 12037 Genie
			{
				graphic = 0x023D;
				break;
			}
			case 0x2F10: // 12048 Kyryn
			{
				graphic = 0x00EB;
				break;
			}
			case 0x3001: // 12289 Beetle
			{
				graphic = 0x00F4;
				break;
			}
			//end FWUO ???

			default: //lightbrown/horse2
			{
				graphic = 0x00C8;
				break;
			}
		}
	}
	else if (IsCorpse())
	{
		graphic = (WORD)m_Count;
		AnimationManager->GetCorpseGraphic(graphic);
	}

	return graphic;
}
//---------------------------------------------------------------------------
/*!
Загрузка мульти в текущий объект
@return 
*/
void TGameItem::LoadMulti()
{
	TIndexMulti *index = Orion->GetMultiPointer(m_Graphic - 0x4000);
	
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

				if (ToLowerA(mo->GetStaticData()->Name) == "nodraw")
				{
					delete mo;
					continue;
				}

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

		TGumpMinimap *minimap = (TGumpMinimap*)GumpManager->GetGump(g_PlayerSerial, 0, GT_MINIMAP);

		if (minimap != NULL)
			minimap->LastX = 0;
	}
}
//---------------------------------------------------------------------------
/*!
Добавить объекта-мульти
@param [__in] obj Ссылка на мульти-объект
@return 
*/
void TGameItem::AddMultiObject( __in TMultiObject *obj)
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
			QFOR(multiobj, multi->m_Items, TMultiObject*)
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

			QFOR(multi, m_Items, TMulti*)
			{
				if (multi->m_Next == NULL)
				{
					multi->m_Next = newmulti;
					break;
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
/*!
Получение объекта мульти в заданных координатах
@param [__in] x Координата X
@param [__in] y Координата Y
@return Ссылка на мульти или NULL
*/
TMulti *TGameItem::GetMultiAtXY(__in short x, __in short y)
{
	QFOR(multi, m_Items, TMulti*)
	{
		if (multi->X == x && multi->Y == y)
			return multi;
	}

	return NULL;
}
//---------------------------------------------------------------------------
/*!
Найти объект внутри (рекурсивно) по типу с учетом (и без) цвета
@param [__in] graphic Индекс картинки
@param [__in_opt] color Цвет предмета
@return Ссылка на найденный объект или NULL
*/
TGameItem *TGameItem::FindItem(__in WORD graphic, __in_opt WORD color)
{
	TGameItem *item = NULL;

	if (color == 0xFFFF) //Поиск по минимальному цвету
	{
		WORD minColor = 0xFFFF;

		QFOR(obj, m_Items, TGameItem*)
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
		}
	}
	else //стандартный поиск
	{
		QFOR(obj, m_Items, TGameItem*)
		{
			if (obj->Graphic == graphic && obj->Color == color)
				item = obj;

			if (!obj->Empty())
			{
				TGameItem *found = obj->FindItem(graphic, color);
				
				if (found != NULL)
					item = found;
			}
		}
	}

	return item;
}
//---------------------------------------------------------------------------
/*!
Найти объект на указанном слое (для трупов)
@param [__in] layer Номер слоя
@return Ссылка на найденный объект или NULL
*/
TGameItem *TGameItem::FindLayer(__in int layer)
{
	QFOR(obj, m_Items, TGameItem*)
	{
		if (obj->Layer == layer)
			return obj;
	}

	return NULL;
}
//---------------------------------------------------------------------------