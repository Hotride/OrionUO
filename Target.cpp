/****************************************************************************
**
** Target.cpp
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

TTarget Target;
//---------------------------------------------------------------------------
TTarget::TTarget()
: m_Type(0), m_Targeting(false), m_MultiGraphic(0), m_Multi(NULL), m_CursorID(0)
{
	//Чистимся
	memset(m_Data, 0, sizeof(m_Data));
	memset(m_LastData, 0, sizeof(m_LastData));
}
//---------------------------------------------------------------------------
void TTarget::Reset()
{
	//Чистимся
	memset(m_Data, 0, sizeof(m_Data));
	memset(m_LastData, 0, sizeof(m_LastData));

	if (m_Multi != NULL)
	{
		delete m_Multi;
		m_Multi = NULL;
	}

	m_Type = 0;
	m_CursorType = 0;
	m_CursorID = 0;
	m_Targeting = false;
	m_MultiGraphic = 0;
}
//---------------------------------------------------------------------------
void TTarget::SetData(PBYTE buf, int size)
{
	//Копируем буффер
	memcpy(m_Data, buf, size);

	//И устанавливаем соответствующие значения
	m_Type = buf[1];
	m_CursorType = buf[6];
	m_CursorID = unpack32(buf + 2);
	m_Targeting = true;
	m_MultiGraphic = false;
}
//---------------------------------------------------------------------------
void TTarget::SetMultiData(PBYTE buf, int size)
{
	//Копируем буффер
	memset(&m_Data[0], 0, 19);
	m_Data[0] = 0x6C;
	m_Data[1] = 1; //Таргет на ландшафт
	memcpy(m_Data + 2, buf + 2, 4); //Копируем ID курсора (ID дида)
	m_MultiGraphic = unpack16(buf + 18);

	//И устанавливаем соответствующие значения
	m_Type = 1;
	m_CursorType = 0;
	m_Targeting = true;
	m_CursorID = unpack32(buf + 2);
}
//---------------------------------------------------------------------------
void TTarget::SendTargetObject(DWORD Serial)
{
	if (!m_Targeting)
		return; //Если в клиенте нет таргета - выход

	g_LastTargetObject = Serial;

	//Пишем серийник объекта, на который ткнули прицелом, остальное - затираем
	pack32(m_Data + 7, Serial);
	pack32(m_Data + 11, 0);
	pack32(m_Data + 15, 0);

	//Скопируем для LastTarget
	memcpy(m_LastData, m_Data, sizeof(m_Data));

	SendTarget();
}
//---------------------------------------------------------------------------
void TTarget::SendTargetTile(WORD tileID, WORD X, WORD Y, char Z)
{
	if (!m_Targeting)
		return; //Если в клиенте нет таргета - выход

	//Пишем координаты и индекс тайла, на который ткнули, остальное трем
	pack32(m_Data + 7, 0);
	pack16(m_Data + 11, X);
	pack16(m_Data + 13, Y);
	m_Data[15] = 0x00;

	if (m_MultiGraphic != 0)
	{
		int grZ = 0;
		int stZ = 0;
		MapManager->GetMapZ(X, Y, grZ, stZ);
		Z = grZ;
	}

	m_Data[16] = Z;
	pack16(m_Data + 17, tileID);

	//Скопируем для LastTarget
	memcpy(m_LastData, m_Data, sizeof(m_Data));

	SendTarget();
}
//---------------------------------------------------------------------------
void TTarget::SendCancelTarget()
{
	if (!m_Targeting)
		return; //Если в клиенте нет таргета - выход

	//Уходят только нули
	pack32(m_Data + 7, 0);
	pack32(m_Data + 11, 0xFFFFFFFF);
	pack32(m_Data + 15, 0);

	SendTarget();
}
//---------------------------------------------------------------------------
void TTarget::SendLastTarget()
{
	if (!m_Targeting)
		return; //Если в клиенте нет таргета - выход

	//Восстановим пакет последнего актуального таргета
	memcpy(m_Data, m_LastData, sizeof(m_Data));
	m_Data[1] = m_Type;
	m_Data[6] = m_CursorType;
	pack32(m_Data + 2, m_CursorID);

	SendTarget();
}
//---------------------------------------------------------------------------
void TTarget::SendTarget()
{
	UO->Send(m_Data, sizeof(m_Data));

	//Чистим данные
	memset(m_Data, 0, sizeof(m_Data));
	m_Targeting = false;
	m_MultiGraphic = 0;
}
//---------------------------------------------------------------------------
void TTarget::UnloadMulti()
{
	if (m_Multi != NULL)
	{
		delete m_Multi;
		m_Multi = NULL;
	}
}
//---------------------------------------------------------------------------
void TTarget::LoadMulti(int x, int y, char z)
{
	TIndexMulti *index = UO->GetMultiPointer(m_MultiGraphic);
	
	if (index != NULL && index->Address != NULL)
	{
		DWORD address = index->Address;

		int count = index->Count;
		
		IFOR(j, 0, count)
		{
			PMULTI_BLOCK pmb = (PMULTI_BLOCK)(address + (j * sizeof(MULTI_BLOCK)));
			
			TMultiObject *mo = new TMultiObject(pmb->ID + 0x4000, x + pmb->X, y + pmb->Y, z + (char)pmb->Z, 2);
			MapManager->AddRender(mo);
			AddMultiObject(mo);
		}
	}
}
//---------------------------------------------------------------------------
void TTarget::AddMultiObject(TMultiObject *obj)
{
	if (m_Multi == NULL)
	{
		m_Multi = new TMulti(obj->X, obj->Y);
		m_Multi->m_Next = NULL;
		m_Multi->m_Items = obj;
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

			multi = m_Multi;

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
TMulti *TTarget::GetMultiAtXY(short x, short y)
{
	TMulti *multi = m_Multi;

	while (multi != NULL)
	{
		if (multi->X == x && multi->Y == y)
			return multi;

		multi = (TMulti*)multi->m_Next;
	}

	return multi;
}
//---------------------------------------------------------------------------