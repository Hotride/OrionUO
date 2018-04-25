//----------------------------------------------------------------------------------
#pragma once
#include <SDL_events.h>
//----------------------------------------------------------------------------------
namespace WISP_MOUSE
{
//----------------------------------------------------------------------------------
class CMouse
{
public:
	WISP_GEOMETRY::CPoint2Di RealPosition = WISP_GEOMETRY::CPoint2Di();
	WISP_GEOMETRY::CPoint2Di Position = WISP_GEOMETRY::CPoint2Di();
	WISP_GEOMETRY::CPoint2Di LeftDropPosition = WISP_GEOMETRY::CPoint2Di();
	WISP_GEOMETRY::CPoint2Di RightDropPosition = WISP_GEOMETRY::CPoint2Di();
	WISP_GEOMETRY::CPoint2Di MidDropPosition = WISP_GEOMETRY::CPoint2Di();

	bool LeftButtonPressed = false;
	bool RightButtonPressed = false;
	bool MidButtonPressed = false;
	bool Dragging = false;
	int DoubleClickDelay = 350;
	bool CancelDoubleClick = false;

	uint LastLeftButtonClickTimer = 0;
	uint LastRightButtonClickTimer = 0;
	uint LastMidButtonClickTimer = 0;

	CMouse();
	virtual ~CMouse();

	void Update();
	void Release() const;
	void Capture() const;

	WISP_GEOMETRY::CPoint2Di LeftDroppedOffset();
	WISP_GEOMETRY::CPoint2Di RightDroppedOffset();
	WISP_GEOMETRY::CPoint2Di MidDroppedOffset();
};
//----------------------------------------------------------------------------------
extern CMouse *g_WispMouse;
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
