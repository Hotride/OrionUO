/***********************************************************************************
**
** GumpPopupMenu.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPPOPUPMENU_H
#define GUMPPOPUPMENU_H
//----------------------------------------------------------------------------------
#include "Gump.h"
//----------------------------------------------------------------------------------
class CGumpPopupMenu : public CGump
{
	SETGET(int, Width);
	SETGET(int, Height);

private:
	CGUIColoredPolygone *m_Polygone;

public:
	CGumpPopupMenu(uint serial, short x, short y);
	virtual ~CGumpPopupMenu();

	virtual void PrepareContent();

	GUMP_BUTTON_EVENT_H;
};
//----------------------------------------------------------------------------------
extern CGumpPopupMenu *g_PopupMenu;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
