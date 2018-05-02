/***********************************************************************************
**
** GumpGrayMenu.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPGRAYMENU_H
#define GUMPGRAYMENU_H
//----------------------------------------------------------------------------------
class CGumpGrayMenu : public CGump
{
public:
    CGumpGrayMenu(uint serial, uint id, short x, short y);
    virtual ~CGumpGrayMenu();

    static const int ID_GGM_CANCEL = 1000;
    static const int ID_GGM_CONTINUE = 1001;

    void SendMenuResponse(int index);

    GUMP_BUTTON_EVENT_H;
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
