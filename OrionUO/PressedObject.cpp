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
	LeftObject = obj.LeftObject;
	LeftGump = obj.LeftGump;
	LeftSerial = obj.LeftSerial;

	RightObject = obj.RightObject;
	RightGump = obj.RightGump;
	RightSerial = obj.RightSerial;

	MidObject = obj.MidObject;
	MidGump = obj.MidGump;
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
	if (obj == LeftObject)
	{
		LeftObject = NULL;
		LeftSerial = 0;
	}

	if (obj == RightObject)
	{
		RightObject = NULL;
		RightSerial = 0;
	}

	if (obj == MidObject)
	{
		MidObject = NULL;
		MidSerial = 0;
	}
}
//----------------------------------------------------------------------------------
void CPressedObject::ClearLeft()
{
	WISPFUN_DEBUG("c198_f4");
	LeftObject = NULL;
	LeftGump = NULL;
	LeftSerial = 0;
}
//----------------------------------------------------------------------------------
void CPressedObject::InitLeft(CRenderObject *obj, CGump *gump)
{
	WISPFUN_DEBUG("c198_f5");
	LeftObject = obj;
	LeftGump = gump;
	LeftSerial = (obj != NULL ? obj->Serial : 0);
}
//----------------------------------------------------------------------------------
void CPressedObject::InitLeft(const CSelectedObject &obj)
{
	WISPFUN_DEBUG("c198_f6");
	LeftObject = obj.Object;
	LeftGump = obj.Gump;
	LeftSerial = (LeftObject != NULL ? obj.Serial : 0);
}
//----------------------------------------------------------------------------------
void CPressedObject::ClearRight()
{
	WISPFUN_DEBUG("c198_f7");
	RightObject = NULL;
	RightGump = NULL;
	RightSerial = 0;
}
//----------------------------------------------------------------------------------
void CPressedObject::InitRight(CRenderObject *obj, CGump *gump)
{
	WISPFUN_DEBUG("c198_f8");
	RightObject = obj;
	RightGump = gump;
	RightSerial = (obj != NULL ? obj->Serial : 0);
}
//----------------------------------------------------------------------------------
void CPressedObject::InitRight(const CSelectedObject &obj)
{
	WISPFUN_DEBUG("c198_f9");
	RightObject = obj.Object;
	RightGump = obj.Gump;
	RightSerial = (RightObject != NULL ? obj.Serial : 0);
}
//----------------------------------------------------------------------------------
void CPressedObject::ClearMid()
{
	WISPFUN_DEBUG("c198_f10");
	MidObject = NULL;
	MidGump = NULL;
	MidSerial = 0;
}
//----------------------------------------------------------------------------------
void CPressedObject::InitMid(CRenderObject *obj, CGump *gump)
{
	WISPFUN_DEBUG("c198_f11");
	MidObject = obj;
	MidGump = gump;
	MidSerial = (obj != NULL ? obj->Serial : 0);
}
//----------------------------------------------------------------------------------
void CPressedObject::InitMid(const CSelectedObject &obj)
{
	WISPFUN_DEBUG("c198_f12");
	MidObject = obj.Object;
	MidGump = obj.Gump;
	MidSerial = (MidObject != NULL ? obj.Serial : 0);
}
//----------------------------------------------------------------------------------
bool CPressedObject::TestMoveOnDrag()
{
	WISPFUN_DEBUG("c198_f13");
	return (LeftObject != NULL && LeftObject->IsGUI() && ((CBaseGUI*)LeftObject)->MoveOnDrag);
}
//----------------------------------------------------------------------------------
