/***********************************************************************************
**
** ClickObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "ClickObject.h"
//----------------------------------------------------------------------------------
CClickObject g_ClickObject;
//----------------------------------------------------------------------------------
CClickObject::CClickObject()
: m_Timer(0), m_X(0), m_Y(0), m_Enabled(false), m_Gump(NULL), m_Object(NULL), m_Page(0)
{
}
//----------------------------------------------------------------------------------
void CClickObject::Clear(CRenderObject *obj)
{
	if (obj == m_Object)
	{
		m_Object = NULL;
		m_Gump = NULL;
		m_Timer = 0;
		m_X = 0;
		m_Y = 0;
		m_Enabled = false;
		m_Page = 0;
	}
}
//----------------------------------------------------------------------------------
void CClickObject::Clear()
{
	m_Object = NULL;
	m_Gump = NULL;
	m_Timer = 0;
	//m_X = 0;
	//m_Y = 0;
	m_Enabled = false;
	m_Page = 0;
}
//----------------------------------------------------------------------------------
void CClickObject::Init(CRenderObject *obj, CGump *gump)
{
	m_Enabled = true;
	m_Object = obj;
	m_Gump = gump;
	m_Timer = 0;
	m_X = 0;
	m_Y = 0;
	m_Page = 0;
}
//----------------------------------------------------------------------------------
