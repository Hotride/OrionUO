/***********************************************************************************
**
** GUIBlending.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIBLENDING_H
#define GUIBLENDING_H
//----------------------------------------------------------------------------------
#include "BaseGUI.h"
//----------------------------------------------------------------------------------
class CGUIBlending : public CBaseGUI
{
	SETGET(GLenum, SFactor);
	SETGET(GLenum, DFactor);

public:
	CGUIBlending(const bool &enabled, const GLenum &sFactor, const GLenum &dFactor);
	virtual ~CGUIBlending();

	virtual void Draw(const bool &checktrans = false);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
