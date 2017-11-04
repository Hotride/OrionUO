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
: m_X(x), m_Y(y), m_Z(z)
{
}
//----------------------------------------------------------------------------------
void CVector::Link(const CVector &v)
{
	m_X = v.X;
	m_Y = v.Y;
	m_Z = v.Z;
}
//----------------------------------------------------------------------------------
void CVector::Link(const double &x, const double &y, const double &z)
{
	m_X = x;
	m_Y = y;
	m_Z = z;
}
//----------------------------------------------------------------------------------
void CVector::Add(const CVector &v)
{
	m_X += v.X;
	m_Y += v.Y;
	m_Z += v.Z;
}
//----------------------------------------------------------------------------------
void CVector::Add(const double &x, const double &y, const double &z)
{
	m_X += x;
	m_Y += y;
	m_Z += z;
}
//----------------------------------------------------------------------------------
void CVector::Merge(const CVector &v)
{
	double newX = m_Y * v.Z - m_Z * v.Y;
	double newY = m_Z * v.X - m_X * v.Z;
	double newZ = m_X * v.Y - m_Y * v.X;

	m_X = newX;
	m_Y = newY;
	m_Z = newZ;
}
//----------------------------------------------------------------------------------
void CVector::Merge(const double &x, const double &y, const double &z)
{
	double newX = m_Y * z - m_Z * y;
	double newY = m_Z * x - m_X * z;
	double newZ = m_X * y - m_Y * x;

	m_X = newX;
	m_Y = newY;
	m_Z = newZ;
}
//----------------------------------------------------------------------------------
void CVector::Normalize()
{
	WISPFUN_DEBUG("c38_f1");
	if (!m_X && !m_Y && !m_Z)
			return;

	double result = m_X * m_X + m_Y * m_Y + m_Z * m_Z;

	//if (result)
	{
		result = sqrtl(result);
		
		m_X /= result;
		m_Y /= result;
		m_Z /= result;
	}
}
//----------------------------------------------------------------------------------
