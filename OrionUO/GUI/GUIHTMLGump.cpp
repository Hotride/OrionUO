// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIHTMLGump.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIHTMLGump::CGUIHTMLGump(const uint &serial, const ushort &graphic, const int &x, const int &y, const int &width, const int &height, const bool &haveBackground, const bool &haveScrollbar)
: CGUIPolygonal(GOT_HTMLGUMP, x, y, width, height), m_HaveBackground(haveBackground),
m_HaveScrollbar(haveScrollbar), m_DataSize(), m_DataOffset(), m_CurrentOffset(),
m_AvailableOffset()
{
	WISPFUN_DEBUG("c60_f1");
	m_Serial = serial;
	m_Graphic = graphic;

	Initalize();
}
//----------------------------------------------------------------------------------
CGUIHTMLGump::~CGUIHTMLGump()
{
}
//----------------------------------------------------------------------------------
void CGUIHTMLGump::PrepareTextures()
{
	WISPFUN_DEBUG("c60_f2");
	QFOR(item, m_Items, CBaseGUI*)
		item->PrepareTextures();
}
//----------------------------------------------------------------------------------
void CGUIHTMLGump::Initalize(bool menu)
{
	WISPFUN_DEBUG("c60_f3");
	Clear();

	if (menu)
	{
		m_Background = new CGUIHTMLResizepic(this, 0, 0, 0, 0, 1, 1);
		m_Background->Visible = false;

		m_HitBoxLeft = new CGUIHTMLHitBox(this, m_Serial, -15, -2, 12, 52);

		m_HitBoxRight = new CGUIHTMLHitBox(this, m_Serial, m_Width + 3, -2, 12, 52);
		m_HitBoxRight->Color = 1;

		m_Slider = new CGUIHTMLSlider(this, m_Serial, 0x00D8, 0x00D8, 0x00D8, 0x00D5, true, false, -10, m_Height + 1, m_Width + 20, 0, 0, 0);
		m_Slider->ScrollStep = GUMP_MENU_PIXEL_STEP;
		m_Slider->Vertical = false;

		m_Scissor = new CGUIScissor(true, m_X, m_Y, 0, 0, m_Width, m_Height);

		Add(m_Background);
		Add(m_Slider);
		Add(m_HitBoxLeft);
		Add(m_HitBoxRight);
		Add(m_Scissor);
	}
	else
	{
		int offsetWidth = m_Width;

		if (m_HaveScrollbar)
		{
			CGLTexture *th = g_Orion.ExecuteGump(0x00FE);

			if (th != NULL)
				offsetWidth -= th->Width;
		}

		m_Background = new CGUIHTMLResizepic(this, 0, m_Graphic, 0, 0, offsetWidth, m_Height);
		m_Background->Visible = m_HaveBackground;

		m_ButtonUp = new CGUIHTMLButton(this, m_Serial, 0x00FA, 0x00FA, 0x00FA, offsetWidth, 0);
		m_ButtonUp->Visible = m_HaveScrollbar;

		CGLTexture *thDown = g_Orion.ExecuteGump(0x00FC);

		int sliderHeight = m_Height;

		if (thDown != NULL)
			sliderHeight -= thDown->Height;

		m_ButtonDown = new CGUIHTMLButton(this, m_Serial, 0x00FC, 0x00FC, 0x00FC, offsetWidth, sliderHeight);
		m_ButtonDown->Color = 1;
		m_ButtonDown->Visible = m_HaveScrollbar;

		CGLTexture *thUp = g_Orion.ExecuteGump(0x00FA);

		int sliderStartY = 0;

		if (thUp != NULL)
		{
			sliderStartY = thUp->Height;
			sliderHeight -= sliderStartY;
		}

		m_Slider = new CGUIHTMLSlider(this, m_Serial, 0x00FE, 0x00FE, 0x00FE, 0x0100, false, true, offsetWidth, sliderStartY, sliderHeight, 0, 0, 0);
		//m_Slider->SetTextParameters(true, STP_RIGHT_CENTER, 3, 0x0021, false);
		m_Slider->Visible = m_HaveScrollbar;

		m_Scissor = new CGUIScissor(true, m_X, m_Y, 0, 0, offsetWidth, m_Height);

		Add(m_Background);
		Add(m_Slider);
		Add(m_ButtonUp);
		Add(m_ButtonDown);
		Add(m_Scissor);
	}
}
//----------------------------------------------------------------------------------
void CGUIHTMLGump::UpdateHeight(const int &height)
{
	WISPFUN_DEBUG("c60_f4");
	m_Height = height;

	m_Background->Height = height;

	CGLTexture *thDown = g_Orion.ExecuteGump(m_ButtonDown->Graphic);

	int sliderHeight = height;

	if (thDown != NULL)
		sliderHeight -= thDown->Height;

	m_ButtonDown->Y = sliderHeight;

	CGLTexture *thUp = g_Orion.ExecuteGump(m_ButtonUp->Graphic);

	int sliderStartY = 0;

	if (thUp != NULL)
	{
		sliderStartY = thUp->Height;
		sliderHeight -= sliderStartY;
	}

	m_Slider->Lenght = sliderHeight;

	m_Scissor->Height = height;

	CalculateDataSize();
}
//----------------------------------------------------------------------------------
void CGUIHTMLGump::ResetDataOffset()
{
	WISPFUN_DEBUG("c60_f5");
	m_Slider->Value = 0;
	m_CurrentOffset.Reset();
}
//----------------------------------------------------------------------------------
void CGUIHTMLGump::CalculateDataSize(CBaseGUI *item, int &startX, int &startY, int &endX, int &endY)
{
	WISPFUN_DEBUG("c60_f6");
	for (; item != NULL; item = (CBaseGUI*)item->m_Next)
	{
		if (item->Type == GOT_HITBOX || !item->Visible)
			continue;
		else if (item->Type == GOT_DATABOX)
		{
			CalculateDataSize((CBaseGUI*)item->m_Items, startX, startY, endX, endY);
			continue;
		}

		if (item->X < startX)
			startX = item->X;

		if (item->Y < startY)
			startY = item->Y;

		WISP_GEOMETRY::CSize size = item->GetSize();

		int curX = item->X + size.Width;
		int curY = item->Y + size.Height;

		if (curX > endX)
			endX = curX;

		if (curY > endY)
			endY = curY;
	}
}
//----------------------------------------------------------------------------------
void CGUIHTMLGump::CalculateDataSize()
{
	WISPFUN_DEBUG("c60_f7");
	CBaseGUI *item = (CBaseGUI*)m_Items;

	IFOR(i, 0, 5)
		item = (CBaseGUI*)item->m_Next;

	int startX = 0;
	int startY = 0;
	int endX = 0;
	int endY = 0;

	CalculateDataSize(item, startX, startY, endX, endY);

	m_DataSize.Width = abs(startX) + abs(endX);
	m_DataSize.Height = abs(startY) + abs(endY);

	m_DataOffset.X = startX;
	m_DataOffset.Y = startY;

	m_AvailableOffset.X = m_DataSize.Width - m_Scissor->Width;

	if (m_AvailableOffset.X < 0)
		m_AvailableOffset.X = 0;

	m_AvailableOffset.Y = m_DataSize.Height - m_Scissor->Height;

	if (m_AvailableOffset.Y < 0)
		m_AvailableOffset.Y = 0;

	m_Slider->MinValue = 0;

	if (m_Slider->Vertical)
		m_Slider->MaxValue = m_AvailableOffset.Y;
	else
		m_Slider->MaxValue = m_AvailableOffset.X;

	m_Slider->CalculateOffset();
}
//----------------------------------------------------------------------------------
bool CGUIHTMLGump::EntryPointerHere()
{
	WISPFUN_DEBUG("c60_f8");
	QFOR(item, m_Items, CBaseGUI*)
	{
		if (item->Visible && item->EntryPointerHere())
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------------
bool CGUIHTMLGump::Select()
{
	WISPFUN_DEBUG("c60_f9");
	WISP_GEOMETRY::CPoint2Di oldPos = g_MouseManager.Position;
	g_MouseManager.Position = WISP_GEOMETRY::CPoint2Di(oldPos.X - m_X, oldPos.Y - m_Y);

	bool selected = false;

	CBaseGUI *item = (CBaseGUI*)m_Items;

	IFOR(i, 0, 5 && !selected)
	{
		selected = item->Select();

		item = (CBaseGUI*)item->m_Next;
	}

	g_MouseManager.Position = oldPos;

	return selected;
}
//----------------------------------------------------------------------------------
void CGUIHTMLGump::Scroll(const bool &up, const uint &delay)
{
	WISPFUN_DEBUG("c60_f10");
	if (m_Slider != NULL)
		m_Slider->OnScroll(up, delay);
}
//----------------------------------------------------------------------------------
