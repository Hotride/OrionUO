/***********************************************************************************
**
** GUIShopItem.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GUIShopItem.h"
#include "../OrionUO.h"
#include "../Managers/ColorManager.h"
#include "../Managers/MouseManager.h"
#include "../Managers/UOFileReader.h"
#include "../SelectedObject.h"
//----------------------------------------------------------------------------------
CGUIShopItem::CGUIShopItem(const uint &serial, const ushort &graphic, const ushort &color, const uint &count, const uint &price, const string &name, const int &x, const int &y)
: CBaseGUI(GOT_SHOPITEM, serial, graphic, color, x, y), m_Count(count), m_Price(price),
m_Name(name), m_Selected(false)
{
	m_MoveOnDrag = true;
	CreateNameText();
	CreateCountText(0);

	m_ImageOffset = 0;
	m_TextOffset = 0;
	m_MaxOffset = 0;

	CGLTexture *th = g_Orion.ExecuteStaticArt(m_Graphic);

	if (th != NULL)
	{
		m_MaxOffset = th->Height;

		if (m_MaxOffset < m_NameText.Height)
		{
			m_ImageOffset = ((m_NameText.Height - m_MaxOffset) / 2);
			m_MaxOffset = m_NameText.Height;
		}
		else
			m_TextOffset = ((m_MaxOffset - m_NameText.Height) / 2);
	}
}
//----------------------------------------------------------------------------------
CGUIShopItem::~CGUIShopItem()
{
	m_NameText.Clear();
	m_CountText.Clear();
}
//----------------------------------------------------------------------------------
void CGUIShopItem::OnClick()
{
	m_Selected = false;

	for (CBaseGUI *item = this; item != NULL; item = (CBaseGUI*)item->m_Next)
	{
		if (item->Type == GOT_SHOPITEM && ((CGUIShopItem*)item)->Selected)
		{
			((CGUIShopItem*)item)->Selected = false;
			((CGUIShopItem*)item)->CreateNameText();
		}
	}

	for (CBaseGUI *item = this; item != NULL; item = (CBaseGUI*)item->m_Prev)
	{
		if (item->Type == GOT_SHOPITEM && ((CGUIShopItem*)item)->Selected)
		{
			((CGUIShopItem*)item)->Selected = false;
			((CGUIShopItem*)item)->CreateNameText();
		}
	}

	m_Selected = true;
	CreateNameText();
}
//----------------------------------------------------------------------------------
void CGUIShopItem::CreateNameText()
{
	ushort textColor = 0x021F;

	if (m_Selected)
		textColor = 0x0021;

	string str = m_Name + " at " + std::to_string(m_Price) + "gp";
	g_FontManager.GenerateA(9, m_NameText, str.c_str(), textColor, 90);
}
//----------------------------------------------------------------------------------
void CGUIShopItem::CreateCountText(const int &lostCount)
{
	ushort textColor = 0x021F;

	if (m_Selected)
		textColor = 0x0021;

	g_FontManager.GenerateA(9, m_CountText, std::to_string(m_Count - lostCount).c_str(), textColor);
}
//----------------------------------------------------------------------------------
void CGUIShopItem::PrepareTextures()
{
	g_Orion.ExecuteStaticArt(m_Graphic);
	g_Orion.ExecuteGump(0x0039);
	g_Orion.ExecuteGump(0x003A);
	g_Orion.ExecuteGump(0x003B);
}
//----------------------------------------------------------------------------------
void CGUIShopItem::SetShaderMode()
{
	int drawMode = (m_Color != 0);

	if (drawMode)
	{
		if (m_PartialHue)
			drawMode = 2;

		g_ColorManager.SendColorsToShader(m_Color);
	}

	glUniform1iARB(g_ShaderDrawMode, drawMode);
}
//----------------------------------------------------------------------------------
void CGUIShopItem::Draw(const bool &checktrans)
{
	CGLTexture *th = g_Orion.ExecuteStaticArt(m_Graphic);

	if (th != NULL)
	{
		glTranslatef((GLfloat)m_X, (GLfloat)m_Y, 0.0f);

		glUniform1iARB(g_ShaderDrawMode, 0);

		m_NameText.Draw(52, m_TextOffset);
		m_CountText.Draw(196 - m_CountText.Width, (m_MaxOffset / 2) - (m_CountText.Height / 2));

		SetShaderMode();
		th->Draw(2, m_ImageOffset, checktrans);
		glUniform1iARB(g_ShaderDrawMode, 0);

		th = g_Orion.ExecuteGump(0x0039);

		if (th != NULL)
			th->Draw(2, m_MaxOffset, checktrans);

		th = g_Orion.ExecuteGump(0x003A);

		if (th != NULL)
			th->Draw(32, m_MaxOffset, 140, 0, checktrans);

		th = g_Orion.ExecuteGump(0x003B);

		if (th != NULL)
			th->Draw(166, m_MaxOffset, checktrans);

		glTranslatef((GLfloat)-m_X, (GLfloat)-m_Y, 0.0f);
	}
}
//----------------------------------------------------------------------------------
bool CGUIShopItem::Select()
{
	int x = g_MouseManager.Position.X - m_X;
	int y = g_MouseManager.Position.Y - m_Y;

	return (x >= 0 && y >= 0 && x < 200 && y < m_MaxOffset + 20);
}
//----------------------------------------------------------------------------------
