// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** Target.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CTarget g_Target;
//----------------------------------------------------------------------------------
CTarget::CTarget()
{
	//Чистимся
	memset(m_Data, 0, sizeof(m_Data));
	memset(m_LastData, 0, sizeof(m_LastData));
}
//----------------------------------------------------------------------------------
void CTarget::Reset()
{
	WISPFUN_DEBUG("c209_f1");
	//Чистимся
	memset(m_Data, 0, sizeof(m_Data));
	memset(m_LastData, 0, sizeof(m_LastData));

	if (m_Multi != NULL)
	{
		delete m_Multi;
		m_Multi = NULL;
	}

	Type = 0;
	CursorType = 0;
	CursorID = 0;
	Targeting = false;
	MultiGraphic = 0;
}
//----------------------------------------------------------------------------------
void CTarget::RequestFromCustomHouse()
{
	Type = 2;
	CursorID = 0;
	CursorType = 0;
	Targeting = true;
	MultiGraphic = 0;

	if (g_CustomHouseGump != NULL)
	{
		g_CustomHouseGump->Erasing = false;
		g_CustomHouseGump->SeekTile = false;
		g_CustomHouseGump->SelectedGraphic = 0;
		g_CustomHouseGump->CombinedStair = false;
		g_CustomHouseGump->WantUpdateContent = true;
	}
}
//----------------------------------------------------------------------------------
void CTarget::SetLastTargetObject(int serial)
{
	Type = 0;
	pack32(m_LastData + 7, serial);
}
//----------------------------------------------------------------------------------
void CTarget::SetData(WISP_DATASTREAM::CDataReader &reader)
{
	WISPFUN_DEBUG("c209_f2");
	//Копируем буффер
	memcpy(&m_Data[0], reader.Start, reader.Size);

	//И устанавливаем соответствующие значения
	Type = reader.ReadUInt8();
	CursorID = reader.ReadUInt32BE();
	CursorType = reader.ReadUInt8();
	Targeting = (CursorType < 3);
	MultiGraphic = 0;
}
//----------------------------------------------------------------------------------
void CTarget::SetMultiData(WISP_DATASTREAM::CDataReader &reader)
{
	WISPFUN_DEBUG("c209_f3");
	//Устанавливаем соответствующие значения
	Type = 1;
	CursorType = 0;
	Targeting = true;
	CursorID = reader.ReadUInt32BE(1);

	//Копируем буффер
	memset(&m_Data[0], 0, 19);
	m_Data[0] = 0x6C;
	m_Data[1] = 1; //Таргет на ландшафт
	memcpy(m_Data + 2, reader.Start + 2, 4); //Копируем ID курсора (ID дида)

	reader.ResetPtr();
	reader.Move(18);
	MultiGraphic = reader.ReadUInt16BE() + 1;
	MultiX = reader.ReadUInt16BE();
	MultiY = reader.ReadUInt16BE();
}
//----------------------------------------------------------------------------------
void CTarget::SendTargetObject(int serial)
{
	WISPFUN_DEBUG("c209_f4");
	if (!Targeting)
		return; //Если в клиенте нет таргета - выход

	//Пишем серийник объекта, на который ткнули прицелом, остальное - затираем
	pack32(m_Data + 7, serial);
	m_Data[1] = 0;

	CGameObject *obj = (g_World != NULL ? g_World->FindWorldObject(serial) : NULL);

	if (obj != NULL)
	{
		pack16(m_Data + 11, obj->GetX());
		pack16(m_Data + 13, obj->GetY());
		m_Data[15] = 0xFF;
		m_Data[16] = obj->GetZ();
		pack16(m_Data + 17, obj->Graphic);
	}
	else
	{
		pack32(m_Data + 11, 0);
		pack32(m_Data + 15, 0);
	}

	if (serial != g_PlayerSerial)
	{
		g_LastTargetObject = serial;

		//Скопируем для LastTarget
		memcpy(m_LastData, m_Data, sizeof(m_Data));

		if (obj != NULL && obj->NPC && ((CGameCharacter*)obj)->MaxHits == 0)
			CPacketStatusRequest(serial).Send();
	}

	SendTarget();
}
//----------------------------------------------------------------------------------
void CTarget::SendTargetTile(ushort tileID, short x, short y, char z)
{
	WISPFUN_DEBUG("c209_f5");
	if (!Targeting)
		return; //Если в клиенте нет таргета - выход

	m_Data[1] = 1;

	//Пишем координаты и индекс тайла, на который ткнули, остальное трем
	pack32(m_Data + 7, 0);
	pack16(m_Data + 11, x);
	pack16(m_Data + 13, y);

	//m_Data[15] = 0xFF;
	//m_Data[16] = z;
	pack16(m_Data + 15, (short)z);
	pack16(m_Data + 17, tileID);

	//Скопируем для LastTarget
	memcpy(m_LastData, m_Data, sizeof(m_Data));

	SendTarget();
}
//----------------------------------------------------------------------------------
void CTarget::SendCancelTarget()
{
	WISPFUN_DEBUG("c209_f6");
	if (!Targeting)
		return; //Если в клиенте нет таргета - выход

	//Уходят только нули
	pack32(m_Data + 7, 0);
	pack32(m_Data + 11, 0xFFFFFFFF);
	pack32(m_Data + 15, 0);

	SendTarget();

	if (g_CustomHouseGump != NULL)
	{
		g_CustomHouseGump->Erasing = false;
		g_CustomHouseGump->SeekTile = false;
		g_CustomHouseGump->SelectedGraphic = 0;
		g_CustomHouseGump->CombinedStair = false;
		g_CustomHouseGump->WantUpdateContent = true;
	}
}
//----------------------------------------------------------------------------------
void CTarget::Plugin_SendTargetObject(int serial)
{
	WISPFUN_DEBUG("c209_f4");
	if (!Targeting)
		return; //Если в клиенте нет таргета - выход

	//Пишем серийник объекта, на который ткнули прицелом, остальное - затираем
	pack32(m_Data + 7, serial);
	m_Data[1] = 0;

	CGameObject *obj = (g_World != NULL ? g_World->FindWorldObject(serial) : NULL);

	if (obj != NULL)
	{
		pack16(m_Data + 11, obj->GetX());
		pack16(m_Data + 13, obj->GetY());
		m_Data[15] = 0xFF;
		m_Data[16] = obj->GetZ();
		pack16(m_Data + 17, obj->Graphic);
	}
	else
	{
		pack32(m_Data + 11, 0);
		pack32(m_Data + 15, 0);
	}

	if (serial != g_PlayerSerial)
	{
		g_LastTargetObject = serial;

		//Скопируем для LastTarget
		memcpy(m_LastData, m_Data, sizeof(m_Data));

		if (obj != NULL && obj->NPC && ((CGameCharacter*)obj)->MaxHits == 0)
		{
			CPacketStatusRequest packet(serial);
			SendMessage(g_OrionWindow.Handle, UOMSG_SEND, (WPARAM)packet.Data().data(), packet.Data().size());
		}
	}

	Plugin_SendTarget();
}
//----------------------------------------------------------------------------------
void CTarget::Plugin_SendTargetTile(ushort tileID, short x, short y, char z)
{
	WISPFUN_DEBUG("c209_f5");
	if (!Targeting)
		return; //Если в клиенте нет таргета - выход

	m_Data[1] = 1;

	//Пишем координаты и индекс тайла, на который ткнули, остальное трем
	pack32(m_Data + 7, 0);
	pack16(m_Data + 11, x);
	pack16(m_Data + 13, y);

	//m_Data[15] = 0xFF;
	//m_Data[16] = z;
	pack16(m_Data + 15, (short)z);
	pack16(m_Data + 17, tileID);

	//Скопируем для LastTarget
	memcpy(m_LastData, m_Data, sizeof(m_Data));

	Plugin_SendTarget();
}
//----------------------------------------------------------------------------------
void CTarget::Plugin_SendCancelTarget()
{
	WISPFUN_DEBUG("c209_f6");
	if (!Targeting)
		return; //Если в клиенте нет таргета - выход

	//Уходят только нули
	pack32(m_Data + 7, 0);
	pack32(m_Data + 11, 0xFFFFFFFF);
	pack32(m_Data + 15, 0);

	Plugin_SendTarget();
}
//----------------------------------------------------------------------------------
void CTarget::SendLastTarget()
{
	WISPFUN_DEBUG("c209_f7");
	if (!Targeting)
		return; //Если в клиенте нет таргета - выход

	//Восстановим пакет последнего актуального таргета
	memcpy(m_Data, m_LastData, sizeof(m_Data));
	m_Data[0] = 0x6C;
	m_Data[1] = Type;
	m_Data[6] = CursorType;
	pack32(m_Data + 2, CursorID);

	SendTarget();
}
//----------------------------------------------------------------------------------
void CTarget::SendTarget()
{
	WISPFUN_DEBUG("c209_f8");

	if (Type != 2)
		g_Orion.Send(m_Data, sizeof(m_Data));

	//Чистим данные
	memset(m_Data, 0, sizeof(m_Data));
	Targeting = false;
	MultiGraphic = 0;

	g_MouseManager.CancelDoubleClick = true;
}
//----------------------------------------------------------------------------------
void CTarget::Plugin_SendTarget()
{
	WISPFUN_DEBUG("c209_f8");
	SendMessage(g_OrionWindow.Handle, UOMSG_SEND, (WPARAM)m_Data, sizeof(m_Data));

	//Чистим данные
	memset(m_Data, 0, sizeof(m_Data));
	Targeting = false;
	MultiGraphic = 0;

	g_MouseManager.CancelDoubleClick = true;
}
//----------------------------------------------------------------------------------
void CTarget::UnloadMulti()
{
	WISPFUN_DEBUG("c209_f9");
	if (m_Multi != NULL)
	{
		delete m_Multi;
		m_Multi = NULL;
	}
}
//----------------------------------------------------------------------------------
void CTarget::LoadMulti(int offsetX, int offsetY, char offsetZ)
{
	WISPFUN_DEBUG("c209_f10");
	UnloadMulti();

	CIndexMulti &index = g_Orion.m_MultiDataIndex[MultiGraphic - 1];

	int count = (int)index.Count;

	if (index.UopBlock != NULL)
	{
		UCHAR_LIST data = g_FileManager.m_MultiCollection.GetData(*index.UopBlock);

		if (data.empty())
			return;

		WISP_DATASTREAM::CDataReader reader(&data[0], data.size());
		reader.Move(8); //ID + Count

		IFOR(i, 0, count)
		{
			ushort graphic = reader.ReadUInt16LE();
			short x = reader.ReadInt16LE();
			short y = reader.ReadInt16LE();
			short z = reader.ReadInt16LE();
			ushort flags = reader.ReadUInt16LE();
			uint clilocsCount = reader.ReadUInt32LE();

			if (clilocsCount)
				reader.Move(clilocsCount * 4);

			CMultiObject *mo = new CMultiObject(graphic, x + offsetX, y + offsetY, (char)z + (char)offsetZ, 2);
			g_MapManager.AddRender(mo);
			AddMultiObject(mo);
		}
	}
	else if (index.Address != NULL)
	{
		int itemOffset = sizeof(MULTI_BLOCK);

		if (g_PacketManager.GetClientVersion() >= CV_7090)
			itemOffset = sizeof(MULTI_BLOCK_NEW);

		IFOR(j, 0, count)
		{
			PMULTI_BLOCK pmb = (PMULTI_BLOCK)(index.Address + (j * itemOffset));

			CMultiObject *mo = new CMultiObject(pmb->ID, offsetX + pmb->X, offsetY + pmb->Y, offsetZ + (char)pmb->Z, 2);
			g_MapManager.AddRender(mo);
			AddMultiObject(mo);
		}
	}
}
//----------------------------------------------------------------------------------
void CTarget::AddMultiObject(CMultiObject *obj)
{
	WISPFUN_DEBUG("c209_f11");
	if (m_Multi == NULL)
	{
		m_Multi = new CMulti(obj->GetX(), obj->GetY());
		m_Multi->m_Next = NULL;
		m_Multi->m_Prev = NULL;
		m_Multi->m_Items = obj;
		obj->m_Next = NULL;
		obj->m_Prev = NULL;
	}
	else
	{
		CMulti *multi = GetMultiAtXY(obj->GetX(), obj->GetY());

		if (multi != NULL)
		{
			QFOR(multiobj, multi->m_Items, CMultiObject*)
			{
				if (obj->GetZ() < multiobj->GetZ())
				{
					if (multiobj->m_Prev == NULL)
						multi->Insert(multiobj->m_Prev, obj);
					else
						multi->Insert(multiobj, obj);

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
			CMulti *newmulti = new CMulti(obj->GetX(), obj->GetY());
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
					newmulti->m_Prev = multi;
					break;
				}

				multi = (CMulti*)multi->m_Next;
			}
		}
	}
}
//----------------------------------------------------------------------------------
CMulti *CTarget::GetMultiAtXY(short x, short y)
{
	WISPFUN_DEBUG("c209_f12");
	CMulti *multi = m_Multi;

	while (multi != NULL)
	{
		if (multi->X == x && multi->Y == y)
			break;

		multi = (CMulti*)multi->m_Next;
	}

	return multi;
}
//----------------------------------------------------------------------------------
