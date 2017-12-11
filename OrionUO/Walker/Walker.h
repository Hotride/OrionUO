/***********************************************************************************
**
** Walker.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef WALKER_H
#define WALKER_H
//----------------------------------------------------------------------------------
class CStepInfo
{
	SETGET(uchar, Direction, 0);
	SETGET(uchar, OldDirection, 0);
	SETGET(uchar, Sequence, 0);
	SETGET(bool, Accepted, false);
	SETGET(bool, Running, false);
	SETGET(bool, NoRotation, false);
	SETGET(uint, Timer, 0);
	SETGET(ushort, X, 0);
	SETGET(ushort, Y, 0);
	SETGET(uchar, Z, 0);

public:
	CStepInfo() {}
	~CStepInfo() {}
};
//----------------------------------------------------------------------------------
//Класс для обработки шагов игрока
class CWalker
{
	SETGET(uint, LastStepRequestTime, 0);
	SETGET(int, UnacceptedPacketsCount, 0);
	SETGET(int, StepsCount, 0);
	SETGET(uchar, WalkSequence, 0);
	SETGET(uchar, CurrentWalkSequence, 0);
	SETGET(bool, ResendPacketSended, false);
	SETGET(bool, WantChangeCoordinates, false);
	SETGET(bool, WalkingFailed, false);
	SETGET(ushort, CurrentPlayerZ, 0);
	SETGET(ushort, NewPlayerZ, 0);

public:
	CWalker();
	~CWalker() {}

	CStepInfo m_Step[MAX_STEPS_COUNT];

	void Reset();

	void DenyWalk(const uchar &sequence, const int &x, const int &y, const char &z);

	void ConfirmWalk(const uchar &sequence);
};
//----------------------------------------------------------------------------------
extern CWalker g_Walker;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
