/***********************************************************************************
**
** GUIWorldMapTexture.h
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
	SETGET(int, Index);
	SETGET(int, Width);
	SETGET(int, Height);
	SETGET(int, OffsetX);
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
