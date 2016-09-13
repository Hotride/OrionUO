/***********************************************************************************
**
** GUISkillGroup.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUISKILLGROUP_H
#define GUISKILLGROUP_H
//----------------------------------------------------------------------------------
#include "BaseGUI.h"
#include "GUIButton.h"
#include "GUITextEntry.h"
#include "../GLEngine/GLTextTexture.h"
//----------------------------------------------------------------------------------
class CGUISkillGroup : public CBaseGUI
{
	SETGETE(bool, Minimized, OnChangeMinimized);

public:
	CGUISkillGroup(const uint &serial, const uint &minimizeSerial, class CSkillGroupObject *group, const int &x, const int &y);
	virtual ~CGUISkillGroup();

	CGUIButton *m_Minimizer;
	CGUITextEntry *m_Name;

	virtual WISP_GEOMETRY::CSize GetSize();

	virtual void PrepareTextures();

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();

	CBaseGUI *SelectedItem();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
