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
	SETGET(ushort, Graphic, 0);
	SETGET(uchar, Flags, 0);

public:
	CWalkData() {}
	CWalkData(const short &x, const short &y, const char &z, const uchar &direction, const ushort &graphic, const uchar &flags)
		: m_X(x), m_Y(y), m_Z(z), m_Direction(direction), m_Graphic(graphic), m_Flags(flags) {}
	~CWalkData() {}

	//Бег или хотьба
	bool Run() { return (m_Direction & 0x80); }

	//Получить пиксельное смещение для анимации
	void GetOffset(float &x, float &y, float &steps);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
