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
	/*
	00000000 Direction2      db ?
	00000001 Direction       db ?
	00000002 Sequence        db ?
	00000003 field_3         db ?
	00000004 Accepted        dd ?
	00000008 Running         db ?
	00000009 NoRotation      db ?
	0000000A field_A         db ?
	0000000B field_B         db ?
	0000000C Timer           dd ?
	00000010 Z               db ?
	00000011 field_11        db ?
	00000012 field_12        db ?
	00000013 field_01        db ?
	*/
};
//----------------------------------------------------------------------------------
//Класс для обработки шагов игрока
class CWalker
{
private:
	uchar m_Sequence{ 0 }; //Текущий шаг

	uchar m_Direction[256]; //Направления

public:
	CWalker();
	~CWalker() {}

	CFastWalkStack m_FastWalkStack; //Указатель на Fast Walk Stack
	
	CStepInfo m_Step[MAX_STEPS_COUNT];

	void IncSequence(); //Инкремент счетчика шагов
	void SetSequence(const uchar &seq, const uchar &dir); //Установить значение Direction для указанного шага

	uchar GetSequence() const { return m_Sequence; } //Получить номер шага
	uchar GetDirection(const uchar &seq) const { return m_Direction[seq]; } //Получить значение Direction для указанного шага
};
//----------------------------------------------------------------------------------
extern CWalker *g_Walker;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
