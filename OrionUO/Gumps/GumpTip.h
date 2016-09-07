/***********************************************************************************
**
** GumpTip.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPTIP_H
#define GUMPTIP_H
//----------------------------------------------------------------------------------
#include "GumpBaseScroll.h"
//----------------------------------------------------------------------------------
class CGumpTip : public CGumpBaseScroll
{
	SETGET(bool, Updates);

private:
	static const int ID_GT_PREV_GUMP = 1;
	static const int ID_GT_REMOVE_GUMP = 2;
	static const int ID_GT_NEXT_GUMP = 3;

	CGUIButton *m_ButtonPrevGump;
	CGUIButton *m_ButtonNextGump;

	virtual void UpdateHeight();

public:
	CGumpTip(uint serial, short x, short y, string str, bool updates);
	virtual ~CGumpTip();

	void SendTipRequest(uchar flag);

	GUMP_BUTTON_EVENT_H;

	virtual bool OnLeftMouseButtonDoubleClick();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
