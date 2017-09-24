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
	m_Enabled = false;
	m_Serial = 0;
	m_Graphic = 0;
	m_Color = 0;
	m_Count = 0;
	m_TotalCount = 0;
	m_Layer = 0;
	m_Flags = 0;
	m_Container = 0;
	m_IsGameFigure = false;
	m_TiledataPtr = NULL;
	m_UpdatedInWorld = false;
}
//----------------------------------------------------------------------------------
ushort CObjectOnCursor::GetDrawGraphic(bool &doubleDraw)
{
	WISPFUN_DEBUG("c20_f15");
	int index = CGameObject::IsGold(m_Graphic);
	ushort result = m_Graphic;

	const ushort graphicAssociateTable[3][3] =
	{
		{ 0x0EED, 0x0EEE, 0x0EEF },
		{ 0x0EEA, 0x0EEB, 0x0EEC },
		{ 0x0EF0, 0x0EF1, 0x0EF2 }
	};

	if (index)
	{
		int graphicIndex = (int)(m_Count > 1) + (int)(m_Count > 5);
		result = graphicAssociateTable[index - 1][graphicIndex];
	}
	else
		doubleDraw = IsStackable(m_TiledataPtr->Flags) && (m_Count > 1);

	return result;
}
//----------------------------------------------------------------------------------
