/***********************************************************************************
**
** GumpBulletinBoard.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPBULLETINBOARD_H
#define GUMPBULLETINBOARD_H
//----------------------------------------------------------------------------------
//!Класс доски объявлений
class CGumpBulletinBoard : public CGump
{
private:
    //!Идентификаторы кнопок
    static const int ID_GBB_POST_MESSAGE = 1;
    static const int ID_GBB_MESSAGE = 2;
    static const int ID_GBS_HTMLGUMP = 3;

public:
    CGumpBulletinBoard(uint serial, short x, short y, string name);
    virtual ~CGumpBulletinBoard();

    CGUIHTMLGump *m_HTMLGump{ NULL };

    GUMP_BUTTON_EVENT_H;

    virtual bool OnLeftMouseButtonDoubleClick();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
