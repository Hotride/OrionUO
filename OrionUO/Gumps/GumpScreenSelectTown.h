/***********************************************************************************
**
** GumpScreeSelectTown.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPSCREENSELECTTOWN_H
#define GUMPSCREENSELECTTOWN_H
//----------------------------------------------------------------------------------
class CGumpScreenSelectTown : public CGump
{
private:
    //!Идентификаторы кнопок
    static const int ID_STS_QUIT = 1;
    static const int ID_STS_ARROW_PREV = 2;
    static const int ID_STS_ARROW_NEXT = 3;
    static const int ID_STS_HTML_GUMP = 4;
    static const int ID_STS_TOWN = 10;

    //!Список точек для отображения кнопок городов
    vector<WISP_GEOMETRY::CPoint2Di> m_TownButtonText;

    CGUIHTMLGump *m_HTMLGump{ NULL };
    CGUIText *m_Description{ NULL };

public:
    CGumpScreenSelectTown();
    virtual ~CGumpScreenSelectTown();

    virtual void UpdateContent();

    GUMP_BUTTON_EVENT_H;
    GUMP_TEXT_ENTRY_EVENT_H;
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
