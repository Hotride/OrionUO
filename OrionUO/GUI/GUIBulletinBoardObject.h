/***********************************************************************************
**
** GUIBulletinBoardObject.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIBULLETINBOARDOBJECT_H
#define GUIBULLETINBOARDOBJECT_H
//----------------------------------------------------------------------------------
#include "BaseGUI.h"
#include "../GLEngine/GLTextTexture.h"
//----------------------------------------------------------------------------------
class CGUIBulletinBoardObject : public CBaseGUI
{
	SETGET(string, Text, "");

private:
	CGLTextTexture m_Texture;

public:
	CGUIBulletinBoardObject(const uint &serial, const int &x, const int &y, const string &text);
	virtual ~CGUIBulletinBoardObject();

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
