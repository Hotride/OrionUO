﻿// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIShopResult.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIShopResult::CGUIShopResult(CGUIShopItem *shopItem, int x, int y)
: CBaseGUI(GOT_SHOPRESULT, shopItem->Serial, shopItem->Graphic, shopItem->Color, x, y),
Price(shopItem->Price), Name(shopItem->Name)
{
	WISPFUN_DEBUG("c74_f1");
	MoveOnDrag = true;

	string name = Name + "\n" + "at " + std::to_string(Price) + " g.p.";
	g_FontManager.GenerateA(9, m_NameText, name, 0x021F, 100);
	
	int maxCount = shopItem->Count;

	if (maxCount > 999)
		maxCount = 999;

	m_MinMaxButtons = new CGUIMinMaxButtons(Serial, 0x0037, 156, m_NameText.Height / 2, 0, maxCount, 1);
	m_MinMaxButtons->DefaultTextOffset = -122;
	m_MinMaxButtons->SetTextParameters(true, STP_LEFT_CENTER, 9, 0x021F, false);
}
//----------------------------------------------------------------------------------
CGUIShopResult::~CGUIShopResult()
{
	WISPFUN_DEBUG("c74_f2");
	m_NameText.Clear();
	RELEASE_POINTER(m_MinMaxButtons);
}
//----------------------------------------------------------------------------------
CBaseGUI *CGUIShopResult::SelectedItem()
{
	WISPFUN_DEBUG("c74_f3");
	CBaseGUI *result = this;
	WISP_GEOMETRY::CSize size = m_MinMaxButtons->GetSize();

	if (g_Orion.PolygonePixelsInXY(m_X + m_MinMaxButtons->GetX(), m_Y + m_MinMaxButtons->GetY(), size.Width, size.Height))
		result = m_MinMaxButtons;

	return result;
}
//----------------------------------------------------------------------------------
void CGUIShopResult::PrepareTextures()
{
	WISPFUN_DEBUG("c74_f4");
	m_MinMaxButtons->PrepareTextures();
}
//----------------------------------------------------------------------------------
void CGUIShopResult::Draw(bool checktrans)
{
	WISPFUN_DEBUG("c74_f5");
	glTranslatef((GLfloat)m_X, (GLfloat)m_Y, 0.0f);

	glUniform1iARB(g_ShaderDrawMode, SDM_NO_COLOR);

	m_NameText.Draw(34, 0, checktrans);
	m_MinMaxButtons->Draw(checktrans);

	glTranslatef((GLfloat)-m_X, (GLfloat)-m_Y, 0.0f);
}
//----------------------------------------------------------------------------------
bool CGUIShopResult::Select()
{
	WISPFUN_DEBUG("c74_f6");
	int x = g_MouseManager.Position.X - m_X;
	int y = g_MouseManager.Position.Y - m_Y;

	return (x >= 0 && y >= 0 && x < 200 && y < m_NameText.Height);
}
//----------------------------------------------------------------------------------
