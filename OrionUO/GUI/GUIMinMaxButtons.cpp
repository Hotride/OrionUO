/***********************************************************************************
**
** GUIMinMaxButtons.cpp
**
** Copyright (C) September 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GUIMinMaxButtons.h"
#include "../OrionUO.h"
#include "../Managers/FontsManager.h"
#include "../Managers/MouseManager.h"
//----------------------------------------------------------------------------------
CGUIMinMaxButtons::CGUIMinMaxButtons(const uint &serial, const ushort &graphic, const int &x, const int &y, const int &minValue, const int &maxValue, const int &value)
: CBaseGUI(GOT_MINMAXBUTTONS, serial, graphic, 0, x, y), m_MinValue(minValue),
m_MaxValue(maxValue), m_Value(value), m_HaveText(false), m_TextPosition(STP_RIGHT),
m_Font(0), m_TextColor(0), m_Unicode(true), m_TextWidth(0), m_Align(TS_LEFT),
m_TextFlags(0), m_TextX(0), m_TextY(0), m_BaseScrollStep(1), m_ScrollStep(1),
m_LastScrollTime(0), m_DefaultTextOffset(2), m_ScrollMode(0)
{
}
//----------------------------------------------------------------------------------
CGUIMinMaxButtons::~CGUIMinMaxButtons()
{
	m_Text.Clear();
}
//----------------------------------------------------------------------------------
void CGUIMinMaxButtons::UpdateText()
{
	if (m_HaveText)
	{
		if (m_Unicode)
			g_FontManager.GenerateW(m_Font, m_Text, std::to_wstring(m_Value).c_str(), m_TextColor, 30, m_TextWidth, m_Align, m_TextFlags);
		else
			g_FontManager.GenerateA(m_Font, m_Text, std::to_string(m_Value).c_str(), m_TextColor, m_TextWidth, m_Align, m_TextFlags);

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
					textY += 18 + m_DefaultTextOffset;

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
					textX += 36 + m_DefaultTextOffset;

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
					int sliderWidth = 36;

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
					int sliderHeight = 18;

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
void CGUIMinMaxButtons::Scroll(const uint &delay)
{
	if (m_LastScrollTime < g_Ticks && m_ScrollMode)
	{
		if (m_ScrollMode == 1)
			m_Value += m_ScrollStep;
		else
			m_Value -= m_ScrollStep;

		if (m_Value < m_MinValue)
			m_Value = m_MinValue;
		else if (m_Value > m_MaxValue)
			m_Value = m_MaxValue;

		m_ScrollStep++;
		m_LastScrollTime = g_Ticks + delay;

		UpdateText();
	}
}
//----------------------------------------------------------------------------------
void CGUIMinMaxButtons::OnClick()
{
	int x = g_MouseManager.Position.X - m_X;
	int y = g_MouseManager.Position.Y - m_Y;

	if (x >= 0 && y >= 0 && y < 18)
	{
		if (x < 18)
			m_ScrollMode = 1;
		else
			m_ScrollMode = 2;
	}
	else
		m_ScrollMode = 0;

	m_ScrollStep = m_BaseScrollStep;
}
//----------------------------------------------------------------------------------
void CGUIMinMaxButtons::SetTextParameters(const bool &haveText, const SLIDER_TEXT_POSITION &textPosition, const uchar &font, const ushort &color, const bool &unicode, const int &textWidth, const TEXT_ALIGN_TYPE &align, const ushort &textFlags)
{
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
void CGUIMinMaxButtons::PrepareTextures()
{
	g_Orion.ExecuteGump(m_Graphic);
	g_Orion.ExecuteGump(m_Graphic + 1);
}
//----------------------------------------------------------------------------------
void CGUIMinMaxButtons::Draw(const bool &checktrans)
{
	glUniform1iARB(g_ShaderDrawMode, 0);

	IFOR(i, 0, 2)
	{
		CGLTexture *th = g_Orion.ExecuteGump(m_Graphic + i);

		if (th != NULL)
			th->Draw(m_X + (i * 18), m_Y, checktrans);
	}

	if (m_HaveText)
		m_Text.Draw(m_TextX, m_TextY, checktrans);
}
//----------------------------------------------------------------------------------
bool CGUIMinMaxButtons::Select()
{
	int x = g_MouseManager.Position.X - m_X;
	int y = g_MouseManager.Position.Y - m_Y;

	return (x >= 0 && y >= 0 && x < 36 && y < 18);
}
//----------------------------------------------------------------------------------
