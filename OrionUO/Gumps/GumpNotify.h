/***********************************************************************************
**
** GumpNotify.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPNOTIFY_H
#define GUMPNOTIFY_H
//----------------------------------------------------------------------------------
class CGumpNotify : public CGump
{
    uchar Variant = 0;
    short Width = 0;
    short Height = 0;
    string Text = "";

private:
    static const int ID_GN_BUTTON_OK = 1;

    void Process();

public:
    CGumpNotify(short x, short y, uchar variant, short width, short height, string text);
    virtual ~CGumpNotify();

    static const int ID_GN_STATE_LOGOUT = 1;
    static const int ID_GN_STATE_NOTIFICATION = 2;

    GUMP_BUTTON_EVENT_H;
    void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
