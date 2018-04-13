// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GumpBaseScroll.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpBaseScroll::CGumpBaseScroll(GUMP_TYPE type, uint serial, ushort graphic, int height, short x, short y, bool haveMinimizer, int scrollerOffsetY, bool haveBackgroundLines, int scissorOffsetHeight)
: CGump(type, serial, x, y), Height(height), ScrollerOffsetY(scrollerOffsetY),
HaveBackgroundLines(haveBackgroundLines), ScissorOffsetHeight(scissorOffsetHeight)
{
	WISPFUN_DEBUG("c86_f1");
	Page = 2;
	Add(new CGUIPage(2));

	int offsetY = 0;

	if (haveMinimizer)
	{
		CGLTexture *th = g_Orion.ExecuteGump(0x082D);

		if (th != NULL)
			offsetY = th->Height - 3;
	}

	m_Minimizer = (CGUIButton*)Add(new CGUIButton(ID_GBS_BUTTON_MINIMIZE, 0x082D, 0x082D, 0x082D, 0, 0));
	m_Minimizer->Visible = haveMinimizer;

	m_Background = (CGUIScrollBackground*)Add(new CGUIScrollBackground(0, graphic, 0, offsetY, Height));
	WISP_GEOMETRY::CRect rect = m_Background->WorkSpace;

	if (type != GT_SKILLS)
		m_Minimizer->X = 137;
	else
		m_Minimizer->X = 170;

	int heightDecBonus = ScissorOffsetHeight;

	if (HaveBackgroundLines)
	{
		ScrollerOffsetY += 16;
		heightDecBonus += 16;
	}

	m_HTMLGump = (CGUIHTMLGump*)Add(new CGUIHTMLGump(ID_GBS_HTMLGUMP, 0, rect.Position.X, offsetY + rect.Position.Y + ScrollerOffsetY, rect.Size.Width, rect.Size.Height - ScrollerOffsetY - heightDecBonus, false, true));

	CGUIHTMLButton *button = m_HTMLGump->m_ButtonUp;

	if (button != NULL)
	{
		button->Graphic = 0x0824;
		button->GraphicSelected = 0x0824;
		button->GraphicPressed = 0x0824;
		button->CheckPolygone = true;
	}

	button = m_HTMLGump->m_ButtonDown;

	if (button != NULL)
	{
		button->Graphic = 0x0825;
		button->GraphicSelected = 0x0825;
		button->GraphicPressed = 0x0825;
		button->CheckPolygone = true;
	}

	CGUIHTMLSlider *slider = m_HTMLGump->m_Slider;

	if (slider != NULL)
	{
		slider->Graphic = 0x001F;
		slider->GraphicSelected = 0x001F;
		slider->GraphicPressed = 0x001F;
		slider->BackgroundGraphic = 0;
	}

	m_Resizer = (CGUIResizeButton*)Add(new CGUIResizeButton(ID_GBS_BUTTON_RESIZE, 0x082E, 0x082F, 0x082F, 0, offsetY + Height - 3));

	if (type != GT_SKILLS)
		m_Resizer->X = 137;
	else
		m_Resizer->X = 170;
}
//----------------------------------------------------------------------------------
CGumpBaseScroll::~CGumpBaseScroll()
{
}
//----------------------------------------------------------------------------------
void CGumpBaseScroll::UpdateHeight()
{
	WISPFUN_DEBUG("c86_f2");
	Height = StartResizeHeight + g_MouseManager.LeftDroppedOffset().Y;

	if (Height < m_MinHeight)
		Height = m_MinHeight;

	int maxHeight = GetSystemMetrics(SM_CYSCREEN) - 50;

	if (Height >= maxHeight)
		Height = maxHeight;

	int offsetY = 0;

	CGLTexture *th = g_Orion.ExecuteGump(0x082D);

	if (th != NULL && m_Minimizer->Visible)
		offsetY = th->Height - 3;

	m_Background->UpdateHeight(Height);

	int heightDecBonus = ScissorOffsetHeight;

	if (HaveBackgroundLines)
		heightDecBonus += 16;

	m_HTMLGump->UpdateHeight(m_Background->WorkSpace.Size.Height - ScrollerOffsetY - heightDecBonus);

	m_Resizer->Y = offsetY + Height - 3;
}
//----------------------------------------------------------------------------------
void CGumpBaseScroll::GUMP_RESIZE_START_EVENT_C
{
	WISPFUN_DEBUG("c86_f3");
	StartResizeHeight = Height;
}
//----------------------------------------------------------------------------------
void CGumpBaseScroll::GUMP_RESIZE_EVENT_C
{
	WISPFUN_DEBUG("c86_f4");
	if (StartResizeHeight)
	{
		UpdateHeight();
		RecalculateSize();
	}
}
//----------------------------------------------------------------------------------
void CGumpBaseScroll::GUMP_RESIZE_END_EVENT_C
{
	WISPFUN_DEBUG("c86_f5");
	if (StartResizeHeight)
		StartResizeHeight = 0;
}
//----------------------------------------------------------------------------------
