/***********************************************************************************
**
** WalkData.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef WALKDATA_H
#define WALKDATA_H
//----------------------------------------------------------------------------------
//Класс для работы с шагом
class CWalkData
{
	SETGET(short, X, 0);
	SETGET(short, Y, 0);
	SETGET(char, Z, 0);
	SETGET(uchar, Direction, 0);

public:
	CWalkData() {}
	CWalkData(const short &x, const short &y, const char &z, const uchar &direction) : m_X(x), m_Y(y), m_Z(z), m_Direction(direction) {}
	~CWalkData() {}

	//Бег или хотьба
	bool Run() { return (m_Direction & 0x80); }

	//Получить пиксельное смещение для анимации
	void GetOffset(float &x, float &y, float &steps);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
