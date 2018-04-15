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
CGUIHTMLGump::CGUIHTMLGump(int serial, ushort graphic, int x, int y, int width, int height, bool haveBackground, bool haveScrollbar)
: CGUIPolygonal(GOT_HTMLGUMP, x, y, width, height), HaveBackground(haveBackground),
HaveScrollbar(haveScrollbar), DataSize(), DataOffset(), CurrentOffset(), AvailableOffset()
{
	WISPFUN_DEBUG("c60_f1");
	Serial = serial;
	Graphic = graphic;

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

		m_HitBoxLeft = new CGUIHTMLHitBox(this, Serial, -15, -2, 12, 52);

		m_HitBoxRight = new CGUIHTMLHitBox(this, Serial, Width + 3, -2, 12, 52);
		m_HitBoxRight->Color = 1;

		m_Slider = new CGUIHTMLSlider(this, Serial, 0x00D8, 0x00D8, 0x00D8, 0x00D5, true, false, -10, Height + 1, Width + 20, 0, 0, 0);
		m_Slider->ScrollStep = GUMP_MENU_PIXEL_STEP;
		m_Slider->Vertical = false;

		m_Scissor = new CGUIScissor(true, m_X, m_Y, 0, 0, Width, Height);

		Add(m_Background);
		Add(m_Slider);
		Add(m_HitBoxLeft);
		Add(m_HitBoxRight);
		Add(m_Scissor);
	}
	else
	{
		int offsetWidth = Width;

		if (HaveScrollbar)
		{
			CGLTexture *th = g_Orion.ExecuteGump(0x00FE);

			if (th != NULL)
				offsetWidth -= th->Width;
		}

		m_Background = new CGUIHTMLResizepic(this, 0, Graphic, 0, 0, offsetWidth, Height);
		m_Background->Visible = HaveBackground;

		m_ButtonUp = new CGUIHTMLButton(this, Serial, 0x00FA, 0x00FA, 0x00FA, offsetWidth, 0);
		m_ButtonUp->Visible = HaveScrollbar;

		CGLTexture *thDown = g_Orion.ExecuteGump(0x00FC);

		int sliderHeight = Height;

		if (thDown != NULL)
			sliderHeight -= thDown->Height;

		m_ButtonDown = new CGUIHTMLButton(this, Serial, 0x00FC, 0x00FC, 0x00FC, offsetWidth, sliderHeight);
		m_ButtonDown->Color = 1;
		m_ButtonDown->Visible = HaveScrollbar;

		CGLTexture *thUp = g_Orion.ExecuteGump(0x00FA);

		int sliderStartY = 0;

		if (thUp != NULL)
		{
			sliderStartY = thUp->Height;
			sliderHeight -= sliderStartY;
		}

		m_Slider = new CGUIHTMLSlider(this, Serial, 0x00FE, 0x00FE, 0x00FE, 0x0100, false, true, offsetWidth, sliderStartY, sliderHeight, 0, 0, 0);
		//m_Slider->SetTextParameters(true, STP_RIGHT_CENTER, 3, 0x0021, false);
		m_Slider->Visible = HaveScrollbar;

		m_Scissor = new CGUIScissor(true, m_X, m_Y, 0, 0, offsetWidth, Height);

		Add(m_Background);
		Add(m_Slider);
		Add(m_ButtonUp);
		Add(m_ButtonDown);
		Add(m_Scissor);
	}
}
//----------------------------------------------------------------------------------
void CGUIHTMLGump::UpdateHeight(int height)
{
	WISPFUN_DEBUG("c60_f4");
	Height = height;

	m_Background->Height = height;

	CGLTexture *thDown = g_Orion.ExecuteGump(m_ButtonDown->Graphic);

	int sliderHeight = height;

	if (thDown != NULL)
		sliderHeight -= thDown->Height;

	m_ButtonDown->SetY(sliderHeight);

	CGLTexture *thUp = g_Orion.ExecuteGump(m_ButtonUp->Graphic);

	int sliderStartY = 0;

	if (thUp != NULL)
	{
		sliderStartY = thUp->Height;
		sliderHeight -= sliderStartY;
	}

	m_Slider->Length = sliderHeight;

	m_Scissor->Height = height;

	CalculateDataSize();
}
//----------------------------------------------------------------------------------
void CGUIHTMLGump::ResetDataOffset()
{
	WISPFUN_DEBUG("c60_f5");
	m_Slider->Value = 0;
	CurrentOffset.Reset();
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

		if (item->GetX() < startX)
			startX = item->GetX();

		if (item->GetY() < startY)
			startY = item->GetY();

		WISP_GEOMETRY::CSize size = item->GetSize();

		int curX = item->GetX() + size.Width;
		int curY = item->GetY() + size.Height;

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

	DataSize.Width = abs(startX) + abs(endX);
	DataSize.Height = abs(startY) + abs(endY);

	DataOffset.X = startX;
	DataOffset.Y = startY;

	AvailableOffset.X = DataSize.Width - m_Scissor->Width;

	if (AvailableOffset.X < 0)
		AvailableOffset.X = 0;

	AvailableOffset.Y = DataSize.Height - m_Scissor->Height;

	if (AvailableOffset.Y < 0)
		AvailableOffset.Y = 0;

	m_Slider->MinValue = 0;

	if (m_Slider->Vertical)
		m_Slider->MaxValue = AvailableOffset.Y;
	else
		m_Slider->MaxValue = AvailableOffset.X;

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
void CGUIHTMLGump::Scroll(bool up, int delay)
{
	WISPFUN_DEBUG("c60_f10");
	if (m_Slider != NULL)
		m_Slider->OnScroll(up, delay);
}
//----------------------------------------------------------------------------------
