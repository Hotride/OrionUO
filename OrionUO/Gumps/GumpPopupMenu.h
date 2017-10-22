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
class CPopupMenuItemInfo
{
	SETGET(uint, Cliloc, 0);
	SETGET(ushort, Index, 0);
	SETGET(ushort, Flags, 0);
	SETGET(ushort, Color, 0);
	SETGET(ushort, ReplaceColor, 0);

public:
	CPopupMenuItemInfo() {}
};
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

	static void Parse(WISP_NETWORK::CPacketReader &reader);

	GUMP_BUTTON_EVENT_H;
};
//----------------------------------------------------------------------------------
extern CGumpPopupMenu *g_PopupMenu;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
