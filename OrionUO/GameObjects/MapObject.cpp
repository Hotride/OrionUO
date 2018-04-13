// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** MapObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CMapObject::CMapObject(const RENDER_OBJECT_TYPE &type, const uint &serial, const ushort &graphic, const ushort &color, const short &x, const short &y, const char &z)
: CRenderWorldObject(type, serial, graphic, color, x, y, z)
{
}
//----------------------------------------------------------------------------------
CMapObject::~CMapObject()
{
#if UO_DEBUG_INFO!=0
	if (RenderType == ROT_LAND_OBJECT)
		g_LandObjectsCount--;
	else if (RenderType == ROT_STATIC_OBJECT)
		g_StaticsObjectsCount--;
#endif //UO_DEBUG_INFO!=0
}
//----------------------------------------------------------------------------------
