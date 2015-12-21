/****************************************************************************
**
** GameWorld.cpp
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

TGameWorld *World = NULL;
//---------------------------------------------------------------------------
TGameWorld::TGameWorld(DWORD serial)
{
	m_Items = NULL;
	CreatePlayer(serial);
}
//---------------------------------------------------------------------------
TGameWorld::~TGameWorld()
{
	RemovePlayer();

	TGameObject *obj = m_Items;
	while (obj != NULL)
	{
		TGameObject *next = (TGameObject*)obj->m_Next;
		RemoveObject(obj);
		//delete obj;
		obj = next;
	}

	m_Map.clear();

	m_Items = NULL;
}
//---------------------------------------------------------------------------
void TGameWorld::ProcessAnimation()
{
	TGameObject *obj = (TGameObject*)m_Items;
	DWORD ticks = GetTickCount();
	BYTE delay = (ConfigManager.StandartCharactersAnimationDelay ? 0x50 : 50);
	g_AnimCharactersDelayValue = delay;

	while (obj != NULL)
	{
		if (obj->NPC)
		{
			TGameCharacter *gc = (TGameCharacter*)obj;
			BYTE dir = 0;
			gc->UpdateAnimationInfo(dir, true);

			if (ConfigManager.FootstepsSound && gc->IsHuman())
			{
				TWalkData *wd = gc->m_WalkStack.Top();

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

					UO->PlaySoundEffect(soundID, ConfigManager.SoundVolume); //0x0129 - 0x0134

					gc->LastStepSoundTime = ticks + delaySound;
				}
			}

			if (gc->LastAnimationChangeTime < ticks)
			{
				char frameIndex = gc->AnimIndex;
				
				if (gc->AnimationFromServer && !gc->AnimationDirection)
					frameIndex--;
				else
					frameIndex++;

				WORD id = gc->GetMountAnimation();
				TTextureAnimation *anim = AnimationManager->GetAnimation(id);

				bool mirror = false;

				AnimationManager->GetAnimDirection(dir, mirror);

				if (anim != NULL)
				{
					int animGroup = gc->GetAnimationGroup();

					TTextureAnimationGroup *group = anim->GetGroup(animGroup);
					TTextureAnimationDirection *direction = group->GetDirection(dir);

					if (direction->Address == 0)
					{
						bool partialHue = obj->IsHuman();
						WORD color = gc->Color;
						AnimationManager->AnimGroup = animGroup;
						AnimationManager->Direction = dir;
						AnimationManager->Color = 0;

						int offset = (animGroup * 5) + dir;
						AnimationManager->ExecuteDirectionGroup(direction, id, color, partialHue, offset);
					}

					if (direction->Address != 0)
					{
						direction->LastAccessTime = ticks;
						int fc = direction->FrameCount;

						if (gc->AnimationFromServer)
						{
							int ai = (gc->AnimationInterval * 2) + 1;

							if (ai < 1)
								ai = 1;

							gc->LastAnimationChangeTime = ticks + (ai * delay);

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
										BYTE repCount = gc->AnimationRepeatMode;

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
										BYTE repCount = gc->AnimationRepeatMode;

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

				gc->LastAnimationChangeTime = ticks + delay;
			}
		}
		else if (obj->IsCorpse())
		{
			TGameItem *gi = (TGameItem*)obj;
			BYTE dir = gi->Layer;

			if (obj->LastAnimationChangeTime < ticks)
			{
				char frameIndex = obj->AnimIndex + 1;
				
				WORD id = obj->GetMountAnimation();
				TTextureAnimation *anim = AnimationManager->GetAnimation(id);

				bool mirror = false;

				AnimationManager->GetAnimDirection(dir, mirror);

				if (anim != NULL)
				{
					int animGroup = AnimationManager->GetDieGroupIndex(id, gi->UsedLayer);

					TTextureAnimationGroup *group = anim->GetGroup(animGroup);
					TTextureAnimationDirection *direction = group->GetDirection(dir);

					if (direction->Address == 0)
					{
						bool partialHue = obj->IsHuman();
						WORD color = obj->Color;
						AnimationManager->AnimGroup = animGroup;
						AnimationManager->Direction = dir;
						AnimationManager->Color = 0;

						int offset = (animGroup * 5) + dir;
						AnimationManager->ExecuteDirectionGroup(direction, id, color, partialHue, offset);
					}

					if (direction->Address != 0)
					{
						direction->LastAccessTime = ticks;
						int fc = direction->FrameCount;

						if (frameIndex >= fc)
							frameIndex = fc - 1;
						
						obj->AnimIndex = frameIndex;
					}
				}

				obj->LastAnimationChangeTime = ticks + delay;
			}
		}

		obj = (TGameObject*)obj->m_Next;
	}
}
//---------------------------------------------------------------------------
void TGameWorld::CreatePlayer(DWORD serial)
{
	RemovePlayer();

	g_PlayerSerial = serial;
	g_Player = new TPlayer(serial);

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
void TGameWorld::RemovePlayer()
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
void TGameWorld::SetPlayer(DWORD serial)
{
	if (serial != g_Player->Serial)
		CreatePlayer(serial);
}
//---------------------------------------------------------------------------
TGameItem *TGameWorld::GetWorldItem(DWORD serial)
{
	WORLD_MAP::iterator i = m_Map.find(serial);

	if (i == m_Map.end() || (*i).second == NULL)
	{
		TGameItem *obj = new TGameItem(serial);

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

	return (TGameItem*)(*i).second;
}
//---------------------------------------------------------------------------
TGameCharacter *TGameWorld::GetWorldCharacter(DWORD serial)
{
	WORLD_MAP::iterator i = m_Map.find(serial);

	if (i == m_Map.end() || (*i).second == NULL)
	{
		TGameCharacter *obj = new TGameCharacter(serial);

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

	return (TGameCharacter*)(*i).second;
}
//---------------------------------------------------------------------------
TGameObject *TGameWorld::FindWorldObject(DWORD serial)
{
	TGameObject *result = NULL;

	WORLD_MAP::iterator i = m_Map.find(serial);
	if (i != m_Map.end())
		result = (*i).second;

	return result;
}
//---------------------------------------------------------------------------
TGameItem *TGameWorld::FindWorldItem(DWORD serial)
{
	TGameItem *result = NULL;

	WORLD_MAP::iterator i = m_Map.find(serial);
	if (i != m_Map.end() && !((*i).second)->NPC)
		result = (TGameItem*)(*i).second;

	return result;
}
//---------------------------------------------------------------------------
TGameCharacter *TGameWorld::FindWorldCharacter(DWORD serial)
{
	TGameCharacter *result = NULL;

	WORLD_MAP::iterator i = m_Map.find(serial);
	if (i != m_Map.end() && ((*i).second)->NPC)
		result = (TGameCharacter*)(*i).second;

	return result;
}
//---------------------------------------------------------------------------
TGameCharacter *TGameWorld::FindWorldCorpseOwner(DWORD serial)
{
	TGameObject *item = NULL;

	if (serial)
	{
		for (item = (TGameObject*)m_Items; item != NULL; item = (TGameObject*)item->m_Next)
		{
			if (item->NPC && ((TGameCharacter*)item)->CorpseLink == serial)
				break;
		}
	}

	return (TGameCharacter*)item;
}
//---------------------------------------------------------------------------
void TGameWorld::RemoveObject(TGameObject *obj)
{
	RemoveFromContainer(obj);

	DWORD serial = obj->Serial;
	m_Map[serial] = NULL;
	m_Map.erase(serial);
	delete obj;
}
//---------------------------------------------------------------------------
void TGameWorld::RemoveFromContainer(TGameObject *obj)
{
	if (obj->Container != 0xFFFFFFFF)
	{
		TGameObject *container = FindWorldObject(obj->Container);
		if (container != NULL)
			container->Reject(obj);
		else
			obj->Container = 0xFFFFFFFF;
	}
	else
	{
		if (m_Items != NULL)
		{
			if (m_Items->Serial == obj->Serial)
			{
				m_Items = (TGameObject*)m_Items->m_Next;
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
void TGameWorld::ClearContainer(TGameObject *obj)
{
	if (!obj->Empty())
		obj->Clear();
}
//---------------------------------------------------------------------------
void TGameWorld::PutContainer(TGameObject *obj, TGameObject *container)
{
	RemoveFromContainer(obj);
	container->AddItem(obj);
}
//---------------------------------------------------------------------------
void TGameWorld::MoveToTop(TGameObject *obj)
{
	if (obj == NULL)
		return;

	if (obj->Container == 0xFFFFFFFF)
		MapManager->AddRender(obj);

	if (obj->m_Next == NULL)
		return;

	if (obj->Container == 0xFFFFFFFF)
	{
		if (obj->m_Prev == NULL)
		{
			m_Items = (TGameObject*)obj->m_Next;
			m_Items->m_Prev = NULL;

			TGameObject *item = m_Items;

			while (item != NULL)
			{
				if (item->m_Next == NULL)
				{
					item->m_Next = obj;
					obj->m_Prev = item;
					obj->m_Next = NULL;

					break;
				}

				item = (TGameObject*)item->m_Next;
			}
		}
		else
		{
			TGameObject *item = (TGameObject*)obj->m_Next;

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

				item = (TGameObject*)item->m_Next;
			}
		}
	}
	else
	{
		TGameObject *container = FindWorldObject(obj->Container);

		if (container == NULL)
			return;
		
		if (obj->m_Prev == NULL)
		{
			container->m_Items = obj->m_Next;
			container->m_Items->m_Prev = NULL;

			TGameObject *item = (TGameObject*)container->m_Items;

			while (item != NULL)
			{
				if (item->m_Next == NULL)
				{
					item->m_Next = obj;
					obj->m_Prev = item;
					obj->m_Next = NULL;

					break;
				}

				item = (TGameObject*)item->m_Next;
			}
		}
		else
		{
			TGameObject *item = (TGameObject*)obj->m_Next;

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

				item = (TGameObject*)item->m_Next;
			}
		}
	}
}
//---------------------------------------------------------------------------
void TGameWorld::Dump(BYTE tCount, DWORD serial)
{
	trace_printf("World Dump:\n\n");

	TGameObject *obj = m_Items;

	if (serial != 0xFFFFFFFF)
	{
		obj = FindWorldObject(serial);
		if (obj != NULL)
			obj = (TGameObject*)obj->m_Items;
	}

	while (obj != NULL)
	{
		if (obj->Container == serial)
		{
			if (obj->Serial == g_Player->Serial)
				trace_printf("---Player---\n");

			IFOR(i, 0, tCount)
				trace_printf("\t");

			trace_printf("%s%08X:%04X[%04X](%%02X)*%i\tin 0x%08X XYZ=%i,%i,%i on Map %i\n",
				(obj->NPC ? "NPC: " : "Item: "), obj->Serial,
				obj->Graphic, obj->Color, /*obj->Layer,*/
				obj->Count, obj->Container, obj->X,
				obj->Y, obj->Z, obj->MapIndex);

			if (obj->m_Items != NULL)
				Dump(tCount + 1, obj->Container);
		}

		obj = (TGameObject*)obj->m_Next;
	}
}
//---------------------------------------------------------------------------