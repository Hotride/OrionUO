/***********************************************************************************
**
** GUIWorldMapTexture.h
**
** Компонента для отображения текстуры карты мира
**
** Copyright (C) September 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIWORLDMAPTEXTURE_H
#define GUIWORLDMAPTEXTURE_H
//----------------------------------------------------------------------------------
#include "BaseGUI.h"
//----------------------------------------------------------------------------------
class CGUIWorldMapTexture : public CBaseGUI
{
	//!Индекс карты мира
	SETGET(int, Index);

	//!Ширина
	SETGET(int, Width);

	//!Высота
	SETGET(int, Height);

	//!Смещение по оси X
	SETGET(int, OffsetX);

	//!Смещение по оси Y
	SETGET(int, OffsetY);

public:
	CGUIWorldMapTexture(const int &x, const int &y);
	virtual ~CGUIWorldMapTexture();

	virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(m_Width, m_Height); }

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
