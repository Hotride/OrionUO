/***********************************************************************************
**
** GumpAbility.h
**
** Copyright (C) December 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPABILITY_H
#define GUMPABILITY_H
//----------------------------------------------------------------------------------
class CGumpAbility : public CGump
{
private:
	static const int ID_GS_LOCK_MOVING = 1;

	CGUIGumppic *m_Body{ NULL };
	CGUIGlobalColor *m_Colorizer{ NULL };

public:
	CGumpAbility(int serial, int x, int y);
	virtual ~CGumpAbility();

	static void OnAbilityUse(int index);

	virtual void UpdateContent();

	virtual void InitToolTip();

	GUMP_BUTTON_EVENT_H;

	virtual bool OnLeftMouseButtonDoubleClick();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
