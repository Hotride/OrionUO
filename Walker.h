/****************************************************************************
**
** Walker.h
**
** Copyright (C) September 2015 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//---------------------------------------------------------------------------
#ifndef WalkerH
#define WalkerH
//--------------------------------------------------------------------------
const int MAX_FAST_WALK_STACK_SIZE = 255;
//--------------------------------------------------------------------------
class TFastWalkStack
{
private:
	DWORD m_Keys[MAX_FAST_WALK_STACK_SIZE + 1];
	int m_Current;
	int m_Deep;

public:
	TFastWalkStack() :m_Current(0), m_Deep(0) {memset(m_Keys, 0, sizeof(m_Keys));}
	~TFastWalkStack() {}

	void Reset(); //Очистить стек

	void Push(DWORD key); //Добавить элемент в стек
	DWORD Pop(); //Получить элемент и удалить его из стека
};
//--------------------------------------------------------------------------
struct WALKER_SEND_ITEM
{
	BYTE Dir;
	WORD Time;
};
//--------------------------------------------------------------------------
class TWalker
{
private:
	BYTE m_Sequence; //Текущий шаг

	BYTE m_Direction[256]; //Направления

public:
	TWalker();
	~TWalker() {m_SendStack.clear();}

	std::deque<WALKER_SEND_ITEM> m_SendStack;

	TFastWalkStack m_FastWalkStack; //Указатель на Fast Walk Stack
	
	void IncSequence(); //Инкремент счетчика шагов
	void SetSequence(BYTE Seq, BYTE dir); //Установить значение Direction для указанного шага

	BYTE GetSequence() const {return m_Sequence;} //Получить номер шага
	BYTE GetDirection(BYTE Seq) const {return m_Direction[Seq];} //Получить значение Direction для указанного шага
};
//--------------------------------------------------------------------------
class TWalkData
{
private:
	WORD m_X;
	WORD m_Y;
	char m_Z;
	BYTE m_Direction;

public:
	TWalkData *m_Next;
	TWalkData *m_Prev;

	TWalkData() :m_X(0), m_Y(0), m_Z(0), m_Direction(0), m_Next(NULL), m_Prev(NULL) {}
	~TWalkData() {}
	
	SETGET(WORD, X);
	SETGET(WORD, Y);
	SETGET(char, Z);
	SETGET(BYTE, Direction);

	bool Run() {return (m_Direction & 0x80);}

	void GetOffset(float &x, float &y, float &steps);
};
//--------------------------------------------------------------------------
class TWalkStack
{
private:
public:
	TWalkStack() :m_Items(NULL) {}
	~TWalkStack() {Clear();}
	
	TWalkData *m_Items;
	
	TWalkData *Top();

	void Init() {m_Items = NULL;}
	void Clear();

	void Push(TWalkData *obj);
	void Pop();

	bool Empty() {return (m_Items == NULL);}
};
//--------------------------------------------------------------------------
extern TWalker *Walker;
//--------------------------------------------------------------------------
#endif