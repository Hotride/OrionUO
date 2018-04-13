/***********************************************************************************
**
** GUIResizepic.h
**
** Компонента для отображения фона
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIRESIZEPIC_H
#define GUIRESIZEPIC_H
//----------------------------------------------------------------------------------
class CGUIResizepic : public CGUIPolygonal
{
public:
	CGUIResizepic(const uint &serial, const ushort &graphic, const int &x, const int &y, const int &width, const int &height);
	virtual ~CGUIResizepic();

	virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(Width, Height); }

	virtual void PrepareTextures();

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
