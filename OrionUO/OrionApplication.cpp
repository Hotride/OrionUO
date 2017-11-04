// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** OrionApplication.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
COrionApplication g_App;
//----------------------------------------------------------------------------------
void COrionApplication::OnIDLELoop()
{
	WISPFUN_DEBUG("c193_f1");
	g_ConnectionManager.Recv();
}
//----------------------------------------------------------------------------------
