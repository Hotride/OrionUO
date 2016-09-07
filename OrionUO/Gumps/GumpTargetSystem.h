/***********************************************************************************
**
** GumpTargetSystem.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GumpTargetSystemH
#define GumpTargetSystemH
//----------------------------------------------------------------------------------
#include "Gump.h"
//----------------------------------------------------------------------------------
class CGumpTargetSystem : public CGump
{
	SETGET(string, OldName);

private:
	static const int ID_GSB_BUFF_GUMP = 1;
	static const int ID_GSB_LOCK_MOVING = 2;

	CGUIGumppic *m_Body;
	CGUIDataBox *m_DataBox;

public:
	CGumpTargetSystem(uint serial, short x, short y);
	virtual ~CGumpTargetSystem();

	virtual bool CanBeDisplayed();

	virtual void PrepareContent();
	virtual void UpdateContent();

	GUMP_BUTTON_EVENT_H;

	void OnLeftMouseDown();
	bool OnLeftMouseButtonDoubleClick();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
