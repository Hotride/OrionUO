/***********************************************************************************
**
** GUIScrollBackground.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUISCROLLBACKGROUND_H
#define GUISCROLLBACKGROUND_H
//----------------------------------------------------------------------------------
#include "BaseGUI.h"
//----------------------------------------------------------------------------------
class CGUIScrollBackground : public CBaseGUI
{
	SETGET(int, Height);
	SETGET(int, OffsetX);
	SETGET(int, Width);
	SETGET(WISP_GEOMETRY::CRect, WorkSpace);

public:
	CGUIScrollBackground(const uint &serial, const ushort &graphic, const int &x, const int &y, const int &height);
	virtual ~CGUIScrollBackground();

	void UpdateHeight(const int &height);

	virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(m_Width, m_Height); }

	virtual void PrepareTextures();

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
