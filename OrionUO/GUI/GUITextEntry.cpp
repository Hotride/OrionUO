// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUITextEntry.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUITextEntry::CGUITextEntry(int serial, ushort color, ushort colorSelected, ushort colorFocused, int x, int y, int maxWidth, bool unicode, uchar font, TEXT_ALIGN_TYPE align, ushort textFlags, int maxLength)
: CBaseGUI(GOT_TEXTENTRY, serial, 0, color, x, y), ColorSelected(colorSelected),
ColorFocused(colorFocused), Unicode(unicode), Font(font), Align(align),
TextFlags(textFlags), m_Entry(maxLength, maxWidth, maxWidth)
{
}
//----------------------------------------------------------------------------------
CGUITextEntry::~CGUITextEntry()
{
	WISPFUN_DEBUG("c79_f1");
	m_Entry.Clear();
}
//----------------------------------------------------------------------------------
bool CGUITextEntry::EntryPointerHere()
{
	WISPFUN_DEBUG("c79_f2");
	return (g_EntryPointer == &m_Entry);
}
//----------------------------------------------------------------------------------
WISP_GEOMETRY::CSize CGUITextEntry::GetSize()
{
	WISPFUN_DEBUG("c79_f3");
	return WISP_GEOMETRY::CSize(m_Entry.m_Texture.Width, m_Entry.m_Texture.Height);
}
//----------------------------------------------------------------------------------
void CGUITextEntry::SetGlobalColor(bool use, int color, int selected, int focused)
{
	WISPFUN_DEBUG("c79_f4");
	UseGlobalColor = use;

	if (use)
	{
		GlobalColorR = GetRValue(color);
		GlobalColorG = GetGValue(color);
		GlobalColorB = GetBValue(color);
		GlobalColorA = color >> 24;

		if (!GlobalColorA)
			GlobalColorA = 0xFF;

		GlobalColorSelectedR = GetRValue(selected);
		GlobalColorSelectedG = GetGValue(selected);
		GlobalColorSelectedB = GetBValue(selected);
		GlobalColorSelectedA = selected >> 24;

		if (!GlobalColorSelectedA)
			GlobalColorSelectedA = 0xFF;

		GlobalColorFocusedR = GetRValue(focused);
		GlobalColorFocusedG = GetGValue(focused);
		GlobalColorFocusedB = GetBValue(focused);
		GlobalColorFocusedA = focused >> 24;

		if (!GlobalColorFocusedA)
			GlobalColorFocusedA = 0xFF;
	}
}
//----------------------------------------------------------------------------------
void CGUITextEntry::OnClick(CGump *gump, int x, int y)
{
	WISPFUN_DEBUG("c79_f5");
	m_Entry.OnClick(gump, Font, Unicode, x, y, Align, TextFlags);
}
//----------------------------------------------------------------------------------
void CGUITextEntry::OnMouseEnter()
{
	WISPFUN_DEBUG("c79_f6");
	if (g_SelectedObject.Gump != NULL)
		g_SelectedObject.Gump->WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGUITextEntry::OnMouseExit()
{
	WISPFUN_DEBUG("c79_f7");
	if (g_LastSelectedObject.Gump != NULL)
		g_LastSelectedObject.Gump->WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGUITextEntry::PrepareTextures()
{
	WISPFUN_DEBUG("c79_f8");
	ushort color = Color;

	if (!UseGlobalColor)
	{
		if (Focused || &m_Entry == g_EntryPointer)
			color = ColorFocused;
		else if (g_GumpSelectedElement == this || (CheckOnSerial && g_CurrentCheckGump != NULL && g_CurrentCheckGump == g_SelectedObject.Gump && Serial == g_SelectedObject.Serial && g_SelectedObject.Object != NULL && g_SelectedObject.Object->IsGUI()))
			color = ColorSelected;
	}

	if (color && Unicode)
		color++;

	if (Unicode)
		m_Entry.PrepareToDrawW(Font, color, Align, TextFlags);
	else
		m_Entry.PrepareToDrawA(Font, color, Align, TextFlags);
}
//----------------------------------------------------------------------------------
void CGUITextEntry::Draw(bool checktrans)
{
	WISPFUN_DEBUG("c79_f9");
	int y = m_Y;
	ushort color = Color;

	if (Focused || &m_Entry == g_EntryPointer)
	{
		if (UseGlobalColor)
			glColor4ub(GlobalColorFocusedR, GlobalColorFocusedG, GlobalColorFocusedB, GlobalColorFocusedA);
		else
			color = ColorFocused;

		y += FocusedOffsetY;
	}
	else if (g_GumpSelectedElement == this || (CheckOnSerial && g_CurrentCheckGump != NULL && g_CurrentCheckGump == g_SelectedObject.Gump && Serial == g_SelectedObject.Serial && g_SelectedObject.Object != NULL && g_SelectedObject.Object->IsGUI()))
	{
		if (UseGlobalColor)
			glColor4ub(GlobalColorSelectedR, GlobalColorSelectedG, GlobalColorSelectedB, GlobalColorSelectedA);
		else
			color = ColorSelected;
	}
	else if (UseGlobalColor)
		glColor4ub(GlobalColorR, GlobalColorG, GlobalColorB, GlobalColorA);

	if (color && Unicode)
		color++;

	if (checktrans)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (Unicode)
			m_Entry.DrawW(Font, color, m_X, y, Align, TextFlags);
		else
			m_Entry.DrawA(Font, color, m_X, y, Align, TextFlags);

		glDisable(GL_BLEND);

		glEnable(GL_STENCIL_TEST);

		if (Unicode)
			m_Entry.DrawW(Font, color, m_X, y, Align, TextFlags);
		else
			m_Entry.DrawA(Font, color, m_X, y, Align, TextFlags);

		glDisable(GL_STENCIL_TEST);
	}
	else
	{
		if (Unicode)
			m_Entry.DrawW(Font, color, m_X, y, Align, TextFlags);
		else
			m_Entry.DrawA(Font, color, m_X, y, Align, TextFlags);
	}

	if (UseGlobalColor)
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
//----------------------------------------------------------------------------------
bool CGUITextEntry::Select()
{
	WISPFUN_DEBUG("c79_f10");
	int x = g_MouseManager.Position.X - m_X;
	int y = g_MouseManager.Position.Y - m_Y;

	return (x >= 0 && y >= 0 && x < m_Entry.m_Texture.Width && y < m_Entry.m_Texture.Height);
}
//----------------------------------------------------------------------------------
