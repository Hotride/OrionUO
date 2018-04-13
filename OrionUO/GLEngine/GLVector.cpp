// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GLVector.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CVector::CVector(const double &x, const double &y, const double &z)
: X(x), Y(y), Z(z)
{
}
//----------------------------------------------------------------------------------
void CVector::Link(const CVector &v)
{
	X = v.X;
	Y = v.Y;
	Z = v.Z;
}
//----------------------------------------------------------------------------------
void CVector::Link(const double &x, const double &y, const double &z)
{
	X = x;
	Y = y;
	Z = z;
}
//----------------------------------------------------------------------------------
void CVector::Add(const CVector &v)
{
	X += v.X;
	Y += v.Y;
	Z += v.Z;
}
//----------------------------------------------------------------------------------
void CVector::Add(const double &x, const double &y, const double &z)
{
	X += x;
	Y += y;
	Z += z;
}
//----------------------------------------------------------------------------------
void CVector::Merge(const CVector &v)
{
	double newX = Y * v.Z - Z * v.Y;
	double newY = Z * v.X - X * v.Z;
	double newZ = X * v.Y - Y * v.X;

	X = newX;
	Y = newY;
	Z = newZ;
}
//----------------------------------------------------------------------------------
void CVector::Merge(const double &x, const double &y, const double &z)
{
	double newX = Y * z - Z * y;
	double newY = Z * x - X * z;
	double newZ = X * y - Y * x;

	X = newX;
	Y = newY;
	Z = newZ;
}
//----------------------------------------------------------------------------------
void CVector::Normalize()
{
	WISPFUN_DEBUG("c38_f1");
	if (!X && !Y && !Z)
			return;

	double result = X * X + Y * Y + Z * Z;

	//if (result)
	{
		result = sqrtl(result);
		
		X /= result;
		Y /= result;
		Z /= result;
	}
}
//----------------------------------------------------------------------------------
