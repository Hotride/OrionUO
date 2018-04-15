// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIHTMLSlider.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIHTMLSlider::CGUIHTMLSlider(CGUIHTMLGump *htmlGump, int serial, ushort graphic, ushort graphicSelected, ushort graphicPressed, ushort backgroundGraphic, bool compositeBackground, bool vertical, int x, int y, int lenght, int minValue, int maxValue, int value)
: CGUISlider(serial, graphic, graphicSelected, graphicPressed, backgroundGraphic, compositeBackground, vertical, x, y, lenght, minValue, maxValue, value),
m_HTMLGump(htmlGump)
{
}
//----------------------------------------------------------------------------------
CGUIHTMLSlider::~CGUIHTMLSlider()
{
}
//----------------------------------------------------------------------------------
void CGUIHTMLSlider::CalculateOffset()
{
	WISPFUN_DEBUG("c63_f1");
	CGUISlider::CalculateOffset();

	if (m_HTMLGump != NULL)
	{
		WISP_GEOMETRY::CPoint2Di currentOffset = m_HTMLGump->CurrentOffset;
		WISP_GEOMETRY::CPoint2Di availableOffset = m_HTMLGump->AvailableOffset;

		if (Vertical)
			currentOffset.Y = (int)((availableOffset.Y * Percents) / 100.0f);
		else
			currentOffset.X = (int)((availableOffset.X * Percents) / 100.0f);

		m_HTMLGump->CurrentOffset = currentOffset;
	}
}
//----------------------------------------------------------------------------------
