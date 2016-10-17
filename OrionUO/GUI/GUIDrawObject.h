/***********************************************************************************
**
** GUIDrawObject.h
**
** Компонента для отображения рисуемых гампов (общая реализация).
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIDRAWOBJECT_H
#define GUIDRAWOBJECT_H
//----------------------------------------------------------------------------------
#include "BaseGUI.h"
//----------------------------------------------------------------------------------
class CGUIDrawObject : public CBaseGUI
{
public:
	CGUIDrawObject(const GUMP_OBJECT_TYPE &type, const uint &serial, const ushort &graphic, const ushort &color, const int &x, const int &y);
	virtual ~CGUIDrawObject();

	virtual WISP_GEOMETRY::CSize GetSize();

	virtual void SetShaderMode();
	virtual void PrepareTextures();

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
