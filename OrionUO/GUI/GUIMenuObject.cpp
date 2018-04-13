// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIMenuObject.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIMenuObject::CGUIMenuObject(const uint &serial, const ushort &graphic, const ushort &color, const int &x, const int &y, const string &text)
: CGUITilepic(graphic, color, x, y), Text(text)
{
	Serial = serial;
	MoveOnDrag = true;
}
//----------------------------------------------------------------------------------
CGUIMenuObject::~CGUIMenuObject()
{
}
//----------------------------------------------------------------------------------
bool CGUIMenuObject::Select()
{
	WISPFUN_DEBUG("c66_f1");
	int x = g_MouseManager.Position.X - m_X;
	int y = g_MouseManager.Position.Y - m_Y;

	WISP_GEOMETRY::CSize size = g_Orion.GetStaticArtDimension(Graphic);

	return (x >= 0 && y >= 0 && x < size.Width && y < size.Height);
}
//----------------------------------------------------------------------------------
void CGUIMenuObject::OnMouseEnter()
{
	WISPFUN_DEBUG("c66_f2");
	if (g_SelectedObject.Gump != NULL && g_SelectedObject.Gump->GumpType == GT_MENU)
	{
		CGumpMenu *menu = (CGumpMenu*)g_SelectedObject.Gump;

		if (menu->Text != Text)
		{
			menu->Text = Text;
			menu->TextChanged = true;
		}
	}
}
//----------------------------------------------------------------------------------
void CGUIMenuObject::OnMouseExit()
{
	WISPFUN_DEBUG("c66_f3");
	if (g_LastSelectedObject.Gump != NULL && g_LastSelectedObject.Gump->GumpType == GT_MENU)
	{
		CGumpMenu *menu = (CGumpMenu*)g_LastSelectedObject.Gump;

		if (menu->Text == Text)
		{
			menu->Text = "";
			menu->TextChanged = true;
		}
	}
}
//----------------------------------------------------------------------------------
