// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//----------------------------------------------------------------------------------
#include "stdafx.h"

namespace WISP_MOUSE
{
//forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable: 4800)

CMouse *g_WispMouse = NULL;
//----------------------------------------------------------------------------------
CMouse::CMouse()
{
	g_WispMouse = this;
}
//----------------------------------------------------------------------------------
CMouse::~CMouse()
{
	g_WispMouse = NULL;
}
//----------------------------------------------------------------------------------
void CMouse::Update()
{
	WISPFUN_DEBUG("c8_f1");
	POINT pos;
	GetCursorPos(&pos);

	if (WISP_WINDOW::g_WispWindow != NULL)
		ScreenToClient(WISP_WINDOW::g_WispWindow->Handle, &pos);

	m_Dragging = (m_LeftButtonPressed || m_RightButtonPressed || m_MidButtonPressed);

	m_Position.X = pos.x;
	m_Position.Y = pos.y;

	m_RealPosition = m_Position;
}
//----------------------------------------------------------------------------------
void CMouse::Update(WPARAM wParam, LPARAM lParam)
{
	WISPFUN_DEBUG("c8_f2");
	m_LeftButtonPressed = (bool)(wParam & MK_LBUTTON);
	m_RightButtonPressed = (bool)(wParam & MK_RBUTTON);
	m_MidButtonPressed = (bool)(wParam & MK_MBUTTON);

	m_Dragging = (m_LeftButtonPressed || m_RightButtonPressed || m_MidButtonPressed);

	POINT pos;
	GetCursorPos(&pos);

	if (WISP_WINDOW::g_WispWindow != NULL)
		ScreenToClient(WISP_WINDOW::g_WispWindow->Handle, &pos);

	m_Position.X = pos.x;
	m_Position.Y = pos.y;

	//m_Position.X = LOWORD(lParam);
	//m_Position.Y = HIWORD(lParam);

	m_RealPosition = m_Position;
}
//----------------------------------------------------------------------------------
void CMouse::ReleaseCapture()
{
	WISPFUN_DEBUG("c8_f3");
	if (!(m_LeftButtonPressed || m_RightButtonPressed || m_MidButtonPressed))
		::ReleaseCapture();
}
//----------------------------------------------------------------------------------
WISP_GEOMETRY::CPoint2Di CMouse::LeftDroppedOffset()
{
	WISPFUN_DEBUG("c8_f4");
	WISP_GEOMETRY::CPoint2Di position;

	if (m_LeftButtonPressed)
	{
		position.X = m_RealPosition.X - m_LeftDropPosition.X;
		position.Y = m_RealPosition.Y - m_LeftDropPosition.Y;
	}

	return position;
}
//----------------------------------------------------------------------------------
WISP_GEOMETRY::CPoint2Di CMouse::RightDroppedOffset()
{
	WISPFUN_DEBUG("c8_f5");
	WISP_GEOMETRY::CPoint2Di position;

	if (m_RightButtonPressed)
	{
		position.X = m_RealPosition.X - m_RightDropPosition.X;
		position.Y = m_RealPosition.Y - m_RightDropPosition.Y;
	}

	return position;
}
//----------------------------------------------------------------------------------
WISP_GEOMETRY::CPoint2Di CMouse::MidDroppedOffset()
{
	WISPFUN_DEBUG("c8_f6");
	WISP_GEOMETRY::CPoint2Di position;

	if (m_MidButtonPressed)
	{
		position.X = m_RealPosition.X - m_MidDropPosition.X;
		position.Y = m_RealPosition.Y - m_MidDropPosition.Y;
	}

	return position;
}
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
