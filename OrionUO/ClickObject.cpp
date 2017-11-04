// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** ClickObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CClickObject g_ClickObject;
//----------------------------------------------------------------------------------
void CClickObject::Clear(CRenderObject *obj)
{
	WISPFUN_DEBUG("c185_f1");
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
	WISPFUN_DEBUG("c185_f2");
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
	WISPFUN_DEBUG("c185_f3");
	m_Enabled = true;
	m_Object = obj;
	m_Gump = gump;
	m_Timer = 0;
	m_X = 0;
	m_Y = 0;
	m_Page = 0;
}
//----------------------------------------------------------------------------------
