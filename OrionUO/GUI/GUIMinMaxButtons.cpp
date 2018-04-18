// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIMinMaxButtons.cpp
**
** Copyright (C) September 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIMinMaxButtons::CGUIMinMaxButtons(int serial, ushort graphic, int x, int y, int minValue, int maxValue, int value)
: CBaseGUI(GOT_MINMAXBUTTONS, serial, graphic, 0, x, y), MinValue(minValue),
MaxValue(maxValue), Value(value)
{
}
//----------------------------------------------------------------------------------
CGUIMinMaxButtons::~CGUIMinMaxButtons()
{
	WISPFUN_DEBUG("c67_f1");
	Text.Clear();
}
//----------------------------------------------------------------------------------
void CGUIMinMaxButtons::UpdateText()
{
	WISPFUN_DEBUG("c67_f2");
	if (HaveText)
	{
		if (Unicode)
			g_FontManager.GenerateW(Font, Text, std::to_wstring(Value), TextColor, 30, TextWidth, Align, TextFlags);
		else
			g_FontManager.GenerateA(Font, Text, std::to_string(Value), TextColor, TextWidth, Align, TextFlags);

		CGLTexture *th = g_Orion.ExecuteGump(Graphic);

		if (th != NULL)
		{
			int textX = m_X;
			int textY = m_Y;

			switch (TextPosition)
			{
				case STP_TOP:
				case STP_TOP_CENTER:
				{
					textY -= Text.Height - DefaultTextOffset;

					break;
				}
				case STP_BOTTOM:
				case STP_BOTTOM_CENTER:
				{
					textY += 18 + DefaultTextOffset;

					break;
				}
				case STP_LEFT:
					//textY -= 4;
				case STP_LEFT_CENTER:
				{
					textX -= Text.Width - DefaultTextOffset;

					break;
				}
				case STP_RIGHT:
					//textY -= 4;
				case STP_RIGHT_CENTER:
				{
					textX += 36 + DefaultTextOffset;

					break;
				}
				default:
					break;
			}

			switch (TextPosition)
			{
				case STP_TOP_CENTER:
				case STP_BOTTOM_CENTER:
				{
					int textWidth = Text.Width;
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
					int textHeight = Text.Height;
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

			TextX = textX;
			TextY = textY;
		}
	}
}
//----------------------------------------------------------------------------------
void CGUIMinMaxButtons::Scroll(int delay)
{
	WISPFUN_DEBUG("c67_f3");
	if (LastScrollTime < g_Ticks && m_ScrollMode)
	{
		if (m_ScrollMode == 1)
			Value += ScrollStep;
		else
			Value -= ScrollStep;

		if (Value < MinValue)
			Value = MinValue;
		else if (Value > MaxValue)
			Value = MaxValue;

		ScrollStep++;
		LastScrollTime = g_Ticks + delay;

		UpdateText();
	}
}
//----------------------------------------------------------------------------------
void CGUIMinMaxButtons::OnClick()
{
	WISPFUN_DEBUG("c67_f4");
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

	LastScrollTime = g_Ticks + 100;
	ScrollStep = BaseScrollStep;
}
//----------------------------------------------------------------------------------
void CGUIMinMaxButtons::SetTextParameters(bool haveText, SLIDER_TEXT_POSITION textPosition, uchar font, ushort color, bool unicode, int textWidth, TEXT_ALIGN_TYPE align, ushort textFlags)
{
	WISPFUN_DEBUG("c67_f5");
	HaveText = haveText;
	TextPosition = textPosition;
	Font = font;
	TextColor = color;
	Unicode = unicode;
	TextWidth = textWidth;
	Align = align;
	TextFlags = textFlags;

	UpdateText();
}
//----------------------------------------------------------------------------------
void CGUIMinMaxButtons::PrepareTextures()
{
	WISPFUN_DEBUG("c67_f6");
	g_Orion.ExecuteGump(Graphic);
	g_Orion.ExecuteGump(Graphic + 1);
}
//----------------------------------------------------------------------------------
void CGUIMinMaxButtons::Draw(bool checktrans)
{
	WISPFUN_DEBUG("c67_f7");
	glUniform1iARB(g_ShaderDrawMode, SDM_NO_COLOR);

	IFOR(i, 0, 2)
	{
		CGLTexture *th = g_Orion.ExecuteGump(Graphic + (int)i);

		if (th != NULL)
			th->Draw(m_X + ((int)i * 18), m_Y, checktrans);
	}

	if (HaveText)
		Text.Draw(TextX, TextY, checktrans);
}
//----------------------------------------------------------------------------------
bool CGUIMinMaxButtons::Select()
{
	WISPFUN_DEBUG("c67_f8");
	int x = g_MouseManager.Position.X - m_X;
	int y = g_MouseManager.Position.Y - m_Y;

	return (x >= 0 && y >= 0 && x < 36 && y < 18);
}
//----------------------------------------------------------------------------------
