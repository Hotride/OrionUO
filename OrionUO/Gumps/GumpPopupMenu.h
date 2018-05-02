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
public:
    uint Cliloc = 0;
    ushort Index = 0;
    ushort Flags = 0;
    ushort Color = 0;
    ushort ReplaceColor = 0;

    CPopupMenuItemInfo() {}
};
//----------------------------------------------------------------------------------
class CGumpPopupMenu : public CGump
{
    int Width = 0;
    int Height = 0;

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
