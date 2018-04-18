// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GumpDye.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpDye::CGumpDye(uint serial, short x, short y, ushort graphic)
: CGumpSelectColor(serial, x, y, SCGS_OPT_TOOLTIP_TEXT)
{
	NoClose = true;
	GumpType = GT_DYE;
	Graphic = graphic;
}
//----------------------------------------------------------------------------------
CGumpDye::~CGumpDye()
{
}
//----------------------------------------------------------------------------------
ushort CGumpDye::GetCurrentColor()
{
	WISPFUN_DEBUG("c95_f1");
	ushort startColor = m_ColorRef + 2;
	ushort color = 0;

	IFOR(y, 0, 10 && !color)
	{
		IFOR(x, 0, 20)
		{
			if (m_SelectedIndex == ID_GSC_COLORS + (x * 30 + y))
			{
				color = startColor;
				break;
			}

			startColor += 5;
		}
	}

	return color;
}
//----------------------------------------------------------------------------------
void CGumpDye::UpdateContent()
{
	WISPFUN_DEBUG("c95_f2");
	CGumpSelectColor::UpdateContent();

	if (m_Tube == NULL)
	{
		Add(new CGUIShader(&g_ColorizerShader, true));

		m_Tube = (CGUITilepic*)Add(new CGUITilepic(0x0FAB, GetCurrentColor(), 200, 58));
		m_Tube->PartialHue = true;

		Add(new CGUIShader(&g_ColorizerShader, false));
	}
	else
		m_Tube->Color = GetCurrentColor();
}
//----------------------------------------------------------------------------------
void CGumpDye::OnSelectColor(ushort color)
{
	WISPFUN_DEBUG("c95_f3");
	CPacketDyeDataResponse(Serial, Graphic, color + 1).Send();
	RemoveMark = true;
}
//----------------------------------------------------------------------------------
void CGumpDye::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c95_f4");
	CGumpSelectColor::OnButton(serial);

	if (serial >= ID_GSC_COLORS && m_Tube != NULL)
		m_Tube->Color = GetCurrentColor();
}
//----------------------------------------------------------------------------------
void CGumpDye::GUMP_SLIDER_CLICK_EVENT_C
{
	WISPFUN_DEBUG("c95_f5");
	OnSliderMove(serial);
}
//----------------------------------------------------------------------------------
void CGumpDye::GUMP_SLIDER_MOVE_EVENT_C
{
	WISPFUN_DEBUG("c95_f6");
	CGumpSelectColor::OnSliderMove(serial);

	if (m_Tube != NULL)
		m_Tube->Color = GetCurrentColor();
}
//----------------------------------------------------------------------------------
