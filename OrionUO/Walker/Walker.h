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
#include "FastWalk.h"
//----------------------------------------------------------------------------------
class CStepInfo
{
	SETGET(uchar, Direction2, 0);
	SETGET(uchar, Direction, 0);
	SETGET(uchar, Sequence, 0);
	SETGET(bool, Accepted, false);
	SETGET(bool, Running, false);
	SETGET(bool, NoRotation, false);
	SETGET(uint, Timer, 0);
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
	SETGET(int, WalkSequence, 0);
	SETGET(int, CurrentWalkSequence, 0);
	SETGET(bool, ResendPacketSended, false);
	SETGET(bool, WantChangeCoordinates, false);
	SETGET(bool, WalkingFailed, false);
	SETGET(ushort, CurrentPlayerZ, 0);
	SETGET(ushort, NewPlayerZ, 0);

private:
	uchar m_Sequence{ 0 }; //Текущий шаг

	uchar m_Direction[256]; //Направления

public:
	CWalker();
	~CWalker() {}

	CStepInfo m_Step[MAX_STEPS_COUNT];

	void IncSequence(); //Инкремент счетчика шагов
	void SetSequence(const uchar &seq, const uchar &dir); //Установить значение Direction для указанного шага

	uchar GetSequence() const { return m_Sequence; } //Получить номер шага
	uchar GetDirection(const uchar &seq) const { return m_Direction[seq]; } //Получить значение Direction для указанного шага

	void Reset();

	void DenyWalk(const uchar &sequence, const int &x, const int &y, const char &z);

	void ConfirmWalk(const uchar &sequence);
};
//----------------------------------------------------------------------------------
extern CWalker g_Walker;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
