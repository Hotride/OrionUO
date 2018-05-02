/***********************************************************************************
**
** TargetGump.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef TARGETGUMP_H
#define TARGETGUMP_H
//----------------------------------------------------------------------------------
class CTargetGump
{
public:
    int X = 0;
    int Y = 0;
    int Hits = 0;
    ushort Color = 0;
    ushort HealthColor = 0;
    CGameCharacter *TargetedCharacter = NULL;
    CTargetGump();
    ~CTargetGump();

    void Draw();
};
//----------------------------------------------------------------------------------
class CNewTargetSystem
{
public:
    uint Serial = 0;
    int X = 0;
    int TopY = 0;
    int BottomY = 0;
    int GumpX = 20;
    int GumpY = 20;
    int Hits = 0;
    ushort GumpTop = 0;
    ushort GumpBottom = 0;
    ushort ColorGump = 0;
    ushort HealthColor = 0;
    CGameCharacter *TargetedCharacter = NULL;
    CNewTargetSystem();
    ~CNewTargetSystem();

    void Draw();
};
//----------------------------------------------------------------------------------
extern CTargetGump g_TargetGump;
extern CTargetGump g_AttackTargetGump;
extern CNewTargetSystem g_NewTargetSystem;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
