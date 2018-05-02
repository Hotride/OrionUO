/***********************************************************************************
**
** GumpScreenMain.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPSCREENMAIN_H
#define GUMPSCREENMAIN_H
//----------------------------------------------------------------------------------
class CGumpScreenMain : public CGump
{
private:
    //!Идентификаторы кнопок
    static const int ID_MS_QUIT = 1;
    static const int ID_MS_ARROW_NEXT = 2;
    static const int ID_MS_ACCOUNT = 3;
    static const int ID_MS_PASSWORD = 4;
    static const int ID_MS_SAVEPASSWORD = 5;
    static const int ID_MS_AUTOLOGIN = 6;

    CGUIButton *m_Arrow{ NULL };

public:
    CGumpScreenMain();
    virtual ~CGumpScreenMain();

    CEntryText *m_PasswordFake{ NULL };

    virtual void PrepareContent();

    virtual void UpdateContent();

    virtual void InitToolTip();

    GUMP_BUTTON_EVENT_H;
    GUMP_TEXT_ENTRY_EVENT_H;
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
