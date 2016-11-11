/***********************************************************************************
**
** GUITextEntry.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GUITextEntry.h"
#include "../Managers/MouseManager.h"
#include "../Managers/FontsManager.h"
#include "../SelectedObject.h"
//----------------------------------------------------------------------------------
CGUITextEntry::CGUITextEntry(const uint &serial, const ushort &color, const ushort &colorSelected, const ushort &colorFocused, const int &x, const int &y, const int &maxWidth, const bool &unicode, const uchar &font, const TEXT_ALIGN_TYPE &align, const ushort &textFlags, const int &maxLength)
: CBaseGUI(GOT_TEXTENTRY, serial, 0, color, x, y), m_ColorSelected(colorSelected),
m_ColorFocused(colorFocused), m_Unicode(unicode), m_Font(font), m_Align(align),
m_TextFlags(textFlags), m_CheckOnSerial(false), m_ReadOnly(false), m_Focused(false),
m_UseGlobalColor(false), m_GlobalColorR(0), m_GlobalColorG(0), m_GlobalColorB(0),
m_GlobalColorA(0), m_GlobalColorSelectedR(0), m_GlobalColorSelectedG(0),
m_GlobalColorSelectedB(0), m_GlobalColorSelectedA(0), m_GlobalColorFocusedR(0),
m_GlobalColorFocusedG(0), m_GlobalColorFocusedB(0), m_GlobalColorFocusedA(0),
m_FocusedOffsetY(0), m_Entry(maxLength, maxWidth, maxWidth)
{
}
//----------------------------------------------------------------------------------
CGUITextEntry::~CGUITextEntry()
{
	m_Entry.Clear();
}
//----------------------------------------------------------------------------------
bool CGUITextEntry::EntryPointerHere()
{
	return (g_EntryPointer == &m_Entry);
}
//----------------------------------------------------------------------------------
WISP_GEOMETRY::CSize CGUITextEntry::GetSize()
{
	return WISP_GEOMETRY::CSize(m_Entry.m_Texture.Width, m_Entry.m_Texture.Height);
}
//----------------------------------------------------------------------------------
void CGUITextEntry::SetGlobalColor(const bool &use, const uint &color, const uint &selected, const uint &focused)
{
	m_UseGlobalColor = use;

	if (use)
	{
		m_GlobalColorR = GetRValue(color);
		m_GlobalColorG = GetGValue(color);
		m_GlobalColorB = GetBValue(color);
		m_GlobalColorA = color >> 24;

		if (!m_GlobalColorA)
			m_GlobalColorA = 0xFF;

		m_GlobalColorSelectedR = GetRValue(selected);
		m_GlobalColorSelectedG = GetGValue(selected);
		m_GlobalColorSelectedB = GetBValue(selected);
		m_GlobalColorSelectedA = selected >> 24;

		if (!m_GlobalColorSelectedA)
			m_GlobalColorSelectedA = 0xFF;

		m_GlobalColorFocusedR = GetRValue(focused);
		m_GlobalColorFocusedG = GetGValue(focused);
		m_GlobalColorFocusedB = GetBValue(focused);
		m_GlobalColorFocusedA = focused >> 24;

		if (!m_GlobalColorFocusedA)
			m_GlobalColorFocusedA = 0xFF;
	}
}
//----------------------------------------------------------------------------------
void CGUITextEntry::OnClick(CGump *gump, const int &x, const int &y)
{
	m_Entry.OnClick(gump, m_Font, m_Unicode, x, y, m_Align, m_TextFlags);
}
//----------------------------------------------------------------------------------
void CGUITextEntry::OnMouseEnter()
{
	if (g_SelectedObject.Gump() != NULL)
		g_SelectedObject.Gump()->WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGUITextEntry::OnMouseExit()
{
	if (g_LastSelectedObject.Gump() != NULL)
		g_LastSelectedObject.Gump()->WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGUITextEntry::PrepareTextures()
{
	ushort color = m_Color;

	if (!m_UseGlobalColor)
	{
		if (m_Focused || &m_Entry == g_EntryPointer)
			color = m_ColorFocused;
		else if (g_GumpSelectedElement == this || (m_CheckOnSerial && m_Serial == g_SelectedObject.Serial && g_SelectedObject.Object() != NULL && g_SelectedObject.Object()->IsGUI()))
			color = m_ColorSelected;
	}

	if (color && m_Unicode)
		color++;

	if (m_Unicode)
		m_Entry.PrepareToDrawW(m_Font, color, m_Align, m_TextFlags);
	else
		m_Entry.PrepareToDrawA(m_Font, color, m_Align, m_TextFlags);
}
//----------------------------------------------------------------------------------
void CGUITextEntry::Draw(const bool &checktrans)
{
	int y = m_Y;
	ushort color = m_Color;

	if (m_Focused || &m_Entry == g_EntryPointer)
	{
		if (m_UseGlobalColor)
			glColor4ub(m_GlobalColorFocusedR, m_GlobalColorFocusedG, m_GlobalColorFocusedB, m_GlobalColorFocusedA);
		else
			color = m_ColorFocused;

		y += m_FocusedOffsetY;
	}
	else if (g_GumpSelectedElement == this || (m_CheckOnSerial && m_Serial == g_SelectedObject.Serial && g_SelectedObject.Object() != NULL && g_SelectedObject.Object()->IsGUI()))
	{
		if (m_UseGlobalColor)
			glColor4ub(m_GlobalColorSelectedR, m_GlobalColorSelectedG, m_GlobalColorSelectedB, m_GlobalColorSelectedA);
		else
			color = m_ColorSelected;
	}
	else if (m_UseGlobalColor)
		glColor4ub(m_GlobalColorR, m_GlobalColorG, m_GlobalColorB, m_GlobalColorA);

	if (color && m_Unicode)
		color++;

	if (checktrans)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (m_Unicode)
			m_Entry.DrawW(m_Font, color, m_X, y, m_Align, m_TextFlags);
		else
			m_Entry.DrawA(m_Font, color, m_X, y, m_Align, m_TextFlags);

		glDisable(GL_BLEND);

		glEnable(GL_STENCIL_TEST);

		if (m_Unicode)
			m_Entry.DrawW(m_Font, color, m_X, y, m_Align, m_TextFlags);
		else
			m_Entry.DrawA(m_Font, color, m_X, y, m_Align, m_TextFlags);

		glDisable(GL_STENCIL_TEST);
	}
	else
	{
		if (m_Unicode)
			m_Entry.DrawW(m_Font, color, m_X, y, m_Align, m_TextFlags);
		else
			m_Entry.DrawA(m_Font, color, m_X, y, m_Align, m_TextFlags);
	}

	if (m_UseGlobalColor)
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
//----------------------------------------------------------------------------------
bool CGUITextEntry::Select()
{
	int x = g_MouseManager.Position.X - m_X;
	int y = g_MouseManager.Position.Y - m_Y;

	return (x >= 0 && y >= 0 && x < m_Entry.m_Texture.Width && y < m_Entry.m_Texture.Height);
}
//----------------------------------------------------------------------------------
