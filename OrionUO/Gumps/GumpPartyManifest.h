/***********************************************************************************
**
** GumpPartyManifest.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPPARTYMANIFEST_H
#define GUMPPARTYMANIFEST_H
//----------------------------------------------------------------------------------
class CGumpPartyManifest : public CGump
{
private:
    static const int ID_GPM_BUTTON_OKAY = 1;
    static const int ID_GPM_BUTTON_CANCEL = 2;
    static const int ID_GPM_BUTTON_SEND_MESSAGE = 3;
    static const int ID_GPM_BUTTON_LOOT_TYPE = 4;
    static const int ID_GPM_BUTTON_LEAVE = 5;
    static const int ID_GPM_BUTTON_ADD = 6;
    static const int ID_GPM_BUTTON_TELL_MEMBER = 10;
    static const int ID_GPM_BUTTON_KICK_MEMBER = 20;

    bool m_CanLoot{ false };

public:
    CGumpPartyManifest(uint serial, short x, short y, bool canLoot);
    virtual ~CGumpPartyManifest();

    virtual void UpdateContent();

    GUMP_BUTTON_EVENT_H;
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
