// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUISlider.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUISlider::CGUISlider(const uint &serial, const ushort &graphic, const ushort &graphicSelected, const ushort &graphicPressed, const ushort &backgroundGraphic, const bool &compositeBackground, const bool &vertical, const int &x, const int &y, const int &lenght, const int &minValue, const int &maxValue, const int &value)
: CBaseGUI(GOT_SLIDER, serial, graphic, 0, x, y), m_GraphicSelected(graphicSelected),
m_GraphicPressed(graphicPressed), m_BackgroundGraphic(backgroundGraphic),
m_CompositeBackground(compositeBackground), m_Vertical(vertical), m_Lenght(lenght),
m_MinValue(minValue), m_MaxValue(maxValue), m_Value(value)
{
	WISPFUN_DEBUG("c77_f1");
	CalculateOffset();
}
//----------------------------------------------------------------------------------
CGUISlider::~CGUISlider()
{
	WISPFUN_DEBUG("c77_f2");
	m_Text.Clear();
}
//----------------------------------------------------------------------------------
void CGUISlider::UpdateText()
{
	WISPFUN_DEBUG("c77_f3");
	if (m_HaveText)
	{
		if (m_Unicode)
			g_FontManager.GenerateW(m_Font, m_Text, std::to_wstring(m_Value), m_TextColor, 30, m_TextWidth, m_Align, m_TextFlags);
		else
			g_FontManager.GenerateA(m_Font, m_Text, std::to_string(m_Value), m_TextColor, m_TextWidth, m_Align, m_TextFlags);

		CGLTexture *th = g_Orion.ExecuteGump(m_Graphic);

		if (th != NULL)
		{
			int textX = m_X;
			int textY = m_Y;

			switch (m_TextPosition)
			{
				case STP_TOP:
				case STP_TOP_CENTER:
				{
					textY -= m_Text.Height - m_DefaultTextOffset;

					break;
				}
				case STP_BOTTOM:
				case STP_BOTTOM_CENTER:
				{
					if (m_Vertical)
						textY += m_Lenght + m_DefaultTextOffset;
					else
						textY += th->Height + m_DefaultTextOffset;

					break;
				}
				case STP_LEFT:
					//textY -= 4;
				case STP_LEFT_CENTER:
				{
					textX -= m_Text.Width - m_DefaultTextOffset;

					break;
				}
				case STP_RIGHT:
					//textY -= 4;
				case STP_RIGHT_CENTER:
				{
					if (m_Vertical)
						textX += th->Width + m_DefaultTextOffset;
					else
						textX += m_Lenght + m_DefaultTextOffset;

					break;
				}
				default:
					break;
			}

			switch (m_TextPosition)
			{
				case STP_TOP_CENTER:
				case STP_BOTTOM_CENTER:
				{
					int textWidth = m_Text.Width;
					int sliderWidth = m_Lenght;

					if (m_Vertical)
						sliderWidth = th->Width;

					int deltaX = abs(sliderWidth - textWidth) / 2;

					if (sliderWidth > textWidth)
						textX += deltaX;
					else
						textX -= deltaX;

					break;
				}
				case STP_LEFT_CENTER:
				case STP_RIGHT_CENTER:
				{
					int textHeight = m_Text.Height;
					int sliderHeight = m_Lenght;

					if (!m_Vertical)
						sliderHeight = th->Height;

					int deltaY = abs(sliderHeight - textHeight) / 2;

					if (sliderHeight > textHeight)
						textY += deltaY;
					else
						textY -= deltaY;

					break;
				}
				default:
					break;
			}

			m_TextX = textX;
			m_TextY = textY;
		}
	}
}
//----------------------------------------------------------------------------------
WISP_GEOMETRY::CSize CGUISlider::GetSize()
{
	WISPFUN_DEBUG("c77_f4");
	WISP_GEOMETRY::CSize size;

	CGLTexture *th = g_Orion.ExecuteGump(m_Graphic);

	if (th != NULL)
	{
		if (m_Vertical)
		{
			size.Width = th->Width;
			size.Height = m_Lenght;
		}
		else
		{
			size.Width = m_Lenght;
			size.Height = th->Height;
		}

		if (m_HaveText)
		{
			//m_Text.Draw(m_TextX, m_TextY, checktrans);
		}
	}

	return size;
}
//----------------------------------------------------------------------------------
void CGUISlider::OnScroll(const bool &up, const uint &delay)
{
	WISPFUN_DEBUG("c77_f5");
	if (m_LastScrollTime < g_Ticks)
	{
		if (up)
			m_Value += m_ScrollStep;
		else
			m_Value -= m_ScrollStep;

		if (m_Value < m_MinValue)
			m_Value = m_MinValue;
		else if (m_Value > m_MaxValue)
			m_Value = m_MaxValue;

		m_LastScrollTime = g_Ticks + delay;

		CalculateOffset();
		UpdateText();
	}
}
//----------------------------------------------------------------------------------
void CGUISlider::OnClick(const int &x, const int &y)
{
	WISPFUN_DEBUG("c77_f6");
	int lenght = m_Lenght;
	int maxValue = m_MaxValue - m_MinValue;

	CGLTexture *th = g_Orion.ExecuteGump(m_Graphic);

	if (th != NULL)
		lenght -= (m_Vertical ? (th->Height / 2) : th->Width);

	float percents = ((m_Vertical ? y : x) / (float)lenght) * 100.0f;

	m_Value = (int)((maxValue * percents) / 100.0f) + m_MinValue;

	CalculateOffset();
	UpdateText();
}
//----------------------------------------------------------------------------------
void CGUISlider::CalculateOffset()
{
	WISPFUN_DEBUG("c77_f7");
	if (m_Value < m_MinValue)
		m_Value = m_MinValue;
	else if (m_Value > m_MaxValue)
		m_Value = m_MaxValue;

	int value = m_Value - m_MinValue;
	int maxValue = m_MaxValue - m_MinValue;
	int lenght = m_Lenght;

	CGLTexture *th = g_Orion.ExecuteGump(m_Graphic);

	if (th != NULL)
		lenght -= (m_Vertical ? th->Height : th->Width);

	m_Percents = (int)((value / (float)maxValue) * 100.0f);

	m_Offset = (int)((lenght * m_Percents) / 100.0f);

	if (m_Offset < 0)
		m_Offset = 0;
}
//----------------------------------------------------------------------------------
void CGUISlider::SetTextParameters(const bool &haveText, const SLIDER_TEXT_POSITION &textPosition, const uchar &font, const ushort &color, const bool &unicode, const int &textWidth, const TEXT_ALIGN_TYPE &align, const ushort &textFlags)
{
	WISPFUN_DEBUG("c77_f8");
	m_HaveText = haveText;
	m_TextPosition = textPosition;
	m_Font = font;
	m_TextColor = color;
	m_Unicode = unicode;
	m_TextWidth = textWidth;
	m_Align = align;
	m_TextFlags = textFlags;

	UpdateText();
}
//----------------------------------------------------------------------------------
void CGUISlider::PrepareTextures()
{
	WISPFUN_DEBUG("c77_f9");
	g_Orion.ExecuteGump(m_Graphic);
	g_Orion.ExecuteGump(m_GraphicSelected);
	g_Orion.ExecuteGump(m_GraphicPressed);

	if (m_BackgroundGraphic)
	{
		if (m_CompositeBackground)
			g_Orion.ExecuteGumpPart(m_BackgroundGraphic, 3);
		else
			g_Orion.ExecuteGump(m_BackgroundGraphic);
	}
}
//----------------------------------------------------------------------------------
ushort CGUISlider::GetDrawGraphic()
{
	WISPFUN_DEBUG("c77_f10");
	ushort graphic = m_Graphic;

	if (g_GumpPressedElement == this)
		graphic = m_GraphicPressed;
	else if (g_GumpSelectedElement == this)
		graphic = m_GraphicSelected;

	return graphic;
}
//----------------------------------------------------------------------------------
void CGUISlider::Draw(const bool &checktrans)
{
	WISPFUN_DEBUG("c77_f11");
	/*m_Value++;
	if (m_Value > m_MaxValue)
	{
		m_Value = m_MinValue;

		m_TextPosition = (SLIDER_TEXT_POSITION)(m_TextPosition + 1);

		if (m_TextPosition > STP_RIGHT)
			m_TextPosition = STP_TOP;
	}
	UpdateText();
	CalculateOffset();*/

	glUniform1iARB(g_ShaderDrawMode, SDM_NO_COLOR);

	if (m_BackgroundGraphic)
	{
		if (m_CompositeBackground)
		{
			CGLTexture *th[3] = { NULL };

			IFOR(i, 0, 3)
			{
				th[i] = g_Orion.ExecuteGump(m_BackgroundGraphic + i);

				if (th[i] == NULL)
					return;
			}

			if (m_Vertical)
			{
				th[0]->Draw(m_X, m_Y, checktrans);
				th[2]->Draw(m_X, m_Y + (m_Lenght - th[2]->Height), checktrans);
				th[1]->Draw(m_X, m_Y + th[0]->Height, 0, (m_Lenght - (th[0]->Height + th[2]->Height)), checktrans);
			}
			else
			{
				th[0]->Draw(m_X, m_Y, checktrans);
				th[2]->Draw(m_X + (m_Lenght - th[2]->Width), m_Y, checktrans);
				th[1]->Draw(m_X + th[0]->Width, m_Y, (m_Lenght - (th[0]->Width + th[2]->Width)), 0, checktrans);
			}
		}
		else
		{
			CGLTexture *th = g_Orion.ExecuteGump(m_BackgroundGraphic);

			if (th != NULL)
			{
				if (m_Vertical)
					th->Draw(m_X, m_Y, 0, m_Lenght, checktrans);
				else
					th->Draw(m_X, m_Y, m_Lenght, 0, checktrans);
			}
		}
	}

	CGLTexture *th = g_Orion.ExecuteGump(GetDrawGraphic());

	if (th != NULL)
	{
		if (m_Vertical)
			th->Draw(m_X, m_Y + m_Offset, checktrans);
		else
			th->Draw(m_X + m_Offset, m_Y, checktrans);

		if (m_HaveText)
			m_Text.Draw(m_TextX, m_TextY, checktrans);
	}
}
//----------------------------------------------------------------------------------
bool CGUISlider::Select()
{
	WISPFUN_DEBUG("c77_f12");
	CGLTexture *th = g_Orion.ExecuteGump(m_Graphic);

	if (th != NULL)
	{
		int buttonX = m_X;
		int buttonY = m_Y;

		if (m_Vertical)
			buttonY += m_Offset;
		else
			buttonX += m_Offset;

		if (th->Select(buttonX, buttonY, !m_CheckPolygone))
			return true;

		if (m_BackgroundGraphic)
		{
			int x = g_MouseManager.Position.X - m_X;
			int y = g_MouseManager.Position.Y - m_Y;

			if (x >= 0 && y >= 0)
			{
				if (m_Vertical)
					return (x < th->Width && y < m_Lenght);
				else
					return (x < m_Lenght && y < th->Height);
			}
		}
	}

	return false;
}
//----------------------------------------------------------------------------------
void CGUISlider::OnMouseEnter()
{
	WISPFUN_DEBUG("c77_f13");
	if (g_SelectedObject.Gump != NULL)
		g_SelectedObject.Gump->WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGUISlider::OnMouseExit()
{
	WISPFUN_DEBUG("c77_f14");
	if (g_LastSelectedObject.Gump != NULL)
		g_LastSelectedObject.Gump->WantRedraw = true;
}
//----------------------------------------------------------------------------------
