/***********************************************************************************
**
** GLVector.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GLVECTOR_H
#define GLVECTOR_H
//----------------------------------------------------------------------------------
//Класс вектора нормали
class CVector
{
public:
	double X = 0.0;
	double Y = 0.0;
	double Z = 0.0;

	CVector(const double &x = 0, const double &y = 0, const double &z = 0);
	~CVector() {};

	//Копирование векторов
	void Link(const CVector &v);

	//Изменение координат
	void Link(const double &x, const double &y, const double &z);

	//Складывание векторов
	void Add(const CVector &v);

	//Складывание координат
	void Add(const double &x, const double &y, const double &z);

	//Слияние векторов
	void Merge(const CVector &v);

	//Слияние координат
	void Merge(const double &x, const double &y, const double &z);

	//Нормализация
	void Normalize();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
