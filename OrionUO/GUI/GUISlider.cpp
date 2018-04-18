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
CGUISlider::CGUISlider(int serial, ushort graphic, ushort graphicSelected, ushort graphicPressed, ushort backgroundGraphic, bool compositeBackground, bool vertical, int x, int y, int length, int minValue, int maxValue, int value)
: CBaseGUI(GOT_SLIDER, serial, graphic, 0, x, y), GraphicSelected(graphicSelected),
GraphicPressed(graphicPressed), BackgroundGraphic(backgroundGraphic),
CompositeBackground(compositeBackground), Vertical(vertical), Length(length),
MinValue(minValue), MaxValue(maxValue), Value(value)
{
	WISPFUN_DEBUG("c77_f1");
	CalculateOffset();
}
//----------------------------------------------------------------------------------
CGUISlider::~CGUISlider()
{
	WISPFUN_DEBUG("c77_f2");
	Text.Clear();
}
//----------------------------------------------------------------------------------
void CGUISlider::UpdateText()
{
	WISPFUN_DEBUG("c77_f3");
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
					if (Vertical)
						textY += Length + DefaultTextOffset;
					else
						textY += th->Height + DefaultTextOffset;

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
					if (Vertical)
						textX += th->Width + DefaultTextOffset;
					else
						textX += Length + DefaultTextOffset;

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
					int sliderWidth = Length;

					if (Vertical)
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
					int textHeight = Text.Height;
					int sliderHeight = Length;

					if (!Vertical)
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

			TextX = textX;
			TextY = textY;
		}
	}
}
//----------------------------------------------------------------------------------
WISP_GEOMETRY::CSize CGUISlider::GetSize()
{
	WISPFUN_DEBUG("c77_f4");
	WISP_GEOMETRY::CSize size;

	CGLTexture *th = g_Orion.ExecuteGump(Graphic);

	if (th != NULL)
	{
		if (Vertical)
		{
			size.Width = th->Width;
			size.Height = Length;
		}
		else
		{
			size.Width = Length;
			size.Height = th->Height;
		}

		if (HaveText)
		{
			//Text.Draw(TextX, TextY, checktrans);
		}
	}

	return size;
}
//----------------------------------------------------------------------------------
void CGUISlider::OnScroll(bool up, int delay)
{
	WISPFUN_DEBUG("c77_f5");
	if (LastScrollTime < g_Ticks)
	{
		if (up)
			Value += ScrollStep;
		else
			Value -= ScrollStep;

		if (Value < MinValue)
			Value = MinValue;
		else if (Value > MaxValue)
			Value = MaxValue;

		LastScrollTime = g_Ticks + delay;

		CalculateOffset();
		UpdateText();
	}
}
//----------------------------------------------------------------------------------
void CGUISlider::OnClick(int x, int y)
{
	WISPFUN_DEBUG("c77_f6");
	int lenght = Length;
	int maxValue = MaxValue - MinValue;

	CGLTexture *th = g_Orion.ExecuteGump(Graphic);

	if (th != NULL)
		lenght -= (Vertical ? (th->Height / 2) : th->Width);

	float percents = ((Vertical ? y : x) / (float)lenght) * 100.0f;

	Value = (int)((maxValue * percents) / 100.0f) + MinValue;

	CalculateOffset();
	UpdateText();
}
//----------------------------------------------------------------------------------
void CGUISlider::CalculateOffset()
{
	WISPFUN_DEBUG("c77_f7");
	if (Value < MinValue)
		Value = MinValue;
	else if (Value > MaxValue)
		Value = MaxValue;

	int value = Value - MinValue;
	int maxValue = MaxValue - MinValue;
	int lenght = Length;

	CGLTexture *th = g_Orion.ExecuteGump(Graphic);

	if (th != NULL)
		lenght -= (Vertical ? th->Height : th->Width);

	if (maxValue > 0)
		Percents = ((value / (float)maxValue) * 100.0f);
	else
		Percents = 0.0f;

	Offset = (int)((lenght * Percents) / 100.0f);

	if (Offset < 0)
		Offset = 0;
}
//----------------------------------------------------------------------------------
void CGUISlider::SetTextParameters(bool haveText, SLIDER_TEXT_POSITION textPosition, uchar font, ushort color, bool unicode, int textWidth, TEXT_ALIGN_TYPE align, ushort textFlags)
{
	WISPFUN_DEBUG("c77_f8");
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
void CGUISlider::PrepareTextures()
{
	WISPFUN_DEBUG("c77_f9");
	g_Orion.ExecuteGump(Graphic);
	g_Orion.ExecuteGump(GraphicSelected);
	g_Orion.ExecuteGump(GraphicPressed);

	if (BackgroundGraphic)
	{
		if (CompositeBackground)
			g_Orion.ExecuteGumpPart(BackgroundGraphic, 3);
		else
			g_Orion.ExecuteGump(BackgroundGraphic);
	}
}
//----------------------------------------------------------------------------------
ushort CGUISlider::GetDrawGraphic()
{
	WISPFUN_DEBUG("c77_f10");
	ushort graphic = Graphic;

	if (g_GumpPressedElement == this)
		graphic = GraphicPressed;
	else if (g_GumpSelectedElement == this)
		graphic = GraphicSelected;

	return graphic;
}
//----------------------------------------------------------------------------------
void CGUISlider::Draw(bool checktrans)
{
	WISPFUN_DEBUG("c77_f11");
	/*Value++;
	if (Value > MaxValue)
	{
		Value = MinValue;

		TextPosition = (SLIDER_TEXT_POSITION)(TextPosition + 1);

		if (TextPosition > STP_RIGHT)
			TextPosition = STP_TOP;
	}
	UpdateText();
	CalculateOffset();*/

	glUniform1iARB(g_ShaderDrawMode, SDM_NO_COLOR);

	if (BackgroundGraphic)
	{
		if (CompositeBackground)
		{
			CGLTexture *th[3] = { NULL };

			IFOR(i, 0, 3)
			{
				th[i] = g_Orion.ExecuteGump(BackgroundGraphic + (int)i);

				if (th[i] == NULL)
					return;
			}

			if (Vertical)
			{
				th[0]->Draw(m_X, m_Y, checktrans);
				th[2]->Draw(m_X, m_Y + (Length - th[2]->Height), checktrans);
				th[1]->Draw(m_X, m_Y + th[0]->Height, 0, (Length - (th[0]->Height + th[2]->Height)), checktrans);
			}
			else
			{
				th[0]->Draw(m_X, m_Y, checktrans);
				th[2]->Draw(m_X + (Length - th[2]->Width), m_Y, checktrans);
				th[1]->Draw(m_X + th[0]->Width, m_Y, (Length - (th[0]->Width + th[2]->Width)), 0, checktrans);
			}
		}
		else
		{
			CGLTexture *th = g_Orion.ExecuteGump(BackgroundGraphic);

			if (th != NULL)
			{
				if (Vertical)
					th->Draw(m_X, m_Y, 0, Length, checktrans);
				else
					th->Draw(m_X, m_Y, Length, 0, checktrans);
			}
		}
	}

	CGLTexture *th = g_Orion.ExecuteGump(GetDrawGraphic());

	if (th != NULL)
	{
		if (Vertical)
			th->Draw(m_X, m_Y + Offset, checktrans);
		else
			th->Draw(m_X + Offset, m_Y, checktrans);

		if (HaveText)
			Text.Draw(TextX, TextY, checktrans);
	}
}
//----------------------------------------------------------------------------------
bool CGUISlider::Select()
{
	WISPFUN_DEBUG("c77_f12");
	CGLTexture *th = g_Orion.ExecuteGump(Graphic);

	if (th != NULL)
	{
		int buttonX = m_X;
		int buttonY = m_Y;

		if (Vertical)
			buttonY += Offset;
		else
			buttonX += Offset;

		if (th->Select(buttonX, buttonY, !CheckPolygone))
			return true;

		if (BackgroundGraphic)
		{
			int x = g_MouseManager.Position.X - m_X;
			int y = g_MouseManager.Position.Y - m_Y;

			if (x >= 0 && y >= 0)
			{
				if (Vertical)
					return (x < th->Width && y < Length);
				else
					return (x < Length && y < th->Height);
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
