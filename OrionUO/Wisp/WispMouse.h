//----------------------------------------------------------------------------------
#ifndef WISPMOUSE_H
#define WISPMOUSE_H
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
	int DoubleClickDelay = 350 /*GetDoubleClickTime()*/;
	bool CancelDoubleClick = false;

	uint LastLeftButtonClickTimer = 0;
	uint LastRightButtonClickTimer = 0;
	uint LastMidButtonClickTimer = 0;

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
