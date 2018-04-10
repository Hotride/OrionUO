// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** DateTimeStamp.cpp
**
** Copyright (C) September 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"

#if !defined(ORION_LINUX)

//----------------------------------------------------------------------------------
string GetBuildDateTimeStamp()
{
	return string(__DATE__ " " __TIME__);
}
//----------------------------------------------------------------------------------

#else

#include "GitRevision.h"
string GetBuildDateTimeStamp()
{
	return string(__DATE__ " " __TIME__ " (" GIT_REV_STR ")");
}

#endif