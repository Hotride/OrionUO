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
class CGumpBaseScroll : public CGump
{
public:
    int Height = 0;
    int StartResizeHeight = 0;
    int ScrollerOffsetY = 0;
    int ScissorOffsetHeight = 0;
    bool HaveBackgroundLines = false;

protected:
    static const int ID_GBS_HTMLGUMP = 0xFFFFFFF0;
    static const int ID_GBS_BUTTON_MINIMIZE = 0xFFFFFFF1;
    static const int ID_GBS_BUTTON_RESIZE = 0xFFFFFFF2;

    CGUIButton *m_Minimizer{ NULL };
    CGUIScrollBackground *m_Background{ NULL };
    CGUIHTMLGump *m_HTMLGump{ NULL };
    CGUIResizeButton *m_Resizer{ NULL };

    int m_MinHeight{ 160 };

    virtual void UpdateHeight();

public:
    CGumpBaseScroll(
        GUMP_TYPE type,
        uint serial,
        ushort graphic,
        int height,
        short x,
        short y,
        bool haveMinimizer,
        int scrollerOffsetY = 0,
        bool haveBackgroundLines = false,
        int scissorOffsetHeight = 0);
    virtual ~CGumpBaseScroll();

    GUMP_RESIZE_START_EVENT_H;
    GUMP_RESIZE_EVENT_H;
    GUMP_RESIZE_END_EVENT_H;
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
