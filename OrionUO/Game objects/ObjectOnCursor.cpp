/***********************************************************************************
**
** ObjectOnCursor.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "ObjectOnCursor.h"
#include "../OrionUO.h"
//----------------------------------------------------------------------------------
CObjectOnCursor *g_ObjectInHand = NULL;
//----------------------------------------------------------------------------------
CObjectOnCursor::CObjectOnCursor()
: CGameItem(), m_Separated(false), m_Deleted(false), m_Dropped(false),
m_IsGameFigure(false), m_DragCount(1), m_NoDraw(false)
{
}
//----------------------------------------------------------------------------------
CObjectOnCursor::CObjectOnCursor(CGameItem *obj)
: CGameItem(), m_Separated(false), m_Deleted(false), m_Dropped(false),
m_IsGameFigure(false), m_DragCount(obj->Count), m_NoDraw(false)
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
	m_Container = obj->Container;
	m_UsedLayer = obj->UsedLayer;
	m_AnimID = obj->AnimID;
	m_MapIndex = obj->MapIndex;
	m_Name = obj->Name;

	if (m_Graphic < g_Orion.StaticDataCount)
		m_TiledataPtr = &g_Orion.m_StaticData[m_Graphic / 32].Tiles[m_Graphic % 32];
}
//----------------------------------------------------------------------------------
