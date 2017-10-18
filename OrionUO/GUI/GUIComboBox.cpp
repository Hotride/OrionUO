/***********************************************************************************
**
** GUIComboBox.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIComboBox::CGUIComboBox(const uint &serial, const ushort &graphic, const bool &compositeBackground, const ushort &openGraphic, const int &x, const int &y, const int &width, const int &showItemsCount, const bool &showMaximizedCenter)
: CBaseGUI(GOT_COMBOBOX, serial, graphic, 0, x, y), m_OpenGraphic(openGraphic),
m_CompositeBackground(compositeBackground), m_ShowItemsCount(showItemsCount),
m_Width(width), m_ShowMaximizedCenter(showMaximizedCenter), m_OpenedWidth(width)
{
	WISPFUN_DEBUG("c49_f1");
	m_MoveOnDrag = false;
	m_ArrowX = 0;
	m_OffsetY = 0;
	m_StepY = 0;
	m_MinimizedArrowX = m_Width - 16;
	m_WorkWidth = m_Width - 6;
	m_WorkHeight = m_ShowItemsCount * 15;

	if (m_CompositeBackground)
	{
		CGLTexture *th = g_Orion.ExecuteGump(m_OpenGraphic);

		if (th != NULL)
		{
			m_ArrowX = th->Width - 24;
			m_OffsetY = th->Height;
		}

		th = g_Orion.ExecuteGump(m_OpenGraphic + 1);

		if (th != NULL)
		{
			m_StepY = th->Height;
			m_WorkWidth = th->Width - 12;
		}

		th = g_Orion.ExecuteGump(m_Graphic);

		if (th != NULL && !m_Width)
			m_MinimizedArrowX = th->Width - 16;
	}
}
//----------------------------------------------------------------------------------
CGUIComboBox::~CGUIComboBox()
{
	WISPFUN_DEBUG("c49_f2");
	if (m_Text != NULL)
	{
		delete m_Text;
		m_Text = NULL;
	}
}
//----------------------------------------------------------------------------------
void CGUIComboBox::RecalculateWidth()
{
	WISPFUN_DEBUG("c49_f3");
	if (!m_CompositeBackground)
	{
		m_OpenedWidth = 0;

		QFOR(item, m_Items, CBaseGUI*)
		{
			if (item->Type == GOT_TEXT)
			{
				CGUIText *text = (CGUIText*)item;

				if (m_OpenedWidth < text->m_Texture.Width)
					m_OpenedWidth = text->m_Texture.Width;
			}
		}

		m_OpenedWidth += 8;

		if (m_OpenedWidth < m_Width)
			m_OpenedWidth = m_Width;

		m_WorkWidth = m_OpenedWidth - 6;
	}
}
//----------------------------------------------------------------------------------
void CGUIComboBox::OnChangeShowItemsCount(const int &val)
{
	WISPFUN_DEBUG("c49_f4");
	m_WorkHeight = val * 15;
}
//----------------------------------------------------------------------------------
WISP_GEOMETRY::CSize CGUIComboBox::GetSize()
{
	WISPFUN_DEBUG("c49_f5");
	WISP_GEOMETRY::CSize size;
	/*CGLTexture *th = g_Orion.ExecuteGump(GetDrawGraphic());

	if (th != NULL)
	{
		size.Width = th->Width;
		size.Height = th->Height;
	}*/

	return size;
}
//----------------------------------------------------------------------------------
void CGUIComboBox::PrepareTextures()
{
	WISPFUN_DEBUG("c49_f6");
	if (m_CompositeBackground)
	{
		g_Orion.ExecuteGump(m_Graphic);
		g_Orion.ExecuteGump(0x0985);
		g_Orion.ExecuteGump(0x0983);
		g_Orion.ExecuteGumpPart(m_OpenGraphic, 5);
	}
	else
	{
		g_Orion.ExecuteResizepic(m_Graphic);
		g_Orion.ExecuteResizepic(m_OpenGraphic);
		g_Orion.ExecuteGump(0x00FC);
	}
}
//----------------------------------------------------------------------------------
CBaseGUI *CGUIComboBox::SkipToStart()
{
	WISPFUN_DEBUG("c49_f7");
	CBaseGUI *start = (CBaseGUI*)m_Items;

	int index = 0;

	QFOR(item, m_Items, CBaseGUI*)
	{
		if (item->Type == GOT_TEXT)
		{
			if (index == m_StartIndex)
			{
				start = (CBaseGUI*)item;
				break;
			}

			index++;
		}
	}

	return start;
}
//----------------------------------------------------------------------------------
void CGUIComboBox::Draw(const bool &checktrans)
{
	WISPFUN_DEBUG("c49_f8");
	if (m_Text != NULL)
		m_Text->m_Texture.Draw(m_X + m_Text->X, m_Y + m_Text->Y + m_TextOffsetY, checktrans);

	if (g_PressedObject.LeftObject == this) //maximized
	{
		int currentX = m_X + 3;
		int currentY = m_Y + 3;

		if (m_ShowMaximizedCenter)
			currentY -= m_WorkHeight / 2;

		if (m_CompositeBackground)
		{
			int bodyY = m_Y + m_OffsetY;
			int bodyStep = m_StepY;

			currentX = m_X + 12;
			currentY = bodyY + 1;
			int posY = m_Y;

			if (m_ShowMaximizedCenter)
			{
				currentY -= m_WorkHeight / 2;
				posY -= m_WorkHeight / 2;
				bodyY -= m_WorkHeight / 2;
			}

			g_Orion.DrawGump(m_OpenGraphic, 0, m_X, posY);
			g_Orion.DrawGump(0x0983, 0, m_X + m_ArrowX, posY + 2);

			int graphicOffset = 0;

			IFOR(i, 0, m_ShowItemsCount)
			{
				g_Orion.DrawGump(m_OpenGraphic + 1 + graphicOffset, 0, m_X + 5, bodyY);
				graphicOffset = (graphicOffset + 1) % 3;
				bodyY += bodyStep;
			}

			g_Orion.DrawGump(m_OpenGraphic + 4, 0, m_X, bodyY);
			g_Orion.DrawGump(0x0985, 0, m_X + m_ArrowX, bodyY);
		}
		else
			g_Orion.DrawResizepicGump(m_OpenGraphic, m_X, m_Y, m_OpenedWidth, m_WorkHeight + 6);

		if (g_ConfigManager.UseGLListsForInterface)
			g_GL.PushScissor(currentX, currentY, m_WorkWidth, m_WorkHeight);
		else
			g_GL.PushScissor((int)g_GumpTranslate.X + currentX, g_OrionWindow.Size.Height - ((int)g_GumpTranslate.Y + currentY) - m_WorkHeight, m_WorkWidth, m_WorkHeight);

		CBaseGUI *start = SkipToStart();
		int count = 0;

		QFOR(item, start, CBaseGUI*)
		{
			if (item->Type == GOT_TEXT)
			{
				if (g_SelectedObject.Object == item)
				{
					glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
					g_GL.DrawPolygone(currentX, currentY, m_WorkWidth, 14);
					glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
				}

				CGUIText *text = (CGUIText*)item;

				text->m_Texture.Draw(currentX, currentY + m_TextOffsetY);
				currentY += 15;

				count++;

				if (count > m_ShowItemsCount)
					break;
			}
		}

		g_GL.PopScissor();
	}
	else
	{
		CGUIText *selected = NULL;
		int index = 0;

		QFOR(item, m_Items, CBaseGUI*)
		{
			if (item->Type == GOT_TEXT)
			{
				if (index == m_SelectedIndex)
				{
					selected = (CGUIText*)item;
					break;
				}

				index++;
			}
		}

		if (m_CompositeBackground)
		{
			if (m_Width)
				g_Orion.DrawGump(m_Graphic, 0, m_X, m_Y, m_Width, 0);
			else
				g_Orion.DrawGump(m_Graphic, 0, m_X, m_Y);

			if (selected != NULL)
			{
				if (g_ConfigManager.UseGLListsForInterface)
					g_GL.PushScissor(m_X + 6, m_Y, m_MinimizedArrowX, 20);
				else
					g_GL.PushScissor((int)g_GumpTranslate.X + m_X + 6, g_OrionWindow.Size.Height - ((int)g_GumpTranslate.Y + m_Y) - 20, m_MinimizedArrowX, 20);

				selected->m_Texture.Draw(m_X + 6, m_Y + 6 + m_TextOffsetY);
				g_GL.PopScissor();
			}

			g_Orion.DrawGump(0x0985, 0, m_X + m_MinimizedArrowX, m_Y + 6);
		}
		else
		{
			g_Orion.DrawResizepicGump(m_Graphic, m_X, m_Y, m_Width, 20);

			if (selected != NULL)
			{
				if (g_ConfigManager.UseGLListsForInterface)
					g_GL.PushScissor(m_X + 3, m_Y, m_Width - 6, 20);
				else
					g_GL.PushScissor((int)g_GumpTranslate.X + m_X + 3, g_OrionWindow.Size.Height - ((int)g_GumpTranslate.Y + m_Y) - 20, m_Width - 6, 20);

				selected->m_Texture.Draw(m_X + 3, m_Y + 4 + m_TextOffsetY);
				g_GL.PopScissor();
			}

			g_Orion.DrawGump(0x00FC, 0, m_X + m_MinimizedArrowX, m_Y - 1);
		}
	}
}
//----------------------------------------------------------------------------------
bool CGUIComboBox::Select()
{
	WISPFUN_DEBUG("c49_f9");
	m_ListingDirection = 0;
	bool select = false;

	if (g_PressedObject.LeftObject == this) //maximized
	{
		int currentX = m_X + 3;
		int currentY = m_Y + 3;

		if (m_CompositeBackground)
		{
			currentY = m_Y + m_OffsetY + 1;
			currentX = m_X + 12;
		}

		if (m_ShowMaximizedCenter)
			currentY -= m_WorkHeight / 2;

		select = g_Orion.PolygonePixelsInXY(currentX, currentY, m_WorkWidth, m_WorkHeight);

		if (!select)
		{
			if (g_MouseManager.Position.Y < currentY)
				m_ListingDirection = 1;
			else if (g_MouseManager.Position.Y > currentY + m_WorkHeight)
				m_ListingDirection = 2;
		}
	}
	else
	{
		if (m_CompositeBackground)
		{
			if (m_Width)
				select = g_Orion.GumpPixelsInXY(m_Graphic, m_X, m_Y, m_Width, 0);
			else
				select = g_Orion.GumpPixelsInXY(m_Graphic, m_X, m_Y);
		}
		else
			select = g_Orion.ResizepicPixelsInXY(m_Graphic, m_X, m_Y, m_Width, 20);
	}

	return select;
}
//----------------------------------------------------------------------------------
CBaseGUI *CGUIComboBox::SelectedItem()
{
	WISPFUN_DEBUG("c49_f10");
	CBaseGUI *select = this;

	if (g_PressedObject.LeftObject == this) //maximized
	{
		int currentX = m_X + 3;
		int currentY = m_Y + 3;

		if (m_CompositeBackground)
		{
			currentY = m_Y + m_OffsetY + 1;
			currentX = m_X + 12;
		}

		if (m_ShowMaximizedCenter)
			currentY -= m_WorkHeight / 2;

		CBaseGUI *start = SkipToStart();
		int count = 0;

		QFOR(item, start, CBaseGUI*)
		{
			if (item->Type == GOT_TEXT)
			{
				if (g_Orion.PolygonePixelsInXY(currentX, currentY, m_WorkWidth, 14))
				{
					select = item;
					break;
				}

				currentY += 15;

				count++;

				if (count > m_ShowItemsCount)
					break;
			}
		}
	}

	return select;
}
//----------------------------------------------------------------------------------
int CGUIComboBox::IsSelectedItem()
{
	WISPFUN_DEBUG("c49_f11");
	int select = -1;

	if (g_PressedObject.LeftObject == this) //maximized
	{
		CBaseGUI *start = SkipToStart();
		int count = 0;

		QFOR(item, start, CBaseGUI*)
		{
			if (item->Type == GOT_TEXT)
			{
				if (g_SelectedObject.Object == item)
				{
					select = m_StartIndex + count;
					break;
				}

				count++;

				if (count > m_ShowItemsCount)
					break;
			}
		}
	}

	return select;
}
//----------------------------------------------------------------------------------
