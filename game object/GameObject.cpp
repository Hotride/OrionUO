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
TGameObject::TGameObject(DWORD serial)
: TRenderStaticObject(ROT_GAME_OBJECT, serial, 0, 0, 0, 0, 0), m_Container(0xFFFFFFFF),
m_MapIndex(0), m_Count(0), m_Flags(0), m_Name(""), m_NPC(false), m_Clicked(false),
m_Effects(NULL), m_AnimIndex(0), m_LastAnimationChangeTime(GetTickCount())
{
#if UO_DEBUG_INFO!=0
	g_GameObjectsCount++;
#endif //UO_DEBUG_INFO!=0
}
//---------------------------------------------------------------------------
TGameObject::~TGameObject()
{
	if (m_Effects != NULL)
	{
		delete m_Effects;
		m_Effects = NULL;
	}
	
	m_Next = NULL;
	m_Prev = NULL;
	
	Clear();

#if UO_DEBUG_INFO!=0
	g_GameObjectsCount--;
#endif //UO_DEBUG_INFO!=0
}
//---------------------------------------------------------------------------
void TGameObject::AddText(TTextData *msg)
{
	m_TextControl->Add(msg);

	string msgname = "You see: ";

	if (m_Clicked)
	{
		m_Clicked = false;

		if (IsPlayer()) //(m_NPC)
			msgname = m_Name + ": ";
	}

	UO->AddJournalMessage(msg, msgname);
}
//---------------------------------------------------------------------------
WORD TGameObject::GetMountAnimation()
{
	return m_Graphic; // + UO->GetStaticPointer(m_Graphic)->Increment;
}
//---------------------------------------------------------------------------
void TGameObject::Clear()
{
	if (!Empty())
	{
		TGameObject *obj = (TGameObject*)m_Items;

		while (obj != NULL)
		{
			TGameObject *next = (TGameObject*)obj->m_Next;

			World->RemoveObject(obj);

			obj = next;
		}

		m_Items = NULL;
	}
}
//---------------------------------------------------------------------------
int TGameObject::IsGold()
{
	switch (m_Graphic)
	{
		case 0x0EED:
			return 1;
		case 0x0EEA:
			return 2;
		case 0x0EF0:
			return 3;
		default:
			break;
	}

	return 0;
}
//---------------------------------------------------------------------------
WORD TGameObject::GetDrawGraphic(bool &doubleDraw)
{
	int index = IsGold();
	WORD result = m_Graphic;

	const WORD graphicAccociateTable[3][3] =
	{
		{0x0EED, 0x0EEE, 0x0EEF},
		{0x0EEA, 0x0EEB, 0x0EEC},
		{0x0EF0, 0x0EF1, 0x0EF2}
	};

	if (index)
	{
		int graphicIndex = (int)(m_Count > 1) + (int)(m_Count > 5);
		result = graphicAccociateTable[index - 1][graphicIndex];
	}
	else
		doubleDraw = IsStackable();

	return result;
}
//---------------------------------------------------------------------------
void TGameObject::DrawEffects(int &drawX, int &drawY, DWORD &ticks)
{
	TGameEffect *effect = m_Effects;
	char gZ = Z;
	char z = gZ * 4;

	int x = drawX;
	int y = drawY;

	if (NPC)
	{
		TGameCharacter *gc = (TGameCharacter*)this;
		x += gc->OffsetX;
		y += gc->OffsetY;
	}

	while (effect != NULL)
	{
		TGameEffect *next = (TGameEffect*)effect->m_Next;
		
		WORD eGraphic = 0;
		WORD gGraphic = 0;

		if (effect->Duration < ticks)
			RemoveEffect(effect);
		else if (effect->LastChangeFrameTime < ticks)
		{
			effect->LastChangeFrameTime = ticks + effect->Speed;

			if (effect->EffectType == EF_LIGHTING) //lighting
			{
				int animIndex = effect->AnimIndex;
				
				gGraphic = 0x4E20 + animIndex;

				animIndex++;
				if (animIndex >= 10)
					RemoveEffect(effect);
				else
					effect->AnimIndex = animIndex;
			}
			else if (effect->EffectType == EF_STAY_AT_SOURCE)
				eGraphic = effect->CalculateCurrentGraphic();
		}
		else
		{
			if (effect->EffectType == EF_LIGHTING) //lighting
				gGraphic = 0x4E20 + effect->AnimIndex;
			else if (effect->EffectType == EF_STAY_AT_SOURCE)
				eGraphic = effect->GetCurrentGraphic();
		}
		
		if (gGraphic != 0)
		{
			POINT p = {0, 0};
			UO->GetGumpDimension(gGraphic, p);

			UO->DrawGump(gGraphic, effect->Color, x - (p.x / 2), (y - p.y) - z);
		}
		else if (eGraphic != 0)
		{
			effect->ApplyRenderMode();

			UO->DrawStaticArt(eGraphic, effect->Color, x, y, gZ);

			glDisable(GL_BLEND);
		}

		effect = next;
	}
}
//---------------------------------------------------------------------------
void TGameObject::AddEffect(TGameEffect *effect)
{
	if (m_Effects == NULL)
	{
		m_Effects = effect;
		effect->m_Next = NULL;
		effect->m_Prev = NULL;
	}
	else
	{
		effect->m_Next = m_Effects;
		m_Effects->m_Prev = effect;
		effect->m_Prev = NULL;
		m_Effects = effect;
	}
}
//---------------------------------------------------------------------------
void TGameObject::RemoveEffect(TGameEffect *effect)
{
	if (effect->m_Prev == NULL)
	{
		m_Effects = (TGameEffect*)effect->m_Next;

		if (m_Effects != NULL)
			m_Effects->m_Prev = NULL;
	}
	else
	{
		effect->m_Prev->m_Next = effect->m_Next;

		if (effect->m_Next != NULL)
			effect->m_Next->m_Prev = effect->m_Prev;
	}
	
	effect->m_Next = NULL;
	effect->m_Prev = NULL;
	delete effect;
}
//---------------------------------------------------------------------------
void TGameObject::AddObject(TGameObject *obj)
{
	World->RemoveFromContainer(obj);

	if (m_Next == NULL)
	{
		m_Next = obj;
		m_Next->m_Prev = this;
		m_Next->m_Next = NULL;

		((TGameObject*)m_Next)->Container = m_Container;
	}
	else
	{
		TGameObject *item = (TGameObject*)m_Next;
		while (item->m_Next != NULL)
			item = (TGameObject*)item->m_Next;

		item->m_Next = obj;
		obj->m_Next = NULL;
		obj->m_Prev = item;

		obj->Container = m_Container;
	}
}
//---------------------------------------------------------------------------
void TGameObject::AddItem(TGameObject *obj)
{
	if (obj->Container != 0xFFFFFFFF)
		return;

	World->RemoveFromContainer(obj);

	if (m_Items != NULL)
	{
		TGameObject *item = (TGameObject*)m_Items;
		while (item->m_Next != NULL)
			item = (TGameObject*)item->m_Next;

		item->m_Next = obj;
		obj->m_Next = NULL;
		obj->m_Prev = item;
	}
	else
	{
		m_Items = obj;
		m_Items->m_Next = NULL;
		m_Items->m_Prev = NULL;
	}

	obj->Container = Serial;
}
//---------------------------------------------------------------------------
void TGameObject::Reject(TGameObject *obj)
{
	if (obj->Container != Serial)
		return;

	if (m_Items != NULL)
	{
		if (((TGameObject*)m_Items)->Serial == obj->Serial)
		{
			if (m_Items->m_Next != NULL)
			{
				m_Items = m_Items->m_Next;
				m_Items->m_Prev = NULL;
			}
			else
				m_Items = NULL;
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

	obj->m_Next = NULL;
	obj->m_Prev = NULL;
	obj->Container = 0xFFFFFFFF;
}
//---------------------------------------------------------------------------
TGameObject *TGameObject::GetTopObject()
{
	TGameObject *obj = this;

	while (obj->Container != 0xFFFFFFFF)
		obj = World->FindWorldObject(obj->Container);

	return obj;
}
//---------------------------------------------------------------------------
int GetDistance(TGameObject *current, TGameObject *target)
{
	if (current != NULL && target != NULL)
	{
		int distx = abs(target->X - current->X);
		int disty = abs(target->Y - current->Y);

		if (disty > distx)
			distx = disty;

		return distx;
	}

	return 100500;
}
//---------------------------------------------------------------------------
int GetDistance(TGameObject *current, POINT target)
{
	if (current != NULL)
	{
		int distx = abs(target.x - current->X);
		int disty = abs(target.y - current->Y);

		if (disty > distx)
			distx = disty;

		return distx;
	}

	return 100500;
}
//---------------------------------------------------------------------------
int GetDistance(POINT current, TGameObject *target)
{
	if (target != NULL)
	{
		int distx = abs(target->X - current.x);
		int disty = abs(target->Y - current.y);

		if (disty > distx)
			distx = disty;

		return distx;
	}

	return 100500;
}
//---------------------------------------------------------------------------
int GetDistance(POINT current, POINT target)
{
	int distx = abs(target.x - current.x);
	int disty = abs(target.y - current.y);

	if (disty > distx)
		distx = disty;

	return distx;
}
//---------------------------------------------------------------------------
int GetTopObjDistance(TGameObject *current, TGameObject *target)
{
	if (current != NULL && target != NULL)
	{
		while (target != NULL && target->Container != 0xFFFFFFFF)
			target = World->FindWorldObject(target->Container);

		if (target != NULL)
		{
			int distx = abs(target->X - current->X);
			int disty = abs(target->Y - current->Y);

			if (disty > distx)
				distx = disty;

			return distx; 
		}
	}

	return 100500;
}
//---------------------------------------------------------------------------