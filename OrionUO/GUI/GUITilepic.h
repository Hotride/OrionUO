/***********************************************************************************
**
** GUITilepic.h
**
** Компонента для отображения картинки статики
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUITILEPIC_H
#define GUITILEPIC_H
//----------------------------------------------------------------------------------
class CGUITilepic : public CGUIDrawObject
{
public:
	CGUITilepic(const ushort &graphic, const ushort &color, const int &x, const int &y);
	virtual ~CGUITilepic();

	virtual WISP_GEOMETRY::CSize GetSize();

	virtual void PrepareTextures();

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
