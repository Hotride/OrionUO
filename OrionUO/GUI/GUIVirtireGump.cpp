// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIVirtureGump.cpp
**
** Copyright (C) May 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIVirtureGump::CGUIVirtureGump(ushort graphic, int x, int y)
: CGUIDrawObject(GOT_VIRTURE_GUMP, graphic, graphic, 0, x, y)
{
}
//----------------------------------------------------------------------------------
CGUIVirtureGump::~CGUIVirtureGump()
{
}
//----------------------------------------------------------------------------------
bool CGUIVirtureGump::Select()
{
	bool select = false;

	CGLTexture *th = g_Orion.ExecuteGump(Graphic);

	if (th != NULL)
	{
		int x = g_MouseManager.Position.X - m_X;
		int y = g_MouseManager.Position.Y - m_Y;

		select = (x >= 0 && y >= 0 && x < th->Width && y < th->Height);
	}

	return select;
}
//----------------------------------------------------------------------------------
