// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GumpPropertyIcon.cpp
**
** Copyright (C) November 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpPropertyIcon::CGumpPropertyIcon(const int &x, const int &y)
: CGump(GT_PROPERTY_ICON, 0, x, y)
{
	m_Locker.Serial = ID_GPI_LOCK_MOVING;

	int width = 0;
	g_ToolTip.CreateTextTexture(m_Texture, m_Text, width, 300);
}
//----------------------------------------------------------------------------------
CGumpPropertyIcon::~CGumpPropertyIcon()
{
}
//----------------------------------------------------------------------------------
void CGumpPropertyIcon::SetText(const wstring &val)
{
	m_Text = val;
	int width = 0;
	g_ToolTip.CreateTextTexture(m_Texture, val, width, (g_ConfigManager.GetItemPropertiesMode() == OPM_ALWAYS_UP ? 300 : 0));
	m_Object = NULL;
	WantUpdateContent = true;
}
//----------------------------------------------------------------------------------
void CGumpPropertyIcon::PrepareContent()
{
	if (g_ConfigManager.GetItemPropertiesMode() == OPM_AT_ICON && m_Object != NULL && m_Object != g_SelectedObject.Object)
	{
		m_Object = NULL;
		g_ObjectPropertiesManager.Reset();
		WantUpdateContent = true;
	}
}
//----------------------------------------------------------------------------------
void CGumpPropertyIcon::UpdateContent()
{
	Clear();

	uchar mode = g_ConfigManager.GetItemPropertiesMode();

	if (mode == OPM_AT_ICON)
	{
		if (m_Object == NULL)
			Add(new CGUIGumppic(0x00E3, 0, 0));
		else
		{
			Add(new CGUIColoredPolygone(0, 0, 0, 0, m_Texture.Width + 12, m_Texture.Height + 8, 0xC07F7F7F));
			Add(new CGUIColoredPolygone(0, 0, 6, 20, m_Texture.Width, m_Texture.Height - 16, 0xC0000000));

			Add(new CGUIExternalTexture(&m_Texture, false, 6, 2));
		}
	}
	else if (mode == OPM_ALWAYS_UP)
	{
		int height = m_Texture.Height;

		if (height < 150)
			height = 150;

		Add(new CGUIColoredPolygone(0, 0, 0, 0, m_Texture.Width + 12, height + 8, 0xC07F7F7F));
		Add(new CGUIColoredPolygone(0, 0, 6, 20, m_Texture.Width, height - 16, 0xC0000000));

		Add(new CGUIExternalTexture(&m_Texture, false, 6, 2));

		Add(new CGUIButton(ID_GPI_MINIMIZE, 0x0A96, 0x0A97, 0x0A97, m_Texture.Width - 10, 0));
	}
	else if (mode == OPM_FOLLOW_MOUSE)
		Add(new CGUIGumppic(0x00E1, 0, 0));
	else if (mode == OPM_SINGLE_CLICK)
		Add(new CGUIGumppic(0x00EC, 0, 0));
}
//----------------------------------------------------------------------------------
void CGumpPropertyIcon::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c126_f13");
	if (serial == ID_GPI_LOCK_MOVING)
		LockMoving = !LockMoving;
	else if (serial == ID_GPI_MINIMIZE)
		g_ConfigManager.SetItemPropertiesMode(OPM_FOLLOW_MOUSE);
}
//----------------------------------------------------------------------------------
bool CGumpPropertyIcon::OnLeftMouseButtonDoubleClick()
{
	WISPFUN_DEBUG("c126_f14");
	
	uchar mode = g_ConfigManager.GetItemPropertiesMode() + 1;

	if (mode > OPM_SINGLE_CLICK)
		mode = OPM_AT_ICON;

	g_ConfigManager.SetItemPropertiesMode(mode);

	return true;
}
//----------------------------------------------------------------------------------
