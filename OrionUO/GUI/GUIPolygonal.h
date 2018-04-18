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
public:
	//!Ширина
	int Width = 0;

	//!Высота
	int Height = 0;

	//!Вызов события по нажатию левой кнопки мыши или по отпусканию
	bool CallOnMouseUp = false;

	CGUIPolygonal(GUMP_OBJECT_TYPE type, int x, int y, int width, int height, bool callOnMouseUp = false);
	virtual ~CGUIPolygonal();

	virtual bool Select();

	virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(Width, Height); }
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
