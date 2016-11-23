/***********************************************************************************
**
** GUISkillGroup.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GUISkillGroup.h"
#include "../OrionUO.h"
#include "../Managers/MouseManager.h"
#include "../SelectedObject.h"
#include "../PressedObject.h"
#include "../Skills.h"
#include "../Game objects/GamePlayer.h"
#include "../SkillGroup.h"
//----------------------------------------------------------------------------------
CGUISkillGroup::CGUISkillGroup(const uint &serial, const uint &minimizeSerial, CSkillGroupObject *group, const int &x, const int &y)
: CBaseGUI(GOT_SKILLGROUP, serial, 0, 0, x, y)
{
	m_Minimized = !group->Maximized;
	ushort graphic = (m_Minimized ? 0x0827 : 0x0826);
	m_Minimizer = new CGUIButton(minimizeSerial, graphic, graphic, graphic, 0, 0);

	m_Name = new CGUITextEntry(serial, 0, 0, 0, 16, -5, 0, false, 6);
	m_Name->m_Entry.SetText(group->Name);
}
//----------------------------------------------------------------------------------
CGUISkillGroup::~CGUISkillGroup()
{
	RELEASE_POINTER(m_Minimizer);
	RELEASE_POINTER(m_Name);
}
//----------------------------------------------------------------------------------
void CGUISkillGroup::OnChangeMinimized(const bool &val)
{
	ushort graphic = (val ? 0x0827 : 0x0826);
	m_Minimizer->Graphic = graphic;
	m_Minimizer->GraphicSelected = graphic;
	m_Minimizer->GraphicPressed = graphic;
}
//----------------------------------------------------------------------------------
void CGUISkillGroup::UpdateDataPositions()
{
	int y = 0;

	QFOR(item, m_Items, CBaseGUI*)
	{
		item->Y = y;
		y += 17;
	}
}
//----------------------------------------------------------------------------------
void CGUISkillGroup::PrepareTextures()
{
	m_Minimizer->PrepareTextures();
	g_Orion.ExecuteGump(0x0835);

	QFOR(item, m_Items, CBaseGUI*)
		item->PrepareTextures();
}
//----------------------------------------------------------------------------------
bool CGUISkillGroup::EntryPointerHere()
{
	return (g_EntryPointer == &m_Name->m_Entry);
}
//----------------------------------------------------------------------------------
CBaseGUI *CGUISkillGroup::SelectedItem()
{
	CBaseGUI *selected = m_Name;

	if (g_Orion.PolygonePixelsInXY(m_X + m_Minimizer->X, m_Y + m_Minimizer->Y, 14, 14))
		selected = m_Minimizer;
	else if (!m_Minimized)
	{
		WISP_GEOMETRY::CPoint2Di oldMouse = g_MouseManager.Position;
		g_MouseManager.Position = WISP_GEOMETRY::CPoint2Di(oldMouse.X - m_X, oldMouse.Y - (m_Y + 19));

		QFOR(item, m_Items, CBaseGUI*)
		{
			if (item->Select())
			{
				selected = item;

				if (item->Type == GOT_SKILLITEM)
					selected = ((CGUISkillItem*)item)->SelectedItem();

				break;
			}
		}

		g_MouseManager.Position = oldMouse;
	}

	return selected;
}
//----------------------------------------------------------------------------------
WISP_GEOMETRY::CSize CGUISkillGroup::GetSize()
{
	WISP_GEOMETRY::CSize size(220, 19);

	if (!m_Minimized && m_Items != NULL)
		size.Height += GetItemsCount() * 17;

	return size;
}
//----------------------------------------------------------------------------------
void CGUISkillGroup::Draw(const bool &checktrans)
{
	glTranslatef((GLfloat)m_X, (GLfloat)m_Y, 0.0f);

	m_Minimizer->Draw(checktrans);

	bool drawOrnament = true;

	if (m_Name->Focused && g_EntryPointer == &m_Name->m_Entry)
	{
		drawOrnament = false;
		g_GL.DrawPolygone(16, 0, 200, 14);
	}
	else if (m_Name->Focused)
		g_GL.DrawPolygone(16, 0, m_Name->m_Entry.m_Texture.Width, 14);

	m_Name->Draw(checktrans);

	if (drawOrnament)
	{
		int x = 11 + m_Name->m_Entry.m_Texture.Width;
		int width = 215 - x;

		if (x > 0)
			g_Orion.DrawGump(0x0835, 0, x, 5, width, 0);
	}

	if (!m_Minimized && m_Items != NULL)
	{
		glTranslatef(0.0f, 19.0f, 0.0f);

		QFOR(item, m_Items, CBaseGUI*)
			item->Draw(checktrans);

		glTranslatef(0.0f, -19.0f, 0.0f);
	}

	glTranslatef((GLfloat)-m_X, (GLfloat)-m_Y, 0.0f);
}
//----------------------------------------------------------------------------------
bool CGUISkillGroup::Select()
{
	int x = g_MouseManager.Position.X - m_X;
	int y = g_MouseManager.Position.Y - m_Y;

	bool result = (x >= 0 && y >= 0 && x < 220 && y < 19);

	if (!m_Minimized && !result)
	{
		WISP_GEOMETRY::CPoint2Di oldMouse = g_MouseManager.Position;
		g_MouseManager.Position = WISP_GEOMETRY::CPoint2Di(oldMouse.X - m_X, oldMouse.Y - (m_Y + 19));

		QFOR(item, m_Items, CBaseGUI*)
		{
			if (item->Select())
			{
				result = true;
				break;
			}
		}

		g_MouseManager.Position = oldMouse;
	}

	return result;
}
//----------------------------------------------------------------------------------
