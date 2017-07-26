//----------------------------------------------------------------------------------
#ifndef WISPMOUSE_H
#define WISPMOUSE_H
#include "WispGeometry.h"
//----------------------------------------------------------------------------------
namespace WISP_MOUSE
{
//----------------------------------------------------------------------------------
class CMouse
{
	SETGET(WISP_GEOMETRY::CPoint2Di, RealPosition, WISP_GEOMETRY::CPoint2Di());
	SETGET(WISP_GEOMETRY::CPoint2Di, Position, WISP_GEOMETRY::CPoint2Di());
	SETGET(WISP_GEOMETRY::CPoint2Di, LeftDropPosition, WISP_GEOMETRY::CPoint2Di());
	SETGET(WISP_GEOMETRY::CPoint2Di, RightDropPosition, WISP_GEOMETRY::CPoint2Di());
	SETGET(WISP_GEOMETRY::CPoint2Di, MidDropPosition, WISP_GEOMETRY::CPoint2Di());

	SETGET(bool, LeftButtonPressed, false);
	SETGET(bool, RightButtonPressed, false);
	SETGET(bool, MidButtonPressed, false);
	SETGET(bool, Dragging, false);
	SETGET(int, DoubleClickDelay, 350 /*GetDoubleClickTime()*/);
	SETGET(bool, CancelDoubleClick, false);

	SETGET(uint, LastLeftButtonClickTimer, 0);
	SETGET(uint, LastRightButtonClickTimer, 0);
	SETGET(uint, LastMidButtonClickTimer, 0);

public:
	CMouse();
	virtual ~CMouse();

	void Update();
	void Update(WPARAM wParam, LPARAM lParam);
	void ReleaseCapture();

	WISP_GEOMETRY::CPoint2Di LeftDroppedOffset();
	WISP_GEOMETRY::CPoint2Di RightDroppedOffset();
	WISP_GEOMETRY::CPoint2Di MidDroppedOffset();
};
//----------------------------------------------------------------------------------
extern CMouse *g_WispMouse;
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
#endif
