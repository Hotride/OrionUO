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
#include "Globals.h"
//----------------------------------------------------------------------------------
class CTargetGump
{
	SETGET(int, X);
	SETGET(int, Y);
	SETGET(int, Hits);
	SETGET(ushort, Color);

public:
	CTargetGump();
	~CTargetGump();

	void Draw();
};
//----------------------------------------------------------------------------------
class CNewTargetSystem
{
	SETGET(uint, Serial);
	SETGET(int, X);
	SETGET(int, TopY);
	SETGET(int, BottomY);
	SETGET(int, GumpX);
	SETGET(int, GumpY);
	SETGET(int, Hits);
	SETGET(ushort, GumpTop);
	SETGET(ushort, GumpBottom);
	SETGET(ushort, ColorGump);

public:
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
