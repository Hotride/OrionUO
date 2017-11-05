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
	SETGET(int, X, 0);
	SETGET(int, Y, 0);
	SETGET(int, Hits, 0);
	SETGET(ushort, Color, 0);
	SETGET(ushort, HealthColor, 0);
	SETGETP(CGameCharacter*, TargetedCharacter, NULL);

public:
	CTargetGump();
	~CTargetGump();

	void Draw();
};
//----------------------------------------------------------------------------------
class CNewTargetSystem
{
	SETGET(uint, Serial, 0);
	SETGET(int, X, 0);
	SETGET(int, TopY, 0);
	SETGET(int, BottomY, 0);
	SETGET(int, GumpX, 20);
	SETGET(int, GumpY, 20);
	SETGET(int, Hits, 0);
	SETGET(ushort, GumpTop, 0);
	SETGET(ushort, GumpBottom, 0);
	SETGET(ushort, ColorGump, 0);
	SETGET(ushort, HealthColor, 0);
	SETGETP(CGameCharacter*, TargetedCharacter, NULL);

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
