// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** PressedObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CPressedObject g_PressedObject;
//----------------------------------------------------------------------------------
CPressedObject::CPressedObject()
{
}
//----------------------------------------------------------------------------------
CPressedObject::~CPressedObject()
{
}
//----------------------------------------------------------------------------------
void CPressedObject::Init(const CPressedObject &obj)
{
	WISPFUN_DEBUG("c198_f1");
	m_LeftObject = obj.LeftObject;
	m_LeftGump = obj.LeftGump;
	LeftSerial = obj.LeftSerial;

	m_RightObject = obj.RightObject;
	m_RightGump = obj.RightGump;
	RightSerial = obj.RightSerial;

	m_MidObject = obj.MidObject;
	m_MidGump = obj.MidGump;
	MidSerial = obj.MidSerial;
}
//----------------------------------------------------------------------------------
void CPressedObject::ClearAll()
{
	WISPFUN_DEBUG("c198_f2");
	ClearLeft();
	ClearRight();
	ClearMid();
}
//----------------------------------------------------------------------------------
void CPressedObject::Clear(CRenderObject *obj)
{
	WISPFUN_DEBUG("c198_f3");
	if (obj == m_LeftObject)
	{
		m_LeftObject = NULL;
		LeftSerial = 0;
	}

	if (obj == m_RightObject)
	{
		m_RightObject = NULL;
		RightSerial = 0;
	}

	if (obj == m_MidObject)
	{
		m_MidObject = NULL;
		MidSerial = 0;
	}
}
//----------------------------------------------------------------------------------
void CPressedObject::ClearLeft()
{
	WISPFUN_DEBUG("c198_f4");
	m_LeftObject = NULL;
	m_LeftGump = NULL;
	LeftSerial = 0;
}
//----------------------------------------------------------------------------------
void CPressedObject::InitLeft(CRenderObject *obj, CGump *gump)
{
	WISPFUN_DEBUG("c198_f5");
	m_LeftObject = obj;
	m_LeftGump = gump;
	LeftSerial = (obj != NULL ? obj->Serial : 0);
}
//----------------------------------------------------------------------------------
void CPressedObject::InitLeft(const CSelectedObject &obj)
{
	WISPFUN_DEBUG("c198_f6");
	m_LeftObject = obj.Object;
	m_LeftGump = obj.Gump;
	LeftSerial = (m_LeftObject != NULL ? obj.Serial : 0);
}
//----------------------------------------------------------------------------------
void CPressedObject::ClearRight()
{
	WISPFUN_DEBUG("c198_f7");
	m_RightObject = NULL;
	m_RightGump = NULL;
	RightSerial = 0;
}
//----------------------------------------------------------------------------------
void CPressedObject::InitRight(CRenderObject *obj, CGump *gump)
{
	WISPFUN_DEBUG("c198_f8");
	m_RightObject = obj;
	m_RightGump = gump;
	RightSerial = (obj != NULL ? obj->Serial : 0);
}
//----------------------------------------------------------------------------------
void CPressedObject::InitRight(const CSelectedObject &obj)
{
	WISPFUN_DEBUG("c198_f9");
	m_RightObject = obj.Object;
	m_RightGump = obj.Gump;
	RightSerial = (m_RightObject != NULL ? obj.Serial : 0);
}
//----------------------------------------------------------------------------------
void CPressedObject::ClearMid()
{
	WISPFUN_DEBUG("c198_f10");
	m_MidObject = NULL;
	m_MidGump = NULL;
	MidSerial = 0;
}
//----------------------------------------------------------------------------------
void CPressedObject::InitMid(CRenderObject *obj, CGump *gump)
{
	WISPFUN_DEBUG("c198_f11");
	m_MidObject = obj;
	m_MidGump = gump;
	MidSerial = (obj != NULL ? obj->Serial : 0);
}
//----------------------------------------------------------------------------------
void CPressedObject::InitMid(const CSelectedObject &obj)
{
	WISPFUN_DEBUG("c198_f12");
	m_MidObject = obj.Object;
	m_MidGump = obj.Gump;
	MidSerial = (m_MidObject != NULL ? obj.Serial : 0);
}
//----------------------------------------------------------------------------------
bool CPressedObject::TestMoveOnDrag()
{
	WISPFUN_DEBUG("c198_f13");
	return (m_LeftObject != NULL && m_LeftObject->IsGUI() && ((CBaseGUI*)m_LeftObject)->MoveOnDrag);
}
//----------------------------------------------------------------------------------
