/***********************************************************************************
**
** GUIShader.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUISHADER_H
#define GUISHADER_H
//----------------------------------------------------------------------------------
#include "BaseGUI.h"
#include "../GLEngine/GLShader.h"
//----------------------------------------------------------------------------------
class CGUIShader : public CBaseGUI
{
	SETGET(bool, Enabled);

private:
	CGLShader *m_Shader;

public:
	CGUIShader(CGLShader *shader, const bool &enabled);
	virtual ~CGUIShader();

	virtual void Draw(const bool &checktrans = false);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
