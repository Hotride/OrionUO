/***********************************************************************************
**
** GUIGumppicTiled.h
**
** Компонента для отображения повторяющихся гампов
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIGUMPPICTILED_H
#define GUIGUMPPICTILED_H
//----------------------------------------------------------------------------------
class CGUIGumppicTiled : public CGUIDrawObject
{
	//!Ширина
	int Width = 0;

	//!Высота
	int Height = 0;

public:
	CGUIGumppicTiled(const ushort &graphic, const int &x, const int &y, const int &width, const int &height);
	virtual ~CGUIGumppicTiled();

	virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(Width, Height); }

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
