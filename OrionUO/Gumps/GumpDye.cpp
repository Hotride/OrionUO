/***********************************************************************************
**
** GumpDye.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GumpDye.h"
#include "../Network/Packets.h"
//----------------------------------------------------------------------------------
CGumpDye::CGumpDye(uint serial, short x, short y, ushort graphic)
: CGumpSelectColor(serial, x, y, SCGS_OPT_TOOLTIP_TEXT), m_Tube(NULL)
{
	m_NoClose = true;
	m_GumpType = GT_DYE;
	m_Graphic = graphic;
}
//----------------------------------------------------------------------------------
CGumpDye::~CGumpDye()
{
}
//----------------------------------------------------------------------------------
ushort CGumpDye::GetCurrentColor()
{
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
	CGumpSelectColor::UpdateContent();

	if (m_Tube == NULL)
	{
		Add(new CGUIShader(g_ColorizerShader, true));

		m_Tube = (CGUITilepic*)Add(new CGUITilepic(0x0FAB, GetCurrentColor(), 200, 58));
		m_Tube->PartialHue = true;

		Add(new CGUIShader(g_ColorizerShader, false));
	}
	else
		m_Tube->Color = GetCurrentColor();
}
//----------------------------------------------------------------------------------
void CGumpDye::OnSelectColor(const ushort &color)
{
	CPacketDyeDataResponse(m_Serial, m_Graphic, color).Send();
	m_RemoveMark = true;
}
//----------------------------------------------------------------------------------
void CGumpDye::GUMP_BUTTON_EVENT_C
{
	CGumpSelectColor::OnButton(serial);

	if (serial >= ID_GSC_COLORS && m_Tube != NULL)
		m_Tube->Color = GetCurrentColor();
}
//----------------------------------------------------------------------------------
void CGumpDye::GUMP_SLIDER_CLICK_EVENT_C
{
	OnSliderMove(serial);
}
//----------------------------------------------------------------------------------
void CGumpDye::GUMP_SLIDER_MOVE_EVENT_C
{
	CGumpSelectColor::OnSliderMove(serial);

	if (m_Tube != NULL)
		m_Tube->Color = GetCurrentColor();
}
//----------------------------------------------------------------------------------
