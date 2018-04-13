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
public:
	uchar Direction = 0;
	uchar OldDirection = 0;
	uchar Sequence = 0;
	bool Accepted = false;
	bool Running = false;
	bool NoRotation = false;
	uint Timer = 0;
	ushort X = 0;
	ushort Y = 0;
	uchar Z = 0;

	CStepInfo() {}
	~CStepInfo() {}
};
//----------------------------------------------------------------------------------
//Класс для обработки шагов игрока
class CWalker
{
public:
	uint LastStepRequestTime = 0;
	int UnacceptedPacketsCount = 0;
	int StepsCount = 0;
	uchar WalkSequence = 0;
	uchar CurrentWalkSequence = 0;
	bool ResendPacketSended = false;
	bool WantChangeCoordinates = false;
	bool WalkingFailed = false;
	ushort CurrentPlayerZ = 0;
	ushort NewPlayerZ = 0;

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
