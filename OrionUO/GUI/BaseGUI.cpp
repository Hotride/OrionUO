/***********************************************************************************
**
** BaseGUI.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "BaseGUI.h"
#include "../OrionUO.h"
//----------------------------------------------------------------------------------
CBaseGUI::CBaseGUI(const GUMP_OBJECT_TYPE &type, const uint &serial, const ushort &graphic, const ushort &color, const int &x, const int &y)
: CRenderObject(serial, graphic, color, x, y), m_Type(type), m_MoveOnDrag(false),
m_PartialHue(false), m_CheckPolygone(false), m_Enabled(true), m_Visible(true),
m_SelectOnly(false), m_DrawOnly(false)
{
}
//----------------------------------------------------------------------------------
CBaseGUI::~CBaseGUI()
{
}
//----------------------------------------------------------------------------------
