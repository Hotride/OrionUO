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
public:
	short X = 0;
	short Y = 0;
	char Z = 0;
	uchar Direction = 0;
	ushort Graphic = 0;
	uchar Flags = 0;

	CWalkData() {}
	CWalkData(short x, short y, char z, uchar direction, ushort graphic, uchar flags)
		: X(x), Y(y), Z(z), Direction(direction), Graphic(graphic), Flags(flags) {}
	~CWalkData() {}

	//Бег или хотьба
	bool Run() { return (Direction & 0x80); }

	//Получить пиксельное смещение для анимации
	void GetOffset(float &x, float &y, float &steps);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
