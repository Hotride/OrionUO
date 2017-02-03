/***********************************************************************************
**
** GameWorld.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GameWorld.h"
#include "../OrionUO.h"
#include "../Managers/ConfigManager.h"
#include "../Managers/SoundManager.h"
#include "../Managers/MapManager.h"
#include "../Managers/AnimationManager.h"
#include "GamePlayer.h"
#include "../Party.h"
#include "../Walker/WalkData.h"
//----------------------------------------------------------------------------------
CGameWorld *g_World = NULL;
//----------------------------------------------------------------------------------
CGameWorld::CGameWorld(const uint &serial)
{
	m_Items = NULL;
	CreatePlayer(serial);
}
//----------------------------------------------------------------------------------
CGameWorld::~CGameWorld()
{
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
void CGameWorld::ProcessSound(const uint &ticks, CGameCharacter *gc)
{
	if (g_ConfigManager.FootstepsSound && gc->IsHuman() && !gc->Hidden())
	{
		CWalkData *wd = gc->m_WalkStack.Top();

		if (wd != NULL && gc->LastStepSoundTime < ticks)
		{
			int incID = gc->StepSoundOffset;
			int soundID = 0x012B;
			int delaySound = 400;

			if (gc->FindLayer(OL_MOUNT) != NULL)
			{
				if (wd->Direction & 0x80)
				{
					soundID = 0x0129;
					delaySound = 150;
				}
				else
				{
					//soundID = 0x0129;
					incID = 0;
					delaySound = 350;
				}
			}

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
	int delay = (g_ConfigManager.StandartCharactersAnimationDelay ? ORIGINAL_CHARACTERS_ANIMATION_DELAY : ORION_CHARACTERS_ANIMATION_DELAY);
	g_AnimCharactersDelayValue = (float)delay;

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
				g_AnimationManager.GetBodyGraphic(id);
				int animGroup = gc->GetAnimationGroup(id);

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

					if (direction.FrameCount == 0)
						g_AnimationManager.LoadDirectionGroup(direction);

					if (direction.Address != 0)
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
								frameIndex = 0;
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
				
				WORD id = obj->GetMountAnimation();

				bool mirror = false;

				g_AnimationManager.GetAnimDirection(dir, mirror);

				if (id < MAX_ANIMATIONS_DATA_INDEX_COUNT && dir < 5)
				{
					int animGroup = g_AnimationManager.GetDieGroupIndex(id, gi->UsedLayer);

					CTextureAnimationDirection &direction = g_AnimationManager.m_DataIndex[id].m_Groups[animGroup].m_Direction[dir];

					if (direction.FrameCount == 0)
						g_AnimationManager.LoadDirectionGroup(direction);

					if (direction.Address != 0)
					{
						direction.LastAccessTime = g_Ticks;
						int fc = direction.FrameCount;

						if (frameIndex >= fc)
						{
							if (gi->FieldColor)
								gi->FieldColor = 2;

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
}
//---------------------------------------------------------------------------
/*!
Создать игрока
@param [__in] serial Серийник игрока
@return 
*/
void CGameWorld::CreatePlayer(const uint &serial)
{
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
	CGameCharacter *result = NULL;

	WORLD_MAP::iterator i = m_Map.find(serial);
	if (i != m_Map.end() && ((*i).second)->NPC)
		result = i->second->GameCharacterPtr();

	return result;
}
//---------------------------------------------------------------------------
/*!
Удалить объект из памяти
@param [__in] obj Ссылка на объект
@return 
*/
void CGameWorld::RemoveObject(CGameObject *obj)
{
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
	if (obj->Container != 0xFFFFFFFF)
	{
		CGameObject *container = FindWorldObject(obj->Container);
		if (container != NULL)
			container->Reject(obj);
		else
			obj->Container = 0xFFFFFFFF;
	}
	else
	{
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
//---------------------------------------------------------------------------
/*!
Дамп предметов, хранящихся в памяти
@param [__in] nCount Количество отступов
@param [__in_opt] serial Серийник родителя
@return
*/
void CGameWorld::Dump(uchar tCount, uint serial)
{
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