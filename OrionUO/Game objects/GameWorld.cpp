/***********************************************************************************
**
** GameWorld.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGameWorld *g_World = NULL;
//----------------------------------------------------------------------------------
CGameWorld::CGameWorld(const uint &serial)
{
	WISPFUN_DEBUG("c22_f1");
	CreatePlayer(serial);
}
//----------------------------------------------------------------------------------
CGameWorld::~CGameWorld()
{
	WISPFUN_DEBUG("c22_f2");
	RemovePlayer();

	CGameObject *obj = m_Items;
	while (obj != NULL)
	{
		CGameObject *next = (CGameObject*)obj->m_Next;
		RemoveObject(obj);
		//delete obj;
		obj = next;
	}

	m_Map.clear();

	m_Items = NULL;
}
//----------------------------------------------------------------------------------
void CGameWorld::ResetObjectHandlesState()
{
	QFOR(obj, m_Items, CGameObject*)
		obj->ClosedObjectHandle = false;
}
//----------------------------------------------------------------------------------
void CGameWorld::ProcessSound(const uint &ticks, CGameCharacter *gc)
{
	WISPFUN_DEBUG("c22_f3");
	if (g_ConfigManager.FootstepsSound && gc->IsHuman() && !gc->Hidden())
	{
		if (!gc->m_Steps.empty() && gc->LastStepSoundTime < ticks)
		{
			int incID = gc->StepSoundOffset;
			int soundID = 0x012B;
			int delaySound = 400;

			if (gc->FindLayer(OL_MOUNT) != NULL)
			{			
				if (gc->m_Steps.back().Direction & 0x80)
				{		
					soundID = 0x0129;
					delaySound = 150;
				}
				else
				{
					incID = 0;
					delaySound = 350;
				}
			}

			if (g_ConfigManager.StandartCharactersAnimationDelay)
				delaySound = delaySound * 13 / 10;
			soundID += incID;
			gc->StepSoundOffset = (incID + 1) % 2;

			g_Orion.PlaySoundEffect(soundID, g_SoundManager.GetVolumeValue()); //0x0129 - 0x0134

			gc->LastStepSoundTime = ticks + delaySound;
		}
	}
}
//---------------------------------------------------------------------------
/*!
Обработка анимации всех персонажей
@return 
*/
void CGameWorld::ProcessAnimation()
{
	WISPFUN_DEBUG("c22_f4");
	int delay = (g_ConfigManager.StandartCharactersAnimationDelay ? ORIGINAL_CHARACTERS_ANIMATION_DELAY : ORION_CHARACTERS_ANIMATION_DELAY);
	g_AnimCharactersDelayValue = (float)delay;
	deque<CGameObject*> toRemove;

	QFOR(obj, m_Items, CGameObject*)
	{
		if (obj->NPC)
		{
			CGameCharacter *gc = obj->GameCharacterPtr();
			uchar dir = 0;
			gc->UpdateAnimationInfo(dir, true);

			ProcessSound(g_Ticks, gc);

			if (gc->LastAnimationChangeTime < g_Ticks)
			{
				char frameIndex = gc->AnimIndex;
				
				if (gc->AnimationFromServer && !gc->AnimationDirection)
					frameIndex--;
				else
					frameIndex++;

				ushort id = gc->GetMountAnimation();
				int animGroup = gc->GetAnimationGroup(id);
				gc->ProcessGargoyleAnims(animGroup);

				CGameItem *mount = gc->FindLayer(OL_MOUNT);
				if (mount != NULL)
				{
					switch (animGroup)
					{
						case PAG_FIDGET_1:
						case PAG_FIDGET_2:
						case PAG_FIDGET_3:
						{
							id = mount->GetMountAnimation();
							animGroup = gc->GetAnimationGroup(id);
							break;
						}
						default:
							break;
					}
				}

				bool mirror = false;

				g_AnimationManager.GetAnimDirection(dir, mirror);

				int currentDelay = delay;

				if (id < MAX_ANIMATIONS_DATA_INDEX_COUNT && dir < 5)
				{
					CTextureAnimationDirection &direction = g_AnimationManager.m_DataIndex[id].m_Groups[animGroup].m_Direction[dir];
					g_AnimationManager.AnimID = id;
					g_AnimationManager.AnimGroup = animGroup;
					g_AnimationManager.Direction = dir;
					if (direction.FrameCount == 0)
						g_AnimationManager.LoadDirectionGroup(direction);

					if (direction.Address != 0 || direction.IsUOP)
					{
						direction.LastAccessTime = g_Ticks;
						int fc = direction.FrameCount;

						if (gc->AnimationFromServer)
						{
							currentDelay += currentDelay * (int)(gc->AnimationInterval + 1);

							if (!gc->AnimationFrameCount)
								gc->AnimationFrameCount = fc;
							else
								fc = gc->AnimationFrameCount;

							if (gc->AnimationDirection) //forward
							{
								if (frameIndex >= fc)
								{
									frameIndex = 0;

									if (gc->AnimationRepeat)
									{
										uchar repCount = gc->AnimationRepeatMode;

										if (repCount == 2)
										{
											repCount--;
											gc->AnimationRepeatMode = repCount;
										}
										else if (repCount == 1)
											gc->SetAnimation(0xFF);
									}
									else
										gc->SetAnimation(0xFF);
								}
							}
							else //backward
							{
								if (frameIndex < 0)
								{
									if (!fc)
										frameIndex = 0;
									else
										frameIndex = fc - 1;

									if (gc->AnimationRepeat)
									{
										uchar repCount = gc->AnimationRepeatMode;

										if (repCount == 2)
										{
											repCount--;
											gc->AnimationRepeatMode = repCount;
										}
										else if (repCount == 1)
											gc->SetAnimation(0xFF);
									}
									else
										gc->SetAnimation(0xFF);
								}
							}
						}
						else
						{
							if (frameIndex >= fc)
							{
								frameIndex = 0;

								if (obj->Serial & 0x80000000)
									toRemove.push_back(obj);
							}
						}
						
						gc->AnimIndex = frameIndex;
					}
				}

				gc->LastAnimationChangeTime = g_Ticks + currentDelay;
			}
		}
		else if (obj->IsCorpse())
		{
			CGameItem *gi = (CGameItem*)obj;
			uchar dir = gi->Layer;

			if (obj->LastAnimationChangeTime < g_Ticks)
			{
				char frameIndex = obj->AnimIndex + 1;
				
				ushort id = obj->GetMountAnimation();

				bool mirror = false;

				g_AnimationManager.GetAnimDirection(dir, mirror);

				if (id < MAX_ANIMATIONS_DATA_INDEX_COUNT && dir < 5)
				{
					int animGroup = g_AnimationManager.GetDieGroupIndex(id, gi->UsedLayer);

					CTextureAnimationDirection &direction = g_AnimationManager.m_DataIndex[id].m_Groups[animGroup].m_Direction[dir];
					g_AnimationManager.AnimID = id;
					g_AnimationManager.AnimGroup = animGroup;
					g_AnimationManager.Direction = dir;
					if (direction.FrameCount == 0)
						g_AnimationManager.LoadDirectionGroup(direction);

					if (direction.Address != 0 || direction.IsUOP)
					{
						direction.LastAccessTime = g_Ticks;
						int fc = direction.FrameCount;

						if (frameIndex >= fc)
						{
							frameIndex = fc - 1;
						}
						
						obj->AnimIndex = frameIndex;
					}
				}

				obj->LastAnimationChangeTime = g_Ticks + delay;
			}
		}

		obj->UpdateEffects();
	}

	if (!toRemove.empty())
	{
		for (CGameObject *obj : toRemove)
		{
			g_CorpseManager.Remove(0, obj->Serial);

			RemoveObject(obj);
		}

		g_GameScreen.RenderListInitalized = false;
	}
}
//---------------------------------------------------------------------------
/*!
Создать игрока
@param [__in] serial Серийник игрока
@return 
*/
void CGameWorld::CreatePlayer(const uint &serial)
{
	WISPFUN_DEBUG("c22_f5");
	RemovePlayer();

	g_PlayerSerial = serial;
	g_Player = new CPlayer(serial);

	m_Map[serial] = g_Player;

	if (m_Items != NULL)
		m_Items->Add(g_Player);
	else
	{
		m_Items = g_Player;
		m_Items->m_Next = NULL;
		m_Items->m_Prev = NULL;
	}
}
//---------------------------------------------------------------------------
/*!
Удалить игрока
@return 
*/
void CGameWorld::RemovePlayer()
{
	WISPFUN_DEBUG("c22_f6");
	if (g_Player != NULL)
	{
		RemoveFromContainer(g_Player);
		m_Map[g_Player->Serial] = NULL;
		m_Map.erase(g_Player->Serial);
		delete g_Player;
		g_Player = NULL;
		g_PlayerSerial = 0;
	}
}
//---------------------------------------------------------------------------
/*!
Установить текущего чара с указанным серийником как основного
@param [__in] serial Серийник нового игрока
@return 
*/
void CGameWorld::SetPlayer(const uint &serial)
{
	WISPFUN_DEBUG("c22_f6");
	if (serial != g_Player->Serial)
		CreatePlayer(serial);
}
//---------------------------------------------------------------------------
/*!
Создать (или взять, если уже существует) игровой предмет
@param [__in] serial Серийник предмета
@return Ссылка на предмет
*/
CGameItem *CGameWorld::GetWorldItem(const uint &serial)
{
	WISPFUN_DEBUG("c22_f8");
	WORLD_MAP::iterator i = m_Map.find(serial);

	if (i == m_Map.end() || (*i).second == NULL)
	{
		CGameItem *obj = new CGameItem(serial);

		m_Map[serial] = obj;

		if (m_Items != NULL)
			m_Items->AddObject(obj);
		else
		{
			m_Items = obj;
			m_Items->m_Next = NULL;
			m_Items->m_Prev = NULL;
		}

		return obj;
	}

	return (CGameItem*)(*i).second;
}
//---------------------------------------------------------------------------
/*!
Создать (или взять, если уже существует) игрового персонажа
@param [__in] serial Серийник персонажа
@return Ссылка на персонажа
*/
CGameCharacter *CGameWorld::GetWorldCharacter(const uint &serial)
{
	WISPFUN_DEBUG("c22_f9");
	WORLD_MAP::iterator i = m_Map.find(serial);

	if (i == m_Map.end() || (*i).second == NULL)
	{
		CGameCharacter *obj = new CGameCharacter(serial);

		m_Map[serial] = obj;

		if (m_Items != NULL)
			m_Items->AddObject(obj);
		else
		{
			m_Items = obj;
			m_Items->m_Next = NULL;
			m_Items->m_Prev = NULL;
		}

		return obj;
	}

	return i->second->GameCharacterPtr();
}
//---------------------------------------------------------------------------
/*!
Найти игровой объект в памяти
@param [__in] serial Серийник объекта
@return Ссылка на объект или NULL
*/
CGameObject *CGameWorld::FindWorldObject(const uint &serial)
{
	WISPFUN_DEBUG("c22_f10");
	CGameObject *result = NULL;

	WORLD_MAP::iterator i = m_Map.find(serial);
	if (i != m_Map.end())
		result = (*i).second;

	return result;
}
//---------------------------------------------------------------------------
/*!
Найти игровой предмет в памяти
@param [__in] serial Серийник предмета
@return Ссылка на предмет или NULL
*/
CGameItem *CGameWorld::FindWorldItem(const uint &serial)
{
	WISPFUN_DEBUG("c22_f11");
	CGameItem *result = NULL;

	WORLD_MAP::iterator i = m_Map.find(serial);
	if (i != m_Map.end() && !((*i).second)->NPC)
		result = (CGameItem*)(*i).second;

	return result;
}
//---------------------------------------------------------------------------
/*!
Найти игрового персонажа в памяти
@param [__in] serial Серийник персонажа
@return Ссылка а персонажа или NULL
*/
CGameCharacter *CGameWorld::FindWorldCharacter(const uint &serial)
{
	WISPFUN_DEBUG("c22_f12");
	CGameCharacter *result = NULL;

	WORLD_MAP::iterator i = m_Map.find(serial);
	if (i != m_Map.end() && ((*i).second)->NPC)
		result = i->second->GameCharacterPtr();

	return result;
}
//---------------------------------------------------------------------------
void CGameWorld::ReplaceObject(CGameObject *obj, const uint &newSerial)
{
	WISPFUN_DEBUG("c22_f12_1");

	m_Map[obj->Serial] = NULL;
	m_Map.erase(obj->Serial);

	QFOR(item, obj->m_Items, CGameObject*)
		item->Container = newSerial;

	m_Map[newSerial] = obj;
	obj->Serial = newSerial;
}
//---------------------------------------------------------------------------
/*!
Удалить объект из памяти
@param [__in] obj Ссылка на объект
@return 
*/
void CGameWorld::RemoveObject(CGameObject *obj)
{
	WISPFUN_DEBUG("c22_f13");
	RemoveFromContainer(obj);

	uint serial = obj->Serial;
	m_Map[serial] = NULL;
	m_Map.erase(serial);
	delete obj;
}
//---------------------------------------------------------------------------
/*!
Вынуть объект из контейнера
@param [__in] obj Ссылка на объект
@return 
*/
void CGameWorld::RemoveFromContainer(CGameObject *obj)
{
	WISPFUN_DEBUG("c22_f14");
	uint containerSerial = obj->Container;

	if (containerSerial != 0xFFFFFFFF)
	{
		if (containerSerial < 0x40000000)
			g_GumpManager.UpdateContent(containerSerial, 0, GT_PAPERDOLL);
		else
			g_GumpManager.UpdateContent(containerSerial, 0, GT_CONTAINER);

		if (g_TooltipsEnabled)
			g_PacketManager.AddMegaClilocRequest(containerSerial);

		CGameObject *container = FindWorldObject(containerSerial);

		if (container != NULL)
			container->Reject(obj);
		else
			obj->Container = 0xFFFFFFFF;
	}
	else
	{
		g_GameScreen.RenderListInitalized = false;

		if (m_Items != NULL)
		{
			if (m_Items == obj)
			{
				m_Items = (CGameObject*)m_Items->m_Next;
				if (m_Items != NULL)
					m_Items->m_Prev = NULL;
			}
			else
			{
				if (obj->m_Next != NULL)
				{
					if (obj->m_Prev != NULL)
					{
						obj->m_Prev->m_Next = obj->m_Next;
						obj->m_Next->m_Prev = obj->m_Prev;
					}
					else //WTF???
						obj->m_Next->m_Prev = NULL;
				}
				else if (obj->m_Prev != NULL)
					obj->m_Prev->m_Next = NULL;
			}
		}
	}
	
	obj->m_Next = NULL;
	obj->m_Prev = NULL;
	obj->RemoveRender();
}
//---------------------------------------------------------------------------
/*!
Очистить указанный контейнер
@param [__in] obj Ссылка на объект (контейнер)
@return 
*/
void CGameWorld::ClearContainer(CGameObject *obj)
{
	WISPFUN_DEBUG("c22_f15");
	if (!obj->Empty())
		obj->Clear();
}
//---------------------------------------------------------------------------
/*!
Положить в контейнер
@param [__in] obj Ссылка на объект
@param [__in] container Ссылка на контейнер
@return 
*/
void CGameWorld::PutContainer(CGameObject *obj, CGameObject *container)
{
	WISPFUN_DEBUG("c22_f16");
	RemoveFromContainer(obj);
	container->AddItem(obj);
}
//---------------------------------------------------------------------------
/*!
Поднять объект вверх в очереди
@param [__in] obj Ссылка на объект
@return 
*/
void CGameWorld::MoveToTop(CGameObject *obj)
{
	WISPFUN_DEBUG("c22_f17");
	if (obj == NULL)
		return;

	if (obj->Container == 0xFFFFFFFF)
		g_MapManager->AddRender(obj);

	if (obj->m_Next == NULL)
		return;

	if (obj->Container == 0xFFFFFFFF)
	{
		if (obj->m_Prev == NULL)
		{
			m_Items = (CGameObject*)obj->m_Next;
			m_Items->m_Prev = NULL;

			CGameObject *item = m_Items;

			while (item != NULL)
			{
				if (item->m_Next == NULL)
				{
					item->m_Next = obj;
					obj->m_Prev = item;
					obj->m_Next = NULL;

					break;
				}

				item = (CGameObject*)item->m_Next;
			}
		}
		else
		{
			CGameObject *item = (CGameObject*)obj->m_Next;

			obj->m_Prev->m_Next = obj->m_Next;
			obj->m_Next->m_Prev = obj->m_Prev;
			
			while (item != NULL)
			{
				if (item->m_Next == NULL)
				{
					item->m_Next = obj;
					obj->m_Prev = item;
					obj->m_Next = NULL;

					break;
				}

				item = (CGameObject*)item->m_Next;
			}
		}
	}
	else
	{
		CGameObject *container = FindWorldObject(obj->Container);

		if (container == NULL)
			return;
		
		if (obj->m_Prev == NULL)
		{
			container->m_Items = obj->m_Next;
			container->m_Items->m_Prev = NULL;

			CGameObject *item = (CGameObject*)container->m_Items;

			while (item != NULL)
			{
				if (item->m_Next == NULL)
				{
					item->m_Next = obj;
					obj->m_Prev = item;
					obj->m_Next = NULL;

					break;
				}

				item = (CGameObject*)item->m_Next;
			}
		}
		else
		{
			CGameObject *item = (CGameObject*)obj->m_Next;

			obj->m_Prev->m_Next = obj->m_Next;
			obj->m_Next->m_Prev = obj->m_Prev;
			
			while (item != NULL)
			{
				if (item->m_Next == NULL)
				{
					item->m_Next = obj;
					obj->m_Prev = item;
					obj->m_Next = NULL;

					break;
				}

				item = (CGameObject*)item->m_Next;
			}
		}
	}
}
//---------------------------------------------------------------------------
/*!
Поиск объекта
@param [__in] serialStart Начальный серийник для поиска
@param [__in] scanDistance Дистанция поиска
@param [__in] scanType Тип объектов поиска
@param [__in] scanMode Режим поиска
@return Ссылка на найденный объект или NULL
*/
CGameObject *CGameWorld::SearchWorldObject(const uint &serialStart, const int &scanDistance, const SCAN_TYPE_OBJECT &scanType, const SCAN_MODE_OBJECT &scanMode)
{
	WISPFUN_DEBUG("c22_f18");
	CGameObject *result = NULL;

	CGameObject *start = FindWorldObject(serialStart);

	int count = 2;
	int startI = 0;

	if (scanMode == SMO_PREV)
	{
		if (start == NULL || start->m_Prev == NULL)
		{
			start = m_Items;
			startI = 1;
		}
		else
			start = (CGameObject*)start->m_Prev;
	}
	else
	{
		if (start == NULL || start->m_Next == NULL)
		{
			start = m_Items;
			startI = 1;
		}
		else
			start = (CGameObject*)start->m_Next;
	}

	if (start != NULL)
	{
		CGameObject *obj = start;
		int distance = 100500;
		CGameObject *distanceResult = NULL;

		IFOR(i, startI, count && result == NULL)
		{
			if (i)
			{
				obj = m_Items;

				if (scanMode == SMO_PREV)
				{
					while (obj != NULL && obj->m_Next != NULL)
						obj = (CGameObject*)obj->m_Next;
				}
			}

			while (obj != NULL && result == NULL)
			{
				int dist = GetDistance(obj, g_Player);

				if (obj->Serial != serialStart && dist <= scanDistance)
				{
					bool condition = false;

					if (scanType == STO_OBJECTS)
						condition = (!obj->NPC && !((CGameItem*)obj)->MultiBody);
					else if (obj->NPC && !obj->IsPlayer())
					{
						if (scanType == STO_HOSTLE)
						{
							CGameCharacter *gc = obj->GameCharacterPtr();

							condition = (gc->Notoriety >= NT_SOMEONE_GRAY || gc->Notoriety <= NT_MURDERER);
						}
						else if (scanType == STO_PARTY)
							condition = g_Party.Contains(obj->Serial);
						//else if (scanType == STO_FOLLOWERS)
						//	condition = false;
						else //if (scanType == STO_MOBILES)
							condition = true;
					}

					if (condition)
					{
						if (scanMode == SMO_NEAREST)
						{
							if (dist < distance)
							{
								distance = dist;
								distanceResult = obj;
							}
						}
						else
						{
							result = obj;

							break;
						}
					}
				}

				if (scanMode == SMO_PREV)
					obj = (CGameObject*)obj->m_Prev;
				else
					obj = (CGameObject*)obj->m_Next;
			}
		}

		if (distanceResult != NULL)
			result = distanceResult;
	}

	return result;
}

//----------------------------------------------------------------------------------
void CGameWorld::UpdateGameObject(const uint &serial, ushort graphic, const uchar &graphicIncrement, const int &count, const int &x, const int &y, const char &z, const uchar &direction, const ushort &color, const uchar &flags, const int &a11, const UPDATE_GAME_OBJECT_TYPE &updateType, const ushort &a13)
{
	LOG("UpdateGameObject 0x%08lX:0x%04X 0x%04X (%i) %d:%d:%d %i\n", serial, graphic, color, count, x, y, z, direction);

	CGameCharacter *character = NULL;
	CGameItem *item = NULL;
	CGameObject *obj = g_World->FindWorldObject(serial);

	graphic += graphicIncrement;

	if (obj == NULL)
	{
		LOG("created ");

		if (!(serial & 0x40000000) && updateType != 3)
		{
			character = g_World->GetWorldCharacter(serial);

			if (character == NULL)
			{
				LOG("No memory?\n");
				return;
			}

			obj = character;
			character->Graphic = graphic;
			character->OnGraphicChange(1000);
			character->Direction = direction;
			character->Color = g_ColorManager.FixColor(color, (color & 0x8000));
			character->X = x;
			character->Y = y;
			character->Z = z;
			character->Flags = flags;
		}
		else
		{
			item = g_World->GetWorldItem(serial);

			if (item == NULL)
			{
				LOG("No memory?\n");
				return;
			}

			obj = item;
		}
	}
	else
	{
		LOG("updated ");

		if (obj->Container != 0xFFFFFFFF)
		{
			g_World->RemoveFromContainer(obj);
			obj->Container = 0xFFFFFFFF;
			g_World->m_Items->AddObject(obj);
		}

		if (obj->NPC)
			character = (CGameCharacter*)obj;
		else
			item = (CGameItem*)obj;
	}

	if (obj == NULL)
		return;

	obj->MapIndex = g_CurrentMap;

	if (!obj->NPC)
	{
		if (updateType == UGOT_MULTI)
		{
			item->MultiBody = true;
			item->WantUpdateMulti = ((graphic & 0x3FFF) != obj->Graphic) || (obj->X != x) || (obj->Y != y) || (obj->Z != z);

			item->Graphic = graphic & 0x3FFF;
		}
		else
		{
			item->MultiBody = false;

			item->Graphic = graphic;
		}

		if (item->Dragged)
		{
			g_GumpManager.CloseGump(serial, 0, GT_DRAG);
			item->Dragged = false;
		}

		item->X = x;
		item->Y = y;
		item->Z = z;

		if (graphic == 0x2006)
			item->Layer = direction;

		item->Color = g_ColorManager.FixColor(color, (color & 0x8000));
		item->Count = count;
		item->Flags = flags;

		item->OnGraphicChange(direction);

		LOG("serial:0x%08X graphic:0x%04X color:0x%04X count:%i xyz:%d,%d,%d flags:0x%02X\n", obj->Serial, obj->Graphic, obj->Color, item->Count, obj->X, obj->Y, obj->Z, obj->Flags);
	}
	else
	{
		bool found = false;

		if (character->m_Steps.size() != MAX_STEPS_COUNT)
		{
			//if (character->Graphic == graphic && character->Flags == flags)
			{
				if (!character->m_Steps.empty())
				{
					CWalkData &wd = character->m_Steps.back();

					if (wd.X == x && wd.Y == y && wd.Z == z && wd.Direction == direction)
					{
						found = true;
					}
				}
				else if (character->X == x && character->Y == y && character->Z == z && character->Direction == direction)
				{
					found = true;
				}
			}

			if (!found)
			{
				if (character->m_Steps.empty())
					character->LastStepTime = g_Ticks;

				character->m_Steps.push_back(CWalkData(x, y, z, direction, graphic & 0x3FFF, flags));
				found = true;
			}
		}

		if (!found)
		{
			character->Graphic = graphic & 0x3FFF;
			character->X = x;
			character->Y = y;
			character->Z = z;
			character->Direction = direction;
			character->Color = g_ColorManager.FixColor(color, (color & 0x8000));
			character->Flags = flags;

			character->m_Steps.clear();

			character->OffsetX = 0;
			character->OffsetY = 0;
			character->OffsetZ = 0;
		}

		LOG("NPC serial:0x%08X graphic:0x%04X color:0x%04X xyz:%d,%d,%d flags:0x%02X direction:%d notoriety:%d\n", obj->Serial, obj->Graphic, obj->Color, obj->X, obj->Y, obj->Z, obj->Flags, character->Direction, character->Notoriety);
	}

	if (obj != NULL)
	{
		if (g_TooltipsEnabled && !obj->ClilocMessage.length())
			g_PacketManager.AddMegaClilocRequest(obj->Serial);

		g_World->MoveToTop(obj);
	}
}
//----------------------------------------------------------------------------------
/*void __cdecl UpdateGameObject(int serial, signed int graphic, int graphicIncrement, int count, int x, int y, char z, unsigned __int8 direction, unsigned __int16 color, unsigned __int8 flags, int a11, char updateType, __int16 a13)
{
	v13 = a13;
	v66 = 0;
	LOWORD(v67) = 0;
	v65 = 0;
	v64 = -1;
	if (g_PlayerX < g_MapWidth)
	{
		v14 = sub_5E1510();
		if (g_PlayerX < v14 && x > g_MapWidth - v14)
			x -= g_MapWidth;
		if (g_PlayerX >= g_MapWidth - v14 && x <= v14)
			x += g_MapWidth;
		if (g_PlayerY < v14 && y > g_MapHeight - v14)
			y -= g_MapHeight;
		if (g_PlayerY >= g_MapHeight - v14 && y <= v14)
			y += g_MapHeight;
	}
	if (updateType == 2)
	{
		v64 = (unsigned __int16)graphic;
		graphic = 1;
	}
	obj = (CGameCharacter *)g_Player;
	v16 = color;
	if (!g_Player || serial != g_Player->GameCharacter.GameObject.Serial)
	{
		obj = (CGameCharacter *)g_WorldObjectFirstPtr;
		if (!g_WorldObjectFirstPtr)
		{
		LABEL_23:
			if (!(serial & 0x40000000) && updateType != 3)
			{
				characterMem = (CGameCharacter *)j__malloc(0x298u);
				v13 = 0;
				v68 = 4;
				if (characterMem)
					obj = CGameCharacter__CGameCharacter(characterMem, serial, graphic, graphicIncrement);
				else
					obj = 0;
				v63 = color;
				v68 = -1;
				obj->field_1F0 = 0;
				obj->field_1F4 = 0;
				obj->GameObject.Direction = direction & 0x7F;
				obj->field_1A4 = (unsigned int)direction >> 7;
				v18 = FixColor(v63);
				obj->GameObject.Color = v18;
				if (dword_739F58
					&& (v19 = obj->GameObject.Graphic,
					LOWORD(v19) = v19 + 0x4000,
					(unsigned __int8)(*(int(__stdcall **)(int))(*(_DWORD *)dword_739F58 + 4))(v19))
					&& !obj->GameObject.Color)
					obj->GameObject.Color2 = sub_452670(dword_739F58, 0, (int)obj, obj->GameObject.Graphic);
				else
					obj->GameObject.Color2 = v18;
				LOWORD(v66) = x;
				HIWORD(v66) = y;
				LOWORD(v67) = z;
				obj->field_1F8 = z;
				v20 = obj->GameObject.VTable[25];
				LOWORD(v63) = v67;
				((void(__thiscall *)(CGameCharacter *, int, signed int))v20)(obj, v66, v63);
				sub_5B1850(obj, flags);
				goto LABEL_66;
			}
			if ((_WORD)graphic == 0x2006)
			{
				corpseMem = (CGameCorpse *)j__malloc(0x188u);
				v68 = 0;
				if (corpseMem)
				{
					v22 = (CGameObject *)CGameCorpse__CGameCorpse(corpseMem, serial, 0x2006, count);
					v68 = -1;
				LABEL_48:
					obj = (CGameCharacter *)v22;
					v22->Direction = direction;
					v26 = color & 0x8000;
					v27 = color & 0x4000;
					v28 = color & 0x3FFF;
					if (color & 0x3FFF)
					{
						if ((unsigned int)v28 > 0xBB7)
							v27 |= v26 | 1;
						else
							LOWORD(v27) = v26 | v27 | v28;
					}
					else
					{
						LOWORD(v27) = color & 0x8000;
					}
					obj->GameObject.Color = v27;
					if (dword_739F58
						&& (v29 = obj->GameObject.Graphic,
						LOWORD(v29) = v29 + 0x4000,
						(unsigned __int8)(*(int(__stdcall **)(int))(*(_DWORD *)dword_739F58 + 4))(v29))
						&& !obj->GameObject.Color)
					{
						LOWORD(v30) = sub_452670(dword_739F58, a13, (int)obj, obj->GameObject.Graphic);
					}
					else
					{
						v31 = color & 0x8000;
						v30 = color & 0x4000;
						v32 = color & 0x3FFF;
						if (color & 0x3FFF)
						{
							if ((unsigned int)v32 > 0xBB7)
								v30 |= v31 | 1;
							else
								LOWORD(v30) = v31 | v30 | v32;
						}
						else
						{
							LOWORD(v30) = color & 0x8000;
						}
					}
					obj->GameObject.Color2 = v30;
					v63 = flags;
					v62 = obj;
					obj->GameObject.Count = count;
					sub_5B1850(v62, v63);
					v16 = color;
					v65 = 1;
					obj->GameObject.field_A0 = updateType == 3;
					goto LABEL_66;
				}
			}
			else
			{
				v23 = 48 * (unsigned __int16)graphic;
				if (*(_DWORD *)((char *)g_StaticTiledata + v23) & UO_FLAG_MAP)
				{
					v24 = (CGameMap *)j__malloc(0xF8u);
					v68 = 1;
					if (v24)
					{
						v22 = (CGameObject *)CGameMap__CGameMap(v24, serial, graphic, graphicIncrement, 1);
						v68 = -1;
						goto LABEL_48;
					}
				}
				else if (sub_5CDC90((unsigned __int16)graphic))
				{
					v68 = 2;
					if (j__malloc(0x118u))
					{
						v22 = (CGameObject *)sub_5D3C20(serial, graphic, graphicIncrement, 1);
						v68 = -1;
						goto LABEL_48;
					}
				}
				else
				{
					if (!(*(_DWORD *)((char *)g_StaticTiledata + v23) & UO_FLAG_CONTAINER))
					{
						v22 = CreateCGameObject(
							(char)graphicIncrement,
							a13,
							serial,
							(unsigned __int16)graphic,
							graphicIncrement,
							a13 != 0);
						goto LABEL_48;
					}
					v25 = (CGameContainer *)j__malloc(0x108u);
					v68 = 3;
					if (v25)
					{
						v22 = (CGameObject *)CGameContainer__CGameContainer(v25, serial, graphic, graphicIncrement, 1);
						v68 = -1;
						goto LABEL_48;
					}
				}
			}
			v22 = 0;
			v68 = -1;
			goto LABEL_48;
		}
		while (obj->GameObject.Serial != serial)
		{
			obj = (CGameCharacter *)obj->GameObject.NextObject;
			if (!obj)
				goto LABEL_23;
		}
	}
	if (!obj)
		goto LABEL_23;
	v33 = obj->GameObject.ValidateBits;
	v63 = 0x10F4;
	v62 = (CGameCharacter *)byte_6C6AFD;
	ValidateObject((int)obj, v33);
	if (!(*(int(__thiscall **)(CGameCharacter *))((int(__thiscall **)(_DWORD))obj->GameObject.VTable + 9))(obj)
		&& !obj->GameObject.field_C8)
		(*(void(__thiscall **)(CGameCharacter *))((void(__thiscall **)(_DWORD))obj->GameObject.VTable + 20))(obj);
LABEL_66:
	if (obj)
		v34 = obj->GameObject.ValidateBits;
	else
		v34 = 0xFEEDBEEF;
	v63 = 0x1106;
	v62 = (CGameCharacter *)byte_6C6AFD;
	ValidateObject((int)obj, v34);
	if (!(*(int(__thiscall **)(CGameCharacter *))((int(__thiscall **)(_DWORD))obj->GameObject.VTable + 9))(obj))
	{
		obj->GameObject.field_A0 = updateType == 3;
		LOWORD(v66) = x;
		HIWORD(v66) = y;
		LOWORD(v67) = z;
		obj->GameObject.GraphicIncrement = graphicIncrement;
		v63 = flags;
		v62 = obj;
		obj->GameObject.Direction = direction;
		obj->GameObject.Count = count;
		sub_5B1850(v62, v63);
		v35 = v16 & 0x8000;
		v36 = v16 & 0x4000;
		v37 = v16 & 0x3FFF;
		if (v16 & 0x3FFF)
		{
			if ((unsigned int)v37 > 0xBB7)
				v35 |= v36 | 1;
			else
				v35 |= v36 | v37;
		}
		obj->GameObject.Color = v35;
		if (v64 == -1)
		{
			v13 = graphic;
			if (obj->GameObject.Graphic != (_WORD)graphic)
			{
				obj->GameObject.Graphic = graphic;
				obj->GameObject.field_94 = 0;
			}
			if (dword_739F58 && (unsigned __int8)(*(int(__stdcall **)(int))(*(_DWORD *)dword_739F58 + 4))(graphic + 0x4000))
				obj->GameObject.Graphic2 = sub_446CA0(dword_739F58, graphic, (unsigned __int16)graphic + 0x4000, 0) & 0x3FFF;
			else
				obj->GameObject.Graphic2 = graphic;
			v38 = obj->GameObject.VTable[25];
			LOWORD(v63) = v67;
			((void(__thiscall *)(CGameCharacter *, int, signed int))v38)(obj, v66, v63);
		}
		else
		{
			sub_59D910((int)obj, (int)&v66, v64);
			sub_59C820(obj, (unsigned __int16)v16);
		}
		if (dword_739F58
			&& (v39 = obj->GameObject.Graphic,
			LOWORD(v39) = v39 + 0x4000,
			(unsigned __int8)(*(int(__stdcall **)(int))(*(_DWORD *)dword_739F58 + 4))(v39))
			&& !obj->GameObject.Color)
		{
			obj->GameObject.Color2 = sub_452670(dword_739F58, v13, (int)obj, obj->GameObject.Graphic);
		}
		else
		{
			v40 = v16 & 0x8000;
			v41 = v16 & 0x4000;
			v42 = v16 & 0x3FFF;
			if (v16 & 0x3FFF)
			{
				if ((unsigned int)v42 > 0xBB7)
					obj->GameObject.Color2 = v40 | v41 | 1;
				else
					obj->GameObject.Color2 = v40 | v41 | v42;
			}
			else
			{
				obj->GameObject.Color2 = v40;
			}
		}
		goto LABEL_150;
	}
	v43 = sub_45CFB0();
	v54 = obj->field_1EC == 4;
	obj->field_1AC = v43;
	if (v54)
	{
		v44 = 0;
		if (obj->GameObject.InRenderQueue)
		{
			(*(void(__thiscall **)(CGameCharacter *))((void(__thiscall **)(_DWORD))obj->GameObject.VTable + 5))(obj);
			v44 = 1;
		}
		v45 = graphic;
		obj->GameObject.X = x;
		v63 = graphic;
		obj->field_19C = 1;
		obj->GameObject.Y = y;
		obj->GameObject.Z = z;
		obj->GameObject.Graphic = graphic;
		sub_5B01F0((int)&obj->field_108, v63);
		UpdateRace(obj);
		obj->GameObject.Direction = direction & 0x7F;
		v46 = color & 0x8000;
		v47 = color & 0x4000;
		v48 = color & 0x3FFF;
		if (color & 0x3FFF)
		{
			if ((unsigned int)v48 > 0xBB7)
				v47 |= v46 | 1;
			else
				LOWORD(v47) = v46 | v47 | v48;
		}
		else
		{
			LOWORD(v47) = color & 0x8000;
		}
		obj->GameObject.Color = v47;
		obj->field_1EC = 0;
		if (dword_739F58)
		{
			if ((unsigned __int8)(*(int(__stdcall **)(int))(*(_DWORD *)dword_739F58 + 4))(graphic + 0x4000))
			{
				v49 = sub_446CA0(dword_739F58, v44, (unsigned __int16)graphic + 0x4000, 0);
				v54 = obj->GameObject.Color == 0;
				obj->GameObject.Graphic2 = v49 & 0x3FFF;
				if (v54)
				{
					v50 = sub_452670(dword_739F58, v44, (int)obj, graphic);
					goto LABEL_107;
				}
			LABEL_106:
				v50 = FixColor(color);
			LABEL_107:
				v63 = flags;
				v62 = obj;
				obj->GameObject.Color2 = v50;
				sub_5B1850(v62, v63);
				obj->field_1A8 = 0;
				if (v44)
					(*(void(__thiscall **)(CGameCharacter *))((void(__thiscall **)(_DWORD))obj->GameObject.VTable + 4))(obj);
				goto LABEL_137;
			}
			v45 = graphic;
		}
		obj->GameObject.Graphic2 = v45;
		goto LABEL_106;
	}
	if (obj->GameObject.Color != v16)
	{
		v63 = (unsigned __int16)v16;
		obj->GameObject.Color = v16;
		obj->GameObject.Color2 = FixColor(v63);
	}
	if (obj->GameObject.Flags != flags)
		sub_5B1850(obj, flags);
	if (obj->GameObject.Graphic != (_WORD)graphic)
	{
		v63 = graphic;
		obj->GameObject.Graphic = graphic;
		sub_5B01F0((int)&obj->field_108, v63);
		UpdateRace(obj);
		if (dword_739F58
			&& (v51 = obj->GameObject.Graphic,
			LOWORD(v51) = v51 + 0x4000,
			(unsigned __int8)(*(int(__stdcall **)(int))(*(_DWORD *)dword_739F58 + 4))(v51)))
			obj->GameObject.Graphic2 = sub_446CA0(dword_739F58, v13, obj->GameObject.Graphic + 0x4000, 0) & 0x3FFF;
		else
			obj->GameObject.Graphic2 = obj->GameObject.Graphic;
	}
	if (obj->field_1A8 && obj->GameObject.InRenderQueue)
	{
		(*(void(__thiscall **)(CGameCharacter *))((void(__thiscall **)(_DWORD))obj->GameObject.VTable + 5))(obj);
		(*(void(__thiscall **)(CGameCharacter *))((void(__thiscall **)(_DWORD))obj->GameObject.VTable + 4))(obj);
	}
	v52 = obj->field_1EC;
	v53 = 1;
	updateTypea = 1;
	if (v52)
	{
		v55 = (char *)obj + 12 * v52;
		if (*((_WORD *)&obj->field_1A8 + 6 * v52) == x && *((_WORD *)v55 + 213) == y && *((_WORD *)v55 + 214) == z)
			v53 = 0;
		v54 = *((_BYTE *)&obj->GameObject.VTable + 4 * (3 * v52 + 108)) == direction;
	}
	else
	{
		if (obj->GameObject.X == x && obj->GameObject.Y == y && obj->GameObject.Z == z)
			v53 = 0;
		v54 = ((direction ^ obj->GameObject.Direction) & 0x7F) == 0;
	}
	if (v54)
		updateTypea = 0;
	if (v53 || updateTypea)
	{
		obj->field_1A8 = v53 == 0;
		*((_WORD *)&obj->field_1B4 + 6 * v52) = x;
		*((_WORD *)&obj->field_1B4 + 6 * obj->field_1EC + 1) = y;
		*((_WORD *)&obj->field_1B8 + 6 * obj->field_1EC) = z;
		*((_BYTE *)&obj->GameObject.VTable + 12 * (obj->field_1EC++ + 37)) = direction;
	}
LABEL_137:
	if (g_Player->field_2AC
		&& obj->GameObject.Serial == g_Player->field_2B4
		&& !(*(int(__thiscall **)(CGameCharacter *, signed int))((int(__thiscall **)(_DWORD, _DWORD))obj->GameObject.VTable
		+ 22))(
		obj,
		3))
	{
		v56 = obj->field_1EC;
		if (v56 <= 0)
		{
			v58 = obj->GameObject.Z;
			v64 = *(_DWORD *)&obj->GameObject.X;
		}
		else
		{
			v57 = 3 * v56;
			v58 = *((_WORD *)&obj->field_1AC + 2 * v57);
			v64 = *(&obj->field_1A8 + v57);
		}
		if (g_Pathfinding)
			sub_5C3CB0();
		v63 = 1;
		v62 = (CGameCharacter *)100;
		v59 = sub_5C4300(
			g_Player->GameCharacter.GameObject.X,
			g_Player->GameCharacter.GameObject.Y,
			g_Player->GameCharacter.GameObject.Z,
			(signed __int16)v64,
			SHIWORD(v64),
			v58,
			100,
			1);
		dword_AE72B0 = v59;
		if (!v59)
		{
			CreateSystemMessageASCII(0, 3, "Can't get there (follow)");
			g_Pathfinding = 0;
			return;
		}
		g_Pathfinding = 1;
		v60 = v59;
		sub_503C40();
		for (i = *(_DWORD *)(v60 + 36); i; i = *(_DWORD *)(i + 36))
			v60 = i;
		dword_AE72B0 = v60;
		dword_AE72A4 = *(_DWORD *)(v60 + 16);
		dword_AE72A0 = *(_DWORD *)(v60 + 20);
		dword_AE729C = *(_DWORD *)(v60 + 24);
		dword_AE7298 = g_Player->GameCharacter.GameObject.Direction;
		dword_AE72AC = v60;
	}
LABEL_150:
	if (v65 == 1)
		sub_5ADA80(obj);
	if ((*(int(__thiscall **)(CGameCharacter *))((int(__thiscall **)(_DWORD))obj->GameObject.VTable + 23))(obj)
		|| (*(int(__thiscall **)(CGameCharacter *))((int(__thiscall **)(_DWORD))obj->GameObject.VTable + 33))(obj))
		sub_534560(&obj->GameObject);
	else
		(*((void(**)(void))obj->GameObject.VTable + 1))();
}*/
//----------------------------------------------------------------------------------
void CGameWorld::UpdatePlayer(const uint &serial, const ushort &graphic, const uchar &graphicIncrement, const ushort &color, const uchar &flags, const int &x, const int &y, const ushort &serverID, const uchar &direction, const char &z)
{
	if (serial == g_PlayerSerial)
	{
		g_Player->CloseBank();

		g_Walker.WalkingFailed = false;

		g_Player->X = x;
		g_Player->Y = y;
		g_Player->Z = z;

		bool oldDead = g_Player->Dead();
		ushort oldGraphic = g_Player->Graphic;

		g_Player->Graphic = graphic;
		g_Player->OnGraphicChange();

		g_Player->Direction = direction;
		g_Player->Color = g_ColorManager.FixColor(color);

		//UpdatePlayerCoordinates(x, y, z, serverID);

		g_Player->Flags = flags;

		g_Walker.DenyWalk(-1, -1, -1, -1);
		g_Weather.Reset();

		if (oldGraphic && oldGraphic != g_Player->Graphic)
		{
			if (g_Player->Dead())
			{
				g_Target.Reset();

				if (g_ConfigManager.Music)
					g_Orion.PlayMusic(42, true);

				g_DeathScreenTimer = g_Ticks + DEATH_SCREEN_DELAY;
			}
		}

		if (oldDead != g_Player->Dead())
		{
			if (g_Player->Dead())
				g_Orion.ChangeSeason(ST_DESOLATION, DEATH_MUSIC_INDEX);
			else
				g_Orion.ChangeSeason(g_OldSeason, g_OldSeasonMusic);
		}

		g_Walker.ResendPacketSended = false;

		g_Player->UpdateRemoveRange();
		g_GumpManager.RemoveRangedGumps();

		g_World->MoveToTop(g_Player);
	}
}
//----------------------------------------------------------------------------------
void CGameWorld::UpdateItemInContainer(CGameObject *obj, CGameObject *container, const int &x, const int &y)
{
	obj->X = x;
	obj->Y = y;
	g_World->PutContainer(obj, container);

	uint containerSerial = container->Serial;

	if (g_TooltipsEnabled && !obj->ClilocMessage.length())
		g_PacketManager.AddMegaClilocRequest(obj->Serial);

	CGump *gump = g_GumpManager.UpdateContent(containerSerial, 0, GT_BULLETIN_BOARD);

	if (gump != NULL) //Message board item
		CPacketBulletinBoardRequestMessageSummary(containerSerial, obj->Serial).Send();
	else
	{
		gump = g_GumpManager.UpdateContent(containerSerial, 0, GT_SPELLBOOK);

		if (gump == NULL)
		{
			gump = g_GumpManager.UpdateContent(containerSerial, 0, GT_CONTAINER);

			if (gump != NULL && gump->GumpType == GT_CONTAINER)
				((CGumpContainer*)gump)->UpdateItemCoordinates(obj);

			if (g_TooltipsEnabled)
				g_PacketManager.AddMegaClilocRequest(containerSerial);
		}

		if (gump != NULL && !container->NPC)
		{
			((CGameItem*)container)->Opened = true;
		}
	}

	CGameObject *top = container->GetTopObject();

	if (top != NULL)
	{
		top = top->FindSecureTradeBox();

		if (top != NULL)
			g_GumpManager.UpdateContent(0, top->Serial, GT_TRADE);
	}
}
//----------------------------------------------------------------------------------
void CGameWorld::UpdateContainedItem(const uint &serial, const ushort &graphic, const uchar &graphicIncrement, const ushort &count, const int &x, const int &y, const uint &containerSerial, const ushort &color)
{
	CGameObject *container = g_World->FindWorldObject(containerSerial);

	if (container == NULL)
		return;

	CGameObject *obj = g_World->FindWorldObject(serial);

	if (obj != NULL && (!container->IsCorpse() || ((CGameItem*)obj)->Layer == OL_NONE))
	{
		g_World->RemoveObject(obj);
		obj = NULL;
	}

	if (obj == NULL)
	{
		if (serial & 0x40000000)
			obj = g_World->GetWorldItem(serial);
		else
			obj = g_World->GetWorldCharacter(serial);
	}

	if (obj == NULL)
	{
		LOG("No memory?\n");
		return;
	}

	obj->MapIndex = g_CurrentMap;

	obj->Graphic = graphic + graphicIncrement;
	obj->OnGraphicChange();
	obj->Color = g_ColorManager.FixColor(color, (color & 0x8000));

	obj->Count = count;

	UpdateItemInContainer(obj, container, x, y);

	//sub_534560(v19);

	g_World->MoveToTop(obj);

	LOG("\t|0x%08X<0x%08X:%04X*%d (%d,%d) %04X\n", containerSerial, serial, graphic + graphicIncrement, count, x, y, color);
}
//---------------------------------------------------------------------------
/*!
Дамп предметов, хранящихся в памяти
@param [__in] nCount Количество отступов
@param [__in_opt] serial Серийник родителя
@return
*/
void CGameWorld::Dump(uchar tCount, uint serial)
{
	WISPFUN_DEBUG("c22_f19");
	LOG("World Dump:\n\n");

	CGameObject *obj = m_Items;

	if (serial != 0xFFFFFFFF)
	{
		obj = FindWorldObject(serial);
		if (obj != NULL)
			obj = (CGameObject*)obj->m_Items;
	}

	while (obj != NULL)
	{
		if (obj->Container == serial)
		{
			if (obj->Serial == g_Player->Serial)
				LOG("---Player---\n");

			IFOR(i, 0, tCount)
				LOG("\t");

			LOG("%s%08X:%04X[%04X](%%02X)*%i\tin 0x%08X XYZ=%i,%i,%i on Map %i\n", (obj->NPC ? "NPC: " : "Item: "), obj->Serial, obj->Graphic, obj->Color, /*obj->Layer,*/ obj->Count, obj->Container, obj->X, obj->Y, obj->Z, obj->MapIndex);

			if (obj->m_Items != NULL)
				Dump(tCount + 1, obj->Container);
		}

		obj = (CGameObject*)obj->m_Next;
	}
}
//---------------------------------------------------------------------------