// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIPolygonal.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIPolygonal::CGUIPolygonal(GUMP_OBJECT_TYPE type, int x, int y, int width, int height, bool callOnMouseUp)
: CBaseGUI(type, 0, 0, 0, x, y), Width(width), Height(height),
CallOnMouseUp(callOnMouseUp)
{
}
//----------------------------------------------------------------------------------
CGUIPolygonal::~CGUIPolygonal()
{
}
//----------------------------------------------------------------------------------
bool CGUIPolygonal::Select()
{
	WISPFUN_DEBUG("c68_f1");
	int x = g_MouseManager.Position.X - m_X;
	int y = g_MouseManager.Position.Y - m_Y;

	return (x >= 0 && y >= 0 && x < Width && y < Height);
}
//----------------------------------------------------------------------------------
