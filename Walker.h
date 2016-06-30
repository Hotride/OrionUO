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
//--------------------------------------------------------------------------
#ifndef WalkerH
#define WalkerH
//--------------------------------------------------------------------------
//Класс для обработки стека fastwalk
class TFastWalkStack
{
private:
	//fastwalk стек
	DWORD m_Keys[MAX_FAST_WALK_STACK_SIZE + 1];

	//Текущий индекс
	int m_Current;

	//Глубина стека
	int m_Deep;

public:
	TFastWalkStack() :m_Current(0), m_Deep(0) {memset(m_Keys, 0, sizeof(m_Keys));}
	~TFastWalkStack() {}

	void Reset(); //Очистить стек

	void Push(DWORD key); //Добавить элемент в стек
	DWORD Pop(); //Получить элемент и удалить его из стека
};
//--------------------------------------------------------------------------
//Класс для обработки шагов игрока
class TWalker
{
private:
	BYTE m_Sequence; //Текущий шаг

	BYTE m_Direction[256]; //Направления

public:
	TWalker();
	~TWalker() {}

	TFastWalkStack m_FastWalkStack; //Указатель на Fast Walk Stack
	
	void IncSequence(); //Инкремент счетчика шагов
	void SetSequence(BYTE Seq, BYTE dir); //Установить значение Direction для указанного шага

	BYTE GetSequence() const {return m_Sequence;} //Получить номер шага
	BYTE GetDirection(BYTE Seq) const {return m_Direction[Seq];} //Получить значение Direction для указанного шага
};
//--------------------------------------------------------------------------
//Класс для работы с шагом
class TWalkData
{
private:
	//Координаты, куда ведет шаг
	WORD m_X;
	WORD m_Y;
	char m_Z;

	//Направление
	BYTE m_Direction;

public:
	//Ссылки на следующий и предыдущий элементы
	TWalkData *m_Next;
	TWalkData *m_Prev;

	TWalkData() :m_X(0), m_Y(0), m_Z(0), m_Direction(0), m_Next(NULL), m_Prev(NULL) {}
	~TWalkData() {}
	
	SETGET(WORD, X);
	SETGET(WORD, Y);
	SETGET(char, Z);
	SETGET(BYTE, Direction);

	//Бег или хотьба
	bool Run() {return (m_Direction & 0x80);}

	//Получить пиксельное смещение для анимации
	void GetOffset(float &x, float &y, float &steps);
};
//--------------------------------------------------------------------------
//Класс для хранения шагов
class TWalkStack
{
private:
public:
	TWalkStack() :m_Items(NULL) {}
	~TWalkStack() {Clear();}

	//Список шагов
	TWalkData *m_Items;

	//Верхний шаг
	TWalkData *Top();

	void Init() {m_Items = NULL;}
	void Clear();

	//Добавить шаг в стек
	void Push(TWalkData *obj);

	//Удалить шаг из стека
	void Pop();

	//Есть ли в стеке шаги
	bool Empty() {return (m_Items == NULL);}
};
//--------------------------------------------------------------------------
extern TWalker *Walker;
//--------------------------------------------------------------------------
#endif