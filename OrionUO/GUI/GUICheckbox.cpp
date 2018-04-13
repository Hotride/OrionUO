// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUICheckbox.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUICheckbox::CGUICheckbox(const uint &serial, const ushort &graphic, const ushort &graphicChecked, const ushort &graphicDisabled, const int &x, const int &y)
: CGUIDrawObject(GOT_CHECKBOX, serial, graphic, 0, x, y), GraphicChecked(graphicChecked),
GraphicSelected(graphic), GraphicDisabled(graphicDisabled)
{
}
//----------------------------------------------------------------------------------
CGUICheckbox::~CGUICheckbox()
{
	WISPFUN_DEBUG("c46_f1");
	Text.Clear();
}
//----------------------------------------------------------------------------------
void CGUICheckbox::SetTextParameters(const uchar &font, const wstring &text, const ushort &color, const SLIDER_TEXT_POSITION &textPosition, const int &textWidth, const TEXT_ALIGN_TYPE &align, const ushort &textFlags)
{
	WISPFUN_DEBUG("c46_f2");
	TextPosition = textPosition;
	g_FontManager.GenerateW(font, Text, text, color, 30, textWidth, align, textFlags);
	UpdateTextPosition();
}
//----------------------------------------------------------------------------------
void CGUICheckbox::SetTextParameters(const uchar &font, const string &text, const ushort &color, const SLIDER_TEXT_POSITION &textPosition, const int &textWidth, const TEXT_ALIGN_TYPE &align, const ushort &textFlags)
{
	WISPFUN_DEBUG("c46_f3");
	TextPosition = textPosition;
	g_FontManager.GenerateA(font, Text, text, color, textWidth, align, textFlags);
	UpdateTextPosition();
}
//----------------------------------------------------------------------------------
void CGUICheckbox::UpdateTextPosition()
{
	WISPFUN_DEBUG("c46_f4");
	int textX = m_X;
	int textY = m_Y;

	CGLTexture *th = g_Orion.ExecuteGump(Graphic);

	if (th != NULL)
	{
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
				textX += th->Width + DefaultTextOffset;

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
				int sliderWidth = th->Width;

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
				int sliderHeight = th->Height;

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
	}

	TextX = textX;
	TextY = textY;
}
//----------------------------------------------------------------------------------
void CGUICheckbox::PrepareTextures()
{
	WISPFUN_DEBUG("c46_f5");
	g_Orion.ExecuteGump(Graphic);
	g_Orion.ExecuteGump(GraphicChecked);
	g_Orion.ExecuteGump(GraphicDisabled);
}
//----------------------------------------------------------------------------------
ushort CGUICheckbox::GetDrawGraphic()
{
	WISPFUN_DEBUG("c46_f6");
	ushort graphic = Graphic;

	if (!Enabled)
		graphic = GraphicDisabled;
	else if (Checked)
		graphic = GraphicChecked;
	else if (g_SelectedObject.Object == this)
		graphic = GraphicSelected;

	return graphic;
}
//----------------------------------------------------------------------------------
void CGUICheckbox::Draw(const bool &checktrans)
{
	WISPFUN_DEBUG("c46_f7");
	CGUIDrawObject::Draw(checktrans);

	Text.Draw(TextX, TextY, checktrans);
}
//----------------------------------------------------------------------------------
bool CGUICheckbox::Select()
{
	WISPFUN_DEBUG("c46_f8");
	bool result = CGUIDrawObject::Select();

	if (!result && !Text.Empty())
	{
		int x = g_MouseManager.Position.X - TextX;
		int y = g_MouseManager.Position.Y - TextY;

		result = (x >= 0 && y >= 0 && x < Text.Width && y < Text.Height);
	}

	return result;
}
//----------------------------------------------------------------------------------
void CGUICheckbox::OnMouseEnter()
{
	WISPFUN_DEBUG("c46_f9");
	if (Graphic != GraphicSelected && g_SelectedObject.Gump != NULL)
		g_SelectedObject.Gump->WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGUICheckbox::OnMouseExit()
{
	WISPFUN_DEBUG("c46_f10");
	if (Graphic != GraphicSelected && g_LastSelectedObject.Gump != NULL)
		g_LastSelectedObject.Gump->WantRedraw = true;
}
//----------------------------------------------------------------------------------
