// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIShopItem.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIShopItem::CGUIShopItem(const uint &serial, const ushort &graphic, const ushort &color, const uint &count, const uint &price, const string &name, const int &x, const int &y)
: CBaseGUI(GOT_SHOPITEM, serial, graphic, color, x, y), m_Count(count), m_Price(price),
m_Name(name)
{
	WISPFUN_DEBUG("c73_f1");
	m_MoveOnDrag = true;

	CreateCountText(0);
	CreateNameText();
	UpdateOffsets();
}
//----------------------------------------------------------------------------------
CGUIShopItem::~CGUIShopItem()
{
	WISPFUN_DEBUG("c73_f2");
	m_NameText.Clear();
	m_CountText.Clear();
}
//----------------------------------------------------------------------------------
void CGUIShopItem::UpdateOffsets()
{
	if (m_Serial >= 0x40000000)
	{
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
	else
	{
		uchar group = 0;

		switch (g_AnimationManager.GetGroupIndex(m_Graphic))
		{
			case AG_LOW:
			{
				group = LAG_STAND;
				break;
			}
			case AG_HIGHT:
			{
				group = HAG_STAND;
				break;
			}
			case AG_PEOPLE:
			{
				group = PAG_STAND;
				break;
			}
			default:
				break;
		}

		ANIMATION_DIMENSIONS dims = g_AnimationManager.GetAnimationDimensions(0, m_Graphic, 1, group, false);

		if (dims.Height)
		{
			m_MaxOffset = dims.Height;

			if (m_MaxOffset > 35)
				m_MaxOffset = 35;

			if (m_MaxOffset < m_NameText.Height)
			{
				m_ImageOffset = ((m_NameText.Height - m_MaxOffset) / 2);
				m_MaxOffset = m_NameText.Height;
			}
			else
				m_TextOffset = ((m_MaxOffset - m_NameText.Height) / 2);
		}
	}
}
//----------------------------------------------------------------------------------
void CGUIShopItem::OnClick()
{
	WISPFUN_DEBUG("c73_f3");
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
	WISPFUN_DEBUG("c73_f4");
	ushort textColor = 0x021F;

	if (m_Selected)
		textColor = 0x0021;

	string str = m_Name + " at " + std::to_string(m_Price) + "gp";
	g_FontManager.GenerateA(9, m_NameText, str, textColor, 90);
}
//----------------------------------------------------------------------------------
void CGUIShopItem::CreateCountText(const int &lostCount)
{
	WISPFUN_DEBUG("c73_f5");
	ushort textColor = 0x021F;

	if (m_Selected)
		textColor = 0x0021;

	g_FontManager.GenerateA(9, m_CountText, std::to_string(m_Count - lostCount), textColor);
}
//----------------------------------------------------------------------------------
void CGUIShopItem::PrepareTextures()
{
	WISPFUN_DEBUG("c73_f6");

	if (m_Serial >= 0x40000000)
		g_Orion.ExecuteStaticArt(m_Graphic);
	else
	{
		uchar group = 0;

		switch (g_AnimationManager.GetGroupIndex(m_Graphic))
		{
			case AG_LOW:
			{
				group = LAG_STAND;
				break;
			}
			case AG_HIGHT:
			{
				group = HAG_STAND;
				break;
			}
			case AG_PEOPLE:
			{
				group = PAG_STAND;
				break;
			}
			default:
				break;
		}

		CTextureAnimationDirection &direction = g_AnimationManager.m_DataIndex[m_Graphic].m_Groups[group].m_Direction[1];
		direction.LastAccessTime = GetTickCount() + 60000;
		g_AnimationManager.AnimID = m_Graphic;
		g_AnimationManager.AnimGroup = group;
		g_AnimationManager.Direction = 1;

		if (direction.FrameCount == 0)
			g_AnimationManager.LoadDirectionGroup(direction);
	}

	g_Orion.ExecuteGump(0x0039);
	g_Orion.ExecuteGump(0x003A);
	g_Orion.ExecuteGump(0x003B);
}
//----------------------------------------------------------------------------------
void CGUIShopItem::SetShaderMode()
{
	WISPFUN_DEBUG("c73_f7");
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
	WISPFUN_DEBUG("c73_f8");
	CGLTexture *th = NULL;

	glTranslatef((GLfloat)m_X, (GLfloat)m_Y, 0.0f);

	glUniform1iARB(g_ShaderDrawMode, 0);

	m_NameText.Draw(52, m_TextOffset);
	m_CountText.Draw(196 - m_CountText.Width, (m_MaxOffset / 2) - (m_CountText.Height / 2));

	SetShaderMode();

	if (m_Serial >= 0x40000000)
	{
		th = g_Orion.ExecuteStaticArt(m_Graphic);

		if (th != NULL)
			th->Draw(2, m_ImageOffset, checktrans);
	}
	else
	{
		uchar group = 0;

		switch (g_AnimationManager.GetGroupIndex(m_Graphic))
		{
			case AG_LOW:
			{
				group = LAG_STAND;
				break;
			}
			case AG_HIGHT:
			{
				group = HAG_STAND;
				break;
			}
			case AG_PEOPLE:
			{
				group = PAG_STAND;
				break;
			}
			default:
				break;
		}

		CTextureAnimationDirection &direction = g_AnimationManager.m_DataIndex[m_Graphic].m_Groups[group].m_Direction[1];

		if (direction.FrameCount != 0)
		{
			CGLTexture &originalTexture = direction.m_Frames[0];

			if (originalTexture.Texture)
			{
				CGLTexture tex;
				tex.Texture = originalTexture.Texture;

				if (originalTexture.Width > 35)
					tex.Width = 35;
				else
					tex.Width = originalTexture.Width;

				if (originalTexture.Height > 35)
					tex.Height = 35;
				else
					tex.Height = originalTexture.Height;

				g_GL.GL1_Draw(tex, 2, m_ImageOffset);

				tex.Texture = 0;
				
				//originalTexture.Draw(2, m_ImageOffset, checktrans);
			}
		}
	}

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
//----------------------------------------------------------------------------------
bool CGUIShopItem::Select()
{
	WISPFUN_DEBUG("c73_f9");
	int x = g_MouseManager.Position.X - m_X;
	int y = g_MouseManager.Position.Y - m_Y;

	return (x >= 0 && y >= -10 && x < 200 && y < m_MaxOffset);
}
//----------------------------------------------------------------------------------
