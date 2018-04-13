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
public:
	//!Индекс карты мира
	int Index = 0;
	//!Ширина
	int Width = 0;
	//!Высота
	int Height = 0;
	//!Смещение по оси X
	int OffsetX = 0;
	//!Смещение по оси Y
	int OffsetY = 0;

	CGUIWorldMapTexture(const int &x, const int &y);
	virtual ~CGUIWorldMapTexture();

	//virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(Width, Height); }

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
