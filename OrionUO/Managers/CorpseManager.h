/***********************************************************************************
**
** CorpseManager.h
**
** Copyright (C) August 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef CORPSEMANAGER_H
#define CORPSEMANAGER_H
//----------------------------------------------------------------------------------
class CCorpse
{
public:
    uint CorpseSerial = 0;
    uint ObjectSerial = 0;
    uchar Direction = 0;
    bool Running = false;

    CCorpse() {}
    CCorpse(int corpseSerial, int objectSerial, uchar direction, bool running)
        : CorpseSerial(corpseSerial)
        , ObjectSerial(objectSerial)
        , Direction(direction)
        , Running(running)
    {
    }
    ~CCorpse() {}
};
//----------------------------------------------------------------------------------
//!Класс менеджера трупов
class CCorpseManager
{
private:
    deque<CCorpse> m_List;

public:
    CCorpseManager() {}
    ~CCorpseManager() {}

    void Add(const CCorpse &corpse);

    void Remove(int corpseSerial, int objectSerial);

    bool InList(int corpseSerial, int objectSerial);

    CGameObject *GetCorpseObject(int serial);

    void Clear() { m_List.clear(); }
};
//----------------------------------------------------------------------------------
//!Менеджер трупов
extern CCorpseManager g_CorpseManager;
//----------------------------------------------------------------------------------
#endif //CORPSEMANAGER_H
//----------------------------------------------------------------------------------
