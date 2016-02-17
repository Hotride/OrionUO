/****************************************************************************
**
** ObjectOnCursor.h
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

TObjectOnCursor *ObjectInHand = NULL;
//---------------------------------------------------------------------------
TObjectOnCursor::TObjectOnCursor()
: TGameItem(), m_Separated(false), m_Deleted(false), m_Dropped(false),
m_IsGameFigure(false), m_DragCount(1)
{
	Name = "";
}
//---------------------------------------------------------------------------
TObjectOnCursor::TObjectOnCursor(TGameItem *obj)
: TGameItem(), m_Separated(false), m_Deleted(false), m_Dropped(false),
m_IsGameFigure(false), m_DragCount(obj->Count)
{
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
	m_AnimID = obj->AnimID;
	m_MapIndex = obj->MapIndex;
	m_Name = obj->GetName();

	m_TiledataPtr = &UO->m_StaticData[m_Graphic / 32].Tiles[m_Graphic % 32];
}
//---------------------------------------------------------------------------