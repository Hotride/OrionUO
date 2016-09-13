/***********************************************************************************
**
** GUISkillItem.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUISKILLITEM_H
#define GUISKILLITEM_H
//----------------------------------------------------------------------------------
#include "BaseGUI.h"
#include "GUIButton.h"
#include "../GLEngine/GLTextTexture.h"
//----------------------------------------------------------------------------------
class CGUISkillItem : public CBaseGUI
{
	SETGET(int, Index);
	SETGETE(uchar, Status, OnChangeStatus);

private:
	CGLTextTexture m_NameText;
	CGLTextTexture m_ValueText;

	ushort GetStatusButtonGraphic();

public:
	CGUISkillItem(const uint &serial, const uint &useSerial, const uint &statusSerial, const int &index, const int &x, const int &y);
	virtual ~CGUISkillItem();

	CGUIButton *m_ButtonUse;
	CGUIButton *m_ButtonStatus;

	void CreateValueText(const bool &showReal = false, const bool &showCap = false);

	virtual WISP_GEOMETRY::CSize GetSize() { return WISP_GEOMETRY::CSize(255, 17); }

	virtual void PrepareTextures();

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();

	CBaseGUI *SelectedItem();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
