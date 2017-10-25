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
class CGUIWorldMapTexture : public CBaseGUI
{
	//!Индекс карты мира
	SETGET(int, Index, 0);
	//!Ширина
	SETGET(int, Width, 0);
	//!Высота
	SETGET(int, Height, 0);
	//!Смещение по оси X
	SETGET(int, OffsetX, 0);
	//!Смещение по оси Y
	SETGET(int, OffsetY, 0);

public:
	CGUIWorldMapTexture(const int &x, const int &y);
	virtual ~CGUIWorldMapTexture();

	//virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(m_Width, m_Height); }

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
