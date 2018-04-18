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

	Dragging = (LeftButtonPressed || RightButtonPressed || MidButtonPressed);

	Position.X = pos.x;
	Position.Y = pos.y;

	RealPosition = Position;
}
//----------------------------------------------------------------------------------
void CMouse::Update(WPARAM wParam, LPARAM lParam)
{
	WISPFUN_DEBUG("c8_f2");
	LeftButtonPressed = (bool)(wParam & MK_LBUTTON);
	RightButtonPressed = (bool)(wParam & MK_RBUTTON);
	MidButtonPressed = (bool)(wParam & MK_MBUTTON);

	Dragging = (LeftButtonPressed || RightButtonPressed || MidButtonPressed);

	POINT pos;
	GetCursorPos(&pos);

	if (WISP_WINDOW::g_WispWindow != NULL)
		ScreenToClient(WISP_WINDOW::g_WispWindow->Handle, &pos);

	Position.X = pos.x;
	Position.Y = pos.y;

	//Position.X = LOWORD(lParam);
	//Position.Y = HIWORD(lParam);

	RealPosition = Position;
}
//----------------------------------------------------------------------------------
void CMouse::ReleaseCapture()
{
	WISPFUN_DEBUG("c8_f3");
	if (!(LeftButtonPressed || RightButtonPressed || MidButtonPressed))
		::ReleaseCapture();
}
//----------------------------------------------------------------------------------
WISP_GEOMETRY::CPoint2Di CMouse::LeftDroppedOffset()
{
	WISPFUN_DEBUG("c8_f4");
	WISP_GEOMETRY::CPoint2Di position;

	if (LeftButtonPressed)
	{
		position.X = RealPosition.X - LeftDropPosition.X;
		position.Y = RealPosition.Y - LeftDropPosition.Y;
	}

	return position;
}
//----------------------------------------------------------------------------------
WISP_GEOMETRY::CPoint2Di CMouse::RightDroppedOffset()
{
	WISPFUN_DEBUG("c8_f5");
	WISP_GEOMETRY::CPoint2Di position;

	if (RightButtonPressed)
	{
		position.X = RealPosition.X - RightDropPosition.X;
		position.Y = RealPosition.Y - RightDropPosition.Y;
	}

	return position;
}
//----------------------------------------------------------------------------------
WISP_GEOMETRY::CPoint2Di CMouse::MidDroppedOffset()
{
	WISPFUN_DEBUG("c8_f6");
	WISP_GEOMETRY::CPoint2Di position;

	if (MidButtonPressed)
	{
		position.X = RealPosition.X - MidDropPosition.X;
		position.Y = RealPosition.Y - MidDropPosition.Y;
	}

	return position;
}
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
