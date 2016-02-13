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
	Serial = obj->Serial;
	Graphic = obj->Graphic;
	Color = obj->Color;
	X = obj->X;
	Y = obj->Y;
	Z = obj->Z;
	Count = obj->Count;
	Layer = obj->Layer;
	Flags = obj->Flags;
	NPC = obj->NPC;
	ImageID = obj->ImageID;
	Container = obj->Container;
	UsedLayer = obj->UsedLayer;
	AnimID = obj->AnimID;
	MapIndex = obj->MapIndex;
	Name = obj->GetName();
}
//---------------------------------------------------------------------------