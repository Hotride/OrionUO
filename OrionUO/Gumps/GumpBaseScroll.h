/***********************************************************************************
**
** GumpBaseScroll.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPBASESCROLL_H
#define GUMPBASESCROLL_H
//----------------------------------------------------------------------------------
#include "Gump.h"
//----------------------------------------------------------------------------------
class CGumpBaseScroll : public CGump
{
	SETGET(int, Height);
	SETGET(int, StartResizeHeight);
	SETGET(int, ScrollerOffsetY);
	SETGET(int, ScissorOffsetHeight);
	SETGET(bool, HaveBackgroundLines);

protected:
	static const int ID_GBS_HTMLGUMP = 0xFFFFFFF0;
	static const int ID_GBS_BUTTON_MINIMIZE = 0xFFFFFFF1;
	static const int ID_GBS_BUTTON_RESIZE = 0xFFFFFFF2;

	CGUIButton *m_Minimizer;
	CGUIScrollBackground *m_Background;
	CGUIHTMLGump *m_HTMLGump;
	CGUIResizeButton *m_Resizer;

	int m_MinHeight;

	virtual void UpdateHeight();

public:
	CGumpBaseScroll(GUMP_TYPE type, uint serial, ushort graphic, int height, short x, short y, bool haveMinimizer, int scrollerOffsetY = 0, bool haveBackgroundLines = false, int scissorOffsetHeight = 0);
	virtual ~CGumpBaseScroll();

	GUMP_RESIZE_START_EVENT_H;
	GUMP_RESIZE_EVENT_H;
	GUMP_RESIZE_END_EVENT_H;
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
