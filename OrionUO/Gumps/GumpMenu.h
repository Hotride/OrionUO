/***********************************************************************************
**
** GumpMenu.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPMENU_H
#define GUMPMENU_H
//----------------------------------------------------------------------------------
class CGumpMenu : public CGump
{
public:
    string Text = "";
    bool TextChanged = false;

protected:
    virtual void CalculateGumpState();

public:
    CGumpMenu(uint serial, uint id, short x, short y);
    virtual ~CGumpMenu();

    static const int ID_GM_HTMLGUMP = 1000;

    CGUIText *m_TextObject{ NULL };

    void SendMenuResponse(int index);

    virtual void PrepareContent();

    virtual bool OnLeftMouseButtonDoubleClick();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
