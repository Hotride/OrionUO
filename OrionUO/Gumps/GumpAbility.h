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
	CGumpAbility(const uint &serial, const int &x, const int &y);
	virtual ~CGumpAbility();

	static void OnAbilityUse(const int &index);

	virtual void UpdateContent();

	virtual void InitToolTip();

	GUMP_BUTTON_EVENT_H;

	virtual bool OnLeftMouseButtonDoubleClick();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
