/***********************************************************************************
**
** PressedObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "PressedObject.h"
//----------------------------------------------------------------------------------
CPressedObject g_PressedObject;
//----------------------------------------------------------------------------------
CPressedObject::CPressedObject()
: m_LeftObject(NULL), m_LeftGump(NULL), m_RightObject(NULL), m_RightGump(NULL),
m_MidObject(NULL), m_MidGump(NULL), m_LeftSerial(0), m_RightSerial(0), m_MidSerial(0)
{
}
//----------------------------------------------------------------------------------
CPressedObject::~CPressedObject()
{
}
//----------------------------------------------------------------------------------
void CPressedObject::Init(const CPressedObject &obj)
{
	m_LeftObject = obj.LeftObject();
	m_LeftGump = obj.LeftGump();
	m_LeftSerial = obj.LeftSerial;

	m_RightObject = obj.RightObject();
	m_RightGump = obj.RightGump();
	m_RightSerial = obj.RightSerial;

	m_MidObject = obj.MidObject();
	m_MidGump = obj.MidGump();
	m_MidSerial = obj.MidSerial;
}
//----------------------------------------------------------------------------------
void CPressedObject::ClearAll()
{
	ClearLeft();
	ClearRight();
	ClearMid();
}
//----------------------------------------------------------------------------------
void CPressedObject::Clear(CRenderObject *obj)
{
	if (obj == m_LeftObject)
	{
		m_LeftObject = NULL;
		m_LeftSerial = 0;
	}

	if (obj == m_RightObject)
	{
		m_RightObject = NULL;
		m_RightSerial = 0;
	}

	if (obj == m_MidObject)
	{
		m_MidObject = NULL;
		m_MidSerial = 0;
	}
}
//----------------------------------------------------------------------------------
void CPressedObject::ClearLeft()
{
	m_LeftObject = NULL;
	m_LeftGump = NULL;
	m_LeftSerial = 0;
}
//----------------------------------------------------------------------------------
void CPressedObject::InitLeft(CRenderObject *obj, CGump *gump)
{
	m_LeftObject = obj;
	m_LeftGump = gump;
	m_LeftSerial = (obj != NULL ? obj->Serial : 0);
}
//----------------------------------------------------------------------------------
void CPressedObject::InitLeft(const CSelectedObject &obj)
{
	m_LeftObject = obj.Object();
	m_LeftGump = obj.Gump();
	m_LeftSerial = (obj.Object() != NULL ? obj.Object()->Serial : 0);
}
//----------------------------------------------------------------------------------
void CPressedObject::ClearRight()
{
	m_RightObject = NULL;
	m_RightGump = NULL;
	m_RightSerial = 0;
}
//----------------------------------------------------------------------------------
void CPressedObject::InitRight(CRenderObject *obj, CGump *gump)
{
	m_RightObject = obj;
	m_RightGump = gump;
	m_RightSerial = (obj != NULL ? obj->Serial : 0);
}
//----------------------------------------------------------------------------------
void CPressedObject::InitRight(const CSelectedObject &obj)
{
	m_RightObject = obj.Object();
	m_RightGump = obj.Gump();
	m_RightSerial = (obj.Object() != NULL ? obj.Object()->Serial : 0);
}
//----------------------------------------------------------------------------------
void CPressedObject::ClearMid()
{
	m_MidObject = NULL;
	m_MidGump = NULL;
	m_MidSerial = 0;
}
//----------------------------------------------------------------------------------
void CPressedObject::InitMid(CRenderObject *obj, CGump *gump)
{
	m_MidObject = obj;
	m_MidGump = gump;
	m_MidSerial = (obj != NULL ? obj->Serial : 0);
}
//----------------------------------------------------------------------------------
void CPressedObject::InitMid(const CSelectedObject &obj)
{
	m_MidObject = obj.Object();
	m_MidGump = obj.Gump();
	m_MidSerial = (obj.Object() != NULL ? obj.Object()->Serial : 0);
}
//----------------------------------------------------------------------------------
bool CPressedObject::TestMoveOnDrag()
{
	return (m_LeftObject != NULL && m_LeftObject->IsGUI() && ((CBaseGUI*)m_LeftObject)->MoveOnDrag);
}
//----------------------------------------------------------------------------------
