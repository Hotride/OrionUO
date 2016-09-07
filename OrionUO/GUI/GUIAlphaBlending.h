/***********************************************************************************
**
** GUIAlphaBlending.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIALPHABLENDING_H
#define GUIALPHABLENDING_H
//----------------------------------------------------------------------------------
#include "GUIBlending.h"
//----------------------------------------------------------------------------------
class CGUIAlphaBlending : public CGUIBlending
{
	SETGET(float, Alpha);

public:
	CGUIAlphaBlending(const bool &enabled, const float &alpha);
	virtual ~CGUIAlphaBlending();

	virtual void Draw(const bool &checktrans = false);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
