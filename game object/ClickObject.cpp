/****************************************************************************
**
** ClickObject.cpp
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
#include "stdafx.h"

TClickObject g_ClickObject;
//---------------------------------------------------------------------------
TClickObject::TClickObject()
: m_Type(COT_NONE), m_Serial(0), m_GumpID(0), m_Timer(0), m_GumpType(GT_NONE),
m_X(0), m_Y(0), m_Z(0), m_GumpButtonID(0)
{
}
//---------------------------------------------------------------------------
/*!
Инициализация
@param [__in] type Тип объекта, на который кликнули
@return 
*/
void TClickObject::Init( __in CLICK_OBJECT_TYPE type)
{
	m_Type = type;
	m_Serial = 0;
	m_GumpID = 0;
	m_Timer = 0;
	m_GumpType = GT_NONE;
	m_X = 0;
	m_Y = 0;
	m_Z = 0;
	m_GumpButtonID = 0;
}
//---------------------------------------------------------------------------