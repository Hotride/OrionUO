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
	SETGET(int, Width, 0);
	SETGET(int, Height, 0);

private:
	CGUIColoredPolygone *m_Polygone{ NULL };

public:
	CGumpPopupMenu(uint serial, short x, short y);
	virtual ~CGumpPopupMenu();

	static const int ID_GPM_MAXIMIZE = 0xDEADBEEF;

	virtual void PrepareContent();

	GUMP_BUTTON_EVENT_H;
};
//----------------------------------------------------------------------------------
extern CGumpPopupMenu *g_PopupMenu;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
