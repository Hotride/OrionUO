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
	SETGET(WISP_GEOMETRY::CPoint2Di, RealPosition);
	SETGET(WISP_GEOMETRY::CPoint2Di, Position);
	SETGET(WISP_GEOMETRY::CPoint2Di, LeftDropPosition);
	SETGET(WISP_GEOMETRY::CPoint2Di, RightDropPosition);
	SETGET(WISP_GEOMETRY::CPoint2Di, MidDropPosition);

	SETGET(bool, LeftButtonPressed);
	SETGET(bool, RightButtonPressed);
	SETGET(bool, MidButtonPressed);
	SETGET(bool, Dragging);
	SETGET(int, DoubleClickDelay);
	SETGET(bool, CancelDoubleClick);

	SETGET(uint, LastLeftButtonClickTimer);
	SETGET(uint, LastRightButtonClickTimer);
	SETGET(uint, LastMidButtonClickTimer);

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
