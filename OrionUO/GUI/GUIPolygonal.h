/***********************************************************************************
**
** GUIPolygonal.h
**
** Компонента для полигонов
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIPOLYGONAL_H
#define GUIPOLYGONAL_H
//----------------------------------------------------------------------------------
class CGUIPolygonal : public CBaseGUI
{
	//!Ширина
	SETGET(int, Width, 0);

	//!Высота
	SETGET(int, Height, 0);

	//!Вызов события по нажатию левой кнопки мыши или по отпусканию
	SETGET(bool, CallOnMouseUp, false);

public:
	CGUIPolygonal(const GUMP_OBJECT_TYPE &type, const int &x, const int &y, const int &width, const int &height, const bool &callOnMouseUp = false);
	virtual ~CGUIPolygonal();

	virtual bool Select();

	virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(m_Width, m_Height); }
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
