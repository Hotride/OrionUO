﻿// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** ObjectOnCursor.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CObjectOnCursor g_ObjectInHand;
//----------------------------------------------------------------------------------
void CObjectOnCursor::Clear()
{
	Enabled = false;
	Dropped = false;
	Serial = 0;
	Graphic = 0;
	Color = 0;
	Count = 0;
	TotalCount = 0;
	Layer = 0;
	Flags = 0;
	Container = 0;
	IsGameFigure = false;
	TiledataPtr = NULL;
	UpdatedInWorld = false;
}
//----------------------------------------------------------------------------------
ushort CObjectOnCursor::GetDrawGraphic(bool &doubleDraw)
{
	WISPFUN_DEBUG("c20_f15");
	int index = CGameObject::IsGold(Graphic);
	ushort result = Graphic;

	const ushort graphicAssociateTable[3][3] =
	{
		{ 0x0EED, 0x0EEE, 0x0EEF },
		{ 0x0EEA, 0x0EEB, 0x0EEC },
		{ 0x0EF0, 0x0EF1, 0x0EF2 }
	};

	if (index)
	{
		int graphicIndex = (int)(Count > 1) + (int)(Count > 5);
		result = graphicAssociateTable[index - 1][graphicIndex];
	}
	else
		doubleDraw = IsStackable(TiledataPtr->Flags) && (Count > 1);

	return result;
}
//----------------------------------------------------------------------------------
